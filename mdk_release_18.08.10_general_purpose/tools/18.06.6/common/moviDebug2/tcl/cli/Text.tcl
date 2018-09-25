# ***************************************************************************
# Copyright (C) 2016 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/Text.tcl
# Description: Text processing; Indentation && alignmnent
# Created on : Mar 12, 2016
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::cli {

    variable FILL_CHAR { }

    proc Indent {num} {
        variable FILL_CHAR
        Print [string repeat $FILL_CHAR $num]
    }

    proc AlignLeft {text width} {
        variable FILL_CHAR
        set len [string length $text]
        if {$len < $width} {
            append text [string repeat $FILL_CHAR [expr {$width - $len}]]
        } elseif {$len > $width} {
            set text [string range $text 0 [expr {$width - 1}]]
        } else {
            set text
        }
    }

    proc AlignRight {text width} {
        variable FILL_CHAR
        set len [string length $text]
        if {$len < $width} {
            set result [string repeat $FILL_CHAR [expr {$width - $len}]]
            append result $text
        } elseif {$len > $width} {
            string range $text [expr {$len - $width}] end
        } else {
            set text
        }
    }

    proc AlignCenter {text width {fill {}}} {
        if {$fill == {}} {
            variable FILL_CHAR
            set fill $FILL_CHAR
        } else {
            set fill [string range $fill 0 0]
        }
        set len [string length $text]
        if {$len < $width} {
            set spaceTotal [expr {$width - $len}]
            set spaceLeft [expr {int( $spaceTotal / 2 )}]
            set spaceRight [expr {$spaceTotal - $spaceLeft}]

            set result [string repeat $fill $spaceLeft]
            append result $text
            append result [string repeat $fill $spaceRight]

        } elseif {$len > $width} {
            set extraTotal [expr {$len - $width}]
            set extraLeft [expr {int( $extraTotal / 2 )}]
            set extraRight [expr {$extraTotal - $extraLeft}]
            string range $text [expr {$extraLeft}] [expr {$len - $extraRight - 1}]
        } else {
            set text
        }
    }

    proc Shorten {text maxlen {ratio 0.5} } {
        set len [string length $text]
        if {$len <= $maxlen} {
            return $text
        }
        set leftSize [expr {round( ( $maxlen - 1.5 ) * $ratio)} ]
        set rightSize [expr {$maxlen - $leftSize - 3} ]

        set tmp [mdbg::cli::AlignLeft $text $leftSize]
        append tmp ...
        append tmp [mdbg::cli::AlignRight $text $rightSize]
    }

    proc CharWrap {text max} {
        set result [list]
        foreach line [split $text \n] {
            set len [string length $line]
            set count [expr {( $len + $max - 1 ) / $max}]
            set begin 0
            set end [expr {$max - 1} ]
            for {set i 0} {$i < $count} {incr i} {
                lappend result [string range $line $begin $end]
                incr begin $max
                incr end $max
            }
        }
        return $result
    }

    proc WordWrap {text max} {
        set SEPARATORS(<) {\< \{ \[ \( \\ \+ \- \~ \* }
        set SEPARATORS(>) {\> \} \] \) \/ \|}
        set SEPARATORS(.) {\. \, \! \? \; \:}

        set result [list]

        foreach line [split $text "\n"] {
            set len [string length $line]
            if {$len < $max} {
                lappend result $line
                continue
            }
            while {[set len [string length $line]]} {
                if {$len <= $max} {
                    set found $len
                } else {
                    set found $max
                    set nextChar [string index $line $max]
                    if {[string is space -strict $nextChar]} {
                        incr found
                    } else {
                        for {set i $max} {[incr i -1] > 0} {} {
                            set thisChar [string index $line $i]
                            if {
                                [string is space -strict $thisChar]
                                || $thisChar in $SEPARATORS(.) && $nextChar ni $SEPARATORS(.) && $nextChar ni $SEPARATORS(>)
                                || $thisChar in $SEPARATORS(>) && $nextChar ni $SEPARATORS(>) && $nextChar ni $SEPARATORS(.)
                                || $thisChar ni $SEPARATORS(<) && $nextChar in $SEPARATORS(<) && $nextChar ni $SEPARATORS(.)
                            } {
                                set found [expr {$i + 1}]
                                break
                            }
                            set nextChar $thisChar
                        }
                    }
                }
                set front [string range $line 0 [expr {$found - 1}]]
                lappend result [string trimright $front]
                set line [string trimleft [string range $line $found end]]
            }
        }
        set result
    }

    proc WrapIndented {text max {min 8}} {
        regexp {^([ ]*)(.*)$} $text -> indent text
        if {$text == ""} {
            return $indent$text
        }
        set len [string length $indent]
        if {$len >= ($max - $min)} {
            set text [string range $indent $min end]$text
            set indent [string range $indent 0 $min-1]
            set len $min
        }
        incr max -$len
        lmap line [WordWrap $text $max] {
            string cat $indent $line
        }
    }

    proc FileLineColumn {FILE LINE {COLUMN {}}} {
        set result $FILE
        if {$LINE != {}} {
            append result ":$LINE"
            if {$COLUMN != {}} {
                append result ":$COLUMN"
            }
        } else {
            if {$COLUMN != {}} {
                append result ":*:$COLUMN"
            }
        }
        return $result
    }

    #  Unindent text (from first non-empty line's indentation level)
    proc Unindent {value} {
        set lines [split $value \n]
        set referenceLineIndex [lsearch -not -regexp $lines {^\s*$}]
        if {$referenceLineIndex >= 0} {
            set referenceLine [lindex $lines $referenceLineIndex]
            if {[regexp {^\s+} $referenceLine spaces]} {
                set RmWsRegEx "^\\s{1,[string length $spaces]}"
                set lastIndex [expr {
                    [llength $lines] - [lsearch -not -regexp [lreverse $lines] {^\s*$}] - 1
                }]

                set lines [lrange $lines $referenceLineIndex $lastIndex]
                set resultList [list]
                foreach line $lines {
                    regsub $RmWsRegEx $line {} line
                    lappend resultList $line
                }
                set value [join $resultList \n]
            }
        }
        string trimleft $value
    }

    # NOTE: untested
    proc RemoveListComments {text} {
        set result ""
        set linePrefix ""
        set braceLevel 0
        set inComment 0
        while {text != ""} {
            if {[
                    regexp {^(.*?)(\\\{|\\\}|\n|#|\\)((.)?(.*))$} $text \
                        -> pre special text next after
                ]} {
                if {!$inComment} {
                    append result $pre
                    if {!$braceLevel} {
                        append linePrefix $pre
                    }
                }
                switch -exact $special {
                    "\\" {
                        append result $special
                        append result $next
                        set text $after
                    }
                    "#" {
                        if {!$braceLevel} {
                            if {$inComment} {
                                append result $special
                            } elseif {[string is space $linePrefix]} {
                                set inComment 1
                            }
                        }
                    }
                    "\n" {
                        if {!$braceLevel} {
                            set linePrefix ""
                            if {$inComment} {
                                set special ""
                                set $inComment
                            }
                        }
                    }
                    "\{" {
                        incr braceLevel
                        append linePrefix $special
                    }
                    "\}" {
                        if {$braceLevel} {
                            incr braceLevel -1
                        }
                    }
                }
                if {!$inComment} {
                    append result $special
                    if {!$braceLevel} {
                        append linePrefix $special
                    }
                }
            } else {
                append result $text
                set text ""
            }
        }
        return $result
    }

} ; # namespace eval mdbg::cli