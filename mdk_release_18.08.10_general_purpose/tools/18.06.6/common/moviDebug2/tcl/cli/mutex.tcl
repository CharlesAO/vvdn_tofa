# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : mutex.tcl
# Description: mutex command
# Created on : Aug 23, 2016
# Author     : Luminita Daraban (luminita.daraban@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Mutex {

        namespace path {::mdbg}

        cli::TabularDisplay create Display {
            INDEX {
                -align right
            }
            USED {
                -align center
                -expr {
                    $USED ? "USED" : "FREE"
                }
            }
            OWNER {
                -align center
            }
        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

        cli::MultiCmdArgParser create ArgumentParser -brief "Hardware mutex operations." -description {

            Control over hardware mutexes.

            The following subcommands are supported:

            SUBCOMMAND                                  DESCRIPTION

            mutex status                                Display the status of all mutexes.
            mutex lock <mutexNumber> <core>             Lock a mutex.
            mutex unlock <mutexNumber> <core>           Unlock a mutex.

            *** EXAMPLES

            Show the current state of all mutexes:
            % mutex status

            Lock the mutex 0 on LOS:
            % mutex lock 0 LOS

            Unlock the mutex 0 on LOS:
            % mutex unlock 0 LOS

        } -auto mutexCmd {
            status {
                -brief "Get the status of mutex."
                -description {

                    Get the status of mutex.

                    *** EXAMPLES

                    Show the current state of all mutexes:
                    % mutex status
                }
                -args {
                    -quiet {
                        -flag
                        -brief "Don't display; return list of dictionaries."
                        -description {
                            Normally the subcommand displays a table to the standard output\
                            and will leave no result in the Tcl interpreter.
                            This flag inhibits the display and causes the command to return a result\
                            usable for further processing by Tcl scripts.
                        }
                    }
                }
            }
            lock {
                -brief "Lock mutex."
                -description {

                    Lock mutex.

                    *** EXAMPLES

                    Lock the mutex 0 through Shave 0:
                    % mutex lock 0 S0
                }
            }
            unlock {
                -brief "Unlock mutex."
                -description {

                    Unlock mutex.

                    *** EXAMPLES

                    Unlock the mutex 0 through Shave 0:
                    % mutex unlock 0 S0
                }
            }
            status {
                -args {
                    range {
                        -optional -regexp {^[0-9]+\.\.[0-9]+$}
                        -hidden
                    }
                    index {
                        -optional -regexp {^0|[1-9][0-9]*|0[xX][0-9A-Fa-f]+$}
                        -synopsis {?index[..index]?}
                        -brief "Mutex index or index range."
                        -var mutexNumber
                    }
                }
                -mutex {
                    {range index}
                }
            }
            lock - unlock {
                -args {
                    index {
                        -type integer
                        -var mutexNumber
                        -brief "Mutex index."
                    }
                }
            }
            lock - unlock {
                -args {
                    target {
                        -optional
                        -var target
                        -brief{
                            "Select target for mutex operation."
                        }
                        -description {
                            If not specified, uses the current target.
                        }
                    }
                }
                -arg-complete-scripts {
                    index {
                        mdbg::mutex::getValidMutexIndices $PREFIX*
                    }
                    target {
                        mdbg::mutex::getValidTargets $PREFIX*
                    }
                }
            }
            help {
            }
        } ; # cli::MultiCmdArgParser

        proc getObjectForCore {coreId} {
            set core [cli::Target::ctxId2target $coreId]
            dict get $::mdbg::mutex::CORE_TO_MUTEX_MAP $core
        }

    };  # namespace eval cli::Mutex

    proc mutex {args} {

        set result ""
        cli::Mutex::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            switch -exact $mutexCmd {
                lock - unlock {
                    set coreId [cli::Target::getContextIdFromTargetVar target]
                }
            }
            switch -exact $mutexCmd {
                status {
                    if {![info exists mutexNumber]} {
                        set itemList [mdbg::mutex::statusList range]
                    } else {
                        set item [mdbg::mutex::status $mutexNumber]
                        set itemList [list $item]
                    }
                    if {$quiet} {
                        if {[info exists item]} {
                            return $item
                        }
                        return $itemList
                    }
                    cli::Mutex::Display display $itemList
                }
                lock {
                    [cli::Mutex::getObjectForCore $coreId] lock $mutexNumber
                }
                unlock {
                    [cli::Mutex::getObjectForCore $coreId] unlock $mutexNumber
                }
                help {
                    help mutex
                    }
                default {
                    return -code error "\"$mutexCmd\" not implemented"
                }
            } ; # switch
        }
    }
    shell::autocomplete::addCommand mutex mdbg::cli::Mutex::ArgumentParser autocomplete
    cli::Help::Manager add mutex -parser mdbg::cli::Mutex::ArgumentParser
} ; # namespace eval mdbg

