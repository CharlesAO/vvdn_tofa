# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : uartCommon.tcl
# Description: Myriad2 UART support ported from C++ to Tcl
# Created on : May 19, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::uart {

    namespace path ::mdbg

    proc init {{uartClkDivider 1}} {
        mvproto::lock {
            INIT_CLOCKS

            if {![mdbg::isPlatformMoviSim]} {
                setReg32 $REGS::MCR 1 << 4
                setReg32 $REGS::LCR 1 << 7
                setReg32 $REGS::DLH ($uartClkDivider >> 8) & 0xFF
                setReg32 $REGS::DLL ($uartClkDivider >> 0) & 0xFF
                setReg32 $REGS::LCR 0
                setReg32 $REGS::LCR (3 << 0) | (0 << 2) | (1 << 3)
                setReg32 $REGS::FCR 0x7
                setReg32 $REGS::IER 0x83
            } else {
                # FIXME: do we need this and what's it
                setReg32 $REGS::LCR 1
                setReg32 $REGS::FCR 0x80001843
            }
        }
    }

    proc start {} {
        variable STOPPED
        set STOPPED 0
        updateNow
    }

    proc updateNow {} {
        variable STOPPED
        if {!$STOPPED} {
            CANCEL_HANDLER
            if [mdbg::isPlatformMoviSim] {
                CYCLIC_UPDATE READ_MVSIM
            } else {
                CYCLIC_UPDATE READ_ASIC
            }
        }
    }

    proc flushAll {} {
        if {![DISABLED]} {
            FLUSH 1
        }
    }

    proc flushLine {} {
        if {![DISABLED]} {
            FLUSH 0
        }
    }

    proc stop {} {
        variable STOPPED
        set STOPPED 1
        variable HANDLER_ID
        CANCEL_HANDLER
        return
    }

    oo::Event create LINE_EVENT

    oo::objdefine LINE_EVENT {

        #
        #  Override/Protected
        #
        method CreateKey {counter} {
            return "UART#$counter"
        }
    }

    proc addHandler {command args} {
        uplevel 1 [list \
            [namespace current]::LINE_EVENT bind $command {*}$args \
        ]
    }

    proc AddScopedHandler {command argList NS} {
        namespace inscope $NS [namespace current]::LINE_EVENT bind $command {*}$argList
    }

    proc listHandlers {} {
        LINE_EVENT keys
    }

    proc cancelHandler {id} {
        LINE_EVENT cancel $id
    }

    proc clearHandlers {pattern} {
        LINE_EVENT clear
    }

    upvar 0 ::mdbg::Channels::uart UART_CHANNEL

    set OUTPUT [expr {[info exists UART_CHANNEL] ? $UART_CHANNEL : {stdout}}]

    set SILENT 0

    set QUEUE {}
    set PREFIX "UART: "

    set STOPPED 1
    set IDLE_INTERVAL 750
    set BUSY_INTERVAL 20
    set HANDLER_ID ""

    proc CANCEL_HANDLER {} {
        variable HANDLER_ID
        if {$HANDLER_ID != ""} {
            after cancel $HANDLER_ID
            set HANDLER_ID ""
        }
    }

    trace add variable UART_CHANNEL write [namespace current]::TRACE_UART_CHANNEL_CHANGE

    proc TRACE_UART_CHANNEL_CHANGE {name1 name2 op} {
        set OUTPUT $UART_CHANNEL
    }

    proc CYCLIC_UPDATE {what} {
        variable BUSY_INTERVAL
        variable IDLE_INTERVAL
        variable HANDLER_ID
        set HANDLER_ID ""
        if {[catch {
            if {[$what]} {
                set interval $BUSY_INTERVAL
            } else {
                set interval $IDLE_INTERVAL
            }
            set HANDLER_ID [after $interval [namespace current]::CYCLIC_UPDATE $what]
        } result]} {
            puts $::mdbg::Channels::warning "UART update stopped."
            if {![regexp {mvproto::.*(Channel closed)} $result]} {
                puts stderr $result
            }
            set STOPPED 1
        }
    }

    proc DISABLED {} {
        expr { [info exists ::mdbg::UART] &&
               [string is false -strict $::mdbg::UART]}
    }

    proc AUTO_INIT {} {
        REGS::INIT
        if {![DISABLED]} {
            init
            start
        }
    }

    proc FLUSH {all} {
        variable LAST_READ_LENGTH
        if {[mdbg::isPlatformMoviSim]} {
            set READ READ_MVSIM
            set maxEmptyReads 16
        } else {
            set READ READ_ASIC
            set maxEmptyReads 4
        }
        variable QUEUE
        set maxReadBytes 64
        set eol [expr {$all ? 1 : 0}]
        while {$maxReadBytes > 0 && $maxEmptyReads > 0 } {
            switch -exact [$READ $eol] {
                0 {
                    # nothing read
                    incr maxEmptyReads -1
                    if {$READ == "READ_MVSIM"} {
                        # the simulator might be too slow
                        after 1
                        continue
                    }
                    if {!$all} {
                        return; # done
                    }
                }
                1 {
                    # something read, incomplete line
                    incr maxReadBytes -$LAST_READ_LENGTH
                }
                default {
                    # new line
                    if {!$all} {
                        return; # done!
                    }
                    incr maxReadBytes -$LAST_READ_LENGTH
                }
            }
        }
        if {$QUEUE != "" && $all} {
            ADD_TO_QUEUE 10 ; # forced newline
        }
    }

    # return 1 if new line
    proc ADD_TO_QUEUE {retUartData} {
        variable QUEUE
        variable OUTPUT
        variable PREFIX
        variable SILENT

        set char [binary format "c" [expr {$retUartData & 0xFF}]]
        append QUEUE $char
        if {$char == "\n"} {
            set line $QUEUE
            set QUEUE {}
            if {$OUTPUT != ""} {
                if {!$SILENT} {
                    puts -nonewline $OUTPUT "$PREFIX$line"
                }
            }
            LINE_EVENT raise $line
            return 1
        }
        return 0
    }

    proc READ_MVSIM {{eol 0}} {
        variable LAST_READ_LENGTH
        set LAST_READ_LENGTH 0
        set retUartStatus [getReg32 $REGS::USR]
        if { ( $retUartStatus & 4 ) == 0 } {
            if {[set rxCount [expr {[getReg32 $REGS::TFL] & 0x7F}]] > 0} {
                while {$rxCount > 0} {
                    set retUartData [getReg32 $REGS::RBR]
                    incr LAST_READ_LENGTH
                    if {[ADD_TO_QUEUE $retUartData] && $eol} {
                        return "\n"
                    }
                    incr rxCount -1
                }
                return 1;
            }
        }
        return 0
    }

    proc READ_ASIC {{eol 0}} {
        variable LAST_READ_LENGTH
        set LAST_READ_LENGTH 0

        set BIT_RFNE [expr {1 << 3}]
        set FIFO_ADDRESS_WIDTH 6
        set MASK_RFL_COUNT \
            [expr {(1 << ( 1 + $FIFO_ADDRESS_WIDTH) ) - 1}]

        set retUartStatus [getReg32 $REGS::USR]
        set rxFifoIsNotEmpty [expr {$retUartStatus & $BIT_RFNE}]

        if {$rxFifoIsNotEmpty} {
            set rxCount [getReg32 $REGS::RFL]
            if {[set rxCount [expr {$rxCount & $MASK_RFL_COUNT}]] > 0} {
                while {$rxCount > 0} {
                    set retUartData [getReg32 $REGS::RBR]
                    incr LAST_READ_LENGTH
                    if {[ADD_TO_QUEUE $retUartData] && $eol} {
                        return "\n"
                    }
                    incr rxCount -1
                }
                return 1
            }
        }
        return 0
    }

    proc FLUSH_AT_EXIT {} {
        variable STOPPED
        if {[catch {
            if {!$STOPPED} {
                flushAll
            }
        } result]} {
            if {![string match {*mvproto::*} $result]} {
                puts stderr $::errorInfo
            } else {
                cli::PutWarning "UART flush at exit failed."
            }
        }
    }

    dll::atexit {
        FLUSH_AT_EXIT
    }

} ; # namespace eval mdbg::uart
