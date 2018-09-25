# ***************************************************************************
# Copyright (C) 2018 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : myriad2/cpr.tcl
# Description: CPR command ported from MA2100/MA2x5x bringup
# ***************************************************************************

# TODO: This module makes an assumption that the SoC input clocks are the same as the ones used on MV0182/MV0212
# In future it will be based on board spec configured in projectSettings.tcl in each app scripts folder
#namespace eval mdbg{

namespace eval mdbg::cpr {

    namespace path ::mdbg

    set global_refClkHz 32768
    set global_osc1_in 12000
    set global_osc2_in 27000

    proc makeSureAONIsAccessible {} {
        if { [isMA2x5x]} {
            setBit [const CPR_CLK_EN1_ADR] [expr [const CSS_AON] - 32]
        } elseif { [isMA2100]} {
            # nothing needs to be done in this case
        } else {
            return -code error "makeSureAONIsAccessible needs to be customized for MV_SOC_REV= [MV_SOC_REV]"
        }
    }

    proc getOscInKhz {refClk} {
        if {$refClk == 0} {
            variable global_osc1_in
            variable global_refClkHz
            if { [isMA2x5x]} {
                makeSureAONIsAccessible
                if {[expr [peek [const AON_RETENTION0_ADR]] & (1 << 6)]} {
                    # We are using the 32khz clock as a source
                    return [expr $global_refClkHz / 1000]
                } else {
                    return $global_osc1_in
                }
            } elseif { [isMA2100]} {
                return $global_osc1_in
            }
            return -code error "getOscInKhz needs to be customized for MV_SOC_REV= [MV_SOC_REV]"
        } else {
            variable global_osc2_in
            return $global_osc2_in
        }
    }

    proc fMhz {frequency} {
        return "[format %6.6g $frequency]Mhz"
    }

    proc calcDividedClockMaster {inputClk divider} {
        # master divider is 10-bit only
        set numerator   [extractBits $divider 16 25]
        set denominator [extractBits $divider  0  9]
        if {$denominator == 0 || $numerator == 0 || $numerator >= $denominator} {
            set outputClk $inputClk
        } else {
            set outputClk [expr double($inputClk * $numerator) / $denominator]
        }
        return $outputClk
    }

    proc calcDividedClockAux {inputClk divider} {
        # aux divider is 12-bit
        set numerator   [extractBits $divider 16 27]
        set denominator [extractBits $divider  0 11]
        if {$denominator == 0 || $numerator == 0 || $numerator >= $denominator} {
            set outputClk $inputClk
        } else {
            set outputClk [expr double($inputClk * $numerator) / $denominator]
        }
        return $outputClk
    }

    proc getPllState {intPDown intBypass extPllBypass pllLocked pllPending pllTimeout} {
        set lockState "UNKNOWN"
        if {$pllPending} {
            set lockState "PENDING"
        } elseif {$intPDown} {
            set lockState "PWD DWN"
        } elseif {$pllTimeout} {
            set lockState "TIMEOUT"
        } elseif {$pllLocked == 1} {
            set lockState "LOCKED "
        }
        if {$intBypass == 0 && $extPllBypass == 0} {
            set bypassState "ENABLED"
        } else {
            set bypassState "BYPASS "
        }
        return "($lockState,$bypassState)"
    }

    proc decodePll {oscIn pllCtrl pllStatus NF_ref NR_ref NO_ref FVCO_ref pllFreq_ref intBypass_ref intPDown_ref pllLocked_ref pllPending_ref pllTimeout_ref} {
        upvar $NF_ref NF
        upvar $NR_ref NR
        upvar $NO_ref NO
        upvar $FVCO_ref FVCO
        upvar $pllFreq_ref pllFreq
        upvar $intBypass_ref  intBypass
        upvar $intPDown_ref   intPDown
        upvar $pllLocked_ref  pllLocked
        upvar $pllPending_ref  pllPending
        upvar $pllTimeout_ref  pllTimeout

        set fdivRaw   [extractBits $pllCtrl  11 17]
        set divinRaw  [extractBits $pllCtrl   6 10]
        set divoutRaw [extractBits $pllCtrl   4  5]
        set bandRaw   [extractBits $pllCtrl   2  3]
        set intBypass [extractBits $pllCtrl   1  1]
        set intPDown  [extractBits $pllCtrl   0  0]
        set pllPending [expr ![extractBits $pllStatus 1  1]]
        if {!$pllPending} {
            set pllLocked [extractBits $pllStatus 0  0]
            set pllTimeout [extractBits $pllStatus 2  2]
        } else {
            set pllLocked 0
            set pllTimeout 0
        }

        set NF [expr  2 * ($fdivRaw  + 1)]
        set NR [expr $divinRaw + 1]
        set NO [expr pow(2,$divoutRaw)]

        if {$pllLocked} {
            set FVCO [expr $oscIn * $NF / $NR / 1000]
            set pllFreq  [expr $FVCO / $NO]
        } else {
            set FVCO 0
            set pllFreq 0
        }

        return pllFreq
    }

    proc generatePllValue {NF NO NR BAND intBP intPD {src 0}} {
        set fdivRaw   [expr ($NF /2) -1]
        #  puts "fdivRaw $fdivRaw"
        set divinRaw  [expr $NR - 1]
        #  puts "divinRaw $divinRaw"
        set divoutRaw [expr {round ([log2 $NO])}]
        #  puts "divoutRaw $divoutRaw"

        set pllConfig 0
        set pllConfig [setBitRange $pllConfig 0  0  $intPD]
        set pllConfig [setBitRange $pllConfig 1  1  $intBP]
        set pllConfig [setBitRange $pllConfig 2  2  $BAND]
        set pllConfig [setBitRange $pllConfig 4  5  $divoutRaw]
        set pllConfig [setBitRange $pllConfig 6  10 $divinRaw]
        set pllConfig [setBitRange $pllConfig 11 17 $fdivRaw]
        set pllConfig [setBitRange $pllConfig 18 18 $src]

        #  puts "pllConfig: [hex $pllConfig]"

        return $pllConfig
    }

    proc setPllRaw {pllNum configValue {keepPll0Bypassed 0} {quiet 0}} {
        set extPllBypassAdr [const CPR_CLK_BYPASS_ADR]
        if {$pllNum == 0} {
            set pllCtrlAdr        [const CPR_PLL_CTRL0_ADR]
            set pllStatusAdr      [const CPR_PLL_STAT0_ADR]
            set extBypassBitNum   0
        } else {
            set pllCtrlAdr        [const CPR_PLL_CTRL1_ADR]
            set pllStatusAdr      [const CPR_PLL_STAT1_ADR]
            set extBypassBitNum   1
        }
        setBit $extPllBypassAdr $extBypassBitNum
        if {!$quiet} { puts "$pllCtrlAdr $configValue" }
        poke $pllCtrlAdr $configValue
        set stat [peek $pllStatusAdr]
        set timeoutCounter 100
        while {$timeoutCounter >= 0 && !($stat & 2)} {
            incr timeoutCounter -1
            set stat [peek $pllStatusAdr]
        }
        if {$timeoutCounter <= 0 || !($stat & 1)} {
            puts "WARNING: timeout setting pll!"
        } else {
            if {!$keepPll0Bypassed} {
                clearBit $extPllBypassAdr $extBypassBitNum
            }
        }
        if {!$keepPll0Bypassed} {
            if {!$quiet} { pll $pllNum }
        }
    }

    proc setPllFreq {pllNum freqMhz {src 0} {keepPll0Bypassed 0} {quiet 0}} {
        if {$pllNum == 0 && $src != 0} {
            return -code error "ERROR: Invalid source for PLL0"
        }
        makeSureAONIsAccessible
        if {$pllNum == 0} {
            if { [isMA2x5x]} {
                # make sure osc0 input is enabled
                setBit  [const AON_RETENTION0_ADR] 15
                # make sure the first pll input is not the 32khz refclock input
                clearBit [const AON_RETENTION0_ADR] 6
            } elseif { ![isMA2100]} {
                return -code error "setPllFreq needs to be customized for MV_SOC_REV= [MV_SOC_REV]"
            }
        }
        set oscIn [getOscInKhz $src]
        set achieved 0
        set conf [calculatePllConfigValue $oscIn [expr $freqMhz * 1000] achieved]
        if {$conf == 0} {
            if {!$quiet} { puts "setPllFreq: Invalid PLL frequency" }
        } else {
            if {!$quiet} { puts "setPllFreq: achieved frequency: $achieved" }
            set conf [setBitRange $conf 18 18 $src]
            if {$src} {
                # if the source is pll1, then enable osc1
                if {isMA2100} {
                    setBit [const CPR_RETENTION6_ADR]  3
                } else {
                    setBit [const AON_RETENTION0_ADR] 18
                }
            }
            setPllRaw $pllNum $conf $keepPll0Bypassed $quiet
        }
    }

    proc setSysFreq {freqMhz {keepPll0Bypassed 0} {quiet 0}} {
        set sysDividerAdr   [const CPR_CLK_DIV_ADR]
        set denom 1
        while {$freqMhz < 100 && $denom < 1024} {
            set denom [expr $denom * 2]
            set freqMhz [expr $freqMhz * 2]
        }
        if {$denom >= 1024} {
            puts "ERROR: too small system frequency. Cannot make master divider denominator high enough to satisfy minimum PLL fVCO frequency constraint"
            return
        }
        poke $sysDividerAdr [expr $denom | 0x00010000]
        setPllFreq 0 $freqMhz 0 $keepPll0Bypassed $quiet
    }

    proc getPllFreqMhz {pllNum} {

        set extPllBypassRaw [peek [const CPR_CLK_BYPASS_ADR]]
        if {$pllNum == 0} {
            set pllCtrl      [peek [const CPR_PLL_CTRL0_ADR]]
            set pllStatus    [peek [const CPR_PLL_STAT0_ADR]]
            set extPllBypass [extractBits $extPllBypassRaw  0 0]
        } else {
            set pllCtrl      [peek [const CPR_PLL_CTRL1_ADR]]
            set pllStatus    [peek [const CPR_PLL_STAT1_ADR]]
            set extPllBypass [extractBits $extPllBypassRaw  1 1]
        }

        set NF 0
        set NR 0
        set NO 0
        set FVCO 0
        set pll 0
        set intBypass 0
        set intPDown 0
        set pllLocked 0
        set pllPending 1
        set pllTimeout 0

        set refClkId 0
        if {$pllNum == 1} {
            if {$pllCtrl & (1 << 18)} {
                set refClkId 1
            }
        }
        set oscIn        [getOscInKhz $refClkId]

        decodePll $oscIn $pllCtrl $pllStatus NF NR NO FVCO pll intBypass intPDown pllLocked pllPending pllTimeout
        if { $intBypass == 0 && $extPllBypass == 0 && $pllLocked == 1 } {
            return $pll
        } else {
            return [expr $oscIn / 1000.]
        }
    }

    proc getSysFreqMhz {} {
        set sysDivider   [peek [const CPR_CLK_DIV_ADR]]
        set sysFreq   [calcDividedClockMaster [getPllFreqMhz 0] $sysDivider]
        return $sysFreq
    }

    proc compareFirstListItem {a b} {
        set a [lindex $a 0]
        set b [lindex $b 0]
        if {$a < $b} {
            return -1
        } elseif {$a > $b} {
            return 1;
        } else {
            return 0;
        }
    }

