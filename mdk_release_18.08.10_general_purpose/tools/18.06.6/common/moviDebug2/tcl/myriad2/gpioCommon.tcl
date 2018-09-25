# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : gpioCommon.tcl
# Description: Common classes, procedures for platforms
# Created on : Feb 10, 2016
# Author     : Daraban Luminita (luminita.daraban@movidius.com)
# ***************************************************************************

namespace eval mdbg::gpio {

    # struct Coordinates
    #    unsigned int pin;
    #    unsigned int mode;
    #    string signalName;
    oo::class create Coordinates {

        variable Pin Mode SignalName

        constructor {pin mode signalName} {
            set Pin $pin
            set Mode $mode
            set SignalName $signalName
        }

        method getPin {} {
            return $Pin
        }

        method getMode {} {
            return $Mode
        }

        method getSignalName {} {
            return $SignalName
        }

    }; # oo::class Coordinates

    # struct GpioNames
    #    const char* signals[6];
    #    unsigned int direction[6];
    #    int bypassGroup;
    oo::class create GpioNames {

        variable Signals Directions BypassGroup

        constructor {signals direction {bypassGroup ""}} {
            set Signals $signals
            set Directions $direction
            set BypassGroup $bypassGroup
        }

        method getSignals {} {
            # variable Signals
            return $Signals
        }

        method getSignal {mode} {
            # variable Signals
            lindex $Signals $mode
        }

        method hasSignal {mode} {
            expr {
                [string length [my getSignal $mode]] != 0
            }
        }

        method getDirection {mode} {
            # variable Directions
            format %d [
                lindex $Directions $mode
            ]
        }

        method getBypassGroup {} {
            # variable BypassGroup
            return $BypassGroup
        }

    }; # oo::class GpioNames

	oo::class create MA2x5x+_GpioInterfaceBase {
        variable DPlatform
        variable GPIONumber

        constructor {platform} {
            namespace path [concat [namespace path] ::mdbg]

            set DPlatform $platform
            set GPIONumber [$platform getGpioNumber]
            set baseAddress [$platform getGpioAddress]

            my INIT_DEF $baseAddress
        }

        # ------------
        mdbg::oo::DefineMap create DEF
        DEF add GPIO_NUMBER_MODES           6

        method SignalInfo {pinNumber} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioMode
            set itemList [dict create \
            INDEX $pinNumber \
            MODE [my getMode $gpioMode $pinNumber] \
            STATUS [my getDir $gpioMode $pinNumber] \
            RAW [my getRaw $pinNumber] \
            VOLT [my getVoltage $pinNumber] \
            DRIVE [my getDrive $gpioMode] \
            SLEW [my getSlew $gpioMode] \
            PUP [my getPUP $gpioMode] \
            SMT [my getSMT $gpioMode] \
            WAK [my getWak $gpioMode] \
            INV [my getEnInv $gpioMode] \
            DATA_INV [my getDataInv $gpioMode] \
            REN [my getRen $gpioMode]
            ]
            return $itemList
        }

        method statusRange {startGpio endGpio} {
            set itemList [list]
            for {set pin $startGpio} {$pin <= $endGpio} {incr pin} {
                lappend itemList [my SignalInfo $pin]
            }
            return $itemList
        }

        method statusAll {} {
            my statusRange 0 [expr {$GPIONumber - 1}]
        }

        method INIT_DEF {baseAddress} {
            mdbg::oo::DefineMap create DEF

            DEF add BASE_ADDR $baseAddress
            DEF add GPIO_DATA_OUT               BASE_ADDR + 0x0000
            DEF add GPIO_DATA_INPUT             BASE_ADDR + 0x0020
            DEF add GPIO_DATA_INPUT_RAW         BASE_ADDR + 0x0040
            DEF add GPIO_PAD_CTRL               BASE_ADDR + 0x0188
            DEF add GPIO_MODE                   BASE_ADDR + 0x0200

            # mode
            DEF add GPIO_MODE_BITS                   0x00007

            # pads
            DEF add PAD_OPBIAS_SEL              0x00100
            DEF add PAD_REN                     0x00080
            DEF add PAD_SCHMITT                 0x00040
            DEF add PAD_SLEW                    0x00020
            DEF add PAD_VOLT_SEL                0x00010
            DEF add PAD_DRIVE_3                 0x0000C
            DEF add PAD_DRIVE_2                 0x00008
            DEF add PAD_DRIVE_1                 0x00004
            DEF add PAD_DRIVE_0                 0x00000
            DEF add PAD_DRIVE                   0x0000C
            DEF add PAD_PUPD_SEL_3              0x00003
            DEF add PAD_PUPD_SEL_2              0x00002
            DEF add PAD_PUPD_SEL_1              0x00001
            DEF add PAD_PUPD_SEL_0              0x00000
            DEF add PAD_PUPD_SEL                0x00003

            DEF add GPIO_NUMBER_MODES           6

            DEF add GPIO_PULLUP      0x3000
            DEF add GPIO_REN         0x800
            DEF add GPIO_SCHMITT     0x400
            DEF add GPIO_SLEW        0x200
            DEF add GPIO_DRIVE       0x180
            DEF add GPIO_WAKE        0x40
            # GPIO_OE_INV is EN_INV from documentation
            DEF add GPIO_OE_INV      0x20
            DEF add GPIO_DATA_INV    0x10
            DEF add GPIO_DIR         0x8 ;# input_enable
            DEF add GPIO_MODE_SELECT 0x7
        }



