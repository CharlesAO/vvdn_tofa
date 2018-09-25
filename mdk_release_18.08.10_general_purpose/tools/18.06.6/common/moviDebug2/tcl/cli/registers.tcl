# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : registers.tcl
# Description: TCF registers display
# Created on : Feb 11, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Registers {

        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -auto -brief "TCF Registers" -description {
            !!! Type "help registers list" for more help.
        } -default list regCmd {
            list {
                -brief "List TCF registers"
            }
            list - get {
                -args {
                    -target {
                        -optional
                        -brief "Select target."
                        -complete-script {
                            cli::Target::getValidTargets $PREFIX*
                        }
                    }
                    -frame {
                        -type integer
                        -optional
                        -brief "Select stack frame"
                    }
                }
            }
            get {
                -brief "Get value of register"
                -args {
                    name {-var regName -brief "Name of register"}
                }
            }
            list {
                -args {
                    -all {
                        -brief "Show all registers for context."
                        -flag
                    }
                    -bits {
                        -brief "Show bitfields."
                        -flag
                    }
                    -quiet {
                        -flag
                        -brief "Don't display; return list of dictionaries."
                        -description {
                            Normally the subcommand displays a table to the standard output\
                            and will leave no result in the Tcl interpreter.
                            This flag inhibits the display and causes the command to return a result\
                            usable for further processing by Tcl scripts.
                        }
                    }
                    -no-read {
                        -flag
                        -brief "Don't read values"
                        -var noRead
                    }
                    -force-read {
                        -flag
                        -brief "Force read values"
                        -var forceRead
                        -description {
                            Normally the subcommand will not read values which are marked as "ReadOnce",
                            because reading them would destroy their value.
                            This flag forces read on those values as well.
                        }
                    }
                    prefix {
                        -brief "Name prefix glob pattern."
                        -var pattern
                        -optional
                    }
                }
                -mutex {
                    {-all pattern}
                }
            }
        }

        cli::TcfTabularDisplay create Display Registers::CONTEXT {

            {NAME INDENT} {
                -header "NAME"
                -expr {
                    "$INDENT$NAME"
                }
            }

            SIZE {
                -align right
                -skip
            }

            ID { -skip }

            VALUE {
                -align right
            }

            PROCESS_ID {
                -header "PROCESS ID"
                -skip
            }

            PARENT_ID {
                -header "CONTEXT"
                -transform {
                    if {[string match R*. $PARENT_ID]} {
                        return
                    }
                    set PARENT_ID
                }
            }

            BITS {
                -align right
                -transform {
                    if {[llength $BITS] <= 1} {
                        return $BITS
                    }
                    set result [list]
                    set hi ""
                    set lo ""

                    set lappend_hi_lo {
                        if {$lo == ""} {
                            lappend result $hi
                        } else {
                            lappend result ${hi}:${lo}
                        }
                    }

                    foreach bit [lreverse $BITS] {
                        if {$hi == ""} {
                            set hi $bit
                        } elseif {$lo == "" && $hi == $bit + 1 || $lo == $bit + 1} {
                            set lo $bit
                        } else {
                            eval $lappend_hi_lo
                            set hi $bit
                            set lo ""
                        }
                    }
                    if {$hi != ""} {
                        eval $lappend_hi_lo
                    }
                    join $result ","
                }
            }

            DWARF_ID {
                -header "ID"
                -align right
            }

            DESCRIPTION {
                -expand
                -wrap word
            }

            {READABLE READ_ONCE WRITEABLE WRITEONCE SIDE_EFFECTS VOLATILE} {
                -header "Details"
                -default {0 0 0 0 0 0}
                -transform {
                    set result [list]
                    foreach {expr option} {
                        {!$READABLE && !$WRITEABLE}    "no read, no write"
                        {!$WRITEABLE && $READABLE}     "read only"
                         $READ_ONCE                    "read once"
                        {!$READABLE && $WRITEABLE}     "write only"
                         $WRITEONCE                    "write once"
                         $SIDE_EFFECTS                 "side effects"
                         $VOLATILE                     "volatile"
                    } {
                        if $expr {
                            lappend result $option
                        }
                    }
                    join $result ", "
                }
            }

            "*" {
                -expand
                -wrap word
                -transform {
                     prettyTclObject ${*}
                }
                -skip
            }

        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

        proc scanRegisterValue {data size {isfloat 0} } {
            if {$isfloat && $size in {2 4 8} } {
                cli::Binary::scanFloat $data $size
            } elseif {$size in {1 2 4 8}} {
                set value [cli::Binary::scanUnsigned $data $size]
                format "0x%0[expr $size * 2]X" $value
            } else {
                cli::Binary::scanByteList $data
            }
        }

        proc getRegisterValue {regContext} {
            set DEFAULTS {false false "" "" 4}
            set KEYS [list FLOAT BIG_ENDIAN VALUE VALUES SIZE BITS FIRST_BIT LEFT_TO_RIGHT]
            lassign [tcf::getContextFieldList Registers $regContext $KEYS $DEFAULTS] {*}$KEYS

            if {$VALUE == ""} {
                return
            }

            set data [cli::Binary::formatBase64 $VALUE]
            if {$BITS != ""} {
                binary scan $data b* allbits
                set result ""
                foreach bit $BITS {
                    set bit [expr {$bit - $FIRST_BIT}]
                    append result [string index $allbits $bit]
                }
                if {!$LEFT_TO_RIGHT} {
                    set result [string reverse $result]
                }
                return $result
            }
            scanRegisterValue $data $SIZE $FLOAT
        }

    } ; # namespace eval cli::Registers

    proc registers {args} {

        cli::Registers::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {

            set contextId [cli::Target::getContextIdFromTargetVar target]
            if {[info exists frame]} {
                set contextId [cli::CallStack::getFrameId $contextId $frame]
            }

            switch $regCmd {
                list {
                    set regIdList [::tcf::Registers::getChildren $contextId]

                    set regContextList [list]

                    foreach id $regIdList {
                        set regContext [::tcf::Registers::getContext $id]
                        lassign [::tcf::getContextFieldList Registers $regContext {
                            NAME ROLE DWARF_ID EH_FRAME_ID FIRST_BIT LEFT_TO_RIGHT READABLE READ_ONCE
                        } { ""   ""         -1           0        0              0        0         0} \
                        ]   name role dwarf_id eh_frame_id first_bit left_to_right readable read_once

                        if {[info exists pattern]} {
                            if {![string match -nocase $pattern* $name]} {
                                continue
                            }
                        } elseif {!$all && $role == "" && $dwarf_id < 0 && $eh_frame_id <= 0} {
                            continue
                        }
                        lassign [tcf::getContextFieldList Registers $regContext \
                            {READABLE READONCE} {false false}] readable readOnce

                        if {  $readable ? !$read_once || $forceRead
                                        :  $read_once && $forceRead
                        } {
                            cli::CatchAndPrintError {
                                set regValue [
                                    ::tcf::Registers::get::base64 $id
                                ]
                            }
                            dict set regContext VALUE $regValue
                        }
                        lappend regContextList $regContext

                        if {!$bits} {
                            continue
                        }

                        foreach child [::tcf::Registers::getChildren $id] {
                            cli::CatchAndPrintError {
                                set bitContext [::tcf::Registers::getContext $child]
                                dict set bitContext INDENT " ."
                                dict set bitContext VALUE $regValue
                                dict set bitContext FIRST_BIT     $first_bit
                                dict set bitContext LEFT_TO_RIGHT $left_to_right
                                lappend regContextList $bitContext
                            }
                        }
                    }

                    cli::UpdateList regContextList regContext {
                        if {![dict exists $regContext VALUE]} {
                            continue
                        }
                        dict set regContext VALUE [cli::Registers::getRegisterValue $regContext]
                        if {$quiet} {
                            dict unset regContext INDENT
                        }
                    }

                    if {$quiet} {
                        return $regContextList
                    }

                    cli::Registers::Display display $regContextList
                }
                get {
                    set regIdList [::tcf::Registers::getChildren $contextId]
                    set regContextList [list]
                    foreach id $regIdList {
                        set regContext [::tcf::Registers::getContext $id]
                        set regCtxName [string toupper [::tcf::getContextField Registers $regContext NAME]]
                        if {[string equal -nocase $regName $regCtxName]} {
                            set regContextList [list $regContext]
                            break
                        } elseif {[string match -nocase $regName* $regCtxName]} {
                            lappend regContextList $regContext
                        }
                    }
                    if {[llength $regContextList] != 1} {
                        return -code error "unknown or ambiguous register name \"$regName\""
                    }
                    lassign $regContextList regContext

                    set id [::tcf::getContextField Registers $regContext ID]

                    set data [::tcf::Registers::get::bytearray $id]

                    lassign [::tcf::getContextFieldList Registers $regContext {SIZE IS_FLOAT} {4 0}] size isFloat

                    cli::Registers::scanRegisterValue $data $size $isFloat
                }
            }
        }
    }

    cli::Help::Manager add registers -parser cli::Registers::ArgumentParser
    shell::autocomplete::addCommand registers cli::Registers::ArgumentParser autocomplete
    namespace export registers

} ; # namespace eval mdbg