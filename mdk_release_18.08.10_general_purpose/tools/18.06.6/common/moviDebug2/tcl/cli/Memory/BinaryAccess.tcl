# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : BinaryReadWrite.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    proc readBinaryValue {symInfoVar} {
        upvar 1 $symInfoVar symInfo
        set kind [dict get $symInfo KIND]
        set size [dict get $symInfo SIZE]
        if {$size == ""} {
            return
        }
        set jtag [dict exists $symInfo JTAG]
        if {!$jtag} {
            set targetId [dict get $symInfo TARGET_ID]
            set memContextId [dict get $symInfo MEMORY_ID]
        }
        set index [dict get $symInfo INDEX]
        set offset [expr {$index * $size}]

        switch -exact $kind {
            -addr - -reg -
            -sym  - -var -
            -id {
                if {[getSymData $symInfo data]} {
                    return $data
                } else {
                    dict update symInfo ADDRESS addr {
                        if {![info exists addr]} {
                            return
                        }
                        if {$offset} {
                            cli::incrAddr addr $offset
                        }
                    }
                    if {$jtag} {
                        jtag readBlock $addr $size
                    } else {
                        tcf::Memory::getBytes $memContextId $addr $size
                    }
                }
            }
            -expr {
                if {[dict exists $symInfo ExprObj]} {
                    set exprObj [dict get $symInfo ExprObj]
                }
                if {!$jtag && $targetId == $memContextId && $offset == 0} {
                    set value [$exprObj evaluate]
                    set addr [$exprObj getValueInfoField ADDRESS]
                    dict set symInfo ADDRESS $addr
                    set len [string length $value]
                    if {$len < $size} {
                        if {$addr == {}} {
                            return -code error "cannot read arbitrary size: expression has no address"
                        }
                        append value [tcf::Memory::getBytes $memContextId \
                            [expr {$addr + $len}] \
                            [expr {$size - $len}] \
                        ]
                    } elseif {$len > $size} {
                        set value [string range $value 0 [expr {$size - 1}]]
                    }
                    return $value
                }
                if {[dict exists symInfo ADDRESS]} {
                    set addr [dict get symInfo ADDRESS]
                } else {
                    Using_AddressOf_ExprObj $exprObj addrExpr {
                        set ptrData [$addrExpr evaluate]
                        set ptrSize [string length $ptrData]
                        set addr [cli::Binary::scanPointer $ptrData $ptrSize]
                    }
                }
                if {$offset} {
                    cli::incrAddr addr $offset
                }
                dict set symInfo ADDRESS $addr
                if {$jtag} {
                    jtag readBlock $addr $size
                } else {
                    tcf::Memory::getBytes $memContextId $addr $size
                }
            }
        }
    }

    proc readAsciiZ {symInfoVar} {
        upvar 1 $symInfoVar symInfo
        set jtag [dict exists $symInfo JTAG]
        if {!$jtag} {
            # set targetId [dict get $symInfo TARGET_ID]
            set memContextId [dict get $symInfo MEMORY_ID]
        }
        if {[dict exists $symInfo ADDRESS]} {
            set addr [dict get $symInfo ADDRESS]
        } else {
            switch -exact [dict get $symInfo KIND] {
                -expr {
                    set exprObj [dict get $symInfo ExprObj]
                    Using_AddressOf_ExprObj $exprObj addrExpr {
                        set ptrData [$addrExpr evaluate]
                        set ptrSize [string length $ptrData]
                        set addr [cli::Binary::scanPointer $ptrData $ptrSize]
                        dict set symInfo ADDRESS $addr
                    }
                }
                default {
                    return -code error "<asciiz> location has no address"
                }
            }
        }
        set maxLen [dict get $symInfo ASCIIZ_MAX_LEN]
        set blockLen 64
        set data [binary format a* ""]
        set term [binary format c 0]
        set readLen 0
        while { $maxLen > 0 } {
            set size [expr {$maxLen < $blockLen ? $maxLen : $blockLen - $addr % $blockLen}]
            set block [
                if {$jtag} {
                    jtag readBlock $addr $size
                } else {
                    tcf::Memory::getBytes $memContextId $addr $size
                }
            ];
            set len [string first $term $block]
            if {$len >= 0} {
                set block [string range $block 0 $len-1]
                set maxLen 0
            } else {
                incr maxLen -$size
                incr addr $size
            }
            append data $block
        }
        dict set symInfo SIZE [string length $data]
        encoding convertfrom ascii $data
    }

    proc writeBinaryValue {symInfoVar data} {
        set WRITE_SCRIPTS {
            SET:JTAG  { jtag writeBlock $addr $data }
            SET:MEM   { tcf::Memory::setBytes $memContextId $addr $data }
            FILL:JTAG { jtag fillBlock $addr $fillSize $data }
            FILL:MEM  { tcf::Memory::fillBytes $memContextId $addr $fillSize $data }
        }
        upvar 1 $symInfoVar symInfo
        set kind [dict get $symInfo KIND]
        if {! [set jtag [dict exists $symInfo JTAG]]} {
            set targetId     [dict get $symInfo TARGET_ID]
            set memContextId [dict get $symInfo MEMORY_ID]
        }
        if {[set fill [dict exists $symInfo FILL]]} {
            set fillSize [dict get $symInfo SIZE]
        }
        set MODE [expr {$fill ? "FILL" : "SET" }]:[expr {$jtag ? "JTAG" : "MEM"}]
        set WRITE_SCRIPT [dict get $WRITE_SCRIPTS $MODE]

        switch -exact $kind {
            -addr - -reg -
            -sym  - -var -
            -id {
                set addr [getSymAddress $symInfo]
                uplevel 0 $WRITE_SCRIPT
            }
            -expr {
                set exprObj [dict get $symInfo ExprObj]
                if {![$exprObj canAssign]} {
                    error "expression is not assignable"
                }
                if {!$jtag && $targetId == $memContextId} {
                    if 1 {
                        $exprObj assign $data
                    } else {
                        # TODO check if this is required
                        set size [dict get $symInfo SIZE]
                        set dataSize [string length $data]
                        if {$size >= $dataSize} {
                            $exprObj assign $data
                            if {$size == $dataSize} {
                                return
                            }
                            set data [string range $data $size end]
                            set OFFSET $size
                        }
                    }
                    return
                }

                Using_AddressOf_ExprObj $exprObj addrExpr {
                    set ptrData [$addrExpr evaluate]
                    set ptrSize [string length $ptrData]
                    set addr [cli::Binary::scanPointer $ptrData $ptrSize]
                    dict set symInfo ADDRESS $addr
                    if [info exists OFFSET] {
                        cli::incrAddr addr $OFFSET
                    }
                    uplevel 0 $WRITE_SCRIPT
                }
            }
        }
    }

    proc Using_AddressOf_ExprObj {exprObj addrExprName script} {
        set parentId                   [$exprObj getParentID]
        set language                   [$exprObj getLanguage]
        set expression [format {&(%s)} [$exprObj getExpression]]

        uplevel 1 [list \
            cli::UsingObj ::tcf::ExprContext $addrExprName \
            $parentId $language $expression \
            $script
        ]
    }

} ; # namespace eval mdbg::cli::Memory
