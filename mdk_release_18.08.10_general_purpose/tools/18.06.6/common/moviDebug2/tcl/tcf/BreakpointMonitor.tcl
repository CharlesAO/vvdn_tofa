# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : BreakpointMonitor.tcl
# Description:
# Created on : Jan 30, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval tcf {

    oo::class create BreakpointMonitor {

        # {Key(TYPE CONTEXT ADDRESS) -> {{BPIDs} Properties(ERROR CONDITION_ERROR) Instance}}
        variable InstanceDict
        variable ShutDown

        constructor { } {
            set InstanceDict [dict create]
            set ShutDown 0
            my Events +
        }

        destructor {
            my shutdown
        }

        method shutdown {} {
            if {!$ShutDown} {
                set ShutDown 1
                my Events -
            }
        }

        #
        #  Virtual/Abstract
        #

        method OnInstanceError {instance} {
        }

        method OnNoInstancePlantedError {bpid instance} {
        }

        method OnInstanceCondError {instance} {
        }

        method OnInstanceHit {instance} {
        }

        method OnInstancePlanted {instance} {
        }

        method OnInstanceUnplanted {instance} {
        }

        #
        #  Protected
        #
        method Debug {message args} {
            puts $::mdbg::Channels::debug [join [list $message {*}$args] \n]
        }

        method DebugInstanceStr {instance} {
            my InstanceFieldList $instance {TYPE CONTEXT ADDRESS ERROR CONDITION_ERROR HIT_COUNT}
        }

        method OnInstanceAdded {instance} {
            lassign [my InstanceFieldList $instance {
                ERROR TYPE ADDRESS
            }]  error type address
            if {$error != {}} {
                my Catch {
                    my OnInstanceError $instance
                }
            } elseif {$address != {}} {
                my Catch {
                    my OnInstancePlanted $instance
                }
            }
        }

        method OnInstanceRemoved {instance} {
            lassign [my InstanceFieldList $instance {
                ERROR TYPE ADDRESS
            }]  error type address
            if {$error == {} && $type != {} && $address != {}} {
                my Catch {
                    my OnInstanceUnplanted $instance
                }
            }
        }

        method OnInstanceChanged {newInstance oldInstance} {
            lassign [my InstanceFieldList $oldInstance {
               ERROR    HIT_COUNT   CONDITION_ERROR  TYPE       ADDRESS
            }] oldError oldHitCount oldCondError     oldType    oldAddress
            lassign [my InstanceFieldList $newInstance {
               ERROR    HIT_COUNT   CONDITION_ERROR  TYPE       ADDRESS
            }] newError newHitCount newCondError     newType    newAddress

            if {$newHitCount != $oldHitCount} {
                if {$newType != {} && $newAddress != {}} {
                    my Catch {
                        my OnInstanceHit $newInstance
                    }
                }
            }

            if {$newType != $oldType || $newAddress != $oldAddress} {
                if {$oldType != {} && $oldAddress != {}} {
                    my Catch {
                        my OnInstanceUnplanted $oldInstance
                    }
                }
                if {$newType != {} && $newAddress != {}} {
                    my Catch {
                        my OnInstancePlanted $newInstance
                    }
                }
            }

            if {$newError != $oldError} {
                if {$newError != {}} {
                    my Catch {
                        my OnInstanceError $newInstance
                    }
                }
            }

            if {$newCondError != $oldCondError} {
                if {$newCondError != {}} {
                    my Catch {
                        my OnInstanceCondError $newInstance
                    }
                }
            }
        }

        method InstanceField { instance field {default {}} } {
            ::tcf::fields::getNamedField Breakpoints::STATUS::INSTANCE $instance $field $default
        }

        method InstanceFieldList { instance fieldList {defaultList {}} } {
            ::tcf::fields::getNamedFieldList Breakpoints::STATUS::INSTANCE $instance $fieldList $defaultList
        }

        method InstanceKey {instance} {
            my InstanceFieldList $instance {TYPE CONTEXT ADDRESS}
        }

        method InstanceProperties {instance} {
            my InstanceFieldList $instance {ERROR HIT_COUNT CONDITION_ERROR}
        }

        #
        #  Private
        #

        method GetBpInstanceDict {bpid} {
            variable InstanceDict
            dict filter $InstanceDict script {key value} {
                set bpIdList [lindex $value 0]
                expr {$bpid in $bpIdList}
            }
        }

        method ListDiff {A B} {
            foreach a $A b $B {
                if {$a != $b} {
                    return 1
                }
            }
            return 0
        }

        method AddRowBpId {rowVar bpid} {
            upvar 1 $rowVar row
            set bpIdList [lindex $row 0]
            if {$bpid ni $bpIdList} {
                lappend bpIdList $bpid
                lset row 0 $bpIdList
                return 1
            }
            return 0
        }

        method RemoveRowBpId {rowVar bpid instanceVar} {
            upvar 1 $rowVar row
            set bpIdList [lindex $row 0]
            set index [lsearch -exact $bpIdList $bpid]
            if {$index < 0} {
                return 0
            }
            if {[llength $bpIdList] == 1} {
                upvar $instanceVar instance
                set instance [lindex $row 2]
                unset row
                return 1
            }
            lset row 0 [lreplace $bpIdList $index $index]
            return 0
        }

        method UpdateRowProps {rowVar props instance prevInstanceVar} {
            upvar 1 $rowVar row
            upvar 1 $prevInstanceVar prevInstance

            set rowProps [lindex $row 1]
            if {[my ListDiff $props $rowProps]} {
                lset row 1 $props
                set prevInstance [lindex $row 2]
                lset row 2 $instance
                return 1
            }
            return 0
        }

        method Catch {script} {
            if {[catch {
                uplevel 1 $script
            } error options]} {
                my ErrorMsg $error $options
            }
        }

        method ErrorMsg {error options} {
            if {[dict exists $options -errorinfo]} {
                puts stderr [dict get $options -errorinfo]
            } else {
                puts stderr $error
            }
        }

        method UpdateInstance {bpid key instance} {

            set props [my InstanceProperties $instance]
            set changed 0

            dict update InstanceDict $key row {
                if {[info exists row]} {
                    my AddRowBpId row $bpid
                    if {[my UpdateRowProps row $props $instance prevInstance]} {
                        set changed 1
                        my Catch {
                            my OnInstanceChanged $instance $prevInstance
                        }
                    }
                } else {
                    set changed 1
                    set row [list $bpid $props $instance]
                    my Catch {
                        my OnInstanceAdded $instance
                    }
                }
            }

            return $changed
        }

        method RemoveInstance {bpid key} {
            dict update InstanceDict $key row {
                if {![info exists row]} {
                    return
                }
                if {[my RemoveRowBpId row $bpid instance]} {
                    my Catch {
                        my OnInstanceRemoved $instance
                    }
                }
            }
        }

        method UpdateBpInstances {bpid instances} {

            set missing [my GetBpInstanceDict $bpid]
            set changed 0
            foreach instance $instances {
                set key [my InstanceKey $instance]
                dict unset missing $key
                if {[my UpdateInstance $bpid $key $instance]} {
                    set changed 1
                }
            }
            dict for {key value} $missing {
                my RemoveInstance $bpid $key
                set changed 1
            }

            if {$changed} {
                my CheckNoInstancePlanted $bpid $instances
            }

            return
        }

        method CheckNoInstancePlanted {bpid instances} {

            foreach instance $instances {
                lassign [my InstanceFieldList $instance {
                   ERROR    TYPE       ADDRESS
                }] error    type       address

                if {$type != "" && $address != "" && $error == "" } {
                    return
                }
            }

            foreach instance $instances {
                my OnNoInstancePlantedError $bpid $instance
            }
        }

        method RemoveBpInstances {bpid} {
            set removed [my GetBpInstanceDict $bpid]
            dict for {key value} $removed {
                my RemoveInstance $bpid $key
            }
        }

        method IsShutDown {} {
            return $ShutDown
        }

        method OnAdded {event bpList} {
            if {[my IsShutDown]} {
                return
            }
        }
        method OnChanged {event bpList} {
            if {[my IsShutDown]} {
                return
            }
        }

        method OnRemoved {event bpIdList} {
            if {[my IsShutDown]} {
                return
            }
            foreach bpid $bpIdList {
                my RemoveBpInstances $bpid
            }
        }

        method OnStatus {event bpid status} {
            if {[my IsShutDown]} {
                return
            }
            set instances [::tcf::fields::getNamedField Breakpoints::STATUS $status INSTANCES]
            my UpdateBpInstances $bpid $instances
        }

        method Events {+/-} {
            tcf::Breakpoints::Events::contextAdded${+/-}    my OnAdded
            tcf::Breakpoints::Events::contextChanged${+/-}  my OnChanged
            tcf::Breakpoints::Events::status${+/-}          my OnStatus
            tcf::Breakpoints::Events::contextRemoved${+/-}  my OnRemoved
        }

    } ; # oo::class BreakpointMonitor

} ; # namespace eval tcf
