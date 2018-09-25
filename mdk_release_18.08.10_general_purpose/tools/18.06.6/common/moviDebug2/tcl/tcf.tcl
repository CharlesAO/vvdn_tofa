# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf.tcl
# Description: Loads the TCF/Tcl subsystem implementation
# Created on : Aug 13, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::tcf {

    # Load scripts from subdirectory using global namespace and bottom stack level
    apply {list {

        set dir [file join [file dirname [info script]] tcf]

        foreach item $list {
            # a primitive form of comments
            if {[string index $item 0] == "#"} {
                continue
            }

            set script $item.tcl

            set file [file join $dir $script]
            if {![file exists $file]} {
                puts $mdbg::Channels::warning "warning: tcf/$script not found!"
                continue
            }

            uplevel #0 [list source $file]
        }

    } ::} {
        json
        protocol
        fields

        Breakpoints
        StackTrace
        ContextQuery
        Disassembly
        Memory
        MemoryMap
        Processes
        Registers
        RunControl
        SysMonitor
        Symbols
        Expressions
        LineNumbers

        Platform
        MyriadCore

        BreakpointMonitor
        SymbolCache
        ExprObj

        utility
    }
}

