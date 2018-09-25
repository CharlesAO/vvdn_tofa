# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : registers.tcl
# Description: Generic register definition and name-based access framework
# Created on : Aug 13, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

#
# Get/Set register by name
#
namespace eval mdbg {

    proc setReg32 {name args} {
        ::mvproto::set32 [getRegAddr $name] [expr $args]
    }

    proc getReg32 {name} {
        ::mvproto::get32 [getRegAddr $name]
    }

    set VALID_REG_CHANGE_OPS [list &= |= ^= += -= *= /= >>= <<=]

    proc changeReg32 {name operation args} {
        variable VALID_REG_CHANGE_OPS
        set addr [getRegAddr $name]
        if {$operation ni $VALID_REG_CHANGE_OPS} {
            return -code error "invalid operation: must be one of [join $VALID_REG_CHANGE_OPS {, }]"
        }
        set op [string range $operation 0 end-1]
        set value [::mvproto::get32 $addr]
        set value [expr "int( \$value $op \[expr \$args\] & 0xFFFFFFFF)"]
        ::mvproto::set32 $addr $value
        format 0x%.8X $value
    }

    namespace export getReg32
    namespace export setReg32
    namespace export changeReg32
}

namespace eval tcl::mathfunc {
    proc clog2 {x} {
        expr {wide(ceil(log($x)/log(2)))}
    }
} ; # namespace eval tcl::mathfunc

#
# Generic Register definition framework
#
namespace eval mdbg {

    variable REGISTER_MAP {}

    namespace eval REGISTERS {
        proc CLEAR_VARS {} {
            foreach VAR [info vars [namespace current]::*] {
                namespace eval :: [list unset $VAR]
            }
        }
        proc CLEAR_DEFS {} {
            set [namespace parent]::REGISTER_MAP [dict create]
        }
        proc CLEAR {} {
            CLEAR_VARS
            CLEAR_DEFS
        }

        proc SPLIT_NAME_OP_OFFSET {nameOrAddressAndOffset nameVar opVar offsetVar} {
            upvar 1 $nameVar name
            upvar 1 $opVar op
            upvar 1 $offsetVar offset

            set NAME    {(?: [A-Z_]+ (?: [A-Z_0-9] | [(][^()]*[)] )*)}
            set OP      {[+-]}
            set NUMBER  {(?: 0 | [1-9][0-9]* | 0x[0-9A-F]+ )}

            regexp -nocase -expanded "^ \
                ($NAME)                 \
                ($OP)                   \
                (                       \
                    (?: $NAME $OP )*    \
                    $NUMBER             \
                )                       \
                $" $nameOrAddressAndOffset -> name op offset
        }
    }

    proc REG_VAR_NAME {name} {
        return "::mdbg::REGISTERS::$name"
    }

    proc REG_DEFS {defs} {
        variable REGISTER_MAP
        # removes 98% of line comments
        if {[regsub -all -line {^\s*[#][^$]*$} $defs {} defs_without_comments]} {
            set defs $defs_without_comments
        }
        dict for {name def} $defs {
            dict set REGISTER_MAP $name $def
            set var [REG_VAR_NAME $name]
            if {[info exists $var]} {
                unset $var
            }
        }
    }

    proc getRegAddr {nameOrAddress} {

        variable REGISTER_MAP

        if {[string is integer -strict $nameOrAddress]} {
            return $nameOrAddress
        }

        if {[REGISTERS::SPLIT_NAME_OP_OFFSET $nameOrAddress name op offset]} {
            return [format "0x%.8X" [expr {[getRegAddr $name]} $op {[getRegAddr $offset]}]]
        }

        # Look for cached value
        set var [REG_VAR_NAME $nameOrAddress]

        if {[info exists $var]} {
            return [set $var]
        }

        set expression [getRegDef $nameOrAddress]
        set dynamic 0

        # The [@] makes the expression value dynamic/non-memoizable.
        if {[regsub {^\s*\[@\]} $expression {} expression ]} {
            set dynamic 1
        }

        # Replace register value reads with functions
        if {[regsub -all -expanded {
            (?:
                \[ \s*
                GET32
            )
            \s*
            (                           # 1
                [_A-Za-z][0-9_A-Za-z]*
                (?:[(]([^)]+)[)])?
            )
            \s* \]
        } $expression {[::mdbg::getReg32 \1]} expression]} {
            set dynamic 1
        }

        # replace $clog(x) with clog(x)
        regsub -all -expanded {
            \$ [(]
                \$(                           # 1
                  clog2 | size | bits
                  )
            [)]
            (                             # 2
                [(][^)]*[)]
            )
        } $expression {\1\2} expression

        # Replace conditional identifier names with conditional statement
        regsub -all -expanded {
            \$(                           # 1
                [_A-Za-z][0-9_A-Za-z]*
                (?:[(](?:[^)]+)[)])?
            )
            \s*
            [?][?]
        } $expression {[::mdbg::isRegDefined \1] ? [::mdbg::getRegAddr \1] :} expression

        # Replace identifier names with function calls
        regsub -all -expanded {
            \$(                           # 1
                [_A-Za-z][0-9_A-Za-z]*
                (?:[(]([^)]+)[)])?
            )
        } $expression {[::mdbg::getRegAddr \1]} expression

