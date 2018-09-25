# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : mdump.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    namespace eval cli::Memory {

    } ; # namespace eval cli::Memory

    proc mdump {args} {
        set memCmd dump
        set parser [cli::Memory::ArgumentParser SubCmdParser $memCmd]
        $parser parseCallerArgs 1 $args

        set size [expr {4 * $count}]
        set options [list -quiet -type binary -count 1 -size $size]
        if {$jtag} {
            lappend options -jtag
        } elseif {[info exists memTarget]} {
            lappend options -mem $memTarget
        }
        if {[info exists target]} {
            lappend options -target $target
        }
        if {[info exists frame]} {
            lappend options -frame $frame
        }
        set result [lindex [mget $where {*}$options] 0]

        set value [dict get $result VALUE]
        set address [dict get $result ADDRESS]

        cli::Binary::forEachBlock 16 row $value {
            set numbers [cli::Binary::scanU32List $row "0x%.8X"]
            set ascii [cli::Binary::scanAscii $row]
            cli::Putf {[%.8X] = %-48s %s} $address $numbers $ascii
            incr address 16
        }
    }

    shell::autocomplete::addCommand {mdump} [cli::Memory::ArgumentParser SubCmdParser dump] autocomplete
    cli::Help::Manager add mdump -parser [cli::Memory::ArgumentParser SubCmdParser dump]

} ; # namespace eval mdbg