# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : tcf/tcf.tcl
# Description: TCF-related utility functions and wrappers
#              Also contains context state change display
# Created on : Aug 13, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf {

    proc getField {dict field {default {}}} {
        if [dict exists $dict $field] {
            dict get $dict $field
        } else {
            return $default
        }
    }

    proc getFields {dict fields {defaults {}}} {
        set result [list]
        foreach field $fields default $defaults {
            lappend result [getField $dict $field $default]
        }
        return $result
    }

    proc getName {contextObj} {
        getField $contextObj Name
    }

    proc getID {contextObj} {
        getField $contextObj ID
    }

    proc getContextIdByName {name} {
        ContextQuery::query Name=[json::String $name]
    }

    proc getContext {service id} {
        ::tcf::${service}::getContext $id
    }

    proc getContextByName {service name} {
        getContext $service [getContextIdByName $name]
    }

    proc getContextChildren {service id} {
        ::tcf::${service}::getChildren $id
    }

    proc getContextName {args} {
        switch -exact [llength $args] {
            1 {
                set service RunControl
                set id [lindex $args 0]
            }
            2 {
                lassign $args service id
            }
            default {
                return -code error "wrong # args: should be \"[lindex [info level 0] 0] ?service? id\""
            }
        }
        getName [getContext $service $id]
    }

    proc getContextNames {idList {service RunControl}} {
        set result [list]
        foreach id $idList {
            lappend result [getContextName $id $service]
        }
        return $result
    }

    proc prettyContext {contextObj} {
        set result [list]
        dict for {key value} $contextObj {
            lappend result [format "%s: %s" $key $value]
        }
        join $result \n
    }

    proc prettyFieldList {fieldList {format {}} } {
        if [string length $format] {
            set rows [list]
            foreach row $fieldList {
                lappend rows [format $format {*}$row]
            }
            set fieldList $rows
        }
        join $fieldList \n
    }

    proc getContextField {service context field {default {}}} {
        fields::getNamedField ${service}::CONTEXT $context $field $default
    }

    proc getContextFieldList {service context fields {defaults {}}} {
        fields::getNamedFieldList ${service}::CONTEXT $context $fields $defaults
    }

    proc getContextFieldDict {service context} {
        fields::getNamedFieldDict ${service}::CONTEXT $context
    }

    #proc getContextListFields {service contextIdList fieldList {defaults {}}} {
    #    fields::getNamedFieldTable \
    #        ${service}::CONTEXT \
    #        [getContextList $contextIdList $service] \
    #        $fieldList \
    #        $defaults
    #}

    proc queryContext {{query {}}} {
        ::tcf::ContextQuery::query $query
    }

    proc hasContextState {contextId} {
        set context [tcf::RunControl::getContext $contextId]
        fields::getNamedField RunControl::CONTEXT $context HasState false
    }

    proc getContextPC {contextId} {
        if {[isFrameContextId $contextId]} {
            set frameContext [lindex [tcf::StackTrace::getContext $contextId] 0]
            return [getContextField StackTrace $frameContext INSTRUCTION_PTR]
        }

        set state [::tcf::RunControl::getState $contextId]
        switch -exact [getRunStateName $state detail] {
            SUSPENDED {
                fields::getNamedField RunControl::STATE $state PC
            }
            RUNNING {
                format {0x%.8X} [::tcf::MyriadCore::getPC $contextId]
            }
            default {
                return -code error "invalid core state: $detail"
            }
        }
    }

    proc getRunStateName {runState {detailVar {}}} {
        set isSuspended [fields::getNamedField RunControl::STATE $runState SUSPENDED]
        if {[string is true -strict $isSuspended]} {
            if {$detailVar != {}} {
                upvar 1 $detailVar reason
                set reason [fields::getNamedField RunControl::STATE $runState REASON]
            }
            return SUSPENDED
        }
        if {$detailVar != {}} {
            upvar 1 $detailVar stateName
        }
        set stateData  [fields::getNamedField RunControl::STATE $runState DATA]
        set stateName [fields::getNamedField RunControl::STATE::DATA $stateData STATE_NAME]
        if {$stateName == {}} {
            return RUNNING
        }
        return INACTIVE
    }

    proc getSuspendReason {runState} {
        if {[getRunStateName $runState] != {SUSPENDED}} {
            return
        }
        fields::getNamedField RunControl::STATE $runState REASON
    }

    proc isContextRunning {contextId} {
        expr {[getRunStateName [::tcf::RunControl::getState $contextId]] == {RUNNING}}
    }

    proc isContextSuspended {contextId} {
        expr {[getRunStateName [::tcf::RunControl::getState $contextId]] == {SUSPENDED}}
    }

    proc isContextInBreakpoint {contextId} {
        set state [::tcf::RunControl::getState $contextId]
        expr  {
            [getRunStateName $state]  == {SUSPENDED} &&
            [getSuspendReason $state] == {BREAKPOINT}
        }
    }

    proc getContextBreakpointsHit {contextId} {
        set state [::tcf::RunControl::getState $contextId]
        if {   [getRunStateName $state]  != {SUSPENDED}
            || [getSuspendReason $state] != {BREAKPOINT}
        } {
            return [list]
        };
        set data [fields::getNamedField RunControl::STATE $state DATA]
        fields::getNamedField RunControl::STATE::DATA $data BPS [list]
    }

    proc isContextRunningOrSuspended {contextId} {
        expr {[getRunStateName [::tcf::RunControl::getState $contextId]] in {RUNNING SUSPENDED}}
    }

    proc isAnyBreakpointHit {rootContextId} {
        set contextIdList [queryContext [format {ID="%s"/**} $rootContextId]]
        foreach id $contextIdList {
            if {![catch {tcf::RunControl::getState $id} runState]} {
                if {[getSuspendReason $runState] == {BREAKPOINT}} {
                    return 1
                }
            }
        }
        return 0
    }

    namespace export *Context*
    namespace export *pretty*

    # events debug support
    if {[info exists ::mdbg::DEBUG_RAW_EVENTS] &&
        [string is true $::mdbg::DEBUG_RAW_EVENTS]} {

        proc DEBUG_RAW_EVENTS {service name rawData} {
            if {![string equal $service Locator]} {
                binary scan $rawData a* strData
                set strData [string map {\000  " * "} $strData]
                puts $::mdbg::Channels::event [concat {TCF Event:} $service * $name * $strData]
            }
        }

        tcf::protocol::registerRawEventHandler * * [namespace current]::DEBUG_RAW_EVENTS
    }

} ; # namespace eval tcf

