# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : continue.tcl
# Description: `cont` command for moviDebug2 cli
# Created on : Aug 31, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Continue {

        namespace path {::mdbg}

        proc getSuspendedContextIds {} {
            set ids [list]
            foreach id [cli::Target::getAllRunContextIds] {
                set state [cli::State::get $id]
                if {[dict exists $state SUSPENDED] && [dict get $state SUSPENDED]} {
                    lappend ids $id
                }
            }
            return $ids
        }

        proc IgnoreResumeError {result} {
            expr {
                $result in {"Already running" "Invalid argument"}
            }
        }

        proc all-async {ids} {
            foreach contextId $ids {
                if {[catch {
                    async $contextId
                } result]} {
                    if {[IgnoreResumeError $result]} {
                        continue
                    }
                    cli::PutError "$result"
                    cli::PutDebug "    (while resuming $contextId)"
                }
            }
        }

        proc async {id} {
            tcf::RunControl::resume $id NORMAL
        }

        proc sync {id timeout} {
            cli::Wait::contextResumedAfter $id $timeout {
                async $id
            }
            cli::DisplayState::onContSync $id
        }

        proc wait {id timeout} {
            cli::Wait::contextSuspendedOrAnyBreakpointAfter $id $timeout {
                async $id
            }
            cli::DisplayState::onContWait $id
        }

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Continue execution of target."
            -description {
                Resumes normal execution of target (a.k.a continue).
                Optionally it can be asynchronous, which means that it will send the Resume command \
                to the target but it will not wait for the target(s) to really resume.

                At also can wait until the target or suspends again or a breakpoint is hit by another target.

                See "-wait" and "-async" switches.

                *** ALIASES

                ALIAS               FULL FORM
                contw               cont -wait

                *** EXAMPLES

                Resume execution, wait until current core stops or a breakpoint is hit:
                % contw

                Resumes execution of the Leon RT:
                % cont LRT

            }
            -args {
                -wait {
                    -flag
                    -brief "Wait for target to suspend again."
                    -description {
                        Incompatible with "-async".
                    }
                }
                -async {
                    -flag
                    -brief "Do not wait for target to resume."
                    -description {
                        Normally the command will wait for the target to resume.
                        This wait can be bypassed, the command becoming asynchronous.
                        Incompatible with synchronisation options "-wait" and "-timeout".
                    }
                }
                -timeout {
                    -type integer
                    -default {}
                    -brief "Raise error on timeout (ms)."
                    -description {
                        Causes an error to be raised if the timeout period elapses before normal return.
                        The normal return condition is:
                        - target Suspended -- with the "-wait" flag present.
                        - target Resumed otherwise.
                        Incompatible with "-async".
                    }
                }
                -all {
                    -flag
                    -brief "Continue execution of all suspended targets (asynchronous)."
                    -description {
                        This is essentially an asynchronous operation, so the "-async" option is implied.
                        Cannot be combined with options incompatible with "-async", such as "-wait" or "-timeout".
                    }
                }
                -target {
                    -optional
                    -brief "Specify target."
                    -synopsis {?[-target] target?}
                    -description {
                        If missing, the current target is used, except for the "-all" option.

                        The "-target" option name can be missing if this is the last argument.
                    }
                    -complete-script {
                        mdbg::cli::Target::getValidTargets $PREFIX*
                    }
                    -comment {
                        # Keep this right before the next one for consisten help output!
                    }
                }
                target {
                    -hidden
                    -optional
                }
            }
            -mutex {
                {{-all -async} {-timeout -wait}}
                {-all -target target}
            }
        }

    } ; # namespace eval cli::Continue

    proc cont {args} {

        cli::Continue::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {

            set id [cli::Target::getContextIdFromTargetVar target]
            if {$all} {
                cli::Continue::all-async [cli::Continue::getSuspendedContextIds]
                return
            }
            if {$async} {
              cli::Continue::async $id
              return
            }
            cli::TrapInterrupt {
                  if {$wait} {
                      cli::Continue::wait $id $timeout
                  } else {
                      cli::Continue::sync $id $timeout
                  }
            }
        }
    }

    shell::autocomplete::addCommand cont cli::Continue::ArgumentParser autocomplete
    cli::Help::Manager add cont -parser cli::Continue::ArgumentParser

    cli::Alias contw cont -wait

    namespace export cont contw

} ; # namespace eval mdbg
