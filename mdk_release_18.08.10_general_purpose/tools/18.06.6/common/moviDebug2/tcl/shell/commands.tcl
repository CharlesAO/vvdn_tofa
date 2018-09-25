# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : commands.tcl
# Description: MoviDebug2 shell commonly used commands
# Created on : Jun 11, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::shell {

    if {$::tcl_platform(platform) == "windows"} {

        proc ls {args} {
            exec dir /w {*}$args
        }

        proc dir {args} {
            exec dir {*}$args
        }

        proc exec {command args} {
            ::exec cmd /c $command {*}$args <@stdin >@stdout 2>@stderr
        }

        proc cls {} {
            exec cls
        }

        proc clear {} {
            cls
        }

    } else {

        proc ls {args} {
            exec ls {*}$args
        }

        proc dir {args} {
            exec ls -l {*}$args
        }

        proc cls {} {
            clear
        }

        proc clear {} {
            exec clear
        }

        proc exec {command args} {
            ::exec sh -c [ShEscapeList $command {*}$args] <@stdin >@stdout 2>@stderr
        }

        proc ShEscapeList {args} {
            set result [list]
            foreach arg $args {
                lappend result [ShQuote $arg]
            }
            join $result " "
        }

        proc ShQuote {arg} {
            set arg [string map {\x27 {'\''}} $arg]
            return "'$arg'"
        }

    }

    proc make {args} {
        exec make {*}$args
    }

    proc cd {args} {
        ::tcl::cd {*}$args
        pwd
    }

    proc load {args} {
        if {[llength $args] == 1 && [string match {*.elf} [lindex $args 0]]} {
            if {[info exists ::mdbg::Channels::warning]} {
                upvar 0 ::mdbg::Channels::warning WARNCH
            } else {
                set WARNCH stderr
            }
            puts $WARNCH "Warning: loading Elf files into the target using the Tcl built-in \"load\" command is deprecated!"
            puts $WARNCH "         Please use the \"loadfile\" command or one of its variants (e.g \"loadelf\")."
            puts $WARNCH "Continuing in 5 seconds..."
            ::mdbg::wait -ms 5000
            ::mdbg::loadfile -elf [lindex $args 0]
        } else {
            uplevel 1 [list ::tcl::load {*}$args]
        }
    }

    autocomplete::addScript {
        load
    } {
        concat \
            [autocomplete::listOfFiles -types f -- $PREFIX*.elf $PREFIX*[info sharedlibextension]] \
            [autocomplete::listOfFiles -types d -- $PREFIX* ]
    }

    proc lasted {args} {
        set start [clock milliseconds]
        set result [uplevel 1 $args]
        set finish [clock milliseconds]
        if {[info exists ::mdbg::Channels::info]} {
            upvar ::mdbg::Channels::info INFOCH
        } else {
            set INFOCH stdout
        }
        puts $INFOCH "lasted [expr {$finish - $start}] ms"
        return $result
    }

    proc EXPORT_BUILTINS {args} {
        foreach cmd $args {
            if ![string length [info command ::tcl::$cmd]] {
                namespace inscope :: rename $cmd ::tcl::$cmd
                namespace export $cmd
                namespace inscope :: namespace import [namespace current]::$cmd
            }
        }
        rename EXPORT_BUILTINS ""
    }

    proc EXPORT {args} {
        foreach cmd $args {
            namespace export $cmd
            namespace inscope :: namespace import [namespace current]::$cmd
        }
        rename EXPORT ""
    }

    EXPORT ls dir make q quit lasted cls clear
    EXPORT_BUILTINS cd load
    # vwait

    autocomplete::addScript {cd} {
        autocomplete::listOfFiles -types d -- $PREFIX*
    }

} ; # namespace eval mdbg::shell

namespace eval mdbg {

    proc USING_NAMESPACE { args } {
        set upNsPath [uplevel 1 {namespace path}]
        foreach ns [list ::mdbg {*}$args] {
            if {$ns ni $upNsPath} {
                lappend upNsPath $ns
            }
        }
        uplevel 1 [list namespace path $upNsPath]
    }

} ; # namespace eval mdbg

interp alias {} ::mdbg::quit {} exit
interp alias {} ::mdbg::q {} exit

