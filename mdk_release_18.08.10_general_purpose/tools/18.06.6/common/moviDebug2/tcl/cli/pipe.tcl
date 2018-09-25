# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/pipe.tcl
# Description: Pipe command for moviDebug2 CLI
# Created on : Aug 15, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Pipe {
        namespace path ::mdbg

        mdbg::pipe::PipeManager create Manager mdbg::jtag

        mdbg::cli::MultiCmdArgParser create ArgumentParser -auto -brief "Debug pipe management." -description {

            Manipulate debug pipes.

            The pipe structure is a circular buffer in the application like the following:

                #define CON_Q_ELEMENTS (8) /* or any size */
                #define CON_Q_SIZE     (CON_Q_ELEMENTS + 1)

                typedef struct {
                    volatile u32  canaryStart;   // Used to detect corruption of queue
                    volatile int  in;            // write index
                    volatile int  out;           // read index
                    volatile int  queueSize;
                    volatile u32  canaryEnd;     // Used to detect corruption of queue
                    volatile u8   buffer[CON_Q_SIZE];
                } tyMvConsoleQueue;

                tyMvConsoleQueue mvConsoleTxQueue =
                {
                    .canaryStart = 0x11223344,
                    .in          = 0,
                    .out         = 0,
                    .queueSize   = CON_Q_SIZE,
                    .canaryEnd   = 0xAABBCCDD,
                };


            At most two of these queue structures in the target's memory can be associated with\
            a DEBUG PIPE object, one READ QUEUE and one WRITE QUEUE.

            Each DEBUG PIPE has it's own `pipeName` which identifies the object.

            The queue structures need not be adjacent, their addresses can be associated individually,\
            and also by symbolic name. The DEBUG PIPE object need not be always associated with a queue.

            The memory location occupied by these unidirectional queues will be monitored and can be used\
            to achieve bi-directional comunication between the debugger and the target.

        } pipeCommand  {
            create {
                -brief "Create pipe."
                -args {
                    pipeName {-brief "Name of pipe. Must not already exist"}
                }

            }
            configure {
                -brief "Configure pipe."
            }
            configure - puts - send - peek - recv - flush - delete - status - info {
                -args {
                    pipeName {-brief "Name of pipe. Must already exist."}
                }
            }
            create - configure {
                -args {
                    -stdout {-flag -brief "Redirect to stdout."}
                    -stderr {-flag -brief "Redirect tot stderr."}
                    -readaddr {-var readAddr -type integer -optional -brief "Set read queue address."}
                    -readsym {-var readSymName -optional -brief "Set read queue address from symbol name."}
                    -writeaddr {-var writeAddr -type integer -optional -brief "Set write queue address."}
                    -writesym {-var writeSymName -optional -brief "Set write queue address from symbol name."  }
                    -in {-var inFile -optional -brief "Redirect from input file."}
                    -out {-var outFile -optional -brief "Redirect to output file."}
                    -append {-flag -require -out -brief "Open output file in append mode."}
                    -prefix {-flag -require {-dest -out -channel} -brief "Set prefix for each printed line." }
                    -target {-optional -brief "Use `target` for memory I/O"}
                    -source {-var sourceChannel -optional -brief "Redirect from source Tcl channel."}
                    -dest {-var destChannel -optional -brief "Redirect to dest Tcl channel"}
                    -channel {-var twoWayChannel -optional -brief "Redirect to+from channel."}
                    -tcp {-var tcpPort -type integer -optional -brief "Open TCP port and redirect bidirectionally."}
                }
                -mutex {
                    {-stderr -stdout -out -dest -channel -tcp}
                    {-prefix {-stderr -stdout}}
                    {-append {-dest -channel -tcp}}
                    {-source -in {-channel -tcp}}
                    {-readaddr -readsym}
                    {-writeaddr -writesym}
                }
                -arg-complete-scripts {
                    {-in -out} {
                        mdbg::shell::autocomplete::listOfFiles $PREFIX*
                    }
                    -target {
                        mdbg::tcf::getValidTargets $PREFIX*
                    }
                }
            }
            list {
                -brief "List all pipes."
                -args {
                }
            }
            puts {
                -brief "Put string to pipe."
                -args {
                    -nonewline {-flag}
                    string {}
                }
            }
            send {
                -brief "Send binary data to pipe."
                -args {
                    -format {-var binFmt -optional}
                    data {}
                }
            }
            peek {
                -brief "Peek available bytes in pipe."
            }
            recv {
                -brief "Receive data from pipe."
            }

            peek - recv {
                -args {
                    -scan {-var binFmt -optional}
                    -line {-flag -var scanLine}
                    maxCount {-type integer -default 1024}
                }
                -mutex {
                    {-scan -line}
                }
            }
            flush {
                -brief "Flush pipe contents"
                -description {
                    Reads data from pipe until there's nothing more to read and\
                    transfers it according to the pipe's read configuration.

                    *** NOTES

                    - Tcl Event processing occurs during call.
                    - May never stop if the Myriad application is continuously producing output.
                    - Control-C can interrupt this in interactive mode.
                }
            }

            delete {
                -brief "Delete pipe."
            }
            reset {
                -brief "Reset all pipes."
            }
            interval {
                -brief "Set global pipe update intervals (ms)."
                -args {
                    -busy {-optional -var busyInterval -type integer}
                    -idle {-optional -var idleInterval -type integer}
                }
                -requires {
                    {-busy -idle}
                }
            }
            status {
                -brief "Pipe status."
                -description {
                    !!! NOT IMPLEMENTED
                }
            }
            info {
                -brief "Pipe information."
                -description {
                    !!! NOT IMPLEMENTED
                }
                -args {
                    args {
                        -choice {
                            -stdout -stderr -readaddr
                            -readsym -writeaddr -writesym
                            -in -out -append -prefix
                            -target -source -dest -channel
                            -tcp
                        }
                    }
                }
            }
        } {
            -arg-complete-scripts {
               pipeName {
                   if {$COMMAND == "create"} {
                       list $PREFIX
                   } else {
                       mdbg::cli::Pipe::Manager findNames $PREFIX*
                   }
               }
               {-readsym -writesym} {
                   cli::Symbols::getElfSymbols name [targetid] $PREFIX*
               }
            }
        }

        proc SymAddr {symName target} {
            mdbg::sym addr -target $target $symName
        }

        # PipeName -> [server ?client address port?]
        set TcpPipes [dict create]

        proc TcpServerAccept {pipeName newClient newAddress newPort} {

            variable Manager
            variable TcpPipes

            if {![dict exists $TcpPipes $pipeName]} {
                cli::Warning "PIPE:$pipeName should not be a TCP server! Refusing request from $newAddress:$newPort."
                close $newClient
                return
            }
            lassign [dict get $TcpPipes $pipeName] server prevClient prevAddress prevPort
            if {$prevClient != {}} {
                if {![eof $prevClient]} {
                    cli::PutWarning "PIPE:$pipeName - Refusing $newAddress:$newPort (already connected to $prevAddress:$prevPort)"
                    close $newClient
                    return
                }
                close $prevClient
            }

            cli::PutInfo "PIPE:$pipeName - TCP Connection from $newAddress:$newPort"

            cli::Pipe::Manager setPrivateChannel $pipeName output {}
            cli::Pipe::Manager setPrivateChannel $pipeName input {}

            Manager toChan $pipeName $newClient
            Manager fromChan $pipeName $newClient

            dict set TcpPipes $pipeName [list $server $newClient $newAddress $newPort]

            chan configure $newClient -blocking false -buffering none -translation binary
        }

        proc connectTcp {pipeName port} {
            variable TcpPipes

            if {[dict exists $TcpPipes $pipeName]} {
                error "Pipe is already connected to TCP"
            }
            set server [socket -server \
                [list [namespace current]::TcpServerAccept $pipeName] \
                $port
            ]
            dict set TcpPipes $pipeName $server
            return
        }

        proc disconnectTcp {pipeName} {
            variable TcpPipes
            variable Manager
            if {[dict exists $TcpPipes $pipeName]} {
                Manager toNone $pipeName
                Manager fromNone $pipeName
                lassign [dict get $TcpPipes $pipeName] server client
                dict unset TcpPipes $pipeName
                cli::CatchAndPrintError {
                    close $server
                } "Pipe disconnect TCP"
                if {$client != {}} {
                    cli::CatchAndPrintError {
                        close $client
                    } "Pipe disconnect TCP"
                }
            }
            return
        }

        dll::atexit {
            if {[catch {Manager FlushReadAtExit}]} {
                puts stderr ::$errorInfo
            }
        }

    } ; # namespace eval cli::Pipe

    proc pipe {args} {

        cli::NoErrorStack {

            cli::Pipe::ArgumentParser parseCallerArgs 1 $args

            switch -exact $pipeCommand {
                create {
                    set pipe [cli::Pipe::Manager create $pipeName]
                }
                default {
                    if {[info exists pipeName]} {
                        set pipe [cli::Pipe::Manager get $pipeName]
                    }
                }

            }

            switch -exact $pipeCommand {
                create - configure {
                    if {[info exists twoWayChannel]} {
                        set destChannel $twoWayChannel
                        set sourceChannel $twoWayChannel
                    }

                    if {$stdout} {
                        set destChannel $::mdbg::Channels::pipeout
                        set prefix 1
                    } elseif {$stderr} {
                        set destChannel $::mdbg::Channels::pipeerr
                        set prefix 1
                    }

                    set toChan toChan
                    if {$prefix} {
                        set toChan toConsole
                    }

                    if {[info exists destChannel]} {
                        cli::Pipe::disconnectTcp $pipeName

                        if {$destChannel ni {"" "-"}} {
                            cli::Pipe::Manager $toChan $pipeName $destChannel
                        } else {
                            cli::Pipe::Manager toNone $pipeName
                        }
                        cli::Pipe::Manager setPrivateChannel $pipeName output {}
                    }

                    if {[info exists sourceChannel]} {
                        cli::Pipe::disconnectTcp $pipeName

                        if {$sourceChannel ni {"" "-"}} {
                            cli::Pipe::Manager fromChan $pipeName $sourceChannel
                        } else {
                            cli::Pipe::Manager fromNone $pipeName
                        }
                        cli::Pipe::Manager setPrivateChannel $pipeName input {}
                    }

                    if {[info exists outFile]} {
                        cli::Pipe::disconnectTcp $pipeName

                        if {$outFile ni {"" "-"}} {
                            mdbg::cli::CatchAndPrintError {
                                set mode [expr {$append ? "ab" : "wb"}]
                                set outf [open $outFile $mode]
                                fconfigure $outf -buffering none
                                cli::Pipe::Manager $toChan $pipeName $outf
                                cli::Pipe::Manager setPrivateChannel $pipeName output $outf
                            } "pipe $pipeCommand $pipeName -out \"$outFile\""
                        } else {
                            cli::Pipe::Manager toNone $pipeName
                            cli::Pipe::Manager setPrivateChannel $pipeName output {}
                        }
                    }

                    if {[info exists inFile]} {
                        cli::Pipe::disconnectTcp $pipeName

                        if {$inFile ni {"" "-"}} {
                            mdbg::cli::CatchAndPrintError {
                                if {![file exists $inFile]} {
                                    error "\"$inFile\" does to exist!"
                                }
                                set inf [open $inFile "rb"]
                                cli::Pipe::Manager fromChan $pipeName $inf
                                cli::Pipe::Manager setPrivateChannel $pipeName input $inf
                            } "pipe $pipeCommand $pipeName -in \"$inFile\""
                        } else {
                            cli::Pipe::Manager fromNone $pipeName
                            cli::Pipe::Manager setPrivateChannel $pipeName input {}
                        }
                    }

                    if {![info exists target]} {
                        set target {}
                    }

                    if {[info exists readSymName]} {
                        cli::CatchAndPrintError {
                            set readAddr [cli::Pipe::SymAddr $readSymName $target]
                        } "pipe $pipeCommand $pipeName -readsym \"$readSymName\""
                    }

                    if {[info exists writeSymName]} {
                        cli::CatchAndPrintError {
                                set writeAddr [cli::Pipe::SymAddr $writeSymName $target]
                        } "pipe $pipeCommand $pipeName -writesym \"$writeSymName\""
                    }

                    if {[info exists readAddr]} {
                        cli::Pipe::Manager setReadAddr $pipeName $readAddr
                    }

                    if {[info exists writeAddr]} {
                        cli::Pipe::Manager setWriteAddr $pipeName $writeAddr
                    }

                    if {[info exists tcpPort]} {
                        cli::Pipe::disconnectTcp $pipeName
                        cli::Pipe::connectTcp $pipeName $tcpPort
                    }
                }
                list {
                    cli::Pipe::Manager getNames
                }
                puts {
                    if {!$nonewline} {
                        append string \n
                    }
                    $pipe send [binary format a* "$string"]
                    return
                }
                send {
                    if {[info exists binFmt]} {
                        set data [binary format $binFmt $data]
                    }
                    $pipe send $data
                    return
                }
                peek {
                    if {[info exists scanLine]} {
                        set data [$pipe peekLine $maxCount]
                    } else {
                        set data [$pipe peek $maxCount]
                        if {[info exists binFmt]} {
                            binary scan $data $binFmt data
                        }
                    }
                    return $data
                }
                recv {
                    if {[info exists scanLine]} {
                        set data [$pipe recvLine $maxCount]
                    } else {
                        if {[info exists binFmt]} {
                            set data [$pipe peek $maxCount]
                            set scanLength [binary scan $data $binFmt data]
                            $pipe drop $scanLength
                        } else {
                            set data [$pipe recv $maxCount]
                        }
                    }
                    return $data
                }
                flush {
                    cli::TrapInterrupt {
                        while {[cli::Pipe::Manager Read $pipeName] > 0} {
                            cli::CheckInterrupt
                        }
                    }
                }
                info -
                status -
                delete {
                    cli::Pipe::Manager delete $pipeName
                    cli::Pipe::disconnectTcp $pipeName
                }
                reset {
                    foreach pipeName [dict keys $::mdbg::cli::Pipe::TcpPipes] {
                        cli::Pipe::disconnectTcp $pipeName
                    }
                    cli::Pipe::Manager reset
                }
                interval {
                    if {[info exists busyInterval]} {
                        cli::Pipe::Manager setBusyInterval $busyInterval
                    }
                    if {[info exists idleInterval]} {
                        cli::Pipe::Manager setIdleInterval $idleInterval
                    }
                    cli::Pipe::Manager getIntervals
                }
                default {
                    return -code error "\"$pipeCommand\" not implemented"
                }
            } ; # switch
        } ; # cli::noErrorStack
    } ; # proc pipe

    shell::autocomplete::addCommand pipe mdbg::cli::Pipe::ArgumentParser autocomplete
    cli::Help::Manager add pipe -parser mdbg::cli::Pipe::ArgumentParser

    namespace export pipe

} ; # namespace eval mdbg

