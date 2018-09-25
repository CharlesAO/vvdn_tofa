namespace eval mdbg {

    namespace eval cli::Target {

        namespace path ::mdbg

        variable ROOT_CONTEXT_ID
        variable TARGET_TO_CONTEXT

        if {[namespace exists ::tcf::RunControl::Events]} {

            proc OnContextRemoved {event idlist} {
                variable ROOT_CONTEXT_ID
                variable TARGET_TO_CONTEXT
                if {[info exists TARGET_TO_CONTEXT]} {
                    foreach id $idlist {
                        dict for {target ctx} $TARGET_TO_CONTEXT {
                            if {$ctx == $id} {
                                dict unset TARGET_TO_CONTEXT $target
                            }
                        }
                    }
                }
                if {[info exists ROOT_CONTEXT_ID]} {
                    foreach id $idlist {
                        if {[string equal $id $ROOT_CONTEXT_ID]} {
                            unset ROOT_CONTEXT_ID
                        }
                    }
                }
            }

            ::tcf::RunControl::Events::contextRemoved+ [namespace current]::OnContextRemoved
            dll::atexit {
                ::tcf::RunControl::Events::contextRemoved- [namespace current]::OnContextRemoved
            }

        }

        proc getRootContext {} {
            variable ROOT_CONTEXT_ID

            if {[info exists ROOT_CONTEXT_ID]} {
                return $ROOT_CONTEXT_ID
            }

            set rootContextList [tcf::queryContext /*]
            set len [llength $rootContextList]

            if {!$len} {
                return -code error "No root context!"
            }
            if {$len > 1} {
                set rootContextList [lsort $rootContextList]
                cli::Warning "multiple root contexts!"
            }
            set ROOT_CONTEXT_ID [lindex $rootContextList 0]
        }

        proc getAllRunContextIds {} {
            set result [list]
            set queue [list [getRootContext]]

            while {[llength $queue]} {
                set id [lindex $queue 0]
                lappend result $id
                set queue [concat \
                    [tcf::RunControl::getChildren $id] \
                    [lrange $queue 1 end] \
                ]
            }

            set result
        }

        proc ctxId2target {id {default {}}} {
            if {[regexp {^P[0-9]+:.(.+)$} $id -> target]} {
                return $target
            } else {
                return $default
            }
        }

        proc getTargetNameFromContextId {contextID} {
            set target [ctxId2target $contextID]
            if {$target != {}} {
                return $target
            }
            return -code error "Invalid context ID format:\n should be P<NUMBER>:<LETTER><TARGET>"
        }

        proc getContextIdFromTarget {target} {
            variable TARGET_TO_CONTEXT

            set target [string toupper $target]

            if {[regexp {^P[0-9]+:.(.+)$} $target context target]} {
                if {[info exists TARGET_TO_CONTEXT]} {
                    if {$context in [dict values $TARGET_TO_CONTEXT]} {
                        return $context
                    }
                }
                set targetContext [tcf::queryContext ID=\"$context\"]
                if {$targetContext == ""} {
                    return -code error "Invalid target context \"$context\"."
                }
                dict set TARGET_TO_CONTEXT $target $targetContext
                return $targetContext
            }

            if {[info exists TARGET_TO_CONTEXT] && [dict exists $TARGET_TO_CONTEXT $target]} {
                return [dict get $TARGET_TO_CONTEXT $target]
            }

            set rootContext [getRootContext]

            if {![regexp {^(P[0-9]+:.)A$} $rootContext -> prefix]} {
                return -code error "Root context id `$rootContext` has an improper format:\n should have been P<NUMBER>:<LETTER>A"
            }

            set quoted [format {"%s%s"} $prefix $target]

            set targetContext [tcf::queryContext ID=$quoted]

            if {$targetContext == ""} {
                return -code error "Invalid target $quoted."
            }

            set id [lindex $targetContext 0]
            dict set TARGET_TO_CONTEXT $target $id

            set id
        }

        proc getContextIdFromTargetList {targetList} {
            set result [list]
            foreach target $targetList {
                lappend result [getContextIdFromTarget $target]
            }
            set result
        }

        proc getValidTargets {pattern} {
            set result [list]
            foreach id [tcf::queryContext] {
                set target [ctxId2target $id]
                if {[string length $target] && [string match -nocase $pattern $target]} {
                    lappend result $target
                }
            }
            set result
        }

        proc getTargetContext {target} {
            cli::DeprecatedWarning [namespace current]::getContextIdFromTarget
            getContextIdFromTarget $target
        }

        proc isSet {} {
            info exists ::mdbg::TARGET
        }

        proc getContextIdFromTargetVar {args} {
            foreach targetVar $args {
                upvar 1 $targetVar target
                if {[info exists target]} {
                    return [targetid $target]
                }
            }
            targetid
        }

        proc getContextName {id {nameOrErrorVar {}} } {
            if {$nameOrErrorVar != {}} {
                upvar 1 $nameOrErrorVar nameOrError
            }
            if {[catch {
                        set result [::tcf::getContextName $id]
                    } nameOrError]} {
                return
            }
            return $result
        }

        proc getContextEventPrefix {id} {
            set target [ctxId2target $id]
            set name [getContextName $id]
            if {$target != ""} {
                set result "$target"
                if {$name != ""} {
                    append result ": $name ($id)"
                } else {
                    append result " ($id):"
                }
            } else {
                set result "$id:"
                if {[info exists name]} {
                    append result " $name"
                }
            }
            return $result
        }

        proc getBreakpointContextName {id} {
            set name [getContextName $id]
            expr {
                $name != "" ? "$name ($id)" : "$id"
            }
        }

    } ; # namespace eval cli::Target

    # returns context id for core
    # empty `core` means current target's context id
    proc targetid {{core {}}} {
        if {[string length $core]} {
            return [cli::Target::getContextIdFromTarget $core]
        }
        variable TARGET
        if {![info exists TARGET]} {
            return -code error "Target not set"
        }
        set TARGET
    }

    proc TARGET_ID {{core {}}} {
        cli::DeprecatedWarning "targetid"
        tailcall targetid $core
    }

    proc target {{core {}}} {
        variable TARGET
        if {[string length $core]} {
            set TARGET [cli::Target::getContextIdFromTarget $core]
        } elseif {![info exists TARGET]} {
            return -code error "Target not set"
        } else {
            set TARGET
        }
    }

    namespace export target targetid

    shell::autocomplete::addScript target {
        lsort -dictionary [cli::Target::getValidTargets $PREFIX*]
    }
    cli::Help::Manager add target -synopsis {
        target ?core?
    } -short "Get or set current target" -long {
        The current target determines the default context for most of the debugger commands.

        Its canonical name (prefixed with "P<processor#>:{A|S|F|V|?}") is displayed in the command prompt.

        The command recognizes the bare unprefixed core identifier and the canonical name.
        When called without arguments, it will return the canonical current target name.

        *** ALIASES

        ALIAS       FULL FORM
        t           target

        *** EXAMPLES

        Set the current target to the Leon OS:
        % target LOS

        Dump 1K of uncached DDR memory:
        % target A
        % mdump DDR_BASE_ADR [expr 1024/4]

    } -args {
        core {
            -synopsis {?core?}
            -short "target (core) identifier"
            -long {
                e.g "LOS", "LRT", "S0", "S1"
            }
        }
    }

    cli::Help::Manager add targetid -synopsis {
        targetid ?core?
    } -short "Get canonical TCF context ID of (current) target" -long {
        The canonical target name is the always prefixed with "P<processor#>:{A|S|F|V|?}")

        The command recognizes the bare unprefixed core identifier and the canonical name.

        When called without arguments, it will return the current target's canonical name.

        *** NOTE

        Contrary to the `target` command, this will not change the current target.

        *** EXAMPLES

        Get the canonical name of the root context
        % targetid A

        Get the canonical name of the Leon RT
        % targetid LOS


    } -args {
        core {
            -synopsis {?core?}
            -short "target (core) or group identifier"
            -long {
                e.g "A", "LOS", "LRT", "S0", "S1", "LALL", "LOS:RTEMS", etc
            }
        }
    }


    cli::Alias t target
    namespace export target targetid

} ; # namespace eval mdbg
