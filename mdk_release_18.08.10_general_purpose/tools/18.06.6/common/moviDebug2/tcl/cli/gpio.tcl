# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : gpio.tcl
# Description: moviDebug2 CLI `gpio` command
# Created on : Feb 11, 2016
# Author     : Luminita Daraban (luminita.daraban@movidius.com)
# ***************************************************************************

namespace eval mdbg {

    namespace eval cli::Gpio {

        namespace path {::mdbg}

        cli::TabularDisplay create Display {
            INDEX {
                -header INDEX
                -present
                -align right
            }
            MODE {
                -header MODE
                -present
                -align center
                -transform {
                    lassign $MODE value name
                    if {$name != ""} {
                        return $name
                    }
                    if {$value != ""} {
                        return "Mode $value"
                    }
                    return
                }
            }
            STATUS {
                -header STATUS
                -present
                -align center
            }
            RAW {
                -header RAW
                -present
                -align center
            }
            VOLT {
                -header VOLT
                -present
                -align center
            }
            DRIVE {
                -header DRIVE
                -present
                -align center
            }
            SLEW {
                -header SLEW
                -present
                -align center
            }
            PUP {
                -header PUP
                -present
                -align center
            }
            SMT  {
                -header SMT
                -present
                -align center
            }
            WAK  {
                -header WAK
                -present
                -align center
            }
            INV  {
                -header OE_INV
                -present
                -align center
            }
            DATA_INV  {
                -header DATA_INV
                -present
                -align center
            }
            REN  {
                -header REN
                -present
                -align center
            }
        }

        oo::objdefine Display {
            mixin cli::ListResultTable
        }

        cli::GridDisplay create FindPinDisplay {
            0 {
                -header "ID"
            }
            1 {
                -header "Mode 0"
                -align center
            }
            2 {
                -header "Mode 1"
                -align center
            }
            3 {
                -header "Mode 2"
                -align center
            }
            4 {
                -header "Mode 3"
                -align center
            }
            5 {
                -header "Mode 4"
                -align center
            }
            6 {
                -header "Mode 5"
                -align center
            }
            7 {
                -header "Mode 6"
                -align center
            }
        }

        oo::objdefine FindPinDisplay mixin cli::ListResultTable

