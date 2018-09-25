# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : classic.tcl
# Description: moviDebugTcl (classic moviDebug Tcl library) support
#              All commands in the mdbg::classic namespace
# Created on : Jul 8, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::classic {

    #
    #  Load and initialise moviDebugTcl Classic in separate interpreter
    #
    proc init {moviSim|asic {myriad myriad2} args} {

        variable ID
        if [::info exists ID] {
            return -code error "Already initialised"
        }

        # Create separate interpreter
        ::set ID [interp create -safe]
        interp marktrusted $ID
        interp expose $ID load

        # Determine path from path of loaded moviDebug2.so (should be next to it)
        ::set loaded [info loaded]
        ::set module [lsearch -inline -glob $loaded *Movidebug]
        ::set dir [file dirname [lindex $module 0]]
        ::set classicDll [file join $dir moviDebugTcl[info sharedlibextension]]

        if {[::set code [catch {
                    interp eval $ID [list load $classicDll]
                    interp eval $ID [list mdbg::init ${moviSim|asic} $myriad {*}$args]
                    ::set commandList [interp eval $ID {info commands mdbg::*}]
                } error options ]]} {
            interp delete $ID
            ::unset ID
            return -code $code -options $options $error
        }

        # create aliases
        foreach command $commandList {
            if {$command in {::mdbg::init}} {
                continue
            }
            uplevel 1 namespace inscope :: [list \
                interp alias {} [namespace current]::[namespace tail $command] $ID $command
            ]
        }
    }

    proc done {} {
        ::variable ID
        if {![::info exists ID]} {
            ::return -code error "Not initialised"
        }
        ::set commandList [interp eval $ID {info commands mdbg::*}]
        foreach command $commandList {
            if {$command in {::mdbg::init}} {
                continue
            }

            if {[catch {
                uplevel 1 namespace inscope :: [
                list interp alias {} [namespace current]::[namespace tail $command] {}
                ]
            } error]} {
                puts stderr $error
            }
        }

        ::interp delete $ID
        ::unset ID
    }

    proc ImportNamespacePath {} {

        ::set ns [namespace current]
        ::set path [uplevel 1 namespace path]
        if {$ns ni $path} {
            lappend path $ns
            uplevel 1 [list namespace path $path]
        }
    }

}
