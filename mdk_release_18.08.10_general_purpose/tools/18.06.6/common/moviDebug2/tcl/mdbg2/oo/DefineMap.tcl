# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : DefineMap
# Description: Map symbolic definitions to numbers
# Created on : Aug 14, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
package require TclOO

namespace eval mdbg::oo {

    oo::class create DefineMap {

        variable FORMAT
        variable DEFINES

        #
        #  Public
        #

        constructor {} {
            set FORMAT 0x%.8X
            set DEFINES [dict create]
        }

        method add {name defx args} {
            dict set DEFINES $name [
                my upexprf 1 [concat $defx $args]
            ]
        }

        method has {name} {
            dict exists $DEFINES $name
        }

        method get {name} {
            dict get $DEFINES $name
        }

        method expr {defx args} {
            my upexpr 1 $defx {*}$args
        }

        method exprf {defx args} {
            my upexprf 1 $defx {*}$args
        }

        method upexpr {level defx args} {
            incr level
            set newexpr [
                my ReplaceDefs [concat $defx $args]
            ]
            uplevel $level expr [list $newexpr]
        }

        method upexprf {level defx args} {
            incr level
            format $FORMAT [
                my upexpr $level $defx {*}$args
            ]
        }

        method list {defl} {
            variable DEFINES
            set result [list]
            foreach def $defl {
                lappend result [dict get $DEFINES $def]
            }
            return $result
        }

        method switch {expr body} {
            my upswitch 1 $expr $body
        }

        method upswitch {level expr body} {
            incr level
            set newBody [list]
            foreach {caseExpr body} $body {
                lappend newBody \
                    [my upexprf $level $caseExpr] \
                    [list uplevel $level $body]
            }
            set selector [my upexprf $level $expr]
            switch -exact $selector $newBody
        }

        #
        #  Private
        #

        method IsWordChar {char} {
            string is wordchar -strict $char
        }

        method ReplaceWord {word} {
            if {![my has $word]} {
                return $word
            } else {
                return "([my get $word])"
            }
        }

        method ReplaceDefs {defx} {
            set result ""
            set word ""
            set state NONWORD

            set len [string length $defx]
            for {set i 0} {$i < $len} {incr i} {
                set char [string index $defx $i]
                set iswc [my IsWordChar $char]
                switch -exact $state {
                    NONWORD {
                        if {$iswc} {
                            set state WORD
                            append word $char
                        } else {
                            append result $char
                        }
                    }
                    WORD {
                        if {$iswc} {
                            append word $char
                        } else {
                            my AppendReplacedWord result $word
                            set word ""
                            set state NONWORD
                            append result $char
                        }

                    }
                }
            }
            my AppendReplacedWord result $word
        }

        method AppendReplacedWord {resultVar word} {
            upvar 1 $resultVar result
            if {[llength $word]} {
                append result [my ReplaceWord $word]
            } else {
                return $result
            }
        }

    } ; # oo::class create DefineMap

} ; # namespace eval mdbg::oo