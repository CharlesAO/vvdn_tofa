# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : mfill.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    namespace eval cli::Memory {

    } ; # namespace eval cli::Memory

    proc mfill {args} {
        set memCmd fill
        set parser [cli::Memory::ArgumentParser SubCmdParser $memCmd]
        $parser parseCallerArgs 1 $args
        cli::NoErrorStack {
            cli::Memory::ArgumentParser setupArgs

            cli::UsingObj [cli::Memory::SymbolCacheClass] symCache {
                cli::Memory::ArgumentParser createSymInfo
                cli::Memory::fillSymTypeAddrSize symInfo $symCache
                cli::Memory::ArgumentParser fillSymInfo

                try {
                    set dataType [dict get $symInfo TYPE]
                    set dataSize [dict get $symInfo SIZE]

                    if {$zero} {
                        set data [binary format x]
                    } else {
                        if {[info exists tclBinaryFormat]} {
                            set data [binary format $tclBinaryFormat {*}$data]
                        } else {
                            if {[llength $data] > 1} {
                                return -code error "too many data arguments"
                            }
                            set data [binary format a* [lindex $data 0]]
                        }
                    }
                    if {[info exists srcSliceLength]} {
                        if {[info exists srcSliceOffset]} {
                            set data [string range $data $srcSliceOffset [expr {$srcSliceOffset + $srcSliceLength - 1}]]
                        } else {
                            set data [string range $data 0 [expr {$srcSliceLength - 1}]]
                        }
                        if {[string length $data] != $srcSliceLength} {
                            return -code error "insufficient source slice data"
                        }
                    } else {
                        if {[info exists srcSliceOffset]} {
                            set data [string range $data $srcSliceOffset end]
                        }
                    }
                    if {[string length $data] == 0} {
                        return -code error "empty data"
                    }

                    cli::Memory::writeBinaryValue symInfo $data

                } finally {
                    cli::Memory::cleanupSymInfo symInfo
                }
            } ; # cli::UsingObj
        }
    }

    shell::autocomplete::addCommand {mfill} [cli::Memory::ArgumentParser SubCmdParser fill] autocomplete
    cli::Help::Manager add mfill -parser [cli::Memory::ArgumentParser SubCmdParser fill]

} ; # namespace eval mdbg