    proc listPllConfigValues {oscInKHz {src 0}} {
        # Constraints for PGN28HPM18MF3000 using high band
        set DRVCPR_BS1_MINFREFK         25000
        set DRVCPR_BS1_MAXFREFK         50000
        set DRVCPR_BS1_MINFVCOK         1500000
        set DRVCPR_BS1_MAXFVCOK         3000000
        set DRVCPR_BS1_MINFOUTK         187500
        set DRVCPR_BS1_MAXFOUTK         3000000

        # Constraints for PGN28HPM18MF3000 using low band
        set DRVCPR_BS0_MINFREFK         10000
        set DRVCPR_BS0_MAXFREFK         50000
        set DRVCPR_BS0_MINFVCOK         800000
        set DRVCPR_BS0_MAXFVCOK         1600000
        set DRVCPR_BS0_MINFOUTK         100000
        set DRVCPR_BS0_MAXFOUTK         1600000

        # Standard limits for PGN28HPM18MF3000
        set DRVCPR_MINFEEDBACK          16
        set DRVCPR_MAXFEEDBACK          160
        set DRVCPR_MINIDIV              1
        set DRVCPR_MAXIDIV              33
        set DRVCPR_MINODIV              0
        set DRVCPR_MAXODIV              4

        set result {}

        for { set inDiv $DRVCPR_MINIDIV } { $inDiv < $DRVCPR_MAXIDIV } { incr inDiv 1 } {
            set inDivVal [expr $inDiv - 1]
            set fRef [expr (1.0 * $oscInKHz) / $inDiv]
            set bandVal 0
            # Check low band constraint
            if {($fRef >= $DRVCPR_BS0_MINFREFK) && ($fRef <= $DRVCPR_BS0_MAXFREFK) } {
                for { set fbDiv $DRVCPR_MINFEEDBACK } { $fbDiv <= $DRVCPR_MAXFEEDBACK } { set fbDiv [expr $fbDiv + 2] } {
                    set fbDivVal [expr ($fbDiv / 2) - 1]
                    set fVco [expr $fRef  * $fbDiv]
                    if {($fVco <= $DRVCPR_BS0_MAXFVCOK) && ($fVco >= $DRVCPR_BS0_MINFVCOK)} {
                        for { set outDivVal $DRVCPR_MINODIV } { $outDivVal < $DRVCPR_MAXODIV } { incr outDivVal } {
                            set outDivReal [expr 1 << $outDivVal]
                            set fOut [expr $fVco / $outDivReal]
                            set pllCtrlVal [expr     \
                                ( $inDivVal  << 6)  | \
                                ( $fbDivVal  << 11) | \
                                ( $outDivVal << 4 ) | \
                                ( $bandVal   << 2)  | \
                                ( $src       << 18)]
                            lappend result [list $fOut $src $pllCtrlVal]
                        }
                    }
                }
            }
            set bandVal 1
            # Check high band constraint
            if {($fRef >= $DRVCPR_BS1_MINFREFK) && ($fRef <= $DRVCPR_BS1_MAXFREFK)} {
                for { set fbDiv $DRVCPR_MINFEEDBACK } { $fbDiv <= $DRVCPR_MAXFEEDBACK } { set fbDiv [expr $fbDiv + 2] } {
                    set fbDivVal [expr ($fbDiv / 2) - 1]
                    set fVco [expr $fRef  * $fbDiv]
                    if {($fVco <= $DRVCPR_BS1_MAXFVCOK) && ($fVco >= $DRVCPR_BS1_MINFVCOK)} {
                        for { set $outDivVal $DRVCPR_MINODIV } { $outDivVal < $DRVCPR_MAXODIV } {incr outDivVal } {
                            set outDivReal [expr 1 << $outDivVal]
                            set fOut [expr $fVco / $outDivReal]
                            set pllCtrlVal [expr     \
                                ( $inDivVal  << 6)  | \
                                ( $fbDivVal  << 11) | \
                                ( $outDivVal << 4 ) | \
                                ( $bandVal   << 2)  | \
                                ( $src       << 18)]
                            lappend result [list $fOut $src $pllCtrlVal]
                        }
                    }
                }
            }
        }

        set result [lsort -command compareFirstListItem $result]
        return $result
    }

    proc __possible_aux_freq_add_more {pos_frequencies_name partial_res source_freqs num den minFreqKhz maxFreqKhz} {
        upvar $pos_frequencies_name pos_frequencies
        foreach pllsett $source_freqs {
            set pllfreq [lindex $pllsett 0]
            if {$pllfreq < $minFreqKhz} { continue }
            if {$pllfreq > $maxFreqKhz} { continue }
            set pllsrc  [lindex $pllsett 1]
            set pllconf [lindex $pllsett 2]
            set auxfreq [expr $pllfreq * $num / $den]

            set found 0
            if {[llength [dict get $pos_frequencies $auxfreq]] != 0} {set found 1}
            if {!$found} {
                lappend partial_res [list $auxfreq $pllfreq $pllsrc $pllconf $num $den]
                dict set pos_frequencies {*}[list $auxfreq y]
            }
        }
        return $partial_res
    }

    proc listPossibleAuxFreq {{maxNum 6} {maxDen 6} {minFreqKhz 0.0} {maxFreqKhz 3000000.0} {generateNon50Duty 1}} {
        set minFreqKhz [expr $minFreqKhz + 0.0]
        set maxFreqKhz [expr $maxFreqKhz + 0.0]
        set oscIn0 [getOscInKhz 0]
        set src0res [listPllConfigValues $oscIn0 0]
        set oscIn1 [getOscInKhz 1]
        set src1res [listPllConfigValues $oscIn1 1]
        set pllores [concat $src0res $src1res [list [list $oscIn0 0 3] [list $oscIn1 1 [expr 3 | (1 << 18)]]]]

        set auxres [list]

        set auxres [__possible_aux_freq_add_more pos_frequencies $auxres $pllores 1 1 $minFreqKhz $maxFreqKhz]
        # First loop over beautiful dividers (constant period, 50% duty cycle)
        for {set den 2} {$den <= $maxDen} {set den [expr $den + 2]} {
            set auxres [__possible_aux_freq_add_more pos_frequencies $auxres $pllores 1 $den [expr $minFreqKhz * $den] [expr $maxFreqKhz * $den]]
        }

        if {$generateNon50Duty} {
            # Now loop over dividers that are constant period but not necessarily 50% duty cycle
            for {set den 3} {$den <= $maxDen} {set den [expr $den + 2]} {
                set auxres [__possible_aux_freq_add_more pos_frequencies $auxres $pllores 1 $den [expr $minFreqKhz * $den] [expr $maxFreqKhz * $den]]
            }

            for {set num 2} {$num <= $maxNum} {incr num} {
                for {set den [expr $num + 1]} {$den <= $maxDen} {incr den} {
                    set auxres [__possible_aux_freq_add_more pos_frequencies $auxres $pllores $num $den  [expr $minFreqKhz * $den / $num] [expr $maxFreqKhz * $den / $num]]
                }
            }
        }

        set auxres [lsort -command compareFirstListItem $auxres]
        return $auxres
    }

    proc iteratePossibleAuxFreq { auxClock command {maxNum 6} {maxDen 6} {minFreqKhz 0.0} {maxFreqKhz 3000000.0} {generateNon50Duty 1}} {
        set possibleFrequencies [listPossibleAuxFreq $maxNum $maxDen $minFreqKhz $maxFreqKhz $generateNon50Duty]
        foreach freqsett $possibleFrequencies {
            set auxfreq [lindex $freqsett 0]
            set pllfreq [lindex $freqsett 1]
            set pllsrc [lindex $freqsett  2]
            set pllconf [lindex $freqsett 3]
            set num [lindex $freqsett     4]
            set den [lindex $freqsett     5]
            if {($pllconf & 3 ) == 3} {
                if {$pllsrc} {
                    cpr auxClockSet $auxClock REFCLK1 $num $den
                } else {
                    cpr auxClockSet $auxClock REFCLK0 $num $den
                }
            } else {
                setPllFreq 1 [expr $pllfreq/1000.] $pllsrc
                cpr auxClockSet $auxClock PLL1 $num $den
            }

            eval $command
        }
    }

