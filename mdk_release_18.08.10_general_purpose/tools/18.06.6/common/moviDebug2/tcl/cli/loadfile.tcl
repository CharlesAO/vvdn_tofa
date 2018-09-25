# **************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/load.tcl
# Description: Load command + variants
# Created on : Aug 19, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::LoadFile {

        mdbg::cli::ArgumentParser create ArgumentParser {
            -auto
            -brief {
                File load/verify/run operations.
            }
            -description {
                Allows loading files into the target platform.

                The loaded ELF file can also be started right away\
                and the command can be run synchronously or asynchronously.

                - Synchronous operation (default):
                  The command will not normally exit until the target suspends/starts execution.
                - Asynchronous operation ("-async" flag):
                  the command returns immediately without waiting for its effect to become active.

                *** NOTES

                - Currenty only ELF and BIN loading is implemented.
                - Loading always happens using JTAG interface, cache coherency is not managed.
                - The startup core needs to be specified before ELF loading, because the entry point is\
                set during this command.
                - Main startup core get started up / reset in the Myriad2 TCF model automatically.
                - If the ELF contains Shave window address settings, the corresponding shaves are started up.
                - Breakpoints always get reevaluated when the symbol information becomes (un)available.
                - "loadfile -run" is implemented more efficiently than "load" followed by "run".

                *** ALIASES

                ALIAS            COMMAND
                loadelf          loadfile -elf
                loadandrun       loadfile -elf -run
                verify           loadfile -verify -noload
                loadandverify    loadfile -verify
                loadsym          loadfile -elf -symbols-only

                *** EXAMPLES

                Load my ELF file:
                % loadfile myElfFile.elf
                % loadfile d:/mdk/examples/HowTo/Cpp/cppshave/output/cppshave.elf
                % loadfile {d:\mdk\examples\HowTo\Cpp\cppshave\output\cppshave.elf}

                Load and verify my ELF file:
                % loadfile -verify output/myElfFile.elf

                Load binary data to address 0x70000000:
                % loadfile -address 0x70000000 myFile.bin

                Reset board, initialise DDR, load myElfFile.elf, verify, run, wait for stop:
                % loadfile -elf -breset -ddrinit -verify -run -wait myElfFile.elf

                Board Reset, DDR Init, Load, Run, Wait:
                % loadelf -BDRW myElfFile.elf

                Only load symbols information and memory maps from myElfFile.elf:
                % loadfile -elf -symbols-only myElfFile.elf
            }
            -args {
                -elf {
                    -flag
                    -var fileIsElf
                    -brief "Treat `fileNameOrChannel` as name of ELF file."
                    -description {
                        Normally the file type can be detected from its extension.
                        This switch also enables a lot of extra flags:
                        "-breset", "-ddrinit", "-runw", "-[B[D]][R[W]]"
                    }
                }
                -verify {
                    -flag
                    -brief "Verify loaded bytes."
                    -description {
                        Verifies loaded data. Returns error on failure
                    }
                }
                -symbols-only {
                    -flag
                    -var symbolsOnly
                    -brief "Only load symbol information."
                    -description {
                        This switch allows the user to load symbol information for the target,\
                        without loading the program/data bytes of an ELF file, effectively attaching\
                        to a running application.
                        Incompatible with any file type other than ELF.
                    }
                }
                -startupcore {
                    -optional
                    -var startupCore
                    -brief "Set startup core."
                    -description {
                        Select a core name to use as startup for Elf file
                        Incompatible with any file type other than ELF.
                    }
                    -complete-script {
                        lsort -dictionary [cli::Target::getValidTargets $PREFIX*]
                    }
                }
                -quiet {
                    -flag
                    -brief "Don't display section names during load"
                    -description {
                        Supresses messages during load.
                    }
                }
                -binary {
                    -flag
                    -var fileIsBin
                    -requires {-address}
                    -brief "Treat `fileNameOrChannel` as name of file containing raw binary data."
                    -description {
                        Requires "-address" to be specified.
                        Incompatible with other file type switches: "-elf", "-hex", "-chan".
                    }
                }
                -hex {
                    -flag
                    -var fileIsHex
                    -brief "Treat `fileNameOrChannel` as name of file containing HEX data."
                    -description {
                        HEX format is Movidius-specific.
                        !! NOT IMPLEMENTED !!
                        Incompatible with other file type switches: "-elf", "-binary", "-chan".
                    }
                }
                -chan {
                    -flag
                    -var fileIsChan -requires {-address}
                    -brief "Treat `fileNameOrChannel` as name of Tcl channel."
                    -description {
                        !! NOT IMPLEMENTED !!
                        Allows loading binary data directly from open Tcl channel.\
                        (e.g. pipe, socket)
                        Incompatible with other file type switches: "-elf", "-binary", "-hex".
                    }
                }
                -address {
                    -optional
                    -var baseAddress
                    -brief "Specify base address for loading file data."
                    -description {
                        Address, symbol or register name.
                        Especially for "-binary" and "-channel" types.

                        It's also theoretically possible to load/reloc ELF .so files at `baseAddress`.\
                        This is not supported currently.
                    }
                    -complete-script {
                        concat [mdbg::findReg $PREFIX*] [cli::Symbols::getElfSymbols name [targetid] $PREFIX*]
                    }
                }
                -noload {
                    -flag
                    -var noLoad
                    -requires {-verify}
                    -brief "Bypass loading of bytes on verify."
                    -description {
                        Allows verification of bytes without loading the data first.
                        Requires "-verify" flag.
                    }
                }
                -size {
                    -optional
                    -var loadSize
                    -type integer
                    -requires {-chan -binary}
                    -brief "Override size of loaded data."
                    -description {
                        Allows overriding the size of raw bytes loaded.
                        Normally the whole file/channel data is loaded (read until EOF).
                        Requires "-chan" or "-binary" flag to be specified.
                    }
                    -hidden
                }
                -offset {
                    -optional
                    -var fileOffset
                    -type integer
                    -requires {-chan -binary}
                    -brief "Override source offset of loaded data."
                    -description {
                        Allows overriding the size of raw bytes loaded.
                        Normally data is read from beginning of file or current read offset of channel.
                        This switch will cause a (forward) seek in the source channel (if possible) or skips the number of\
                        bytes equal to offset. Negative numbers should cause a negative seek if possible on the channel.
                        Requires "-chan" or "-binary" flag to be specified.
                    }
                    -hidden
                }
                -async {
                    -flag
                    -brief "Asynchronous operation."
                    -description {
                        For ELF file type, the load/-run can be done asynchronously,\
                        that means that the command will return immediately, not waiting for\
                        the target's state change.
                        Incompatible with synchronisation switches "-wait", "-runw", "-timeout".
                        Incompatible with "-elf" flags "-breset", "-ddrinit", "-[B[D]][R[W]]".
                    }
                }
                -breset {
                    -flag
                    -requires {-elf}
                    -brief "Reset board before load."
                    -description {
                        Issues "breset" command before loading.
                        Requires "-elf" switch or "loadelf" alias.
                    }
                }
                -ddrinit {
                    -flag
                    -requires {-elf}
                    -brief "Initialise DDR before load."
                    -description {
                        Issues "ddrinit" command before loading.
                        Requires "-elf" switch or "loadelf" alias.
                    }
                }
                -run {
                    -flag
                    -brief "Run loaded elf."
                    -description {
                        Works even without specifying "-elf" flag.
                        Ignored if file name not recognized as ELF.
                        Synchronous by default. Can be executed asynchronously.
                        Incompatible with other file type switches: "-binary", "-hex", "-chan".
                    }
                }
                -wait {
                    -flag
                    -require {-run}
                    -brief "Wait for the application to suspend."
                    -description {
                        The command will not return until the application stops for any reason.
                        Requires "-run" flag.
                    }
                }
                -runw {
                    -flag
                    -requires {-elf}
                    -brief "-run -wait"
                    -hidden
                }

                -stopatmain {
                    -flag
                    -description {
                        Works even without specifying "-elf" flag.
                        For a POSIX RTEMS application the "main" function is the "POSIX_Init" function of the startup core.
                        For non-POSIX RTEMS application the "main" function is the "Init" function of the startup core.
                     }
                    -requires {-run -runw -BDR -BDRW -BR -BRW -R -RW}
                    -brief "Stop at main function"
                }

                -timeout {
                    -default {}
                    -brief "Timeout (ms)."
                    -description {
                        Raises error if command does not return normally in the specified number of milliseconds.
                        Incompatible with "-async".
                    }
                }

                {-B -BD -BDR -BDRS        -BDRW -BR -BRS -BRSW -BRW -R -RS -RSW -RW} {-flag -requires {-elf} -hidden}
                {                  -BDRSW                                          } {
                    -flag -requires {-elf}
                    -synopsis {
                        ?-[B[D]][R[S][W]]?
                    }
                    -brief "[-breset [-ddrinit]] [-run [-stopatmain] [-wait]]"
                    -description {
                        Short versions of long options.

                        Not all combinations are allowed and the order of the characters is important.

                        Short and long version of same option cannot be both specified.
                        Incompatible with same other options as long version.
                    }
                }

                fileNameOrChannel {
                    -var name
                    -brief "File name or channel."
                    -description {
                        Specifies a file name or a Tcl channel name.
                        This is a mandatory argument.
                        What it means depends on the presence of "-elf", "-binary", "-hex", "-chan" flags.
                        If none of these are present, the CLI will try to determine the meaning of the name.

                        NOTE: There is no default case for this detection logic.
                    }
                    -complete-script {
                        # TODO move to proc

                        set ACNS ::mdbg::shell::autocomplete
                        set ListOfFiles ${ACNS}::listOfFiles
                        set AppendList ${ACNS}::appendList

                        set elf [expr {"-elf" in $PARAMS}]
                        set bin [expr {"-binary" in $PARAMS}]
                        set hex [expr {"-hex" in $PARAMS}]
                        set chan [expr {"-chan" in $PARAMS}]

                        set auto [expr {!$elf && !$bin && !$hex && !$chan}]

                        set result [list]

                        if {$auto || $elf} {
                            $AppendList result [$ListOfFiles -types f -- $PREFIX*.elf]
                        }
                        if {$auto || $bin} {
                            $AppendList result [$ListOfFiles -types f -- $PREFIX*.bin]
                        }
                        if {$auto || $hex} {
                            $AppendList result [$ListOfFiles -types f -- $PREFIX*.hex]
                        }
                        if {$auto || $chan} {
                            $AppendList result [chan names $PREFIX*]
                        }
                        $AppendList result [$ListOfFiles -types d -- $PREFIX*]
                    }
                }
            }
            -mutex {
                { {-elf -run -runw -symbols-only -startupcore} -binary -hex -chan}
                { -elf { -address -size -offset} }
                { -symbols-only {-breset -run -runw} }
                { -async {-timeout -wait -runw} }
                { -runw {-run -wait} }
            }
            -comment {
               #  -B -BD -BDR -BDRS -BDRSW -BDRW -BR -BRS -BRSW -BRW -R -RS -RSW -RW
            }
            -mutex {
                { -B -BD -BDR -BDRS -BDRSW -BDRW -BR -BRS -BRSW -BRW                  { -breset                                                 } }
                {    -BD -BDR -BDRS -BDRSW -BDRW                                      { -ddrinit                                                } }
                {        -BDR -BDRS -BDRSW -BDRW -BR -BRS -BRSW -BRW -R -RS -RSW -RW  { -run     -runw              -symbols-only               } }
                {                   -BDRSW -BDRW          -BRSW -BRW        -RSW -RW  { -wait    -runw              -async                      } }
                {             -BDRS -BDRSW           -BRS -BRSW         -RS -RSW      {                 -stopatmain                             } }
            }
        }

        oo::objdefine ArgumentParser {
            method detectFileType { {level 0} } {
                incr level
                uplevel $level {
                    set $fileIsChan [info exists channel]
                    set fileIsAuto [expr {!$fileIsChan && !$fileIsElf && !$fileIsBin && !$fileIsHex}]
                    if {$fileIsAuto} {
                        switch -glob $name {
                            *.elf {
                                set fileIsAuto 0
                                set fileIsElf 1
                            }
                            *.bin {
                                set fileIsAuto 0
                                set fileIsBin 1
                            }
                            *.hex {
                                set fileIsAuto 0
                                set fileIsHex 1
                            }
                            default {
                                set fileIsChan [expr {$name in [chan names]}]
                                set fileIsAuto [expr {!$fileIsChan}]
                            }
                        }
                    }
                    set fileType [expr {
                       $fileIsElf ? "ELF" :
                       $fileIsBin ? "BIN" :
                       $fileIsHex ? "HEX" :
                       $fileIsChan ? "CHAN" :
                       "AUTO"
                    } ];

                } ; # uplevel $level
            }

            method setupElfArgs { {level 0} } {
                incr level

                my SetFlagIf $level breset            B BD BDR BDRS BDRSW BDRW BR BRS BRSW BRW
                my SetFlagIf $level ddrinit             BD BDR BDRS BDRSW BDRW
                my SetFlagIf $level run         runw       BDR BDRS BDRSW BDRW BR BRS BRSW BRW R RS RSW RW
                my SetFlagIf $level wait        runw                BDRSW BDRW        BRSW BRW      RSW RW
                my SetFlagIf $level stopatmain                 BDRS BDRSW         BRS BRSW       RS RSW

                uplevel $level {
                    unset -nocomplain --        runw  B BD BDR BDRS BDRSW BDRW BR BRS BRSW BRW R RS RSW RW
                }
            }

            #
            #  Private
            #
            method SetFlagIf {level destFlagVar args} {
                incr level
                upvar $level $destFlagVar destFlag
                foreach srcFlagVar $args {
                    upvar $level $srcFlagVar srcFlag
                    if {$srcFlag} {
                        set destFlag 1
                        break
                    }
                }
            }

        }  ; # oo::objdefine ArgumentParser

        proc checkValidFile {fileName} {
            if {![file exists $fileName]} {
                return -code error "\"$fileName\" does not exist"
            }
            switch -exact [file type $fileName] {
                file {
                }
                link {
                    set linkTarget [file readlink $fileName]
                    if {![file isfile $linkTarget]} {
                        return -code error "\"$fileName\" -> \"$linkTarget\" is not a file"
                    }
                }
                directory {
                    return -code error "\"$fileName\" is a directory"
                }
                default {
                    return -code error "\"$fileName\" is not a regular file nor symbolic link to a file"
                }
            }
            #if {![file readable $fileName]} {
            #    return -code error "\"$fileName\" is not readable"
            #}

            return $fileName
        }

    } ; # namespace eval LoadFile

    proc loadfile {args} {
        cli::LoadFile::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            cli::LoadFile::ArgumentParser detectFileType

            switch -exact $fileType {
                ELF {
                    # TODO: this should really be handled by the C++ side
                    #       but it's not -- so we provide some safeguards
                    cli::LoadFile::checkValidFile $name
                    cli::LoadFile::ArgumentParser setupElfArgs

                    if {$breset} {
                        if {!$quiet} {
                            cli::Verbose "Resetting board (-breset provided)"
                        }
                        if {$timeout != {}} {
                            breset -timeout $timeout
                        } else {
                            breset
                        }
                    }

                    if {$ddrinit} {
                        if {!$quiet} {
                            cli::Verbose "Initialising DDR (-ddrinit provided)"
                        }
                        ddrinit
                    }

                    if {$noLoad} {
                        set rootContext [cli::Target::getRootContext]
                        return [::tcf::Platform::verify $rootContext $name \
                            [dict create \
                                Quiet [json::Boolean $quiet] \
                            ] \
                        ]
                    }

                    if {[info exists startupCore]} {
                        set mainCore [cli::Target::getContextIdFromTarget $startupCore]
                        set StartupCore_ArgList [list StartupCore [json::String $mainCore]]
                    } else {
                        set StartupCore_ArgList [list]
                    }

                    set result [::tcf::ProcessesV1::start $name {} {} {} \
                        [dict create \
                            Attach true \
                            StopAtEntry [json::Boolean [expr {! $run }]] \
                            StopAtMain [json::Boolean $stopatmain] \
                            Verify [json::Boolean $verify] \
                            SymbolsOnly [json::Boolean $symbolsOnly] \
                            BoardReset false \
                            Quiet [json::Boolean $quiet] \
                            {*}$StartupCore_ArgList \
                        ]
                    ]
                    set rootContext [tcf::getID $result]
                    if {![info exists mainCore]} {
                        set mainCore [tcf::Platform::getMainCore $rootContext]
                    }
                    if {$mainCore != {}} {
                        set mdbg::TARGET $mainCore
                        if {!$async && !$symbolsOnly} {
                            cli::TrapInterrupt {
                                if {$run} {
                                    if {$wait} {
                                        cli::Wait::contextSuspendedOrAnyBreakpoint $mainCore $timeout
                                    } else {
                                        cli::Wait::contextRunningOrSuspended $mainCore $timeout
                                    }
                                } else {
                                    cli::Wait::contextSuspended $mainCore $timeout
                                }
                            }
                        }
                    } else {
                        set mdbg::TARGET $rootContext
                    }
                }
                BIN {
                    if {$run} {
                        error "Running binary data is not possible!"
                    }
                    if {[info exists baseAddress]} {
                        cli::LoadFile::checkValidFile $name
                        set baseAddress [cli::Symbols::getAddress $baseAddress]
                        set infile [open $name rb]
                        if {[info exists fileOffset]} {
                            seek $infile $fileOffset
                        }
                        try {
                            if {[info exists loadSize]} {
                                set buffer [read $infile $loadSize]
                            } else {
                                set fsize [file size $name]
                                set buffer [read $infile $fsize]
                            }
                            if {!$noLoad} {
                                jtag writeBlock $baseAddress $buffer
                            }
                            if {$verify} {
                                set memData [jtag readBlock $baseAddress $fsize]
                                if { $buffer != $memData} {
                                    error "File not loaded correctly."
                                } else {
                                    if {!$quiet} {
                                        cli::PutInfo "File loaded correctly."
                                    }
                                }
                            }
                        } finally {
                            close $infile
                        }
                    } else {
                        error "-address is mandatory!"
                    }

                }
                HEX {
                    if {$run} {
                        error "Running hex files is not possible!"
                    }
                    error "Loading hex files is not implemented".
                }
                CHAN {
                    if {$run} {
                        error "Running binary data from Tcl channels is not possible!"
                    }
                    error "Loading from Tcl channel is not implemented".
                }
                default {
                    error "Unsupported file type: `$name`. Must be \"*.elf\", \"*.bin\", \"*.hex\" or Tcl channel"
                }
            }
        }
    }

    shell::autocomplete::addCommand loadfile cli::LoadFile::ArgumentParser autocomplete

    cli::Help::Manager add loadfile  -parser cli::LoadFile::ArgumentParser

    cli::Alias loadelf       loadfile -elf
    cli::Alias loadandrun    loadelf -run
    cli::Alias verify        loadfile -verify -noload
    cli::Alias loadandverify loadfile -verify
    cli::Alias loadsym       loadfile -elf -symbols-only

    namespace export loadfile loadelf loadsym

} ; # namespace eval mdbg

