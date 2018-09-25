# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : coreRegisters.tcl
# Description: getRegisters ported from C to Tcl with some enhancements
# Created on : October 23, 2015
# Author     : Ancuta Maria IVASCU (ancuta.ivascu@movidius.com)
# ***************************************************************************

namespace eval mdbg::myriad2::CoreRegisters {
    namespace path ::mdbg

    proc isLeonRunning {leon} {
        set valueDSU [getReg32 LEON_${leon}_DSU_CTRL_ADR]
        expr { ( $valueDSU & 0xE40) == 0}
    }

    proc getCmxRstnCtrlValue {} {
        expr {[isRegDefined CMX_RSTN_CTRL_ADR] ? [getReg32 CMX_RSTN_CTRL_ADR]
            : [isRegDefined CMX_RSTN_CTRL    ] ? [getReg32 CMX_RSTN_CTRL    ]
            : [error {register "CMX_RSTN_CTRL[_ADR]" is not defined}]
        }
    }

    proc isShaveRunning {shave} {
        expr {
            !([getReg32 SVU_OSR\($shave\)] & 0xF)
         && ([getCmxRstnCtrlValue] & (1 << $shave))
        }
    }

    proc hexstr {number {width 0}} {
        if {$width <= 0} {
            if {$number == 0} {
                return 0
            } elseif {$number == 1} {
                return 1
            }
            format %#x $number
        } else {
            format "%#.${width}x" $number
        }
    }

    proc binstr {number {width 0} } {
        if {$width <= 0} {
            if {$number == 0} {
                return 0
            } elseif {$number == 1} {
                return 1
            }
            format %#b $number
        } else {
            format "%#.${width}b" $number
        }
    }

    proc maskedBitRange {value low {high {}}} {
        if {$high == {}} {
            set high $low
        }
        expr {
            $value & ( (( 1 << ( $high + 1 - $low ) ) - 1) << $low )
        }
    }

    proc bitrange {value low {high {}}} {
        if {$high == {}} {
            set high $low
        }
        expr {
            ( $value >> $low ) & (( 1 << ( $high + 1 - $low ) ) - 1)
        }
    }

    proc getASR_FIELDS {leon index} {
        set value [getReg32 LEON_${leon}_DSU_ASR${index}_REG_ADR]
        if {$index % 2 == 0} {
            dict create \
                WADDR        [hexstr [maskedBitRange $value 2 31] 8] \
                IF           [bitrange $value  0   ]
        } else {
            dict create \
                WMASK        [hexstr [maskedBitRange $value 2 31] 8] \
                DL           [bitrange $value  1   ] \
                DS           [bitrange $value  0   ]
        }
    }

    proc getASR17 {leon} {
        set value [getReg32 LEON_${leon}_DSU_ASR17_REG_ADR]
        dict create \
            INDEX        [bitrange $value 28 31] \
            CS           [bitrange $value 17   ] \
            CF           [bitrange $value 15 16] \
            DW           [bitrange $value 14   ] \
            SV           [bitrange $value 13   ] \
            LD           [bitrange $value 12   ] \
            FPU          [bitrange $value 10 11] \
            M            [bitrange $value  9   ] \
            V8           [bitrange $value  8   ] \
            NWP          [bitrange $value  5  7] \
            NWIN         [bitrange $value  0  4]
    }

    proc getFSR_FIELDS {leon} {
        set value [getReg32 LEON_${leon}_DSU_FSR_REG_ADR]
        dict create \
            RD        [bitrange $value 30 31] \
            fcc       [bitrange $value 10 11] \
            NS        [bitrange $value 22   ] \
            qne       [bitrange $value 13   ] \
            ftt       [bitrange $value 14 16] \
            cexc      [bitrange $value  0  4] \
            TEM       [bitrange $value 23 27] \
            aexc      [bitrange $value  5  9]
    }