    proc calculatePllConfigValue {oscIn sysClkDesired {pAchievedFreqKhzVarName ""}} {
        if { $pAchievedFreqKhzVarName != ""} {
            upvar $pAchievedFreqKhzVarName pAchievedFreqKhz
        }
        # Constraints for PGN28HPM18MF3000 using high band
        set DRVCPR_BS1_MINFREFK         25000
        set DRVCPR_BS1_MAXFREFK         50000
        set DRVCPR_BS1_MINFVCOK         1500000
        set DRVCPR_BS1_MAXFVCOK         3000000
        set DRVCPR_BS1_MINFOUTK         187500
        set DRVCPR_BS1_MAXFOUTK         3000000

        # Constraints for PGN28HPM18MF3000 using low band
        set DRVCPR_BS0_MINFREFK         10000
        set DRVCPR_BS0_MAXFREFK         50000
        set DRVCPR_BS0_MINFVCOK         800000
        set DRVCPR_BS0_MAXFVCOK         1600000
        set DRVCPR_BS0_MINFOUTK         100000
        set DRVCPR_BS0_MAXFOUTK         1600000

        # Standard limits for PGN28HPM18MF3000
        set DRVCPR_MINFEEDBACK          16
        set DRVCPR_MAXFEEDBACK          160
        set DRVCPR_MINIDIV              1
        set DRVCPR_MAXIDIV              33
        set DRVCPR_MINODIV              0
        set DRVCPR_MAXODIV              4

        #u32 inDivBk = 0, feedbackDivBk = 0, outDivBk = 0, bandBk = 0;
        # 1 GHz difference:
        set diffBk 1000000

        for { set inDiv $DRVCPR_MINIDIV } { $inDiv < $DRVCPR_MAXIDIV } { incr inDiv 1 } {
            set fRef [expr $oscIn / $inDiv]
            # Check low band constraint
            if {($fRef >= $DRVCPR_BS0_MINFREFK) && ($fRef <= $DRVCPR_BS0_MAXFREFK) } {
                for { set outDiv $DRVCPR_MINODIV } { $outDiv < $DRVCPR_MAXODIV } { incr outDiv } {
                    set fVco [expr $sysClkDesired  * (1<<$outDiv)]
                    if {($fVco <= $DRVCPR_BS0_MAXFVCOK) && ($fVco >= $DRVCPR_BS0_MINFVCOK)} {
                        # The following calcuation attempts to find a feedback value which is close to optimum.
                        # However the integer division is imprecise and is rounded down
                        # Furthermore the LSB is knocked off as feedback values must be even
                        set feedbackEstimate [expr int(($sysClkDesired * (1 << $outDiv) * $inDiv) / $oscIn) & 0xFFFFFFFE ]
                        # As we rounded down, we should also try the next higher feedback value as it might be closer to the target
                        for { set feedbackDiv $feedbackEstimate } { $feedbackDiv <= $feedbackEstimate + 2 } { incr feedbackDiv 2 } {
                            if {($feedbackDiv >= $DRVCPR_MINFEEDBACK) && ($feedbackDiv <= $DRVCPR_MAXFEEDBACK)} {
                                set freq [expr $oscIn * $feedbackDiv / ((1<<$outDiv) *$inDiv)]

                                if { $sysClkDesired > $freq } {
                                    set diff [expr $sysClkDesired - $freq]
                                } else {
                                    set diff [expr $freq - $sysClkDesired]
                                }
                                # This will make sure that if there is the same result for bands 0 and 1, band 0 settings will be used as it offers best jitter.
                                if {$diff <= $diffBk} {
                                    # If and only if we have a candidate we must repeat the FVco checks
                                    # as previously this was only sanity checked based on estimated value rather than achieved.
                                    set fVco [expr $freq  * (1<<$outDiv)]
                                    #DPRINTF2("\nPLL_Calc: Try bs:0 fb:%d iDiv:%d outDiv:%d fDes:%d fVco:%d diff:%d\n",feedbackDiv,inDiv,outDiv,sysClkDesired,fVco,diff);
                                    if { ($fVco <= $DRVCPR_BS0_MAXFVCOK) && ($fVco >= $DRVCPR_BS0_MINFVCOK) } {
                                        set inDivBk $inDiv
                                        set feedbackDivBk $feedbackDiv
                                        set outDivBk $outDiv
                                        set bandBk 0
                                        set diffBk $diff
                                    }
                                }
                            }
                        }
                    }
                }
            }
            # Check high band constraint
            if {($fRef >= $DRVCPR_BS1_MINFREFK) && ($fRef <= $DRVCPR_BS1_MAXFREFK)} {
                for { set $outDiv $DRVCPR_MINODIV } { $outDiv < $DRVCPR_MAXODIV } {incr outDiv } {
                    set fVco [expr $sysClkDesired  * (1<<$outDiv)]
                    if {($fVco <= $DRVCPR_BS1_MAXFVCOK) && ($fVco >= $DRVCPR_BS1_MINFVCOK)} {
                        # The following calcuation attempts to find a feedback value which is close to optimum.
                        # However the integer division is imprecise and is rounded down
                        # Furthermore the LSB is knocked off as feedback values must be even
                        set feedbackEstimate [expr int(($sysClkDesired * (1 << $outDiv) * $inDiv) / $oscIn) & 0xFFFFFFFE ]
                        # As we rounded down, we should also try the next higher feedback value as it might be closer to the target
                        for {set feedbackDiv $feedbackEstimate} {$feedbackDiv <= $feedbackEstimate + 2} {incr feedbackDiv 2 } {
                            if {($feedbackDiv >= $DRVCPR_MINFEEDBACK) && ($feedbackDiv <= $DRVCPR_MAXFEEDBACK)} {
                                set freq [ expr $oscIn * $feedbackDiv / ((1<<$outDiv) * $inDiv) ]

                                if {$sysClkDesired > $freq} {
                                    set diff [expr $sysClkDesired - $freq ]
                                } else {
                                    set diff [expr $freq - $sysClkDesired ]
                                }

                                if {$diff < $diffBk} {
                                    # If and only if we have a candidate we must repeat the FVco checks
                                    # as previously this was only sanity checked based on estimated value rather than achieved.
                                    set fVco [expr $freq  * (1<<$outDiv) ]
                                    #DPRINTF2("\nPLL_Calc: Try bs:1 fb:%d iDiv:%d outDiv:%d fDes:$%d fVco:%d diff:%d\n",feedbackDiv,inDiv,outDiv,sysClkDesired,fVco,diff);
                                    if {($fVco <= $DRVCPR_BS1_MAXFVCOK) && ($fVco >= $DRVCPR_BS1_MINFVCOK)} {
                                        set inDivBk $inDiv
                                        set feedbackDivBk $feedbackDiv
                                        set outDivBk $outDiv
                                        set bandBk 1
                                        set diffBk $diff
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        set maxDeviation [expr $oscIn + 1]
        if {$diffBk > $maxDeviation} {
            set pAchievedFreqKhz 0
            # don't set, difference too big
            return 0
        }

        set pllCtrlVal [expr \
            ( ($inDivBk - 1) << 6)       | \
            ( (($feedbackDivBk >> 1) - 1) << 11) | \
            ( $outDivBk << 4 )         | \
            ( $bandBk << 2)]

        set freq [expr $oscIn * $feedbackDivBk / ((1<<$outDivBk) * $inDivBk)]

        set pAchievedFreqKhz $freq
        return $pllCtrlVal
    }

    proc pll {{pllNum "all"}} {
        if {$pllNum == "all"} {
            pll 0
            pll 1
            return
        }
        if {($pllNum != 0) && ($pllNum != 1)} {
            puts "ERROR: please use: 'pll 0' or 'pll 1'"
            return
        }
        set extPllBypassRaw [peek [const CPR_CLK_BYPASS_ADR]]
        set sysDivider   [peek [const CPR_CLK_DIV_ADR]]
        if {$pllNum == 0} {
            set pllCtrl      [peek [const CPR_PLL_CTRL0_ADR]]
            set pllStatus    [peek [const CPR_PLL_STAT0_ADR]]
            set extPllBypass [extractBits $extPllBypassRaw  0 0]
        } else {
            set pllCtrl      [peek [const CPR_PLL_CTRL1_ADR]]
            set pllStatus    [peek [const CPR_PLL_STAT1_ADR]]
            set extPllBypass [extractBits $extPllBypassRaw  1 1]
        }
        set refClkNum 0
        if {$pllNum == 1} {
            if {$pllCtrl & (1 << 18)} {
                set refClkNum 1
            }
        }
        set oscIn        [getOscInKhz $refClkNum]

        set NF 0
        set NR 0
        set NO 0
        set FVCO 0
        set pll 0
        set intBypass 0
        set intPDown 0
        set pllLocked 0
        set pllPending 1
        set pllTimeout 0

        decodePll $oscIn $pllCtrl $pllStatus NF NR NO FVCO pll intBypass intPDown pllLocked pllPending pllTimeout
        set oscInMhz [expr $oscIn / 1000.]
        if {$pllLocked} {
            set frefMhz [expr $oscInMhz / $NR]
        } else {
            set frefMhz 0
        }

        puts "PLL$pllNum Status: [getPllState $intPDown $intBypass $extPllBypass $pllLocked $pllPending $pllTimeout]    PLL Internals: <[format 0x%08X $pllCtrl]> "
        puts "--------------                    ---------------------------             "
        puts "FIN          : [fMhz $oscInMhz]          NF: [format %3d $NF  ]  (Feedback  )"
        puts "FREF         : [fMhz $frefMhz ]          NO: [format %3.0g $NO]  (Output Div)"
        puts "FVCO         : [fMhz $FVCO    ]          NR: [format %3d $NR  ]  (Input  Div)"
        puts "PLL Freq     : [fMhz $pll     ]          Source: refclk$refClkNum / osc[expr $refClkNum + 1]"
        if {$pllNum == 0} {
            # PLL0 defines the system clock so let's print information about that too.
            set numerator   [extractBits $sysDivider 16 25]
            set denominator [extractBits $sysDivider  0  9]
            puts "System Freq  : [fMhz [getSysFreqMhz ]]          MasterDiv: $numerator/$denominator"
        }
        puts ""
    }

    proc getAuxClockControlAddress {clk} {
        if {[constExists CPR_AUX${clk}_CLK_CTRL_ADR]} {
            return [const CPR_AUX${clk}_CLK_CTRL_ADR]
        }
        return -code error "No AUX clock exists with this number: $clk"
    }

    proc getAuxclockDivider {clk} {
        return [peek [getAuxClockControlAddress $clk]]
    }

    proc gC  {clockType clk} {
        if {$clockType == "css"} {
            if {$clk >= 32} {
                set clkReg [peek [const CPR_CLK_EN1_ADR]]
                set bit     [expr $clk - 32]
            } else {
                set clkReg [peek [const CPR_CLK_EN0_ADR]]
                set bit    $clk
            }
        } elseif { $clockType == "mss" } {
            set clkReg [peek [const MSS_CLK_CTRL_ADR]]
            set bit    $clk
        } elseif { $clockType == "upa" } {
            set clkReg [peek [const CMX_CLK_CTRL]]
            set bit    $clk
        } elseif { $clockType == "sipp" } {
            set clkReg [peek [const MSS_SIPP_CLK_CTRL_ADR]]
            set bit    $clk
        } elseif { $clockType == "aux" } {
            set clkReg [peek [const CPR_AUX_CLK_EN_ADR]]
            set bit    $clk
        }
        #  puts [format "0x%08X" $clkReg]
        if {[isBitSet $bit $clkReg] == 1} {
            return "ON "
        } else {
            return "---"
        }
    }

    proc clockSet  {{action ""} {clockType ""} {clk ""} {verbose y}} {
        if { $action == "" || $clockType == "" || $clk == ""} {
            puts "usage: clockSet enable|disable css|aux|mss|upa|sipp clockNum"
            return
        }
        if {$clockType == "css"} {
            if {$clk >= 32} {
                set clkRegAdr [const CPR_CLK_EN1_ADR]
                set bit     [expr $clk - 32]
            } else {
                set clkRegAdr [const CPR_CLK_EN0_ADR]
                set bit    $clk
            }
        } elseif { $clockType == "mss" } {
            set clkRegAdr [const MSS_CLK_CTRL_ADR]
            set bit    $clk
        } elseif { $clockType == "upa" } {
            set clkRegAdr [const CMX_CLK_CTRL]
            set bit    $clk
        } elseif { $clockType == "sipp" } {
            set clkRegAdr [const MSS_SIPP_CLK_CTRL_ADR]
            set bit    $clk
        } elseif { $clockType == "aux" } {
            set clkRegAdr [const CPR_AUX_CLK_EN_ADR]
            set bit    $clk
        } else {
            # for usage:
            clockSet
            return
        }

        #puts [format "0x%08X" $clkRegAdr]
        if { $action == "enable" } {
            if {$verbose == y} {
                puts "Enabling  $clockType clock: $clk"
            }
            setBit $clkRegAdr $bit
        } elseif { $action == "disable" } {
            clearBit $clkRegAdr $bit
            if {$verbose == y} {
                puts "Disabling $clockType clock: $clk"
            }
        } else {
            # for usage:
            clockSet
            return
        }
    }

    proc getClockSource {action src} {
        switch $src {
            "0" {
                set srcName         "PLL0"
                set srcFreqMhz      [getPllFreqMhz 0]
            }
            "1" {
                set srcName         "PLL1"
                set srcFreqMhz      [getPllFreqMhz 1]
            }
            "2" {
                set srcName         "REFCLK0"
                set srcFreqMhz      [expr [getOscInKhz 0]/1000.]
            }
            "3" {
                set srcName         "REFCLK1"
                set srcFreqMhz      [expr [getOscInKhz 1]/1000.]
            }
            "4" {
                set srcName         "32Khz"
                set srcFreqMhz      [expr 32768 / 1000000.]
            }
            "5" {
                set srcName         "CPR_AUX"
                if {[isBitSet 28 [peek [const CPR_AUX_CLK_EN_ADR]]] == 1} {
                    set dividerRaw  [getAuxclockDivider 28]
                    set numerator   [extractBits $dividerRaw 16 27]
                    set denominator [extractBits $dividerRaw  0 11]
                    set srcId       [extractBits $dividerRaw 28 31]
                    set srcFreqMhz  [getAuxClkFrequencyMhz 28]
                } else {
                    set srcFreqMhz 0; # Clock disabled
                }
            }
            "6" {
                set srcName         "SYS_CLK"
                set srcFreqMhz      [getSysFreqMhz ]
            }
            "7" {
                set srcName         "SYS_CLK_DIV2"
                set srcFreqMhz      [expr [getSysFreqMhz ] / 2.]
            }
            "default" {
                set srcName         "INVALID"
                set srcFreqMhz      0; # N/A
            }
        }

        if {$action == "freqMhz"} {
            return $srcFreqMhz
        } else {
            return $srcName
        }

    }

    dict set cprClockSourceLookupName 0  "PLL0        "
    dict set cprClockSourceLookupName 1  "PLL1        "
    dict set cprClockSourceLookupName 2  "REFCLK0     "
    dict set cprClockSourceLookupName 3  "REFCLK1     "
    dict set cprClockSourceLookupName 4  "32KHZ       "
    dict set cprClockSourceLookupName 5  "CPR_AUX     "
    dict set cprClockSourceLookupName 6  "SYS_CLK     "
    dict set cprClockSourceLookupName 7  "SYS_CLK_DIV2"

    dict set cprClockSourceLookupNum PLL0                0
    dict set cprClockSourceLookupNum PLL1                1
    dict set cprClockSourceLookupNum REFCLK0             2
    dict set cprClockSourceLookupNum REFCLK1             3
    dict set cprClockSourceLookupNum 32KHZ               4
    dict set cprClockSourceLookupNum CPR_AUX             5
    dict set cprClockSourceLookupNum SYS_CLK             6
    dict set cprClockSourceLookupNum SYS_CLK_DIV2        7

    proc auxClockSet {clk src numerator denominator {enable 1}} {

        set auxClockEnableRegAddr [const CPR_AUX_CLK_EN_ADR]

        if {[string is integer $clk]} {
            set clkNum $clk
        } else {
            set clkNum [lindex [clockLookup name $clk] 1]
        }

        if {[string is integer $src]} {
            set srcId $src
        } else {
            switch $src {
                "PLL0"          {set srcId 0  }
                "PLL1"          {set srcId 1  }
                "REFCLK0"       {set srcId 2  }
                "REFCLK1"       {set srcId 3  }
                "32KHZ"         {set srcId 4  }
                "CPR_AUX"       {set srcId 5  }
                "SYS_CLK"       {set srcId 6  }
                "SYS_CLK_DIV2"  {set srcId 7  }
                default         {set srcId 100}
            }
        }

        set auxDividerAddr  [getAuxClockControlAddress ${clkNum}]

        set value [peek $auxDividerAddr]
        set value [setBitRange  $value  0 11 $denominator]
        set value [setBitRange  $value 16 27 $numerator]
        set value [setBitRange  $value 28 31 $srcId]
        poke $auxDividerAddr $value

        if {$enable} {
            setBit $auxClockEnableRegAddr $clkNum
        } else {
            clearBit $auxClockEnableRegAddr $clkNum
        }

        #puts "auxDividerAddr:$auxDividerAddr"

    }

    proc getAuxClkFrequencyMhz {clk} {

        set dividerRaw  [getAuxclockDivider $clk]
        set numerator   [extractBits $dividerRaw 16 27]
        set denominator [extractBits $dividerRaw  0 11]
        set src         [extractBits $dividerRaw 28 31]

        set bypass  0
        if {$numerator == 0} {
            set bypass 1
        }
        if {$denominator == 0} {
            set bypass 1
        }
        if {$numerator >= $denominator} {
            set bypass 1
        }

        set srcFreqMhz  [getClockSource freqMhz $src]

        if {$bypass == 1} {
            set auxClkFreqMhz $srcFreqMhz; # Bypass Mode
        } else {
            set auxClkFreqMhz [expr double($srcFreqMhz * $numerator) / $denominator]
        }
        if {[isBitSet $clk [peek [const CPR_AUX_CLK_EN_ADR]]] == 0} {
            set auxClkFreqMhz 0.0
        }

        return $auxClkFreqMhz
    }

    proc gAux {clk} {
        variable cprClockSourceLookupName
        set dividerRaw  [getAuxclockDivider $clk]
        set numerator   [extractBits $dividerRaw 16 27]
        set denominator [extractBits $dividerRaw  0 11]
        set srcId       [extractBits $dividerRaw 28 31]
        if {[isBitSet $clk [peek [const CPR_AUX_CLK_EN_ADR]]] == 1} {
            set auxClkFreqMhz [getAuxClkFrequencyMhz $clk]
            return "[format %6.6g $auxClkFreqMhz]Mhz  \[[format %06d $numerator]/[format %06d $denominator]\] src:[dict get $cprClockSourceLookupName $srcId] "
        } else {
            return "---------  \[[format %06d $numerator]/[format %06d $denominator]\] src:[dict get $cprClockSourceLookupName $srcId] "
        }
    }

    proc getValidClocks {pattern} {
        concat \
            [dict values [getCssClockName] $pattern] \
            [dict values [getMssClockName] $pattern] \
            [dict values [getSippClockName] $pattern]\
            [dict values [getUpaClockName] $pattern] \
            [dict values [getAuxClockName] $pattern]
    }

    proc getClockGroups {} {
        list css mss sipp upa aux
    }

    proc getCssClockName {} {
        # TODO: Really this should have been a 2D array
        # hpp: Dictionary will do just fine

        AddListToDict cssClockName {
             0    CSS_LOS
             1    CSS_LAHB_CTRL
             2    CSS_APB4_CTRL
             3    CSS_CPR
             4    CSS_ROM
             5    CSS_LOS_L2C
             6    CSS_MAHB_CTRL
             7    CSS_LOS_ICB
             8    CSS_LOS_DSU
             9    CSS_LOS_TIM
             10   CSS_GPIO
             11   CSS_JTAG
             12   CSS_SDIO
             13   CSS_BIST2
             14   CSS_GETH
             15   CSS_BIST
             16   CSS_APB1_CTRL
             17   CSS_AHB_DMA
             18   CSS_APB3_CTRL
             19   CSS_I2C0
             20   CSS_I2C1
             21   CSS_I2C2
             22   CSS_UART
             23   CSS_SPI0
             24   CSS_SPI1
             25   CSS_SPI2
             26   CSS_I2S0
             27   CSS_I2S1
             28   CSS_I2S2
             29   CSS_SAHB_CTRL
             30   CSS_MSS_MAS
             31   CSS_UPA_MAS
             32   CSS_DSS_APB
             33   CSS_DSS_APB_RST_PHY
             34   CSS_DSS_BUS_MAHB
             35   CSS_DSS_BUS_DXI
             36   CSS_DSS_BUS_AAXI
             37   CSS_DSS_BUS_MXI
             38   CSS_LAHB2SHB
             39   CSS_SAHB2MAHB
             40   CSS_USB
        }

        if {![isMA2100]} {
            AddListToDict cssClockName {
                41   CSS_USB_APBSLV
                42   CSS_AON
            }
        }

        return $cssClockName
    }

    proc getMssClockName {} {
        AddListToDict mssClockName {
            0    MSS_APB_SLV
            1    MSS_APB2_CTRL
            2    MSS_RTBRIDGE
            3    MSS_RTAHB_CTRL
            4    MSS_LRT
            5    MSS_LRT_DSU
            6    MSS_LRT_L2C
            7    MSS_LRT_ICB
            8    MSS_AXI_BRIDGE
            9    MSS_MXI_CTRL
            10   MSS_MXI_DEFSLV
            11   MSS_AXI_MON
            12   MSS_NAL
        }
        if {[isMA2100]} {
            AddListToDict mssClockName {
                13   MSS_MIPI
                14   MSS_CIF0
                15   MSS_CIF1
                16   MSS_LCD
                17   MSS_SIPP
                18   MSS_TIM
                19   MSS_SIPP_ABPSLV
            }
        } else {
            AddListToDict mssClockName {
                13   MSS_CIF0
                14   MSS_CIF1
                15   MSS_LCD
                16   MSS_TIM
                17   MSS_AMC
                18   MSS_SIPP  ;# SIPP/MIPI master (MSS_SPMP in regMA2150)
            }
        }
        return $mssClockName
    }

    proc getSippClockName {} {
        if {[isMA2100]} {
            AddListToDict sippClockName {
                0   DEV_SIPP_RAW
                1   DEV_SIPP_LSC
                2   DEV_SIPP_DBYR
                3   DEV_SIPP_CHROMA
                4   DEV_SIPP_LUMA
                5   DEV_SIPP_SHARPEN
                6   DEV_SIPP_UPFIRDN
                7   DEV_SIPP_MED
                8   DEV_SIPP_LUT
                9   DEV_SIPP_EDGE_OP
                10  DEV_SIPP_CONV
                11  DEV_SIPP_HARRIS
                12  DEV_SIPP_CC
                14  DEV_SIPP_MIPI_TX0
                15  DEV_SIPP_MIPI_TX1
                16  DEV_SIPP_MIPI_RX0
                17  DEV_SIPP_MIPI_RX1
                18  DEV_SIPP_MIPI_RX2
                19  DEV_SIPP_MIPI_RX3
                20  DEV_SIPP_DBYR_PPM
            }
        } else {
            AddListToDict sippClockName {
                0   MSS_SP_SIGMA
                1   MSS_SP_LSC
                2   MSS_SP_RAW
                3   MSS_SP_DBYR
                4   MSS_SP_DOGL
                5   MSS_SP_LUMA
                6   MSS_SP_SHARPEN
                7   MSS_SP_CGEN
                8   MSS_SP_MED
                9   MSS_SP_CHROMA
                10  MSS_SP_CC
                11  MSS_SP_LUT
                12  MSS_SP_EDGE_OP
                13  MSS_SP_CONV
                14  MSS_SP_HARRIS
                15  MSS_SP_UPFIRDN0
                16  MSS_SP_UPFIRDN1
                17  MSS_SP_UPFIRDN2
                18  MSS_SP_MIPI_TX0
                19  MSS_SP_MIPI_TX1
                20  MSS_SP_MIPI_RX0
                21  MSS_SP_MIPI_RX1
                22  MSS_SP_MIPI_RX2
                23  MSS_SP_MIPI_RX3
                24  MSS_SP_MIPI
                25  MSS_SP_SIPP
                26  MSS_SP_SIPP_ABPSLV
                27  MSS_SP_APB_SLV
            }
        }
        return $sippClockName
    }

    proc getUpaClockName {} {
        AddListToDict upaClockName {
            0    UPA_SH0
            1    UPA_SH1
            2    UPA_SH2
            3    UPA_SH3
            4    UPA_SH4
            5    UPA_SH5
            6    UPA_SH6
            7    UPA_SH7
            8    UPA_SH8
            9    UPA_SH9
            10   UPA_SH10
            11   UPA_SH11
            12   UPA_SHAVE_L2
            13   UPA_CDMA
            14   UPA_BIC
            15   UPA_CTRL
        }
        if {![isMA2100]} {
            AddListToDict upaClockName {
                16   UPA_MTX
            }
        }
        return $upaClockName
    }

    proc getAuxClockName {} {
        AddListToDict auxClockName {
                0    CSS_AUX_I2S0
                1    CSS_AUX_I2S1
                2    CSS_AUX_I2S2
                3    CSS_AUX_SDIO
                4    CSS_AUX_GPIO
                5    CSS_AUX_32KHZ
                6    CSS_AUX_UART_SCLK
                7    CSS_AUX_CIF0
                8    CSS_AUX_CIF1
                9    CSS_AUX_LCD
                10   CSS_AUX_NAL_PAR
                11   CSS_AUX_NAL_SYN
                12   CSS_AUX_MIPI_TX0
                13   CSS_AUX_MIPI_TX1
                14   CSS_AUX_USB_PHY_EXTREFCLK
                15   CSS_AUX_USB_CTRL_SUSPEND_CLK
                16   CSS_AUX_USB_CTRL_REF_CLK
                17   CSS_AUX_DDR_REF
                18   CSS_AUX_BIST2
                19   CSS_AUX_MEDIA
                20   CSS_AUX_DDR_CORE
                21   CSS_AUX_DDR_CORE_PHY
                22   CSS_AUX_MIPI_ECFG
                23   CSS_AUX_MIPI_CFG
                24   CSS_AUX_USB_PHY_REF_ALT_CLK
                25   CSS_AUX_GPIO1
                26   CSS_AUX_GPIO2
                27   CSS_AUX_GPIO3
                28   CSS_AUX_CPR
            }
        if {![isMA2100]} {
            AddListToDict auxClockName {
                        29   CSS_AUX_TSENS
                    }
        }
        return $auxClockName
    }

    proc clockLookup {{action default} {param1 default} {param2 default}} {

        set result ""

        set cssClockName [getCssClockName]
        set mssClockName [getMssClockName]
        set sippClockName [getSippClockName]
        set upaClockName [getUpaClockName]
        set auxClockName [getAuxClockName]

        switch $action {
            "number"  {
                set clockGroup $param1
                set clockNum   $param2
                switch $clockGroup {
                    "css" {
                        set result [dict get $cssClockName $clockNum]
                    }
                    "mss" {
                        set result [dict get $mssClockName $clockNum]
                    }
                    "sipp" {
                        set result [dict get $sippClockName $clockNum]
                    }
                    "upa" {
                        set result [dict get $upaClockName $clockNum]
                    }
                    "aux" {
                        set result [dict get $auxClockName $clockNum]
                    }
                    "default" {
                        puts "Error: invalid clock group please use <css|mss|sipp|upa|aux>"
                    }
                }
            }
            "getSupportedClocks" {
                set clockGroup $param1
                switch $clockGroup {
                    "css" {
                        set result [lsort -integer [dict keys $cssClockName]]
                    }
                    "mss" {
                        set result [lsort -integer [dict keys $mssClockName]]
                    }
                    "sipp" {
                        set result [lsort -integer [dict keys $sippClockName]]
                    }
                    "upa" {
                        set result [lsort -integer [dict keys $upaClockName]]
                    }
                    "aux" {
                        set result [lsort -integer [dict keys $auxClockName]]
                    }
                    "default" {
                        puts "Error: invalid clock group please use <css|mss|sipp|upa|aux>"
                    }
                }
            }
            "getClockArray" {
                set clockGroup $param1
                switch $clockGroup {
                    "css" {
                        set result [dict get $cssClockName]
                    }
                    "mss" {
                        set result [dict get $mssClockName]
                    }
                    "sipp" {
                        set result [dict get $sippClockName]
                    }
                    "upa" {
                        set result [dict get $upaClockName]
                    }
                    "aux" {
                        set result [dict get $auxClockName]
                    }
                    "default" {
                        puts "Error: invalid clock group please use <css|mss|sipp|upa|aux>"
                    }
                }
            }
            "name"  {
                set clockName $param1
                set foundInGroup ""
                foreach element [lsort -integer [dict keys $cssClockName]] {
                    if {[dict get $cssClockName $element] == $clockName} {
                        set result $element
                        set foundInGroup  css
                    }
                }
                foreach element [lsort -integer [dict keys $mssClockName]] {
                    if {[dict get $mssClockName $element] == $clockName} {
                        set result $element
                        set foundInGroup  mss
                    }
                }
                foreach element [lsort -integer [dict keys $sippClockName]] {
                    if {[dict get $sippClockName $element] == $clockName} {
                        set result $element
                        set foundInGroup  sipp
                    }
                }
                foreach element [lsort -integer [dict keys $upaClockName]] {
                    if {[dict get $upaClockName $element] == $clockName} {
                        set result $element
                        set foundInGroup  upa
                    }
                }
                foreach element [lsort -integer [dict keys $auxClockName]] {
                    if {[dict get $auxClockName $element] == $clockName || [dict get $auxClockName $element] == "CSS_$clockName" } {
                        set result $element
                        set foundInGroup  aux
                    }
                }
                if {$result == ""} {
                    puts "Error: Clock $clockName was not found"
                } else {
                    set result [list $foundInGroup $result]
                }

            }
            "list"  {
                set clockGroup $param1
                switch $clockGroup {
                    "css" {
                        foreach element [lsort -integer [dict keys $cssClockName]] {
                            puts "[format %02d $element] : [dict get $cssClockName $element]"
                        }
                    }
                    "mss" {
                        foreach element [lsort -integer [dict keys $mssClockName]] {
                            puts "[format %02d $element] : [dict get $mssClockName $element]"
                        }
                    }
                    "sipp" {
                        foreach element [lsort -integer [dict keys $sippClockName]] {
                            puts "[format %02d $element] : [dict get $sippClockName $element]"
                        }
                    }
                    "upa" {
                        foreach element [lsort -integer [dict keys $upaClockName]] {
                            puts "[format %02d $element] : [dict get $upaClockName $element]"
                        }
                    }
                    "aux" {
                        foreach element [lsort -integer [dict keys $auxClockName]] {
                            puts "[format %02d $element] : [dict get $auxClockName $element]"
                        }
                    }
                    "default" {
                        puts "Error: invalid clock group please use <css|mss|sipp|upa|aux>"
                    }
                }
            }
            default {
                puts "Usage Examples:"
                puts "---------------------------"
                puts "clockLookup name   CSS_I2C"
                puts "clockLookup number css 3"
            }
        }

        return $result
    }

    # Warning: Do not reformat this table. It is carefully constructed to line up correctly when printed
    proc sysClocks {} {
        # TODO: Add System Clock Frequency calculation
        # TODO: Show which clocks are 50% clocks
        set spaces____ "   "
        set equals____ "==="
        set minuses___ "---"
        set minuses____ "---"
        set spaces_____ "   "
        puts "System Clocks:                                                                      "
        puts "---------------                                                                     "
        if {[isMA2100]} {
            puts "CPU Subsystem (CSS)      $spaces____       $spaces____                  $spaces____   Media Subsystem (MSS)"
            puts "==============$equals____==================$equals____==================$equals____=  ==============$equals____====================$equals____==============="
            puts "LeonOS:       $spaces____  System Bus:     $spaces____  Peripheral:     $spaces____   LeonRT        $spaces____  Busses            $spaces____  Peripherals"
            puts "------------- $spaces____  --------------- $spaces____  --------------- $spaces____   ------------- $spaces____  ----------------- $spaces____  --------------"
            puts "CSS_LOS     : [gC css 0 ]  CSS_LAHB_CTRL : [gC css 1 ]  CSS_AHB_DMA   : [gC css 17]   MSS_LRT     : [gC mss 4 ]  MSS_RTBRIDGE    : [gC mss 2 ]  MSS_MIPI     : [gC mss 13]"
            puts "CSS_ROM     : [gC css 4 ]  CSS_MAHB_CTRL : [gC css 6 ]  CSS_USB       : [gC css 40]   MSS_LRT_DSU : [gC mss 5 ]  MSS_RTAHB_CTRL  : [gC mss 3 ]  MSS_CIF0     : [gC mss 14]"
            puts "CSS_LOS_L2C : [gC css 5 ]  CSS_SAHB_CTRL : [gC css 29]  CSS_SDIO      : [gC css 12]   MSS_LRT_L2C : [gC mss 6 ]  MSS_APB_SLV     : [gC mss 0 ]  MSS_CIF1     : [gC mss 15]"
            puts "CSS_LOS_ICB : [gC css 7 ]  CSS_LAHB2SHB  : [gC css 38]  CSS_BIST2     : [gC css 13]   MSS_LRT_ICB : [gC mss 7 ]  MSS_APB2_CTRL   : [gC mss 1 ]  MSS_LCD      : [gC mss 16]"
            puts "CSS_LOS_DSU : [gC css 8 ]  CSS_SAHB2MAHB : [gC css 39]  CSS_GETH      : [gC css 14]   MSS_TIM     : [gC mss 18]  MSS_SIPP_ABPSLV : [gC mss 19]  MSS_AMC/SIPP : [gC mss 17]"
            puts "CSS_LOS_TIM : [gC css 9 ]  CSS_APB1_CTRL : [gC css 16]  CSS_BIST      : [gC css 15]                 $spaces____  MSS_AXI_BRIDGE  : [gC mss 8 ]  MSS_NAL      : [gC mss 12]"
            puts "CSS_GPIO    : [gC css 10]  CSS_APB3_CTRL : [gC css 18]                  $spaces____                 $spaces____  MSS_MXI_CTRL    : [gC mss 9 ]                   "
            puts "CSS_JTAG    : [gC css 11]  CSS_APB4_CTRL : [gC css 2 ]                  $spaces____                 $spaces____  MSS_MXI_DEFSLV  : [gC mss 10]                   "
            puts "              $spaces____                  $spaces____                  $spaces____                 $spaces____  MSS_AXI_MON     : [gC mss 11]                   "
            puts ""
            puts "Serial      $spaces____  DDR            $spaces____  MSS/UPA       $spaces____      SIPP              $spaces____                       $spaces____     "
            puts "----------  $spaces____  ---------------$spaces____  ------------  $spaces____      ------------------$minuses___-----------------------$minuses___-----"
            puts "CSS_I2C0  : [gC css 19]  CSS_DSS_APB  : [gC css 32]  CSS_MSS_MAS : [gC css 30]      SIPP_RAW        : [gC sipp  0 ]   SIPP_MIPI_TX0   : [gC sipp 14]"
            puts "CSS_I2C1  : [gC css 20]  CSS_DSS_BUS  : [gC css 34]  CSS_UPA_MAS : [gC css 31]      SIPP_LSC        : [gC sipp  1 ]   SIPP_MIPI_TX1   : [gC sipp 15]"
            puts "CSS_I2C2  : [gC css 21]                 $spaces____                $spaces____      SIPP_DEBAYER    : [gC sipp  2 ]   SIPP_MIPI_RX0   : [gC sipp 16]"
            puts "CSS_UART  : [gC css 22]                 $spaces____                $spaces____      SIPP_CHROMA_DN  : [gC sipp  3 ]   SIPP_MIPI_RX1   : [gC sipp 17]"
            puts "CSS_SPI0  : [gC css 23]                 $spaces____                $spaces____      SIPP_LUMA_DN    : [gC sipp  4 ]   SIPP_MIPI_RX2   : [gC sipp 18]"
            puts "CSS_SPI1  : [gC css 24]                 $spaces____                $spaces____      SIPP_SHARPEN    : [gC sipp  5 ]   SIPP_MIPI_RX3   : [gC sipp 19]"
            puts "CSS_SPI2  : [gC css 25]                 $spaces____                $spaces____      SIPP_POLYPHASE  : [gC sipp  6 ]                                 "
            puts "CSS_I2S0  : [gC css 26]                 $spaces____                $spaces____      SIPP_MEDIAN     : [gC sipp  7 ]                                 "
            puts "CSS_I2S1  : [gC css 27]                 $spaces____                $spaces____      SIPP_LUT        : [gC sipp  8 ]                                 "
            puts "CSS_I2S2  : [gC css 28]                 $spaces____                $spaces____      SIPP_EDGE_OP    : [gC sipp  9 ]                                 "
            puts "            $spaces____                 $spaces____                $spaces____      SIPP_CONV_KERN  : [gC sipp 10 ]                                 "
            puts "            $spaces____                 $spaces____                $spaces____      SIPP_HARRIS     : [gC sipp 11 ]                                 "
        } else {
            puts "CPU Subsystem (CSS)      $spaces____       $spaces____                  $spaces____   Media Subsystem (MSS)"
            puts "==============$equals____==================$equals____==================$equals____=  =================$equals____======================$equals____================"
            puts "LeonOS:       $spaces____  System Bus:     $spaces____  Peripheral:     $spaces____   LeonRT           $spaces____  Busses              $spaces____  Peripherals   "
            puts "------------- $spaces____  --------------- $spaces____  --------------- $spaces____   ---------------- $spaces____  ------------------- $spaces____  --------------"
            puts "CSS_LOS     : [gC css 0 ]  CSS_LAHB_CTRL : [gC css 1 ]  CSS_AHB_DMA   : [gC css 17]   MSS_CP_LRT     : [gC mss 4 ]  MSS_CP_RTBRIDGE   : [gC mss 2 ]  MSS_CP_SPMP : [gC mss 18]"
            puts "CSS_ROM     : [gC css 4 ]  CSS_MAHB_CTRL : [gC css 6 ]  CSS_USB       : [gC css 40]   MSS_CP_LRT_DSU : [gC mss 5 ]  MSS_CP_RTAHB_CTRL : [gC mss 3 ]  MSS_CP_CIF0 : [gC mss 13]"
            puts "CSS_LOS_L2C : [gC css 5 ]  CSS_SAHB_CTRL : [gC css 29]  CSS_SDIO      : [gC css 12]   MSS_CP_LRT_L2C : [gC mss 6 ]  MSS_CP_APB_SLV    : [gC mss 0 ]  MSS_CP_CIF1 : [gC mss 14]"
            puts "CSS_LOS_ICB : [gC css 7 ]  CSS_LAHB2SHB  : [gC css 38]  CSS_BIST2     : [gC css 13]   MSS_CP_LRT_ICB : [gC mss 7 ]  MSS_CP_APB2_CTRL  : [gC mss 1 ]  MSS_CP_LCD  : [gC mss 15]"
            puts "CSS_LOS_DSU : [gC css 8 ]  CSS_SAHB2MAHB : [gC css 39]  CSS_GETH      : [gC css 14]   MSS_CP_TIM     : [gC mss 16]  MSS_CP_AXI_BRIDGE : [gC mss 8 ]  MSS_CP_AMC  : [gC mss 17]"
            puts "CSS_LOS_TIM : [gC css 9 ]  CSS_APB1_CTRL : [gC css 16]  CSS_BIST      : [gC css 15]                    $spaces____  MSS_CP_MXI_CTRL   : [gC mss 9 ]  MSS_CP_NAL  : [gC mss 12]"
            puts "CSS_GPIO    : [gC css 10]  CSS_APB3_CTRL : [gC css 18]  CSS_USB_APBSLV: [gC css 41]                    $spaces____  MSS_CP_MXI_DEFSLV : [gC mss 10]                "
            puts "CSS_JTAG    : [gC css 11]  CSS_APB4_CTRL : [gC css 2 ]  CSS_AON         [gC css 42]                    $spaces____  MSS_CP_AXI_MON    : [gC mss 11]                "
            puts ""
            puts "Serial      $spaces____  DDR            $spaces____  MSS/UPA       $spaces____      SIPP (_HWF suffixes skipped) $spaces_____           $spaces_____"
            puts "----------  $spaces____  ---------------$spaces____  ------------  $spaces____      -----------------$minuses____-----------------------$minuses____"
            puts "CSS_I2C0  : [gC css 19]  CSS_DSS_APB  : [gC css 32]  CSS_MSS_MAS : [gC css 30]      MSS_SP_SIGMA   : [gC sipp 0 ]  MSS_SP_HARRIS      : [gC sipp 14]"
            puts "CSS_I2C1  : [gC css 20]  CSS_DSS_BUS  : [gC css 34]  CSS_UPA_MAS : [gC css 31]      MSS_SP_LSC     : [gC sipp 1 ]  MSS_SP_UPFIRDN0    : [gC sipp 15]"
            puts "CSS_I2C2  : [gC css 21]                 $spaces____                $spaces____      MSS_SP_RAW     : [gC sipp 2 ]  MSS_SP_UPFIRDN1    : [gC sipp 16]"
            puts "CSS_UART  : [gC css 22]                 $spaces____                $spaces____      MSS_SP_DBYR    : [gC sipp 3 ]  MSS_SP_UPFIRDN2    : [gC sipp 17]"
            puts "CSS_SPI0  : [gC css 23]                 $spaces____                $spaces____      MSS_SP_DOGL    : [gC sipp 4 ]  MSS_SP_MIPI_TX0    : [gC sipp 18]"
            puts "CSS_SPI1  : [gC css 24]                 $spaces____                $spaces____      MSS_SP_LUMA    : [gC sipp 5 ]  MSS_SP_MIPI_TX1    : [gC sipp 19]"
            puts "CSS_SPI2  : [gC css 25]                 $spaces____                $spaces____      MSS_SP_SHARPEN : [gC sipp 6 ]  MSS_SP_MIPI_RX0    : [gC sipp 20]"
            puts "CSS_I2S0  : [gC css 26]                 $spaces____                $spaces____      MSS_SP_CGEN    : [gC sipp 7 ]  MSS_SP_MIPI_RX1    : [gC sipp 21]"
            puts "CSS_I2S1  : [gC css 27]                 $spaces____                $spaces____      MSS_SP_MED     : [gC sipp 8 ]  MSS_SP_MIPI_RX2    : [gC sipp 22]"
            puts "CSS_I2S2  : [gC css 28]                 $spaces____                $spaces____      MSS_SP_CHROMA  : [gC sipp 9 ]  MSS_SP_MIPI_RX3    : [gC sipp 23]"
            puts "            $spaces____                 $spaces____                $spaces____      MSS_SP_CC      : [gC sipp 10]  MSS_SP_MIPI        : [gC sipp 24]"
            puts "            $spaces____                 $spaces____                $spaces____      MSS_SP_LUT     : [gC sipp 11]  MSS_SP_SIPP        : [gC sipp 25]"
            puts "            $spaces____                 $spaces____                $spaces____      MSS_SP_EDGE_OP : [gC sipp 12]  MSS_SP_SIPP_ABPSLV : [gC sipp 26]"
            puts "            $spaces____                 $spaces____                $spaces____      MSS_SP_CONV    : [gC sipp 13]  MSS_SP_APB_SLV     : [gC sipp 27]"
        }
        puts ""
        puts "UPA Subsystem (UPA)   $spaces____     $spaces____              $spaces____          System Frequencies  "
        puts "===========$equals____================$equals____==============$equals____===       ==================  "
        puts "UPA_SH0  : [gC upa 0 ]    UPA_SH6  : [gC upa 6 ]   UPA_SHAVE_L2 : [gC upa 12]       System Freq :[fMhz [getSysFreqMhz  ]]"
        puts "UPA_SH1  : [gC upa 1 ]    UPA_SH7  : [gC upa 7 ]   UPA_CDMA     : [gC upa 13]       Pll0   Freq :[fMhz [getPllFreqMhz 0]]"
        puts "UPA_SH2  : [gC upa 2 ]    UPA_SH8  : [gC upa 8 ]   UPA_BIC      : [gC upa 14]       Pll1   Freq :[fMhz [getPllFreqMhz 1]]"
        puts "UPA_SH3  : [gC upa 3 ]    UPA_SH9  : [gC upa 9 ]   UPA_CTRL     : [gC upa 15]"
        puts "UPA_SH4  : [gC upa 4 ]    UPA_SH10 : [gC upa 10]                             "
        puts "UPA_SH5  : [gC upa 5 ]    UPA_SH11 : [gC upa 11]                             "
    }

    proc auxClocks {} {
        set minuses_ "--------------------------------------------"
        set spaces__ "                                            "
        puts ""
        puts "Auxilary Clocks: "
        puts "---------------"
        # nr. 21 is not presented because it's reset-only
        puts "Audio/Video/Media:  $spaces__ Other:"
        puts "--------------------$minuses_ --------------------------$minuses_"
        puts "CSS_AUX_I2S0      : [gAux  0] CSS_AUX_DDR_REF              : [gAux 17]"
        puts "CSS_AUX_I2S1      : [gAux  1] CSS_AUX_DDR_CORE             : [gAux 20]"
        puts "CSS_AUX_I2S2      : [gAux  2] CSS_AUX_USB_PHY_EXTREFCLK    : [gAux 14]"
        puts "CSS_AUX_CIF0      : [gAux  7] CSS_AUX_USB_CTRL_SUSPEND_CLK : [gAux 15]"
        puts "CSS_AUX_CIF1      : [gAux  8] CSS_AUX_USB_CTRL_REF_CLK     : [gAux 16]"
        puts "CSS_AUX_LCD       : [gAux  9] CSS_AUX_USB_PHY_REF_ALT_CLK  : [gAux 24]"
        puts "CSS_AUX_NAL_PAR   : [gAux 10] CSS_AUX_SDIO                 : [gAux  3]"
        puts "CSS_AUX_NAL_SYN   : [gAux 11] CSS_AUX_32KHZ                : [gAux  5]"
        puts "CSS_AUX_MIPI_TX0  : [gAux 12] CSS_AUX_UART                 : [gAux  6]"
        puts "CSS_AUX_MIPI_TX1  : [gAux 13] CSS_AUX_GPIO                 : [gAux  4]"
        puts "CSS_AUX_MIPI_ECFG : [gAux 22] CSS_AUX_GPIO1                : [gAux 25]"
        puts "CSS_AUX_MIPI_CFG  : [gAux 23] CSS_AUX_GPIO2                : [gAux 26]"
        puts "CSS_AUX_BIST2     : [gAux 18] CSS_AUX_GPIO3                : [gAux 27]"
        puts "CSS_AUX_MEDIA     : [gAux 19] CSS_AUX_CPR                  : [gAux 28]"
        if {![isMA2100]} {
            puts "                    $spaces__ CSS_AUX_TSENS                : [gAux 29]"
        }
    }

    proc gR  {rstType rstNo} {
        if { $rstType == "mss" } {
            set rstReg [peek [const MSS_RSTN_CTRL_ADR]]
            set bit    $rstNo
        } elseif { $rstType == "upa" } {
            set rstReg [peek [const CMX_RSTN_CTRL]]
            set bit    $rstNo
        } elseif { $rstType == "aux" } {
            set rstReg [peek [const CPR_AUX_CLK_RST_ADR]]
            set bit    $rstNo
        } elseif { ($rstType == "css") && ($rstNo > 31) } {
            set rstReg [peek [const CPR_BLK_RST1_ADR]]
            set bit    [expr $rstNo - 32]
        } else {
            return -code error "reset type not supported"
        }
        #  puts [format "0x%08X" $clkReg]
        if {[isBitSet $bit $rstReg]} {
            return "deasserted"
        } else {
            return "ASSERTED  "
        }
    }

    proc setResetBit  {rstType rstNo val} {
        if { $rstType == "css" } {
            set rstRegA [const CPR_BLK_RST0_ADR]
            set bit    $rstNo
            if { $rstNo > 31} {
                set rstRegA [const CPR_BLK_RST1_ADR]
                set bit  [expr $rstNo - 32]
            }
        } elseif { $rstType == "mss" } {
            set rstRegA [const MSS_RSTN_CTRL_ADR]
            set bit    $rstNo
        } elseif { $rstType == "upa" } {
            set rstRegA [const CMX_RSTN_CTRL]
            set bit    $rstNo
        } elseif { $rstType == "aux" } {
            set rstRegA [const CPR_AUX_CLK_RST_ADR]
            set bit    $rstNo
        } else {
            puts "ERROR: reset type not recognized!"
            return
        }
        setBitRangeInReg $rstRegA  $bit  $bit  $val
    }

    proc showResets {}  {
        set spaces____ "          "
        set equals____ "=========="
        puts "Non-auto-deasserting resets:"
        puts "======================="
        puts "Auxiliary resets:"
        puts "================="
        puts "CSS_AUX_DDR_CORE_CTRL : [gR aux 20]"
        puts "CSS_AUX_DDR_CORE_PHY  : [gR aux 21]"
        puts ""
        puts "UPA Subsystem (UPA)   $spaces____     $spaces____              $spaces____    "
        puts "===========$equals____================$equals____==============$equals____===="
        puts "UPA_SH0  : [gR upa 0 ]     UPA_SH6  : [gR upa 6 ]   UPA_SHAVE_L2 : [gR upa 12]"
        puts "UPA_SH1  : [gR upa 1 ]     UPA_SH7  : [gR upa 7 ]   UPA_CDMA     : [gR upa 13]"
        puts "UPA_SH2  : [gR upa 2 ]     UPA_SH8  : [gR upa 8 ]   UPA_BIC      : [gR upa 14]"
        puts "UPA_SH3  : [gR upa 3 ]     UPA_SH9  : [gR upa 9 ]   UPA_CTRL     : [gR upa 15]"
        set mtxrst ""
        if {![isMA2100]} {
            set mtxrst "UPA_MTX      : [gR upa 16]"
        }
        puts "UPA_SH4  : [gR upa 4 ]     UPA_SH10 : [gR upa 10]   $mtxrst"
        puts "UPA_SH5  : [gR upa 5 ]     UPA_SH11 : [gR upa 11]                             "
        puts ""
        if {[isMA2100]} {
            puts "Media Subsystem (MSS)                                                         "
            puts "==============$equals____=====================$equals____===================$equals____"
            puts "LeonRT        $spaces____   Busses            $spaces____   Peripherals"
            puts "------------  $spaces____   ----------        $spaces____   ------------"
            puts "MSS_LRT     : [gR mss 4 ]   MSS_RTBRIDGE    : [gR mss 2 ]   MSS_MIPI      : [gR mss 13]"
            puts "MSS_LRT_DSU : [gR mss 5 ]   MSS_RTAHB_CTRL  : [gR mss 3 ]   MSS_CIF0      : [gR mss 14]"
            puts "MSS_LRT_L2C : [gR mss 6 ]   MSS_APB_SLV     : [gR mss 0 ]   MSS_CIF1      : [gR mss 15]"
            puts "MSS_LRT_ICB : [gR mss 7 ]   MSS_APB2_CTRL   : [gR mss 1 ]   MSS_LCD       : [gR mss 16]"
            puts "MSS_TIM     : [gR mss 18]   MSS_SIPP_ABPSLV : [gR mss 19]   MSS_AMC/SIPP  : [gR mss 17]"
            puts "              $spaces____   MSS_AXI_BRIDGE  : [gR mss 8 ]   MSS_NAL       : [gR mss 12]"
            puts "              $spaces____   MSS_MXI_CTRL    : [gR mss 9 ]                              "
            puts "              $spaces____   MSS_MXI_DEFSLV  : [gR mss 10]                              "
            puts "              $spaces____   MSS_AXI_MON     : [gR mss 11]                              "
        } else {
            puts "Media Subsystem (MSS)"
            puts "=================$equals____======================$equals____================"
            puts "LeonRT           $spaces____  Busses              $spaces____  Peripherals   "
            puts "---------------- $spaces____  ------------------- $spaces____  --------------"
            puts "MSS_CP_LRT     : [gR mss 4 ]  MSS_CP_RTBRIDGE   : [gR mss 2 ]  MSS_CP_SPMP : [gR mss 18]"
            puts "MSS_CP_LRT_DSU : [gR mss 5 ]  MSS_CP_RTAHB_CTRL : [gR mss 3 ]  MSS_CP_CIF0 : [gR mss 13]"
            puts "MSS_CP_LRT_L2C : [gR mss 6 ]  MSS_CP_APB_SLV    : [gR mss 0 ]  MSS_CP_CIF1 : [gR mss 14]"
            puts "MSS_CP_LRT_ICB : [gR mss 7 ]  MSS_CP_APB2_CTRL  : [gR mss 1 ]  MSS_CP_LCD  : [gR mss 15]"
            puts "MSS_CP_TIM     : [gR mss 16]  MSS_CP_AXI_BRIDGE : [gR mss 8 ]  MSS_CP_AMC  : [gR mss 17]"
            puts "                 $spaces____  MSS_CP_MXI_CTRL   : [gR mss 9 ]  MSS_CP_NAL  : [gR mss 12]"
            puts "                 $spaces____  MSS_CP_MXI_DEFSLV : [gR mss 10]                "
            puts "                 $spaces____  MSS_CP_AXI_MON    : [gR mss 11]                "
            puts "Other Resets"
            puts "================"
            puts "CSS_DSS_APB_RST_PHY: [gR css 33]"
        }
        puts ""
    }

    proc RstCmd {{action default} args} {
        switch $action {
            "show" {
                showResets
            }
            "list" {
                foreach name [mdbg::findReg "CSS_*" "MSS_*" "UPA_*"] {
                    if { [string match CSS_AUX* $name] && ![string match CSS_AUX_DDR_CORE_* $name] } {
                        continue
                    }
                    if { ![string match *_ADR $name] } {
                        set expansion [mdbg::getRegDef $name]
                        set value [mdbg::getRegAddr $name]
                        set line [format "%-30s %9s       %s" $name $value $expansion]
                        puts "$line"
                    }
                }
            }
            "listautodeasserting" {
                foreach name [mdbg::findReg "CSS_*"] {
                    if { [string match CSS_AUX* $name] } {
                        continue
                    }
                    if { ![string match *_ADR $name] } {
                        set expansion [mdbg::getRegDef $name]
                        set value [mdbg::getRegAddr $name]
                        set line [format "%-30s %9s       %s" $name $value $expansion]
                        puts "$line"
                    }
                }
            }
            default {
                set maybename [lindex $args 0]
                if { [regexp {^AUX_} $maybename] } {
                    set maybename "CSS_$maybename"
                }
                set rstId $args
                set constVal ""
                if {[constExists $maybename]} {
                    set constVal [const $maybename]
                }
                if { [regexp {^CSS_AUX_} $maybename] } {
                    if { $constVal != "" } {
                        set rstId "aux [const $maybename]"
                    } else {
                        puts "ERROR: reset name not recognized"
                        return
                    }
                } elseif { [regexp {^CSS_} $maybename] } {
                    if { $constVal != "" } {
                        set rstId "css [const $maybename]"
                    } else {
                        puts "ERROR: reset name not recognized"
                        return
                    }
                } elseif { [regexp {^MSS_} $maybename] } {
                    if { $constVal != "" } {
                        set rstId "mss [const $maybename]"
                    } else {
                        puts "ERROR: reset name not recognized"
                        return
                    }
                } elseif { [regexp {^UPA_} $maybename] } {
                    if { $constVal != "" } {
                        set rstId "upa [const $maybename]"
                    } else {
                        puts "ERROR: reset name not recognized"
                        return
                    }
                }
                switch $action {
                    "assert" {

                        if { ![regexp {^CSS_AUX} $maybename] } {
                            if { [regexp {^CSS_} $maybename] } {
                                puts "WARNING: this reset bit will auto-deassert!"
                            }
                        }
                        if {$maybename == "css"} {
                            puts "WARNING: this reset bit will auto-deassert!"
                        }
                        setResetBit [lindex $rstId 0] [lindex $rstId 1] 0
                    }
                    "deassert" {
                        setResetBit [lindex $rstId 0] [lindex $rstId 1] 1
                    }
                    "pulse" {
                        setResetBit [lindex $rstId 0] [lindex $rstId 1] 0
                        setResetBit [lindex $rstId 0] [lindex $rstId 1] 1
                    }

                    default {
                        puts "usage: reset <show|list|listautodeasserting>"
                        puts "       reset <assert|deassert|pulse> CSS_I2S0"
                        puts "       reset <assert|deassert|pulse> css 10"
                    }
                }
            }
        }
    }

    proc allClocks {} {
        pll
        sysClocks
        auxClocks
    }

    proc isMyriad2V1_X {} {
        set cprVersion [peek [const CPR_VERSION_ID_ADR]]
        return [expr $cprVersion == 0x1C5CD611]
    }

    proc arePwStatusBitsInvertedComparedToPwEnable {} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwStatus [peek [const CPR_PWR_ISLAND_STATUS]]
            set pwEnable [peek [const CPR_PWR_ISLAND_EN_ADR]]
        } else {
            set pwStatus [peek [const AON_PWR_ISLAND_STATUS]]
            set pwEnable [peek [const AON_PWR_ISLAND_EN_ADR]]
        }
        return [expr ($pwStatus ^ $pwEnable) & (1 << 15)]
    }

    proc isMyriad2V1_0_or_V1_1 {} {
        return [expr [isMyriad2V1_X] && ![arePwStatusBitsInvertedComparedToPwEnable]]
    }

    proc myriad2InvertedIslandEnables {} {
        if { [isMyriad2V1_0_or_V1_1] } {
            return 0xfffffffe
        } else {
            return 0
        }
    }

    proc myriad2InvertedTrickleEnables {} {
        if { [isMyriad2V1_X] } {
            return 0xfffffffe
        } else {
            return 0;
        }
    }

    proc myriad2InvertedIslandStatus {} {
        if { [arePwStatusBitsInvertedComparedToPwEnable] } {
            return [expr 0xffffffff ^ [myriad2InvertedIslandEnables]]
        } else {
            return [myriad2InvertedIslandEnables]
        }
    }

    proc isIslandEnabled {islandNum} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        set islandStatus [expr [peek $pwrIslandStatusAdr] ^ [myriad2InvertedIslandStatus]]

        if {[isBitSet $islandNum $islandStatus] == 1} {
            return "ON "
        } else {
            return "OFF"
        }
    }

    proc isIslandTrickleEnabled {islandNum} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        set islandTrickleStatus [expr [peek $pwrIslandTrickleEnAdr] ^ [myriad2InvertedTrickleEnables]]

        if {[isBitSet $islandNum $islandTrickleStatus] == 1} {
            return "TRICKLE_ON "
        } else {
            return "TRICKLE_OFF"
        }
    }

    proc isIslandIsolated {islandNum} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        set islandIsoStatus [peek $pwrIsoEnAdr]

        if {[isBitClear $islandNum $islandIsoStatus] == 1} {
            return "NOT_ISOLATED"
        } else {
            return "ISOLATED    "
        }
    }

    proc islandTrickle {action number} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        set trickle [expr [peek $pwrIslandTrickleEnAdr] ^ [myriad2InvertedTrickleEnables]]

        if { $action == "enable" } {
            puts "Enable Trickle to island $number"
            set trickle [setBitRange $trickle $number $number 1]
        } elseif { $action == "disable"} {
            puts "Disable Trickle to island $number"
            set trickle [setBitRange $trickle $number $number 0]
        }
        poke $pwrIslandTrickleEnAdr [expr $trickle ^ [myriad2InvertedTrickleEnables]]
    }

    proc islandIso {action number} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        if { $action == "enable" } {
            puts "Enable Isolation for island $number"
            setBit $pwrIsoEnAdr $number
        } elseif { $action == "disable"} {
            puts "Disable Isolation for island $number"
            clearBit $pwrIsoEnAdr $number
        }
    }

    proc islandPwrRaw {action number} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        set island_en [expr [peek $pwrIslandEnableAdr] ^ [myriad2InvertedIslandEnables]]

        if { $action == "enable" } {
            puts "Enable Power for island $number"
            set island_en [setBitRange $island_en $number $number 1]
        } elseif { $action == "disable"} {
            puts "Disable Power for island $number"
            set island_en [setBitRange $island_en $number $number 0]
        }

        poke $pwrIslandEnableAdr [expr $island_en ^ [myriad2InvertedIslandEnables]]
    }

