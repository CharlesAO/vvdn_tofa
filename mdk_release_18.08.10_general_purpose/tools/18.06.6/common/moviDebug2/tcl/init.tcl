# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : init.tcl
# Description: moviDebug2 Shell init script
# Created on : May 25, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::shell {

    # Load scripts from subdirectory using global namespace and bottom stack frame
    apply {list {
        set dir [file join [file dirname [info script]] shell]

        foreach item $list {
            if {[string index $item 0] == "#"} {
                continue
            }

            lassign $item fileList options

            if {[lindex $options 0] == "if"} {
                set condition [lindex $options 1]
                if {![uplevel #0 [list expr $condition]]} {
                    continue
                }
                set options [lrange $options 2 end]
            }

            foreach file $fileList {
                set LOAD_SCRIPT [list source [file join $dir $file.tcl]]

                if {[lindex $options 0] == "catch"} {
                    set LOAD_SCRIPT [list catch $LOAD_SCRIPT]
                }

                uplevel #0 $LOAD_SCRIPT
            }
        }
    } ::} {
        ParamSplitter
        autocomplete
        eclipse_terminal
        tcf_log
        commands

        {classic                {if {[info exists ::mdbg::CLASSIC_TCL] && $::mdbg::CLASSIC_TCL}}}
        {{bricks serpent}        catch}
    }
}