    proc getPSR {core} {
        set value [getReg32 LEON_${core}_DSU_PSR_ADR]
        dict create \
            IMPL          [bitrange $value 28 31] \
            VER           [bitrange $value 24 27] \
            ICC  [binstr [bitrange $value 20 23]] \
            EC            [bitrange $value 13   ] \
            EF            [bitrange $value 12   ] \
            PIL           [bitrange $value  8 11] \
            S             [bitrange $value  7   ] \
            PS            [bitrange $value  6   ] \
            ET            [bitrange $value  5   ] \
            CWP           [bitrange $value  0  4]
    }

    proc getDSU {core} {
        set value [getReg32 LEON_${core}_DSU_CTRL_ADR]
        dict create \
            PW  [bitrange $value 11] \
            HL  [bitrange $value 10] \
            PE  [bitrange $value  9] \
            EB  [bitrange $value  8] \
            EE  [bitrange $value  7] \
            DM  [bitrange $value  6] \
            BZ  [bitrange $value  5] \
            BX  [bitrange $value  4] \
            BS  [bitrange $value  3] \
            BE  [bitrange $value  2] \
            TE  [bitrange $value  1]
    }

    proc getWIM {core} {
        getReg32 LEON_${core}_DSU_WIM_ADR
    }

    proc getTRAP {core} {
        set value [getReg32 LEON_${core}_DSU_TRAP_REG_ADR]
        dict create \
            EM               [bitrange $value 12] \
            TRAPTYPE [hexstr [bitrange $value 4 11] 2]
    }

    proc getValidFloat {result} {
        if {![string is double -strict $result]} {
            return
        }
        if {[regexp -nocase {^(.*)[(].*[)]$} $result  -> nan]} {
            return $nan
        }
        set result
    }

    proc getDoubleFromHex {hexValue1 hexValue2} {
        set binValue1 [cli::Binary::formatUnsigned 4 $hexValue1]
        set binValue2 [cli::Binary::formatUnsigned 4 $hexValue2]
        cli::Binary::scanFloat $binValue2$binValue1 8
    }

    proc getFloatFromHex {hexValue} {
        set binValue [cli::Binary::formatUnsigned 4 $hexValue]
        cli::Binary::scanFloat $binValue 4
    }

    proc getFloat {x} {
        if {($x & 0x7f800000) == 0} {
            if {($x & 0x007fffff) == 0} {
                if {($x & 0x80000000) == 0} {
                    return "0.0"
                }
                return "-0.0"
            } else {
                if {($x & 0x80000000) == 0} {
                    return "Den"
                }
                return "-Den"
            }
        } elseif {($x & 0x7f800000) == 0x7f800000} {
            if {($x & 0x007fffff) == 0} {
                if {($x & 0x80000000) == 0} {
                    return "Inf"
                }
                return "-Inf"
            } else {
                if {($x & 0x400000) == 0} {
                    return "sNaN"
                }
                return "qNaN"
            }
        }
        return [ format %.7g [ getFloatFromHex $x ] ]
    }

    proc getDouble {x_lo x_hi} {
        if {($x_hi & 0x7ff00000) == 0} {
            if {(($x_hi & 0x000fffff) | $x_lo) == 0} {
                if {($x_hi & 0x80000000) == 0} {
                    return "0.0"
                }
                return "-0.0"
            } else {
                if {($x_hi & 0x80000000) == 0} {
                    return "Den"
                }
                return "-Den"
            }
        } elseif {($x_hi & 0x7ff00000) == 0x7ff00000} {
            if {(($x_hi & 0x000fffff) | $x_lo) == 0} {
                if {($x_hi & 0x80000000) == 0} {
                    return "Inf"
                }
                return "-Inf"
            } else {
                if {($x_hi & 0x80000) == 0} {
                    return "sNaN"
                }
                return "qNaN"
            }
        }
        return [ format %.16g [ getDoubleFromHex $x_hi $x_lo ] ]
    }