    proc getIslandNameByNumber {islandNumber} {

        dict set islandNameLookup 0    "CSS_DIGITAL "
        if {[isMA2100]} {
            dict set islandNameLookup 1    "CSS_ANALOG  "
            dict set islandNameLookup 2    "RETENTION   "
        } else {
            dict set islandNameLookup 1    "RETENTION   "
            dict set islandNameLookup 2    "USB         "
        }
        dict set islandNameLookup 3    "SHAVE_0     "
        dict set islandNameLookup 4    "SHAVE_1     "
        dict set islandNameLookup 5    "SHAVE_2     "
        dict set islandNameLookup 6    "SHAVE_3     "
        dict set islandNameLookup 7    "SHAVE_4     "
        dict set islandNameLookup 8    "SHAVE_5     "
        dict set islandNameLookup 9    "SHAVE_6     "
        dict set islandNameLookup 10   "SHAVE_7     "
        dict set islandNameLookup 11   "SHAVE_8     "
        dict set islandNameLookup 12   "SHAVE_9     "
        dict set islandNameLookup 13   "SHAVE_10    "
        dict set islandNameLookup 14   "SHAVE_11    "
        dict set islandNameLookup 15   "PMB         "
        if {[isMA2100]} {
            dict set islandNameLookup 16   "MSS_DIGITAL "
            dict set islandNameLookup 17   "MSS_ANALOG  "
            dict set islandNameLookup 18   "DSS_DIGITAL "
            dict set islandNameLookup 19   "DSS_ANALOG  "
        } else {
            dict set islandNameLookup 16   "MSS_CPU     "
            dict set islandNameLookup 17   "MSS_AMC     "
            dict set islandNameLookup 18   "MSS_SIPP    "
            dict set islandNameLookup 19   "DSS         "
        }

        return [dict get $islandNameLookup $islandNumber]
    }

