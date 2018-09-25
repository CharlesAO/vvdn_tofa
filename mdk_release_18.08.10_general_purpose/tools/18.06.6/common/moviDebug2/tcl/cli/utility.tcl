# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : utility.tcl
# Description:
# Created on : Feb 14, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    proc prettyTclResult {cmd args} {
        set result [uplevel 1 [list $cmd {*}$args]]
        prettyTclObject $result
    }

    proc repeat {count body args} {
        cli::TrapInterrupt {
            for {set i 0} {$i < $count} {incr i} {
                uplevel 1 $body $args
                cli::CheckInterrupt
            }
        }
    }

    proc fallback {args} {
        cli::Fallback::ArgumentParser parseCallerArgs 1 $args
        set tries [list $script {*}$fallback]
        set count [llength $tries]
        foreach body $tries {
            incr count -1
            if {$debug} {
                cli::PutDebug $body
            }
            set code [catch {uplevel 1 $body} result options]
            # alles ok
            if {$code == 0} {
                return $result
            }
            # error
            if {$code == 1 && $count} {
                if {!$silent} {
                    if {!$info} {
                        cli::PutError $result
                    } else {
                        cli::PutError [dict get $options -errorinfo]
                    }
                }
                continue
            }
            dict with options {
                incr -level
            }
            return -options $options $result
        }
    }

    proc alias {name command args} {
        set origin [uplevel 1 namespace origin [list $command]]
        if {$origin != {}} {
            set command $origin
        } else {
            cli::Warning "command \"$command\" is not defined"
        }
        if {![string match ::* $name]} {
            set ns [uplevel 1 namespace current]
            if {![string match *:: $ns]} {
                append ns ::
            }
            set name $ns$name
        }
        uplevel 1 [list interp alias {} $name {} $command {*}$args]
    }

    cli::Alias prtobj prettyTclObject
    cli::Alias prtres prettyTclResult

    shell::autocomplete::addCommand repeat shell::autocomplete::paramCompleteAfter 2
    shell::autocomplete::addCommand fallback cli::Fallback::ArgumentParser autocomplete
    shell::autocomplete::addCommand prettyTclResult shell::autocomplete::paramCompleteAfter 1

    cli::Help::Manager add repeat -short "Repeat command or script." -synopsis {
        repeat count command ?args...?
    } -args {
        count {
            -short "Number of iterations."
        }
        command {
            -short "Command or script to repeat."
        }
        args {
            -short "Arguments to `command`."
        }
    } -long {
        Allows repeating a command or script several times.

        Usually there's no real need to do this but it can be useful when migrating MoviDebug Classic scrips.

        *** EXAMPLES

        Do ten line steps, printing the current line after each one:
        % repeat 10 step into -line
        Note: Usually it's faster to do this without intermediary feedback:
        % step into -line -count 10

        Do the same line steps, but pause a second between them:
        % repeat 10 {step -line; wait -ms 1000}

        Display the current PC, function+offset, file and line for a running core, once per second:
        # Interrupt with Control-C.
        # Note: "forever" is always greater than any number so this will work.
        % repeat forever { puts "[target]: [state -pc -sym -file -line]"; wait -ms 1000 }
    }

    cli::Help::Manager add alias -short "Define command shortcut." -synopsis {
        alias name command ?args...?
    } -args {
        name {
            -short "Name of alias"
        }
        command {
            -short "Target command to refer to."
        }
        args {
            -short "Extra argument of target command"
        }
    } -long {
        Useful, autocomplete-friendly version of the\
        "interp alias" command.

        Lets the user define their own shortcuts to commands\
        and command-argument combinations.

        Note: For more complicated situations, use the Tcl built-in "proc" to define procedures .

        *** EXAMPLES

        Simplify typing:
        % alias c continue
        % alias i step into -line
        % alias s step over
        % alias r step return
        % alias l listsource -2 +3
        % alias k callstack

        Define a jtag-only `mget` command:
        % alias jget mget -jtag
        - this is legal afterwards:
        % jget -sym mvConsoleTxQueue

        Define a printf-like command:
        % alias printf apply {{args} {puts [format {*}$args]}}
        - allows for
        % printf "Address is 0x%08x" $addr

    }

    shell::autocomplete::addCommand alias shell::autocomplete::paramCompleteAfter 2

    # This one is defined in the shell
    cli::Help::Manager add {eval%} \
        -synopsis {
        ::mdbg::eval% script ?args...?
    } \
        -short {Pseudo-interactive script evaluation} \
        -long {
        This facility was added to provide for non-interactive script debugging.

        The executed script is echoed to the standard output
        along with a virtual prompt showing the current target.

        The result of the command is returned to the caller
        but it's also echoed back to the standard output.

        The namespace path is temporarily altered to include the "::mdbg" namespace.
        This allows better access to moviDebug commands within scripts.

        *** EXAMPLES

        Sample script (example.tcl)

            # Set up convenient shortcut for command
            interp alias {} % {} ::mdbg::eval%

            % breset
            % loadfile $elf
            # Set a breakpoint, store value
            set bpid [
                % breakpoint add -location main
            ]
            % runw
            % breakpoint remove $bpid
            % cont -wait
            # Avoid substitution with braces:
            % {
                breset; loadfile $elf; run; wait
            }


        Run command:
            moviDebug2 -D:elf=output/001_HelloWorldLeon.elf --script example.tcl

        Output:
            ...
            P0:AA% breset
            ...
            P0:AA% loadfile output/001_HelloWorldLeon.elf
            P0:ALOS
            ...
            P0:ALOS% breakpoint add -location main
            #1
            ...
            P0:ALOS% runw
            ...
            P0:ALOS% breakpoint remove #1
            ...
            P0:ALOS% cont -wait
            ...
            P0:AA% breset; loadfile $elf; run; wait
            ...
    }

    namespace export repeat fallback alias

} ; # namespace eval mdbg