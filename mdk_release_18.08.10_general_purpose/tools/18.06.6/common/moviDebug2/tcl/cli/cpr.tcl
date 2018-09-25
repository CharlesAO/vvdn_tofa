# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : cli/cpr.tcl
# Description: MoviDebug2 `cpr` command
# Created on : Jan 25, 2016
# Author     : Ancuta-Maria Ivascu (ancuta.ivascu@movidius.com)
# ***************************************************************************
namespace eval mdbg {

    namespace eval cli::Cpr {

        namespace path ::mdbg

        cli::MultiCmdArgParser create ArgumentParser -brief "CPR management." -description {
            Control over CPR module.

            The following subcommands are supported:

            SUBCOMMAND, VARIANTS                        DESCRIPTION

            cpr enable                                  Enables given clock.
            cpr disable                                 Disables given clock.
            cpr list <css|mss|sipp|upa|aux>             List all clock ids by
                                                        group.
            cpr sys                                     Display system clock
                                                        status.
            cpr aux                                     Display Aux Clock
                                                        Status.
            cpr pll                                     Display State of pll.
            cpr island <status|enable|disable>          Power Island manipulation.
            cpr setSysFreq <freqMhz>                    Use PLL0 and sys divider
                                                        to achieve freqMhz.
            cpr auxClockSet                             Setup CPR_IO_CLK1 from
                                                        source REFCLK1 with
                                                        8/16 divider.
            cpr reset <show|list|listautodeasserting>   Show sticky resets
                                                        /list all reset signals.
            cpr reset <assert|deassert|pulse>           Alter a reset signal by
                                                        constant name or by
                                                        domain and number.
            cpr boot <getMode|listModes>                Either display currently
                                                        latched boot mode, or
                                                        list.
            cpr summary                                 Display a summary of CPR
                                                        clock and PLL status.
            cpr help                                    Display help for CPR
                                                        command.

            *** ALIASES

            ALIAS       COMMAND
            mclk        cpr

            *** EXAMPLES

            To enable CSS_I2C0 clock:
            % cpr enable   CSS_I2C0
            To enable CSS clock #20:
            % cpr enable   css 20
            To enable all myriad2 clocks:
            % cpr enable   all
            To disable CSS_I2C0 clock:
            % cpr disable  CSS_I2C0
            To disable CSS clock #20:
            % cpr disable  css 20
            To list all clock ids by group:
            % cpr list css
            To display system clock status:
            % cpr sys
            To display Aux Clock Status:
            % cpr aux
            To display State of PLL 0:
            % cpr pll 0
            To display status of the power islands:
            % cpr island status
            To achieve freqMhz by using PLL0 and sys divider:
            % cpr setSysFreq <freqMhz>
            To setup CPR_IO_CLK1 from source REFCLK1 with 8/16 divider:
            % cpr auxClockSet AUX_GPIO1 REFCLK1 8 16
            To list all reset signals:
            % cpr reset list
            To alter a reset signal by constant name:
            % cpr reset assert CSS_I2C0
            To alter a reset signal by domain and number:
            % cpr reset pulse css 10
            To display currently latched boot mode:
            % cpr boot status
            Display a table of the possible GPIO configured boot modes:
            % cpr boot listModes
            To display a summary of CPR clock and PLL status:
            % cpr summary

        } -auto -default help cprCmd {
            enable {
                -brief "Enable CPR."
                -args {
                    args {
                        -var enableArgs
                        -complete-script {
                            cpr::getValidClocks [string toupper $PREFIX]*
                        }
                    }
                }
            }

            disable {
                -brief "Disable CPR."
                -args {
                    args {
                        -var disableArgs
                        -complete-script {
                            cpr::getValidClocks [string toupper $PREFIX]*
                        }
                    }
                }
            }
            list {
                -brief "Lists all clock ids by group"
                -args {
                    subcmd {
                        -brief "Groups of clock (css, mss, sipp, upa, aux... etc)."
                        -complete-script {
                            lsearch -all -glob -inline [cpr::getClockGroups] $PREFIX*
                        }
                    }
                }
            }
            sys {
                -brief "Displays system clock status"
            }
            aux {
                -brief "Display Aux Clock Status"
            }
            pll {
                -brief "Display State of pll"
                -args {
                    subcmd {
                        -choice {0 1 all}
                    }
                }
            }
            island {
                -brief "Power Island manipulation"
                -args {
                    subcmd {
                        -choice {status enable disable}
                    }
                    islandNum {
                        -default ""
                        -brief "Specify the island number."
                    }
                }
            }
            setSysFreq {
                -brief "Use PLL0 and sys divider to achieve freqMhz"
                -args {
                    freqMhz {
                        -brief "Specify the freqMhz"
                    }
                }
            }
            auxClockSet {
                -brief " Setup CPR_IO_CLK1 from source REFCLK1 with 8/16 divider"
                -args {
                    clk {}
                    src {}
                    numerator {
                        -type integer
                        -not-match {-*}
                    }
                    denominator {
                        -type integer
                        -not-match {-*}
                    }
                    enable {
                        -type boolean
                        -default 1
                    }
                }
            }
            reset {
                -brief "Manage reset bits"
                -description {
                    - shows sticky resets
                    - lists all reset signals
                    - alters a reset signal by constant name or by domain and number
                }
                -args {
                    subcmd {
                        -choice {show list listautodeasserting assert deassert pulse}
                    }
                    args {
                        -var resetArgs
                    }
                }
            }
            boot {
                -brief "Either display currently latched boot mode, or list all boot modes"
                -args {
                    subcmd {
                        -choice {getMode listModes status}
                    }
                }
            }
            summary {
                -brief "Display a summary of CPR clock and PLL status"
            }
            grep {
                -brief "Search for clock name by regular expression"
                -description {
                    Only implemented for MA2x8x.
                }
                -args {
                    queryRegex {
                        -var grepQueryRegex
                        -default .*
                    }
                }
            }
            help {
            }
        }
    } ; # namespace eval cli::Cpr

    # Temporary alias for cpr command (historical reasons)
    proc mclk {args} {cpr {*}$args}

    proc cpr {args} {

        set result ""
        cli::Cpr::ArgumentParser parseCallerArgs 1 $args

        if {[string match {ma2[04x]8[05x]} [MV_SOC_REV]]} {
            cli::PutWarning "Absolutely no warranty!"
        }

        cli::NoErrorStack {
            switch -exact $cprCmd {
                disable {
                    cpr::cprEnableDisable disable {*}$disableArgs
                }
                enable {
                    cpr::cprEnableDisable enable {*}$enableArgs
                }
                list {
                    cpr::clockLookup list $subcmd
                }
                sys {
                    cpr::sysClocks
                }
                aux {
                    cpr::auxClocks
                }
                pll {
                    cpr::pll $subcmd
                }
                island {
                    cpr::island $subcmd $islandNum
                }
                setSysFreq {
                    cpr::setSysFreq $freqMhz
                }
                auxClockSet {
                    cpr::auxClockSet $clk $src $numerator $denominator $enable
                }
                reset {
                    cpr::RstCmd $subcmd {*}$resetArgs
                }
                boot {
                    set result [cpr::cprBoot $subcmd]
                }
                summary {
                    cpr::cprSummary
                }
                grep {
                    # MA2x8x
                    cpr::cprGrep $grepQueryRegex
                }
                help {
                    help cpr
                }
            }
        }
        return $result
    }

    shell::autocomplete::addCommand cpr cli::Cpr::ArgumentParser autocomplete
    cli::Help::Manager add cpr -parser cli::Cpr::ArgumentParser

} ; # namespace eval mdbg