    proc getIslandNumberByName {islandName} {
        for {set i 0} {$i < 20} {incr i} {
            set currentName [string trim [getIslandNameByNumber $i]]
            if {[string equal $islandName $currentName]} {
                return $i
            }
        }
        return -1
    }

    proc island {{action help} {islandNameNum 0} args} {
        makeSureAONIsAccessible
        if {[isMA2100]} {
            set pwrIsoEnAdr             [const CPR_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const CPR_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const CPR_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const CPR_PWR_ISLAND_STATUS]
        } else {
            set pwrIsoEnAdr             [const AON_PWR_ISO_EN0_ADR]
            set pwrIslandEnableAdr      [const AON_PWR_ISLAND_EN_ADR]
            set pwrIslandTrickleEnAdr   [const AON_PWR_ISLAND_TRICKLE_EN_ADR]
            set pwrIslandStatusAdr      [const AON_PWR_ISLAND_STATUS]
        }

        if {[string is integer $islandNameNum]} {
            set number $islandNameNum
        } else {
            set number [getIslandNumberByName $islandNameNum]
        }

        if {[llength $args] == 1} {
            set endIslandNameNum [lindex $args 0]
            if {[string is integer $endIslandNameNum]} {
                set endIsland $endIslandNameNum
            } else {
                set endIsland [getIslandNumberByName $endIslandNameNum]
            }
            for {set islandNum $number} {$islandNum <= $endIsland} {incr islandNum} {
                island $action $islandNum
            }
            return
        }

        switch $action {
            "status" {
                puts "Myriad2 Power Island Status:"
                puts "=========================================================="
                for {set i 1} {$i < 20} {incr i} {
                    puts "[format "%02d" $i]   [getIslandNameByNumber $i] : [isIslandEnabled $i] [isIslandIsolated $i] [isIslandTrickleEnabled $i]"
                }
            }
            "enable" {
                puts "Enabling power island [getIslandNameByNumber $number]"
                #  Enable Trickle (L), Enable Power (L), Disable Isolation (L)
                islandTrickle enable $number
                islandPwrRaw enable $number
                islandIso disable $number
            }
            "disable" {
                puts "Disabling power island [getIslandNameByNumber $number]"
                #  Enable Isolation (H), Disable Trickle (H), Disable Power (H)
                islandIso enable $number
                islandTrickle disable $number
                islandPwrRaw disable $number
            }
            default {
                puts "usage: island <status | enable | disable> \[islandNum 1-19)\]"
            }
        }
    }

