# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : mutexCommon.tcl
# Description: Common classes, procedures for platforms
# Created on : Aug 26, 2016
# Author     : Daraban Luminita (luminita.daraban@movidius.com)
# ***************************************************************************

namespace eval mdbg::mutex {

    # struct leonMutex

    oo::class create LeonMutex {
        variable Core
        variable MutexReg
        variable clearAllPending
        variable clearOwnPending
        variable mutexRequestNoRetry
        variable mutexRelease
        variable mutexRequestAutoRetry
        variable noMutexAccess
        variable MX_ACTION_start_bit

        constructor {core mutexRegName} {
            set Core $core
            set clearAllPending         0x5
            set clearOwnPending         0x4
            set mutexRequestNoRetry     0x3
            set mutexRelease            0x2
            set mutexRequestAutoRetry   0x1
            set noMutexAccess           0x0
            set MutexReg $mutexRegName
            set MX_ACTION_start_bit     8

        }

        method lock {mutexNumber} {
            if {![my isUsed $mutexNumber]} {
                set lockValue [expr {$mutexNumber | ($mutexRequestAutoRetry << $MX_ACTION_start_bit) }]
                mdbg::setReg32 $MutexReg $lockValue
            } else {
                return -code error "mutex $mutexNumber is already in use"
              }
        }

        method unlock {mutexNumber} {
            if {[my isUsed $mutexNumber]} {
            set unlockValue [expr {$mutexNumber | ($mutexRelease << $MX_ACTION_start_bit) }]
                mdbg::setReg32 $MutexReg $unlockValue
            } else {
                return -code error "mutex $mutexNumber is already free"
              }
        }

        method isUsed {mutexNumber} {
            #return 0x0 - mutex not in use; 0x1 - mutex in use
            set MTX_Status_value [mdbg::getReg32 CMX_MTX_STATUS_ADR]
            expr {($MTX_Status_value >> $mutexNumber) & 0x1}
        }

    }; # oo::class LeonMutex

    oo::class create ShaveMutex {
        variable Core
        variable OwnMutexId
        variable mutexRelease
        variable mutexRequest
        variable noMutexAccess

        constructor {core ownMutexId} {
            set Core $core
            set OwnMutexId $ownMutexId
            set mutexRelease   0x200
            set mutexRequest   0x100
            set noMutexAccess  0x0
        }

        method lock {mutexNumber} {
            my CheckState
            if {![my isUsed $mutexNumber]} {
                set lockValue [expr {$mutexNumber | $mutexRequest}]
                my SetMutexCtrl $lockValue
            } else {
                return -code error "mutex $mutexNumber is already in use"
            }
        }

        method unlock {mutexNumber} {
            my CheckState
            if {[my isUsed $mutexNumber id]} {
                if {$id != $OwnMutexId} {
                    return -code error "mutex $mutexNumber was locked by someone else"
                }
                set unlockValue [expr {$mutexNumber | $mutexRelease}]
                my SetMutexCtrl $unlockValue
            } else {
                return -code error "mutex $mutexNumber is already free"
            }
        }

        method CheckState {} {
            set state [mdbg::state -quiet -target $Core]
            dict with state {
                if {!$SUSPENDED && !([info exists RUNNING] && $RUNNING)} {
                    return -level 2 -code error "$Core: $STATE_NAME"
                }
            }
        }

        method SetMutexCtrl {value} {
            ::mdbg::mset -target $Core -jtag -reg MUTEX_CTRL $value
        }

        method isUsed {mutexNumber {usedByVar ""}} {
            set value [mdbg::getReg32 CMX_MTX_${mutexNumber}_ADR]

            if {$usedByVar != ""} {
                upvar $usedByVar usedBy
                set usedBy [expr {$value & 0xf}]
            }
            #return 0x0 - mutex not in use; 0x1 - mutex in use
            expr {($value >> 8) & 1}
        }
    } ; # oo::class ShaveMutex

    proc status {mutexNumber} {
        set value [mdbg::getReg32 CMX_MTX_${mutexNumber}_ADR]
        #[8]:  isUsed        0x1      - mutex is used
        #                    0x0      - mutex free
        #[3:0] MX_ID_Value: [0x0-0xb] - mutex granted to shave
        #                    0xc      - mutex granted to LOS
        #                    0xd      - mutex granted to LRT
        set isUsed [expr {($value >> 8) & 0x1}]
        if {$isUsed} {
            set MX_ID_Value [expr {$value & 0xf}]
            if {$MX_ID_Value >= 0} {
                if {$MX_ID_Value <= 0xb} {
                    return [Item $mutexNumber S$MX_ID_Value]
                }
            }
            if {$MX_ID_Value == 0xc} {
                return [Item $mutexNumber LOS]
            }
            if {$MX_ID_Value == 0xd} {
                return [Item $mutexNumber LRT]
            }
        } else {
            return [Item $mutexNumber]
          }
    }

    proc Item {index {owner ""}} {
        set used [expr {$owner != ""}]
        set result [dict create INDEX $index USED $used]
        if {$used} {
            dict set result OWNER $owner
        }
        return $result
    }

    proc statusList {{rangeVar ""}} {
        set lowIndex 0
        set highIndex 31
        upvar 1 $rangeVar range
        if {[info exists range]} {
            regexp {^([0-9]+)..([0-9]+)$} $range -> lowIndex highIndex
        }
        set itemList [list]
        for {set index $lowIndex} {$index <= $highIndex} {incr index} {
            lappend itemList [status $index]
        }
        return $itemList
    }

    proc getValidMutexIndices {pattern} {
        set result [list]
        for {set i 0} {$i < 32} {incr i} {
            if {[string match $pattern $i]} {
                lappend result $i
            }
        }
        return $result
    }

    proc getValidTargets {pattern} {
        variable CORE_TO_MUTEX_MAP
        dict keys $CORE_TO_MUTEX_MAP $pattern
    }

    proc INIT {} {
        variable CORE_TO_MUTEX_MAP
        set CORE_TO_MUTEX_MAP [dict create]
        foreach {
            leon    reg
        } {
            LOS     CMX_MTX_LOS_GET_ADR
            LRT     CMX_MTX_LRT_GET_ADR
        } {
            dict set CORE_TO_MUTEX_MAP $leon [LeonMutex new $leon $reg]
        }

        set numShaves [mdbg::myriad2::getNumberOfShaves [mdbg::MV_SOC_REV]]

        for {set index 0} {$index < $numShaves} {incr index} {
            set shave S$index
            dict set CORE_TO_MUTEX_MAP $shave [ShaveMutex new $shave $index]
        }
    }

    INIT

} ; #namespace eval mdbg::mutex
