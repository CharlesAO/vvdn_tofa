# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : callstack.tcl
# Description: Call stack command
# Created on : Oct 23, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::CallStack {

        namespace path ::mdbg

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Get call stack for current target."
            -description {
                Display a table of the call stack for target.
                Numeric columns include the Instruction Pointer, the Frame Pointer and the Return Address.
                Each row's instruction pointer is interpreted to display symbol and line number information.

                *** ALIASES

                ALIAS       COMMAND
                cs          callstack

                *** NOTES

                A more traditional display format can be attained using:
                % state -stack

                *** EXAMPLES

                Display call stack for current core:
                % callstack

                Display 3 levels from LOS call stack starting with the second level:
                % callstack  -target LOS -start 1 -count 3

                Display top 5 levels of call stack for current core:
                % callstack 5
            }

            -args {
                -target {
                    -optional
                }
                -quiet {
                    -flag
                    -brief "Don't print, return Tcl list"
                }
                -start {
                    -brief "First level."
                    -type integer
                    -default 0
                }
                -count {
                    -var levels
                    -brief "Count of levels."
                    -description {
                        Default is 10 levels.
                    }
                    -type integer
                    -optional
                }
                target {
                    -optional
                    -not-regexp {^[0-9]+$}
                }
                count {
                    -var levels
                    -type integer
                    -optional
                }
            }
            -mutex {
                {-target target}
                {-count count}
            }
            -arg-complete-scripts {
                {-target target} {
                    mdbg::cli::Target::getValidTargets $PREFIX*
                }
            }
        }

        oo::objdefine ArgumentParser {
            method setupArgs {{level 0}} {
                incr level
                uplevel 1 {
                    set contextId [::mdbg::cli::Target::getContextIdFromTargetVar target]
                    if {![info exists levels]} {
                        set levels 10
                    }
                }
            }
        }

        proc getFrames {contextId start count} {
            set result [list]
            set ids [tcf::StackTrace::getChildrenRange $contextId $start [expr {$start + $count - 1}]]
            set frames [tcf::StackTrace::getContext {*}$ids]

            foreach frame $frames  {
                set frameDict [tcf::getFrameFieldDict $frame]
                addLineInformation $contextId frameDict
                lappend result $frameDict
            }
            set result
        }

        proc addLineInformation {contextId frameVar} {
            upvar 1 $frameVar frame
            set IP [::tcf::getFrameField $frame INSTRUCTION_PTR ""]
            if {$IP != ""} {
                dict update frame Line line File file Directory directory {
                    lassign [cli::LineNumbers::getLineFromAddress $contextId $IP] directory file line column
                }
            }
        }

        proc getSimpleFrameRow {frame} {
            set result [list]
            dict with frame {
                foreach var {File Line Directory Column} {
                    if {![info exists $var]} {
                        set $var ""
                    }
                }
                if {$INSTRUCTION_PTR != ""} {
                    lappend result "IP = $INSTRUCTION_PTR"
                    set sym [sym at \
                        -addr $INSTRUCTION_PTR \
                        -context $PARENT_ID \
                        -first -quiet -nocomplain
                    ]
                    if {$sym != ""} {
                        lappend result $sym
                    }
                }
                set src [cli::FileLineColumn [file join $Directory $File] $Line $Column]
                if {$src != ""} {
                    lappend result $src
                }
            }
            return "[join $result ", "]"
        }

        cli::TabularDisplay create Display {
            INDEX {
                -header #
                -align right
            }
            INSTRUCTION_PTR {
                -header IP
                -present
            }
            {INSTRUCTION_PTR PARENT_ID} {
                -header "Symbol"
                -min-width 8
                -expand
                -wrap word
                -transform {
                    sym at \
                        -addr $INSTRUCTION_PTR \
                        -context $PARENT_ID \
                        -first -quiet -nocomplain
                }
            }
            {Directory File Line Column} {
                -header "Source location"
                -expand
                -wrap word
                -transform {
                    ::mdbg::cli::FileLineColumn $File $Line $Column
                }
            }
            RETURN_PTR {
                -header "ReturnAddr"
            }
            FRAME_PTR {
                -header "Frame Ptr."
            }
            LEVEL {
                -header "Level"
                -align right
                -format  "%d "
                -space-after 1
            }
            {ARGS_COUNT ARGS_ADDR} {
                -header "Args"
                -transform {
                    set result [list]
                    if {$ARGS_COUNT != {}} {
                        lappend result $ARGS_COUNT
                    }
                    if {$ARGS_ADDR != {}} {
                        lappend result "@$ARGS_ADDR"
                    }
                    join $result ", "
                }
            }

            {TOP_FRAME WALK} {
                -header "Details"
                -min-width 6
                -expand
                -wrap word
                -transform {
                    set result [list]
                    if {[string is true -strict $TOP_FRAME]} {
                        lappend result "Top frame"
                    }
                    if {[string is true -strict $WALK]} {
                        lappend result "CFI"
                    }
                    join $result ", "
                }
            }

        }

        oo::objdefine Display {
            mixin cli::ResultTable
        }

        proc getFrameId {contextId frame} {
            set frameId [tcf::getStackFrameId $contextId $frame]
            if {$frameId == {}} {
                return -code error "$contextId: stack frame $frame not found"
            }
            return $frameId
        }

    } ; # namespace eval cli::CallStack

    proc callstack {args} {
        cli::CallStack::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            cli::CallStack::ArgumentParser setupArgs

            set frames [cli::CallStack::getFrames $contextId $start $levels]

            if {$quiet} {
                return $frames
            }

            if {[llength $frames]} {
                cli::CallStack::Display display $frames
            } else {
                cli::PutInfo "No frames."
            }
        }
    }

    shell::autocomplete::addCommand {callstack stack} cli::CallStack::ArgumentParser autocomplete
    cli::Help::Manager add callstack -parser cli::CallStack::ArgumentParser

    cli::Alias {cs} callstack

    proc stack {args} {
        cli::DeprecatedWarning callstack
        tailcall callstack {*}$args
    }

    namespace export callstack

} ; # namespace eval mdbg