        cli::MultiCmdArgParser create ArgumentParser -brief "GPIO pin state management" -description {
            Display state of, and configure the General-purpose input/output (GPIO) pins of the MA2x5x chip.

            SUBCOMMAND, VARIANTS    DESCRIPTION

            gpio status             Show GPIO pin status.
            gpio set                Set output value in direct mode.
            gpio configure          Configure a GPIO pin/range.
            gpio listmodes          List all modes.
            gpio get                Get the input value in direct mode.
            gpio getraw             Display the raw status
            gpio findpins           Show all GPIO pins starting with
                                    'regExpression'
            gpio toggle             Invert the output value
            gpio help

            *** EXAMPLES
            Show the current state of all GPIOs:
            % gpio status
            Show the current state of gpio 22 to 35:
            % gpio status 22..35
            Set GPIO 19 high
            % gpio set 19 1
            Display the full gpio mode selection table:
            % gpio get 12
            Show the raw state of gpio 12 pin:
            % gpio getraw 12
            Show the state of the input cell for GPIO 12:
            % gpio listmodes 12
            Show the gpio pins which have a mode containing the string scl:
            % gpio findpins scl
            Advanced pin configurations:
            Configure gpio 3 in mode 0x7 as an output with an output value of 1 (equivalent of gpio set 3 1):
            % gpio configure 3 -mode 7 -out 1 -direction out
            Configure gpios 3 to 7 in mode 0x7 as an output with an output value of 1 (equivalent of gpio set 3 1):
            % gpio configure 3..7 -mode 7 -out 1 -direction out
            Configure gpio 40 to 45 to output 1 with drive strenth 12 mA; voltage 1.8 V; Weak pullup enabled; Schmitt trigger on; wakeup disabled; and reciever enabled:
            % gpio configure 40..45 -mode 7 -out 1 -direction out -drive 12 -voltage 1.8 -res PUP -schmitt on -wakeEn 0 -ren 1
        } -auto gpioCmd {
            status {
                -brief "Show GPIO pin status."
                -description {

                    Displays the status of GPIO pin configuration.

                    *** EXAMPLES

                    Display state of all pins:
                    % gpio status

                    Display status of the first GPIO pin:
                    % gpio status 0

                    To display status for pins from 0 to 10 (inclusive):
                    % gpio status 0..10
                }
            }
            status - listmodes - findpins {
                -args {
                    -quiet {
                        -flag
                        -brief "Don't display; return list of dictionaries."
                        -description {
                            Normally the subcommand displays a table to the standard output\
                            and will leave no result in the Tcl interpreter.
                            This flag inhibits the display and causes the command to return a result\
                            usable for further processing by Tcl scripts.

                            WARNING: Format of returned data is not definitive and subject to change without notice.
                        }
                    }
                    -json {
                        -flag
                        -brief "Don't display; return JSON data."
                        -description {
                            Same as "-quiet" but the output format is JSON instead of Tcl dictionaries.

                            WARNING: The format of the returned data is not 100% definitive.

                            Cannot be used with "-quiet".
                        }
                    }
                }
                -mutex {-quiet -json}
            }
            status - set - listmodes - toggle - configure {
                -args {
                    gpioNumber {
                        -optional
                        -regexp {^[0-9]+$}
                        -synopsis {
                            ?gpioNumber[..gpioNumber]?
                        }
                        -brief {
                            Gpio number or range
                        }
                    }
                    gpioRange {
                        -optional
                        -regexp {^[0-9]+\.\.[0-9]+$}
                        -hidden
                    }
                }
                -mutex {
                    {gpioNumber gpioRange}
                }

            }
            set - configure - toggle {
                -requires {
                    {gpioNumber gpioRange}
                }
            }
            set {
                -args {
                    newValue {
                        -regexp {^[0-9]+$}
                    }
                }
                -brief {
                    Set output value in direct mode.
                }
                -description {
                     Configures the GPIO pin to output in direct mode and set the output value.

                     *** EXAMPLES

                     To configure the GPIO 40 to output in direct mode and set the output value to 1.
                     % gpio set 40 1

                     To configure the GPIOs between 40 and 45 to output in direct mode and set the output value to 0.
                     % gpio set 40..43 0

                }
                -mutex {
                    {gpioNumber gpioRange}
                }
            }
            get - getraw {
                -brief ""
                -args {
                    gpioNumber {
                        -type integer
                        -brief {
                            Gpio number
                        }
                    }
                }
            }
            get {
                -brief "Get the input value in direct mode."
                -description {

                     Configures the GPIO pin to input in direct mode and get the input value.

                     *** EXAMPLES

                     To configure the GPIO 40 to input in direct mode and get the input value.
                     % gpio get 40
                }
            }
            listmodes {
                -brief "List all modes"
                -description {
                    Display all modes for the GPIO pin.

                    *** EXAMPLES

                    To display the modes for GPIO 0.
                    % gpio listmodes 0

                    To display the modes for GPIOs between 40 and 45
                    % gpio listmodes 40..45
                }
            }
            getraw {
                -brief "Display the raw status"
                -description {
                    Read the raw status bit for the GPIO without changing its configuration.

                    *** EXAMPLES

                    To display raw status of  GPIO 2
                    % gpio getraw 2
                }
            }
            findpins {
                -brief "Show all GPIO pins starting with 'regExpression'"
                -description {
                    Display all Gpio pins which had the mode starting with 'regExpression'

                    *** EXAMPLES

                    To display all Gpio pins which had the mode starting with 'cam_'
                    % gpio findpins ^cam_
                }
                -args {
                    regExpression {
                    }
                }
            }

            configure {
                -brief "Configure a GPIO pin."
                -description {
                    Configures one or more of the parameters for GPIO pin (mode, direction, drive, voltage, res, schmitt, wakeEn, ren, out value).

                    *** EXAMPLES

                     To configure the parameters of GPIO 40
                    % gpio configure 40 -mode 7 -out 1 -direction out -drive 4 -voltage 1.8 -res PUP -schmitt on -wakeEn 0 -ren 1 -local 1
                    To configure the parameters of the GPIOs pins between 40 and 45
                    % gpio configure 40..45 -mode 7 -out 1 -direction out -drive 12 -voltage 1.8 -res PUP -schmitt on -wakeEn 0 -ren 1
                }
                -args {
                    -mode {
                        -optional
                        -choice {0 1 2 3 4 5 6 7}
                    }
                    -direction {
                        -optional
                        -choice {in out}
                    }
                    -drive {
                        -optional
                        -choice {2 4 8 12}
                    }
                    -voltage {
                        -optional
                        -choice {1.8 3.3}
                    }
                    -res {
                        -optional
                        -choice {BUS_KEEPER PUP PDOWN NO_PULL }
                    }
                    -schmitt {
                        -optional
                        -choice {on off}
                    }
                    -wakeEn {
                        -optional
                        -choice {0 1}
                    }
                    -ren {
                        -optional
                        -choice {0 1}
                    }
                    -local {
                        -optional
                        -choice {0 1}
                    }
                    -out {
                        -optional
                        -choice {0 1}
                    }
                    -data_inv {
                        -optional
                        -choice {0 1}
                    }
                    -oe_inv {
                        -optional
                        -choice {on off}
                    }
                    -slew {
                        -optional
                        -choice {fast slow}
                    }
                    -json {
                        -optional
                        -brief "Use JSON data"
                        -description {
                            Set options from a JSON structure similar to an item of the status array.
                            Replaces the functionality of the other options and as such it cannot be used\
                            together with them.
                        }
                    }
                }
                -mutex {
                    {-json {
                        -mode -direction
                        -drive -voltage -res -shmitt
                        -wakeEn -ren -local -out -data_inv -oe_inv -slew
                    }}
                }
            }

            toggle {
                -brief "Invert the output value"
                -description {
                    The behaviour is:
                        if (gpio configured as output in mode 0x7)
                            gpio set gpionNum inverse(previous gpioOutputSetting)
                        else
                            gpio set gpioNum inverse(current gpioRawValue)

                    *** EXAMPLES

                    To invert the output value for GPIO 2
                    % gpio toggle 2

                    To invert the output value for GPIOs between 40 and 45
                    % gpio toggle 40..45
                }
            }

            help {
            }
        }

