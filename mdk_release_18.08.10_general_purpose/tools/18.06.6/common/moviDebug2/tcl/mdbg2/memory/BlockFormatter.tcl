namespace eval mdbg::memory {

    # mixin class to format/scan block-based memory
    # requires host to have readBlock/writeBlock methods
    oo::class create BlockFormatter {

        method getBlock {args} {
            if {[llength $args] >= (2 + 2)
             && [lindex $args 0] == "-scan"
            } {
                set upVars [list]
                foreach var [lrange $args 3 end] {
                    set upVar UP_$var
                    upvar 1 $var $upVar
                    lappend upVars $upVar
                }
                my scanBlock {*}[lrange $args 1 3] result {*}$upVars
                return $result
            }
            tailcall my readBlock {*}$args
        }

        method setBlock {args} {
            if {[llength $args] > 2
             && [lindex $args 0] == "-format"
            } {
                tailcall my formatBlock {*}[lrange $args 1 end]
            }
            tailcall my writeBlock {*}$args
        }

        method scanBlock {format address size arg args} {
            uplevel 1 [list binary scan [
                my readBlock $address $size
            ] $format $arg {*}$args]
        }

        method formatBlock {format address arg args} {
            tailcall my writeBlock $address [
                binary format $format $arg {*}$args
            ]
        }
    }

}