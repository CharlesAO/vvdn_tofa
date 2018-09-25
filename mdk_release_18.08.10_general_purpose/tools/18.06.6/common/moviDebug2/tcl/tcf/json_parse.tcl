#
#  JSON Parsing to Tcl objects.
#  Not really needed anymore, since we have
#      `tcf::protocol::parseJsonValue`
#  and `tcf::protocol::parseJsonValueList`
#
#  Kept for reference.
#
#  Also the main differences from the C++ implementation
#  are that this version does not support \uXXXX sequences in strings
#  and the C++ version sorts the elements of a json object dictionary by key.
#

namespace eval json::parse {

    proc Full {json} {
        set pos 0
        set end [string length $json]
        set obj {}
        Call Value obj
        SkipSpace
        if [More] {
            Error "Extra characters"
        }
        return $obj
    }

    proc UpVar_IO {} {
        uplevel 1 {
             upvar $jsonVar json
             upvar $resultVar result
         }
    }

    proc More {} {
        upvar pos pos
        upvar end end
        expr $pos < $end
    }

    proc Char {} {
        uplevel 1 string index {$json $pos}
    }

    proc Advance {} {
        upvar pos pos
        incr pos
    }

    proc IsSpace {} {
        uplevel 1 {
             string is space [Char]
         }
    }

    proc SkipSpace {} {
        uplevel 1 {
             while {[More] && [IsSpace]} {
                 Advance
             }
         }
    }

    proc Call {name {resultVar result}} {
        upvar pos pos
        set pos [uplevel 1 $name json {$pos $end} $resultVar]
    }

    proc Literal {jsonVar pos end resultVar} {
        UpVar_IO
        SkipSpace
        set result {}
        while {[More]} {
            switch [Char] {
                \{ - \} -
                \[ - \] -
                {,}     -
                \" - \' -
                \\      -
                {:}     {return $pos}
            }
            if [IsSpace] {
                return $pos
            }
            append result [Char]
            Advance
        }

    }

    proc Value {jsonVar pos end resultVar} {
        UpVar_IO
        while {[More]} {
            SkipSpace
            switch [Char] {
                \{      {return [Call Struct]}
                \[      {return [Call Array]}
                \"      {return [Call String]}
                ","     -
                ":"     {return $pos}
                default {return [Call Literal]}
            }
        }
        return $pos
    }

    proc PosStr {} {
        upvar pos pos
        upvar json json
        set errorstr "at $pos in JSON string\n"
        if {$pos <= 60} {
            set start 0
            set prefix ""
        } else {
            set start [expr $pos - 60]
            set prefix "..."
        }
        append errorstr $prefix [string range $json $start $pos] " < ..."
    }

    proc Error {errorstr} {
        error "$errorstr\n[uplevel 1 PosStr]"
    }

    proc Warning {warnstr} {
        puts stderr "$warnstr\n[uplevel 1 PosStr]"
    }

    proc CharIs {c} {
        string equal [uplevel 1 Char] "$c"
    }

    proc IsNotChar {c} {
        expr ! [string equal [uplevel 1 Char] "$c"]
    }

    proc CheckChar {c} {
        if {![string equal [uplevel 1 Char] "$c"]} {
            uplevel 1 Error {"Unexpected char `[Char]`."}
        }
    }

    variable ESC_DICT {
         b "\b"
         f "\f"
         n "\n"
         r "\r"
         t "\t"
     }

    proc String {jsonVar pos end resultVar} {
        variable ESC_DICT
        UpVar_IO
        CheckChar \"
        Advance
        set result {}
        while {[More] && [IsNotChar \"]} {
            if [CharIs \\] {
                Advance
                if [More] {
                    if [dict exists $ESC_DICT [Char]] {
                        append result [dict get $ESC_DICT [Char]]
                    } else {
                        append result [Char]
                    }
                    Advance
                }
            } else {
                append result [Char]
                Advance
            }
        }
        CheckChar \"
        return [Advance]
    }

    proc Struct {jsonVar pos end resultVar} {
        UpVar_IO
        CheckChar "\{"
        Advance
        set result {}
        while {[More]} {
            SkipSpace
            switch [Char] {
                "\}" {
                    return [Advance]
                }
                "," {
                    if {[llength $result] == 0} {
                        Warning "Unexpected comma."
                    }
                    Advance
                }
            }
            Call KeyValue
        }
        return $pos
    }

    proc Array {jsonVar pos end resultVar} {
        UpVar_IO
        CheckChar "\["
        Advance
        set result {}
        while {[More]} {
            SkipSpace
            switch [Char] {
                "\]" {
                    return [Advance]
                }
                "," {
                    if {[llength $result] == 0} {
                        Warning "Unexpected comma."
                    }
                    Advance
                }
            }
            set value {}
            Call Value value
            lappend result $value
        }
        return $pos
    }

    proc KeyValue {jsonVar pos end resultVar} {
        UpVar_IO
        SkipSpace
        set key {}
        Call String key
        if [dict exists $result $key] {
            Warning "Duplicate key $key"
        }
        SkipSpace
        CheckChar ":"
        Advance
        set value {}
        Call Value value
        dict set result $key $value
        return $pos
    }

}
