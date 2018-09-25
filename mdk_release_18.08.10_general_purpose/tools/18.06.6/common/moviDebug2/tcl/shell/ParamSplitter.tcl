# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : ParamSplitter.tcl
# Description: Tcl command parameter splitter class
# Created on : Aug 13, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
package require TclOO

namespace eval mdbg::shell::autocomplete {

    oo::class create ParamSplitter {

        method split {input} {
            my Reset

            for {set i 0} {$i < [string length $input]} {incr i} {
                my FeedChar [string index $input $i]
            }
            my FeedChar ""
            return $Params
        }

        #
        # Private
        #

        variable State
        variable Stack
        variable Params
        variable Word
        variable Char
        variable CharType

        method Reset {} {
            set State SPACE
            set Stack [list]
            set Word ""
            set Params [list]
            set Char ""
            set CharType ""
        }

        method PushState {state} {
            my DEBUG $Stack $state
            lappend Stack $State
            set State $state
        }

        method PopState {} {
            my DEBUG $Stack
            if {![llength $Stack]} {
                return -code error "State Stack is empty"
            }
            set State [lindex $Stack end]
            set Stack [lrange $Stack 0 end-1]
            return $State
        }

        method IsState {state} {
            string equal $State $state
        }

        method CharType {char} {
            switch -exact $char {
                {}   { return TERM}
                \x24 { return DOLLAR_SIGN }
                \x7B { return OPEN_BRACE }
                \x7D { return CLOSE_BRACE }
                \x5B { return OPEN_BRACKET }
                \x5C { return BACKSLASH }
                \x5D { return CLOSE_BRACKET }
                \x28 { return OPEN_PAREN }
                \x29 { return CLOSE_PAREN }
                \x22 { return DOUBLE_QUOTE }
                : -
                _ {
                    return IDENTIFIER
                }
            }

            if [string is space -strict $char] {
                return SPACE
            }
            if {[string is alnum -strict $char]} {
                return IDENTIFIER
            }
            return OTHER
        }

        method AppendChar {} {
            my DEBUG $Word $Char
            append Word $Char
        }

        method AppendWord {} {
            my DEBUG $Params $Word
            lappend Params $Word
            set Word ""
        }

        method FeedChar {char} {
            set Char $char
            set CharType [my CharType $char]
            my DEBUG "'$Char': $CharType"
            tailcall my $State
        }

        #
        # State methods
        #

        method DEBUG {args} {
            #puts stderr "DEBUG: [concat [lindex [self caller] 2] $args]"
        }

        method SPACE {} {
            my DEBUG
            switch -exact $CharType {
                TERM -
                SPACE {
                }
                default {
                    tailcall my [my PushState WORD_START]
                }
            }
        }

        method WORD_START {} {
            my DEBUG
            switch -exact $CharType {
                OPEN_BRACE {
                    my AppendChar
                    set State WORD_END
                    my PushState BRACE
                }
                DOUBLE_QUOTE {
                    my AppendChar
                    set State WORD_END
                    my PushState QUOTE
                }
                default {
                    tailcall my [set State WORD]
                }
            }
        }

        method WORD {} {
            my DEBUG
            switch -exact $CharType {
                TERM -
                SPACE {
                    tailcall my [set State WORD_END]
                }
                DOLLAR_SIGN {
                    my AppendChar
                    my PushState VARIABLE_START
                }
                BACKSLASH {
                    my AppendChar
                    my PushState BACKSLASH
                }
                OPEN_BRACKET {
                    my AppendChar
                    my PushState BRACKET
                }
                default {
                    my AppendChar
                }
            }
        }

        method WORD_END {} {
            my DEBUG
            switch -exact $CharType {
                TERM -
                SPACE {
                    my PopState
                    if {[my IsState SPACE]} {
                        my AppendWord
                    } else {
                        my AppendChar
                    }
                }
                default {
                    # Incorrect syntax, just collect'em until whitespace
                    my AppendChar
                }
            }
        }

        method BACKSLASH {} {
            my DEBUG
            my AppendChar
            my PopState
        }

        method VARIABLE_START {} {
            my DEBUG
            switch -exact $CharType {
                OPEN_BRACE {
                    my AppendChar
                    set State BRACE
                }
                default {
                    tailcall my [set State VARIABLE]
                }
            }
        }

        method VARIABLE {} {
            my DEBUG
            switch -exact $CharType {
                OPEN_PAREN {
                    my AppendChar
                    set State PAREN
                }
                IDENTIFIER {
                    my AppendChar
                }
                default {
                    tailcall my [my PopState]
                }
            }
        }

        method QUOTE {} {
            my DEBUG
            my AppendChar
            switch -exact $CharType {
                DOLLAR_SIGN {
                    my PushState VARIABLE_START
                }
                BACKSLASH {
                    my PushState BACKSLASH
                }
                OPEN_BRACKET {
                    my PushState BRACKET
                }
                DOUBLE_QUOTE {
                    my PopState
                }
                TERM {
                    tailcall my [my PopState]
                }
            }
        }

        method PAREN {} {
            my DEBUG
            my AppendChar
            switch -exact $CharType {
                BACKSLASH {
                    my PushState BACKSLASH
                }
                CLOSE_PAREN {
                    my PopState
                }
                OPEN_BRACE {
                    my PushState BRACE
                }
                OPEN_BRACKET {
                    my PushState BRACKET
                }
                DOLLAR_SIGN {
                    my PushState VARIABLE_START
                }
                TERM {
                    tailcall my [my PopState]
                }
            }
        }

        method BRACE {} {
            my DEBUG
            my AppendChar
            switch -exact $CharType {
                BACKSLASH {
                    my PushState BACKSLASH
                }
                CLOSE_BRACE {
                    my PopState
                }
                OPEN_BRACE {
                    my PushState BRACE
                }
                TERM {
                    tailcall my [my PopState]
                }
            }
        }

        method BRACKET {} {
            my DEBUG
            my AppendChar
            switch -exact $CharType {
                BACKSLASH {
                    my PushState BACKSLASH
                }
                OPEN_BRACKET {
                    my PushState BRACKET
                }
                CLOSE_BRACKET {
                    my PopState
                }
                OPEN_BRACE {
                    my PushState BRACE
                }
                DOLLAR_SIGN {
                    my PushState VARIABLE_START
                }
                TERM {
                    tailcall my [my PopState]
                }
            }
        }

    } ; # oo::class ParamSplitter

} ; # namespace eval mdbg::shell::autocomplete
