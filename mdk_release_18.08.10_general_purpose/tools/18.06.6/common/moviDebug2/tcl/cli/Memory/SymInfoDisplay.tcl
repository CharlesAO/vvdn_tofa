# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : SymInfoDisplay.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    cli::TabularDisplay create SymInfoDisplay {
        {ADDRESS TYPE_NAME SIZE} {
            -header ADDRESS
            -transform {
                if {$ADDRESS == {}} {
                    return "<no address>"
                }
                if {(
                    $TYPE_NAME in {"" "<binary>" "<asciiz>" } &&
                    [string is integer -strict $ADDRESS] &&
                    [string is integer -strict $SIZE] && $SIZE > 0
                )} {
                    set increment 16
                    if {$TYPE_NAME == "<asciiz>"} {
                        set increment 32
                    }
                    set result [list]
                    set end [expr {$ADDRESS + $SIZE}]
                    while {$ADDRESS < $end} {
                        lappend result $ADDRESS
                        mdbg::cli::incrAddr ADDRESS $increment
                    }
                    return [join $result \n]
                }
                return $ADDRESS
            }
        }
        {NAME} {
            -wrap char
            -expand
        }
        {INDEX SIZE} {
            -header ""
            -align right
            -transform {
                if {$INDEX == {} || $INDEX == 0} {
                    return
                }
                format {<+%#x>} [expr {$INDEX * $SIZE}]
            }
        }

        {VALUE TYPE_NAME TYPE SIZE} {
            -header VALUE
            -transform {
                if {$VALUE == "" && $SIZE > 0 && $TYPE_NAME != "<asciiz>" } {
                    return "<no value>"
                }
                if {$TYPE_NAME == "" && $TYPE == ""} {
                    set TYPE_NAME <binary>
                }
                switch -exact $TYPE_NAME {
                    <binary> {
                        join [mdbg::cli::GroupList [
                            mdbg::cli::Binary::scanByteList $VALUE %.2X
                        ] 16 ] \n
                    }
                    char -
                    "const char" {
                        set char [format %c $VALUE]
                        if {$VALUE < 0} {
                            set VALUE [expr {$VALUE & 0xFF}]
                        } else {
                            if {[string is print $char]} {
                                return "'$char'"
                            }
                            switch -exact $char {
                                "\n" {return {'\n'}}
                                "\r" {return {'\r'}}
                                "\t" {return {'\t'}}
                                "\a" {return {'\a'}}
                                "\b" {return {'\b'}}
                                "\v" {return {'\v'}}
                                "\f" {return {'\f'}}
                            }
                        }
                        set fmt [expr {
                            $char < 0x20 ? {'\%.3o'} : {'\x%.2X'}
                        }]
                        format $fmt $VALUE
                    }
                    <asciiz> {
                        multiLineCStringLiteral $VALUE 32
                    }
                    default {
                        return $VALUE
                    }
                }
            }
        }
        KIND  {
            -skip
        }
        SIZE {
            -align right
        }
        TYPE {
            -skip
        }
        TYPE_NAME {
        }
        CONTEXT_ID {
        }
        TYPE_ID {
            -skip
        }
    }

    oo::objdefine SymInfoDisplay {
        mixin cli::ResultTable
    }

    proc multiLineCStringLiteral {str width} {
        set result [list]
        set len [string length $str]
        set first 0
        set last [expr {$width - 1}]

        while {$first < $len} {
            lappend result [
                format {"%s"} [string map {
                    \n   \\n   \r   \\r   \t   \\t   \a   \\a   \b   \\b   \v   \\v   \f   \\f
                    \x00 \\x00 \x01 \\x01 \x02 \\x02 \x03 \\x03 \x04 \\x04 \x05 \\x05 \x06 \\x06 \x07 \\x07
                    \x08 \\x08 \x09 \\x09 \x0A \\x0A \x0B \\x0B \x0C \\x0C \x0D \\x0D \x0E \\x0E \x0F \\x0F
                    \x10 \\x10 \x11 \\x11 \x12 \\x12 \x13 \\x13 \x14 \\x14 \x15 \\x15 \x16 \\x16 \x17 \\x17
                    \x18 \\x18 \x19 \\x19 \x1A \\x1A \x1B \\x1B \x1C \\x1C \x1D \\x1D \x1E \\x1E \x1F \\x1F
                } [string range $str $first $last] ]
            ]
            incr first $width
            incr last  $width
        }
        join $result \n
    }

    proc displaySymInfo {symInfo} {
        set symInfoList [list]
        fillSymInfoList symInfoList $symInfo
        SymInfoDisplay display $symInfoList
    }

    proc displaySymInfoItemList {itemInfoList} {
        set symInfoList [list]
        foreach symInfo $itemInfoList {
            fillSymInfoList symInfoList $symInfo
        }
        SymInfoDisplay display $symInfoList
    }

    proc cleanupAndFillSymInfoForDisplay {symInfoVar symCache} {
        upvar 1 $symInfoVar symInfo
        dict update symInfo \
            TYPE_ID typeId \
            TYPE_NAME typeName \
            BASES bases \
            FIELDS fields \
            ELEMENTS elements \
        {
            if {![info exists typeName] && [info exists typeId] && $typeId != {}} {
                set typeName [getTypeName $typeId $symCache]
            }

            cleanupAndFillSymInfoListForDisplay bases $symCache
            cleanupAndFillSymInfoListForDisplay fields $symCache
            cleanupAndFillSymInfoListForDisplay elements $symCache

        }

        cleanupSymInfo symInfo
    }

    proc cleanupAndFillSymInfoListForDisplay {listVar symCache} {
        upvar 1 $listVar symList
        if {[info exists symList]} {
            cli::UpdateList symList symInfo {
                cleanupAndFillSymInfoForDisplay symInfo $symCache
            }
        }
    }

    proc fillSymInfoList {resultListVar symInfo} {
        upvar 1 $resultListVar resultList
        lappend resultList $symInfo

        if {[dict exists $symInfo BASES]} {
            foreach baseInfo [dict get $symInfo BASES] {
                fillSymInfoList resultList $baseInfo
            }
        }
        if {[dict exists $symInfo FIELDS]} {
            foreach fieldInfo [dict get $symInfo FIELDS] {
                fillSymInfoList resultList $fieldInfo
            }
        }
        if {[dict exists $symInfo ELEMENTS]} {
            foreach elementInfo [dict get $symInfo ELEMENTS] {
                fillSymInfoList resultList $elementInfo
            }
        }
    }

    proc getSymInfoValue {symInfo} {
        if {[dict exists $symInfo ELEMENTS]} {
            set symList [list]
            foreach elemInfo [dict get $symInfo ELEMENTS] {
                lappend symList [getSymInfoValue $elemInfo]
            }
            return $symList
        }
        if {[dict exists $symInfo BASES]} {
            set symDict [dict create]
            foreach baseInfo [dict get $symInfo BASES] {
                set baseName [dict get $baseInfo TYPE_NAME]
                dict set symDict [string trim $baseName] [getSymInfoValue $baseInfo]
            }
        }
        if {[dict exist $symInfo FIELDS]} {
            if {![info exists symDict]} {
                set symDict [dict create]
            }
            foreach fieldInfo [dict get $symInfo FIELDS] {
                set fieldName [dict get $fieldInfo NAME]
                dict set symDict [string trim $fieldName] [getSymInfoValue $fieldInfo]
            }
        }
        if {[info exists symDict]} {
            return $symDict
        }
        if {[dict exists $symInfo VALUE]} {
            set value [dict get $symInfo VALUE]
            if {[dict exist $symInfo TCL_BINARY_FORMAT]} {
                set tclBinaryFormat [dict get $symInfo TCL_BINARY_FORMAT]
                return [cli::Binary::scanTclFmt $value $tclBinaryFormat]
            }
            return $value
        }

        return
    }

    proc getSymInfoListValue {symInfoList} {
        set symList [list]
        foreach symInfo $symInfoList {
            lappend symList [getSymInfoValue $symInfo]
        }
        set symList
    }

} ; # namespace eval mdbg::cli::Memory