# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : vcshooks/log.tcl
# Description: VCS Hooks log redirect support for moviDebug2
# Created on : Jul 24, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::vcshooks {
    variable VCS_LOG_FILE {}

    proc log {on/off {filename {}} {mode w}} {

        set READ_COUT {
            set block [chan read $::mdbg::vcshooks::VCS_LOG_COUT_RD]
        }
        set READ_CERR {
            set block [chan read $::mdbg::vcshooks::VCS_LOG_CERR_RD]
        }

        set FLUSH_UART {
            mdbg::cli::CatchAndPrintError {
                mdbg::uart::flushLine
            }
            mdbg::cli::CatchAndPrintError {
                mdbg::cli::Pipe::Manager FlushReadConsoleLines
            }
        }

        set WRITE_VCS_OUT {
            set DEFAULT_VCS_OUT $::mdbg::Channels::vcshooks
            if {[string length $DEFAULT_VCS_OUT]} {
                chan puts -nonewline $DEFAULT_VCS_OUT $block
            }
        }
        set WRITE_STDERR {
            chan puts -nonewline stderr $block
        }
        set WRITE_LOG {
            set VCS_LOG_FILE  $::mdbg::vcshooks::VCS_LOG_FILE
            if {[string length $VCS_LOG_FILE]} {
                chan puts -nonewline $VCS_LOG_FILE $block
            }
        }

        switch -- ${on/off} {
            on {
                set READ_COUT_EVENT "$READ_COUT\n$FLUSH_UART\n$WRITE_VCS_OUT\n$WRITE_LOG"
                set READ_CERR_EVENT "$READ_CERR\n$FLUSH_UART\n$WRITE_STDERR\n$WRITE_LOG"
            }
            off {
                set READ_COUT_EVENT "$READ_COUT\n$WRITE_LOG"
                set READ_CERR_EVENT "$READ_CERR\n$WRITE_LOG"
            }
            default {
                return -code error "wrong argument `${on/off}`: must be `on` or `off`"
            }
        }

        variable VCS_LOG_FILE
        variable VCS_LOG_COUT_RD
        variable VCS_LOG_CERR_RD

        set prevLogFile $VCS_LOG_FILE

        if {[string length $filename]} {
            set VCS_LOG_FILE [open $filename $mode]
        } else {
            set VCS_LOG_FILE {}
        }

        chan event $VCS_LOG_COUT_RD readable $READ_COUT_EVENT
        chan event $VCS_LOG_CERR_RD readable $READ_CERR_EVENT

        if { [string length $prevLogFile ] &&
            ![string match std* $prevLogFile]} {
            close $prevLogFile
        }
    }

    log on
}


