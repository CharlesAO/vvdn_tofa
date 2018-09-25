# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/breaknow.tcl
# Description: `breaknow` command
# Created on : Aug 27, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Halt {

        namespace path {::mdbg}

        proc async {id} {
            ::tcf::RunControl::suspend $id
        }

        proc sync {id timeout {temporary 0} {script ""}} {
            set context [::tcf::RunControl::getContext $id]
            lassign [::tcf::getContextFieldList RunControl $context {
                    NAME HAS_STATE CAN_SUSPEND
                    } [list $id false false]
            ] name hasState canSuspend

            if {!$hasState} {
                if {$temporary} {
                    return -code error "Stateless targets cannot be temporarily halted!"
                }
                async $id
                return
            }

            set runState [::tcf::RunControl::getState $id]
            switch -exact [::tcf::getRunStateName $runState stateName] {
                SUSPENDED {
                    if {!$temporary} {
                        cli::PutWarning "Already stopped"
                    } else {
                        return -code error "Already stopped"
                    }

                }
                RUNNING {
                    cli::Wait::contextNotRunningAfter $id $timeout {
                        async $id
                    }
                    set runState [::tcf::RunControl::getState $id]
                }
                INACTIVE {
                }
            }
            cli::DisplayState::onSyncHalt $id $runState
            if {$temporary} {
                try {
                    cli::UpScript 1 $script
                } finally {
                    cli::Continue::sync $id $timeout
                }
            }
        }

        proc getRunningContextIds {} {
            set result [list]
            if {[info command ::tcf::queryContext] != ""} {
                foreach contextId [::tcf::queryContext] {
                    catch {
                        set context [::tcf::RunControl::getContext $contextId]
                        set contextName [::tcf::getField $context Name $contextId]
                        if {
                            [string is true [::tcf::getField $context HasState false]]
                            && [string is true [::tcf::getField $context CanSuspend false]]
                        } {
                            if {[::tcf::isContextRunning $contextId]} {
                                lappend result $contextId
                            }
                        }
                    }
                }
            }
            return $result
        }

        proc IgnoreSuspendError {result} {
            expr {
                $result in {"Already stopped" "Invalid context"}
            }
        }

        proc all-async {} {
            foreach contextId [getRunningContextIds] {
                if {[catch {
                            ::tcf::RunControl::suspend $contextId
                        } result]} {
                    if {[IgnoreSuspendError $result]} {
                        continue
                    }
                    cli::PutError "$result"
                    cli::PutDebug "    (while halting $contextId)"
                }
            }
        }

        proc all-sync {{timeout ""}} {
            if {$timeout != ""} {
                set deadline [expr {[clock milliseconds] + $timeout}]
            }

            cli::DisplayState::InfoEvents {OnSuspend} {
                set contextIdList [getRunningContextIds]
                set waitFor [list]
                foreach contextId $contextIdList {
                    if {[catch {
                        ::tcf::RunControl::suspend $contextId
                        lappend waitFor $contextId
                    } result]} {
                        if {[IgnoreSuspendError $result]} {
                            continue
                        }
                        cli::PutError "$result"
                        cli::PutDebug "    (while halting $contextId)"
                    }
                }
                foreach contextId $waitFor {
                    if {[info exists deadline]} {
                        set timeout [expr {max(0, $deadline - [clock milliseconds]}]
                    }
                    if {[catch {
                        mdbg::cli::Wait::contextNotRunning $waitFor $timeout
                    } result options]} {
                        if {[IgnoreSuspendError $result]} {
                            continue
                        }
                        return -options $options $result
                    }
                }
                return $waitFor
            }
        }

        mdbg::cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Break execution of target(s)."
            -description {
                The command sends the Suspend command to the specified target (or the current one).
                Optionally it can be asynchronous, which means that it will not wait for the target(s) to really suspend.

                *** ALIASES

                ALIAS   FULL FORM
                bn      halt

                *** EXAMPLES

                Suspend all contexts. Wait until they stop:
                % halt -all

                Suspend Leon OS:
                % halt LOS

                Safely refresh and then print the list of running RTEMS threads:
                % halt -temporary { state -children [target]:RTEMS }

            }
            -args {
                -a { -flag -hidden -var all}
                -all {
                    -flag
                    -brief "Suspend all contexts."
                    -synopsis {-a[ll]}
                    -description {
                        Iterates all running TCF RunControl contexts and send the `suspend` command to them.

                        Waits until none of them is running.
                        Can be given a timeout.

                    *** NOTES

                        - recently changed to be synchronous by default.
                    }
                }
                -target {
                    -optional
                    -brief "Select target to halt."
                    -description {
                        See "target" below.
                    }
                }
                -async {
                    -flag
                    -brief "Do not wait for the target to suspend."
                    -description {
                        Cannot be used with "-timeout".
                    }
                }
                -timeout {
                    -type integer
                    -default {}
                    -brief "Throw exception if `timeout` milliseconds elapse before suspending the core."
                    -description {
                        Normally the operation waits indefinitely. This switch allows the operation to have a timeout.
                        This is especially useful for scripted scenarios.

                        Cannot be used with "-async".
                    }
                }
                -temporary {
                    -flag
                    -brief "Resume immediately after halt"
                    -description {
                        This is useful to get a refresh on the list of currenty running RTEMS threads.

                        Optionally can execute "script" while halted.
                        The "script" parameter cannot be given when the "-script" option is also present.
                    }
                    -opt-param script
                }
                -script {
                    -optional
                    -brief "Script to run while temporarily halted."
                    -requires {-temporary}
                    -description {
                        Requires the "-temporary" option.
                        Cannot be used when the "script" parameter is given to the "-temporary" option.
                    }
                }
                target {
                    -optional
                    -complete-script {
                        mdbg::cli::Target::getValidTargets $PREFIX*
                    }
                    -brief "Specify target of command."
                    -description {
                        If missing, the command uses the current target.
                        Same as argument of "-target" option.
                    }
                }
            }
            -mutex {
                {-script script}
                {-all -target target}
                {-async {-timeout -temporary}}
            }
        }

    } ; # namespace eval cli::Halt

    proc halt {args} {
        cli::Halt::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            if {$all} {
                if {$async} {
                    cli::Halt::all-async
                } else {
                    set halted [cli::Halt::all-sync $timeout]
                    if {$temporary} {
                        try {
                            if {[info exists script]} {
                                cli::UpScript 1 $script
                            }
                        } finally {
                            cli::Continue::all-async $halted
                        }
                    }
                }
                return
            }
            set id [cli::Target::getContextIdFromTargetVar target]
            if {$async} {
                cli::Halt::async $id
            } else {
                cli::Halt::sync $id $timeout $temporary {
                    if {[info exists script]} {
                        cli::UpScript 1 $script
                    }
                }
            }
        }
    }

    cli::Help::Manager add halt -parser mdbg::cli::Halt::ArgumentParser
    shell::autocomplete::addCommand halt mdbg::cli::Halt::ArgumentParser autocomplete

    cli::Alias bn halt

    namespace export halt

} ; # namespace eval mdbg
