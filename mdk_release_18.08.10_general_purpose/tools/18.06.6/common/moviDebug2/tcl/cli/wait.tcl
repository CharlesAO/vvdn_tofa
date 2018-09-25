namespace eval mdbg {

    namespace eval cli::Wait {

        namespace path {::mdbg}

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Wait a time period or event(s)."
            -description {
                Allows waiting a specified number of milliseconds or certain events.
                Waiting for events can be time-limited.

                Tcl events are processed during the wait.

                Interruptible using the Control+C key combination in interactive mode.

                *** EXAMPLES

                Wait for 3000 ms:
                % wait -ms 3000

                Wait for 3000 ms for the Shave0 to suspend. Raise error on timeout:
                % wait -target S0 -suspended -timeout 3000

                Wait for 3000 ms for the Leon RT to suspend.\
                Also stop if a breakpoint is hit on another core.\
                Raise error on timeout:
                % wait -target LRT -suspended -anybp -timeout 3000

                Run application and wait until the LRT starts:
                % wait -target LRT -resumed -after {run}
            }
            -args {
                -ms {
                    -optional
                    -type integer
                    -brief "Milliseconds to wait."
                    -description {
                        If provided, causes the command to wait unconditionally,
                        until the specified number of milliseconds pass.
                    }
                }
                -target {
                    -optional
                    -brief "Target to wait for."
                    -description {
                        Specifiy the target of which state is conditioning the wait.
                        If missing, the current target will be considered.

                        This is normally used in conjuction one of the
                        "-suspended", "resumed", "-running" or "-not-running" options.
                    }
                }
                -service {
                    -optional -var TcfService
                    -brief "TCF Service."
                    -description {
                        This is used for advanced use-cases.
                        The specified TCF Service's events will be monitored.
                        Used together with "-event", "-filter-..."
                    }
                }
                -timeout {
                    -type integer
                    -default {}
                    -brief "Raise error on timeout (ms)."
                    -description {
                        Causes the command to raise an error if specified time elapses without normal return.
                    }
                }
                -suspended {
                    -flag
                    -var waitSuspended
                    -brief "Wait on target suspend."
                    -description {
                        This is the default behaviour when waiting on target.
                        Return condition is that the target is in Suspended state.
                    }
                }
                -resumed {
                    -flag
                    -var waitResumed
                    -brief "Wait on target resumed."
                    -description {
                        Return condition is that the target resumes execution from the Suspended or state.
                        It can also mean that the target loses its clock or power.
                    }
                }
                -running {
                    -flag
                    -var waitRunning
                    -brief "Wait on target is running."
                    -description {
                        Return condition is that the target is running \
                        (i.e. it has clock, power, and it's not Suspended)
                    }
                }
                -not-running {
                    -flag
                    -var waitNotRunning
                    -brief "Wait on target not running."
                    -description {
                        Return condition is that the target is *not* running.
                    }
                }
                -anybp {
                    -flag
                    -requires {-suspended}
                    -var waitAnyBP
                    -brief "Wait on any breakpoint hit."
                    -description {
                        Requires "-suspended" switch to be present.
                        Triggers return also on any other breakpoint being hit.
                    }
                }
                -event {
                    -optional
                    -multi
                    -var TcfEvent
                    -requires {-service}
                    -default *
                    -brief "TCF Event."
                    -description {
                        Requires the "-service" argument.

                        Restricts the monitored events of the aforementioned service.
                        Can be given multiple times to specify multiple events.

                        The default is to monitor all events of the service ("*").
                    }
                }
                -filter-script {
                    -var filterBody
                    -requires {-service}
                    -brief "TCF event filter script."
                    -description {
                        Specifies a script to evaluate when one of the monitored events occurs.
                        Should return logical true to signal that the wait condition has been satisfied.
                        (i.e. the wait is over and the command can return)

                        The script body will receive arguments consisting of:
                        - values specified by "-filter-args" argument.
                        - event name
                        - event arguments (depend on actual event)

                        Default implementation is to return true, i.e. the first event will
                        satisfy the wait condition.
                    }
                    -default {
                        return 1
                    }
                }
                -filter-args {
                    -var filterArgs
                    -requires {-filter-script}
                    -default {}
                    -brief "Value(s) transmitted as first parameter(s) of filter script."
                    -description {
                        `filterArgs` is a list of values.
                        Each one of the items will become a separate argument of the filter script.
                    }
                }
                -filter-params {
                    -var filterParams
                    -requires {-filter-script}
                    -default {args}
                    -brief "Formal parameters of filter script."
                    -description {
                        The order is the following:
                        - argument names for each value in the "-filter-args" list
                        - parameter receiving TCF event name
                        - event parameters of the TCF event.

                        The default is "args" which takes in all the actual parameters as a list.
                    }
                }
                -after {
                    -optional
                    -var triggerScript
                    -brief "Run trigger script."
                    -description {
                        Allows specifing a script which will be executed
                        *after* the event monitoring has been set up,
                        on the caller's stack frame.

                        This script is usually asynchronous, and it is expected
                        to cause the wait condition to be satisfied during execution
                        or at a later time.
                    }
                }
                targetOrMs {
                    -optional
                    -brief "Target or milliseconds."
                    -description {
                        This is a combination of the "-ms" and "-target" argument.
                        It is for convenience to allow syntax such as:

                        "wait 1000" - wait a second
                        "wait LOS"  - wait for LeonOS to suspend.

                        It cannot be provided if one of "-ms" and "-target" is already present.
                        It is also incompatible with the advanced options
                        "-service", "-event", "-filter-script", "-filter-args", "-filter-params".
                    }
                }
            }
            -comment {
                # mutually exclusive switch groups are layed out vertically
            }
            -mutex {
                {
                    targetOrMs
                    {-target -ms -timeout -after -suspended -resumed -running -not-running -anybp -service}
                }
                {
                    -service
                    {-target -suspended -resumed -running -not-running -anybp}
                }
                {
                    -resumed
                    -not-running
                    {-suspended -running -anybp}
                }
                {
                    -running
                    -anybp
                }
            }
            -arg-complete-scripts {
                {-target targetOrMs} {
                    ::mdbg::cli::Target::getValidTargets $PREFIX*
                }
            }
        } ; # cli::ArgumentParser create ArgumentParser

        oo::objdefine [namespace current]::ArgumentParser {

            method detectWaitType {numArgs} {
                if {$numArgs == 0} {
                    uplevel 1 {
                        set waitType TARGET_SUSPENDED_OR_ANY_BP
                    }
                    return
                }
                uplevel 1 {
                    if {[info exists ms]} {
                        set waitType MILLISECONDS
                    } elseif {[info exists targetOrMs]} {
                        if {[string is integer -strict $targetOrMs]} {
                            set waitType MILLISECONDS
                            set ms $targetOrMs
                        } else {
                            set waitType TARGET_SUSPENDED_OR_ANY_BP
                            set target $targetOrMs
                        }
                    } elseif {[info exists TcfService]} {
                        set waitType TCF_EVENTS_AFTER
                        if {![info exists triggerScript]} {
                            set triggerScript {}
                        }

                    } else {
                        set waitType TARGET_
                        if {$waitSuspended} {
                            if {$waitRunning} {
                                append waitType RUNNING_OR_
                            }
                            append waitType SUSPENDED
                            if {$waitAnyBP} {
                                append waitType _OR_ANY_BP
                            }
                        } elseif {$waitResumed} {
                            append waitType RESUMED
                        } elseif {$waitRunning} {
                            append waitType RUNNING
                        } elseif {$waitNotRunning} {
                            append waitType NOT_RUNNING
                        } else {
                            append waitType SUSPENDED_OR_ANY_BP
                        }
                        if {[info exists triggerScript]} {
                            append waitType _AFTER
                        }
                    }
                }
            }

            method setupTriggerScript {} {
                uplevel 1 {
                    if {[info exists triggerScript] && $triggerScript != ""} {
                        set triggerScript [list uplevel 1 $triggerScript]
                    }
                }
            }

        } ; # oo::objdefine ArgumentParser

        # helper namespace to contain variables to wait on
        namespace eval Variables {

            # Create wait helper variable
            # @param initialValue - initial value of variable
            # @return - fully qualified name of helper variable
            proc Create {{initialValue {}}} {
                variable INDEX
                set waitVarName [namespace current]::#[incr INDEX]
                set $waitVarName $initialValue
                set waitVarName
            }

        }

        # Wait an interval
        # @param interval - duration inmilliseconds
        # @note Events are processed during wait
        proc ms {interval} {
            set waitVar [Variables::Create 0]
            set afterid [after $interval [list incr $waitVar]]
            try {
                vwait $waitVar
            } finally {
                unset $waitVar
            }
        }

        # Wait for TCF events triggered by script
        #
        # @param triggerScript - script to execute after event monitoring has been set up.
        #                        Can be empty; can return if necessary.
        # @param timeoutms - (optional) number of milliseconds after Timeout exception.
        #                    Can be empty
        # @param Service - service of which events will be monitored
        #                  (see sub-namespaces of ::tcf:: )
        # @param events - list of event handlers which need to be monitored
        #                 (see declarations in ::tcf::${Service}::Events)
        # @param filterParams - list of formal parameters of filterBody
        # @param filterBody - script which should return a logical value
        #                     determining that the conditions for the wait are met
        # @param args - extra arguments to filter body
        #               (these will be passed first;
        #                followed by actual event name,
        #                followed by the rest of event parameters)
        proc TcfEventsAfter {triggerScript timeoutms Service events filterParams filterBody args} {

            set NS [uplevel 1 namespace current]
            set waitVar [Variables::Create]

            set HIT "OK"
            set TIMEOUT "Timeout"
            set hasTimeout [string length $timeoutms]

            if {$hasTimeout} {
                set afterid [after $timeoutms [list set $waitVar $TIMEOUT] ]
            }

            if {[llength $events]} {
                set eventBody [concat \
                    if {[apply} \{ [list $filterParams $filterBody $NS] \} {{*}$args]} \{ \
                    set $waitVar $HIT \
                    \} \n\
                ]

                set lambda [list args $eventBody ::]
            }

            try {
                foreach event $events {
                    ::tcf::${Service}::Events::${event}+ apply $lambda {*}$args
                }

                set error [catch {
                    uplevel 1 $triggerScript
                } result options]

                if {!$error} {
                    if {[set $waitVar] == ""} {
                        vwait $waitVar
                    }
                    set result [set $waitVar]
                }
                if {$hasTimeout} {
                    after cancel $afterid
                }
                unset $waitVar
            } finally {
                foreach event [lreverse $events] {
                    if [catch {
                        ::tcf::${Service}::Events::${event}- apply $lambda {*}$args
                    } warning] {
                        cli::Warning $warning
                    }
                }
            }

            if {$error} {
                dict with options {
                    incr -level
                }
                # cli::PutDebug [prtobj $options]
                return -options $options $result
            }

            if {$result != $HIT} {
                return -code error $result
            }

            return
        }

        proc contextSuspendedAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextSuspended containerSuspended
                } {
                    checkId event contextId pc state reason args
                } {
                    switch -exact $event {
                        contextSuspended {
                            expr {$checkId == $contextId}
                        }
                        containerSuspended {
                            lassign $args contextIdList
                            expr {$checkId in $contextIdList}
                        }
                        default {
                            expr 0
                        }
                    }
                } $id
        }

        proc contextResumedAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextResumed
                    containerResumed
                } {
                    checkId event args
                } {
                    switch -exact $event {
                        contextResumed {
                            lassign $args contextId
                            expr {$checkId == $contextId}
                        }
                        containerResumed {
                            lassign $args contextIdList
                            expr {$checkId in $contextIdList}
                        }
                        default {
                            expr 0
                        }
                    }
                } $id
        }

        proc contextRunningAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextResumed
                    contextStateChanged
                    containerResumed
                } {
                    checkId event args
                } {
                    switch -exact $event {
                        contextResumed {
                            lassign $args contextId
                            expr {$checkId == $contextId && [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                        contextStateChanged {
                            lassign $args contextId
                            expr {$checkId == $contextId && [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                        containerResumed {
                            lassign $args contextIdList
                            expr {$checkId in $contextIdList && [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                    }
                } $id
        }

        proc contextNotRunningAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextSuspended
                    containerSuspended
                    contextResumed
                    containerResumed
                    contextStateChanged
                    contextRemoved
                } {
                    checkId event args
                } {
                    switch -exact $event {
                        contextSuspended {
                            lassign $args contextId pc reason data
                            expr {$checkId == $contextId}
                        }
                        containerSuspended {
                            lassign $args contextId pc reason data contextIdList
                            expr { $checkId == $contextId || $checkId in $contextIdList }
                        }
                        contextResumed {
                            lassign $args contextId
                            expr {$checkId == $contextId && [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                        containerResumed {
                            lassign $args contextIdList
                            expr {$checkId in $contextIdList && ! [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                        contextStateChanged {
                            lassign $args contextId
                            expr {$checkId == $contextId && ! [
                                ::tcf::isContextRunning $checkId
                            ]}
                        }
                        contextRemoved {
                            lassign $args contextId
                            expr {$checkId == $contextId}
                        }
                    }
                } $id
        }

        proc contextRunningOrSuspendedAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextResumed
                    containerResumed
                    contextSuspended
                    containerSuspended
                    contextStateChanged
                } {checkId event args} {
                    switch -exact $event {
                        contextResumed -
                        contextSuspended {
                            lassign $args contextId
                            expr { $checkId == $contextId }
                        }
                        contextStateChanged {
                            lassign $args contextId
                            expr { $checkId == $contextId && [
                                ::tcf::isContextRunning $contextId
                            ]}
                        }
                        containerResumed {
                            lassign $args contextIdList
                            expr { $checkId in $contextIdList }
                        }
                        containerSuspended {
                            lassign $args contextId pc reason data contextIdList
                            expr { $checkId in $contextIdList }
                        }
                    }
                } $id
        }

        proc contextSuspendedOrAnyBreakpointAfter {id timeoutms script args} {
            TcfEventsAfter [list cli::UpScript 1 $script {*}$args] $timeoutms \
                RunControl {
                    contextSuspended
                    containerSuspended
                } {
                    checkId event contextId pc reason data args
                } {
                    switch -exact $event {
                        contextSuspended {
                            expr { $checkId == $contextId || $reason == "Breakpoint"}
                        }
                        containerSuspended {
                            lassign $args contextIdList
                            expr {  $checkId in $contextIdList || $reason == "Breakpoint"}
                        }
                    }
                } $id
        }

        proc contextResumed {id {timeoutms {}}} {
            contextResumedAfter $id $timeoutms {
                if {![tcf::isContextSuspended $id]} {
                    return
                }
            }
        }

        proc contextSuspended {id {timeoutms {}}} {
            contextSuspendedAfter $id $timeoutms {
                if {[tcf::isContextSuspended $id]} {
                    return
                }
            }
        }

        proc contextRunning {id {timeoutms {}}} {
            contextRunningAfter $id $timeoutms {
                if {[tcf::isContextRunning $id]} {
                    return
                }
            }
        }

        proc contextNotRunning {id {timeoutms {}}} {
            contextNotRunningAfter $id $timeoutms {
                if {![tcf::isContextRunning $id]} {
                    return
                }
            }
        }

        proc contextRunningOrSuspended {id {timeoutms {}}} {
            contextRunningOrSuspendedAfter $id $timeoutms {
                if {[tcf::isContextRunningOrSuspended $id]} {
                    return
                }
            }
        }

        proc contextSuspendedOrAnyBreakpoint {id {timeoutms {}}} {
            contextSuspendedOrAnyBreakpointAfter $id $timeoutms {
                set runState [::tcf::RunControl::getState $id]
                if {[::tcf::getRunStateName $runState] == {SUSPENDED}} {
                    return
                }
                if {[tcf::isAnyBreakpointHit [cli::Target::getRootContext]]} {
                    return
                }
            }
        }

    } ; # namespace eval cli::Wait

    proc wait {args} {
        cli::NoErrorStack {
            cli::Wait::ArgumentParser parseCallerArgs 1 $args
            cli::Wait::ArgumentParser detectWaitType [llength $args]
            cli::Wait::ArgumentParser setupTriggerScript

            if {[string match TARGET* $waitType]} {
                set id [cli::Target::getContextIdFromTargetVar target]
            }


            cli::TrapInterrupt {
                switch -exact $waitType {
                    MILLISECONDS {
                        cli::Wait::ms $ms
                    }
                    TCF_EVENTS_AFTER {
                        cli::Wait::TcfEventsAfter \
                            $triggerScript \
                            $timeout \
                            $TcfService $TcfEvent \
                            $filterParams \
                            $filterBody {*}$filterArgs
                    }
                    TARGET_SUSPENDED_AFTER {
                        cli::Wait::contextSuspendedAfter $id $timeout $triggerScript
                    }
                    TARGET_SUSPENDED_OR_ANY_BP {
                        cli::Wait::contextSuspendedOrAnyBreakpoint $id $timeout
                    }
                    TARGET_SUSPENDED_OR_ANY_BP_AFTER {
                        cli::Wait::contextSuspendedOrAnyBreakpointAfter $id $timeout $triggerScript
                    }
                    TARGET_RUNNING_OR_SUSPENDED {
                        cli::Wait::contextRunningOrSuspended $id $timeout
                    }
                    TARGET_RUNNING_OR_SUSPENDED_AFTER {
                        cli::Wait::contextRunningOrSuspendedAfter $id $timeout $triggerScript
                    }
                    TARGET_RESUMED_AFTER {
                        cli::Wait::contextResumedAfter $id $timeout $triggerScript
                    }
                    TARGET_RUNNING_AFTER {
                        cli::Wait::contextRunningAfter $id $timeout $triggerScript
                    }
                    TARGET_NOT_RUNNING_AFTER {
                        cli::Wait::contextNotRunningAfter $id $timeout $triggerScript
                    }
                    TARGET_RESUMED {
                        cli::Wait::contextResumed $id $timeout
                    }
                    TARGET_SUSPENDED {
                        cli::Wait::contextSuspended $id $timeout
                    }
                    TARGET_RUNNING {
                        cli::Wait::contextRunning $id $timeout
                    }
                    TARGET_NOT_RUNNING {
                        cli::Wait::contextNotRunning $id $timeout
                    }
                    default {
                        error "WAIT_$waitType not implemented"
                    }
                }
            }
        }
    }

    shell::autocomplete::addCommand wait cli::Wait::ArgumentParser autocomplete
    cli::Help::Manager add wait -parser cli::Wait::ArgumentParser

    namespace export wait

} ; # namespace eval mdbg
