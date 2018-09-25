namespace eval mdbg::cli {
    namespace path ::mdbg

    #
    #  Try adding namespace(s) to path.
    #
    proc TryUseNamespace {args} {
        set nsList [uplevel 1 namespace path]
        foreach ns $args {
            if {[uplevel 1 namespace exists $ns]} {
                if {$ns ni $nsList} {
                    lappend nsList $ns
                }
            } else {
                set frameInfo [info frame -1]
                Warning "namespace \"$ns\" not found"
            }
        }
        uplevel 1 [list namespace path $nsList]

        if {[info exists frameInfo]} {
            dict with frameInfo {
                if {[info exists type] && $type == "source"} {
                    if {[info exists file]} {
                        set message "file \"$file\""
                        if {[info exists line]} {
                            append message " line $line"
                        }
                        PutWarning "    ($message)"
                    }
                }
            }
        }
    }


    # Warn about caller being deprecated
    # @param numParams - in case when subcommand is deprecated, message can contain
    #                    this amount of actual parameters from caller's invocation
    # @throws - message instead of printing if mdbg::NO_DEPRECATED is defined
    proc DeprecatedWarning { {otherFunc {}} {numParams 0} } {
        set message "\"[lrange [info level -1] 0 $numParams]\" is deprecated!"
        if {$otherFunc != {}} {
            append message " Use \"$otherFunc\" instead!"
        }
        if {[IsFlagVarTrue ::mdbg::NO_DEPRECATED]} {
            return -code error -level 2 $message
        } else {
            Warning $message
        }
    }

    # Not implemented error
    # @param numParams - in case when subcommand is not implemented, message can contain
    #                    this amount of actual parameters from caller's invocation
    # @throws - command not implemented message
    proc NotImplemented { {numParams 0} } {
        set message "\"[lrange [info level -1] 0 $numParams]\" is not implemented"
        return -code error -level 2 $message
    }

    # Raise "wrong # args" error
    # @param arglist list of correct arguments for default variant
    # @param args optional alternative correct argument list(s)
    proc WrongNumArgs { arglist args } {
        set funcName [lindex [info level 1] 0]
        set result "wrong # args"
        set separator ": should be "
        foreach argl [list $arglist {*}$args] {
            append result [format {%s"%s"} $separator [concat $funcName $argl]]
            set separator " or "
        }
        return -code error -level 2 $result
    }

    # for treating Ctrl+C during the operation.
    proc TrapInterrupt {script} {
        upvar #0 ::mdbg::cli::_NESTED_TRAP_INTERRUPT nesting
        upvar #0 ::mdbg::TRAP_INTERRUPT trap
        set trap {}
        incr nesting
        set code [catch { uplevel 1 $script } result options]
        if {[incr nesting -1] <= 0} {
            unset nesting
        }
        try {
            if {$code} {
                if {[info exists trap] && $trap != {}} {
                    return -level 2 -code error $result
                }
                dict with options {
                    incr -level
                }
                return -options $options $result
            }
        } finally {
            if {![info exists nesting]} {
                unset -nocomplain trap
            }
        }
        set result
    }

    proc CheckInterrupt {} {
        upvar #0 ::mdbg::TRAP_INTERRUPT trap
        set code [catch {update} result options]
        if {[info exists trap] && $trap != {}} {
            return -code error -level 2 $mdbg::TRAP_INTERRUPT
        }
        if {$code} {
            return -options $options $result
        }
    }

    # moves errorInfo and errorStack from dictionary to
    # variables in mdbg::cli namespace
    proc MoveErrorToCli {errorOptionsDictVar} {
        upvar 1 $errorOptionsDictVar options

        set DEBUG [IsFlagVarTrue ::mdbg::DEBUG_CLI_ERROR_STACK]

        foreach {opt var} {
            -errorinfo errorInfo
            -errorstack errorStack
        } {
            set var [namespace current]::$var
            if [dict exists $options $opt] {
                set $var [dict get $options $opt]

                if $DEBUG continue

                dict unset options $opt
            }
        }
        return
    }

    proc InternalErrorInfo {} {
        variable errorInfo
        if {![info exists errorInfo]} {
            return
        }
        set errorInfo
    }

    proc InternalErrorStack {} {
        variable errorStack
        if {![info exists errorStack]} {
            return
        }
        set errorStack
    }

    # @param script - script to execute (on caller's stack)
    # sets cli::errorInfo and cli::errorStack on error
    proc CatchAndPrintError {script {contextMessage {}}} {
        set code [catch {
            set result [uplevel 1 $script]
        } error options]

        switch -exact $code {
            0 {
                return $result
            }
            1 {
                MoveErrorToCli options
                if {[string length $contextMessage]} {
                    PutError "$contextMessage: $error"
                } else {
                    PutError $error
                }

                return
            }
        }
        dict with options {
            incr -level
        }
        return -options $options $error
    }

    # Suppresses error info from within script
    # @param script - script to execute (on caller's stack)
    # sets cli::errorInfo and cli::errorStack on error
    proc NoErrorStack {script} {
        upvar #0 [namespace current]::_NESTED_NO_ERROR_STACK nesting
        incr nesting
        set code [catch {
            TrapInterrupt {
                set result [uplevel 1 $script]
            }
        } error options]
        incr nesting -1
        switch -exact $code {
            0 {
                return $result
            }
            1 {
                if {!$nesting} {
                    MoveErrorToCli options
                    dict with options {
                        set -code 1
                        set -level 2
                    }
                    return -options $options $error
                }
            }
        }
        dict with options {
            incr -level
        }
        return -options $options $error
    }

    # Similar to Tcl's uplevel but increments the return
    # level as well for proper flow control in caller.
    # Required when placing nested uplevels in catches
    proc UpScript {level script args} {
        incr level
        if {[catch {
                    uplevel $level $script {*}$args
                } result options]} {
            dict with options {
                incr -level $level
            }
            return -options $options $result
        } else {
            return $result
        }
    }

    proc ParseArgs {name argdefs args} {
        DeprecatedWarning

        set parser [mdbg::cli::ArgumentParser new $argdefs]
        try {
            uplevel 1 [list $parser parse $name $args]

        } finally {
            $parser destroy
        }
    }

    #
    #  Create scoped object of class
    #
    proc UsingObj {class varname args} {
        set level 1
        upvar $level $varname obj
        if {$args == {}} {
            WrongNumArgs [list $class $varname "?constructorArgs...?" "body"]
        }
        set constructorArgs [lrange $args 0 end-1]
        set body [lindex $args end]
        set obj [uplevel $level [list $class new] $constructorArgs]
        try {
            uplevel $level $body
        } finally {
            $obj destroy
            unset obj
        }
    }

    proc PreserveValue {varName script} {
        upvar 1 $varName var

        if {[info exists var]} {
            set prev $var
        }
        try {
            uplevel 1 $script
        } finally {
            if {[info exists prev]} {
                set var $prev
            } else {
                unset -nocomplain var
            }
        }
    }

    # Interactive Debug support.
    # Stops script and provides a
    # mini console to evaluate
    # stuff on caller's stack.
    # empty line allows resuming execution
    proc DEBUG_ME_NOW {args} {
        set debug $::mdbg::Channels::debug
        puts $debug [concat DEBUG_ME_NOW $args]
        if {[info frame] > 1} {
            set frame [info frame -1]
            switch [dict get $frame type] {
                source {
                    set path [dict get $frame file]
                    set line [dict get $frame line]
                    set prompt "$path:$line\n"
                }
                eval {
                    set prompt "\"[dict get $frame cmd]\" "
                }
                proc {
                    set proc [dict get $frame proc]
                    set line [dict get $frame line]
                    set prompt "proc $proc, line $line\n"
                }
                default {
                    set prompt "<[dict get $frame type]> "
                }
            }
        } else {
            set prompt " :: "
        }
        while {[set line [mdbg::cli::ColorGets {yellow bold} "$prompt% "]] != {}} {
            if {[catch {uplevel 1 $line} result options]} {
                if {[dict exists $options -errorinfo]} {
                    PutError [dict get $options -errorinfo]
                    continue
                }
                PutError $result
            }
            PutResult [mdbg::prettyTclObject $result]
        }
    }

    namespace export DEBUG_ME_NOW

    proc DEBUG_LOG_EVERYTHING {} {
        dll::loglevel debug
        tcf::log on
        tcf::logMode mvtcf context
        displaystate verbose
    }

    proc DEBUG_TCL_FRAMES {} {
        set currentFrame [info frame]
        for {set i 2} {$i < $currentFrame} {incr i} {
            set line [list]
            dict for {key value} [info frame -$i] {
                lappend line "$key=[Shorten $value 40 0.66]"
            }
            PutDebug [join $line "; "]
        }
    }
    namespace export DEBUG_TCL_FRAMES

    # --------------

    proc incrAddr {addrVar {offset 1}} {
        upvar 1 $addrVar addr
        set addr [cli::hexAddr [expr {$addr + $offset}]]
    }

    proc decrAddr {addrVar {offset 1}} {
        upvar 1 $addrVar addr
        set addr [cli::hexAddr [expr {$addr - $offset}]]
    }

    proc hexAddr {address} {
        format "0x%08X" $address
    }

    proc hexaddr {address} {
        format "%#.8x" $address
    }

    proc hexStr {num} {
        format "0x%X" $num
    }

    proc hexstr {num} {
        format "%#x" $num
    }

    proc expandTabs {str {width 4}} {
        string map [dict create "\t" [string repeat " " $width]] $str
    }

    proc Alias {newNames oldName args} {
        set currentNs [uplevel 1 namespace current]
        if {![string match *:: currentNs]} {
            append currentNs ::
        }
        if {![string match ::* oldName]} {
            set oldName $currentNs$oldName
        }
        foreach newName $newNames {
            if {![string match ::* $newName]} {
                set newName $currentNs$newName
            }
            namespace inscope :: [list \
                interp alias {} $newName {} $oldName
            ] {*}$args
        }
    }

    proc IsFlagVarTrue {flagVar} {
        upvar 1 $flagVar flag
        expr {[info exists flag] && (
            [string is boolean -strict $flag] ||
            [string is double -strict $flag]
        ) && $flag}
    }

    # Filter or transform list in-place
    proc UpdateList {listVar itemVar scriptBody} {
        upvar 1 $listVar list
        upvar 1 $itemVar item
        set len [llength $list]
        for {set i 0} {$i < $len} {incr i} {
            set item [lindex $list $i]
            try {
                uplevel 1 $scriptBody
            } finally {
                if {![info exists item]} {
                    set prev [expr {$i - 1}]
                    set next [expr {$i + 1}]
                    set list [concat [lrange $list 0 $prev] [lrange $list $next end]]
                    set i $prev
                    incr len -1
                } else {
                    lset list $i $item
                }
            }
        }
        set list
    }

    proc GroupList {list columnCount} {
        set length [llength $list]
        set rows [expr { ( $length + $columnCount - 1 ) / $columnCount}]

        set begin 0
        set end [expr {$columnCount - 1}]

        set result [list]
        for {set row 0} {$row < $rows} {incr row} {
            lappend result [lrange $list $begin $end]
            incr begin $columnCount
            incr end $columnCount
        }
        set result
    }

    ArgumentParser create Fallback::ArgumentParser {
        -brief "Execute series of scripts until one succeeds"
        -args {
            -silent {
                -brief "Don't print error."
                -flag
            }
            -info {
                -brief "Print full error trace."
                -flag
            }
            -debug {
                -brief "Print commands executed."
                -flag
                -hidden
            }
            script {}
            fallback {
                -synopsis {?script...? fallback}
                -multi
            }
        }
        -mutex {
            {-silent {-info -debug}}
        }
        -arg-complete-scripts {
            {script fallback} {
                ::mdbg::shell::autocomplete::listOfCommands $PREFIX
            }
        }
    }

} ; # namespace eval mdbg::cli