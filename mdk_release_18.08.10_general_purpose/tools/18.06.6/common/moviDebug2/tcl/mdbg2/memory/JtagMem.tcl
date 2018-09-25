# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : memory/JtagMem.tcl
# Description: JTAG memory interface (direct access to protocol)
# Created on : Aug 4, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
package require TclOO

namespace eval mdbg::memory {

    oo::class create JtagMem {

        variable BURST_ALIGN
        variable BURST_BOUNDARY
        variable WORD_SIZE
        variable MIN_BURST_SIZE

        constructor {} {
            set BURST_ALIGN 16
            # We could relax the restrictions for Cheetah
            #if {   [info exists ::mvproto::ServerID]
            #    && [dict exists $::mvproto::ServerID dongleType]
            #    && [dict get $::mvproto::ServerID dongleType] == "MV229"
            #} {
            #    set BURST_ALIGN 4
            #}
            set BURST_BOUNDARY [expr 1024 * 1024]
            set WORD_SIZE 4
            set MIN_BURST_SIZE 8
        }

        method lock {script} {
            mvproto::lock {
                uplevel 1 $script
            }
        }

        method Reg2Addr {addressVar} {
            # does nothing
        }

        method readBlock {address size} {
            my Reg2Addr address
            set ranges [my Decompose $address $size]
            set result [binary format a* {}]
            mvproto::lock {
                foreach range $ranges {
                    lassign $range addr size
                    append result [my GetData $addr $size]
                }
            }
            return $result
        }

        method writeBlock {address data} {
            my Reg2Addr address
            set size [string length $data]
            set ranges [my Decompose $address $size]
            mvproto::lock {
                foreach range $ranges {
                    lassign $range addr size offset
                    set subdata [my DataRange $data $offset $size]
                    my SetData $addr $size $subdata
                }
            }
        }

        method fillBlock {address size pattern} {
            my Reg2Addr address
            set ranges [my Decompose $address $size]
            set patSize [expr {
                min ( [string length $pattern], $size )
            }]
            if {$patSize <= 0} {
                set pattern [binary format x]
                set patSize 1
            }

            mvproto::lock {
                foreach range $ranges {
                    lassign $range addr size offset

                    set patOffset [expr $offset % $patSize]
                    set repCount [expr { ( $patOffset + $size + $patSize - 1 ) / $patSize}]
                    set repData [string repeat $pattern $repCount]

                    set subdata [my DataRange $repData $patOffset $size]

                    my SetData $addr $size $subdata

                }
            }
        }

        #
        # Get/Set methods of various size/alignment
        #

        method GetSubWord {address size} {
            lassign [my SplitAddress $address $WORD_SIZE] aligned offset
            set word [my GetWord $aligned]
            string range $word $offset [expr {$offset + $size - 1}]
        }

        method GetWord {address} {
            binary format i [
                ::mvproto::get32 $address
            ]
        }

        method GetBurst {address size} {
            binary format i* [
                ::mvproto::getBurst32 $address [expr {$size / $WORD_SIZE}]
            ]
        }

        method GetData {addr size} {
            if {$size < $WORD_SIZE} {
                tailcall my GetSubWord $addr $size
            } elseif {$size == $WORD_SIZE} {
                tailcall my GetWord $addr
            } else {
                tailcall my GetBurst $addr $size
            }
        }

        method SetSubWord {address data} {
            lassign [my SplitAddress $address $WORD_SIZE] aligned offset
            set word [my GetWord $aligned]
            set size [string length $data]
            set word [string replace $word $offset [
                expr {$offset + $size - 1}] $data
            ]
            my SetWord $aligned $word
        }

        method SetWord {address data} {
            binary scan $data i word
            ::mvproto::set32 $address $word
        }

        method SetBurst {address data} {
            binary scan $data i* words
            ::mvproto::setBurst32 $address $words
        }

        method DataRange {data offset size} {
            string range $data $offset [expr {$offset + $size - 1}]
        }

        method SetData {addr size data} {
            if {$size < $WORD_SIZE} {
                my SetSubWord $addr $data
            } elseif {$size == $WORD_SIZE} {
                my SetWord $addr $data
            } else {
                my SetBurst $addr $data
            }
        }

        # Set burst alingment; return previous value
        method SetBurstAlign {align} {
            if {![string is integer $align]} {
                return -code error "\"$size\" is not an integer"
            }
            if {$align < 4} {
                return -code error "$align is too small (need at least 4)"
            }
            if { $align & ( $align - 1 ) } {
                return -code error "$align is not a power of 2"
            }
            set prevValue $BURST_ALIGN
            set BURST_ALIGN $align
            return $prevValue
        }

        # Set burst boundary; return previous value
        method SetBurstBoundary {size} {
            if {![string is integer $size]} {
                return -code error "\"$size\" is not an integer"
            }
            if {$size < $MIN_BURST_SIZE} {
                return -code error "$size is too small (need at least $MIN_BURST_SIZE)"
            }
            if { $size & ( $size - 1 ) } {
                return -code error "$size is not a power of 2"
            }
            set prevValue $BURST_BOUNDARY
            set BURST_BOUNDARY $size
            return $prevValue
        }

        # Decompose address to manageable ranges
        method Decompose {startAddress totalSize {dataOffset 0}} {
            set result [list]

            set addr $startAddress
            set pos $dataOffset
            set left $totalSize

            # leading word-unaligned bytes
            lassign [my SplitAddress $addr $WORD_SIZE] aligned offs
            if {$offs} {
                set pre [expr $WORD_SIZE - $offs]
                if {$pre > $left} {
                    set pre $left
                }
                my Append result $addr $pre $pos
                my Advance $pre addr left pos
            }

            if {$left >= $WORD_SIZE} {

                # leading burst-unaligned words, aligned
                lassign [my SplitAddress $addr $BURST_ALIGN] aligned offs

                if {$offs} {
                    set pre [expr $BURST_ALIGN - $offs]
                    if {$pre > $left} {
                        set pre $left
                    }

                    while {$pre >= $WORD_SIZE} {
                        my Append result $addr $WORD_SIZE $pos
                        my Advance $WORD_SIZE addr left pos pre
                    }
                }

                # bursts, aligned, between boundaries, of minimum size
                while {$left >= $MIN_BURST_SIZE} {
                    set burstSize [my GetBurstSize $addr $left]
                    if {$burstSize >= $MIN_BURST_SIZE} {
                        my Append result $addr $burstSize $pos
                        my Advance $burstSize addr left pos
                    } else {
                        break
                    }
                }

                # trailing burst-unaligned words, aligned
                while {$left > $WORD_SIZE} {
                    my Append result $addr $WORD_SIZE $pos
                    my Advance $WORD_SIZE addr left pos
                }
            }

            # trailing bytes, aligned
            if {$left > 0} {
                my Append result $addr $left $pos
                my Advance $left addr left pos
            }

            return $result
        }

        #
        #  Helper methods
        #

        method AlignAddress {address alignment} {
            expr { $address & ~ ( $alignment - 1 )}
        }

        method SplitAddress {address alignment} {

            set mask [expr {$alignment - 1}]
            list \
                [expr {$address & ~ $mask}] \
                [expr {$address &   $mask}]

        }

        method Hex {addr} {
            format {0x%.8X} $addr
        }

        #
        #  Decompose helpers
        #

        method Append {resultVar addr size pos} {
            upvar 1 $resultVar result
            lappend result [list [my Hex $addr] $size $pos]
        }

        method Advance {offset addrVar sizeVar posVar {leftVar {}}} {
            upvar 1 $addrVar addr
            upvar 1 $sizeVar size
            upvar 1 $posVar pos

            set negOffset [expr {- $offset}]
            incr addr $offset
            incr size $negOffset
            incr pos $offset

            if {[string length $leftVar]} {
                upvar 1 $leftVar left
                incr left $negOffset
            }
        }

        method GetBurstSize {address size} {

            if {$size > $BURST_BOUNDARY} {
                set size $BURST_BOUNDARY
            }

            set endAddr [expr {$address + $size}]
            set alignedEnd [my AlignAddress $endAddr $BURST_BOUNDARY]

            if {$address < $alignedEnd} {
                set endAddr $alignedEnd
            }

            set endAddr [my AlignAddress $endAddr $WORD_SIZE]
            set size [expr {$endAddr - $address}]

            expr { max( $size, 0 ) }
        }

    } ; # oo::class create JtagMem

} ; # namespace eval mdbg::memory