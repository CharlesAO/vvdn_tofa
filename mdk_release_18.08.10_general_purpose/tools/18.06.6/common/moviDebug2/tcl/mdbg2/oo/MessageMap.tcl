# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : MessageMap.tcl
# Description: Message map with formatted output support
# Created on : Aug 14, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::oo {

    oo::class create MessageMap {

        variable MESSAGES

        constructor {} {
            set MESSAGES [dict create]
        }

        method add {name text args} {
            dict set MESSAGES $name [string cat $text {*}$args]
        }

        method get {name} {
            dict get $MESSAGES $name
        }

        method format {name args} {
            format [my get $name] {*}$args
        }

    } ; # oo::class MessageMap

} ; # namespace eval mdbg