        method getRaw {pinNumber} {
            set gpioRawValue [my GetBitFrom GPIO_DATA_INPUT_RAW $pinNumber]
            if {$gpioRawValue != 0} {
                return "1"
            } else {
                return "0"
            }
        }

        method getMode {gpioModeValue pinNumber} {
            # [0:2 MODE_SELECT]
            set modeValue [DEF expr {($gpioModeValue & GPIO_MODE_SELECT)}]
            set gpioModeObj [lindex [$DPlatform getGPIOModes] $pinNumber]

            if {$gpioModeObj == ""} {
                return
            }

            set signalList [$gpioModeObj getSignals]

            list $modeValue [lindex $signalList $modeValue]
        }

        method setMode {pinNumber newValue} {
            # [0:2 MODE_SELECT]
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            my SetBits gpioModeValue 0 2 $newValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getDir {gpioModeValue pinNumber} {
            # [3 GPIO_DIR]
            set gpioDir [DEF expr { (($gpioModeValue & GPIO_DIR) >> 3) & 0x1}]
            if {( $gpioDir == 0)} {
                set outputValue [my getOutputValue $pinNumber]
                return "output = $outputValue"
            } else {
                set inputValue [my getInputValue $pinNumber]
                return "input = $inputValue"
            }
        }

        method setDir {pinNumber newValue} {
            # [3 GPIO_DIR]
            if {(($newValue == "in") || ($newValue == 1))} {
                set bitVal 1
                }
            if {(($newValue == "out") || ($newValue == 0))} {
                set bitVal 0
                }
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            # set GPIO_DIR (bit number 3)
            my SetBit gpioModeValue 3 $bitVal
            # set GPIO_EN_INV (bit number 5)
            my SetBit gpioModeValue 5 0
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getDataInv {gpioModeValue} {
            # [4 GPIO_DATA_INV]
            set gpioDataInvert [DEF expr { (($gpioModeValue & GPIO_DATA_INV) >> 4) & 0x1}]
            if {$gpioDataInvert == 1} {
                return "ON"
            } else {
                return "OFF"
            }
        }

        method setDataInv {pinNumber newValue} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            # [4 GPIO_DATA_INV]
            my SetBit gpioModeValue 4 $newValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getEnInv {gpioModeValue} {
            # [5 GPIO_OE_INV is GPIO_EN_INV from documentation]
            set gpioEnInvert [DEF expr { (($gpioModeValue & GPIO_OE_INV) >> 5) & 0x1}]
            if {$gpioEnInvert == 1} {
                return "ON"
            } else {
                return "OFF"
                }
        }

        method setEnInv {pinNumber newValue} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            # [5 GPIO_OE_INV is GPIO_EN_INV from documentation]
            my SetBit gpioModeValue 5 $newValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getWak {gpioModeValue} {
            # [6 GPIO_WAKE_EN]
            set gpioWak [DEF expr { (($gpioModeValue & GPIO_WAKE) >> 6) & 0x1}]
            if {$gpioWak == 1} {
                return "Enable"
            } else {
                return "Disable"
                }
        }

        method setWak {pinNumber newValue} {
            # [6 GPIO_WAKE_EN]
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            my SetBit gpioModeValue 6 $newValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getDrive {gpioModeValue} {
            # [7:8 GPIO_DRIVE]
            set gpioDrive [DEF expr { (($gpioModeValue & GPIO_DRIVE) >> 7) & 0x3}]
            if {$gpioDrive == 3} {
                return "12mA"
            } elseif {$gpioDrive == 2} {
                return "8mA"
            } elseif {$gpioDrive == 1} {
                return "4mA"
            } elseif {$gpioDrive == 0} {
                return "2mA"
            } else {
                return "---"
            }
        }

        method setDrive {pinNumber newValue} {
            # [7:8 GPIO_DRIVE]
            if {$newValue == 2} {
                set bitsVal 0b0
            }
            if {$newValue == 4} {
                set bitsVal 0b01
            }
            if {$newValue == 8} {
                set bitsVal 0b10
            }
            if {$newValue == 12} {
                set bitsVal 0b11
            }
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            my SetBits gpioModeValue 7 8 $bitsVal
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getSlew {gpioModeValue} {
            # [9 GPIO_SLEW]
            set gpioSLEW [DEF expr { (($gpioModeValue & GPIO_SLEW) >> 9) & 0x1}]
            if {$gpioSLEW == 1} {
                return "Fast"
            } elseif {$gpioSLEW == 0} {
                return "Slow"
            } else {
                return "---"
            }
        }

        method setSlew {pinNumber newValue} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            if {$newValue == "fast"} {
                set newVal 1
            } else {
                set newVal 0
            }
            # [9 GPIO_SLEW]
            my SetBit gpioModeValue 9 $newVal
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getSMT {gpioModeValue} {
            # [10 GPIO_SCHMITT]
            set gpioSCHMITT [DEF expr { (($gpioModeValue & GPIO_SCHMITT) >> 10) & 0x1}]
            if {$gpioSCHMITT == 1} {
                return "SCHMITT"
            } elseif {$gpioSCHMITT == 0} {
                return "No SCHMITT"
            } else {
                return "---"
            }
        }

        method setSMT {pinNumber newValue} {
            if {$newValue == "off"} {
                set newVal 0
            } else {
                set newVal 1
            }
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            # [10 GPIO_SCHMITT]
            my SetBit gpioModeValue 10 $newVal
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getRen {gpioModeValue} {
            # [11 GPIO_REN]
            set gpioRen [DEF expr { (($gpioModeValue & GPIO_REN) >> 11) & 0x1}]
            if {$gpioRen == 1} {
                return "REN"
            } elseif {$gpioRen == 0} {
                return "No REN"
            } else {
                return "---"
            }
        }

        method setRen {pinNumber newValue} {
            # [11 GPIO_REN]
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            my SetBit gpioModeValue 11 $newValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getPUP {gpioModeValue} {
            # [12:13 GPIO_PULLUP]
            set gpioPULLUP [DEF expr { (($gpioModeValue & GPIO_PULLUP) >> 12)& 0x3}]

            if {$gpioPULLUP == 0x3} {
                return "Pin bus keeper"
            } elseif {$gpioPULLUP == 0x2} {
                return "Pull down"
            } elseif {$gpioPULLUP == 0x1} {
                return "Pull up"
            } elseif {$gpioPULLUP == 0} {
                return "No pull"
            } else {
                return "---"
            }
        }

        method setPUP {pinNumber newValue} {
            # [12:13 GPIO_PULLUP]
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            if {$newValue == "NO_PULL"} {
                set bitsVal 0b0
            }
            if {$newValue == "PUP"} {
                set bitsVal 0b01
            }
            if {$newValue == "PDOWN"} {
                set bitsVal 0b10
            }
            if {$newValue == "BUS_KEEPER"} {
                set bitsVal 0b11
            }
            my SetBits gpioModeValue 12 13 $bitsVal
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method listModesRange {startGpio endGpio} {
            set itemList [list]
            for {set pin $startGpio} {$pin <= $endGpio} {incr pin} {
                lappend itemList {*}[gpio::interface listModes $pin]
            }
            return $itemList
        }

        method listModesAll {} {
            my listModesRange 0 [expr {$GPIONumber - 1}]
        }

        method listModes {pinNumber} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            set gpioModeValue [DEF expr { ($gpioModeValue & GPIO_MODE_SELECT)}]
            set gpioModes [$DPlatform getGPIOModes]
            set result [list]
            set pinIndex 0
            foreach gpioMode $gpioModes {
                set row [list $pinIndex]
                set rowMatches false
                foreach signalName [$gpioMode getSignals] {
                    if {$pinIndex == $pinNumber} {
                        lappend row $signalName
                        set rowMatches true
                    } else {
                        lappend row ---
                    }
                }
                if {$rowMatches} {
                    lappend result $row
                }
                incr pinIndex
            }
            return $result
        }

        method findPins {regExpression} {
            set gpioModes [$DPlatform getGPIOModes]
            set result [list]
            set pinIndex 0
            foreach gpioMode $gpioModes {
                set row [list $pinIndex]
                set rowMatches false
                foreach signalName [$gpioMode getSignals] {
                    if {[regexp $regExpression $signalName]} {
                        lappend row $signalName
                        set rowMatches true
                    } else {
                        lappend row ---
                    }
                }
                if {$rowMatches} {
                    lappend result $row
                }
                incr pinIndex
            }
            return $result
        }

        method setDirectMode {pinNumber} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            set directModeValue 7
            my SetBits gpioModeValue 0 2 $directModeValue
            my Set {GPIO_MODE + ( $pinNumber * 4 )} $gpioModeValue
        }

        method getInputValue {pinNumber} {
            my GetBitFrom GPIO_DATA_INPUT $pinNumber
        }

        method getOutputValue {pinNumber} {
            my GetBitFrom GPIO_DATA_OUT $pinNumber
        }

        method setOutputValue {pinNumber newData} {
            my SetBitTo GPIO_DATA_OUT $pinNumber $newData
        }

        method GetBitFrom {BASE_ADDR_DEF pinNumber} {
            set wordIndex [expr {$pinNumber / 32}]
            set bitIndex [expr {$pinNumber % 32}]
            set bitMask [expr {1 << $bitIndex}]
            my Get "$BASE_ADDR_DEF + $wordIndex * 4" wordValue
            expr { ( $wordValue & $bitMask ) != 0 }
        }

        method SetBitTo {BASE_ADDR_DEF pinNumber bitValue} {
            set wordIndex [expr {$pinNumber / 32}]
            set bitIndex [expr {$pinNumber % 32}]
            my Get "$BASE_ADDR_DEF + $wordIndex * 4" wordValue
            my SetBit wordValue $bitIndex $bitValue
            my Set "$BASE_ADDR_DEF + $wordIndex * 4" $wordValue
        }

        method setDirToOutput {pin} {
            my setDir $pin 0
            }

        method setGpio_Range {startGpio endGpio newData} {
            for {set pin $startGpio} {$pin <= $endGpio} {incr pin} {
                my setDirectMode $pin
                my setDirToOutput $pin
                my setOutputValue $pin $newData
            }
        }

        method setGpio_All {newData} {
            setGpio_Range 0 [expr {$GPIONumber - 1}] $newData
        }

        method toggle {pinNumber} {
            my Get {GPIO_MODE + ( $pinNumber * 4 )} gpioModeValue
            set gpioModeVal [DEF expr {($gpioModeValue & GPIO_MODE_SELECT)}]
            set gpioDir [DEF expr { (($gpioModeValue & GPIO_DIR) >> 3) & 0x1}]
            if {$gpioModeVal == 0x7 && $gpioDir == 0} {
                set outputValue [my getOutputValue $pinNumber]
                set complOutputValue [expr {!$outputValue}]
                my setOutputValue $pinNumber $complOutputValue
            } else {
                set rawVal [my getRaw $pinNumber]
                set complrawVal [expr {!$rawVal}]
                my setOutputValue $pinNumber $complrawVal
            }
        }
        method toggleRange {startGpio endGpio} {
            set itemList [list]
            for {set pin $startGpio} {$pin <= $endGpio} {incr pin} {
                lappend itemList {*}[gpio::interface toggle $pin]
            }
            return $itemList
        }

        method toggleAll {} {
            my toggleRange 0 [expr {$GPIONumber - 1}]
        }

        # ---------------------------------------------

         method setLocal {pinNumber newValue} {
            my Get {GPIO_PAD_CTRL} gpioPadCtrlValue
            my SetBits gpioPadCtrlValue 0 7 $pinNumber
            my SetBit gpioPadCtrlValue 11 $newValue
            my Set {GPIO_PAD_CTRL} $gpioPadCtrlValue
         }

        method Get {addrExp valueVar} {
            upvar 1 $valueVar value
            set addr [DEF upexprf 1 $addrExp]
            set value [::mvproto::get32 $addr]
        }

        method Set {addrExp value} {
            set addr [DEF upexprf 1 $addrExp]
            ::mvproto::set32 $addr $value
        }

        method SetBit {varName bitIndex {bitValue 1}} {
            upvar 1 $varName var
            set bit [expr {1 << $bitIndex}]
            if {$bitValue} {
                set var [expr {$var | $bit}]
            } else {
                set var [expr {$var & ~ $bit}]
            }
        }

        method SetBits {varName low high bits} {
            upvar $varName var
            set ones [expr {
                ( 1 << ( 1 + $high - $low ) ) - 1
            }]
            set var [expr {
                  ( $var & ~ ( $ones << $low ) )
                | ( ( $bits & $ones ) << $low )
            }]
        }

        method SET_GPIO_PIN {oldVal newVal pinNumber} {
            expr {
                ((($oldVal) & (~(1 << (($pinNumber) & 0x1F)))) | ($newVal) << (($pinNumber) & 0x1F))
            }
        }

		method GetPlatform {} {
            return $DPlatform
        }
    }; # oo::class create MA2x5x+_GpioInterfaceBase
} #namespace eval mdbg::gpio