        variable JsonResultLayout {
            status {
                List {
                    Object {
                        INDEX->Index        Number
                        MODE->Mode          {
                            Apply mode {
                                lassign $mode value name
                                Object [dict create \
                                    Value [Number $value] \
                                    Name  [String $name] \
                                ]
                            } ::json
                        }
                        STATUS->Status {
                            Apply status {
                                regexp {input|output} $status direction
                                regexp {[0-9]+} $status inout
                                Object [dict create \
                                    Direction  [String $direction] \
                                    Value      [Number $inout] \
                                ]
                            } ::json
                        }
                        RAW->Raw            Integer
                        VOLT->Voltage {
                            Apply voltage {
                                regsub {V$} $voltage {} result
                                return $result
                            }
                        }
                        DRIVE->Drive {
                            Apply drive {
                                regsub {mA$} $drive {} result
                                return $result
                            }
                        }
                        SLEW->Slew {
                            Enum {
                                "Slow"           {"slow"}
                                "Fast"           {"fast"}
                            }
                        }
                        PUP->Res {
                            Enum {
                                "No pull"        {"NO_PULL"}
                                "Pull up"        {"PUP"}
                                "Pull down"      {"PDOWN"}
                                "Pin bus keeper" {"BUS_KEEPER"}
                            }
                        }
                        SMT->Schmitt {
                            Enum {
                                "No SCHMITT" false
                                "SCHMITT"    true
                            }
                        }
                        WAK->WakeEn {
                            Enum {
                                "Disable"   false
                                "Enable"    true
                            }
                        }
                        INV->OeInv              Boolean
                        DATA_INV->DataInv       Boolean
                        REN->Ren {
                            Enum {
                                "REN"    true
                                "No REN" false
                            }
                        }
                    }
                }
            }
            listmodes {
                List {
                    Apply row {
                        set index [lindex $row 0]
                        set modes [lrange $row 1 end]

                        Object [dict create \
                            Index [Number $index] \
                            Modes [ApplyLayout {
                                List NullableString
                            } $modes] \
                        ]
                    } ::json
                }
            }
            findpins {
                List {
                    Apply row {
                        set index [lindex $row 0]
                        set modes [lrange $row 1 end]

                        Object [dict create \
                            Index [Number $index] \
                            Modes [ApplyLayout {
                                List {NullableString "---"}
                            } $modes] \
                        ]
                    } ::json
                }
            }
        }

