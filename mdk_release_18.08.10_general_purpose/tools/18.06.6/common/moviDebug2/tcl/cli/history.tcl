namespace eval mdbg {

    namespace eval cli::History {

        namespace path ::mdbg

        cli::ArgumentParser create ArgumentParser {
            -auto
            -brief "Execution history."
            -description {
                Displays execution history for selected target.

                *** NOTE

                For SHAVEs the last item is the current instruction.

                *** EXAMPLES

                Displays history from current target:
                % hist

                Displays the last 5 instruntion from current target:
                % hist 5

                Displays the last 10 instructions and their opcode from LOS:
                % hist -target LOS -count 10 -opcode

            }
            -args {
                -target {
                    -brief "Select target."
                    -description {
                        Default is current target.
                    }
                    -optional
                }
                -count {
                    -brief "Maximum count of instructions."
                    -description {

                    }
                    -type integer
                    -default 16
                }
                -quiet {
                    -flag
                    -brief "Don't display, just return values."
                }
                -pc {
                    -flag
                    -brief "Only show program counter."
                }
                -opcodes {
                    -flag
                    -brief "Show opcodes."
                }
                -dont-decode {
                    -var dontDecode
                    -flag
                    -brief "Omit extra column for decoded instructions."
                    -description {
                        Used when "-opcodes" also specified.
                    }
                }
                target {
                    -optional
                    -brief "Same as -target."
                    -description {
                        Added for convenience.
                        Cannot be used together with "-target"
                    }
                }
            }
            -mutex {
                {-target target}
                {-pc -opcodes}
            }
            -arg-complete-scripts {
                {-target target} {
                    mdbg::cli::Target::getValidTargets $PREFIX*
                }
            }
        }

        cli::TcfTabularDisplay create Display MyriadCore::HISTORY  {
            TIME_TAG {
                -header "[Time_Tag]"
                -format [%.8X]
            }
            PC {
                -header "PC"
            }
            OPCODES {
                -header "Opcodes"
                -transform {
                    set bin [cli::Binary::formatBase64 $OPCODES]
                    cli::Binary::scanByteList $bin %.2X
                }
            }
            DISASSEMBLY {
                -header "Disassembly"

            }
            RESULT_OR_STORE {
                -header "Res./Store"
            }
            INSTRUCTION_TRAP {
                -header "Trap"
                -align center
                -expr {
                    $INSTRUCTION_TRAP ? "<TRAP>" : ""
                }
            }
            ERROR_MODE {
                -header "Error"
                -align center
                -expr {
                    $ERROR_MODE ? "<ERROR>" : ""
                }
            }
            MULTI_CYCLE {
                -header "Multi"
                -align center
                -expr {
                    $MULTI_CYCLE ? "<MULTI>" : ""
                }
            }
            BRANCH_TAKEN {
                -header "Branch taken"
                -align center
                -expr {
                    $BRANCH_TAKEN ? "<BRANCH>" : ""
                }
            }
            PREDICATE {
                -header "Predicate"
                -align center
                -expr {
                    $PREDICATE ? "<PRED>" : ""
                }
            }
        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

    } ; # namespace eval cli::History

    proc hist {args} {

        cli::History::ArgumentParser parseCallerArgs 1 $args

        cli::NoErrorStack {

            set contextId [cli::Target::getContextIdFromTargetVar target]
            set disassemble [expr { !$pc && ( $opcodes || !$dontDecode ) }]

            set hist [tcf::MyriadCore::getExecutionHistory $contextId $count $disassemble]

            tcf::fields::Using MyriadCore::HISTORY DISASSEMBLY OPCODES

            if {!$pc} {
                if {$opcodes && $dontDecode} {
                    cli::UpdateList hist item {
                        dict unset item $DISASSEMBLY
                    }
                } elseif {!$opcodes} {
                    cli::UpdateList hist item {
                        dict unset item $OPCODES
                    }
                }
                if {$quiet} {
                    set hist
                } else {
                    cli::History::Display display $hist
                }
            } else {
                cli::UpdateList hist item {
                    set item [tcf::fields::getNamedField MyriadCore::HISTORY $item PC]
                }
                if {$quiet} {
                    set hist
                } else {
                    foreach item $hist {
                        cli::Puts $item
                    }
                }
            }
        }
    }

    shell::autocomplete::addCommand hist cli::History::ArgumentParser autocomplete
    cli::Help::Manager add hist -parser cli::History::ArgumentParser

    namespace export hist

} ; # namespace eval mdbg