# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/memory/SymInfo.tcl
# Description: Symbol information structure for mget/mset etc.
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    proc getSymInfo {name contextId frameId {pc 0}} {
        if {[string is integer -strict $name]} {
            tailcall createAddrSymInfo $name $contextId
        }
        if {![catch {
            createRegSymInfo $name $contextId
        } result]} {
            return $result
        }
        if {![catch {
            createVarSymInfo $name $frameId $pc
        } result]} {
            return $result
        }
        if {![catch {
            createExprSymInfo $name $frameId
        } result]} {
            return $result
        }
        return -code error "symbol \"$name\" not found"
    }

    proc createAddrSymInfo {addr contextId} {
        dict set result KIND -addr
        dict set result ADDRESS [cli::hexAddr $addr]
    }

    proc createRegSymInfo {regName contextId} {
        variable LOCAL_REGISTER_MAP
        if {$contextId != ""} {
            set target [cli::Target::getTargetNameFromContextId $contextId]
            set localRegName [string toupper $regName]
            if {[dict exists $LOCAL_REGISTER_MAP $target $localRegName]} {
                lassign [dict get $LOCAL_REGISTER_MAP $target $localRegName] regName count
                dict set result NAME $regName
                dict set result JTAG JTAG
                if {$count != {}} {
                    dict set result COUNT $count
                }
            } else {
                unset localRegName
            }
        }
        dict set result KIND -reg
        dict set result ADDRESS [mdbg::getRegAddr $regName]
    }

    proc createIdSymInfo {id symCache} {
        dict set result KIND "-id"
        dict set result SYMBOL_ID $id
        set name [$symCache getField $id NAME]
        if {"VARARG" in [$symCache getField $id FLAGS]} {
            append name "..."
        }
        dict set result NAME $name
    }

    proc createVarSymInfo {name contextId {pc 0}} {
        dict set result KIND [expr {$pc ? "-var" : "-sym"}]
        set sym [cli::Symbols::findByName $contextId $name $pc]
        dict set result SYMBOL_ID [dict get $sym ID]
        dict set result ADDRESS [tcf::getSymbolField $sym ADDRESS]
        dict set result SIZE [tcf::getSymbolField $sym SIZE]
        dict set result CONTEXT_ID $contextId
    }

    proc createExprSymInfo {name contextId {lang {}}} {
        dict set result KIND -expr
        dict set result ExprObj [tcf::ExprContext new $contextId $lang $name]
    }

    proc fillSymTypeAddrSize {symInfoVar symbolCache} {
        upvar 1 $symInfoVar symInfo
        set kind [dict get $symInfo KIND]
        switch -exact $kind {
            -sym -
            -var -
            -id {
                set symId [dict get $symInfo SYMBOL_ID]
                dict set symInfo SIZE [$symbolCache getField $symId SIZE]
                dict set symInfo TYPE [$symbolCache getField $symId TYPE_CLASS]
                dict set symInfo TYPE_ID [$symbolCache getField $symId TYPE_ID]
                lassign [$symbolCache getFieldList $symId {
                    ADDRESS REGISTER VALUE
                }]  address register value
                if {$address != {}} {
                    dict set symInfo ADDRESS $address
                } else {
                    if {$register != {}} {
                        dict set symInfo REGISTER $register
                        if {$value != {}} {
                            set data [cli::Binary::formatBase64 $value]
                            dict set symInfo DATA $data
                        }
                    }
                }
            }
            -expr {
                set exprObj [dict get $symInfo ExprObj]
                dict set symInfo SIZE [$exprObj getField SIZE]
                dict set symInfo TYPE [$exprObj getField CLASS]
                set typeId [$exprObj getType]
                dict set symInfo TYPE_ID $typeId
            }
            -addr - -reg {
                dict set symInfo TYPE hex
                dict set symInfo SIZE 4
            }
            default {
                return -code error "$kind not supported"
            }
        }
    }

    proc getExpression {info} {
        if {[dict exists $info EXPRESSION]} {
            dict get $info EXPRESSION
        } elseif {
                 [dict exists $info ExprObj]
            &&  ![dict exists $info JTAG]
            && (![dict exists $info MEMORY_ID] ||
                 [dict exists $info TARGET_ID] && [dict get $info MEMORY_ID] == [dict get $info TARGET_ID])
            && (![dict exists $info INDEX] || [dict get $info INDEX] == 0)
        } {
            set exprObj [dict get $info ExprObj]
            list [$exprObj getParentID] [$exprObj getLanguage] [$exprObj getExpression]
        }
    }

    proc getSymData {symInfo dataVar} {
        upvar 1 $dataVar data
        if {[dict exists $symInfo DATA]} {
            set data [dict get $symInfo DATA]
            return 1
        }
        unset -nocomplain data
        return 0
    }

    proc getSymAddress {symInfo} {
        if {[dict exists $symInfo ADDRESS]} {
            set addr [dict get $symInfo ADDRESS]
            if {$addr != {}} {
                return $addr
            }
        }
        return -code error "symbol has no address in this context"
    }

    proc cleanupSymInfo {symInfoVar} {
        upvar 1 $symInfoVar symInfo
        if {[info exists symInfo]} {
            if {[dict exists $symInfo ExprObj]} {
                set exprObj [dict get $symInfo ExprObj]
                dict unset symInfo ExprObj
                $exprObj destroy
            }
        }
    }


} ; # namespace eval mdbg::cli::Memory