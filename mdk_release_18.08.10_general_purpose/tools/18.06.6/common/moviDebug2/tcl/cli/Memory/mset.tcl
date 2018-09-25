# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : mset.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    namespace eval cli::Memory {
        proc formatEnum {valueOrName symInfoVar symCache} {
            upvar 1 $symInfoVar symInfo

            if {![string is integer -strict $valueOrName]} {
                if {[dict exists $symInfo TYPE_ID]} {
                    set typeId [dict get $symInfo TYPE_ID]
                    foreach enumId [$symCache getChildren $typeId] {
                        lassign [$symCache getFieldList $enumId {
                            CLASS NAME VALUE}] symClass enumName enumValue
                        if {$valueOrName == $enumName} {
                            if {$symClass ni {{} VALUE}} {
                                cli::Warning "enum value found but has class $symClass"
                            }
                            return [cli::Binary::formatBase64 $enumValue]
                        }
                    }
                }
            }
            set size [dict get $symInfo SIZE]
            cli::Binary::formatInt $size $valueOrName
        }
    } ; # namespace eval cli::Memory

    proc mset {args} {
        set memCmd set
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

                    switch -exact $dataType {
                        ENUMERATION {
                            set data [cli::Memory::formatEnum $what symInfo $symCache]
                            cli::Memory::writeBinaryValue symInfo $data
                        }
                        COMPOSITE -
                        ARRAY {
                            set typeName [cli::Memory::getTypeName $symInfo $symCache]
                            return -code error "set -type `$typeName` not implemented"
                        }
                        FUNCTION {
                            set addr [dict get $symInfo ADDRESS]
                            return -code error "set -type `function @$addr` not allowed"
                        }
                        default {
                            set binaryClass [cli::Memory::binaryTypeClass $dataType]
                            set data [cli::Binary::formatData $binaryClass $dataSize $what]
                            cli::Memory::writeBinaryValue symInfo $data
                        }
                    }
                } finally {
                    cli::Memory::cleanupSymInfo symInfo
                }
            } ; # cli::UsingObj
        }
    }

    shell::autocomplete::addCommand {mset} [cli::Memory::ArgumentParser SubCmdParser set] autocomplete
    cli::Help::Manager add mset -parser [cli::Memory::ArgumentParser SubCmdParser set]

} ; # namespace eval mdbg