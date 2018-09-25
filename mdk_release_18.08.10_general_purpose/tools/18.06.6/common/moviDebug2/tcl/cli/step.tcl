namespace eval mdbg {

    namespace eval cli::Step {

        namespace path {::mdbg}

        proc sync {id mode count timeout} {
            cli::Wait::contextSuspendedAfter $id $timeout {
                tcf::RunControl::resume $id $mode $count
            }
        }

        proc syncRange {id mode count low high timeout} {
            cli::Wait::contextSuspendedAfter $id $timeout {
                tcf::RunControl::resumeRange $id $mode $low $high $count
            }
        }

        proc async {id mode count} {
            tcf::RunControl::resume $id $mode $count
        }

        proc asyncRange {id mode count low high} {
            tcf::RunControl::resumeRange $id $mode $low $high $count
        }

        mdbg::cli::MultiCmdArgParser create ArgumentParser -brief "Step-by-step execution." -description {
            Executing the (current) target in a stepwise fashion.
            This can be done either synchronously or asynchronously.

            *** SUBCOMMANDS/ALIASES

            The following modes/aliases are supported:

            FULL FORM             ALIAS(ES)         DESCRIPTION

            step into -asm        stepasm           Execute a single assembly
                                  stepinstr         instruction.

            step into -line       stepline          Execute source code line.
                                                    Steps into function.

            step over -asm        stepoverasm       Execute single assembly
                                  stepoverinstr     instruction or function
                                                    call.
            step over -line       stepover          Execute source code line,
                                  stepoverline      including function call in
                                                    line.
            step over -range                        Executes code in range,
                                                    including function calls.
            step out              stepout           Steps out of current
                                                    function.
            step range                              Execute until control goes
                                                    out of specified range.
            *** EXAMPLES

            Executes an instruction on the Leon OS:
            % step into -asm -target LOS
            Alternatively:
            % target LOS
            % step

            Execute a source code line on the LeonOS (if line information is available):
            % step over -line -target LOS
            Alternatively:
            % target LOS
            % stepover

            Make the LOS step out of the current function:
            % step out -target LOS

        } -auto -common-pre {
            -args {
                -target {
                    -optional
                    -brief "Operate on specified target."
                    -description {
                        If missing, current target is used.
                    }
                    -complete-script {
                        mdbg::cli::Target::getValidTargets $PREFIX*
                    }
                }
                -count {
                    -type integer
                    -default 1
                    -brief "Repeat step `count` times."
                    -description {
                        If specified, the step is performed in the background `count` number of times.
                        The target is not reported as being stopped until all the steps are performed.

                        If missing, the step is performed only once.
                    }
                }
                -timeout {
                    -optional
                    -type integer
                    -brief "Timeout (ms)."
                    -description {
                        Raises an error if the stepping operation does not finish in time.
                        Incompatible with "-async" option.
                    }
                }
                -async {
                    -flag
                    -brief "Return immediately."
                    -description {
                        Bypasses synchronisation with target state changes.
                        Incompatible with "-timeout" argument.
                    }
                }
                -quiet {
                    -flag
                    -brief "Quiet"
                    -description {
                        Does not try to display source information after
                        completing high level step.
                    }
                }
            }
            -mutex {
                {-async -timeout}
            }
        } -default {into} stepCommand {
            into {
                -brief "Step into."
                -description {
                    Stepwise execution of target, entering called function(s).
                }
                -args {
                    -asm {
                        -flag
                        -brief "Instruction step."
                        -description {
                            This is the default mode of operation.
                            Incompatible with "-line".
                        }
                    }
                    -line {
                        -flag
                        -brief "Source line step."
                        -description {
                            Incompatible with "-asm".
                        }
                    }
                }
                -mutex {
                    {-asm -line}
                }
            }
            out - return {
                -brief "Step out."
                -description {
                    Resumes execution until control reaches to the point\
                    where the current function has been called.

                    *** NOTES

                    - For correct behaviour of this function, the debugger will try to employ\
                      hardware breakpoints in the target.
                    - If there are no more hardware breakpoints available it will try to do the\
                      stepping using software breakpoints. This might cause unexpected behaviour\
                      if the return instruction is in the core's instruction pipeline.

                    !!! Make sure you have a free hardware breakpoint in your SHAVE before stepping out.
                }
            }
            over {
                -brief "Step over."
                -description {
                    Stepwise execution, not entering function calls.
                }
                -args {
                    -asm {
                        -flag
                        -brief "Step over instruction."
                        -description {
                            Runs the target to the next instruction.
                            Function calls are considered to be the same instruction.

                            Incompatible with "-line" and "-range".
                        }
                    }
                    -line {
                        -flag
                        -brief "Step over source line."
                        -description {
                            This is the default mode of operation.

                            Runs the target to the next source line.
                            Function calls are considered to be the same line.

                            Incompatible with "-asm" and "-range".
                        }
                    }
                    -range {
                        -flag
                        -requires low
                        -requires high
                        -synopsis {
                            ?-range low high?
                        }
                        -brief "Range step."
                        -description {
                            Runs the target until control goes out of the specified range.
                            The range is defined from `low` to `high - 1`.

                            A function call within the range is considered to be part of the range.

                            Incompatible with "-asm" and "-line" options.
                        }
                    }
                    low {-optional -requires {-range} -type integer -hidden}
                    high {-optional -requires {-range} -type integer -hidden}
                }
                -mutex {
                    {-asm -line -range}
                }
            }
            range {
                -brief "Step within range."
                -description {
                    Resumes execution of target until control reaches out of the given range, for any reason.
                    The range starts from the `low` address and ends just before the `high` address.
                }
                -args {
                    low {
                        -type integer
                        -brief "Low PC address."
                    }
                    high {
                        -type integer
                        -brief "High PC address."
                    }
                }
            }
        }

        proc showCurrentLine {id mode} {
            set pc [tcf::getContextPC $id]
            switch -exact $mode {
                STEP_INSTR -
                STEP_OVER_INSTR {
                    cli::Dasm::showDasmLine $id $pc
                }
                default {
                    cli::LineNumbers::showSourceLine $id $pc
                }
            }
        }

    } ; # namespace eval cli::Step

    proc step {args} {
        cli::NoErrorStack {
            cli::Step::ArgumentParser parseCallerArgs 1 $args

            # -common-pre args
            set id [cli::Target::getContextIdFromTargetVar target]
            set OP [expr {$async ? "async" : "sync"}]
            set stepArgs [list]

            switch -exact $stepCommand {
                into {
                    set mode [expr {$line ? "STEP_INTO_LINE" : "STEP_INSTR"}]
                }
                over {
                    if {$range} {
                        append OP Range
                        set mode "STEP_OVER_RANGE"
                        lappend stepArgs $low $high
                    } else {
                        set mode [expr {$asm ? "STEP_OVER_INSTR" : "STEP_OVER_LINE"}]
                    }
                }
                out {
                    set mode "STEP_OUT"
                }
                return {
                    set mode "STEP_RETURN"
                }
                range {
                    append OP Range
                    set mode "UNTIL_OUT_OF_RANGE"
                    lappend stepArgs $low $high
                }
                default {
                    return -code error "\"step $stepCommand\" not implemented!"
                }
            }
            if {!$async} {
                if {![info exists timeout]} {
                    set timeout {}
                }
                lappend stepArgs $timeout
            }

            cli::Step::$OP $id $mode $count {*}$stepArgs

            cli::CatchAndPrintError {
                if {!$quiet && !$async} {
                    cli::Step::showCurrentLine $id $mode
                }
            }

        }
    }

    shell::autocomplete::addCommand step cli::Step::ArgumentParser autocomplete
    cli::Help::Manager add step -parser cli::Step::ArgumentParser

    cli::Alias stepasm       step into -asm
    cli::Alias stepline      step into -line
    cli::Alias stepoverasm   step over -asm
    cli::Alias stepoverinstr stepoverasm
    cli::Alias stepinstr     stepasm
    cli::Alias stepoverline  step over -line
    cli::Alias stepover      stepoverline
    cli::Alias stepout       step out

    namespace export step

} ; # namespace eval mdbg

