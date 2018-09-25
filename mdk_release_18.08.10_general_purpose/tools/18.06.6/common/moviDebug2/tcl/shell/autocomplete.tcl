# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : autocomplete.tcl
# Description: MoviDebug2 shell autocomplete framework
# Created on : May 26, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::shell {

    # This function gets called from the moviDebug Shell console,
    # whenever the user presses TAB first for a given input.
    # The returned list is used to complete the last input line.
    # Each item will replace the full input line at subsequent TAB keypresses.
    # @param line - all lines of user input (this contains the previous lines in case of
    #               an incomplete multiline command)
    # @returns list of possible alternatives of the full last line of input
    # TODO: full support for multiline commands.
    #       This is NOT TRIVIAL, since the input needs to be properly parsed
    #       according to the 12 rules of the http://wiki.tcl.tk/Dodekalogue
    #
    proc autocomplete {line} {
        set lastLine [lindex [split $line \n] end]
        autocomplete::lineComplete $lastLine
    }

    #
    #  Autocomplete support procedure namespace
    #  Two important procedures should be always present:
    #  `mdbg::shell::autocomplete::addScript` and `mdbg::shell::autocomplete::addCommand`.
    #  (dummy procs are okay which ignore their arguments)
    #  The rest of procs must be called only from
    #  autocompletion callbacks.
    #
    namespace eval autocomplete {
        variable PARAM_COMPLETE_COMMANDS {}
        variable PARAM_COMPLETE_PATTERNS {}

        #
        #  Add autocomplete script body for list of commands
        #  The variables COMMAND, PARAMS and PREFIX will be available inside.
        #  The body should return a list of completion matches for PREFIX (not the full line)
        #
        proc addScript {commandList body} {
            set ns [uplevel 1 namespace current]
            set lambda [list {COMMAND PARAMS PREFIX} $body $ns]
            uplevel 1 [list \
                [namespace current]::addCommand $commandList apply $lambda
            ]
        }

        #
        #  Add custom autocomplete command for a list of commands
        #  The 'completionCommand' will receive all the supplied extra 'args'
        #  as individual arguments, followed by 'command' 'params` and `prefix`.
        #  It should return a list of completion matches for 'prefix'. (not the full line)
        #
        proc addCommand {commandList completionCommand args} {
            set ns [uplevel 1 namespace current]
            set script [list $completionCommand {*}$args]
            foreach cmd $commandList {
                set cmd [fullyQualified $cmd $ns]
                if {![string match {*[*?]*} $cmd]} {
                    addParamCompleteCommand $cmd $script
                } else {
                    addParamCompletePattern $cmd $script
                }
            }
        }

        proc fullyQualified {cmd {ns ::}} {
            if {[string match ::* $cmd]} {
                return $cmd
            }
            if {![string match *:: $ns]} {
                append ns ::
            }
            return $ns$cmd
        }

        proc addParamCompleteCommand {cmd script} {
            variable PARAM_COMPLETE_COMMANDS
            dict set PARAM_COMPLETE_COMMANDS $cmd $script
        }

        proc addParamCompletePattern {cmdPattern script} {
            variable PARAM_COMPLETE_PATTERNS
            dict set PARAM_COMPLETE_PATTERNS $cmdPattern $script
        }

        proc lineComplete {line} {
            set result {}
            if {[regexp {^(.*?)([^;]*)$} $line -> prefix lastCmd]} {
                skipSpaces prefix lastCmd
                appendListWithPrefix result $prefix [commandComplete $lastCmd]
            }
            set result
        }

        proc skipSpaces {preVar lastCmdVar} {
            upvar 1 $preVar prefix
            upvar 1 $lastCmdVar lastCmd
            if {[regexp {([ ]*)(.*)} $lastCmd -> spaces trimmed]} {
                append prefix $spaces
                set lastCmd $trimmed
            }
        }

        proc commandComplete {lastCmd} {

            set command [paramSplitter split $lastCmd]

            DEBUG {
                puts $DBGCH "-------------------------"
                puts $DBGCH "lastCmd       : \"$lastCmd\""
                puts $DBGCH "split command : \"[join $command { | }]\""
            }

            if {![llength $command]} {
                set cmd ""
                set param ""
                lappend command {}
            } else {
                set param  [lindex $command end]
                if {![hasTrailingSpace $param]
                    && [hasTrailingSpace $lastCmd]} {
                    set param ""
                    lappend command {}
                }
                set cmd [expr {
                    [llength $command] > 1 ? [lindex $command 0] : ""
                }]
            }

            set params [lrange $command 1 end-1]
            set prefix [string range $lastCmd 0 [expr {
                  [string length $lastCmd]
                - [string length $param]
                - 1
            }]]

            DEBUG {
                puts $DBGCH "cmd           : $cmd"
                puts $DBGCH "params        : [join $params { | }]"
                puts $DBGCH "param         : $param"
                puts $DBGCH "prefix        : \"$prefix\""
            }

            set result [list]
            if {
                [regexp {^(\[)(.*?[^\]\s]\s*)?$} $param -> prefix2 lastCmd2] ||
                [regexp {^(\{)(.*?[^\}\s]\s*)?$} $param -> prefix2 lastCmd2]
            } {
                DEBUG {
                    puts $DBGCH "lastCmd2      : $lastCmd2"
                    puts $DBGCH "prefix2       : \"$prefix2\""
                }
                appendListWithPrefix result $prefix$prefix2 [commandComplete $lastCmd2]
            } else {
                appendListWithPrefix result $prefix [paramComplete $cmd $params $param]
            }

            set result
        }

        proc paramCompleteAfter {count cmd params param} {
            while {$count > 0} {
                incr count -1
                if {$cmd != {} || $params != {}} {
                    set cmd [lindex $params 0]
                    set params [lrange $params 1 end]
                } elseif {$count || $param == {}} {
                    return [list $param]
                }
            }
            paramComplete $cmd $params $param
        }

        proc paramComplete {cmd params param} {
            expandCommandVariable cmd
            expandAliases cmd params
            set result [list]
            if {[string match {$*} $param]} {
                set px [string range $param 1 end]
                appendListWithPrefix result \$ \
                    [listOfVars $px] \
                    [listOfNamespaces $px]
            } elseif {[string match {\[*} $param]} {
                set px [string range $param 1 end]
                appendListWithPrefix result "\[" \
                    [listOfCommands $px] \
                    [listOfPathNamespaces $px]
            } else {
                set namespaces [listOfNamespaces $param]
                if {[string length $cmd]} {
                    set lopc [listOfParamComplete $cmd $params $param]
                    appendList result $lopc
                } else {
                    appendList result [listOfCommands $param]
                }
                appendList result [listOfPathNamespaces $param]
                appendListWithPrefix result "\$" \
                    [listOfVars $param] \
                    [listOfNamespaces $param]

                if {[string length $cmd] && ![llength $lopc]} {
                    appendList result [listOfFiles $param*]
                }
            }
            set result
        }

        proc expandAliases {cmdVar paramsVar {ns ::}} {
            upvar 1 $cmdVar cmd
            upvar 1 $paramsVar params
            set aliases [interp aliases]
            set origin [originOfCommand $cmd]
            while {$origin in $aliases &&
                [interp target {} $origin] == {}
            } {
                set aliases [lsearch -all -inline -exact -not $aliases $origin]
                set expanded [interp alias {} $origin]
                set cmd [lindex $expanded 0]
                set params [concat [lrange $expanded 1 end] $params]
                set origin [originOfCommand $cmd]
            }
        }

        proc expandCommandVariable {cmdVar} {
            upvar 1 $cmdVar cmd
            if {[regexp {\s*\$(.*?)\s*$} $cmd -> varName]} {
                if {![string match ::* $varName]} {
                    set varName ::$varName
                }
                if {[info exists $varName]} {
                    set cmd [set $varName]
                }
            }
        }

        proc trimLeadingNsSep {nameVar} {
            upvar 1 $nameVar name
            while {[string match ::* $name]} {
                set name [string range $name 2 end]
            }
            set name
        }

        proc trimTrailingNsSep {nameVar} {
            upvar 1 $nameVar name
            while {[string match *:: name]} {
                set name [string range $name 0 end-2]
            }
            set name
        }

        proc relativeCommandName {ns cmd} {
            trimLeadingNsSep ns
            trimLeadingNsSep cmd
            set len [string length $ns]
            set cmdpre [string range $cmd 0 $len-1]
            set cmdpost [string range $cmd $len end]
            if {![string equal $cmdpre $ns]} {
                puts stderr "$cmdpre <> $ns"
                return ::$cmd
            }
            trimLeadingNsSep cmdpost
        }

        proc listOfCommands {prefix {ns ::}} {
            set result [list]
            set parentNS [namespace qualifiers $prefix]
            foreach cmd [namespace inscope $ns info commands $prefix*] {
                set cmd [namespace tail $cmd]
                lappend result [getQualifiedName $parentNS $cmd]
            }
            if {![isFullyQualified $prefix]} {
                foreach pathNS [namespace inscope $ns namespace path] {
                    set nsPrefix [getQualifiedName $pathNS $prefix]
                    foreach cmd [namespace inscope $ns info commands $nsPrefix*] {
                        lappend result [relativeCommandName $pathNS $cmd]
                    }
                }
            }
            escapeList [sortNames $result]
        }

        proc listOfMethods {obj pattern} {
            set result [list]
            if {[info object isa object $obj]} {
                foreach method [info object methods $obj -all] {
                    if {   $method != "destroy"
                        && [string match $pattern $method]} {
                        lappend result $method
                    }
                }
            }
            sortNames $result
        }

        proc filterList {list pattern} {
            lsearch -all -inline -glob $list $pattern
        }

        proc filterNumberRange {low high pattern} {
            set result [list]
            for {set i $low} {$i <= $high} {incr i} {
                if {[string match $pattern $i]} {
                    lappend result $i
                }
            }
            set result
        }

        proc listOfVars {prefix {ns ::}} {
            set result {}
            if {[regexp {^(.*)\((.*)$} $prefix -> var indexPrefix]} {
                foreach index [namespace inscope $ns array names $var] {
                    if {[string match $indexPrefix* $index]} {
                        lappend result [escapeString $var\($index\)]
                    }
                }
            } else {
                foreach var [namespace inscope $ns info vars $prefix*] {
                    if {[namespace inscope $ns array exists $var]} {
                        foreach index [namespace inscope $ns array names $var] {
                            lappend result [escapeString $var\($index\)]
                        }
                    } else {
                        lappend result [escapeString $var]
                    }
                }
            }
            sortNames $result
        }

        proc listOfFiles {arg0 args} {
            if {[llength $args]} {
                set args [lreplace $args 0 -1 $arg0]
            } else {
                set args [list -- $arg0]
            }
            if {[catch {
                        set result [escapeList [lsort -nocase [glob -nocomplain {*}$args]]]
                    } result]} {
                return [list]
            }
            lsort -dictionary $result
        }

        proc isFullyQualified {name} {
            string equal [string range $name 0 1] ::
        }

        proc getQualifiedName {ns name} {
            if {[string length $ns]} {
                if {![string equal [string range $ns end-1 end] ::]} {
                    append ns ::
                }
                return $ns$name
            }
            set name
        }

        proc listOfChildNamespaceNames {parent {ns ::}} {
            set result [list]
            if {[namespace inscope $ns namespace exists $parent]} {
                set children [namespace inscope $ns namespace children $parent]
                foreach child $children {
                    lappend result [namespace tail $child]
                }
            }
            set result
        }

        proc listOfNamespaces {prefix {ns ::}} {
            set parent [namespace qualifiers $prefix]
            set pattern [namespace tail $prefix]*
            set result [list]
            foreach child [listOfChildNamespaceNames $parent $ns] {
                if {[string match $pattern $child]} {
                    lappend result [escapeString ${parent}::${child}]
                }
            }
            sortNames $result
        }

        proc listOfPathNamespaces {prefix {ns ::}} {
            set parent [namespace qualifiers $prefix]
            set pattern [namespace tail $prefix]*
            set result [list]
            foreach child [listOfChildNamespaceNames $parent $ns] {
                if {[string match $pattern $child]} {
                    lappend result [escapeString ${parent}::${child}]
                }
            }
            if {![isFullyQualified $parent]} {
                foreach pathNS [namespace inscope $ns namespace path] {
                    foreach child [listOfChildNamespaceNames ${pathNS}::${parent} $ns] {
                        if {[string match $pattern $child]} {
                            lappend result [escapeString \
                                [getQualifiedName ${parent} ${child}] \
                            ]
                        }
                    }
                }
            }
            sortNames $result
        }

        proc NameOrder {lhs rhs} {
            set llen [string length $lhs]
            set rlen [string length $rhs]
            for {set i 0} {$i < $llen && $i < $rlen} {incr i} {
                set lc [string index $lhs $i]
                set rc [string index $rhs $i]
                set comp [string compare $lc $rc]
                if {!$comp} continue

                if {[string match {[A-Z]} $lc]} {
                    if {[string match {[a-z]} $rc]} {
                        return 1
                    }
                } elseif {[string match {[a-z]} $lc]} {
                    if {[string match {[A-Z]} $rc]} {
                        return -1
                    }
                }
                return $comp
            }
            expr {$llen - $rlen}
        }

        proc sortNames {list} {
            lsort -command NameOrder -unique $list
        }

        proc originOfCommand {cmd {ns ::}} {
            if {
                [catch {
                        set infocmd [namespace inscope $ns info command $cmd]
                        if {[string length $infocmd]} {
                            set cmd [namespace inscope $ns namespace origin $infocmd]
                        } else {
                            if {![isFullyQualified $cmd]} {
                                foreach pathNS [namespace inscope $ns namespace path] {
                                    set nsCmd [getQualifiedName $pathNS $cmd]
                                    set infocmd [namespace inscope $ns info command $nsCmd]
                                    if {$infocmd != ""} {
                                        set cmd [namespace inscope $ns namespace origin $infocmd]
                                    }
                                }
                            }
                        }
                    } error]
            } {
                puts stderr $error
            }
            set cmd
        }

        proc listOfParamComplete {cmd params param {ns ::}} {
            variable PARAM_COMPLETE_COMMANDS
            variable PARAM_COMPLETE_PATTERNS
            set cmd [originOfCommand $cmd $ns]
            set result [list]
            if {[catch {
                        if {[dict exists $PARAM_COMPLETE_COMMANDS $cmd]} {
                            set script [dict get $PARAM_COMPLETE_COMMANDS $cmd]
                            set result [namespace inscope $ns $script $cmd $params $param]
                        } else {
                            set match no
                            dict for {keys script} $PARAM_COMPLETE_PATTERNS {
                                if {[string match $key $cmd]} {
                                    foreach item [namespace inscope $ns $script $cmd $params $param] {
                                        lappend result $item
                                    }
                                    set match yes
                                }
                            }
                            if {!$match} {
                                if {$params == {} && [info object isa object $cmd]} {
                                    set result [filterList [
                                        info object methods $cmd -all
                                    ] $param*]
                                }
                            }
                        }
                    } error]} {
                puts stderr $error
            }
            set result
        }

        proc escapeString {str} {
            string map {
                " " "\\ "
                "\"" "\\\""
                \$ \\\$
                \\ \\\\
                \{ \\\{
                \} \\\}
                \[ \\\[
                \] \\\]
                "\t" "\\t"
                "\b" "\\b"
                "\n" "\\n"
                "\r" "\\n"
                "\v" "\\v"
            } $str
        }

        proc escapeList {args} {
            set result {}
            foreach list $args {
                foreach item $list {
                    lappend result [escapeString $item]
                }
            }
            set result
        }

        proc appendListWithPrefixAndSuffix {listVar prefix suffix args} {
            upvar 1 $listVar result
            foreach items $args {
                foreach item $items {
                    lappend result $prefix$item$suffix
                }
            }
            set result
        }

        proc appendListWithPrefix {listVar prefix args} {
            upvar 1 $listVar result
            foreach items $args {
                foreach item $items {
                    lappend result $prefix$item
                }
            }
            set result
        }

        proc appendList {listVar args} {
            upvar 1 $listVar result
            foreach items $args {
                foreach item $items {
                    lappend $listVar $item
                }
            }
            set result
        }

        proc hasTrailingSpace {str} {
            string is space -strict [string index $str end]
        }

        ParamSplitter create paramSplitter

        proc DEBUG {script {varName ::mdbg::DEBUG_AUTOCOMPLETE} } {
            if {[info exists $varName] &&
                [string is true -strict [set $varName]]
            } {
                uplevel 1 {
                    if {[info exists ::mdbg::Channels::debug]} {
                        upvar 0 ::mdbg::Channels::debug DBGCH
                    } else {
                        set DBGCH stdout
                    }
                }
                uplevel 1 $script
            }
        }

    } ; #namespace eval autocomplete

} ; # namespace eval mdbg::shell

