namespace eval mdbg {

    namespace eval cli::MemMap {

        namespace path ::mdbg

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Show memory map for target"
            -description {
                Display memory map for specified target or current target, as seen by TCF.

                *** EXAMPLES

                Display memory map for Leon OS:
                % memmap LOS

                Show all ".bss" sections for the entire application:
                % memmap A -bss

                Show all executable sections for the entire application:
                % memmap A -exec

                Show all non-BSS data sections for the entire application:
                % memmap -not-bss -target A -not-exec

                Show all Shave 0 code sections
                % memmap -exec -target S0

                Show the current core's section containing `POSIX_Init`:
                % memmap -at POSIX_Init

                Show Leon RT specific sections of the application (name starting with  ".lrt."):
                % memmap A -name .lrt.*

                Show '.text' and '.data' sections in the application (specified by regular expression):
                % memmap A -xname {\.(text|data)$}

            }
            -args {
                target(1) {
                    -optional
                    -var target
                    -hidden
                }
                -target {
                    -optional
                    -brief "Target name"
                    -synopsis {
                        ?-target? target
                    }
                    -description {
                        If missing, will use current target.
                    }
                }
                -quiet {
                    -flag
                    -brief "Return instead of print"
                    -description {
                        The result will be a list of dictionaries containing the section information.
                        NOTE: Not all fields will be present in each list item.
                    }
                }
                -first {
                    -flag "Return first element"
                    -description {
                        Return only the first element of list (dictionary of fields) or nothing none found.

                        Useful with "-at" and "-not-at" options where there is mostly a single element.

                        Implies "-quiet".
                    }
                }
                -name {
                    -optional
                    -var pattern
                    -brief "Only sections matching `pattern`"
                }
                -not-name {
                    -optional
                    -var notpattern
                    -brief "Exclude sections matching `notpattern`"
                }
                -xname {
                    -optional
                    -var regex
                    -brief "Only sections matching `regex` regular expression"
                }
                -not-xname {
                    -optional
                    -var notregex
                    -brief "Exclude sections matching `notregex` regular expression"
                }
                -bss {
                    -flag
                    -brief "Only BSS sections"
                }
                -not-bss {
                    -flag
                    -var notbss
                    -brief "Exclude BSS sections"
                }
                -exec {
                    -flag
                    -brief "Only executable sections"
                }
                -not-exec {
                    -flag
                    -var notexec
                    -brief "Exclude executable sections"
                }
                -read {
                    -flag -brief "Only readable sections"
                }
                -not-read {
                    -var notread
                    -flag
                    -brief "Exclude readable sections"
                }
                -write {
                    -flag
                    -brief "Only writable sections"
                }
                -not-write {
                    -flag
                    -var notwrite
                    -brief "Exclude writable sections"
                }
                -at {
                    -var addr
                    -optional
                    -brief "Return sections containing address, symbol or register"
                }
                -not-at {
                    -var notaddr
                    -optional
                    -brief "Exclude sections containing address, symbol or register"
                }
                target(2) {
                    -optional
                    -var target
                    -brief "Target name. Added for convenience"
                    -description {
                        Same as "-target" option.
                    }
                }
            }
            -mutex {
                {target(1) -target target(2)}
                {-bss -not-bss}
                {-exec -not-exec}
                {-read -not-read}
                {-write -not-write}
            }
            -arg-complete-scripts {
                {target(1) -target target(2)} {
                    lsort -dictionary [cli::Target::getValidTargets $PREFIX*]
                }
                {-at -not-at} {
                    concat [cli::Symbols::getElfSymbols name [mdbg::targetid] $PREFIX*] [mdbg::findReg $PREFIX*]
                }
            }

        } ; # cli::ArgumentParser

