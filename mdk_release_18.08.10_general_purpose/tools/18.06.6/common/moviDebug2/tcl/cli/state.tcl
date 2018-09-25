# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/state.tcl
# Description: CLI `state` multi-command
# Created on : Oct 8, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::State {
        namespace path ::mdbg

        cli::ArgumentParser create ArgumentParser {
            -brief "Processor and thread state."
            -description {
                Display and/or return TCF RunControl context states.

                *** ALIASES

                - ps

                *** EXAMPLES

                Display state of the current core:
                % state

                Display state of the all contexts (cores + threads):
                % state -all

                Display state of Leon OS:
                % state -target LOS
                % state LOS

                Display state of the current core:
                % state

                Display the values of the most important Leon OS registers:
                % state -target LOS -reg

                Return the values of the same LOS registers as Tcl dictionary:
                % state -target LOS -reg -quiet

                Return the PC value of LOS:
                % state -target LOS -pc

                Return 1 when the application exited:
                % state -exited -target [startupcore]

                Return 1 when LRT is running, 0 otherwise:
                % state -isrunning LRT

                Return current file and line:
                % state -file -line

                Return function name+offset at current PC:
                % state -sym

                Something very similar to the MoviDebug Classic `rtemsthreads` command:
                % state -children [target]:RTEMS -stack
            }
            -auto
            -args {
                -all {
                    -synopsis {-a[ll]}
                    -flag
                    -brief "All targets."
                }
                -a { -flag -hidden -var all }
                -reg {
                    -flag
                    -var getRegs
                    -brief "Get register values for target."
                    -description {
                        Target should be LEON or SHAVE core.
                    }
                }
                -F {
                    -flag
                    -var getFRegs
                    -requires {-reg}
                    -brief "Also get float registers."
                    -description {
                        Target should be LEON core.
                    }
                }
                -asr {
                    -flag
                    -var getAsrRegs
                    -requires {-reg}
                    -brief "Also get ASR(17, 24..31) registers."
                    -description {
                        Target should be LEON core.
                    }
                }
                -target {
                    -optional
                    -brief "Specific target."
                }
                -recursive {
                    -flag
                    -brief "Target and its children."
                    -opt-var target
                    -opt-param parent
                }
                -children {
                    -flag
                    -brief "Target's immediate children."
                    -opt-var target
                    -opt-param parent
                }
                -quiet {
                    -flag
                    -brief "Don't print. Return value."
                }
                -isrunning {
                    -flag
                    -var isRunning
                    -brief "Return logical true if target is running."
                }
                -issuspended {
                    -flag
                    -var isSuspended
                    -brief "Return logical true if target is suspended."
                }
                -isbp {
                    -flag
                    -var isBp
                    -brief "Return logical true if target is suspended in breakpoint."
                }
                -bpids {
                    -flag
                    -var getBpIds
                    -brief "Return list of breakpoint IDs the target is currently suspended at."
                }
                -exited {
                    -flag
                    -var isExited
                    -brief "Return logical true if target has finished execution."
                }
                -history {
                    -flag
                    -brief "Get execution history for core."
                    -disabled
                }
                -pc {
                    -flag
                    -var getPC
                    -brief "Get Program Counter for target."
                }
                -sym {
                    -flag -var getSym
                    -brief "Get symbol name+offset at current PC"
                }
                -function {
                    -flag -var getFunction
                    -brief "Get symbol name at current PC without offset (current function)"
                }
                -offset {
                    -flag -var getOffset
                    -brief "Get offset of current PC relative to current symbol name (function)"
                    -requires {-function}
                }
                -basedir {
                    -flag -var getDirectory
                    -brief "Get base directory of current file at PC"
                }
                -file {
                    -flag -var getFile
                    -brief "Get current file (name relative to base directory)"
                }
                -line {
                    -flag -var getLine
                    -brief "Get current line"
                }
                -column {
                    -flag -var getColumn
                    -brief "Get current column"
                }
                -suspended-only {
                    -flag
                    -brief "Only suspended targets."
                    -var suspendedOnly
                }
                -stack {
                    -flag
                    -brief "Show call stack for suspended target(s)"
                }
                -update {
                    -flag
                    -brief "Force a core state update on target or target group."
                    -description {
                        The state update is done synchronously unless the "-async" option is given.

                        This is useful if the automatic core state monitoring is disabled.
                    }
                }
                -async {
                    -flag -requires {-update}
                    -brief "Asynchronous update"
                    -description {
                        Requires the "-update" option.

                        Does not wait for the update to be finished.
                        Causes the function to return and print nothing.

                        Because of this it cannot be used together with many options.
                    }
                }
                target {
                    -optional
                    -brief "Target (after switches)."
                    -description {
                        Same as -target argument.
                        Incompatible with "-all" and "-target".

                        Added for convenience to allow simple use-cases like
                        "ps LOS"
                    }
                }
            }
            -mutex {
                {-all -a -target target parent}
                {-recursive -children -all -a}
                {
                    {-recursive -children -all -a -stack -suspended-only -async}
                    {-reg -asr -F}
                    -issuspended
                    -isrunning
                    -exited
                    -isbp
                    -bpids
                    {-pc -sym -function -offset -directory -file -line -column}
                }
                {-sym -function}
                {-all -suspended-only}
                {-stack -quiet}
                {-async {-suspended-only -stack}}
            }
            -arg-complete-scripts {
                {-target target parent} {
                    cli::Target::getValidTargets $PREFIX*
                }

            }
        }

        oo::objdefine cli::State::ArgumentParser {
            method setupArgs {  {level 0} } {
                incr level
                uplevel $level {
                    if {$all} {
                        set multi 1
                        set contextIds [cli::Target::getAllRunContextIds]
                    } else {
                        set contextId [cli::Target::getContextIdFromTargetVar target]
                        set multi 0
                        set contextIds [list $contextId]
                        if {$children} {
                            set multi 1
                            set contextIds [::tcf::RunControl::getChildren $contextId]
                        } elseif {$recursive} {
                            set multi 1
                            cli::State::getAllRunControlChildren contextIds $contextId
                        } elseif {$suspendedOnly} {
                            set multi 1
                            set contextIds [cli::Target::getAllRunContextIds]
                        }
                    }
                }
            }
        }

        proc getAllRunControlChildren {contextIdsVar contextId } {
            upvar 1 $contextIdsVar contextIds
            foreach id [::tcf::RunControl::getChildren $contextId] {
                lappend contextIds $id
                getAllRunControlChildren contextIds $id
            }
        }

        cli::TabularDisplay create Display {
            ID {}
            NAME {
                -expand
                -wrap word
            }
            {RUNNING SUSPENDED STATE_NAME} {
                -header "STATE"
                -transform {
                    if {$RUNNING != "" && $RUNNING} {
                        return "Running"
                    }
                    if {$SUSPENDED != "" && $SUSPENDED} {
                        return "Suspended"
                    }
                    return $STATE_NAME
                }
            }
            PC {
            }
            REASON {
                -header "SUSPEND REASON"
            }
        }

        proc get {contextId {flags {}} } {
            set context [tcf::getContext RunControl $contextId]

            lassign [tcf::getContextFieldList RunControl $context {
                    ID          NAME       HAS_STATE  CAN_RESUME  CAN_SUSPEND
            } [list $contextId  <unnamed>      false           0            0]
            ]       id          name        hasState   canResume   canSuspend

            if {$hasState} {
                lassign [tcf::RunControl::getState $id] suspended pc reason stateData

                set result [dict create \
                    ID $id \
                    NAME $name \
                    SUSPENDED $suspended \
                ]
                if {$reason != {}} {
                    dict set result REASON $reason
                }

                if {[dict exists $stateData "StateName"]} {
                    set stateName [dict get $stateData "StateName"]
                    if {$stateName != {}} {
                        dict set result STATE_NAME $stateName
                    }
                }

                if {$suspended} {
                    dict set result PC [cli::hexAddr $pc]
                    dict set result RUNNING false
                } else {
                    set running [expr {
                        [dict exists $result STATE_NAME] ? "false" : "true"
                    }]
                    if {$running} {
                        dict set result RUNNING $running
                        catch {
                            set pc [::tcf::MyriadCore::getPC $contextId]
                            dict set result PC [cli::hexAddr $pc]
                        }
                    }
                    set result
                }
            } else {
                dict create ID $id NAME $name
            }
        }


        oo::objdefine Display {
            mixin cli::ListResultTable
        }

    } ; # namespace eval cli::State

    proc state {args} {
        cli::State::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {
            cli::State::ArgumentParser setupArgs

            if {$update} {
                set updateCoreIds [list]
                foreach contextId $contextIds {
                    foreach coreId [tcf::MyriadCore::getCoreList $contextId] {
                        if {$coreId ni $updateCoreIds} {
                            lappend updateCoreIds $coreId
                        }
                    }
                }
                foreach coreId $updateCoreIds {
                    tcf::MyriadCore::updateState $coreId $async
                }
                if {$async} {
                    return
                }
            }

            if {!$multi} {
                set contextId [lindex $contextIds 0]
            }

            if {$isRunning} {
                return [tcf::isContextRunning $contextId]
            }
            if {$isSuspended} {
                return [tcf::isContextSuspended $contextId]
            }
            if {$isBp} {
                return [tcf::isContextInBreakpoint $contextId]
            }
            if {$getBpIds} {
                return [tcf::getContextBreakpointsHit $contextId]
            }
            if {$isExited} {
                set runState [::tcf::RunControl::getState $contextId]
                lassign [::tcf::fields::getNamedFieldList RunControl::STATE $runState \
                    {SUSPENDED REASON} \
                    {false     ""} \
                ]    suspended reason
                return [expr {
                    $suspended && (
                        [string match -nocase {*Application*terminated*} $reason] ||
                        [string match -nocase {*Exited*} $reason]
                    )
                }]
            }
            if {$getPC || $getFunction || $getSym || $getDirectory || $getFile || $getLine || $getColumn} {
                set pc [tcf::getContextPC $contextId]
                set result [list]
                if {$getPC} {
                    lappend result $pc
                }
                if {$getSym || $getFunction} {
                    set sym+offs [sym at -addr $pc -context $contextId -quiet -first -nocomplain]
                    if {$getFunction} {
                        regexp {^([^+]*)([+]?.*)$} ${sym+offs} -> function offset
                        lappend result $function
                        if {$getOffset} {
                            if {$offset in {"+" ""}} {
                                set offset 0
                            }
                            lappend result $offset
                        }
                    } elseif {$getSym} {
                        lappend result ${sym+offs}
                    }
                }
                if {$getDirectory || $getFile || $getLine || $getColumn} {
                    lassign [cli::LineNumbers::getLineFromAddress $contextId $pc] dir file line column
                    if {$getDirectory} {
                        lappend result $dir
                    }
                    if {$getFile} {
                        lappend result $file
                    }
                    if {$getLine} {
                        lappend result $line
                    }
                    if {$getColumn} {
                        lappend result $column
                    }
                }
                if {[llength $result] == 1} {
                    set result [lindex $result 0]
                }
                return $result
            }

            if {$getRegs} {
                if {!$quiet} {
                    cli::State::Reg::showRegs $contextId $getFRegs $getAsrRegs
                    return
                } else {
                    return [cli::State::Reg::getRegs $contextId $getFRegs $getAsrRegs]
                }
            }
            set items [list]

            foreach id $contextIds {
                set item [cli::State::get $id]
                if {$suspendedOnly} {
                    if {![dict exists $item SUSPENDED] ||
                        ![dict get    $item SUSPENDED]
                    } {
                        continue
                    }
                }
                lappend items $item
            }

            if {$quiet} {
                if {!$multi} {
                    return [lindex $items 0]
                }
                return $items
            }

            if {!$stack} {
                cli::State::Display display $items
            } else {
                lassign [cli::State::Display getTable $items] header rows
                cli::State::Display PutHeader $header
                foreach item $items row $rows {
                    cli::State::Display PutRow $row
                    unset -nocomplain SUSPENDED
                    dict with item {
                        if {[info exists SUSPENDED] && $SUSPENDED} {
                            foreach frame [callstack -quiet -target $ID -count 20] {
                                cli::PutInfo [cli::CallStack::getSimpleFrameRow $frame]
                            }
                        }
                    }
                }

            }
        }
    }

    shell::autocomplete::addCommand state cli::State::ArgumentParser autocomplete
    cli::Help::Manager add state -parser cli::State::ArgumentParser

    namespace export state

} ; # namespace eval mdbg

interp alias {} ::mdbg::ps {} ::mdbg::state
