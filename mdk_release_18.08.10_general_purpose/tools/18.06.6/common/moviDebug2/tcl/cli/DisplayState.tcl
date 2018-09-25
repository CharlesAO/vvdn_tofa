# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : DisplayState.tcl
# Created on : Aug 28, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
if {[namespace exists ::tcf::RunControl::Events]} {

    namespace eval mdbg::cli::DisplayState {

        namespace path {::mdbg}

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Control context state display"
            -description {
                Control the display of console messages on state change.

                *** EXAMPLES

                Enable essential messages:
                % displaystate on

                Disable all messages:
                % displaystate off

                Enable all messages:
                % displaystate verbose

                Query current level:
                % displaystate
            }
            -args {
                verbosity {
                    -choice {on off verbose}
                    -brief "Verbosity level"
                    -description {
                        Possible values:
                        - `on` - Enable essential messages.
                        - `off` - Disable all messages.
                        - `verbose` - Enable all messages

                        This parameter is optional. When missing,\
                        the command will return the current verbosity level.
                    }
                    -optional
                }
            }
        }

        variable Verbose false

        proc PutMsg {Put message} {
            cli::$Put $message
        }

        proc onSyncHalt {id runState} {
            if {[Verbosity::IsNormal]} {
                lassign $runState suspended pc reason data
                if {$suspended} {
                    cli::PutInfo [getSuspendMessage $id $pc $reason $data]
                } else {
                    cli::PutWarning [getResumeMessage $id $data]
                }
            }
        }

        proc onContSync {id} {
            if {[Verbosity::IsNormal]} {
                set runState [tcf::RunControl::getState $id]
                lassign $runState suspended pc reason data
                switch -exact [tcf::getRunStateName $runState] {
                    SUSPENDED {
                        switch -glob $reason {
                            Breakpoint -
                            Step -
                            *Application*terminated* {
                                # do nothing (shown by default)
                            }
                            default {
                                cli::PutWarning [getSuspendMessage $id $pc $reason $data]
                            }
                        }
                    }
                    RUNNING {
                        cli::PutInfo [getResumeMessage $id $data]
                    }
                    INACTIVE {
                        cli::PutWarning [getResumeMessage $id $data]
                    }
                }
            }
        }

        proc onContWait {id} {
            if {[Verbosity::IsNormal]} {
                lassign [tcf::RunControl::getState $id] suspended pc reason data
                if {!$suspended} {
                    cli::PutWarning [getResumeMessage $id $data]
                } else {
                    switch -glob $reason {
                        Breakpoint -
                        Step -
                        *Application*terminated* {
                            # do nothing (shown by default)
                        }
                        default {
                            cli::PutInfo [getSuspendMessage $id $pc $reason $data]
                        }
                    }
                }
            }
        }

        namespace eval Verbosity {
            variable Info [list]

            proc CheckInfo {event} {
                variable Info
                if {$event in $Info} {
                    return -level 2 PutInfo
                }
            }

            proc OnSuspend {reason} {
                CheckInfo OnSuspend
                upvar #0 [namespace parent]::Verbose Verbose

                if {[string match {*Application*terminated*} $reason]} {
                    return PutInfo
                }
                switch -exact $reason {
                    Breakpoint {
                        return PutInfo
                    }
                    Step {
                        return PutInfo
                    }
                }
                if {     [string match *trap*      $reason]
                     && ![string match *trap*0x00* $reason]
                } {
                    return PutWarning
                }

                if {!$Verbose} {
                    return -level 2
                }
                return PutEvent
            }

            proc OnResume {} {
                CheckInfo OnResume
                upvar #0 [namespace parent]::Verbose Verbose
                if {!$Verbose} {
                    return -level 2
                }
                return PutEvent
            }

            proc OnStateChange {} {
                CheckInfo OnStateChange
                upvar #0 [namespace parent]::Verbose Verbose
                if {!$Verbose} {
                    return -level 2
                }
                return PutEvent
            }

            proc OnException {} {
                return PutWarning
            }

            proc IsNormal {} {
                upvar #0 [namespace parent]::Verbose Verbose
                upvar #0 [namespace parent]::Events Events
                expr {!$Verbose && [info exists Events] && $Events == {+}};
            }

        } ; # namespace eval Verbosity

        proc getSuspendMessage {id pc reason data} {
            set message [cli::Target::getContextEventPrefix $id]
            append message " suspended"
            if {$pc != {}} {
                append message " at [cli::hexAddr $pc]"
            }
            if {$reason != {}} {
                if {$data != {}} {
                    upvar #0 ::tcf::RunControl::STATE::INDEX::REASON::VALUES::BREAKPOINT BREAKPOINT
                    if {$reason == $BREAKPOINT} {
                        set idList [::tcf::fields::getNamedField RunControl::STATE::DATA $data BPS]
                        append reason "; IDs: [join $idList {, }]"
                    } else {
                        append reason ", $data"
                    }

                }
                append message " ($reason)"
            } elseif {$data != {}} {
                append message " ($data)"
            }
            return $message
        }

        proc SuspendMessage {Put id pc reason data} {
            set message [getSuspendMessage $id $pc $reason $data]

            if {[string match {*Application*terminated*} $reason]} {
                catch {
                    uart::flushLine
                }
            }
            PutMsg $Put $message
        }

        proc ContextSuspended {event id pc reason data} {
            set Put [Verbosity::OnSuspend $reason]
            SuspendMessage $Put $id $pc $reason $data
        }

        proc ContainerSuspended {event ctxId pc reason data idList} {
            set Put [Verbosity::OnSuspend $reason]
            SuspendMessage $Put $ctxId $pc $reason $data
            foreach childId $idList {
                if {$childId != $ctxId} {
                    catch {
                        lassign [tcf::RunControl::getState $childId] suspended pc reason data
                        if {$suspended} {
                            SuspendMessage $Put $childId $pc $reason $data
                        }
                    }
                }
            }
        }

        proc getResumeMessage {id stateData {reason {}} } {
            set message [cli::Target::getContextEventPrefix $id]
            if {[dict exists $stateData StateName]} {
                append message " [dict get $stateData StateName]"
            } else {
                append message " is running..."
            }
            if {$reason != {}} {
                append message " ($reason)"
            }
            return $message
        }

        proc ResumeMessage {Put id {reason {}}} {
            if {[catch {
                        lassign [tcf::RunControl::getState $id] suspended pc suspendReason stateData
                    } error]} {
                PutMsg PutError "$id: state changed. (Error retrieveing state: $error)"
                return
            }
            if {!$suspended} {
                set message [getResumeMessage $id $stateData $reason]
                PutMsg $Put $message
            }
        }

        proc ContextResumed {event id} {
            set Put [Verbosity::OnResume]
            ResumeMessage $Put $id
        }

        proc ContainerResumed {event idlist} {
            set Put [Verbosity::OnResume]
            foreach id $idList {
                ResumeMessage $Put $id "(Container)"
            }
        }

        proc ContextStateChanged {event id} {
            set Put [Verbosity::OnStateChange]
            ResumeMessage $Put $id
        }

        proc ContextException {event id description} {
            set Put [Verbosity::OnException]
            set message [cli::Target::getContextEventPrefix $id]
            append message " exception - $description"
            PutMsg $Put $message
        }

        # Returns previous state
        proc Events {+|-} {
            variable Events
            if { ${+|-} ni {+ -}} {
                return -code error "invalid argument: must be + or -"
            }
            if {![info exists Events]} {
                set Events -
            }
            if {$Events == ${+|-}} {
                return $Events
            }
            set result $Events
            set Events ${+|-}

            foreach {event Handler} {
                    contextSuspended       ContextSuspended
                    containerSuspended     ContainerSuspended
                    contextResumed         ContextResumed
                    containerResumed       ContainerResumed
                    contextStateChanged    ContextStateChanged
                    contextException       ContextException
                } {
                ::tcf::RunControl::Events::${event}${+|-} ${Handler}
            }
            return $result
        }

        proc InfoEvents {events script} {
            variable Verbose
            set prEvt [Events +]
            try {
                cli::PreserveValue Verbosity::Info {
                    lappend Verbosity::Info {*}$events
                    uplevel 1 $script
                }
            } finally {
                Events $prEvt
            }
        }

    } ; # namespace eval mdbg::cli::DisplayState

    namespace eval mdbg {

        proc displaystate {args} {
            cli::DisplayState::ArgumentParser parseCallerArgs 1 $args
            cli::NoErrorStack {
                variable cli::DisplayState::Verbose

                if {![info exists verbosity]} {
                    variable cli::DisplayState::Events
                    if {![info exists  cli::DisplayState::Events]
                                   || $cli::DisplayState::Events == "-"
                    } {
                        return off
                    }
                    if {![info exists   cli::DisplayState::Verbose]
                                   || !$cli::DisplayState::Verbose
                    } {
                        return on
                    }
                    return verbose
                }

                switch -exact $verbosity {
                    on {
                        set cli::DisplayState::Verbose false
                        cli::DisplayState::Events +
                    }
                    off {
                        cli::DisplayState::Events -
                    }
                    verbose {
                        set cli::DisplayState::Verbose true
                        cli::DisplayState::Events +
                    }
                    default {
                        cli::NotImplemented
                    }
                }
            }
            return
        }

        shell::autocomplete::addCommand displaystate cli::DisplayState::ArgumentParser autocomplete
        cli::Help::Manager add displaystate -parser cli::DisplayState::ArgumentParser
        dll::atexit {
            cli::DisplayState::Events -
        }

        namespace export displaystate

        if {[cli::IsFlagVarTrue verbose] || [cli::IsFlagVarTrue VERBOSE_STATE]} {
            displaystate verbose
        } else {
            displaystate on
        }

    } ; # namespace eval mdbg

}; # if <TCF available>