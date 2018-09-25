# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : coreRegisters.tcl
# Description: decode and display core registers values
# Created on : April 26, 2016
# Author     : Ancuta Maria IVASCU (ancuta.ivascu@movidius.com)
# ***************************************************************************

namespace eval mdbg::cli::State::Reg {
    namespace path ::mdbg

    cli::TryUseNamespace ::mdbg::myriad2::CoreRegisters

    proc contextId2Leon {contextId} {
        if {[regexp {L(OS|RT(?:[0-1])?|NN)$} $contextId -> leonName]} {
            return $leonName
        }
        return
    }

    proc contextId2Shave {contextId} {

        if {[regexp {S([0-9]+)$} $contextId -> shaveIndex]} {
            return $shaveIndex
        }
        return
    }

    proc getRegs {contextId getFRegs getAsrRegs} {
        set leon [contextId2Leon $contextId]
        if {$leon != {}} {
            return [getLeonRegs $leon $getFRegs $getAsrRegs]
        }
        set shave [contextId2Shave $contextId]
        if {$shave != {} } {
            return [getShaveRegs $shave]
        }

        return -code error "context is not LEON or SHAVE"
    }

    proc missingRegValue {} {
        return " [string repeat - 8] "
    }

    proc getRegFromDict {dict reg} {
        if [dict exists $dict $reg] {
            dict get $dict $reg
        } else {
            missingRegValue
        }
    }

    proc getRegListFromDict {dict reg n} {
        if [dict exists $dict $reg] {
            dict get $dict $reg
        } else {
            return [lrepeat $n [missingRegValue]]
        }
    }

    proc decodeIcc {value} {
        format {[%s]} [decodeIndividualBits $value {
            3 N -
            2 Z -
            1 V -
            0 C -
        } ""]
    }

    proc decodeRD {RD} {
        lindex {
            "near"
            "zero"
            "+inf"
            "-inf"
        } $RD
    }

    proc decodeFcc {fcc} {
        lindex {
            "EQ"
            "LT"
            "GT"
            "UO"
        } $fcc
    }

    proc decodeWim {value} {
        set result {}
        for {set i 7} {$i >= 0} {incr i -1} {
           if {!($value & (1 << $i)) } {
               append result "-"
           } else {
               append result $i
           }
        }
        format {[%s]} $result
    }

    proc decodeIndividualBits {value bsuList {separator { }}} {
        set result [list]
        foreach {bit set unset} $bsuList {
            lappend result [expr {$value & (1 << $bit) ? $set : $unset}]
        }
        join $result $separator
    }

    proc formatPsrFields {psrFields} {
        set result ""
        dict with psrFields {
            foreach {name script} {
                "icc" {decodeIcc $ICC}
                "EF"  {set EF}
                "CWP" {set CWP}
                "ET"  {set ET}
                "PIL" {format "0x%X" $PIL}
                "S"   {set S}
                "PS"  {set PS}
            } {
                append result " $name=[eval $script]"
            }
        }
        return $result;
    }

    proc formatFsrFields {fsrFields} {
        set result ""
        dict with fsrFields {
            foreach {name script} {
                "cexc=" {decodeFsrBits $cexc}
                " TEM=" {decodeFsrBits $TEM}
                " ftt=" {decodeFtt $ftt}
                 "\n"   {string repeat " " 16}
                "aexc=" {decodeFsrBits $aexc}
                " fcc=" {decodeFcc $fcc}
                " NS="  {set NS}
                " RD="  {decodeRD $RD}
                " qne=" {set qne}
            } {
                append result $name [eval $script]
            }
        }
        return $result;
    }

    proc decodeFsrBits {value} {
        format {[%s]} [decodeIndividualBits $value {
            4 NV --
            3 OF --
            2 UF --
            1 DZ --
            0 NX --
        }]
    }

    proc decodeFtt {ftt} {
        lindex {
            "-"
            "ieee754"
            "unifieished"
            "unimp"
            "sequence"
            "?5"
            "?6"
            "?7"
        } $ftt
    }

    proc decodeTT {value} {
        switch -glob [format %02X $value] {
            00 { return "reset" }
            2B { return "write_error"               }
            01 { return "instruction_access_error"  }
            03 { return "privileged_instruction"    }
            02 { return "illegal_instruction"       }
            04 { return "fp_disabled"               }
            24 { return "cp_disabled"               }
            0B { return "watchpoint_detected"       }
            05 { return "window_overflow"           }
            06 { return "window_underflow"          }
            07 { return "mem_address_not_aligned"   }
            08 { return "fp_exception"              }
            09 { return "data_access_exception"     }
            0a { return "tag_overflow"              }
            2a { return "division_by_0"             }
            1? { return "irq[expr {$value - 0x10}]" }
            80 { return "syscall_0x80"              }
            88 { return "syscall_0x88"              }
            81 { return "sw_breakpoint"             }
        }
        if {$value > 0x80} {
            return "sw_trap_0x[format %02X $value]"
        }
        return "unknown"
    }