    proc isMA2100 {} {
        expr {[MV_SOC_REV] == "ma2100"}
    }

    proc isMA2x5x {} {
        string match ma2?5? [MV_SOC_REV]
    }

    proc cprGetBootString {bootCode} {
        if {[isMA2100]} {
            switch $bootCode {
                "0x00" { set bootString "UART     PLL_OFF GPIO:12,13,14,15" }
                "0x01" { set bootString "I2CM     PLL_OFF GPIO:60,61" }
                "0x02" { set bootString "I2CS     PLL_OFF GPIO:60,61" }
                "0x03" { set bootString "SPIM_16  PLL_OFF GPIO:74,75,76,77" }

                "0x05" { set bootString "SPIS     PLL_OFF GPIO:74,75,76,77" }
                default { set bootString "INVALID BootCode: $bootCode"}
            }
        } elseif {[isMA2x5x]} {
            switch $bootCode {

                "0x00"      { set bootString "SPIME_32bit   Range_B  GPIO:74,75,76,77   IFCFG: 0x00280b0000000180" }
                "0x01"      { set bootString "USBD          Range_C  GPIO:N/A           IFCFG: 0x000000000400c400" }
                "0x02"      { set bootString "USBD          Range_B  GPIO:N/A           IFCFG: 0x000000000400c400" }
                "0x03"      { set bootString "USBD          Range_D  GPIO:N/A           IFCFG: 0x000000002000a800" }
                "0x04"      { set bootString "SPIME_24bit   PLL_OFF  GPIO:74,75,76,77   IFCFG: 0x0000000001000100" }
                "0x05"      { set bootString "SPIME_32bit   Range_C  GPIO:74,75,76,77   IFCFG: 0x00280b0000000180" }
                "0x06"      { set bootString "DEBUG_HALT1   PLL_OFF  GPIO:N/A           IFCFG: 0x0000000000000001" }
                "0x07"      { set bootString "SPIME_24bit   Range_B  GPIO:74,75,76,77   IFCFG: 0x00280b0000000100" }
                "0x08"      { set bootString "UART2         Range_B  GPIO:15,14,13,12   IFCFG: 0x0000000020600271" }
                "0x09"      { set bootString "IICME         Range_B  GPIO:60,61         IFCFG: 0x070002906c01a800" }
                "0x0A"      { set bootString "IICS          Range_B  GPIO:60,61         IFCFG: 0x0007002900003f18" }
                "0x0B"      { set bootString "SPIM_16bit    Range_B  GPIO:74,75,76,77   IFCFG: 0x0000000001000080" }
                "0x0C"      { set bootString "SPIM_24bit    Range_B  GPIO:74,75,76,77   IFCFG: 0x0000000001000100" }
                "0x0D"      { set bootString "SPIS          Range_B  GPIO:74,75,76,77   IFCFG: 0x0000000000000000" }
                "0x0E"      { set bootString "IICS          Range_C  GPIO:12,13         IFCFG: 0x0007002900003f1a" }
                "0x0F"      { set bootString "IICS          Range_C  GPIO:12,13         IFCFG: 0x0005001e00002f1a" }
                "0x10"      { set bootString "UART          Range_C  GPIO:15,14,13,12   IFCFG: 0x0000000020800753" }
                "0x11"      { set bootString "IICME         Range_C  GPIO:60,61         IFCFG: 0x050001e050013400" }
                "0x12"      { set bootString "IICS          Range_C  GPIO:60,61         IFCFG: 0x0005001e00002f18" }
                "0x13"      { set bootString "SPIM_16bit    Range_C  GPIO:74,75,76,77   IFCFG: 0x0000000001000080" }
                "0x14"      { set bootString "SPIM_24bit    Range_C  GPIO:74,75,76,77   IFCFG: 0x0000000001000100" }
                "0x15"      { set bootString "SPIS          Range_C  GPIO:74,75,76,77   IFCFG: 0x0000000000000000" }
                "0x16"      { set bootString "SPIM_24bit    Range_B  GPIO:78,79,80,82   IFCFG: 0x0000000001000121" }
                "0x17"      { set bootString "SPIS          Range_B  GPIO:78,79,80,73   IFCFG: 0x0000000000000001" }
                "0x18"      { set bootString "SPIM_24bit    Range_B  GPIO:18,19,17,16   IFCFG: 0x0000000001000102" }
                "0x19"      { set bootString "SPIS          Range_B  GPIO:18,19,17,16   IFCFG: 0x0000000000000002" }
                "0x1A"      { set bootString "SPIM_24bit    Range_B  GPIO:8,9,10,11,    IFCFG: 0x0000000001000103" }
                "0x1B"      { set bootString "SPIS          Range_B  GPIO:8,9,10,11,    IFCFG: 0x0000000000000003" }
                "0x1C"      { set bootString "SPIM_24bit    Range_B  GPIO:12,13,14,15   IFCFG: 0x0000000001000104" }
                "0x1D"      { set bootString "SPIS          Range_B  GPIO:12,13,14,15   IFCFG: 0x0000000000000004" }
                "0x1E"      { set bootString "SPIM_24bit    Range_B  GPIO:60,61,62,63   IFCFG: 0x0000000001000106" }
                "0x1F"      { set bootString "SPIS          Range_B  GPIO:60,61,62,63   IFCFG: 0x0000000000000006" }
                default { set bootString "INVALID BootCode: $bootCode"}
            }
        }

        return $bootString
    }

