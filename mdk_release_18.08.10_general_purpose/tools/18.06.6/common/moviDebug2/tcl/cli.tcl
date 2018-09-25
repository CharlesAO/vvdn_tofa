# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli.tcl
# Description: Loads the moviDebug2 command implementation files
# Created on : Jun 16, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    # Load scripts from subdir using global namespace and bottom stack frame
    apply {list {
        set dir [file join [file dirname [info script]] cli]
        foreach file $list {
            uplevel #0 source [file join $dir $file.tcl]
        }
    } ::} {
        Puts
        Text
        ArgumentParser
        HelpManager
        Monochrome
        TabularDisplay

        common
        help
        binary

        platform
        registers
        loadfile
        savefile
        target
        run
        dasm
        halt
        continue
        wait
        breakpoint
        linenumbers
        step
        symbols
        memory
        memmap
        jtag
        pipe
        state
        state-reg
        callstack
        history
        uart
        cpr
        gpio
        unsupported/leoncallstack
        utility
        coreconfig
        vcshooks

        DisplayState
        mutex
    }

    catch {
        target [cli::Target::getRootContext]
        target [tcf::Platform::getMainCore [target]]
    }
} ; # namespace eval mdbg