    proc showRegs {contextId getFRegs getAsrRegs} {
        set regs [getRegs $contextId $getFRegs $getAsrRegs]
        if {[dict get $regs CORE] == "LEON"} {
            if {[dict get $regs RUNNING]} {
                mdbg::cli::Warning "IU Registers not available during execution\n"
            }
            cli::PutfResult {          INS        LOCALS       OUTS      GLOBALS}
            for {set index 0} {$index < 8} {incr index} {
                cli::Putf {%s:    %s  %s  %s  %s} \
                    $index \
                    [getRegFromDict $regs I$index] \
                    [getRegFromDict $regs L$index] \
                    [getRegFromDict $regs O$index] \
                    [getRegFromDict $regs G$index]
            }
            if {$getFRegs} {
                cli::NewLine
                cli::UsingObj cli::GridDisplay grid {
                    0 {
                       -header ""
                       -format %2d:
                    }
                    1 {
                        -header "FLOATS"
                        -align center
                    }
                    2 {
                        -header ""
                        -align right
                    }
                    3 {
                        -align right
                        -header ""
                    }
                } {
                    set table [list]
                    for {set index 0} {$index < 32} {incr index} {
                        set row [list $index]                              ; # 0
                        lappend row [getRegFromDict $regs F$index]         ; # 1
                        lappend row [getRegFromDict $regs F${index}.float] ; # 2
                        lappend row [getRegFromDict $regs F$index.double]  ; # 3
                        lappend table $row
                    }
                    oo::objdefine $grid mixin cli::ListResultTable
                    $grid display $table
                }
                cli::NewLine
                if {[dict exists $regs FSR]} {
                    set FSR [dict get $regs FSR]
                    set fsrFields [dict get $regs FSR_FIELDS]
                    cli::Puts "FSR=${FSR}: [formatFsrFields $fsrFields]"
                } else {
                    cli::Puts "FSR:  --------"
                }
            }
            if {$getAsrRegs} {
                cli::NewLine
                cli::PutfResult {           ASR}
                set strAsr "17:"
                append strAsr " " [dict get $regs ASR17] " \["
                set asrFields [dict get $regs ASR17_FIELDS]
                dict with asrFields {
                    append strAsr "SVT=" $SV " DWT=" $DW " INDEX=" $INDEX " NWIN=" $NWIN " NWP=" $NWP " CS=" $CS " CF=" $CF " LD=" $LD " FPU=" $FPU " M=" $M " V8=" $V8
                }
                append strAsr "]"
                puts $strAsr
                for {set regIndex 24} {$regIndex < 32} {incr regIndex} {
                    set str $regIndex
                    append str ": " [dict get $regs ASR${regIndex}] " \["
                    set asrFields${regIndex} [dict get $regs ASR${regIndex}_FIELDS]
                    if {$regIndex % 2 == 0} {
                        dict with asrFields${regIndex} {
                            append str "WADDR = " $WADDR " IF=" $IF
                        }
                    } else {
                        dict with asrFields${regIndex} {
                            append str "WMASK = " $WMASK " DL=" $DL " DS=" $DS
                        }
                    }
                    append str "]"
                    puts $str
                }
            }
            cli::NewLine
            cli::Puts " PC=[dict get $regs PC]"
            cli::Puts "NPC=[dict get $regs NPC]"

            # ---
            set PSR [dict get $regs PSR]
            set psrFields [dict get $regs PSR_FIELDS]
            # ---
            set WIM [dict get $regs WIM]
            # ---
            set TBR [dict get $regs TBR]
            set TBA [expr {$TBR & ~0xfff}]
            set tt [expr {($TBR>>4)&0xFF}]
            # ---

            cli::Puts "PSR=${PSR}:[formatPsrFields $psrFields]"
            cli::Puts "WIM=${WIM}: invalid=[decodeWim $WIM]"
            cli::Putf "TBR=%s: TBA=0x%08X tt=0x%02X (%s)" $TBR $TBA $tt [decodeTT $tt]
            cli::Puts "  Y=[dict get $regs Y]"
            cli::NewLine
        } else {
            if {[dict get $regs RUNNING]} {
                mdbg::cli::Warning "IU Registers not available during execution"
            } else {
                cli::PutfResult {           IRF        V[0]        V[1]        V[2]        V[3]}
                for {set index 0} {$index < 32} {incr index} {
                    cli::Putf {%-5s  %s  %s  %s  %s  %s} \
                        ${index}: \
                        [getRegFromDict $regs I$index] \
                        {*}[getRegListFromDict $regs V$index 8]
                }
            }
            cli::NewLine
            cli::Putf {IP = %s   INEXT = %s} \
                [dict get $regs IP] \
                [dict get $regs INEXT]
            cli::NewLine
        }
    }
}
