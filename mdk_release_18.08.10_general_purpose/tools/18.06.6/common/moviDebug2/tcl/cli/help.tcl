# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/help.tcl
# Description: CLI Help command
# Created on : Sep 30, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {
    namespace eval cli::Help {
        namespace path ::mdbg

        cli::HelpManager create Manager

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Show help"
            -args {
                -short {
                    -flag
                    -brief "Only show short descriptions."
                }
                -version {
                    -flag
                    -brief "Show moviDebug library version."
                }
                command {
                    -optional
                    -brief "Show help for specified command."
                    -description {
                        If missing, all simple commands will be listed.
                    }
                }
                subcommand {
                    -optional
                    -brief {
                        Show help for subcommand of `command` argument.
                    }
                    -description {
                        Can be glob pattern, e.g "*".
                    }
                }
                subsubcmd {
                    -optional
                    -hidden
                }
                args {
                    -var extraArgs
                    -brief "Extra arguments. Ignored."
                    -description {
                        This allows the user to prepend `help` to a full command line,\
                        without requiring to erase the extra provided arguments.
                        In the future the switches might be analysed to provide more\
                        specific information.
                    }
                }
            }
            -mutex {
                {-version {-short command subcommand}}
            }
            -arg-complete-scripts {
                {command subcommand subsubcmd} {
                    while {[string match {-*} [lindex $PARAMS 0]]} {
                        set PARAMS [lrange $PARAMS 1 end]
                    }
                    if {$PARAMS == {}} {
                        mdbg::cli::Help::Manager listSimpleCommands $PREFIX*
                    } elseif {[llength $PARAMS] < 2} {
                        mdbg::cli::Help::Manager listSubCommands [lindex $PARAMS end] $PREFIX*
                    } else {
                        mdbg::cli::Help::Manager listSubSubCommands {*}[lrange $PARAMS end-1 end] $PREFIX*
                    }
                }
            }
        }

        cli::TabularDisplay create HelpListDisplay {
            NAME {
                -header "COMMAND"
                -present
            }
            SHORT {
                -header "DESCRIPTION"
                -present
            }
        }
        oo::objdefine HelpListDisplay {
            mixin cli::ListResultTable
        }

        cli::TabularDisplay create HelpArgDisplay {
            {NAME SYNOPSIS} {
                -header {ARGUMENTS}
                -space-after 4
                -present
                -expr {
                    [string trim $SYNOPSIS] != {} ? $SYNOPSIS : $NAME
                }
                -format "    %s"
            }
            TEXT {
                -header {}
                -wrap word
                -expand
            }
        }

        oo::class create ArgTable {
            superclass cli::ListResultTable

            variable IsLong

            method display {list isLong} {
                set IsLong $isLong
                next $list
            }

            method Inline_PrintTable {} {
                upvar 1 TableHeader header
                upvar 1 TableRows rows
                my PutHeader $header
                if {$IsLong || true} {
                    mdbg::cli::NewLine
                }
                foreach row $rows {
                    my PutRow $row
                    if {$IsLong} {
                        mdbg::cli::NewLine
                    }
                }
            }
        }

        oo::objdefine HelpArgDisplay mixin ArgTable

        proc Wrapped {Puts text width} {
            if {$width > 8} {
                foreach line [cli::WrapIndented $text $width 8] {
                    cli::$Puts $line
                }
            } else {
                cli::$Puts $text
            }
        }

        proc PutLong {long {width 0}} {
            set IND [expr {$width > 0 ? "    " : ""}]
            foreach line [split $long \n] {
                if {[string trim $line] == ""} {
                    cli::NewLine
                } elseif {[string range $line 0 3] == "*** "} {
                    cli::PutResult [string range $line 4 end]
                } elseif {[string range $line 0 3] == "!!! "} {
                    cli::PutWarning "$IND[string range $line 4 end]"
                } elseif {[string match %* $line]} {
                    Wrapped PutResult "$IND$line" $width
                } else {
                    Wrapped Puts "$IND$line" $width
                }
            }
        }

        proc printHelp {obj long|short} {
            if {${long|short} ni {long short}} {
                return -code error "second argument must be either \"long\" or \"short\""
            }
            set width [cli::ConsoleWidth]

            set isLong [expr { ${long|short} == "long" } ]
            cli::PutResult {COMMAND}
            if {$isLong} cli::NewLine
            cli::Putf {    %s - %s} [$obj getName] [$obj getShort]
            cli::NewLine
            cli::PutResult {SYNOPSIS}
            if {$isLong} cli::NewLine
            Wrapped Puts "    [$obj getSynopsis]" $width
            cli::NewLine
            if {[$obj hasArgs]} {
                set items [list]
                foreach arg [$obj getArgs] {
                    lappend items [dict create \
                        NAME [$arg getName] \
                        SYNOPSIS [$arg getSynopsis] \
                        TEXT [$arg getShort]
                    ]
                    if {$isLong} {
                        if {[$arg hasLong]} {
                            lappend items [dict create TEXT [$arg getLong]]
                        }
                    }
                }
                HelpArgDisplay display $items $isLong

                if {!$isLong} cli::NewLine
            }

            if {$isLong && [$obj hasLong]} {
                cli::PutResult {DESCRIPTION}
                cli::NewLine
                PutLong [$obj getLong] $width
                cli::NewLine
            }

        }

    } ; # namespace eval cli::Help

    proc help {args} {
        cli::Help::ArgumentParser parseCallerArgs 1 $args

        if {$version} {
            cli::PutInfo "Movidius Debugger (moviDebug2) v[::mdbg::dll::version]"
            cli::PutInfo "Copyright (C) 2015 Movidius Ltd. All rights reserved."
            return
        }

        if {![info exists command]} {
            set items [list]
            cli::Help::Manager forEachSimpleCommand name obj {
                lappend items [dict create \
                    NAME [$obj getName] \
                    SHORT [$obj getShort] \
                ]
            }
            cli::Help::HelpListDisplay display $items
            return
        }

        set upns [uplevel 1 namespace current]
        cli::Help::Manager NormalizeCommand command $upns

        if {[info exists subcommand]} {
            lappend command $subcommand
        }
        if {[info exists subsubcmd]} {
            lappend command $subsubcmd
        }

        set items [list]
        foreach {name obj} [cli::Help::Manager find -longest -glob $command $upns] {
            lappend items [dict create \
                NAME [$obj getName] \
                SHORT [$obj getShort] \
                OBJ $obj
            ]
        }

        if {$items == {}} {
            cli::PutfWarning {No help for "%s"} $args
            return
        }

        if {[llength $items] > 1} {
            cli::Help::HelpListDisplay display $items
            return
        }

        set obj [dict get [lindex $items 0] OBJ]
        cli::Help::printHelp $obj [expr {$short ? "short" : "long"}]

    }

    shell::autocomplete::addCommand help mdbg::cli::Help::ArgumentParser autocomplete
    cli::Help::Manager add help -parser mdbg::cli::Help::ArgumentParser

} ; # namespace eval mdbg