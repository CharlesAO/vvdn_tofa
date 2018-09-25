# **************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/savefile.tcl
# Description: Save command
# Created on : Oct 29, 2015
# Author     : Ancuta Maria IVASCU (ancuta.ivascu@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::SaveFile {

        mdbg::cli::ArgumentParser create ArgumentParser {
            -auto
            -brief {
                Save memory to file.
            }
            -description {
                Saves a memory region to file.

                *** NOTES

                - To achieve top performance, data is normally saved using the direct JTAG interface.\
                Coherency is not managed.
                - In case when cache coherency is an issue, the "-mem" option forces the operation to be\
                executed using the cache-aware memory model of a specific target.

                !!! Warning: Accessing large amounts of data through the Leon/Shave TCF memory model is very slow!

                *** EXAMPLES

                Save 100 bytes from 0x70000000 in myFile.txt:
                % savefile myFile.txt 0x70000000 100

                Append 1 KiB of data from 0x70000000 to myFile.txt:
                % savefile -append myFile.txt 0x70000000 1024

                Save some uncached data from the DDR (this is actually not so slow):
                % savefile -mem A uncached_ddr.txt DDR_BASE_ADR [expr 64 * 1024]

                Save the contents of the DDR as Shave 0 sees it through the Level 1 and Level 2 caches (reeealy slow):
                % savefile -mem S0 myFile.txt DDR_BASE_ADR [expr 64 * 1024]

                Save the contents of the myGlobalBuffer symbol to disk (auto-detected size):
                % savefile myFile.txt myGlobalBuffer

            }
            -args {
                -append {
                    -flag
                    -var appendVar
                    -brief "Writes to the end of the file."
                    -description {
                    }
                }
                -mem {
                    -brief "Read data using TCF"
                    -var memTarget
                    -description {
                        Use the TCF memory interface of "memTarget".
                        Might get really slow for larger datasets.
                    }
                    -optional
                }
                -hex {
                    -flag
                    -var fileIsHex
                    -brief "Save to HEX file named `fileNameOrChannel`"
                    -description {
                        HEX format is Movidius-specific.
                        !! NOT IMPLEMENTED !!
                        Incompatible with other file type switches: "-chan".
                    }
                }
                -chan {
                    -flag
                    -var fileIsChan
                    -brief "Treat `fileNameOrChannel` as name of Tcl channel."
                    -description {
                        Allows saving binary data directly to open Tcl channel.\
                        (e.g. Tcl pipe, socket)
                        Incompatible with other file type switches: "-hex".
                    }
                }
                fileNameOrChannel {
                    -var name
                    -brief "File name or channel."
                    -description {
                        Specifies a file name or a Tcl channel name.
                        This is a mandatory argument.
                    }
                    -complete-script {
                        set ACNS ::mdbg::shell::autocomplete
                        set ListOfFiles ${ACNS}::listOfFiles
                        set AppendList ${ACNS}::appendList
                        set result [list]
                        if {"-chan" in $PARAMS} {
                            $AppendList result [chan names $PREFIX*]
                        } else {
                            $AppendList result [$ListOfFiles -types d -- $PREFIX*]
                        }
                    }
                }
                address {
                    -var symNameOrAddress
                    -brief "Address to save file data from."
                    -description {
                        Address, symbol or register name.
                        This is a mandatory argument.
                    }
                    -complete-script {
                        concat [mdbg::findReg $PREFIX*] [cli::Symbols::getElfSymbols name [targetid] $PREFIX*]
                    }

                }
                size {
                    -type integer
                    -optional
                    -brief "Size for saved data."
                    -description {
                        If missing, the size of the symbol will be used.
                    }
                }

            }
            -mutex {
                {-hex -chan}
            }
        }

    } ; # namespace eval SaveFile

    proc savefile {args} {
        cli::SaveFile::ArgumentParser parseCallerArgs 1 $args
        cli::NoErrorStack {
            if {$fileIsHex} {
                error "-hex switch not supported yet!";
            }

            set readAddress [cli::Symbols::getAddress $symNameOrAddress]

            if {!$fileIsChan} {
                set openMode [expr {$appendVar ? "a" : "w"}]b
                set outfile [open $name $openMode]
            } else {
                set outfile $name
            }
            if {![info exists size]} {
                set size [cli::Symbols::getSymSize $symNameOrAddress]
            }

            try {
                cli::TrapInterrupt {
                    if {![info exists memTarget]} {
                        set buffer [jtag readBlock $readAddress $size]
                        puts -nonewline $outfile $buffer
                    } else {
                        set contextId [cli::Target::getContextIdFromTarget $memTarget]
                        set PageSize 1024
                        set i 0
                        while {$size > 0} {
                            cli::CheckInterrupt
                            set maxRead [expr {$PageSize - ( $readAddress + $PageSize ) % $PageSize}]
                            set readSize [expr { min( $maxRead, $size ) }]
                            set buffer [tcf::Memory::getBytes $contextId $readAddress $readSize]
                            puts -nonewline $outfile $buffer
                            set size [expr {$size - $readSize}]
                            cli::incrAddr readAddress $readSize
                        }
                    }
                }
            } finally {
                if {!$fileIsChan} {
                    close $outfile
                }
            }
        }
    }

    cli::Help::Manager add savefile  -parser cli::SaveFile::ArgumentParser
    shell::autocomplete::addCommand savefile cli::SaveFile::ArgumentParser autocomplete

    namespace export savefile

} ; # namespace eval mdbg