    proc cprGetBootMode {} {
        set cprBootModeAddr     [const CPR_BOOT_MODE_ADR]
        set cprBootModeValue    [peek $cprBootModeAddr]
        set bootCode   0x[format %02X [extractBits $cprBootModeValue 1 5]]
        set wakeup     [extractBits $cprBootModeValue 0 0]
        set bootGpio64  [extractBits $cprBootModeValue 6 6]
        set bootGpio65  [extractBits $cprBootModeValue 7 7]
        set bootString  [cprGetBootString $bootCode]
        if {$wakeup == 0} {
            set bootType "GPIO Boot"
        } else {
            set bootType "OTP boot"
        }
        puts "BOOT_TYPE: $bootType BOOT_MODE:($bootCode) $bootString"
    }

    # Returns the boot device id
    proc cprGetBootCode {} {
        set cprBootModeAddr     [const CPR_BOOT_MODE_ADR]
        set cprBootModeValue    [peek $cprBootModeAddr]
        set bootCode   0x[format %02X [extractBits $cprBootModeValue 1 5]]

        return $bootCode
    }

    proc cprListBootModes {} {

        for {set i 0} {$i < 32} {incr i} {
            set bootCode 0x[format %02X $i]
            puts "BOOT_CODE: $bootCode => [cprGetBootString $bootCode]"
        }

    }

