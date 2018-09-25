# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/uart.tcl
# Description: MoviDebug2 `uart` command
# Created on : Oct 31, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Uart {

        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -brief "UART management." -description {
            Control over UART communication.

            *** NOTES

            - polling is ON by default.
            - polling is implemented entirely in Tcl
            - polling is done differently for moviDebugServer and moviSim
            - register setup happens during moviDebug2 startup and during board reset\
              IFF the polling is configured ON beforehand.
            - If your application does not use UART for printing, you will need to disable the UART register setup.
              1. from the moviDebug2 command line: using the `--no-uart` option.
              2. issuing the `uart off` and `breset` commands in the CLI

            *** EXAMPLES

            Enable UART polling:
            % uart on

            Disable UART polling:
            % uart off

            Get UART polling status:
            % uart status

            Flush collected UART data (adding a newline forcefully):
            % uart flush

            Listen on port 12345 and send UART output to clients:
            % uart tcp -port 12345

            Same as before, but also wait for the first client to connect before continuing.\
            Raise an error if noone connects in 60 seconds:
            % uart tcp -port 12345 -wait -timeout [expr 60 * 1000]

            !!! Type "help uart *" for subcommand list.
            !!! Type "help uart" + subcommand for more details.

        } -auto uartCmd {
            on {
                -brief "Enable UART."
            }

            off {
                -brief "Disable UART."
            }

            handler {
                -args {
                    args {
                        -var handlerArgs
                        -complete {cli::Uart::HandlerArgParser autocomplete}
                    }
                }
            }

            tcp {
                -brief "Provide UART output on TCP."
                -args {
                    -port {
                        -brief "Specify port."
                        -optional
                        -type integer
                    }
                    -off {
                        -brief "Turn off server."
                        -flag
                    }
                    -wait {
                        -brief "Wait for first client."
                        -flag
                    }
                    -timeout {
                        -brief "Wait timeout (ms)."
                        -type integer
                        -default {}
                        -requires {-wait}
                    }
                }
                -mutex {
                    {-port -off}
                }
                -requires {
                    {-port -off}
                }
            }
            status {
                -brief "Get UART status (on/off)."
            }
            flush {
                -brief "Flush UART data."
            }
            silent {
                -brief "Control display of UART messages"
                -args {
                    state {
                        -brief "Message suppression state."
                        -description {
                            If missing, current state will be returned.
                        }
                        -type boolean
                        -optional
                    }
                }
            }
            interval {
                -brief "Control uart polling interval"
                -args {
                    -idle {
                        -optional
                        -type integer
                    }
                    -busy {
                        -optional
                        -type integer
                    }
                }
            }
            prefix {
                -brief "Get or set the UART message prefix"
                -description {
                    By default the UART messages are prefixed by the string "UART: ".
                    This subcommand allows changing that prefix to an arbitrary string.

                    When called without arguments, it will return the current value of the prefix.

                    *** NOTES

                    - No separators are added to the prefix string so they must be added\
                    to the argument string when needed.
                    - Enclose arguments containing spaces in braces or double quotes.
                    - Alternatively spaces can be escaped (using the backslash character).
                }
                -args {
                    prefix {
                        -optional
                    }
                }
            }
        }

        cli::MultiCmdArgParser create HandlerArgParser -brief "Add/remove UART handler." -description {

            Set or cancel UART data handler callback scripts.

            NOTE: Handler callbacks will receive terminating newline as well.

            *** EXAMPLES

            Process UART output using custom command:
            % proc CustomPrint {prefix line} { puts -nonewline stderr "$prefix $line" }
            % uart handler add CustomPrint "myUART:"

            Process UART output using `apply` and lambda expression:
            % uart handler add apply {line { puts "<< [string range $line 0 end-1] >>" } ::}

            Append UART output lines (without last newline) to the global list "UartOutput":
            % uart handler add { lappend ::UartOutput [string range %0 0 end-1] }

            Append UART output lines to the global string "UartOutput":
            % uart handler add append UartOutput

            Save UART output to some file:
            % set FileHandle [open "UartOutput.txt" w]
            % uart handler add puts -nonewline $FileHandle

            Remove added handler:
            % set handlerID [uart handler add MyCustomHandlerProc]
            % # ...
            % uart handler cancel $handlerID

            Remove all added handlers:
            % uart handler reset

            Get the quoted UART prefix:
            % format {"%s"} [uart prefix]

        } -auto handlerCmd {
            add {
                -brief "Add handler command."
                -description {
                    Returns ID.

                    !!! Type "help uart handler" for examples
                }
                -args {
                    command {
                        -brief "Handler command."
                    }
                    args {
                        -brief "Extra arguments to command before data argument."
                        -var handlerCommandArgs
                    }
                }
            }
            cancel {
                -brief "Cancel handler."
                -description {
                    !!! Type "help uart handler" for examples.
                }
                -args {
                    id {
                        -brief "Handler ID."
                        -var handlerId
                        -complete-script {
                            mdbg::cli::Uart::listNonTcpHandlers $PREFIX*
                        }
                    }
                }
                -mutex
            }
            reset {
                -brief "Clear all UART handlers."
            }
        }

        namespace eval tcp {
            namespace path ::mdbg

            variable server {}
            variable clients {}
            variable clientCount 0

            proc getPort {} {
                variable server
                if {$server == {}} {
                    return
                }
                lindex [chan configure $server -sockname] 2
            }

            proc setPort {port} {
                variable server
                if {$server != {}} {
                    if {[getPort] == $port} {
                        return
                    }
                    turnOff
                }
                set server [socket -server [list [namespace current]::serverAccept] $port]
                return
            }

            proc turnOff {} {
                variable server
                variable clients
                variable clientCount
                if {$server != {}} {
                    cli::CatchAndPrintError {
                        close $server
                    } "UART/TCP: Close server"
                    set server {}
                }
                if {$clients != {}} {
                    dict for {client handlerId} $clients {
                        uart::cancelHandler $handlerId
                        cli::CatchAndPrintError {
                            close $client
                        } "UART/TCP: Close client"
                    }
                }
                set clientCount 0
            }

            proc serverAccept {client address port} {
                variable clients
                variable clientCount
                cli::Verbose "UART/TCP: Connection from $address:$port"
                chan configure $client -blocking false -buffering line
                chan event $client readable [list [namespace current]::clientRead $client]
                set handlerId [mdbg::uart::addHandler clientHandler $client]
                dict set clients $client $handlerId
                incr clientCount
            }

            proc disconnectClient {client} {
                variable clients
                variable clientCount
                catch {
                    lassign [chan configure $client -peername] address name port
                    set peer ${name}:${port}
                    if {$name != $address} {
                        append peer " (${address})"
                    }
                    cli::Verbose "UART/TCP: Disconnect $peer"
                }

                cli::CatchAndPrintError {
                    close $client
                } "UART/TCP: Disconnect client"

                if {[dict exists $clients $client]} {
                    set handlerId [dict get $clients $client]
                    dict unset clients $client
                    incr clientCount -1
                    uart::cancelHandler $handlerId
                }
            }

            proc clientRead {client} {
                if {[chan eof $client]} {
                    cli::Verbose "UART/TCP: Socket EOF."
                    disconnectClient $client
                    return
                }
                set code [catch {
                    chan read $client 1024
                } result]
                if {$code != 0} {
                    cli::Verbose "UART/TCP: Read error: $result"
                    disconnectClient $client
                }
            }

            proc clientHandler {client data} {
                set code [catch {
                    chan puts -nonewline $client $data
                } result]
                if {$code != 0} {
                    cli::Verbose "UART/TCP: Write error: $result"
                    disconnectClient $client
                }
            }

            proc getClientHandlerIDs {} {
                variable clients
                dict values $clients
            }

            proc waitClient {{timeout {}}} {
                cli::Verbose "UART/TCP: Wait for client"
                variable server
                variable clientCount
                if {$server == {}} {
                    return -code error "UART/TCP: Server is not started"
                }
                if {!$clientCount} {
                    try {
                        if {$timeout != {}} {
                            set id [after $timeout "set [namespace current]::clientCount \$[namespace current]::clientCount"]
                        }
                        cli::TrapInterrupt {
                            vwait [namespace current]::clientCount
                            cli::CheckInterrupt
                        }
                    } finally {
                        if {[info exists id]} {
                            after cancel $id
                        }
                    }
                    if {!$clientCount} {
                        return -code error "Timeout"
                    }
                }
            }

        } ; # namespace eval tcp

        proc listNonTcpHandlers {{pattern *}} {
            # Lambda to keep only items matching pattern
            set Filter {{list pattern} {
                lsearch -all -inline -glob $list $pattern
            }}
            set allHandlers [apply $Filter [uart::listHandlers] $pattern]
            set tcpHandlers [apply $Filter [tcp::getClientHandlerIDs] $pattern]

            # Filter out tcp handlers
            set handlers [list]
            foreach handlerId $allHandlers  {
                if {$handlerId ni $tcpHandlers} {
                    lappend handlers $handlerId
                }
            }
            return $handlers
        }

    } ; # namespace eval cli::Uart

    proc uart {args} {

        cli::Uart::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {

            switch -exact $uartCmd {
                on {
                    uart::init
                    uart::start
                    set ::mdbg::UART on
                    cli::Verbose "UART is on."
                }
                off {
                    uart::stop
                    set ::mdbg::UART off
                    cli::Verbose "UART is off."
                }
                tcp {
                    if {[info exists port]} {
                        cli::Uart::tcp::setPort $port
                    }
                    if {$off} {
                        cli::Uart::tcp::turnOff
                    }
                    if {$wait} {
                        cli::Uart::tcp::waitClient $timeout
                    }
                }
                handler {
                    cli::Uart::HandlerArgParser parseCallerArgs 2 $handlerArgs
                    switch -exact $handlerCmd {
                        add {
                            uart::AddScopedHandler $command $handlerCommandArgs [uplevel 1 namespace current]
                        }
                        cancel {
                            uart::cancelHandler $handlerId
                        }
                        reset {
                            foreach handlerId [cli::Uart::listNonTcpHandlers] {
                                uart::cancelHandler $handlerId
                            }
                        }
                        default {
                            cli::NotImplemented 2
                        }
                    }
                }
                status {
                    expr {$::mdbg::uart::STOPPED ? "off" : "on"}
                }
                flush {
                    uart::flushAll
                }
                silent {
                    if {[info exists state]} {
                        set ::mdbg::uart::SILENT [expr $state ? 1 : 0]
                    }
                    set ::mdbg::uart::SILENT
                }
                interval {
                    set restart 0
                    if {[info exists busy]} {
                        set ::mdbg::uart::BUSY_INTERVAL $busy
                        set restart 1
                    }
                    if {[info exists idle]} {
                        set ::mdbg::uart::IDLE_INTERVAL $idle
                        set restart 1
                    }
                    if {$restart} {
                        ::mdbg::uart::updateNow
                    }
                    list -busy $::mdbg::uart::BUSY_INTERVAL -idle $::mdbg::uart::IDLE_INTERVAL
                }
                prefix {
                    if {[info exists prefix]} {
                        set ::mdbg::uart::PREFIX $prefix
                    } else {
                        set ::mdbg::uart::PREFIX
                    }
                }
                default {
                    cli::NotImplemented 1
                }
            }
        }
    }

    shell::autocomplete::addCommand uart cli::Uart::ArgumentParser autocomplete
    cli::Help::Manager add uart -parser cli::Uart::ArgumentParser
    cli::Help::Manager add {uart handler} -parser cli::Uart::HandlerArgParser

    namespace export uart

} ; # namespace eval mdbg
