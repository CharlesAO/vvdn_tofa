# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : pipe/DebugPipe.tcl
# Description: Bidirectional Debug Pipe
# Created on : Aug 7, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::pipe {

    oo::class create DebugPipe {

        variable Memory
        variable ReadQueue
        variable WriteQueue
        variable ReadBuf
        variable WriteBuf
        variable LastWriteCount

        constructor {memory} {
            set Memory $memory
            set LastWriteCount 0
            set ReadBuf ""
            set WriteBuf ""
        }

        destructor {
            my CloseQueue ReadQueue
            my CloseQueue WriteQueue
        }

        method getReadAddr {} {
            if {[my HasQueue ReadQueue]} {
                tailcall $ReadQueue getBaseAddress
            }
            return 0
        }

        method setReadAddr {address} {
            my CloseQueue ReadQueue
            set ReadQueue [mdbg::pipe::Queue new $Memory $address]
            oo::objdefine $ReadQueue {
                unexport write
            }
        }

        method getWriteAddr {} {
            if {[my HasQueue WriteQueue]} {
                tailcall $WriteQueue getBaseAddress
            }
            return 0
        }

        method setWriteAddr {address} {
            my CloseQueue WriteQueue
            set WriteQueue [mdbg::pipe::Queue new $Memory $address]
            oo::objdefine $WriteQueue {
                unexport read
            }
        }

        method send {data} {
            if {[string length $data]} {
                set data [binary format a* $data]
                append WriteBuf $data
            }
            set LastWriteCount [my WriteFromBufferToQueue]
        }

        method peek {size} {
            set toRead [my GetUnread $size]
            if {$toRead > 0} {
                my ReadFromQueueInBuffer $toRead
            }
            my SplitBuf $ReadBuf result {}
            return $result
        }

        method drop {size} {
            set size [my LimitToReadBufLen $size]
            my SplitBuf $ReadBuf $size {} ReadBuf
            return $size
        }

        method recv {size} {
            set toRead [my GetUnread $size]
            if {$toRead > 0} {
                my ReadFromQueueInBuffer $toRead
            }
            set size [my LimitToReadBufLen $size]
            my SplitBuf $ReadBuf $size data ReadBuf
            return $data
        }

        method recvLine {size} {
            set index [my PeekEndLineIndex $size]
            if {$index >= 0} {
                my SplitBuf $ReadBuf [expr {$index + 1}] data ReadBuf
                return $data
            }
            return ""
        }

        method peekLine {size} {
            set index [my PeekEndLineIndex $size]
            if {$index >= 0} {
                my SplitBuf $ReadBuf [expr {$index + 1}] data {}
                return $data
            }
            return ""
        }

        method getReadQueueSize {} {
            if {[my HasQueue ReadQueue]} {
                tailcall $ReadQueue getQueueSize
            }
            return -1
        }

        #
        # Protected
        #
        method LastWriteLen {} {
            return $LastWriteCount
        }

        method UnwrittenLen {} {
            string length $WriteBuf
        }

        export LastWriteLen UnwrittenLen

        #
        #  Private
        #

        method HasQueue {name} {
            info exists $name
        }

        method CloseQueue {name} {
            if {[info exists $name]} {
                [set $name] destroy
                unset $name
            }
        }

        method GetUnread {size} {
            expr { max ( 0 ,
                $size - [string length $ReadBuf]
            )}
        }

        method LimitToReadBufLen {size} {
            expr { max ( 0 ,
                min ( $size , [string length $ReadBuf] )
            )}
        }

        method SplitBuf {buffer pos frontVar backVar} {
            if {[string length $frontVar]} {
                upvar 1 $frontVar front
                set front [string range $buffer 0 [expr { $pos - 1}]]
            }
            if {[string length $backVar]} {
                upvar 1 $backVar back
                set back [string range $buffer $pos end]
            }
        }

        # 4096 is the default buffer size for Tcl channels
        method ReadFromQueueInBuffer { {size 4096} } {

            if {[my HasQueue ReadQueue]} {
                set data [$ReadQueue read $size]
                if {[string length $data]} {
                    append ReadBuf $data
                    tailcall string length $data
                }
                if {[$ReadQueue isValid]} {
                    return 0
                }
            }
            return -1
        }

        method WriteFromBufferToQueue {} {
            if {[string length $WriteBuf] && [my HasQueue WriteQueue]} {
                set count [$WriteQueue write $WriteBuf]
                if {$count > 0} {
                    my SplitBuf $WriteBuf $count {} WriteBuf
                }
                return $count
            }
            return -1
        }

        method FindReadEndLine { {start 0} } {
            tailcall string first "\n" $ReadBuf $start
        }

        method PeekEndLineIndex {maxSize} {
            set index [my FindReadEndLine]
            if {$index < 0} {
                set toRead [my GetUnread $maxSize]
                if {$toRead <= 0} {
                    set index $maxSize
                } else {
                    set prevLen [string length $ReadBuf]
                    set readLen [my ReadFromQueueInBuffer $toRead]
                    if {$readLen > 0} {
                        set nextIndex [my FindReadEndLine $prevLen]
                        if {$nextIndex >= $prevLen} {
                            set index $nextIndex
                        } elseif {$prevLen + $readLen >= $maxSize} {
                            set index $maxSize
                        }
                    }
                }
            }
            return $index
        }

    } ; # oo::class DebugPipe
} ; # namespace eval mdbg::pipe
