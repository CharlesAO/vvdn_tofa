# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/unsupported/leoncallstack.tcl
# Description: Show Leon callstack during execution as moviDebug Classic
#              (unsupported)
# Created on : Apr 26, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::unsupported {

    namespace eval cli::LeonCallstack {

        namespace path ::mdbg
        cli::TryUseNamespace ::mdbg::myriad2::CoreRegisters

        proc isValidWindow {wim cwp} {
            expr {( $wim & (1 << $cwp) ) == 0}
        }

        # Frame registers layout:
        # L0..L7, I0..I7
        variable IREG 8

        proc getFrameRegs {args} {
            mget -jtag -binary -size [expr 16 * 4] -scan "iu*" {*}$args
        }

        proc getStackRegs {sp} {
            getFrameRegs -addr $sp
        }

        proc getWindowRegs {cwp} {
            getFrameRegs -reg L0($cwp)
        }

        proc hexpr {expr args} {
            cli::hexAddr [uplevel 1 [list expr $expr {*}$args]]
        }

        proc getReturnAddress {frameRegList} {
            variable IREG
            hexpr [lindex $frameRegList $IREG+7] + 8
        }

        proc getFramePointer {frameRegList} {
            variable IREG
            cli::hexAddr [lindex $frameRegList $IREG+6]
        }

        proc isValidAddress {addr} {
            expr {
                ($addr & 0xF0000000) == 0x70000000  ||
                ($addr & 0x80000000) == 0x80000000
            }
        }

        proc callerCWP {cwp} {
            expr {($cwp + 1) & 0x7}
        }

        proc leoncallstack {} {
            set target [target]
            set LEON [cli::State::Reg::contextId2Leon $target]
            if {$LEON == ""} {
                return -code error "$target is not LEON core"
            }
            set PSR [getPSR $LEON]
            set CWP [dict get $PSR CWP]
            set WIM [getWIM $LEON]
            set PC [state -pc]
            set level 0
            set saved 0

            set currentFrameRegs [getWindowRegs $CWP]
            set IP $PC
            set saved 0
            set level 0
            while {1} {
                set FP [getFramePointer $currentFrameRegs]
                set RADDR [getReturnAddress $currentFrameRegs]

                set outList [list "IP=$IP"]
                set sym [sym at -first -quiet -nocomplain $IP]
                if {$sym != ""} {
                    lappend outList $sym
                }
                lassign [cli::LineNumbers::getLineFromAddress $target $IP] dir file line col
                if {$file != ""} {
                    lappend outList [cli::FileLineColumn [file join $dir $file] $line $col]
                }
                lappend outList "FP=$FP" "RADDR=$RADDR"
                cli::PutInfo "[format %2d $level]. [join $outList ", "]"
                incr level

                if {![isValidAddress $RADDR]} {
                    break
                }
                set downCWP [callerCWP $CWP]

                if {!$saved && [isValidWindow $WIM $downCWP] } {
                    set nextFrameRegs [getWindowRegs $downCWP]
                } else {
                    if {![isValidAddress $FP]} {
                        break
                    }
                    set nextFrameRegs [getStackRegs $FP]
                    set saved 1
                }

                set CWP $downCWP
                set IP $RADDR
                set currentFrameRegs $nextFrameRegs
            }
        }

        namespace export leoncallstack

    } ; # namespace eval LeonCallstack

    namespace import cli::LeonCallstack::leoncallstack

} ; # namespace eval mdbg::unsupported
