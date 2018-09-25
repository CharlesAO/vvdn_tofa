namespace eval mdbg {

    namespace eval FLAGS {
    }

    proc EVAL_FLAGS {ns flags} {
        set toEval [list]
        foreach arg $flags {
            if [regexp {^[[:alpha:]_]} $arg] {
                set var ${ns}::FLAGS::$arg
                if [info exists $var] {
                    lappend toEval [set $var]
                    continue
                }
            }
            lappend toEval $arg
        }
        expr $toEval
    }

    proc FLAGS {args} {
        EVAL_FLAGS [namespace current] $args
    }

    namespace export FLAGS

}