        cli::TcfTabularDisplay create Display MemoryMap {
            ADDR {
                -header "ADDRESS"
            }
            SECTION_NAME {
                -header "NAME"
                -default "<no name>"
            }
            SIZE {
                -align right
                -min-width 8
            }
            BSS {-default false -transform {
                expr {$BSS ? "BSS" : ""}
            } }
            FLAGS {-header RWX -transform {
                set    result [expr {"READ"  in $FLAGS ? "r" : "-"}]
                append result [expr {"WRITE" in $FLAGS ? "w" : "-"}]
                append result [expr {"EXEC"  in $FLAGS ? "x" : "-"}]
            } }
            FILE_NAME {
                -header "FILE NAME"
                -expand
                -wrap word
            }
            OFFS {
                -header "FILE OFFSET"
                -align right
            }
            FILE_SIZE {
                -header "FILE SIZE"
                -align right
            }
        }

        oo::objdefine Display {
            mixin cli::ResultTable
        }

        proc isBSS {section} {
            ::tcf::fields::getNamedField ::tcf::MemoryMap $section BSS false
        }

        proc getSectionName {section} {
            ::tcf::fields::getNamedField ::tcf::MemoryMap $section SECTION_NAME
        }

        proc getFlags {section} {
            ::tcf::fields::getNamedField ::tcf::MemoryMap $section FLAGS
        }

        # Return Low + High address
        proc getRange {section} {
            lassign [::tcf::fields::getNamedFieldList ::tcf::MemoryMap $section {ADDR SIZE} {0 0}] addr size
            cli::incrAddr size $addr
            list $addr $size
        }
    } ; # namespace eval cli::MemMap

    proc memmap {args} {
        cli::NoErrorStack {
            cli::MemMap::ArgumentParser parseCallerArgs 1 $args

            set id [cli::Target::getContextIdFromTargetVar target]
            set map [tcf::MemoryMap::get $id]

            set at [info exists addr]
            set notat [info exists notaddr]
            set name [info exists pattern]
            set xname [info exists regex]
            set notname [info exists notpattern]
            set notxname [info exists notregex]

            if {$at} {
                set addr [cli::Symbols::getAddress $addr]
            }
            if {$notat} {
                set notaddr [cli::Symbols::getAddress $notaddr]
            }

            cli::UpdateList map section {
                if {$bss || $notbss} {
                    set BSS [cli::MemMap::isBSS $section]
                }
                if {$name || $notname || $xname || $notxname} {
                    set NAME [cli::MemMap::getSectionName $section]
                }
                if {$exec || $notexec || $read || $notread || $write || $notwrite} {
                    set FLAGS [cli::MemMap::getFlags $section]
                }
                if {$at || $notat} {
                    lassign [cli::MemMap::getRange $section] LOW HIGH
                }
                if {
                        $bss   && !$BSS                              || $notbss   && $BSS
                    ||  $name  && ![string match $pattern $NAME]     || $notname  && [string match $notpattern $NAME]
                    ||  $xname && ![regexp       $regex   $NAME]     || $notxname && [regexp       $notregex   $NAME]
                    ||  $exec  && "EXEC"  ni $FLAGS                  || $notexec  && "EXEC"  in $FLAGS
                    ||  $write && "WRITE" ni $FLAGS                  || $notwrite && "WRITE" in $FLAGS
                    ||  $read  && "EXEC"  ni $FLAGS                  || $notread  && "READ"  in $FLAGS
                    ||  $at    && ( $addr < $LOW || $addr >= $HIGH ) || $notat    && ( $notaddr >= $LOW && $notaddr < $HIGH )
                } {
                    unset section
                }
            }
            if {$quiet || $first} {
                set result [::tcf::fields::getNamedFieldDictList ::tcf::MemoryMap $map]
                if {$first} {
                    return [lindex $result 0]
                }
                return $result
            }
            cli::MemMap::Display display $map
        }
    }

    cli::Help::Manager add memmap -parser cli::MemMap::ArgumentParser
    shell::autocomplete::addCommand memmap cli::MemMap::ArgumentParser autocomplete

    namespace export memmap

} ; # namespace eval mdbg