        # Remove hexadecimal digit separators
        regsub -all -expanded -nocase {
            \m(0x[0-9A-F]+)_([0-9A-F]+)\M
        } $expression {\1\2} expression

        # Evaluate and format result
        set addr [namespace eval mdbg::REGISTERS [list expr $expression]]
        set addr [format "0x%.8X" $addr]

        if {!$dynamic} {
            set $var $addr
        }
        return $addr
    }

    proc getRegOffset {regName baseName} {
        format "0x%.8X" [expr {
            [getRegAddr $regName] - [getRegAddr $baseName]
        }]
    }

    proc getRegDef {regName {verbose 0}} {
        variable REGISTER_MAP
        if {![dict exists $REGISTER_MAP $regName]} {
            return -code error "register \"$regName\" is not defined"
        }
        dict get $REGISTER_MAP $regName
    }

    proc isRegDefined {regName} {
        variable REGISTER_MAP
        REGISTERS::SPLIT_NAME_OP_OFFSET $regName regName op offset
        dict exists $REGISTER_MAP $regName
    }

    proc findReg {pattern args} {
        variable REGISTER_MAP
        set result [list]
        set patterns [list $pattern {*}$args]
        dict for {reg def} $REGISTER_MAP {
            set ismatch 0
            foreach pattern $patterns {
                if {[string match $pattern $reg]} {
                    set ismatch 1
                    break
                }
            }
            if {$ismatch} {
                lappend result $reg
            }
        }
        return $result
    }

    proc findRegAddr {pattern} {
        set result [list]
        foreach regName [findReg $pattern] {
            lappend result [list [getRegAddr $regName] $regName]
        }
        lsort -command {
            apply {{lhs rhs} {expr [lindex $lhs 0] - [lindex $rhs 0]}}
        } $result
    }

    proc lsreg {{pattern *}} {
        foreach item [findRegAddr $pattern] {
            lassign $item addr reg
            puts [format {[%s] %s} $addr $reg]
        }
    }

    proc lsregv {{pattern}} {
        set arl [findRegAddr $pattern]
        set w 0
        foreach ar $arl {
            set l [string length [lindex $ar 1]]
            set w [expr {max( $w, $l )}]
        }
        set fmt "\[%s\] %-${w}s : %s"
        foreach ar $arl {
            lassign $ar addr reg
            puts [format $fmt $addr $reg [getReg32 $addr]]
        }
    }

    shell::autocomplete::addScript {
            getRegAddr
            setReg32
            getReg32
            findReg
            lsreg
            lsregv
            getRegDef
        } {
            findReg $PREFIX*
        }

    shell::autocomplete::addScript changeReg32 {
        switch -exact [llength $PARAMS] {
            0 {
                findReg $PREFIX*
            }
            1 {
                lsearch -all -inline -glob $::mdbg::VALID_REG_CHANGE_OPS $PREFIX*
            }
            default {
                list $PREFIX
            }
        }
    }

    namespace export getRegAddr
    namespace export getRegDef
    namespace export findReg
    namespace export lsreg
}
