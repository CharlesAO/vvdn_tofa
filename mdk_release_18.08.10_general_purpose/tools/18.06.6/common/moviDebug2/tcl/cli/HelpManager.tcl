# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/HelpManager.tcl
# Description: CLI Help Manager
# Created on : Sep 30, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::cli {

    oo::class create HelpManager {

        mixin mdbg::oo::LazyFactory

        # Execute script only in debug mode
        method DEBUG {script} {
            if {
                [info exists ::mdbg::DEBUG_HELP_MANAGER] &&
                [string is true $::mdbg::DEBUG_HELP_MANAGER]
            } {
                uplevel 1 $script
            }
        }

        variable Commands

        constructor {} {
            set Commands [dict create]
        }

        destructor {
            foreach cmdHelpObj [dict values $Commands] {
                $cmdHelpObj destroy
            }
        }

        method setCommandHelp {cmd ns synopsis short long} {
            set cmdHelpObj [mdbg::cli::HelpManager::CommandHelp new $cmd $synopsis $short $long]
            my NormalizeCommand cmd $ns
            if {[dict exists $Commands $cmd]} {
                [dict get $Commands $cmd] destroy
            }
            dict set Commands $cmd $cmdHelpObj
            set cmdHelpObj
        }

        method forEachCommand {cmdNameVar cmdObjVar scriptBody} {
            upvar 1 $cmdNameVar cmdName
            upvar 1 $cmdObjVar cmdObj
            dict for {cmdName cmdObj} $Commands {
                uplevel 1 $scriptBody
            }
        }

        method forEachSimpleCommand {cmdNameVar cmdObjVar scriptBody} {
            upvar 1 $cmdNameVar cmdName
            upvar 1 $cmdObjVar cmdObj
            dict for {cmdName cmdObj} $Commands {
                if {[llength $cmdName] > 1} {
                    continue
                }
                uplevel 1 $scriptBody
            }
        }

        method listSimpleCommands {pattern} {
            set result [list]
            my forEachSimpleCommand name obj {
                set objname [$obj getName]
                if {[string match $pattern $objname]} {
                    lappend result $objname
                } elseif {[string match $pattern $name]} {
                    lappend result $name
                }
            }
            set result
        }

        method listSubCommands {command pattern} {
            set result [list]
            set cmdLen [llength $command]
            foreach {cmd obj} [my find -glob [concat $command $pattern]] {
                if {[llength $cmd] > $cmdLen} {
                    lappend result [lindex $cmd end]
                }
            }
            set result
        }

        method listSubSubCommands {command subcommand pattern} {
            set result [list]
            set cmdLen [llength $command]
            foreach {cmd obj} [my find -glob [concat $command $subcommand $pattern]] {
                if {[llength $cmd] > $cmdLen + 1} {
                    lappend result [lindex $cmd end]
                }
            }
            set result
        }


        method getOrigin {cmd ns} {
            set cmdList [namespace inscope $ns info commands $cmd*]

            if {$cmd ni $cmdList} {
                return $cmd
            }
            namespace inscope $ns [list \
                namespace origin $cmd \
            ]
        }

        method getAlias {cmd} {
            set alias [interp alias {} $cmd]
            expr { $alias != {} && [interp target {} $cmd] == {}
                ? $alias
                : $cmd
            }
        }

        method add {args} {
            [my LazyObject AddParser mdbg::cli::ArgumentParser {
                -auto
                -args {
                    command {}
                    -parser {-optional}
                    -synopsis {-unindent -optional}
                    -short {-unindent -optional}
                    -long {-unindent -default {}}
                    -args {-optional -var argHelp}
                    -namespace {-var ns -default {}}
                }
                -mutex {
                    {-parser {-synopsis -short -long -args}}
                }
                -requires {
                    {-parser -synopsis}
                    {-parser -short}
                }
            }] parseArgs [list [self] [self method]] $args

            if {![string match ::* $ns]} {
                set upns [uplevel 1 namespace current]
                if {$ns == {}} {
                    set ns $upns
                } elseif {$upns != "::" && [namespace exists ${upns}::$ns]} {
                    set ns ${upns}::$ns
                } else {
                    set ns ::$ns
                }
            }

            if {[info exists parser]} {
                my AddCommandHelpFromParser $command $ns $parser
                return
            }

            set cmdHelpObj [my setCommandHelp $command $ns $synopsis $short $long]

            if {[info exists argHelp]} {
                my AddArgHelp $cmdHelpObj $argHelp
            }

            set cmdHelpObj
        }

        method get {command {ns ::}} {
            my NormalizeCommand command $ns
            if {![dict exists $Commands $command]} {
                return
            }
            dict get $Commands $command
        }

        method find {args} {
            [my LazyObject FindArgsParser mdbg::cli::ArgumentParser {
                -auto
                -args {
                    -shortest {-flag}
                    -longest {-flag}
                    -glob {-flag}
                    command {}
                    namespace {-var ns -default ::}
                }
                -mutex {
                    {-shortest -longest}
                }
            }] parseArgs [list [self] [self method]] $args

            my NormalizeCommand command $ns

            set result [list]
            set cmdlen [llength $command]
            if {!$glob} {
                for {set i 0} {$i < $cmdlen} {incr i} {
                    set prefix [lrange $command 0 $i]
                        if {[dict exists $Commands $prefix]} {
                            lappend result $prefix [dict get $Commands $prefix]
                            if {$shortest} {
                                return [lrange $result 0 1]
                            }
                        }
                }
                if {$longest} {
                    return [lrange $result end-1 end]
                }
            } else {
                set filtered [dict filter $Commands script {name obj} {
                    set len [llength $name]
                    set ok [expr {$len <= $cmdlen}]
                    for {set i 0} {$ok && $i < $len} {incr i} {
                        if {![string match [lindex $command $i] [lindex $name $i]]} {
                             set ok 0
                        }
                    }
                    set ok
                }]
                set keysByLen [lsort -command {apply {{lhs rhs} {
                        expr {[llength $lhs] - [llength $rhs]}
                }}} [dict keys $filtered] ]

                set minlen [llength [lindex $keysByLen 0]]
                set maxlen [llength [lindex $keysByLen end]]

                foreach key [dict keys $filtered] {
                    if {$shortest && [llength $key] != $minlen
                      || $longest && [llength $key] != $maxlen
                    } {
                        continue
                    }
                    lappend result $key [dict get $Commands $key]
                }
            }
            return $result
        }

        #
        #  Private
        #

        method AddCommandHelpFromParser {command ns parser} {
            set parser [namespace inscope $ns [list namespace origin $parser]]
            set helpDict [$parser getHelp $command]
            # puts mdbg::Channels::debug "helpDict($command)=[mdbg::prtobj $helpDict]"

            my add $command {*}$helpDict -namespace $ns

            if {[info object isa typeof $parser ::mdbg::cli::MultiCmdArgParser]} {
                foreach subCmd [$parser SubCmdList] {
                    set subHelpDict [$parser getHelp $command $subCmd]
                    # puts $mdbg::Channels::debug "subHelpDict($command $subCmd)=[mdbg::prtobj $subHelpDict]"
                    my add [concat $command $subCmd] {*}$subHelpDict -namespace $ns
                }
            }
        }

        method NormalizeCommand {commandVar ns} {
            upvar 1 $commandVar command
            set command [my NormalizeList $command]
            if {$command != {}} {
                set excludeNames [list]
                while {1} {
                    set firstItem [lindex $command 0]
                    if {$firstItem in $excludeNames} {
                        return -code error "Recursive alias detected: $firstItem"
                    } else {
                        lappend excludeNames $firstItem
                    }
                    set origin [my getOrigin $firstItem $ns]
                    set alias [my getAlias $origin]
                    if {$alias == $origin} {
                        lset command 0 $origin
                        break
                    }
                    set command [lreplace $command 0 0 {*}$alias]
                }
            }
        }

        export NormalizeCommand

        method NormalizeList {list} {
            list {*}$list
        }

        method AddArgHelp {cmdHelpObj argHelp} {
            set argDefParser [my LazyObject ArgDefParser mdbg::cli::ArgumentParser {
                -auto
                -args {
                    -synopsis {-unindent -default {}}
                    -short {-unindent}
                    -long {-unindent -default {}}
                }
            }]

            foreach {name defs} $argHelp {
                set vars [$argDefParser parse \{ $defs \}]
                dict with vars {
                    $cmdHelpObj addArg $name $synopsis $short $long
                }
            }
        }

    } ; # oo::class HelpManager

    namespace eval HelpManager {
        oo::class create CommandHelp {

            variable Name
            variable Synopsis
            variable Short
            variable Long
            variable Args

            constructor {name synopsis short long} {
                set Name $name
                set Synopsis $synopsis
                set Short $short
                set Long $long
                set Args [dict create]
            }

            destructor {
                dict for {name argObj} $Args {
                    $argObj destroy
                }
            }

            method getName {} {
                return $Name
            }

            method getSynopsis {} {
                return $Synopsis
            }

            method getShort {} {
                return $Short
            }

            method hasLong {} {
                expr {$Long != {}}
            }

            method getLong {} {
                return $Long
            }

            method hasArgs {} {
                expr {[dict size $Args] > 0}
            }

            method getArgs {} {
                dict values $Args
            }

            method addArg {name synopsis short long} {
                set argObj [mdbg::cli::HelpManager::CmdArgHelp new $name $synopsis $short $long]
                dict set Args $name $argObj
                set argObj
            }

        } ; # oo::class CommandHelp

        oo::class create CmdArgHelp {
            variable Name
            variable Synopsis
            variable Short
            variable Long

            constructor {name synopsis short long} {
                set Name $name
                set Synopsis $synopsis
                set Short $short
                set Long $long
            }

            method getName {} {
                return $Name
            }

            method getSynopsis {} {
                return $Synopsis
            }

            method getShort {} {
                return $Short
            }

            method hasLong {} {
                expr {$Long != {}}
            }

            method getLong {} {
                return $Long
            }

        } ; # oo::class CmdArgHelp

    } ; # namespace HelpManager

} ; # namespace eval mdbg::cli
