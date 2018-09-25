# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : pipe/Queue.tcl
# Description: Unidirectional lockless queue for Debug Pipes (TclOO class)
# Created on : Aug 13, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::pipe {

    set CANARY_START_VALUE 0x11223344
    set CANARY_END_VALUE 0xAABBCCDD
    set QUEUE_MIN_ELEMENTS 8
    set QUEUE_MIN_SIZE [expr {$QUEUE_MIN_ELEMENTS + 1}]
    set QUEUE_HEADER_SIZE [expr {5 * 4}]
    set MIN_READ_SIZE [expr {( $QUEUE_HEADER_SIZE + $QUEUE_MIN_SIZE + 3 ) & ~3}]

    oo::class create Queue {
        #
        # Fields
        #
        variable Mem
        variable MemLockEnabled
        variable BaseAddress
        variable Buffer
        variable Header

        #
        # Public
        #
        constructor {memory baseAddr} {
            set Mem $memory
            set BaseAddress $baseAddr
            set MemLockEnabled false

            set Buffer {}
            set Header {
                canaryStart 0
                in 0
                out 0
                queueSize 0
                canaryEnd 0
            }
        }

        method getBaseAddress {} {
            return $BaseAddress
        }

        method isValid {} {
            expr {
                    [string length $Buffer] >= $::mdbg::pipe::QUEUE_HEADER_SIZE
                &&  [my IsHeaderValid]
            }
        }

        method getQueueSize {} {
            if {![string length $Buffer] < $::mdbg::pipe::QUEUE_HEADER_SIZE} {
                my ReadHeader
            }
            if {![my IsHeaderValid]} {
                return -1
            }
            dict get $Header queueSize
        }

        method write {data} {
            if {![string length $data]} {
                return 0
            }

            my MemLock {
                my ReadHeader

                if {![my IsHeaderValid]} {
                    return -1
                }
                set count [my WriteData $data]

                if {$count > 0} {
                    my WriteHeaderIn
                }
            }

            return $count
        }

        method read {size} {
            if {$size <= 0 } {
                return ""
            }
            my MemLock {
                my ReadHeader

                if {![my IsHeaderValid]} {
                    return ""
                }

                set data [my ReadData $size]

                if {[string length $data]} {
                    my WriteHeaderOut
                }
            }

            return $data
        }

        #
        # Private
        #
        method WriteData {data} {

            set totalSize [string length $data]
            set sizeToWrite $totalSize
            set count 0

            set in [my In]
            #upvar 0 ::mdbg::Channels::debug DBGCH
            #puts $DBGCH "in = $in"
            set out [my Out]
            #puts $DBGCH "out = $out"
            set qsize [my QueueSize]

            set inToOut [expr {$out - $in - 1}]
            #puts $DBGCH "inToOut = $inToOut"
            set inToEnd [expr {$qsize - ( !$out ) - $in }]
            #puts $DBGCH "inToEnd = $inToEnd"

            if {$inToOut >= 0} {
                 if {$inToOut < $sizeToWrite} {
                     set sizeToWrite $inToOut
                 }
            } else {
                if {$inToEnd < $sizeToWrite} {
                    set sizeToWrite $inToEnd
                }
            }

            if {$sizeToWrite == 0} {
                return 0
            }

            my QueueWrite $in $data 0 $sizeToWrite
            set nextIn [my NextIndex $in $sizeToWrite]
            set count $sizeToWrite
            set sizeToWrite [expr {$totalSize - $count}]
            if {$sizeToWrite > 0 && $inToOut < 0 && $nextIn == 0 } {
                set inToOut [expr {$out - 1}]
                # puts stderr "    inToOut = $inToOut"

                if {$inToOut < $sizeToWrite} {
                    set sizeToWrite $inToOut
                }
                if {$sizeToWrite} {
                    my QueueWrite 0 $data $count $sizeToWrite
                    set nextIn $sizeToWrite
                    incr count $sizeToWrite
                }
            }

            dict set Header in $nextIn
            return $count
        }

        method ReadData {size} {
            set in [my In]
            set out [my Out]
            set qsize [my QueueSize]

            set outToIn [expr {$in - $out}]
            set outToEnd [expr {$qsize - $out}]

            if {$outToIn == 0} {
                return ""
            }

            set sizeToRead [expr {
                $outToIn >= 0 ? $outToIn : $outToEnd
            }]

            if {$sizeToRead > $size} {
                set sizeToRead $size
            }

            set data [my QueueRead $out $sizeToRead]
            set nextOut [my NextIndex $out $sizeToRead]

            if {$size > $sizeToRead && $nextOut != $in} {
                set sizeToRead [expr {min( $size - $sizeToRead, $in)}]
                if {$sizeToRead} {
                    set out $nextOut
                    append data [my QueueRead $out $sizeToRead]
                    set nextOut [my NextIndex $out $sizeToRead]
                }
            }

            dict set Header out $nextOut
            return $data
        }

        method MemLock {script} {
            catch {
                if {$MemLockEnabled} {
                    $Mem lock {
                        uplevel 1 $script
                    }
                } else {
                    uplevel 1 $script
                }
            } result options
            dict with options {
                incr -level
            }
            return -options $options $result
        }

        method MemWrite {offset data} {
            $Mem writeBlock [expr {$BaseAddress + $offset}] $data
        }

        method MemRead {offset size} {
            $Mem readBlock [expr {$BaseAddress + $offset}] $size
        }

        method QueueWrite {in data offset size} {
            set slice [string range $data $offset [expr {$offset + $size - 1}]]
            tailcall my MemWrite [expr {$::mdbg::pipe::QUEUE_HEADER_SIZE + $in}] $slice
        }

        method QueueRead {out size} {
            tailcall my MemRead [expr {$::mdbg::pipe::QUEUE_HEADER_SIZE + $out}] $size
        }

        method WriteHeaderIn { } {
            tailcall my MemWrite 4 [
                binary format i [my In]
            ]
        }

        method WriteHeaderOut { } {
            tailcall my MemWrite 8 [
                binary format i [my Out]
            ]
        }

        method In { } {
            dict get $Header in
        }
        method Out {} {
            dict get $Header out
        }
        method QueueSize {} {
            dict get $Header queueSize
        }

        method IsHeaderValid {} {
            dict with Header {
                expr {
                    $canaryStart == $::mdbg::pipe::CANARY_START_VALUE &&
                    0 <= $in && $in < $queueSize &&
                    0 <= $out && $out < $queueSize &&
                    $canaryEnd == $::mdbg::pipe::CANARY_END_VALUE
                }
            }
        }

        method NextIndex {index {increment 1} } {
            expr { ( $index + $increment ) % [my QueueSize] }
        }

        method ReadHeader {} {
            set Buffer [my MemRead 0 $mdbg::pipe::QUEUE_HEADER_SIZE]

            dict with Header {
                binary scan $Buffer "iu i i i iu" \
                    canaryStart in out queueSize canaryEnd
            }
        }

    } ; # oo::class create Queue

} ; # namespace eval mdbg::pipe
