namespace eval mdbg {

    namespace eval cli::Platform {
        namespace path ::mdbg

        cli::ArgumentParser create BoardResetArgParser {
            -auto
            -brief "Board reset."
            -description {
                The command will try to reset the board to its initial state.

                - If TCF is available, the corresponding TCF Platform service command will be called.
                  It waits then for the main core of the platform to be suspended.

                - If TCF is not available, a hand-coded sequence will be executed to reset the board.

                *** NOTES

                - Resetting the board is done differently for moviSim and moviDebugServer.

                *** EXAMPLES

                Reset the board:
                % breset
            }

            -args {
               -timeout {
                   -optional
                   -type integer
                   -default {}
                   -brief "Timeout (ms)."
                   -description {
                       Raise error if main core does not enter into valid suspended state in the specified amount of time.
                       The command will wait forever if this argument is not specified.
                       Useful for scripting.
                   }
               }
               -async {
                   -flag
                   -brief "Asynchronous operation"
                   -description {
                       Don't wait for the main core.
                   }
               }
               -direct {
                   -flag
                   -disabled
               }
            }
            -mutex {
                {-timeout -async -direct}
            }
        }

        proc cleanupBeforeBoardReset {event context args} {
            #cli::PutDebug "Before board reset: $context"
            uart::stop
            # TODO: only clear the read/write functions of pipes
            pipe reset
        }

        proc setupAfterBoardReset {event context args} {
            #cli::PutDebug "After board reset: $context"
            uart::AUTO_INIT
        }

        proc changeWorkDir {event context dir} {
            cli::PutInfo "Changing working directory to \"$dir\""
            if {[catch {cd $dir} error]} {
                cli::PutInfo "Error: $error"
            }
        }

        if {[namespace exists ::tcf::Platform]} {
            ::tcf::Platform::SyncEvents::beforeBoardReset+ cleanupBeforeBoardReset
            ::tcf::Platform::SyncEvents::boardReset+ setupAfterBoardReset
            ::tcf::Platform::SyncEvents::changeWorkDir+ changeWorkDir
        }

    } ; # namespace eval cli::Platform

    proc breset {args} {

        cli::Platform::BoardResetArgParser parseCallerArgs 1 $args

        cli::NoErrorStack {

            if {[namespace exists ::tcf::Platform]} {
                # Try to do the right thing
                set rootContext [cli::Target::getRootContext]
                tcf::protocol::blockEvents {
                    tcf::Platform::boardReset $rootContext
                    if {!$async} {
                        set startupCore [tcf::Platform::getMainCore $rootContext]
                        set defaultMainCore [tcf::Platform::getDefaultMainCore $rootContext]
                    }
                }
                if {!$async} {
                    if {$startupCore != ""} {
                        set ::mdbg::TARGET $startupCore
                    } else {
                        set ::mdbg::TARGET $rootContext
                    }
                    if {$defaultMainCore != ""} {
                        cli::Wait::contextSuspended $defaultMainCore $timeout
                    }
                    update ; # flush out pending events
                }
            } else {
                uart::stop
                pipe reset
                # Try harder
                directBoardReset
                # Reinit + start UART
                uart::AUTO_INIT
            }
        }
        return
    }

    proc ddrinit {} {
        cli::NoErrorStack {
            tcf::Platform::ddrInit [cli::Target::getRootContext]
        }
    }

    cli::Help::Manager add breset -parser cli::Platform::BoardResetArgParser
    shell::autocomplete::addCommand breset cli::Platform::BoardResetArgParser autocomplete

    cli::Help::Manager add ddrinit -short "Initialise DDR." -synopsis {
        ddrinit
    } -long {
        The command uses the TCF Platform service's ddrInit command to initialise the\
        DDR component of the target.

        *** NOTES

        - DDR init is not required for moviSim.
        - access to uninitialised DDR addresses can cause the target's bus(ses) to lock up,\
          causing undefined behaviour (JTAG FlowControlError and such).
        - DDR initialisation is also carried out automatically when loading ELF files having\
          code/data/BSS sections in the DDR.
        - it is *not* recommended to initialise the DDR *after* having loaded an ELF executable on the target.
        - DDR initialisation is done by loading and executing a separate, platform-dependent ELF.
          These files can be found in the "common/moviDebug/ddrinit" directory of the moviTools distribution.
          The DDR initialisation file can be custom overridden by a command-line switch of the moviDebug2 executable.

        *** EXAMPLES

        Initialise the DDR:
        % ddrinit
    }

    proc startupcore {{core {}}} {
        cli::NoErrorStack {
            set root [cli::Target::getRootContext]
            if {$core == {}} {
                cli::Target::getTargetNameFromContextId [tcf::Platform::getMainCore $root]
            } else {
                set contextId [cli::Target::getContextIdFromTarget $core]
                tcf::Platform::setMainCore $root $contextId
            }
        }
        # target $core
    }

    proc getexitcode {} {
        cli::NoErrorStack {
            set root [cli::Target::getRootContext]
            set contextId [tcf::Platform::getMainCore $root]
            set exitCode [tcf::MyriadCore::getExitCode $contextId]
        }
        # target $core
    }

    proc getprogramcounter {{target {}}} {
        cli::DeprecatedWarning "state -pc"
        cli::NoErrorStack {
            set contextId [cli::Target::getContextIdFromTargetVar target]
            ::tcf::getContextPC $contextId
        }
    }

    cli::Help::Manager add startupcore -short "Set/get the startup core for subsequent load/run operations" -synopsis {
        startupcore core
    } -long {
        Set/get the main core for subsequent load/run operations.

        - This will determine the core which will receive the elf file's entry point.
        - The "run" command is equivalent to "cont [startupcore]".

        *** EXAMPLES

        Set Leon RT to be the main core:
        % startupcore LRT
    }

    shell::autocomplete::addScript {
        startupcore
        programcounter
    } {
        lsort -dictionary [cli::Target::getValidTargets $PREFIX*]
    }

    cli::Help::Manager add getexitcode -short "Gets the exit code for the application" -synopsis {
        getexitcode
    } -long {
        The command gets the exit code for the application.

        It works if current target is a Leon Core.

        *** ALIASES

        - getpc
        - getip
        - programcounter
    }

    cli::Help::Manager add getprogramcounter -synopsis {
        getprogramcounter ?core?
    } -short "Get Program Counter." -long {
        Get the Program Counter's value for target (a.k.a Instruction Pointer).
        Target should be a Myriad Core.

        *** NOTES

        !!! This command is deprecated.
        !!! Use "state -pc" instead.

        *** EXAMPLES

        Return the value of PC register of Leon OS:
        % getprogramcounter LOS

        Return the value of IP register of Shave 0:
        % getprogramcounter S0

    } -args {
        core {
            -synopsis core
            -short "Target (core)."
            -long {
                If missing, the current target is used.
            }
        }
    }

    cli::Alias {getpc getip programcounter} getprogramcounter

    namespace export getprogramcounter

} ; # namespace eval mdbg

