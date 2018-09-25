# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : init.tcl
# Description: moviDebug2 Tcl Shared Library init script
# Created on : Mat 25, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
#

# Add missing autocomplete support
if {![namespace exists ::mdbg::shell::autocomplete]} {
    namespace eval ::mdbg::shell::autocomplete {

        proc addScript {cmdlist body} {
            # does nothing
        }

        proc addCommand {cmdlist autocmd args} {
            # does nothing
        }

    }
}

namespace eval mdbg::Channels {

    proc set? {varName def} {
        upvar 1 $varName var
        if {![info exists var]} {
            set var $def
        }
    }

    set? warning  stderr
    set? info     stdout

    set? result   $info
    set? verbose  $info

    set? debug    stdout
    set? event    stdout
    set? uart     stdout

    set? pipeout  stdout
    set? pipeerr  stderr

    set? vcshooks stdout
    set? tcflog   stderr
} ; # namespace eval mdbg::Channels

namespace eval mdbg {

    proc eval% {script args} {
        catch {update}
        set script [concat $script $args]
        set target [expr {
            [info exists ::mdbg::TARGET] ? $::mdbg::TARGET : ""
        }]
        puts $::mdbg::Channels::debug "$target% $script"
        set prevNsPath [uplevel 1 namespace path]
        set missingMdbgNs [expr {"::mdbg" ni $prevNsPath}]
        try {
            if {$missingMdbgNs} {
                uplevel 1 [list namespace path [concat ::mdbg $prevNsPath] ]
            }
            set result [uplevel 1 $script]
            if {$result != {}} {
                puts $::mdbg::Channels::result $result
            }
            set result
        } finally {
            if {$missingMdbgNs} {
                uplevel 1 [list namespace path $prevNsPath]
            }
        }
    }

    namespace export eval%

} ; # namespace eval mdbg

namespace eval mdbg::dll {

    # Load scripts from subdirectory using global namespace and bottom stack frame
    apply {list {
        set dir [file join [file dirname [info script]] mdbg2]
        foreach file $list {
            if {[string index $file 0] == "#"} {
                continue
            }
            uplevel #0 [list source [file join $dir $file.tcl]]
        }
    } ::} {
        registers
        flags

        #getMemBlock

        oo/Event
        oo/LazyFactory
        oo/DefineMap
        oo/MessageMap

        memory/BlockFormatter
        memory/JtagMem

        pipe/Queue
        pipe/DebugPipe
        pipe/Manager

        vcshooks/log
    }

} ; # namespace eval mdbg::dll

