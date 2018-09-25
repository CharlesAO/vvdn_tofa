# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf_log.tcl
# Description: TCF logging support for moviDebug2 shell
# Created on : Jun 2, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::tcf {
    variable TCF_LOG_FILE {}
    proc log {on/off {filename {}}} {
        variable TCF_LOG_FILE
        variable TCF_LOG_CHAN_RD
        switch -- ${on/off} {
            on {
                set prevLogFile $TCF_LOG_FILE
                if [string length $filename] {
                    set TCF_LOG_FILE [open $filename w]
                } else {
                    set TCF_LOG_FILE {}
                }
                if {[string length $prevLogFile ] && ![string match std* $prevLogFile]} {
                    close $prevLogFile
                }
                chan event $TCF_LOG_CHAN_RD readable {
                    set TCF_LOG_FILE  $::mdbg::tcf::TCF_LOG_FILE
                    set block [chan read $::mdbg::tcf::TCF_LOG_CHAN_RD]
                    if [string length $TCF_LOG_FILE] {
                        chan puts -nonewline $TCF_LOG_FILE $block
                    } else {
                        chan puts -nonewline $::mdbg::Channels::tcflog $block
                    }
                }
            }
            off {
                chan event $TCF_LOG_CHAN_RD readable {
                    chan read $::mdbg::tcf::TCF_LOG_CHAN_RD
                }
            }
            default {
                error "wrong argument `${on/off}`: must be `on` or `off`"
            }
        }
    }
}
