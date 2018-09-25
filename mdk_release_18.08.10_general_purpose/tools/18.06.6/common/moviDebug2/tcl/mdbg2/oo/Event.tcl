# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : Event.tcl
# Description: Multi dispatch event class
# Created on : Feb 2, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::oo {

    oo::class create Event {

        variable DefaultArgs
        variable Delegates
        variable LastId

        variable Error
        variable Options

        #
        #  Public
        #

        # Construct object.
        # Specified arguments will be passed to delegates automatically
        constructor {args} {
            set DefaultArgs $args
            set Delegates [dict create]
            set LastId 0
        }

        # Bind command or script to event
        # Returns connection ID
        # `command` is treated as script when there are *no other `args`*, and:
        # - command is a list or
        # - command contains one of the following:
        #   - %args or %{args}  - representing the list of actual parameters
        #   - %0, %1, ...       - representing the positional arguments
        #   - %%                - representing the % character
        method bind {command args} {
            set ns [uplevel 1 namespace current]
            set key [my CreateKey [incr LastId]]
            set delegate [my CreateDelegate $ns $command {*}$args]
            dict set Delegates $key $delegate
            return $key
        }

        # Unbind command or script from event
        # Same rules as for `bind`
        # Returns list of removed connection IDs
        method unbind {command args} {
            set ns [uplevel 1 namespace current]
            set delegate [my CreateDelegate $ns $command {*}$args]
            my RemoveDelegate $delegate
        }

        # Remove connection
        # id - connection ID
        method cancel {id} {
            dict unset Delegates $id
            return
        }

        # Remove all delegates
        method clear {} {
            set Delegates [dict create]
        }

        # Return true if delegates connected
        method empty {} {
            expr {[dict size $Delegates] == 0}
        }

        # Return list of all ids
        method keys {} {
            dict keys $Delegates
        }

        # Raise
        # Call all delegates synchronously
        # Default arguments values are passed first
        # followed by given arguments
        method raise {args} {
            my Invoke {*}$args
        }

        # Post event
        # Call to delegates will be dispatched asynchronously.
        # To cancel dispatch, pass returned ID  to `after cancel`.
        # Default arguments values are passed first
        method post {args} {
            after 0 [namespace code [list my Invoke {*}$args]]
        }

        #
        #  Protected
        #
        method OnError {error options} {
            dict with options {
                if {[info exists -errorinfo]} {
                    set error ${-errorinfo}
                }
            }
            puts stderr $error
        }

        #
        #  Private
        #
        method Invoke {args} {
            set my [self namespace]

            dict for {key delegate} $Delegates {
                lassign $delegate ns command xargs
                set code [namespace inscope $ns catch [list \
                    $command {*}$xargs {*}$DefaultArgs {*}$args \
                ] ${my}::Error ${my}::Options]
                switch -exact $code {
                    1 { my OnError $Error $Options}
                    3 { break }
                    4 { continue }
                }
            }
        }

        # Create connection id based on counter
        method CreateKey {counter} {
            return "[self namespace]::$counter"
        }

        # Create invokable delegate
        method CreateDelegate {ns command args} {
            if {![llength $args] && [my IsScript $command script]} {
                my WrapScript $script $ns
            } else {
                list $ns $command $args
            }
        }

        # Remove delegate
        method RemoveDelegate {delegate} {
            set result [list]
            set Delegates [
                dict filter $Delegates script {key value} {
                    if {$value == $delegate} {
                        lappend result $key
                        continue
                    }
                    expr 1
                }
            ]
            return $result
        }

        # Detect script and optionally converts to lambda body
        # Returns 1 for scripts, 0 for commands
        method IsScript {command scriptVar} {
            set result [expr {
                    [llength $command] != 1
                    || [string match "*\n*" $command]
                }
            ]
            foreach {
                pattern              expansion
            } {
                %args\\M             $args
                %\{args\}            ${args}
                %(\\d+)\\M           "[lindex $args \\1]"
                %%                   %
             } {
                 if {[regsub -all $pattern $command $expansion command]} {
                     incr result
                 }
            }

            if {$result} {
                upvar 1 $scriptVar script
                set script $command
            }
            return $result
        }

        # Wrap script body into lambda-based delegate
        # Allows invoking `break` and `continue` from script
        # Returns delegate
        method WrapScript {script ns} {
            set script [list \
                return -code \
                    {[catch} \
                        [list $script] \
                    {result] $result} \
            ]
            set script [join $script " "]
            set lambda [list args $script $ns]

            list :: apply [list $lambda]
        }


    } ; # oo::class Event

} ; # namespace eval mdbg::oo