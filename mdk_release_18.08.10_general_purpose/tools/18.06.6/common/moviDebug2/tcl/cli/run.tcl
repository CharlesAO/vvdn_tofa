# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : run.tcl
# Description: moviDebug2 CLI `run` command
# Created on : Aug 27, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Run {

        namespace path {::mdbg}

        proc targetID {} {
            tcf::Platform::getMainCore [cli::Target::getRootContext]
        }

        cli::ArgumentParser create ArgumentParser {
            -brief "Run application."
            -description {
                Starts execution of the loaded application.

                This can be performed either synchronously or asynchronously.
                - The synchronous mode -- enabled by default -- permits waiting for the application to be stopped,\
                using a the "-wait" flag.

                A timeout can be provided to give a time limit to the waiting mechanism.
                Exceeding the time limit will raise an error.

                *** NOTES

                - The command is semantically equivalent to the following:
                % cont [startupcore]

                - This means that issuing the command twice -- without "loadfile" -- will NOT run the \
                application again from the beginning.

                - In interactive mode the command is also interruptible by the Control-C key combination.

                *** ALIASES

                ALIAS           FULL FORM
                runw            run -wait
                runandwait      run -wait

                *** EXAMPLES

                Run application (wait until the core starts running):
                % run

                Run application, wait until execution stops:
                % runw

            }
            -auto
            -args {
                -wait {
                    -flag
                    -brief "Wait for application to stop."
                    -description {
                        This flag will cause the command to wait until the application stops.
                        The causes for stopping can be:
                        - application termination
                        - breakpoint was encountered on any other core.

                        Incompatible with the "-async" option.
                    }
                }
                -async {
                    -flag
                    -brief "Return immediately."
                    -description {
                        Suppresses the default wait mechanism.
                        Incompatible with the "-wait" and "-timeout" options.
                    }
                }
                -timeout {
                    -type integer
                    -default {}
                    -brief "Raise error on timeout (ms)."
                    -description {
                        Causes the command to raise an error if it does not return normally\
                        after the specified number of milliseconds.
                        This timepout is also valid when the "-wait" option is not provided.
                        Incompatible with the "-async" option.
                    }
                }
            }
            -mutex {
                {-async { -timeout -wait } }
            }
        }

    } ; # namespace eval cli::Run

    proc run {args} {

        cli::NoErrorStack {

            cli::Run::ArgumentParser parseCallerArgs 1 $args

            set id [cli::Run::targetID]

            if {$async} {
                cli::Continue::async $id
            } else {
                cli::TrapInterrupt {
                    if {$wait} {
                        cli::Continue::wait $id $timeout
                    } else {
                        cli::Continue::sync $id $timeout
                    }
                }
            }
        }
    }

    shell::autocomplete::addCommand run cli::Run::ArgumentParser autocomplete
    cli::Help::Manager add run -parser cli::Run::ArgumentParser

    cli::Alias {
        runw
        runandwait
    } run -wait

    namespace export run runw

} ; # namespace eval mdbg

