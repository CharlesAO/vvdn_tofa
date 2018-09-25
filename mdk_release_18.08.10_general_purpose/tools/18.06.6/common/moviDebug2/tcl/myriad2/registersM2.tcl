# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : registersM2.tcl
# Description: Support for importing myriad2 registers header file definitions
#              to register definitions framework.
# Created on : May 19, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg::REGISTERS {

    proc PARSE_HEADER {headerFileName} {
        set DEFINE_REGEX {
            ^\s*\#\s*define\s+
            (                           # name
                [_A-Za-z][0-9_A-Za-z]*
                (?:[(] \s? [_A-Za-z]+[0-9_A-Za-z]*  \s? [)] )?
            )
            \s*
            (.*?)                       #value
            \s*
            (?:
                //  (.*)     #sl_comment
             |  /\* (.*) \*/ #ml_comment
            )?
            \s*$
        }

        set IDENT_REGSUB {
            (^|\s|[^0-9_A-Za-z])             # 1
            ([A-Z_a-z][0-9_A-Za-z]*)         # 2
        }
        set IDENT_SUBSPEC {\1($\2)}

        #puts "open $headerFileName"
        set headerFile [open $headerFileName r]

        set definitionList {}

        if {[catch {
            while {![eof $headerFile]} {
                gets $headerFile line

                # sanitize line (convert whitespace to space)
                regsub -all {[[:space:]]} $line "\x20" line

                if {[regexp -expanded $DEFINE_REGEX $line -> name value sl_comment ml_comment]} {
                    # puts $::mdbg::Channels::debug "Parsing: $line"

                    # add $ to identifiers in value
                    regsub -expanded -all $IDENT_REGSUB $value $IDENT_SUBSPEC value

                    # remove multiple whitespace
                    regsub -all {[[:space:]]+} $value "\x20" value

                    if {[string length $value]} {
                        lappend definitionList $name $value
                    }

                } else {
                    # puts $::mdbg::Channels::warning "Not parsed: $line"
                }
            }
        } result]} {
            puts stderr "Error parsing $headerFileName: $result"
        }
        close $headerFile
        return $definitionList
    }

    proc FROM_HEADER {file} {
        ::mdbg::REG_DEFS [PARSE_HEADER $file]
    }

}

