# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : mget.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    namespace eval cli::Memory {

        proc binaryTypeClass {type} {
            switch -exact $type {
                int - INTEGER {
                    return INT
                }
                unsigned - CARDINAL {
                    return UNSIGNED
                }
                hex {
                    return HEX_UINT
                }
                float - REAL {
                    return FLOAT
                }
                pointer - POINTER {
                    return POINTER
                }
                "" - binary {
                    return BINARY
                }
                default {
                    return -code error "type \"$type\" not supported"
                }
            }
        }

        proc scanData {data binaryClass dataSize} {
            cli::Binary::scanData $data $binaryClass $dataSize
        }

        proc parseEnum {data symInfoVar symCache} {
            upvar 1 $symInfoVar symInfo
            if {[dict exists $symInfo TYPE_ID]} {
                set typeId [dict get $symInfo TYPE_ID]
                set base64 [binary encode base64 $data]
                foreach enumId [$symCache getChildren $typeId] {
                    lassign [$symCache getFieldList $enumId {
                        CLASS NAME VALUE}] symClass enumName enumValue
                    if {$base64 == $enumValue} {
                        if {$symClass ni {{} VALUE}} {
                            cli::Warning "enum value found but has class $symClass"
                        }
                        return $enumName
                    }
                }
            }
            set size [dict get $symInfo SIZE]
            cli::Binary::scanInt $data $size
        }

        proc parseComposite {data structInfoVar symCache maxDepth {level 0}} {
            incr level

            if {$maxDepth < $level} {
                return
            }
            set indent [string repeat {  } $level ]

            upvar 1 $structInfoVar parentInfo
            set parentType [dict get $parentInfo TYPE_ID]
            set parentName [dict get $parentInfo NAME]
            set parentExpression [getExpression $parentInfo]

            set parentFlags [$symCache getField $parentType FLAGS]

            foreach childId [$symCache getChildren $parentType] {
                cli::CheckInterrupt

                lassign [$symCache getFieldList $childId {
                    NAME CLASS TYPE_CLASS OFFSET SIZE TYPE_ID FLAGS
                }] name symClass dataType dataOffset dataSize childTypeId flags

                if {$symClass ni {{} REFERENCE}} {
                    continue
                }
                if {$dataSize == {}} {
                    continue
                }

                set childTypeName [getTypeName $childTypeId $symCache]

                set isBase [expr {{INHERITANCE} in $flags}]
                if {$isBase} {
                    set childName ${indent}::${childTypeName}
                } else {
                    set childName ${indent}.${name}
                }

                set childInfo [dict create \
                    NAME $childName \
                    TYPE_ID $childTypeId TYPE_NAME $childTypeName \
                    TYPE $dataType OFFSET $dataOffset SIZE $dataSize \
                ]

                if {[llength $parentExpression]} {
                    set childExpression [lrange $parentExpression 0 1]
                    set parentExpressionStr [lindex $parentExpression 2]
                    if {!$isBase} {
                        lappend childExpression "($parentExpressionStr).${name}"
                    } else {
                        lappend childExpression "($childTypeName)($parentExpressionStr)"
                    }
                    dict set childInfo EXPRESSION $childExpression
                } else {
                    set childExpression [list]
                }

                if {$dataOffset != {}} {
                    parseSubData $parentInfo $data $dataOffset $dataSize $dataType \
                        childInfo $symCache $maxDepth $level
                } else {
                    # bit field values can only be evaluated via expression
                    if {[llength $childExpression]} {
                        cli::CatchAndPrintError {
                            cli::UsingObj ::tcf::ExprContext childExpr {*}$childExpression {
                                set subData [$childExpr evaluate]
                                parseSubData $parentInfo $subData 0 $dataSize $dataType \
                                    childInfo $symCache $maxDepth $level
                            }
                            dict unset childInfo ADDRESS
                        }
                    }
                }

                if {$isBase} {
                    dict lappend parentInfo BASES $childInfo
                } else {
                    dict lappend parentInfo FIELDS $childInfo
                }
            }
        }

        proc parseArray {data arrayInfoVar symCache maxDepth {level 0}} {
            incr level
            if {$maxDepth < $level} {
                return
            }
            set indent [string repeat {  } $level ]

            upvar 1 $arrayInfoVar parentInfo
            set arrayType [dict get $parentInfo TYPE_ID]
            # set arrayType [$symCache resolveTypedef $arrayType]
            set arrayLength [getArrayLength $parentInfo $symCache]

            set parentExpression [getExpression $parentInfo]

            lassign [$symCache get $arrayType BASE_TYPE ID NAME TYPE_CLASS SIZE] \
                childTypeId childTypeName dataType dataSize

            for {set index 0} {$index < $arrayLength} {incr index} {
                cli::CheckInterrupt
                set dataOffset [expr {$index * $dataSize}]
                set childName "$indent[format {[%d]} $index]"

                set childInfo [dict create \
                    NAME $childName \
                    TYPE_ID $childTypeId TYPE_NAME $childTypeName \
                    TYPE $dataType OFFSET $dataOffset SIZE $dataSize \
                ]

                if {$parentExpression != ""} {
                    dict set childInfo EXPRESSION "($parentExpression)\[index\]"
                }

                parseSubData $parentInfo $data $dataOffset $dataSize $dataType \
                    childInfo $symCache $maxDepth $level

                dict lappend parentInfo ELEMENTS $childInfo
            }
        }

        proc parseSubData {parentInfo data dataOffset dataSize dataType childInfoVar symCache maxDepth level} {
            upvar 1 $childInfoVar childInfo

            if {[dict exists $parentInfo ADDRESS]} {
                set parentAddress [dict get $parentInfo ADDRESS]
                if {![dict exists $childInfo ADDRESS]} {
                    dict set childInfo ADDRESS [cli::hexAddr \
                        [expr {$parentAddress + $dataOffset}]
                    ]
                }
            }

            set subData [string range $data $dataOffset [expr {$dataOffset + $dataSize - 1}]]
            switch -exact $dataType {
                ENUMERATION {
                    set value [cli::Memory::parseEnum $subData childInfo $symCache]
                }
                COMPOSITE {
                    set value [cli::Memory::fillSymTypeName childInfo $symCache]
                    cli::Memory::parseComposite $subData childInfo $symCache $maxDepth $level
                }
                ARRAY {
                    set value [cli::Memory::fillSymTypeName childInfo $symCache]
                    cli::Memory::parseArray $subData childInfo $symCache $maxDepth $level
                }
                default {
                    set binaryClass [cli::Memory::binaryTypeClass $dataType]
                    set value [cli::Memory::scanData $subData $binaryClass $dataSize]
                }
            }
            dict set childInfo VALUE $value
        }
    } ; # namespace eval cli::Memory

    proc mget {args} {
        set memCmd get
        set parser [cli::Memory::ArgumentParser SubCmdParser $memCmd]
        $parser parseCallerArgs 1 $args
        cli::NoErrorStack {
            cli::TrapInterrupt {
                cli::Memory::ArgumentParser setupArgs

                cli::UsingObj [cli::Memory::SymbolCacheClass] symCache {
                    cli::Memory::ArgumentParser createSymInfo
                    cli::Memory::fillSymTypeAddrSize symInfo $symCache
                    cli::Memory::ArgumentParser fillSymInfo

                    set dataSize [dict get $symInfo SIZE]
                    set dataType [dict get $symInfo TYPE]

                    set hasCount [info exists count]
                    if {!$hasCount} {
                        if {[dict exists $symInfo COUNT]} {
                            set hasCount 1
                            set count [dict get $symInfo COUNT]
                            dict unset symInfo COUNT
                        } else {
                            set count 1
                        }
                    }
                    set scalarResult [expr {
                        !$hasCount || $dataType in {ARRAY FUNCTION}
                    }]

                    try {
                        if {$scalarResult} {
                            set result {}
                        } else {
                            set resultList [list]
                            set itemInfoList [list]
                        }

                        for {set index 0} {$index < $count} {incr index} {
                            cli::CheckInterrupt

                            set itemInfo $symInfo
                            dict set itemInfo INDEX $index

                            unset -nocomplain itemResult

                            switch -exact $dataType {
                                ENUMERATION {
                                    set data [cli::Memory::readBinaryValue itemInfo]
                                    if {[string length $data]} {
                                        set itemResult [cli::Memory::parseEnum $data itemInfo $symCache]
                                    }
                                }
                                COMPOSITE {
                                    set itemResult [cli::Memory::fillSymTypeName itemInfo $symCache]
                                    set data [cli::Memory::readBinaryValue itemInfo]
                                    if {[string length $data]} {
                                        cli::Memory::parseComposite $data itemInfo $symCache $maxDepth
                                    }
                                }
                                ARRAY {
                                    set itemResult [cli::Memory::fillSymTypeName itemInfo $symCache]
                                    if {$hasCount} {
                                        set elemSize [cli::Memory::getArrayElemSize $itemInfo $symCache]
                                        dict set itemInfo LENGTH $count
                                        dict set itemInfo SIZE [expr {$elemSize * $count}]
                                        if {$maxDepth < 1} {
                                            set maxDepth 1
                                        }
                                    }
                                    if {$maxDepth > 0} {
                                        set data [cli::Memory::readBinaryValue itemInfo]
                                        if {[string length $data]} {
                                            cli::Memory::parseArray $data itemInfo $symCache $maxDepth
                                        }
                                    }
                                }
                                FUNCTION {
                                    if {[dict get $itemInfo TYPE_ID] == {}} {
                                        dict set itemInfo TYPE_NAME "<function>"
                                    }
                                    set itemResult [dict get $itemInfo ADDRESS]
                                }
                                ASCIIZ {
                                    if {[dict get $itemInfo ASCIIZ_INDIRECT]} {
                                        set data [cli::Memory::readBinaryValue itemInfo]
                                        dict set itemInfo ADDRESS [cli::Memory::scanData $data POINTER 4]
                                    }
                                    set itemResult [cli::Memory::readAsciiZ itemInfo]
                                }
                                default {
                                    set binaryClass [cli::Memory::binaryTypeClass $dataType]
                                    set data [cli::Memory::readBinaryValue itemInfo]
                                    if {[string length $data]} {
                                        set itemResult [cli::Memory::scanData $data $binaryClass $dataSize]
                                    }
                                }
                            }

                            if {[info exists itemResult]} {
                                if {$dataType != {FUNCTION} || $dataSize != 0} {
                                    dict set itemInfo VALUE $itemResult
                                }
                            }

                            if {$index == 0} {
                                if {[dict exists $itemInfo ADDRESS]} {
                                    dict set symInfo ADDRESS [
                                        dict get $itemInfo ADDRESS
                                    ]
                                }
                            } else {
                                dict unset itemInfo ExprObj
                            }
                            if {![info exists itemResult]} {
                                set itemResult {}
                            }
                            if {$scalarResult} {
                                set symInfo $itemInfo
                                dict unset symInfo INDEX
                                set result $itemResult
                                break;
                            } else {
                                lappend resultList $itemResult
                                lappend itemInfoList $itemInfo
                            }
                        }

                        if {$full || !$value} {
                            if {$scalarResult} {
                                cli::Memory::cleanupAndFillSymInfoForDisplay symInfo $symCache
                            } else {
                                cli::Memory::cleanupAndFillSymInfoListForDisplay itemInfoList $symCache
                            }

                            if {$quiet} {
                                return [expr {$scalarResult ? $symInfo : $itemInfoList}]
                            }
                            if {$scalarResult} {
                                cli::Memory::displaySymInfo $symInfo
                            } else {
                                cli::Memory::displaySymInfoItemList $itemInfoList
                            }
                            if {!$value} {
                                return
                            }
                        }
                        if {$scalarResult} {
                            cli::Memory::getSymInfoValue $symInfo
                        } else {
                            cli::Memory::getSymInfoListValue $itemInfoList
                        }
                    } finally {
                        if {$scalarResult} {
                            cli::Memory::cleanupSymInfo symInfo
                        } else {
                            cli::UpdateList itemInfoList itemInfo {
                                cli::Memory::cleanupSymInfo itemInfo
                            }
                        }
                    }
                }  ; # cli::UsingObj
            } ; # cli::TrapInterrupt
        } ; # cli:: NoErrorStack
    }

    shell::autocomplete::addCommand {mget} [cli::Memory::ArgumentParser SubCmdParser get] autocomplete
    cli::Help::Manager add mget -parser [cli::Memory::ArgumentParser SubCmdParser get]

} ; # namespace eval mdbg