        proc JsonResult {gpioCmd result} {
            variable JsonResultLayout
            json::ApplyLayout [dict get $JsonResultLayout $gpioCmd] $result
        }

        proc WarningJsonIgnored {gpioCmd field value} {
            cli::Verbose "gpio $gpioCmd -json: field value ignored: [list $field]:[list $value]"
        }

        variable CommandJsonFieldToArgumentConverterMap {
            configure {
                Index {
                    WarningJsonIgnored configure Index $Index
                }
                Mode {
                    if {[string is integer $Mode]} {
                        list -mode $Mode
                    } else {
                        if {[dict exists $Mode Value]} {
                            if {[dict exists $Mode Name]} {
                                WarningJsonIgnored configure Mode.Name [dict get $Mode Name]
                            }
                            list -mode [dict get $Mode Value]
                        } else {
                            WarningJsonIgnored configure Mode $Mode
                        }
                    }
                }
                Status {
                    set result [list]
                    if {[dict exists $Status Direction]} {
                        set Direction [dict get $Status Direction]
                        if {[dict exists $Status Value]} {
                            set Value [dict get $Status Value]
                        }
                        switch -nocase -regexp -- $Direction {
                            in(put)? {
                                lappend result -direction in
                                if {[info exists Value]} {
                                    WarningJsonIgnored configure Status.Value $Value
                                }
                            }
                            out(put)? {
                                lappend result -direction out
                                if {[info exists Value]} {
                                    lappend result -out $Value
                                }
                            }
                            default {
                                lappend result -direction $Direction
                            }
                        }
                    }
                    return $result
                }
                Raw {
                    WarningJsonIgnored configure Raw $Raw
                }
                Voltage {
                    list -voltage $Voltage
                }
                Drive {
                    list -drive $Drive
                }
                Slew {
                    list -slew $Slew
                }
                Res {
                    list -res $Res
                }
                Schmitt {
                    list -schmitt [expr {$Schmitt ? on : off }]
                }
                WakeEn {
                    list -wakeEn [expr {$WakeEn ? 1 : 0 }]
                }
                OeInv {
                    list -oe_inv [expr {$OeInv ? on : off }]
                }
                DataInv {
                    list -data_inv [expr {$DataInv ? 1 : 0 }]
                }
                Ren {
                    list -ren [expr {$Ren ? 1 : 0 }]
                }
            }
        }

        proc convertJsonFieldsToCommandArgs {subcommand json} {
            variable CommandJsonFieldToArgumentConverterMap
            set object [tcf::protocol::parseJsonValue $json]
            set args [list]
            set fieldConverterMap [dict get $CommandJsonFieldToArgumentConverterMap $subcommand]
            set ns [namespace current]
            foreach {field value} $object {
                if {[dict exists $fieldConverterMap $field]} {
                    set converter [dict get $fieldConverterMap $field]
                    if {$value in {null ""}} {
                        continue
                    }
                    set result [apply [list [list $field] $converter $ns] $value]
                    lappend args {*}$result
                } else {
                    WarningJsonIgnored $subcommand $field $value
                }
            }
            return $args
        }


    } ; # namespace eval cli::Gpio

