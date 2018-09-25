# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : ArgumentParser.tcl
# Description: Tcl procedure argument parser
# Created on : Aug 11, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli {

    #
    #  Mixin class to support parsing of caller's arguments + debugging
    #
    oo::class create ParseCallerArgs {

        # Execute script only in debug mode
        method DEBUG {script} {
            if {
                [info exists ::mdbg::DEBUG_ARGUMENT_PARSER] &&
                [string is true $::mdbg::DEBUG_ARGUMENT_PARSER]
            } {
                uplevel 1 {
                    upvar 0 ::mdbg::Channels::debug DBGCH
                }
                uplevel 1 $script
            }
        }

        # Parse caller's args.
        # @param cmdLen - number of words from caller's invocation to consider as prefix to arguments
        #                 This is usually 1 meaning that the prefix is only the procedure's name.
        #                 Only affects error messages.
        # @param argList - list of arguments to parse.
        # @returns - nothing
        # @note Side effects: sets variables onto the caller's stack, based on the arguments
        #       and according to the the definition rules.
        method parseCallerArgs {cmdlLen argList} {
            set callerCmd [lrange [info level -1] 0 [expr {$cmdlLen - 1}]]
            if {[catch {
                my SetUpvars 1 [my parse $callerCmd $argList]
            } error]} {
                my DEBUG {
                    puts $DBGCH $::errorInfo
                }
                return -code error -level 2 $error
            }
        }

        # Parse arguments.
        # @param command - name of caller command. May include other parameters, not parsed.
        #                 Only affects error messages.
        # @param argList - list of arguments to parse.
        # @returns - nothing
        # @note Side effects: sets variables onto the caller's stack, based on the arguments
        #       and according to the the definition rules.
        method parseArgs {command argList} {
            if {[catch {
                my SetUpvars 1 [my parse $command $argList]
            } error]} {
                my DEBUG {
                    puts $DBGCH $::errorInfo
                }
                return -code error -level 2 $error
            }
        }

        #
        #  Private
        #

        # Sets variables from dictionary to stack uplevel `level`
        method SetUpvars {level Vars} {
            incr level
            dict for {var value} $Vars {
                uplevel $level [list set $var $value]
            }
        }
    } ; # oo::class create ParseCallerArgs

    #  Argument parser for simple commands (single set of rules)
    #
    #  The definitions are in the form
    #  NOTE: hashmark comments not supported in the actual rules
    #
    #  {
    #       -auto # means automatically resolve switches and choices by unique prefix
    #       -args {
    #           # begin args
    #           arg1beg { <options> }
    #           arg2beg { <options> }
    #           ...
    #           # switches
    #           -switch1 { <options> }
    #           -switch2 { <options> }
    #           ...
    #           # end args
    #           arg1end { <options> }
    #           arg2end { <options> }
    #       }
    #       -mutex {
    #            { -switch1 arg2 ... }    # mutually exclusive
    #            { {-switch2 arg3} arg4 } # mutually exclusive only between -switch2 vs arg4 and arg3 vs arg4
    #            ...
    #       }
    #       -synopsis {
    #             # command syntax (auto-generated if missing)
    #       }
    #       -brief {
    #             Brief description of command
    #       }
    #       -description {
    #             Summary of command
    #       }
    #       TODO: document the rest
    #  }
    #
    #  Argument options:
    #      -optional             # don't set variable if argument not present.
    #      -flag                 # no parameter required, variable will always be set to a boolean
    #      -var <varname>        # explicit variable name: default is argument name without leading dash
    #      -require {arg1 arg2}  # either arg1 or arg2 is required. Multiple -requires can be given
    #      -multi                # allow argument to be specified multiple times. variable will be a list
    #      -default              # set this default value for the variable if argument not present.
    #      -type <type>          # check type of argument using `string is <type> -strict`
    #      -choice {choice1 ...} # allow only values from list
    #      -trim                 # trim string before processing
    #      -unindent             # unindent string before processing (
    #                            # (max amount of whitespace detected after first line break will be removed from each line)
    #      -regexp <pattern>     # check argument against regexp pattern
    #      -not-regexp <pattern> # check argument against regexp pattern. Must not match
    #      -match <pattern>      # check argument against `string match` pattern
    #      -not-match <pattern>  # check argument against `string match` pattern. Must not match
    #      TODO: document the rest

    oo::class create ArgumentParser {

        variable ArgDefs
        variable ArgCompletes
        variable ArgBegin
        variable Switches
        variable ArgEnd

        variable ArgExclude
        variable ArgRequire

        variable Auto
        variable CmdHelp
        variable ArgHelp

        constructor {definitions} {
            oo::objdefine [self] mixin mdbg::cli::ParseCallerArgs

            set Auto 0
            set ArgDefs [dict create]
            set ArgCompletes [dict create]
            set ArgExclude [dict create]
            set ArgRequire [dict create]
            set ArgBegin [list]
            set Switches [list]
            set ArgEnd [list]

            set CmdHelp [dict create SHORT {} LONG {}]
            set ArgHelp [dict create]

            my AddDefs $definitions
        }

        method parse {command arglist {suffix {}} } {
            my DEBUG {
                puts $::mdbg::Channels::debug "[self] [self method] \"$command\" \"$arglist\" \"$suffix\""
            }

            my Parse_Init

            my Parse_BeginArgs
            my Parse_Switches
            my Parse_EndArgs

            my Parse_Done
        }

        method autocomplete {cmd params prefix} {
            my AutoComplete_Init
            my AutoComplete_SearchBeginArgs
            my AutoComplete_SearchSwitches
            my AutoComplete_SearchEndArgs
            my AutoComplete_Done
        }

        method isAuto {} {
            return $Auto
        }

        method getHelp {command} {
            set cmdHelp $CmdHelp
            if {![dict exists $cmdHelp SYNOPSIS]} {
                dict set cmdHelp SYNOPSIS [
                    my GetSynopsis $command
                ]
            }

            set args [dict map {name argHelp} $ArgHelp {
                if {[my IsDisabledArg $name]} {
                    continue
                }

                dict set argHelp SYNOPSIS [my ArgSynopsis $name 0]

                my RemapHelpDict $argHelp
            }]

            dict set cmdHelp ARGS $args
            my RemapHelpDict $cmdHelp
        }
        #
        # Protected
        #
        method AddDefs {defs} {
            set len [llength $defs]
            for {set index 0} {$index < $len} {incr index $step} {
                set step 2
                set arg [lindex $defs $index]
                set param [lindex $defs [expr {$index + 1}]]
                switch -exact $arg {
                    -args {
                        my AddArgDefs $param
                        my DEBUG {
                            puts $DBGCH "[self] ArgDefs = [mdbg::prettyTclObject $ArgDefs]"
                            puts $DBGCH "[self] ArgRequire = [mdbg::prettyTclObject $ArgRequire]"
                            puts $DBGCH "[self] ArgBegin = [mdbg::prettyTclObject $ArgBegin]"
                            puts $DBGCH "[self] Switches = [mdbg::prettyTclObject $Switches]"
                            puts $DBGCH "[self] ArgEnd = [mdbg::prettyTclObject $ArgEnd]"
                        }
                    }
                    -mutex {
                        my AddArgExcludes $param
                        my DEBUG {
                            puts $DBGCH "[self] ArgExclude = [mdbg::prettyTclObject $ArgExclude]"
                        }
                    }
                    -auto {
                        set Auto 1
                        set step 1
                        my DEBUG {
                            puts $DBGCH "[self] Auto = $Auto"
                        }
                    }
                    -arg-complete         -
                    -arg-complete-command {
                        my AddArgCompletes $param
                    }
                    -arg-complete-script  -
                    -arg-complete-scripts {
                        my AddArgCompleteScripts $param
                    }
                    -require -
                    -requires {
                        dict lappend ArgRequire {} {*}$param
                        my DEBUG {
                            puts $DBGCH "[self] ArgRequire = [mdbg::prettyTclObject $ArgRequire]"
                        }
                    }
                    -synopsis {
                        dict set CmdHelp SYNOPSIS [my Unindent $param]
                    }
                    -brief {
                        dict set CmdHelp SHORT [my Unindent $param]
                    }
                    -description {
                        dict set CmdHelp LONG [my Unindent $param]
                    }
                    -comment {
                        # support for adding comments
                    }
                    default {
                        return -code error "Unknown option: \"$arg\""
                    }
                }
            }
        }

        export AddDefs

        #
        # Private
        #
        method AddArgDefs {argdefs} {
            foreach {args options} $argdefs {
                foreach arg $args {
                    my AddArgDef $arg $options
                }
            }
        }

        method AddArgDef {name options} {
            set isArgs [expr {$name == "args"}]
            set isSwitch [expr {!$isArgs && [my IsSwitch $name]}]
            set isEndArg [expr {
                 $isArgs ||
                !$isSwitch && ( [llength $Switches] > 0 || [llength $ArgEnd] > 0)}]

            if {[llength $ArgEnd] > 0 && $isSwitch && $name ni $Switches} {
                return -code error "Invalid definition \"$name\": cannot be added after final unnamed args"
            }

            if {![dict exists $ArgDefs $name]} {
                dict set ArgDefs $name [dict create \
                    VAR [my DefaultVarName $name] \
                    FLAG 0 \
                    TYPE {} \
                    MULTI $isArgs \
                    OPTIONAL $isArgs
                ]
                if {$isArgs} {
                    dict set ArgDefs $name DEFAULT [list]
                }
            }

            set ListName [expr {
                $isSwitch ? "Switches"
                          : $isEndArg ? "ArgEnd"
                                      : "ArgBegin"
            }]
            if {$name ni [set $ListName]} {
                lappend $ListName $name
            }

            set len [llength $options]
            for {set index 0} {$index < $len} {incr index} {
                set opt [lindex $options $index]
                set param [lindex $options [expr {$index + 1}]]
                switch -exact $opt {
                    -flag {
                        dict set ArgDefs $name FLAG 1
                        dict set ArgDefs $name OPTIONAL 1
                        dict set ArgDefs $name DEFAULT 0
                    }
                    -type {
                        dict set ArgDefs $name TYPE $param
                        incr index
                    }
                    -regexp {
                        dict set ArgDefs $name REGEXP $param
                        incr index
                    }
                    -match {
                        dict set ArgDefs $name GLOB $param
                        incr index
                    }
                    -not-regexp {
                        dict set ArgDefs $name !REGEXP $param
                        incr index
                    }
                    -not-match {
                        dict set ArgDefs $name !GLOB $param
                        incr index
                    }
                    -multi {
                        dict set ArgDefs $name MULTI 1
                    }
                    -optional {
                        dict set ArgDefs $name OPTIONAL 1
                    }
                    -trim {
                        dict set ArgDefs $name TRIM 1
                    }
                    -unindent {
                        dict set ArgDefs $name UNINDENT 1
                    }
                    -choice -
                    -choices {
                        dict set ArgDefs $name CHOICES $param
                        incr index
                    }
                    -default {
                        dict set ArgDefs $name DEFAULT $param
                        dict set ArgDefs $name OPTIONAL 1
                        incr index
                    }
                    -require -
                    -requires {
                        dict lappend ArgRequire $name $param
                        incr index
                    }
                    -var {
                        dict set ArgDefs $name VAR $param
                        incr index
                    }
                    -opt-param {
                        dict set ArgDefs $name FLAG 1
                        dict set ArgDefs $name OPT_PARAM $param
                        incr index
                    }
                    -opt-var -
                    -opt-param-var {
                        dict set ArgDefs $name OPT_PARAM_VAR $param
                        incr index
                    }
                    -opt-hidden -
                    -opt-param-hidden {
                        dict set ArgDefs $name OPT_PARAM_HIDDEN 1
                    }
                    -complete {
                        my SetArgComplete $name {*}$param
                        incr index
                    }
                    -complete-script {
                        my SetArgCompleteScript $name $param
                        incr index
                    }
                    -synopsis {
                        my SetArgSynopsis $name [my Unindent $param]
                        incr index
                    }
                    -hidden {
                        my SetHiddenArg $name
                    }
                    -shown {
                        my SetHiddenArg $name 0
                    }
                    -disabled {
                        my DisableArg $name
                    }
                    -enabled {
                        my EnableArg $name
                    }
                    -brief {
                        my SetArgBrief $name [my Unindent $param]
                        incr index
                    }
                    -description {
                        my SetArgDescription $name [my Unindent $param]
                        incr index
                    }
                    -comment {
                        incr index
                    }
                    default {
                        return code -error "Invalid argument definition option: \"$opt\""
                    }
                }
            }
        }

        method IsSwitch {name} {
            expr {
                [string match -* $name] && (
                    ![string is double $name] || (
                        $name in $Switches
                    )
                )
            }
        }

        method DefaultVarName {name} {
            expr {
                [my IsSwitch $name]
                    ? [string range $name 1 end]
                    : $name
            }
        }

        method AddArgExcludes {param} {
            foreach def $param {
                my AddArgExclude $def
            }
        }

        method AddArgExclude {excludeList} {
            set count [llength $excludeList]

            for {set i 0} {$i < $count} {incr i} {
                for {set j [expr {$i + 1}]} { $j < $count} {incr j} {
                    foreach first [lindex $excludeList $i] {
                        foreach second [lindex $excludeList $j] {
                            if {![string equal $first $second]} {
                                dict set ArgExclude $first $second 1
                                dict set ArgExclude $second $first 1
                            }
                        }
                    }
                }
            }
        }

        method AddArgCompletes {param} {
            foreach {names command} $param {
                foreach name $names {
                    my SetArgComplete $name {*}$command
                }
            }
        }

        method AddArgCompleteScripts {param} {
            foreach {names body} $param {
                foreach name $names {
                    my SetArgCompleteScript $name $body
                }
            }
        }

        method SetArgCompleteScript {name body} {
            set lambda [list {COMMAND PARAMS PREFIX} $body ::]
            my SetArgComplete $name apply $lambda
        }

        method SetArgComplete {name command args} {
            dict set ArgCompletes $name [list $command {*}$args]
        }

        method SetArgSynopsis {name synopsis} {
            dict set ArgHelp $name SYNOPSIS $synopsis
        }

        method SetArgBrief {name brief} {
            dict set ArgHelp $name SHORT $brief
        }

        method SetArgDescription {name description} {
            dict set ArgHelp $name LONG $description
        }

        method SetHiddenArg {name {hidden 1} } {
            dict set ArgDefs $name HIDDEN $hidden
        }

        method DisableArg {name } {
            dict set ArgDefs $name DISABLED 1
        }

        method EnableArg {name } {
            dict set ArgDefs $name DISABLED 0
        }

        #
        # Parse partial methods + helpers
        #
        method Parse_Init {} {
            uplevel 1 {
                set Command $command
                set Suffix $suffix
                set ArgList $arglist
                set Names [dict create]
                set Vars [dict create]
                set Index 0
            }
        }

        method Parse_BeginArgs {} {
            uplevel 1 {
                set BeginIndex 0
                while {$BeginIndex < [llength $ArgBegin]} {
                    set name [lindex $ArgBegin $BeginIndex]
                    if {[my IsDisabledArg $name]} {
                        incr BeginIndex
                        continue
                    }

                    if {$Index >= [llength $ArgList]} {
                        if {[my IsOptionalArg $name]} {
                            break;
                        }
                        my WrongNumArgsError $Command $Suffix
                    }
                    set arg [lindex $ArgList $Index]

                    if {[my IsOptionalArg $name] && ![my ArgMatch $name $arg]} {
                         incr BeginIndex
                         continue
                    }

                    if {[my IsSwitch $arg]} {
                        if {[my IsOptionalArg $name]} {
                            break
                        }
                        my WrongNumArgsError $Command $Suffix
                    }

                    if {[my IsOptionalArg $name] && [my IsChoice $name]} {
                        if {![my TestAndSetValidChoice $name arg]} {
                            incr BeginIndex
                            continue
                        }
                        my ValidateArgType $name $arg
                    } else {
                        my ValidateArgValue $name arg
                    }

                    dict set Names $name 1
                    my Parse_SetVarValue $name [my VarName $name] $arg

                    incr BeginIndex
                    incr Index
                }
            }
        }

        method Parse_Switches {} {
            uplevel 1 {
                while {$Index < [llength $ArgList]} {
                    set name [lindex $ArgList $Index]

                    set nextIndex [expr {$Index + 1}]
                    if {$nextIndex < [llength $ArgList]} {
                        set parameter [lindex $ArgList $nextIndex]
                    } else {
                        unset -nocomplain parameter
                    }

                    if {![my IsSwitch $name]} {
                        break;
                    } elseif { [string equal $name "--"] } {
                        incr Index
                        break
                    } elseif {[llength $Switches] == 0} {
                        break;
                    }

                    my ValidateSwitch name

                    if {[my IsExcludedArg $Names $name other]} {
                        return -code error "invalid argument \"$name\": already provided \"$other\""
                    }

                    if {[my IsFlag $name]} {
                        if {[my HasOptParam $name] && [info exists parameter] && ![my IsSwitch $parameter]} {
                            set optParamName [my GetOptParam $name]
                        } else {
                            unset -nocomplain optParamName
                        }
                    } else {
                        incr Index
                        if {![info exists parameter]} {
                            return -code error  "missing value for \"$name\""
                        }
                    }

                    if {[my IsDuplicateArg $Names $name]} {
                        return -code error "duplicate argument \"$name\""
                    }

                    dict incr Names $name

                    if {[my IsFlag $name]} {
                        my Parse_SetVarValue $name [my VarName $name] 1
                        if {[info exists optParamName]} {
                            set error {}
                            if {[my ValidateArgValue $name parameter error]} {
                                if {[my IsExcludedArg $Names $optParamName other]} {
                                    set error "invalid argument \"[list $name $parameter]\": already provided \"$other\""
                                } elseif {[my IsDuplicateArg $Names $optParamName]} {
                                    set error "duplicate argument \"[list $name $parameter]\" "
                                }
                            }
                            if {$error == {}} {
                                my Parse_SetVarValue $name [my GetOptParamVar $name] $parameter
                                dict incr Names $optParamName
                                incr Index
                            } else {
                                my Warning $error
                            }
                        }
                    } else {
                        my ValidateArgValue $name parameter
                        my Parse_SetVarValue $name [my VarName $name] $parameter
                    }

                    incr Index
                }
            }
        }

        method Parse_EndArgs {} {
            uplevel 1 {
                set endIndex 0
                set lastEndIndex [expr {[llength $ArgEnd] - 1}]
                while {$lastEndIndex >= 0
                    && [my IsDisabledArg [lindex $ArgEnd $lastEndIndex]]} {
                    incr lastEndIndex -1
                }
                while {$endIndex <= $lastEndIndex} {
                    set name [lindex $ArgEnd $endIndex]
                    if {[my IsDisabledArg $name]} {
                        incr endIndex
                        continue
                    }

                    if {$Index >= [llength $ArgList]} {
                        if {[my IsOptionalArg $name]} {
                            incr endIndex
                            continue
                        }
                        my WrongNumArgsError $Command $Suffix
                    }

                    set arg [lindex $ArgList $Index]

                    if {[my IsOptionalArg $name] && ![my ArgMatch $name $arg]} {
                         incr endIndex
                         continue
                    }

                    if {[my IsExcludedArg $Names $name other]} {
                        if {[my IsOptionalArg $name] && (
                                $endIndex < $lastEndIndex || (
                                    $endIndex == $lastEndIndex &&
                                    $Index + 1 < [llength $ArgList]
                                )
                            )
                        } {
                            incr endIndex
                            continue
                        }
                        set message "invalid `$name` argument \"$arg\": already provided "
                        if {[my IsSwitch $other]} {
                            append message " \"$other\""
                        } else {
                            if {[dict exists $ArgDefs $other]} {
                                set otherVar [dict get $ArgDefs $other VAR]
                                set otherValue [dict get $Vars $otherVar]
                                append message "`$other` argument \"$otherValue\""
                            } else {
                                set extra "`$other`"
                                foreach otherName [dict keys $Names] {
                                    if {[my IsSwitch $otherName] && [my HasOptParam $otherName] && [my GetOptParam $otherName] == $other} {
                                        set otherVar [my GetOptParamVar $otherName]
                                        set otherValue [dict get $Vars $otherVar]
                                        set extra "`$otherName` argument \"$otherValue\""
                                        break
                                    }
                                }
                                append message " $extra"
                            }
                        }
                        return -code error $message
                    }

                    my ValidateArgValue $name arg

                    dict update Names $name count {
                        incr count
                    }
                    my Parse_SetVarValue $name [my VarName $name] $arg

                    incr Index
                    if {$endIndex == $lastEndIndex} {
                        if {[my IsMultiArg $name]} {
                            if {$Index < [llength $ArgList]} {
                                continue
                            }
                        }
                    }
                    incr endIndex
                }
            }
        }

        method Parse_Done {} {
            uplevel 1 {
                if {$Index != [llength $ArgList]} {
                    my WrongNumArgsError $Command $Suffix
                }

                my CheckMissingRequiredArgs $Names

                my Parse_SetDefaults

                my Parse_CheckNonOptional

                my DEBUG {
                    puts $DBGCH "Names: [mdbg::prettyTclObject $Names]"
                    puts $DBGCH "Vars: [mdbg::prettyTclObject $Vars]"
                }

                return $Vars
            }
        }

        method Parse_SetVarValue {name varName value} {
            upvar 1 Names Names
            upvar 1 Vars Vars
            if {[my ShouldUnindent $name]} {
                set value [my Unindent $value]
            }
            if {[my ShouldTrim $name]} {
                set value [string trim $value]
            }
            dict update Vars $varName varValue {
                if {[my IsMultiArg $name]} {
                    if {[info exists varValue]} {
                        lappend varValue $value
                    } else {
                        set varValue [list $value]
                    }
                } else {
                    set varValue $value
                }
            }
        }

        method Parse_SetDefaults {} {
            uplevel 1 {
                foreach name [dict keys $ArgDefs] {
                    if {![dict exists $Names $name] && [my HasDefault $name]
                    } {
                        dict set Names $name 0
                        set varName [my VarName $name]
                        dict update Vars $varName varValue {
                            if {![info exists varValue]} {
                                set varValue [my GetDefault $name]
                            }
                        }
                    }
                }
            }
        }

        method FindMissingRequiredArgs {Names name} {
            if {![dict exists $ArgRequire $name]} {
                return
            }
            foreach reqs [dict get $ArgRequire $name] {
                if {![llength $reqs]} {
                    continue
                }
                set found 0
                foreach alt $reqs {
                    if {[dict exists $Names $alt]} {
                        set found 1
                    }
                }
                if {!$found} {
                    return $reqs
                }
            }
            return
        }

        method CheckMissingRequiredArgs {Names} {
            set reqs [my FindMissingRequiredArgs $Names {}]
            if {[set rlen [llength $reqs]]} {
                if {$rlen == 1} {
                    set message "argument \"$reqs\" was not provided"
                } else {
                    set message "must provide one of the arguments [my JoinQuoted $reqs]"
                }
                return -code error $message
            }

            foreach name [dict keys $Names] {
                set reqs [my FindMissingRequiredArgs $Names $name]
                if {![set rlen [llength $reqs]]} {
                    continue
                }
                set message "\"$name\" requires "
                if {$rlen == 1} {
                    append message "\"$reqs\" which was not"
                } else {
                    append message "one of [my JoinQuoted $reqs], none of which was"
                }
                append message " provided"
                return -code error $message
            }
        }

        method Parse_CheckNonOptional {} {
            uplevel 1 {
                foreach name [dict keys $ArgDefs] {
                    if {![my IsOptionalArg $name] &&
                        ![dict exists $Names $name]
                    } {
                        if [my IsSwitch $name] {
                            return -code error "argument \"$name\" was not provided"
                        }
                        my WrongNumArgsError $Command $Suffix
                    }
                }
            }
        }

        method WrongNumArgsError {command suffix} {
            return -code error "wrong # args: should be \"[my GetSynopsis $command]\""
        }

        method ArgSynopsis {name {markOptional 1}} {
            if {[dict exists $ArgHelp $name SYNOPSIS]} {
                set synopsis [dict get $ArgHelp $name SYNOPSIS]
                if {$synopsis != {}} {
                    if {!$markOptional} {
                        regsub {^\s*\?(.*)?\?\s*$} $synopsis {\1} synopsis
                    }
                    return $synopsis
                }
            }
            set namePart [expr {[my IsSwitch $name ] ? $name : ""}]

            if {[my IsFlag $name]} {
                if {![my HasOptParam $name] || [my IsOptParamHidden $name]} {
                    set valuePart ""
                } else {
                    set valuePart " ?"
                    if {![my IsChoice $name]} {
                        append valuePart "[my GetOptParam $name]"
                    } else {
                        append valuePart [join [my ArgChoices $name] |]
                    }
                    append valuePart "?"
                    if {$markOptional && [my IsOptionalArg $name]} {
                        append valuePart " "
                    }
                }
            } else {
                set valuePart [expr {[my IsSwitch $name] ? " " : "" }]
                if {![my IsChoice $name]} {
                    append valuePart [expr {
                        [my IsSwitch $name ] ? [my VarName $name] : $name
                    }]
                } else {
                    append valuePart \{
                    append valuePart [join [my ArgChoices $name] |]
                    append valuePart \}
                }
            }
            set questionMark [expr {
                $markOptional && [my IsOptionalArg $name] ? "?" : ""
            }]
            set result $questionMark
            append result $namePart $valuePart
            append result $questionMark
            if {[my IsMultiArg $name]} {
                append result "..."
            }
            set result
        }

        method GetSynopsis {command {suffix {}}} {
            concat $command [my CorrectArgs] $suffix
        }

        method RemapHelpDict {src} {
            dict map {key value} $src {
                switch -exact $key {
                    SYNOPSIS {set key -synopsis}
                    SHORT {set key -short}
                    LONG {set key -long}
                    ARGS {set key -args}
                }
                set value
            }
        }

        method CorrectArgs {} {
            if {[dict exists $CmdHelp SYNOPSIS]} {
                set synopsis [dict get $CmdHelp SYNOPSIS]
                if {$synopsis != {}} {
                    return $synopsis
                }
            }
            set result ""
            foreach name [
                concat $ArgBegin $Switches $ArgEnd
            ] {
                if {[my IsDisabledArg $name]} {
                    continue
                }
                if {[my IsHiddenArg $name]} {
                    continue
                }
                if {[string length $result]} {
                    append result " "
                }
                append result [my ArgSynopsis $name]
            }
            return $result
        }

        method ValidateArgValue {name valueVar {errorVar {}}} {
            upvar 1 $valueVar value
            if {$errorVar != {}} {
                upvar 1 $errorVar error
                set errorVar error
            }
            if {[my IsChoice $name]} {
                if {![my ValidateChoice $name value $errorVar]} {
                    return 0
                }
            }
            my ValidateArgType $name $value $errorVar
        }

        method ValidateArgType {name value {errorVar {}}} {
            if {$errorVar != {}} {
                upvar 1 $errorVar error
                set errorVar error
            }
            if {![my IsOptionalArg $name] && ![my ArgMatch $name $value]} {
                set error "$name: value \"$value\" does not have the required form"
            } else {
                set type [my ArgType $name]
                if {![my IsType $type $value]} {
                    set error "$name: expected $type, got \"$value\""
                } else {
                    return 1
                }
            }
            if {$errorVar != {}} {
                return 0
            }
            return -code error $error
        }

        method IsType {type value} {
            switch -exact $type {
                "" {
                    return 1
                }
                default {
                    tailcall string is $type -strict $value
                }
            }
        }

        method IsHiddenArg {name} {
            expr {
                [dict exists $ArgDefs $name HIDDEN] &&
                [dict get $ArgDefs $name HIDDEN]
            }
        }

        method IsDisabledArg {name} {
            expr {
                [dict exists $ArgDefs $name DISABLED] &&
                [dict get $ArgDefs $name DISABLED]
            }
        }

        method IsChoice {name} {
            dict exists $ArgDefs $name CHOICES
        }

        method HasDefault {name} {
            dict exists $ArgDefs $name DEFAULT
        }

        method GetDefault {name} {
            dict get $ArgDefs $name DEFAULT
        }


        method TestAndSetValidChoice {name valueVar} {
            upvar 1 $valueVar value
            set choices [my ArgChoices $name]
            if {$value in $choices} {
                return 1
            }
            if {$Auto} {
                if {[my AutoChoice $choices value]} {
                    return 1
                }
            }
            return 0
        }

      method ValidateChoice {name valueVar {errorVar {}} } {
            upvar 1 $valueVar value
            if {$errorVar != {}} {
                upvar 1 $errorVar error
                set errorVar error
            }

            if {[my TestAndSetValidChoice $name value]} {
                return 1
            }

            if {[my IsSwitch $name]} {
                set error "invalid value \"$value\" provided for argument \"$name\""
            } else {
                set error "invalid argument \"$value\""
            }
            append error ": should to be one of [my JoinQuoted [my ArgChoices $name]]"

            if {$errorVar != {}} {
                return 0
            }
            return -code error $error
        }

        method ArgChoices {name} {
            dict get $ArgDefs $name CHOICES
        }

        method EnabledSwitches {} {
            set result [list]
            foreach name $Switches {
                if {[my IsDisabledArg $name]} {
                    continue
                }
                lappend result $name
            }
            set result
        }

        method ValidateSwitch {nameVar} {
            upvar 1 $nameVar name

            set switches [my EnabledSwitches]
            if {$name in $switches} {
                return
            }

            if {$Auto} {
                if {[my AutoChoice $switches name]} {
                    return
                }
            }
            return -code error "invalid argument \"$name\": should be one of [my JoinQuoted $switches]"
        }

        method JoinQuoted {items} {
            set quoted [list]
            foreach item $items {
                lappend quoted "\"$item\""
            }
            join $quoted ", "
        }

        method Warning {message} {
            ::mdbg::cli::Warning $message
        }

        #
        #  Autocomplete partial methods + helpers
        #
        method AutoComplete_Init {} {
            uplevel 1 {
                set Command $cmd
                set PrevParams $params
                set LastParamPrefix $prefix
                set Names [dict create]
                set Index 0
                set BeginIndex 0
                set EndIndex 0
                set FoundArgName ""
                set FoundOptName ""
                set FoundAt ""
                set Result [list]
            }
        }

        method AutoComplete_Found {} {
             uplevel 2 string length {$FoundAt}
         }

        method AutoComplete_SearchBeginArgs {} {

             uplevel 1 {

                 while {$BeginIndex < [llength $ArgBegin]} {
                     set name [lindex $ArgBegin $BeginIndex]
                     if {[my IsDisabledArg $name]} {
                         incr BeginIndex
                         continue
                     }

                     if {$Index >= [llength $PrevParams]} {
                         set FoundArgName $name
                         set FoundAt BEGIN_ARGS
                         break
                     }
                     set arg [lindex $PrevParams $Index]

                     if {[my IsSwitch $arg]} {
                         break
                     }

                     if {[my IsChoice $name] && [my IsOptionalArg $name]} {
                         if {![TestAndSetValidChoice $name arg]} {
                             incr BeginIndex
                             continue
                         }
                     }

                     dict update Names $name count {
                        incr count
                     }
                     incr Index
                     incr BeginIndex
                 }
             }
        }

        method AutoComplete_SearchSwitches {} {

            if {[my AutoComplete_Found]} {
                return
            }

            uplevel 1 {
                while {$Index < [llength $PrevParams]} {
                    set name [lindex $PrevParams $Index]

                    if {![my IsSwitch $name]} {
                        break
                    } elseif { [string equal $name "--"] } {
                        incr Index
                        break
                    }

                    if {![my IsValidArg $name] && ![my AutoChoice $Switches name]} {
                        if {[incr Index] >= [llength $PrevParams]} {
                            set FoundArgName $name
                            set FoundAt INVALID_SWITCH_VALUE
                            break
                        }
                        set nextName [lindex $PrevParams $Index]
                        if {[my IsSwitch $nextName]} {
                            continue
                        } else {
                           if {([llength $PrevParams] - $Index) > [llength $ArgEnd]} {
                               incr Index
                               continue
                           } else {
                               break
                           }
                        }
                    } else {
                        dict set Names $name {}
                        if {[my IsFlag $name]} {
                            if {[my HasOptParam $name]} {
                                set optParamName [my GetOptParam $name]
                                if {
                                    [my IsExcludedArg $Names $optParamName] ||
                                    [my IsDuplicateArg $Names $optParamName]
                                } {
                                    unset optParamName
                                }
                                if {[incr Index] >= [llength $PrevParams]} {
                                    if {[info exists optParamName]} {
                                        set FoundArgName $name
                                        set FoundOptName $optParamName
                                        set FoundAt SWITCH_VALUE_OPT
                                    }
                                    break;
                                }
                                if {[info exists optParamName]} {
                                    set name [lindex $PrevParams $Index]
                                    if {![my IsSwitch $name]} {
                                        dict set Names $name {}
                                        incr Index
                                    }
                                }
                            } else {
                                incr Index
                            }
                            continue
                        }
                    }

                    if {[incr Index] >= [llength $PrevParams]} {
                        set FoundArgName $name
                        set FoundAt SWITCH_VALUE
                        break
                    }

                    dict set Names $name {}
                    incr Index
                }
            }
        }
        method AutoComplete_SearchEndArgs {} {

            if {[my AutoComplete_Found]} {
                return
            }

            uplevel 1 {

                set EndIndex 0
                set lastEndIndex [expr {[llength $ArgEnd] - 1}]

                while {$lastEndIndex >= 0 && [my IsDisabledArg [lindex $ArgEnd $lastEndIndex]]} {
                    incr lastEndIndex -1
                }

                while {$EndIndex <= $lastEndIndex} {
                    set name [lindex $ArgEnd $EndIndex]

                    if {[my IsDisabledArg $name]} {
                        incr EndIndex
                        continue
                    }

                    if {$Index >= [llength $PrevParams]} {
                        set FoundArgName $name
                        break
                    }

                    dict set Names $name 1
                    incr Index

                    if {$EndIndex == $lastEndIndex} {
                        if {$name == "args"} {
                            set FoundArgName $name
                            break
                        }
                        if {[my IsMultiArg $name]} {
                            if {$Index < [llength $PrevParams]} {
                                continue
                            }
                        }
                    }
                    incr EndIndex
                }

                set FoundAt END_ARGS
             }
        }
        method AutoComplete_Done {} {
            uplevel 1 {

                my DEBUG {
                    puts "Command         : $Command"
                    puts "PrevParams      : [my JoinQuoted $PrevParams]"
                    puts "LastParamPrefix : $LastParamPrefix"
                    puts "FoundArgName    : $FoundArgName"
                    puts "FoundAt         : $FoundAt"
                    puts "Names           : [dict keys $Names]"
                    puts "Index           : $Index"
                    puts "EndIndex        : $EndIndex"
                }

                switch -exact $FoundAt {
                    BEGIN_ARGS {
                        my AutoComplete_BeginArgs
                        if {$BeginIndex == [llength $ArgBegin]} {
                            my DEBUG {
                                puts "$BeginIndex == \[llength $ArgBegin\]: AutoComplete_Switches!"
                            }
                            my AutoComplete_Switches
                            if {![my IsRequiredSwitchMissing $Names]} {
                                my AutoComplete_EndArgs
                            }
                        }
                    }
                    SWITCH_VALUE {
                        my AutoComplete_SwitchValue
                    }
                    SWITCH_VALUE_OPT {
                        my AutoComplete_SwitchValue
                        my AutoComplete_Switches
                        if {![my IsRequiredSwitchMissing $Names]} {
                            my AutoComplete_EndArgs
                        }
                    }
                    INVALID_SWITCH_VALUE {
                        # nothing to do
                    }
                    END_ARGS {
                        if {$EndIndex == 0 && (
                                $Index == 0 || [lindex $PrevParams [expr $Index -1]] != "--"
                            )
                        } {
                            my AutoComplete_Switches
                        }
                        my AutoComplete_EndArgs
                    }
                }
                set Result
            }
        }

        method AutoComplete_BeginArgs {} {
            uplevel 1 {
                while {$BeginIndex < [llength $ArgBegin]} {
                    set name [lindex $ArgBegin $BeginIndex]
                    if {![my IsDisabledArg $name]} {
                        break;
                    }
                    incr BeginIndex
                }
                if {$BeginIndex < [llength $ArgBegin]} {
                    my AutoComplete_Result [my AutoComplete_Arg $name]
                    while {[my IsOptionalArg $name]} {
                        while {[incr BeginIndex] < [llength $ArgBegin]} {
                            set name [lindex $ArgBegin $BeginIndex]
                            if {![my IsDisabledArg $name]} {
                                break
                            }
                        }
                        if {$BeginIndex < [llength $ArgBegin]} {
                            my AutoComplete_Result [my AutoComplete_Arg $name]
                        } else {
                            break
                        }
                    }
                }
                while {$BeginIndex < [llength $ArgBegin]
                    && [my IsDisabledArg [lindex $ArgBegin $BeginIndex]]
                } {
                    incr BeginIndex
                }
            }
        }

        method AutoComplete_Switches {} {
            uplevel 1 {
                foreach name [my PrefixSearch $Switches $LastParamPrefix] {
                    if {[my IsDisabledArg $name]} {
                        continue
                    }
                    if {[my IsExcludedArg $Names $name]} {
                        continue
                    }
                    if {[my IsDuplicateArg $Names $name]} {
                        continue
                    }
                    set {[my IsRequiredPreviousArgMissing $Names $name]} {
                        continue
                    }

                    lappend Result $name
                }
            }
        }

        method AutoComplete_SwitchValue {} {
            uplevel 1 {
                set name $FoundArgName
                my AutoComplete_Result [my AutoComplete_Arg $name]
                if {$FoundOptName != ""} {
                    my AutoComplete_Result [my AutoComplete_Arg $FoundOptName]
                }
            }
        }

        method AutoComplete_EndArgs {} {
            uplevel 1 {
                while {$EndIndex < [llength $ArgEnd]} {
                    set name [lindex $ArgEnd $EndIndex]
                    if {[my IsDisabledArg $name]} {
                        incr EndIndex
                        continue
                    }
                    my AutoComplete_Result [my AutoComplete_Arg $name]

                    if {![my IsOptionalArg $name]} {
                        break
                    }
                    incr EndIndex
                }
            }
        }

        method IsRequiredSwitchMissing {Names} {
            foreach name $Switches {
                if {![my IsOptionalArg $name] && ![dict exists $Names $name]} {
                    return 1
                }
            }
            return 0
        }

        method IsRequiredPreviousArgMissing {Names name} {
            # Check if something is missing
            if {[my FindMissingRequiredArgs $Names $name] == {}} {
                # alles ok!
                return 0
            }
            # Add args following name to local copy of Names
            foreach arg [my GetArgsFollowing $name] {
                if {![dict exists $Names $arg]} {
                    dict set $Names $arg 0
                }
            }
            # Check again, no excuses this time
            tailcall my FindMissingRequiredArgs $Names $name
        }

        method GetArgsFollowing {name} {
            set isSwitch [my IsSwitch $name]

            if {!$isSwitch && [set index [lindex -exact $ArgBegin $name]] >= 0} {
                incr index
                tailcall concat [lrange $ArgBegin $index end] $Switches $ArgEnd
            } elseif {$isSwitch && [set index [lindex -exact $Switches $name] >= 0]} {
                incr index
                tailcall concat [lrange $Switches $index end] $ArgEnd
            } elseif {!$isSwitch && [set index [lindex -exact $ArgEnd $name] >= 0]} {
                incr index
                tailcall lrange $ArgEnd $index
            } else {
                return
            }
        }

        method AutoComplete_Result {result} {
            if {[llength $result]} {
               upvar 1 Result upresult
               set upresult [concat $upresult $result]
            }
        }

        method AutoComplete_Arg {name} {
            uplevel 1 [list my ArgComplete $name] {$Command $PrevParams $LastParamPrefix}
        }

        method ArgComplete {name cmd params prefix} {
            if {[my HasArgComplete $name]} {
                set script [my GetArgComplete $name]
                namespace inscope :: $script $cmd $params $prefix
            } else {
                if {[my IsChoice $name]} {
                    my PrefixSearch [my ArgChoices $name] $prefix
                } else {
                    # TODO: ?
                    list
                }
            }
        }

        #
        #  Miscellaneous helper methods
        #

        method IsValidArg {name} {
            expr {
                [dict exists $ArgDefs $name] && ![my IsDisabledArg $name]
            }
        }

        method IsDuplicateArg {Names name} {
            expr {
                [dict exists $Names $name] && ![my IsMultiArg $name]
            }
        }

        method IsExcludedArg {Names name {otherVar {}}} {
            if {[dict exists $ArgExclude $name]} {
                foreach other [dict keys [dict get $ArgExclude $name]] {
                    if {[dict exists $Names $other]} {
                        if {[string length $otherVar]} {
                            uplevel 1 [list set $otherVar $other]
                        }
                        return 1
                    }
                }
            }
            return 0
        }

        method PrefixSearch {itemList itemPrefix} {
            lsearch -all -inline -glob $itemList $itemPrefix*
        }

        method AutoChoice {itemList itemVar} {
            upvar 1 $itemVar item
            set found [my PrefixSearch $itemList $item]
            if {[llength $found] == 1} {
                set item [lindex $found 0]
                return 1
            }
            return 0
        }

        method IsOptionalArg {name} {
            dict get $ArgDefs $name OPTIONAL
        }

        method IsFlag {name} {
            dict get $ArgDefs $name FLAG
        }

        method HasOptParam {name} {
            dict exists $ArgDefs $name OPT_PARAM
        }

        method GetOptParam {name} {
            dict get $ArgDefs $name OPT_PARAM
        }

        method GetOptParamVar {name} {
            if {[dict exists $ArgDefs $name OPT_PARAM_VAR]} {
                dict get $ArgDefs $name OPT_PARAM_VAR
            } else {
                dict get $ArgDefs $name OPT_PARAM
            }
        }

        method IsOptParamHidden {name} {
            expr {
                [dict exists $ArgDefs $name OPT_PARAM_HIDDEN] &&
                [dict get $ArgDefs $name OPT_PARAM_HIDDEN]
            }
        }

        method IsMultiArg {name} {
            dict get $ArgDefs $name MULTI
        }

        method VarName {name} {
            dict get $ArgDefs $name VAR
        }

        method ArgMatch {name value} {
            foreach  {key op inv} {
                REGEXP  regexp 0
                !REGEXP regexp 1
                GLOB    {string match} 0
                !GLOB   {string match} 1
            } {
                if {[dict exists $ArgDefs $name $key]} {
                    set pattern [dict get $ArgDefs $name $key]
                    set result [{*}$op $pattern $value]
                    if {!$inv && !$result || $inv && $result} {
                        return 0
                    }
                }
            }
            return 1
        }

        method ArgType {name} {
            dict get $ArgDefs $name TYPE
        }

        method ShouldTrim {name} {
            expr {
                [dict exists $ArgDefs $name TRIM] && [dict get $ArgDefs $name TRIM]
            }
        }

        method ShouldUnindent {name} {
            expr {
                [dict exists $ArgDefs $name UNINDENT] && [dict get $ArgDefs $name UNINDENT]
            }
        }

        method HasArgComplete {name} {
            dict exists $ArgCompletes $name
        }

        method GetArgComplete {name} {
            dict get $ArgCompletes $name
        }

        method Unindent {text} {
            mdbg::cli::Unindent $text
        }

    } ; # oo::class create ArgumentParser

    #
    #  Parser for compound commands.
    #  Different subcommands have separate parsing rules)
    #
    #  Also multiple shared rulesets can be specified, see `breakpoints`
    #
    #  The constructor argument list is the following:
    #
    #  [-auto  # - global auto-resolve names by unique prefix (subcommands, switches, choices) ]
    #  [ -synopsis { command syntax; optional; auto-generated if missing } ]
    #  [ -brief { short command description } ]
    #  [ -description { long command description } ]
    #  [ -common-pre { <rules shared by all subcommands, notably common arguments at their beginning> } ]
    #  [ -default <default subcommand name when missing> ]
    #  <subCmdVarName: name of subcommand variable - mandatory>
    #  { # subcommand defintions - mandatory
    #     [<name1> - # dash means fall through case]
    #     [<name2> {
    #           ArgumentParser rules (see above)
    #     }]
    #     ...
    #  }
    #  [ { <common post-def rules, shared by all the subcommand parsers, eg shared autocomplete logic> } ]
    #
    #  For real-world examples, @see the construction of
    #     mdbg::cli::Step::ArgumentParser and
    #     mdbg::cli::Pipe::ArgumentParser

    oo::class create MultiCmdArgParser {

        variable MainParser
        variable ParserMap
        variable SubCmdVarName
        variable DefaultSubCmd

        #
        # Public
        #

        constructor {args} {
            oo::objdefine [self] mixin mdbg::cli::ParseCallerArgs

            [my ConstructorArgParser] parseArgs [list [self] [self method]] $args

            my ConstructorBody
        }

        destructor {
            if {[info exists ParserMap]} {
                foreach parser [dict values $ParserMap] {
                    $parser destroy
                }
            }
        }

        method parse {command arglist} {
            set mainvars [MainParser parse $command $arglist]

            set subcommand [dict get $mainvars subcommand]
            set arglist [dict get $mainvars args]

            set vars [dict create $SubCmdVarName $subcommand]
            set subParser [dict get $ParserMap $subcommand]
            set command [concat $command $subcommand]
            set subvars [$subParser parse $command $arglist]

            dict merge $vars $subvars
        }

        method autocomplete {cmd params prefix} {
            # TODO: support arguments before/after subcommand
            #       (still part of main parser rules)

            if {[llength $params] == 0} {
                set result [MainParser autocomplete $cmd $params $prefix]
                if {[info exists DefaultSubCmd] && [dict exists $ParserMap $DefaultSubCmd]} {
                    set subParser [dict get $ParserMap $DefaultSubCmd]
                    concat $result [$subParser autocomplete $DefaultSubCmd {} $prefix]
                } else {
                    set result
                }
            } else {
                set subcmd [lindex $params 0]
                if {[dict exists $ParserMap $subcmd]} {
                    set subParser [dict get $ParserMap $subcmd]
                    set params [lrange $params 1 end]
                    $subParser autocomplete $subcmd $params $prefix
                } elseif {[info exists DefaultSubCmd] && [dict exists $ParserMap $DefaultSubCmd]} {
                    set subParser [dict get $ParserMap $DefaultSubCmd]
                    $subParser autocomplete $DefaultSubCmd $params $prefix
                } else {
                    list
                }
            }
        }

        method getHelp {command {subcommand {}}} {
            if {$subcommand == {}} {
                MainParser getHelp $command
            } else {
                [my SubCmdParser $subcommand] getHelp [concat $command $subcommand]
            }
        }

        #
        #  Protected
        #

        method SubCmdList {} {
            dict keys $ParserMap
        }
        export SubCmdList

        method SubCmdParser {subcommand} {
            dict get $ParserMap $subcommand
        }
        export SubCmdParser

        #
        #  Private
        #

        method ConstructorArgParser {} {
            set Parser ::mdbg::cli::MultiCmdArgumentParser_ConstructorArgumentParser

            if {[info command $Parser] == ""} {
                mdbg::cli::ArgumentParser create $Parser {
                    -comment {
                        -pre {-var preDefinitions -default {}}
                    }
                    -args {
                        -auto {-flag -var auto}
                        -synopsis { -optional }
                        -brief { -optional }
                        -description { -optional }
                        -common-pre {-var commonDefinitions -default {}}
                        -default {-var defaultSubCmd -optional}
                        subCmdVarName {-var subCmdVarName}
                        subcommandDefs {-var subCmdDefs}
                        commonPostDefs {-var postDefinitions -default {}}
                    }
                }
            }

            return $Parser
        }

        method ConstructorBody {} {
            uplevel 1 {
                set autoDef [expr {$auto ? "-auto" : ""}]

                set SubCmdVarName $subCmdVarName
                set ParserMap [dict create]

                set subCommandChoices [list]
                set sameSubCommandDefList [list]

                foreach {subcommand definitions} $subCmdDefs {
                    lappend sameSubCommandDefList $subcommand
                    if {$definitions == "-"} {
                        continue
                    }
                    set subParserDefs [concat $autoDef $commonDefinitions $definitions $postDefinitions]

                    my DEBUG {
                        puts $DBGCH "subParser for [join $sameSubCommandDefList {, }]"
                    }

                    set subParser {}
                    foreach subCmd $sameSubCommandDefList {
                        if {[dict exists $ParserMap $subCmd]} {
                            set subParser [dict get $ParserMap $subCmd]
                            $subParser AddDefs $subParserDefs
                        } else {
                            set subParser [mdbg::cli::ArgumentParser new $subParserDefs]
                            dict set ParserMap $subCmd $subParser
                        }
                        my DEBUG {
                            puts $DBGCH "subParser for $subCmd is $subParser"
                        }
                        if {$subCmd ni $subCommandChoices} {
                            lappend subCommandChoices $subCmd
                        }
                    }
                    set sameSubCommandDefList [list]
                }

                set defaultSubCmdArgDef [
                    if {[info exists defaultSubCmd]} {
                        set DefaultSubCmd $defaultSubCmd
                        list -default $defaultSubCmd
                    } else {
                        list
                    }
                ]

                set helpDefs [list]
                if {[info exists synopsis]} {
                    lappend helpDefs -synopsis $synopsis
                }
                if {[info exists brief]} {
                    lappend helpDefs -brief $brief
                }
                if {[info exists description]} {
                    lappend helpDefs -description $description
                }

                # $preDefinitions
                set mainParserArgs [concat $autoDef $helpDefs [list \
                    -args [list \
                        subcommand [concat \
                            [list -choice $subCommandChoices] \
                            $defaultSubCmdArgDef
                        ] \
                        args {}
                    ] \
                ]]

                my DEBUG {
                    puts $DBGCH "create [self namespace]::MainParser"
                }

                mdbg::cli::ArgumentParser create MainParser $mainParserArgs

                oo::objdefine MainParser {
                    export  TestAndSetValidChoice
                }
            }

        }

    } ; # oo::class MultiCmdArgumentParser

    proc DEBUG_ARGUMENT_PARSER {body} {
        incr ::mdbg::DEBUG_ARGUMENT_PARSER +1
        try {
            uplevel 1 $body
        } finally {
            incr ::mdbg::DEBUG_ARGUMENT_PARSER -1
        }
    }

} ; # namespace eval mdbg::cli
