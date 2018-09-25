# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/memory.tcl
# Description: Getting and setting data from memory/registers
# Created on : Sep 14, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Memory {

        namespace path ::mdbg

        apply {{list} {
            set dir [file dirname [info script]]
            foreach file $list {
                uplevel #0 source [file join $dir Memory $file.tcl]
            }
        } ::} {
            ArgumentParser

            SymInfo
            SymCache
            SymInfoDisplay

            BinaryAccess

            mget
            mset
            mfill
            mdump

            interactive

            myriad2-local-registers
        }

    } ; # namespace eval cli::Memory

    proc mem {memCmd args} {
        cli::NoErrorStack {
            switch -exact $memCmd {
                get - set - fill - dump {
                    set mMemCmd "m${memCmd}"
                    cli::DeprecatedWarning $mMemCmd 1
                    $mMemCmd {*}$args
                }
                default {
                    return -code error {invalid subcommand: must be "get", "set", "fill", or "dump"}
                }
            }
        }
    }

    shell::autocomplete::addCommand {mem} cli::Memory::ArgumentParser autocomplete

    cli::Memory::BUILD_LOCAL_REGISTER_MAP

    cli::Alias mgetv mget -value
    cli::Alias mgetb mget -binary
    cli::Alias mgets mget -asciiz

    namespace export mget mset mdump

} ; # namespace eval mdbg
