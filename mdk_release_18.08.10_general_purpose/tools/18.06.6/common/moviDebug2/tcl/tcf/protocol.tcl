# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : protocol.tcl
# Description: Generic TCF protocol command/event Tcl interface
# Created on : May 29, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf::protocol {

    proc DebugCommand { service name args } {
        join [splitStringZeroList [sendCommand $service $name [joinStringZeroList $args]]] " * "
    }

    proc JsonListResult {service name arglist {errorIndex 0}} {
        set rawArgs [joinStringZeroList $arglist]
        set rawResult [sendCommand $service $name $rawArgs]
        set rawResultList [splitStringZeroList $rawResult]
        set rawError [lindex $rawResultList $errorIndex]
        if [string length $rawError] {
            RaiseTcfError [parseJsonValue $rawError] 3
        }
        parseJsonValueList [lreplace \
            $rawResultList $errorIndex $errorIndex]
    }

    proc RaiseTcfError {dict {level 1}} {
        set errFormat $dict
        set errCode ""
        if [dict exists $dict Format] {
            set errFormat [dict get $dict Format]
            if [dict exists $dict Code] {
                set errCode [dict get $dict Code]
            }
        }
        return -code error -level $level -errorcode $errCode $errFormat
    }

    proc jsonCommand {service name args} {
        lindex [JsonListResult $service $name $args] 0
    }

    proc jsonListCommand {expectedLength errorIndex service name args} {
        set result [JsonListResult $service $name $args $errorIndex]
        set len [llength $result]
        if {$expectedLength != $len} {
            return -code error -level 2 "TCF Result error: $len does not match expected length of $expectedLength"
        }
        return $result
    }

    proc jsonEventData {rawData} {
        parseJsonValue [lindex [splitStringZeroList $rawData] 0]
    }

    proc jsonEventDataList {rawData} {
        parseJsonValueList [splitStringZeroList $rawData]
    }

    proc JsonEventHandler {script service name rawData} {
        set data [jsonEventData $rawData]
        lappend script $name $data
        namespace inscope :: $script
    }

    proc JsonEventHandler2 {script service name rawData} {
        set dataList [jsonEventDataList $rawData]
        lappend script $name {*}$dataList
        namespace inscope :: $script
    }

    proc registerEventHandler {service name command args} {
        registerRawEventHandler $service $name ::tcf::protocol::JsonEventHandler [list $command {*}$args]
    }

    proc unregisterEventHandler {service name command args} {
        unregisterRawEventHandler $service $name ::tcf::protocol::JsonEventHandler [list $command {*}$args]
    }

    proc registerEventHandler2 {service name command args} {
        registerRawEventHandler $service $name ::tcf::protocol::JsonEventHandler2 [list $command {*}$args]
    }

    proc unregisterEventHandler2 {service name command args} {
        unregisterRawEventHandler $service $name ::tcf::protocol::JsonEventHandler2 [list $command {*}$args]
    }

    proc registerSyncEventHandler {service name command args} {
        registerRawSyncEventHandler $service $name ::tcf::protocol::JsonEventHandler [list $command {*}$args]
    }

    proc unregisterSyncEventHandler {service name command args} {
        unregisterRawSyncEventHandler $service $name ::tcf::protocol::JsonEventHandler [list $command {*}$args]
    }

    proc registerSyncEventHandler2 {service name command args} {
        registerRawSyncEventHandler $service $name ::tcf::protocol::JsonEventHandler2 [list $command {*}$args]
    }

    proc unregisterSyncEventHandler2 {service name command args} {
        unregisterRawSyncEventHandler $service $name ::tcf::protocol::JsonEventHandler2 [list $command {*}$args]
    }

    namespace eval Handler {

        proc UpProc {service name op function {level 1}} {
            incr level
            uplevel $level proc ${name}${op} {{command args}} \{ \
                [namespace parent]::$function $service $name \
                {namespace inscope [uplevel 1 namespace current] $command {*}$args} \}
        }

    } ; # namespace eval DeclareEventHandler

    proc JsonEvent {service name} {
        Handler::UpProc $service $name + registerEventHandler
        Handler::UpProc $service $name - unregisterEventHandler
    }

    proc JsonEvent2 {service name} {
        Handler::UpProc $service $name + registerEventHandler2
        Handler::UpProc $service $name - unregisterEventHandler2
    }

    proc JsonSyncEvent {service name} {
        Handler::UpProc $service $name + registerSyncEventHandler
        Handler::UpProc $service $name - unregisterSyncEventHandler
    }

    proc JsonSyncEvent2 {service name} {
        Handler::UpProc $service $name + registerSyncEventHandler2
        Handler::UpProc $service $name - unregisterSyncEventHandler2
    }

} ; # namespace eval tcf::protocol
