# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : interactive.tcl
# Description: Interactive mget
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {
    namespace eval cli::Memory {
        namespace eval Autocomplete {

        } ; # namespace eval Autocomplete

        proc setInteractiveAutocomplete {baseopts opts} {
            set origCmdName ::mdbg::shell::autocomplete
            if {[info commands $origCmdName] == ""} {
                return
            }
            set newCmdNamePrefix [namespace current]::Autocomplete::autocomplete
            set i 1
            while {[info commands [set newCmdName $newCmdNamePrefix#$i]] != ""} {
                incr i
            }
            ::rename $origCmdName $newCmdName

            uplevel #0 "
                proc mdbg::shell::autocomplete {line} {
                    mdbg::cli::Memory::InteractiveAutocomplete \$line [list $baseopts] [list $opts]
                }
            "

            return $newCmdName
        }
        proc restoreAutocomplete {cmdName} {
            if {$cmdName != "" && [info commands $cmdName] != ""} {
                set origCmdName ::mdbg::shell::autocomplete
                ::rename $origCmdName ""
                ::rename $cmdName $origCmdName
            }
        }

        proc Autocomplete-mget/i {cmd params prefix} {
            set Parser [ArgumentParser SubCmdParser get]
            $Parser autocomplete mget [concat {""} $params] $prefix
        }

        proc InteractiveAutocomplete {line baseopts opts} {
            cli::CatchAndPrintError {
                if {[regexp {^((?:[^/]+|/[^/]+)*(//+))(.*)$} $line -> lineprefix // options]} {
                    regexp {^\s*(.*?)(\s*)$} $options -> options space
                    if {$space == "" && [llength $options] > 0} {
                        set prefix [lindex $options end]
                        set options [lrange $options 0 end-1]
                    } else {
                        set prefix ""
                    }
                    if {[string match ///* ${//}]} {
                         set params $options
                    } else {
                        set params [concat $baseopts $options]
                    }
                    if {[llength $options]} {
                        append lineprefix " $options "
                    } else {
                        append lineprefix " "
                    }
                    set suggestions [Autocomplete-mget/i {} $params $prefix]
                } else {
                    regexp -nocase {^(.*?)(\w*)$} $line -> lineprefix prefix
                    set suggestions [ArgumentParser autocomplete-location [concat $baseopts $opts] $prefix]
                }
                set result [list]
                foreach word $suggestions {
                    lappend result "$lineprefix$word"
                }
                return $result
            }
        }
    } ; # namespace eval cli::Memory

    proc mget/i {args} {
        cli::NoErrorStack {
            cli::PutInfo "-------------------------------------------------------------------"
            cli::PutInfo "Entering interactive `mget` session, leave with Ctrl-D or no input."
            cli::PutInfo "- Multiple locations should be separated by \";\""
            cli::PutInfo "- Change `mget` options using \"... // -option \[value\]...\""
            cli::PutInfo "-------------------------------------------------------------------"

            set textattr {blue bold}
            set target ""
            if {[info exists ::mdbg::TARGET]} {
                set target $::mdbg::TARGET
                if {[state -issuspended]} {
                    lassign [state -pc -sym -basedir -file -line] pc sym basedir file line
                    set context "IP=$pc"
                    if {$sym != ""} {
                        append context " <$sym>"
                    }
                    if {$file != ""} {
                        append context ", $file:$line"
                    }
                    cli::PutInfo $context
                }
            }
            set baseopts $args
            set opts ""
            try {
                while true {
                    set command mget
                    if {[llength $baseopts]} {
                        append command " $baseopts"
                    }
                    if {[llength $opts]} {
                        append command " $opts"
                    }
                    cli::PutInfo "$target% $command"

                    set completionCmdName [cli::Memory::setInteractiveAutocomplete $baseopts $opts]
                    try {
                        set input [cli::ColorGetsWithEvents $textattr ">>> "]
                    } finally {
                        cli::Memory::restoreAutocomplete $completionCmdName
                    }

                    if {$input == ""} {
                        break;
                    }

                    regexp {^\s*(.*?)\s*(?:(//+)\s*(.*)\s*)?$} $input -> location // newopts

                    switch -glob ${//} {
                        // {
                            set opts $newopts
                        }
                        ///* {
                            set baseopts $newopts
                            set opts ""
                        }
                        "" {
                            if {$location == ""} {
                                unset -nocomplain lastLocation
                                continue
                            }
                        }
                    }
                    if {$location == ""} {
                        if {![info exists lastLocation]} {
                            continue
                        }
                        set location $lastLocation
                    } else {
                        set lastLocation $location
                    }
                    foreach location [split $location ";"] {
                        if {![string is space $location]} {
                            cli::CatchAndPrintError {
                                mdbg::eval% [list mget {*}$baseopts {*}$opts [string trim $location]]
                            }
                        }
                    }
                }
            } finally {
                cli::PutInfo "-----------------------------------"
                cli::PutInfo "Leaving interactive `mget` session."
                cli::PutInfo "-----------------------------------"
            }
        }
    }

    shell::autocomplete::addCommand {mget/i} cli::Memory::Autocomplete-mget/i
    cli::Help::Manager add {mget/i} \
        -short "Interactive `mget` session" \
        -synopsis "mget/i ?options?..." \
        -args {
            options {
                -synopsis {?options?...}
                -short "Any option available for `mget` except `location(*)`"
                -long {
                    These will be the base options for the interactive `mget` session.
                }
            }
        } \
        -long {
            Start an interactive `mget` session.

            The standard input is read for locations separated by `;` or newline.
            Empty input or end-of-file (Ctrl-D) ends the session.
            Empty locations are ignored.

            `mget` is performed for each location separately, using the base options and additional parameters

            Text after C-style `//` comments modify the additional parameters for `mget`.
            Text after C-style `///` comments sets the base options and clears any additional parameters.

            Basic autocomplete is avaiable in the interactive `mget` shell.

            !!! Tcl event dispatch is blocked while user input is being entered!
        }

} ; # namespace eval mdbg