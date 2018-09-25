# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/Puts.tcl
# Description: Output methods
# Created on : Mar 12, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli {

    proc Puts {str} {
        puts $str
    }

    proc NewLine {} {
        puts {}
    }

    proc Print {str} {
        puts -nonewline $str
    }

    proc PutError {str} {
        puts stderr $str
    }

    proc NewErrorLine {} {
        puts stderr {}
    }

    #
    #  Formatted output methods
    #

    proc Putf {format args} {
        Puts [format $format {*}$args]
    }

    proc Printf {format args} {
        Print [format $format {*}$args]
    }

    proc PutfError {format args} {
        PutError [format $format {*}$args]
    }

    #
    #  Commonly used special output methods (un- & formatted)
    #

    proc Warning {string} {
        PutWarning "Warning: $string"
    }

    proc Verbose {string} {
        if {[IsFlagVarTrue ::mdbg::verbose]} {
            puts $::mdbg::Channels::verbose $string
        }
    }

    proc Verbosef {format args} {
        if {[IsFlagVarTrue ::mdbg::verbose]} {
            Verbose [format $format {*}$args]
        }
    }

    proc PutInfo {string} {
        puts $::mdbg::Channels::info $string
    }

    proc PutfInfo {format args} {
        PutInfo [format $format {*}$args]
    }

    proc PutResult {string} {
        puts $::mdbg::Channels::result $string
    }

    proc PutfResult {format args} {
        PutResult [format $format {*}$args]
    }

    proc PutDebug {string} {
        puts $::mdbg::Channels::debug $string
    }

    proc PutfDebug {format args} {
        PutDebug [format $format {*}$args]
    }

    proc PutWarning {string} {
        puts $::mdbg::Channels::warning $string
    }

    proc PutfWarning {format args} {
        PutWarning [format $format {*}$args]
    }

    proc PutEvent {message} {
        puts $::mdbg::Channels::event $message
    }

    proc PutfEvent {format args} {
        puts PutEvent [format $format {*}$args]
    }

    #
    #   Terminal input/output and colours
    #

    proc PutColor {color text} {
        catch {
            set attr [chan configure stdout -textattr]
        }
        if {[info exists attr]} {
            chan configure stdout -textattr $color
        }
        try {
            puts stdout $text
        } finally {
            if {[info exists attr]} {
                chan configure stdout -textattr $attr
            }
        }
    }

    proc Gets {prompt} {
        puts -nonewline stdout $prompt
        flush stdout
        gets stdin
    }

    proc ColorGets {color prompt} {
        catch {
            set attr [chan configure stdout -textattr]
        }
        if {[info exists attr]} {
            chan configure stdout -textattr $color
        }
        try {
            Gets $prompt
        } finally {
            if {[info exists attr]} {
                chan configure stdout -textattr $attr
            }
        }
    }

    proc ColorGetsWithEvents {color prompt} {
        catch {
            set attr [chan configure stdin -events]
        }
        if {[info exists attr]} {
            chan configure stdin -events 1
            after 0 {
                chan configure stdin -events 0
            }
        }
        try {
            ColorGets $color $prompt
        } finally {
            if {[info exists attr]} {
                chan configure stdin -events $attr
            }
        }
    }

    proc IsEclipseTerminal { {black|white {}} } {
        upvar #0 ::mdbg::shell::ECLIPSE_TM_TERMINAL terminal
        expr {
            [info exists terminal] && (
                ${black|white} == "" ||
                $terminal == ${black|white}
            )
        }
    }

    proc RemapColor {color} {
        set Proc [info commands ::mdbg::shell::RemapColorInEclipseTerminal]
        if {$Proc == ""} {
            return $color
        }
        tailcall $Proc $color
    }

    proc ConsoleWidth {} {
        if {[info command ::mdbg::shell::console] != ""} {
            catch {
                set cols [::mdbg::shell::console cols]
            }
        }
        if {   [info exists cols]
            && [string is integer -strict $cols]
            && $cols > 0
        } {

            return $cols
        }
        return 0
    }

} ; # namespace eval cli