    proc getLeonRegs {leon {float 0} {asr 0} } {
        set core [string toupper $leon]
        set coreIndex [lsearch -exact {OS RT RT0 RT1 NN} $leon]
        if {$coreIndex < -1} {
            return -code error "invalid core: must be one of the leon cores"
        }
        set bit [lindex { 15 17 -1 -1 -1} $coreIndex]
        if {$bit != -1} {
            # re-read the cpr gen-ctrl-value
            set cprvalue [mdbg::cv::myriad2::getCprGenCtrlValue 1]
            set dsuEnabled [expr { $bit == -1 || ($cprvalue & (1 << $bit)) != 0}]
            if {!$dsuEnabled} {
                return -code error "DSU not enabled"
            }
        }
        dict set regs CORE "LEON"
        if {![isLeonRunning $leon]} {
            dict set regs RUNNING 0
            for {set regIndex 0} {$regIndex < 8} {incr regIndex} {
                dict set regs I${regIndex} [getReg32 LEON_${leon}_REG_I${regIndex}_ADR]
                dict set regs L${regIndex} [getReg32 LEON_${leon}_REG_L${regIndex}_ADR]
                dict set regs O${regIndex} [getReg32 LEON_${leon}_REG_O${regIndex}_ADR]
                dict set regs G${regIndex} [getReg32 LEON_${leon}_REG_G${regIndex}_ADR]
            }
            if {$float != 0 && $leon != "NN"} {
                set prevName ""
                for {set regIndex 0} {$regIndex < 32} { incr regIndex } {
                    set hex [getReg32 LEON_${leon}_REG_F${regIndex}_ADR]
                    set name F${regIndex}
                    dict set regs $name $hex
                    dict set regs ${name}.float [getFloat $hex]
                    if {$prevName != ""} {
                        dict set regs ${prevName}.double [getDouble $prevHex $hex]
                        set prevName ""
                    } else {
                        set prevName $name
                        set prevHex $hex
                    }
                }
                dict set regs FSR [getReg32 LEON_${leon}_DSU_FSR_REG_ADR]
                dict set regs FSR_FIELDS [getFSR_FIELDS $leon]
            }
            if {$asr != 0} {
                dict set regs ASR17 [getReg32 LEON_${leon}_DSU_ASR17_REG_ADR]
                dict set regs ASR17_FIELDS [getASR17 $leon]
                for {set regIndex 24} {$regIndex < 32} {incr regIndex} {
                    dict set regs ASR${regIndex} [getReg32 LEON_${leon}_DSU_ASR${regIndex}_REG_ADR]
                    dict set regs ASR${regIndex}_FIELDS [getASR_FIELDS $leon $regIndex]
                }
            }
        } else {
            dict set regs RUNNING 1
        }
        dict set regs PSR             [getReg32 LEON_${core}_DSU_PSR_ADR]
        dict set regs PSR_FIELDS      [getPSR $leon]
        dict set regs WIM             [getWIM $leon]
        dict set regs TBR             [getReg32 LEON_${leon}_DSU_TBR_ADR ]
        dict set regs Y               [getReg32 LEON_${leon}_DSU_Y_REG_ADR]
        dict set regs PC              [getReg32 LEON_${leon}_DSU_PC_REG_ADR]
        dict set regs NPC             [getReg32 LEON_${leon}_DSU_NPC_REG_ADR]
        dict set regs TRAP            [getTRAP $leon]
    }

    proc getShaveRegs {shave} {
        set maxShaveIndex [expr {[myriad2::getNumberOfShaves [MV_SOC_REV]] - 1}]
        if {$shave < 0 || $shave > $maxShaveIndex} {
            return -code error "invalid shave index: must be between 0 and $maxShaveIndex"
        }
        dict set regs CORE "SHAVE"
        if {![isShaveRunning $shave]} {
            dict set regs RUNNING 0
            for {set regIndex 0} {$regIndex < 32} { incr regIndex } {
                dict set regs I$regIndex [getReg32 SVU_REG_I$regIndex\($shave\)]
                for {set vIndex 0} {$vIndex < 4} { incr vIndex } {
                    dict lappend regs V$regIndex [getReg32 SVU_REG_V$regIndex\_$vIndex\($shave\)]
                }
            }
        } else {
            dict set regs RUNNING 1
        }
        dict set regs IP [getReg32 SVU_TRF\($shave\)]
        dict set regs INEXT [getReg32 SVU_INEXT\($shave\)]
        #set regs
    }

} ; #namespace mdbg::myriad2::CoreRegisters

