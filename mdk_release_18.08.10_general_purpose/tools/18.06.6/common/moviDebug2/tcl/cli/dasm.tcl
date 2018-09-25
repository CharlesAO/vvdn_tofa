# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : dasm.tcl
# Description: moviDebug2 CLI `dasm` command
# Created on : Oct 29, 2015
# Author     : Samuel Ghinet (samuel.ghinet@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Dasm {

        namespace path {::mdbg}

        cli::ArgumentParser create ArgumentParser {
            -brief "Disassemble instructions."
            -description {
                The command is used to disassemble instruction opcodes at a particular address.

                - It uses the current program counter as default starting address.
                - The number of disassembled instructions is 10 by default .

                This behaviour can be customised with switches.

                *** NOTES

                - The disassembled instructions will be shown from the\
                target's point of view, including the contents of the L1/L2 cache.
                - Software breakpoint instructions are not visible in the disassembly.

                *** EXAMPLES

                Disassemble instructions (10, starting from PC):
                % dasm

                Disassemble instructions starting from 0x70000000:
                % dasm 0x70000000
                % dasm -address 0x70000000

                Disassemble 15 instructions starting from 0x70000000:
                % dasm 0x70000000 15
                % dasm -address 0x70000000 -count 15

                Disassemble instructions starting from `main`:
                % dasm main

                Disassemble 15 instructions (from current PC):
                % dasm 15
                % dasm -count 15

                Disassemble 15 Leon OS instructions starting from 0x70000000:
                % dasm -count 15 -target LOS -address 0x70000000
                % dasm -t LOS 0x70000000 15
            }
            -auto
            -args {
                -address {
                    -type integer
                    -optional
                    -brief "Disassemble instructions starting from address"
                    -description {
                        By default the current PC (program counter) is used.
                    }
                }
                -count {
                    -type integer
                    -optional
                    -brief "Number of instructions"
                    -description {
                        The default is 10.
                    }
                }
                -target {
                    -optional
                    -brief "Select target for disassembly."
                    -description {
                        If not specified, uses the current target.
                    }
                }
                -quiet {
                    -optional
                    -flag
                    -brief "Dont' print/format; return list of address/instruction pairs"
                    -description {
                        Normally the command prints its result and returns nothing.
                        This switch causes the output to be omitted and returned as a\
                        Tcl list of dictionaries.
                    }
                }
                -opcodes {
                    -optional
                    -flag
                    -brief "Show opcodes."
                    -description {
                        Show opcodes in addition to decoded instruction.
                    }
                }
                -addrlist {
                    -optional
                    -disabled
                    -type integer
                    -brief "Disassemble one instruction at each address in list"
                    -description {
                    }
                }

                address {
                    -optional
                    -hidden
                    -comment "Skipped if small decimal or hexadecimal number"
                    -not-regexp {(?i)^(0|[1-9][0-9]{0,4}|0x[0-9a-f]{1,3})$}
                }
                count {
                    -optional
                    -type integer
                    -hidden
                }
            }
            -mutex {
                {-address address}
                {-count count}
            }
        }

        cli::TcfTabularDisplay create Display Disassembly::RESULT  {
            ISA {
                -header "ISA"
            }
            ADDRESS {
                -present
                -header "Address"
            }
            SIZE {
                -header "Size"
                -align right
                -skip
            }
            OPCODE {
                -header "Opcode"
                -transform {
                    set bin [cli::Binary::formatBase64 $OPCODE]
                    set hexa [cli::Binary::scanByteList $bin %02X]
                    return $hexa
                }
            }
            INSTR {
                -present
                -header "Instruction"
                -transform {
                    set result [list]
                    foreach item $INSTR {
                        if {[dict exists $item "Type"]} {
                            set type [dict get $item Type]

                            switch $type {
                                String -
                                Register {
                                    set value [dict get $item Text]
                                    lappend result $value
                                }
                                Address -
                                Displacement -
                                Immediate {
                                    set value [dict get $item Value]
                                    lappend result $value
                                }
                            }
                        } else {
                            # lappend result "<no type>"
                        }
                    }
                    join $result "; "
                }
            }
        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

        oo::objdefine ArgumentParser {

            method setupArgs {} {
                uplevel 1 {
                    set contextId [cli::Target::getContextIdFromTargetVar target]

                    if {[info exists address] && ![string is integer $address]} {
                        set address [sym addr -target $contextId $address]
                    }
                }
            }

        }

        proc disassemble {id address count {opcodes 0}} {
            set longResults {}
            set maxInstrSize 16

            set params [list $id $address]
            lappend params [expr $maxInstrSize * $count]
            if {$opcodes} {
                lappend params [list OpcodeValue [json::Boolean 1] Simplified [json::Boolean 1]]
            }
            set longResults [tcf::Disassembly::disassemble {*}$params]

            set processedResults [list]
            set idx 0
            # search for first struct in list whose address >= '$address', and append
            # to the processedResults. Also, the processedResults will have stripped all
            # structs that follow beyond the given $count
            foreach struct $longResults {
                set curAddr [dict get $struct Address]
                if {$curAddr >= $address && $idx < $count} {
                    lappend processedResults $struct
                    incr idx
                }
            }

            set processedResults
        }

        proc showDasmLine {id pc} {
            set dasmlines [disassemble $id $pc 1]
            if {[llength $dasmlines]} {
                set dasm [lindex $dasmlines 0]
                lassign [tcf::getDasmResultFieldList $dasm {ADDRESS INSTR}] addr instr
                if {[llength $instr]} {
                    if {[set text [tcf::getField [lindex $instr 0] Text]] != {}} {
                        cli::PutfInfo {[0x%08X]: %s} $addr $text
                    }
                }
            }
        }

    } ; # namespace eval cli::Dasm

    proc dasm {args} {

        cli::NoErrorStack {

            cli::Dasm::ArgumentParser parseCallerArgs 1 $args
            cli::Dasm::ArgumentParser setupArgs

            if {![info exists address]} {
                set address [::tcf::getContextPC $contextId]
            }
            if {![info exists count]} {
                set count 10
            }

            if {$count > 0xFFF} {
                return -code error "count too large: must be less than 0xFFF"
            }

            set processedResults [cli::Dasm::disassemble $contextId $address $count $opcodes]

            if {$quiet} {
                return $processedResults
            }

            if {[llength $processedResults]} {
                cli::Dasm::Display display $processedResults
            } else {
                cli::PutInfo "No result."
            }
        }
    }

    shell::autocomplete::addCommand dasm cli::Dasm::ArgumentParser autocomplete
    cli::Help::Manager add dasm -parser cli::Dasm::ArgumentParser

    namespace export dasm

} ; # namespace eval mdbg
