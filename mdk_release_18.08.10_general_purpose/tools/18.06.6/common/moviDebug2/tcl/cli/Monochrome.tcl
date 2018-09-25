# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/Monochrome.tcl
# Description: Support to disable the blue colour of ResultTables.
#              Some users don't like them or have strange terminal colour
#              schemes where they're not clearly visible.
# Created on : Nov 18, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    # Monochrome is enabled by setting the mdbg::MONOCHROME to logical `true` or `false`
    # The system environment variable MV_DBG2_MONOCHROME determines the default value if not set.
    set MONOCHROME [expr {
        [info exists MONOCHROME] ? [string is true $MONOCHROME] :
        [info exists ::env(MV_DBG2_MONOCHROME)] && [string is true $::env(MV_DBG2_MONOCHROME)]
    }]
    namespace eval cli {
        # Components can subscribe to this event to receive notifications of change
        ::mdbg::oo::Event create MonochromeChanged
        # Whenever the variable is written to, the event will be raised
        trace add variable ::mdbg::MONOCHROME write [list ::mdbg::cli::MonochromeChanged raise]
    } ; # namespace eval cli

} ; # namespace eval mdbg