    if {[info commands gpio] == {}} {

        proc gpio {args} {

            set result ""
            cli::Gpio::ArgumentParser parseCallerArgs 1 $args

            cli::NoErrorStack {

                if {$gpioCmd in {set configure}} {
                    if {[info exists gpioRange]} {
                        lassign [split $gpioRange .] gpioNumber . endGpioNumber
                    } else {
                        set endGpioNumber $gpioNumber
                    }
                }

                switch -exact $gpioCmd {
                    status {
                        if {[info exists gpioRange]} {
                            lassign [split $gpioRange .] startGpio . endGpio
                            set itemList [gpio::interface statusRange $startGpio $endGpio]
                        } elseif {[info exists gpioNumber]} {
                            set itemList [gpio::interface statusRange $gpioNumber $gpioNumber]
                        } else {
                            set itemList [gpio::interface statusAll]
                        }
                        if {$json} {
                            return [cli::Gpio::JsonResult $gpioCmd $itemList]
                        }
                        if {$quiet} {
                            return $itemList
                        }
                        cli::Gpio::Display display $itemList
                    }

                    set {
                        if {[info exists gpioRange]} {
                            lassign [split $gpioRange .] startGpio . endGpio
                            gpio::interface setGpio_Range $startGpio $endGpio $newValue
                        } elseif {[info exists gpioNumber]} {
                            gpio::interface setGpio_Range $gpioNumber $gpioNumber $newValue
                        } else {
                            gpio::interface setGpio_All $newValue
                        }
                    }

                    get {
                        return [gpio::interface getInputValue $gpioNumber]
                    }

                    findpins {
                        set raw [gpio::interface findPins $regExpression]
                        if {$json} {
                            return [cli::Gpio::JsonResult $gpioCmd $raw]
                        }
                        if {$quiet} {
                            return $raw
                        }
                        cli::Gpio::FindPinDisplay display $raw
                    }

                    listmodes {
                        if {[info exists gpioRange]} {
                            lassign [split $gpioRange .] startGpio . endGpio
                            set raw [gpio::interface listModesRange $startGpio $endGpio]
                        } elseif {[info exists gpioNumber]} {
                            set raw [gpio::interface listModesRange $gpioNumber $gpioNumber]
                        } else {
                            set raw [gpio::interface listModesAll]
                        }
                        if {$json} {
                            return [cli::Gpio::JsonResult $gpioCmd $raw]
                        }
                        if {$quiet} {
                            return  $raw
                        }
                        cli::Gpio::FindPinDisplay display $raw
                    }

                    getraw {
                        return [gpio::interface getRaw $gpioNumber]
                   }

                    configure {
                        if {[info exists json]} {
                            set args [cli::Gpio::convertJsonFieldsToCommandArgs $gpioCmd $json]
                            tailcall mdbg::gpio configure $gpioNumber..$endGpioNumber {*}$args
                        }
                        while {$gpioNumber <= $endGpioNumber} {
                            if {[info exists mode]} {
                                gpio::interface setMode $gpioNumber $mode
                            }
                            if {[info exists direction]} {
                                gpio::interface setDir $gpioNumber $direction
                            }
                            if {[info exists drive]} {
                                gpio::interface setDrive $gpioNumber $drive
                            }
                            if {[info exists voltage]} {
                                gpio::interface setVoltage $gpioNumber $voltage
                            }
                            if {[info exists res]} {
                                gpio::interface setPUP $gpioNumber $res
                            }
                            if {[info exists schmitt]} {
                                gpio::interface setSMT $gpioNumber $schmitt
                            }
                            if {[info exists wakeEn]} {
                                gpio::interface setWak $gpioNumber $wakeEn
                            }
                            if {[info exists ren]} {
                                gpio::interface setRen $gpioNumber $ren
                            }
                            if {[info exists local]} {
                                gpio::interface setLocal $gpioNumber $local
                            }
                            if {[info exists out]} {
                                gpio::interface setOutputValue $gpioNumber $out
                            }
                            if {[info exists data_inv]} {
                                gpio::interface setDataInv $gpioNumber $data_inv
                            }
                            # [OE_INV is EN_INV from documentation]
                            if {[info exists oe_inv]} {
                                gpio::interface setEnInv $gpioNumber $oe_inv
                            }
                            if {[info exists slew]} {
                                gpio::interface setSlew $gpioNumber $slew
                            }
                            incr gpioNumber
                        }
                    }

                    toggle {
                        if {[info exists gpioRange]} {
                            lassign [split $gpioRange .] startGpio . endGpio
                            set toggle [gpio::interface toggleRange $startGpio $endGpio]
                        } elseif {[info exists gpioNumber]} {
                            set toggle [gpio::interface toggleRange $gpioNumber $gpioNumber]
                        } else {
                            set toggle [gpio::interface toggleAll]
                        }
                    }

                    help {
                        help gpio
                    }

                    default {
                        return -code error "\"$gpioCmd\" not implemented"
                    }
                }
            }
            return $result
        }

        shell::autocomplete::addCommand gpio cli::Gpio::ArgumentParser autocomplete
        cli::Help::Manager add gpio -parser cli::Gpio::ArgumentParser
    }

} ; # namespace eval mdbg