    proc cprBoot {args} {
        set result ""

        if {[llength $args] == 0} {
            puts "Usage: cpr boot <getMode|listModes>"
            return;
        }
        set cmd [lindex $args 0]
        switch $cmd {
            "status"   {
                cprGetBootMode
            }
            "listModes" {
                cprListBootModes
            }
            # Get Mode returns the raw mode value
            "getMode" {
                set result [cprGetBootCode]
            }
            default {
                puts "Usage: cpr boot <showMode|listModes|getMode>"
            }
        }
        return $result
    }

    proc cprEnableDisable {action args} {
        if {[llength $args] == 1} {
            # Assume command of the form
            # clock $action CSS_I2C0
            if {[string is integer [lindex $args 0]]} {
                puts "Error: Please use clock name not number"
            } else {
                if {([lindex $args 0] == "ALL") ||([lindex $args 0] == "all") } {
                    if {$action == "enable"} {
                        puts "Enabling all Myriad2 System Clocks"
                        cpr enable css   0 64 quiet
                        cpr enable mss   0 32 quiet
                        cpr enable sipp  0 32 quiet
                        cpr enable upa   0 32 quiet
                    } else {
                        puts "I'm sorry Dave, I'm afraid I can't do that"
                    }
                } else {
                    set clockResult [clockLookup name [lindex $args 0]]
                    #puts "clockSet $action [lindex $clockResult 0] [lindex $clockResult 1]"
                    clockSet $action [lindex $clockResult 0] [lindex $clockResult 1] quiet
                }
            }
        } elseif {[llength $args] == 2} {
            # Assume command of the form
            # clock $action css 19
            clockSet $action [lindex $args 0] [lindex $args 1]
        } elseif {([llength $args] == 3) || ([llength $args] == 4)} {
            # Assume command of the form
            # clock $action css 19 23
            # i.e. a range set
            for {set i [lindex $args 1]} {$i < [lindex $args 2]} {incr i} {
                dict set clockArray {*}[clockLookup getClockArray [lindex $args 0]]
                #foreach val [dict keys $clockArray] {
                #    puts "$val:[dict get $clockArray $val]"
                #}
                if {[dict exists $clockArray $i]} {
                    #puts "clockSet $action [lindex $args 0] $i"
                    # Note: Last argument is the optional verbose specifier
                    clockSet $action [lindex $args 0] $i [lindex $args 1]
                }
            }
        }
    }

    proc cprSummary {} {
        pll 0
        pll 1
        showResets
        sysClocks
        auxClocks
    }

    proc cprGrep {{queryRegex .*}} {
        variable MV_SOC_REV
        puts "cpr grep not implemented for MV_SOC_REV=$MV_SOC_REV"
    }

} ; # namespace eval mdbg::cpr

