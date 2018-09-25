# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/binary.tcl
# Description: Conversion between string and common binary representations
# Created on : Sep 15, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::cli::Binary {
    namespace path ::mdbg

    proc scanData {data type size} {
        switch -glob $type {
            INT {
                scanInt $data $size
            }
            UNSIGNED {
                scanUnsigned $data $size
            }
            HEX_UINT {
                scanHexUnsigned $data $size
            }
            FLOAT {
                scanFloat $data $size
            }
            POINTER {
                scanPointer $data $size
            }
            BINARY {
                scanBinary $data $size
            }
            BE_* {
                return -code error "conversion from $type not implemented"
            }
            default {
                return -code error "unsupported type $type"
            }
        }
    }

    proc decimal {value} {
        format %d $value
    }

    proc scanInt {data size} {
        set count [switch -exact [decimal $size] {
            1 {
                binary scan $data c value
            }
            2 {
                binary scan $data s value
            }
            4 {
                binary scan $data i value
            }
            8 {
                binary scan $data w value
            }
            default {
                return -code error "invalid size for integer"
            }
        }]
        if {$count != 1} {
            return -code error "error scanning integer: insufficient length"
        }
        return $value
    }

    proc scanUnsigned {data size} {
        set count [switch -exact [decimal $size] {
            1 {
                binary scan $data cu value
            }
            2 {
                binary scan $data su value
            }
            4 {
                binary scan $data iu value
            }
            8 {
                binary scan $data wu value
            }
            default {
                return -code error "invalid size for unsigned"
            }
        }]
        if {$count != 1} {
            return -code error "error scanning unsigned: insufficient length"
        }
        return $value
    }

    proc scanHexUnsigned {data size} {
        format "%#.[expr {$size * 2}]x" [scanUnsigned $data $size]
    }

    proc scanFloat {data size} {
        set count [switch -exact [decimal $size] {
            2 {
                set value [mdbg::binaryToFp16 -le $data]
                llength $value
            }
            4 {
                binary scan $data r value
            }
            8 {
                binary scan $data q value
            }
            default {
                return -code error "invalid size for float"
            }
        }]
        if {$count != 1} {
            return -code error "error scanning float: insufficient length"
        }
        return $value
    }

    proc scanPointer {data size} {
        set count [switch -exact [decimal $size] {
            4 {
                binary scan $data iu value
            }
            8 {
                binary scan $data wu value
            }
            default {
                return -code error "invalid size for pointer"
            }
        }]
        if {$count != 1} {
            return -code error "error scanning pointer: insufficient length"
        }
        format "0x%.[expr {$size * 2}]X" $value
    }

    proc scanBinary {data size} {
        if {[string length $data] < $size} {
            return -code error "insufficient length for binary to scan"
        }
        string range $data 0 [expr {$size - 1}]
    }

    proc scanTclFmt {data format} {
        set len [string length $format]
        set vars [list]
        for {set i 0} {$i < $len} {incr i} {
            lappend vars var$i
        }
        set len [binary scan $data $format {*}$vars]
        if {$len == 0} {
            return
        }
        if {$len == 1} {
            return $var0
        }
        set result [list]
        for {set i 0} {$i < $len} {incr i} {
            lappend result [set var$i]
        }
        return $result
    }

    proc scanByteList {data {fmt {}} } {
        binary scan $data cu* byteList
        if {$fmt != {}} {
            cli::UpdateList byteList byte {
                set byte [format $fmt $byte]
            }
        }
        return $byteList
    }

    proc scanU32List {data {fmt {}}} {
        binary scan $data iu* u32List
        if {$fmt != {}} {
            cli::UpdateList u32List u32 {
                set u32 [format $fmt $u32]
            }
        }
        return $u32List
    }

    proc forEachChar {charVar string body} {
        set length [string length $string]
        upvar 1 $charVar char
        for {set i 0} {$i < $length} {incr i} {
            set char [string index $string $i]
            uplevel 1 $body
        }
    }

    proc scanAscii {data {npch .}} {
        set result {}
        forEachChar char $data {
            append result [expr {
                [string is print $char] ? $char : $npch
            }]
        }
        set result
    }

    proc forEachBlock {blockSize blockVar data body} {
        set numBlocks [expr {([string length $data] + $blockSize - 1 ) / $blockSize}]
        set begin 0
        set end [expr {$blockSize - 1}]
        upvar 1 $blockVar block
        for {set i 0} {$i < $numBlocks} {incr i} {
            set block [string range $data $begin $end]
            uplevel 1 $body
            incr begin $blockSize
            incr end   $blockSize
        }
    }

    proc formatData {type size value} {
        switch -glob $type {
            INT {
                formatInt $size $value
            }
            UNSIGNED - HEX_UINT {
                formatUnsigned $size $value
            }
            FLOAT {
                formatFloat $size $value
            }
            POINTER {
                formatPointer $size $value
            }
            BINARY {
                formatBinary $size $value
            }
            BE_* {
                return -code error "conversion from $type not implemented"
            }
            default {
                return -code error "unsupported type $type"
            }
        }
    }

    proc formatInt {size value} {
        switch -exact [decimal $size] {
            1 {
                binary format c $value
            }
            2 {
                binary format s $value
            }
            4 {
                binary format i $value
            }
            8 {
                binary format w $value
            }
            default {
                return -code error "invalid size for integer"
            }
        }
    }

    proc formatUnsigned {size value} {
        switch -exact [decimal $size] {
            1 {
                binary format cu $value
            }
            2 {
                binary format su $value
            }
            4 {
                binary format iu $value
            }
            8 {
                binary format wu $value
            }
            default {
                return -code error "invalid size for unsigned"
            }
        }
    }

    proc formatFloat {size value} {
        switch -exact [decimal $size] {
            2 {
                mdbg::fp16ToBinary -le $value
            }
            4 {
                binary format r $value
            }
            8 {
                binary format q $value
            }
            default {
                return -code error "invalid size for float"
            }
        }
    }

    proc formatPointer {size value} {
        switch -exact [decimal $size] {
            4 {
                binary format iu $value
            }
            8 {
                binary format wu $value
            }
            default {
                return -code error "invalid size for pointer"
            }
        }
    }

    proc formatBinary {size value} {
        if {[string length $value] < $size} {
            return -code error "insufficient length for binary to format"
        }
        string range $value 0 [expr {$size - 1}]
    }

    proc formatBase64 {base64str} {
        binary decode base64 $base64str
    }

} ; # namespace eval cli::Binary
