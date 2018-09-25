# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : gpio.tcl
# Description: moviDebug (Classic) gpio command ported from C code to Tcl
# Created on : Aug 4, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::gpio {

    oo::class create Myriad2GpioPlatform {

        # Gpio modes
        variable GPIOModes

        constructor {} {
            ::mdbg::oo::DefineMap create DEF

            DEF add ADDRESS_START_GPIO 0x20E30000
            DEF add GPIO_NUMBER 85
            DEF add GPIO_SHIFT_WITH_24 0x1801
            DEF add GPIO_SHIFT_WITH_12 0x0C01
            DEF add GPIO_SHIFT_MASK 0xfff
            DEF add GPIO_SYNC_MASK 0xfff000

            my INIT_GPIO_MODES
        }

        destructor {
            my DONE_GPIO_MODES
            DEF destroy
        }

        method getGpioAddress {} {
            DEF get ADDRESS_START_GPIO
        }

        method getGpioNumber {} {
            DEF get GPIO_NUMBER
        }

        # return - [list shiftValue maskValue triggerShift]
        method getUpdatePadRingParams {} {
            list [DEF get GPIO_SHIFT_WITH_24] \
                 [DEF get GPIO_SHIFT_MASK] \
                 [DEF get GPIO_SHIFT_WITH_12]
        }

#        method _updatePadRing {gpio} {
#            $gpio UpPad {*} [my getUpdatePadRingParams]
#
#        }

        # @return [list gpioNum maskValue]
        method getSyncPadParams {pin} {
            list [DEF expr {GPIO_NUMBER - $pin + 1}] \
                 [DEF get GPIO_SYNC_MASK]
        }

#        method _syncPad {gpio pin valueVar} {
#            upvar 1 $valueVar value
#            $gpio SyncPad $pin value {*}[my getSyncPadParams $pin]
#        }

        method getDataShiftingParams {gpioNum} {
            list $gpioNum [DEF get GPIO_SHIFT_WITH_12]
        }

#        method _dataShifting {gpio gpioNum valueVar} {
#            upvar 1 $valueVar value
#            $gpio DataShifting value [my getDataShiftingParams]
#        }

        method getVoltage {pinNr} {
            return "1.8V"
        }

        method setVoltageValue {gpioDEF gpioMSG value pinPadVar} {
            upvar 1 $pinPadVar pinPad
            if {[string match 1* $value] } {
                set pinPad [$gpioDEF expr {$pinPad | PAD_VOLT_SEL} ]
                return [$gpioMSG get MSG_DBG_NOTE_VOLTAGE]1.8V
            } else {
                return -code error [$gpioMSG get MSG_DBG_ERR_INVALID_VOLTAGE_VALUE]
            }
        }

        method getGpioSignalNames {pattern} {
            set result [list]
            foreach gpioMode $GPIOModes {
                foreach signalName [$gpioMode getSignals] {
                    if {   [string length $signalName]
                        && [string match $pattern $signalName]} {
                        lappend result $signalName
                    }
                }
            }
            return $result
        }

        method getGPIOModes {} {
            return $GPIOModes
        }

        method INIT_GPIO_MODES {} {
            set GPIOModes [list]

            my AddGPIOMode { "cam_mclk"            "i2s1_sck"            "debug_0"               ""                    "eth_125_ref_i"       ""                 } { 2 3 2 0 1 0 }  1  ;# 0
            my AddGPIOMode { "cam_pclk"            "i2s1_ws"             "debug_1"               ""                    "eth_tx_clk"          ""                 } { 3 3 2 0 1 0 }  2  ;# 1
            my AddGPIOMode { "cam_vsync"           "i2s1_in_sd0"         "debug_2"               "uart_apb_sout"       "eth_tx_en"           ""                 } { 3 1 2 2 2 0 }  2  ;# 2
            my AddGPIOMode { "cam_hsync"           "i2s1_in_sd1"         "debug_3"               "uart_apb_sin"        "eth_tx_err"          ""                 } { 3 1 2 1 2 0 }  3  ;# 3
            my AddGPIOMode { "cam_data0"           "i2s1_in_sd2"         "debug_4"               ""                    "eth_rx_clk"          ""                 } { 1 1 2 0 1 0 }  4  ;# 4
            my AddGPIOMode { "cam_data1"           ""                    "debug_5"               "spi2_ss_out_in_1"    "eth_rx_dv"           ""                 } { 1 0 2 3 1 0 }  5  ;# 5
            my AddGPIOMode { "cam_data2"           "i2s1_out_sd0"        "debug_6"               "spi2_ss_out_in_2"    "eth_rx_er"           ""                 } { 1 2 2 3 1 0 }  5  ;# 6
            my AddGPIOMode { "cam_data3"           "i2s1_out_sd1"        "debug_7"               "spi2_ss_out_in_3"    "eth_rx_col"          ""                 } { 1 2 2 3 1 0 }  5  ;# 7
            my AddGPIOMode { "cam_data4"           "i2s1_out_sd2"        "debug_8"               "spi2_mosi"           "eth_rx_crs"          ""                 } { 1 2 2 3 1 0 }  5  ;# 8
            my AddGPIOMode { "cam_data5"           "i2c0_cur_src_en"     "debug_9"               "spi2_miso"           "eth_mdio"            ""                 } { 1 2 2 3 3 0 }  6  ;# 9
            my AddGPIOMode { "cam_data6"           "i2c0_scl"            "debug_10"              "spi2_sclk_out"       "eth_mdc"             ""                 } { 1 2 2 3 2 0 }  6  ;# 10
            my AddGPIOMode { "cam_data7"           "i2c0_sda"            "debug_11"              "spi2_ss_out_in_0"    "eth_txd_0"           ""                 } { 1 2 2 3 2 0 }  6  ;# 11
            my AddGPIOMode { "cam_data8"           "uart_apb_rts_n"      "i2c1_scl"              "spi1_mosi"           "eth_txd_1"           ""                 } { 1 2 2 3 2 0 }  1  ;# 12
            my AddGPIOMode { "cam_data9"           "uart_apb_cts_n"      "i2c1_sda"              "spi1_miso"           "eth_txd_2"           ""                 } { 1 1 2 3 2 0 }  1  ;# 13
            my AddGPIOMode { "cam_data10"          "uart_apb_sin"        "uart_apb_sir_in"       "spi1_sclk_out"       "eth_txd_3"           ""                 } { 1 1 1 3 2 0 }  1  ;# 14
            my AddGPIOMode { "cam_data11"          "uart_apb_sout"       "uart_apb_sir_out_n"    "spi1_ss_out_in_0"    "eth_txd_4"           ""                 } { 1 2 2 3 2 0 }  7  ;# 15
            my AddGPIOMode { "cam_data12"          "eth_125_ref_i"       "lcd_data_24"           "sd_hst1_dat_3"       "spi0_ss_out_in_0"    "lcd_data_13"      } { 1 1 2 3 3 2 }  7  ;# 16
            my AddGPIOMode { "cam_data13"          "eth_tx_clk"          "lcd_data_25"           "sd_hst1_clk"         "spi0_sclk_out"       "lcd_data_14"      } { 1 1 2 2 3 2 }  7  ;# 17
            my AddGPIOMode { "cam_data14"          "eth_tx_en"           "lcd_data_26"           "sd_hst1_cmd"         "spi0_mosi"           "lcd_data_15"      } { 1 2 2 3 3 2 }  8  ;# 18
            my AddGPIOMode { "cam_data15"          "eth_tx_err"          "lcd_data_27"           "sd_hst1_dat_0"       "spi0_miso"           "lcd_data_16"      } { 1 2 2 3 3 2 }  8  ;# 19
            my AddGPIOMode { "cam_data16"          "eth_rx_clk"          "lcd_data_28"           "sd_hst1_dat_1"       "i2c0_scl"            "lcd_data_17"      } { 1 1 2 3 2 2 }  8  ;# 20
            my AddGPIOMode { "cam_data17"          "eth_rx_dv"           "lcd_data_29"           "sd_hst1_dat_2"       "i2c0_sda"            "lcd_data_18"      } { 1 1 2 3 2 2 }  7  ;# 21
            my AddGPIOMode { "cam_data18"          "eth_rx_er"           "lcd_data_30"           "sd_hst1_cd"          "eth_gtx_clk"         "lcd_data_19"      } { 1 1 2 1 2 2 }  8  ;# 22
            my AddGPIOMode { "cam_data19"          "eth_rx_col"          "lcd_data_31"           "sd_hst1_dat_4"       "i2s0_sck"            "lcd_data_20"      } { 1 1 2 3 3 2 }  8  ;# 23
            my AddGPIOMode { "cam_data20"          "eth_rx_crs"          "lcd_data_32"           "sd_hst1_dat_5"       "i2s0_ws"             "lcd_data_21"      } { 1 1 2 3 3 2 }  9  ;# 24
            my AddGPIOMode { "cam_data21"          "eth_mdio"            "lcd_data_33"           "sd_hst1_dat_6"       "i2s0_in_sd0"         "lcd_data_22"      } { 1 3 2 3 1 2 }  9  ;# 25
            my AddGPIOMode { "cam_data22"          "eth_mdc"             "lcd_data_34"           "sd_hst1_dat_7"       "i2s0_out_sd0"        "lcd_data_23"      } { 1 2 2 3 2 2 }  9  ;# 26
            my AddGPIOMode { "cam_data23"          "eth_txd_0"           "lcd_data_35"           "sd_hst1_wp"          "i2s0_in_sd1"         "pwm_out_1"        } { 1 2 2 1 1 2 }  9  ;# 27
            my AddGPIOMode { "lcd_mclk"            "eth_gtx_clk"         "cam_mclk"              "i2s0_sck"            "i2s0_out_sd1"        "debug_0"          } { 2 2 2 3 2 2 } 10  ;# 28
            my AddGPIOMode { "lcd_pclk"            ""                    "cam_pclk"              "i2s0_ws"             ""                    "debug_1"          } { 3 0 2 3 0 2 } 11  ;# 29
            my AddGPIOMode { "lcd_vsync"           ""                    "cam_vsync"             "i2s0_in_sd0"         ""                    "debug_2"          } { 2 0 3 1 0 2 } 11  ;# 30
            my AddGPIOMode { "lcd_hsync"           ""                    "cam_hsync"             "i2s0_in_sd1"         "uart_apb_sout"       "debug_3"          } { 2 0 3 1 2 2 } 10  ;# 31
            my AddGPIOMode { "lcd_data_en"         ""                    "cam_data0"             "i2s0_in_sd2"         ""                    "debug_4"          } { 2 0 1 1 0 2 } 10  ;# 32
            my AddGPIOMode { "lcd_pwm_0"           ""                    "cam_data1"             "pwm_out_0"           "uart_apb_sin"        "debug_5"          } { 2 0 1 2 1 2 } 10  ;# 33
            my AddGPIOMode { "lcd_pwm_1"           ""                    "cam_data2"             "i2s0_out_sd0"        "uart_apb_sir_in"     "debug_6"          } { 2 0 1 2 1 2 } 10  ;# 34
            my AddGPIOMode { "lcd_pwm_2"           "eth_gtx_clk"         "cam_data3"             "i2s0_out_sd1"        "uart_apb_sir_out_n"  "debug_7"          } { 2 2 1 2 2 2 } 12  ;# 35
            my AddGPIOMode { "lcd_data_0"          ""                    "cam_data4"             "i2s0_out_sd2"        ""                    "debug_8"          } { 2 0 1 2 0 2 } 12  ;# 36
            my AddGPIOMode { "lcd_data_1"          ""                    "cam_data5"             ""                    ""                    "debug_9"          } { 2 0 1 0 0 2 } 13  ;# 37
            my AddGPIOMode { "lcd_data_2"          ""                    "cam_data6"             "eth_txd_5"           ""                    "debug_10"         } { 2 0 1 2 0 2 } 14  ;# 38
            my AddGPIOMode { "lcd_data_3"          ""                    "cam_data7"             "eth_txd_6"           "cpr_io_out_clk_1"    "debug_11"         } { 2 0 1 2 2 2 } 15  ;# 39
            my AddGPIOMode { "lcd_data_4"          "acif_clk"            "cam_data8"             "eth_txd_7"           "cpr_io_out_clk_2"    "debug_12"         } { 2 2 1 2 2 2 } 12  ;# 40
            my AddGPIOMode { "lcd_data_5"          "acif_cmd"            "cam_data9"             "eth_rxd_0"           "cpr_io_out_clk_3"    "debug_13"         } { 2 2 1 1 2 2 } 12  ;# 41
            my AddGPIOMode { "lcd_data_6"          "acif_data_0"         "cam_data10"            "eth_rxd_1"           "spi0_sclk_out"       "debug_14"         } { 2 1 1 1 3 2 } 12  ;# 42
            my AddGPIOMode { "lcd_data_7"          "acif_data_1"         "cam_data11"            "eth_rxd_2"           "uart_apb_cts_n"      "debug_15"         } { 2 1 1 1 1 2 } 15  ;# 43
            my AddGPIOMode { "lcd_data_8"          "acif_cal"            "cpr_io_out_clk_0"      "eth_rxd_3"           "uart_apb_rts_n"      "pwm_out_2"        } { 2 2 2 1 2 2 } 14  ;# 44
            my AddGPIOMode { "lcd_data_9"          "sd_hst0_cd"          "pwm_out_0"             "eth_rxd_4"           "spi0_mosi"           ""                 } { 2 1 2 1 3 0 } 16  ;# 45
            my AddGPIOMode { "lcd_data_10"         "sd_hst0_clk"         "pwm_out_1"             "eth_rxd_5"           "spi0_miso"           ""                 } { 2 2 2 1 3 0 } 16  ;# 46
            my AddGPIOMode { "lcd_data_11"         "sd_hst0_cmd"         "i2s2_sck"              "eth_rxd_6"           "i2c2_scl"            ""                 } { 2 3 3 1 2 0 } 12  ;# 47
            my AddGPIOMode { "lcd_data_12"         "sd_hst0_dat_0"       "i2s2_ws"               "eth_rxd_7"           "i2c2_sda"            ""                 } { 2 3 3 1 2 0 } 12  ;# 48
            my AddGPIOMode { "lcd_data_13"         "sd_hst0_dat_1"       "i2s2_in_sd0"           "pwm_out_3"           "i2c1_scl"            ""                 } { 2 3 1 2 2 0 } 17  ;# 49
            my AddGPIOMode { "lcd_data_14"         "sd_hst0_dat_2"       "i2s2_in_sd1"           "pwm_out_4"           "i2c1_sda"            ""                 } { 2 3 1 2 2 0 } 17  ;# 50
            my AddGPIOMode { "lcd_data_15"         "sd_hst0_dat_3"       "i2s2_in_sd2"           "pwm_out_5"           "uart_apb_sout"       ""                 } { 2 3 1 2 2 0 } 17  ;# 51
            my AddGPIOMode { "lcd_data_16"         "sd_hst0_dat_4"       "cpr_io_out_clk_3"      "acif_clk"            "i2c1_cur_src_en"     ""                 } { 2 3 2 2 0 0 } 17  ;# 52
            my AddGPIOMode { "lcd_data_17"         "sd_hst0_dat_5"       "i2s2_out_sd0"          "acif_cmd"            "spi2_ss_out_in_1"    ""                 } { 2 3 2 2 3 0 } 18  ;# 53
            my AddGPIOMode { "lcd_data_18"         "sd_hst0_dat_6"       "i2s2_out_sd1"          "acif_data_0"         "spi0_ss_out_in_1"    ""                 } { 2 3 2 1 3 0 } 18  ;# 54
            my AddGPIOMode { "lcd_data_19"         "sd_hst0_dat_7"       "i2s2_out_sd2"          "acif_data_1"         "spi2_ss_out_in_2"    ""                 } { 2 3 2 1 3 0 } 19  ;# 55
            my AddGPIOMode { "lcd_data_20"         "sd_hst0_wp"          ""                      "acif_cal"            "spi0_ss_out_in_3"    ""                 } { 2 1 0 2 3 0 } 19  ;# 56
            my AddGPIOMode { "lcd_data_21"         "sd_hst0_irq"         "cpr_io_out_clk_1"      "i2c2_cur_src_en"     "spi0_ss_out_in_2"    ""                 } { 2 1 2 0 3 0 } 18  ;# 57
            my AddGPIOMode { "lcd_data_22"         "sd_hst0_mmc_rstn"    "cpr_io_out_clk_2"      "spi2_ss_out_in_3"    "spi0_sclk_out"       ""                 } { 2 2 2 3 3 0 } 18  ;# 58
            my AddGPIOMode { "lcd_data_23"         "sd_hst0_pwr_a"       "spi1_ss_out_in_1"      "cpr_io_out_clk_0"    "spi0_ss_out_in_0"    ""                 } { 2 2 3 2 3 0 } 18  ;# 59
            my AddGPIOMode { "i2c0_scl"            "sd_hst0_pwr_b"       "spi1_ss_out_in_2"      "spi2_mosi"           "eth_gtx_clk"         ""                 } { 2 2 3 3 2 0 } 18  ;# 60
            my AddGPIOMode { "i2c0_sda"            "sd_hst0_pwr_c"       "spi1_ss_out_in_3"      "spi2_miso"           ""                    ""                 } { 2 2 3 3 0 0 } 20  ;# 61
            my AddGPIOMode { "i2c1_scl"            "eth_txd_1"           "sd_hst0_cd"            "spi2_sclk_out"       "pwm_out_3"           ""                 } { 2 2 1 3 2 0 } 20  ;# 62
            my AddGPIOMode { "i2c1_sda"            "eth_txd_2"           "sd_hst0_clk"           "spi2_ss_out_in_0"    "pwm_out_4"           ""                 } { 2 2 2 3 2 0 } 21  ;# 63
            my AddGPIOMode { "i2s0_sck"            "eth_txd_3"           "sd_hst0_cmd"           "lcd_mclk"            "cpr_io_out_clk_0"    ""                 } { 3 2 3 2 2 0 } 21  ;# 64
            my AddGPIOMode { "i2s0_ws"             "eth_txd_4"           "sd_hst0_dat_0"         "lcd_pclk"            "cam_data12"          ""                 } { 3 2 3 3 1 0 } 22  ;# 65
            my AddGPIOMode { "i2s0_in_sd0"         "eth_txd_5"           "sd_hst0_dat_1"         "lcd_vsync"           "cam_data13"          ""                 } { 1 2 3 2 1 0 } 23  ;# 66
            my AddGPIOMode { "i2s0_in_sd1"         "eth_txd_6"           "sd_hst0_dat_2"         "lcd_hsync"           "cam_data14"          ""                 } { 1 2 3 2 1 0 } 24  ;# 67
            my AddGPIOMode { "i2s0_in_sd2"         "eth_txd_7"           "sd_hst0_dat_3"         "lcd_data_en"         "cam_data15"          ""                 } { 1 2 3 2 1 0 } 24  ;# 68
            my AddGPIOMode { "i2c1_sda"            "eth_rxd_0"           "sd_hst0_dat_4"         "lcd_pwm_0"           "cam_data16"          ""                 } { 2 1 3 2 1 0 } 25  ;# 69
            my AddGPIOMode { "i2s0_out_sd0"        "eth_rxd_1"           "sd_hst0_dat_5"         "lcd_pwm_1"           "cam_data17"          "rt_dsu_active"    } { 2 1 3 2 1 2 } 25  ;# 70
            my AddGPIOMode { "i2s0_out_sd1"        "eth_rxd_2"           "sd_hst0_dat_6"         "lcd_pwm_2"           "cam_data18"          "rt_dsu_tstop"     } { 2 1 3 2 1 2 } 26  ;# 71
            my AddGPIOMode { "i2s0_out_sd2"        "eth_rxd_3"           "sd_hst0_dat_7"         "lcd_data_0"          "cam_data19"          "dsu_active"       } { 2 1 3 2 1 2 } 26  ;# 72
            my AddGPIOMode { "spi1_ss_out_in_0"    "eth_rxd_4"           "sd_hst0_wp"            "lcd_data_1"          "cam_data20"          "dsu_tstop"        } { 3 1 1 2 1 2 } 27  ;# 73
            my AddGPIOMode { "spi0_mosi"           "eth_rxd_5"           "sd_hst0_irq"           "lcd_data_2"          "cam_data21"          "pwm_out_4"        } { 3 1 1 2 1 2 } 28  ;# 74
            my AddGPIOMode { "spi0_miso"           "eth_rxd_6"           "sd_hst0_mmc_rstn"      "lcd_data_3"          "cam_data22"          "pwm_out_5"        } { 3 1 2 2 1 2 } 28  ;# 75
            my AddGPIOMode { "spi0_sclk_out"       "eth_rxd_7"           "sd_hst0_pwr_a"         "lcd_data_4"          "cam_data23"          ""                 } { 3 1 2 2 1 0 } 29  ;# 76
            my AddGPIOMode { "spi0_ss_out_in_0"    "pwm_out_0"           "sd_hst0_pwr_b"         "lcd_data_5"          "i2c1_scl"            "uart_apb_sin"     } { 3 2 2 2 2 1 } 29  ;# 77
            my AddGPIOMode { "spi1_mosi"           "sd_hst1_irq"         "sd_hst0_pwr_c"         "lcd_data_6"          "i2c1_sda"            "spi0_ss_out_in_1" } { 3 2 2 2 2 3 } 30  ;# 78
            my AddGPIOMode { "spi1_miso"           "sd_hst1_mmc_rstn"    "i2c2_scl"              "lcd_data_7"          "debug_12"            "spi0_ss_out_in_2" } { 3 2 2 2 2 3 } 30  ;# 79
            my AddGPIOMode { "spi1_sclk_out"       "sd_hst1_pwr_a"       "i2c2_sda"              "lcd_data_8"          "debug_13"            ""                 } { 3 2 2 2 2 0 } 31  ;# 80
            my AddGPIOMode { "spi1_ss_out_in_0"    "sd_hst1_pwr_b"       "cpr_io_out_clk_1"      "lcd_data_9"          "spi0_ss_out_in_3"    ""                 } { 3 2 0 2 3 0 } 32  ;# 81
            my AddGPIOMode { "spi1_ss_out_in_1"    "sd_hst1_pwr_c"       "cam_pwm_0"             "lcd_data_10"         "pwm_out_3"           "i2s2_out_sd2"     } { 3 2 2 2 2 2 } 33  ;# 82
            my AddGPIOMode { "spi1_ss_out_in_2"    "cpr_io_out_clk_0"    "cam_pwm_1"             "lcd_data_11"         "debug_14"            "i2s2_out_sd0"     } { 3 2 2 2 2 2 } 34  ;# 83
            my AddGPIOMode { "spi1_ss_out_in_3"    "pwm_out_2"           "cam_pwm_2"             "lcd_data_12"         "debug_15"            "i2s2_out_sd1"     } { 3 2 2 2 2 2 } 35  ;# 84
        }

        method AddGPIOMode {args} {
            lappend GPIOModes [mdbg::gpio::GpioNames new {*}$args]
        }

        method DONE_GPIO_MODES {} {
            foreach gpioMode $GPIOModes {
                $gpioMode destroy
            }
            unset GPIOModes
        }

    } ; # oo::class Myriad2GpioPlatform

    #
    #  GPIO command/interface class
    #
    oo::class create GpioInterface {

        variable DPlatform

        variable GPIONumber
        variable GPIOPadRing

        constructor {platform} {
            namespace path [concat [namespace path] ::mdbg]

            set DPlatform $platform
            set GPIONumber [$platform getGpioNumber]
            set GPIOPadRing [lrepeat 142 0]

            set baseAddress [$platform getGpioAddress]

            my INIT_DEF $baseAddress
            my INIT_MSG
        }

        # ------------

        method status {args} {
            tailcall my GetGpioStatus {*}$args
        }
        method st {args} {
            tailcall my GetGpioStatus {*}$args
        }

        method configure {args} {
            tailcall my SetGpioConfig {*}$args
        }

        method cfg {args} {
            tailcall my SetGpioConfig {*}$args
        }

        method registerMap {args} {
            tailcall my SetGpioConfig {*}$args
        }

        method si {args} {
            tailcall my GetGpioSignal {*}$args
        }

        method sig {args} {
            tailcall my GetGpioSignal {*}$args
        }
        method signal {args} {
            tailcall my GetGpioSignal {*}$args
        }

        method help {args} {
            tailcall my PrintGpioHelp {*}$args
        }

        method GetSignalNames {{pattern *}} {
            $DPlatform getGpioSignalNames $pattern
        }

        method GetGpioNumber {} {
            return $GPIONumber
        }

        method GetPlatform {} {
            return $DPlatform
        }

        export GetPlatform GetGpioNumber GetSignalNames

        #method unknown {args} {
        #    my PrintGpioHelp ""
        #}


        # ---------------------------

        method GetGpioStatus {args} {

            my UpdatePadRing

            if {![llength $args] || [lindex $args 0] == "all"} {
                for {set i 0} {$i < $GPIONumber} {incr i} {
                    my PrintPinInfo $i
                }
            } elseif {[llength $args] == 1} {
                if {[lindex $args 0] == "help"} {
                    my PrintGpioHelp "st"
                } else {
                    if {[scan $args %u i] != 1
                        || $i >= $GPIONumber
                    } {
                        my ErrorfMsg MSG_DBG_ERR_GPIO_PINS_ARE_0 [
                            expr $GPIONumber - 1
                        ]
                    }
                    my PrintPinInfo $i
                }
            } elseif { [llength $args] == 2 } {
                # print a range of pins
                if { [scan $args {%u %u} startPin numberOfPins] != 2
                    || $startPin >= $GPIONumber
                    || [set endPin [
                            expr $startPin + $numberOfPins
                        ]] > $GPIONumber
                } {
                    my ErrorfMsg MSG_DBG_ERR_GPIO_PINS_ARE_0 [
                        expr $GPIONumber - 1
                    ]
                }

                for {set i $startPin} {$i < $endPin} {incr i} {
                    my PrintPinInfo $i
                }
            } else {
                my ErrorMsg MSG_DBG_ERR_TOO_MANY_ARGUMENTS_FOR_GPIO_ST_COMMAND
            }
        }

        # ------------

        method GetGpioSignal {args} {
            set gpioModes [$DPlatform getGPIOModes]

            my UpdatePadRing

            if {[llength $args] == 0
                || [lindex $args 0] == "all"
            } {
                for {set i 0} {$i < $GPIONumber} {incr i} {
                    my Get {GPIO_CTRL + ( $i << 2 )} tempVal
                    # if the mode is configured, print it
                    if { ( $tempVal & 0x7) < 0x7 } {
                        my PrintPinInfo $i
                    }
                }
            } elseif {[lindex $args 0] == "help"} {
                my PrintGpioHelp "si"
            } else {
                set errorList [list]
                set signalList [list]
                try {
                    foreach sigName $args {
                        if [catch {
                            lappend signalList [my FindSignal $gpioModes $sigName]
                        } error] {
                            lappend errorList $error
                        }
                    }
                    # signal(s) found, check that it is (they are) mapped
                    foreach signal $signalList {
                        # read mode
                        my Get {GPIO_CTRL + ( [$signal getPin] << 2 )} tempVal
                        # verify correct mode
                        set tempVal [expr $tempVal & 0x7]
                        if {$tempVal == [$signal getMode]} {
                            my PrintPinInfo [$signal getPin]
                        } else {
                            my PutfMsg MSG_DBG_INFO_SIGNAL_IS_NOT_MAPPED [$signal getSignalName]
                        }
                    }
                } finally {
                    foreach signal $signalList  {
                        $signal destroy
                    }
                }
                if [llength $errorList] {
                    my Error [join $errorList \n]
                }
            }
        }

        # ------------

        method SetGpioConfig {args} {
            variable GPIONumber
            variable DEF
            variable MSG
            variable GPIOPadRing

            if {[llength $args] <= 1} {
                my PrintGpioHelp "cfg"
            } else {
                if {[scan [lindex $args 0] %u pinNb] != 1
                    || $pinNb >= $GPIONumber
                } {
                    my ErrorMsg MSG_DBG_ERR_GPIO_PINS_ARE_0 [
                        expr $GPIONumber - 1
                    ]
                }

                variable DPlatform
                set gpioModes [$DPlatform getGPIOModes]

                set params [lrange $args 1 end]

                # check if pad synchronization is required
                set padSync [expr {
                    [lsearch -regexp $params {^-?(p|r|s|v|dr)}] >= 0
                }]

                #  do pad synchronization first step
                if {$padSync} {
                    my SynchronizePadStep1 $pinNb
                }

                my PutfMsg MSG_DBG_INFO_PIN_GPIO_CONFIGURATION $pinNb

                # Read pin configuration register
                my Get {GPIO_CTRL + ( $pinNb << 2 )} newCtrlRegister

                # output enable (active zero)
                set outputEnable 0

                # loop through configurations
                while {[llength $params]} {

                    # remove vector element
                    set params [lassign $params command]
                    # cli::PutDebug "$command | $params"

                    switch -regexp $command {
                        ^-?in(put)?$ {
                            # setting direction to input
                            set outputEnable 1
                            my SetBit newCtrlRegister 11 $outputEnable
                            my PutfMsg MSG_DBG_INFO_OUTENABLE $outputEnable
                            continue
                        }
                        ^-?out(put)?$ {
                            # setting direction to output
                            set outputEnable 0
                            my SetBit newCtrlRegister 11 $outputEnable
                            my PutfMsg MSG_DBG_INFO_OUTENABLE $outputEnable
                            continue
                        }
                    }

                    if {[regexp {^([^=]+)[=](.*)$} $command -> command param]} {
                        # command contains parameter after `=` sign
                        if {$param == ""} {
                            my ErrorMsg MSG_DBG_ERR_NO_PARAMETER_VALUE_SPECIFIED
                        }
                    } else {
                        # check that we have enough parameters specified for the following commands
                        if {![llength $params]} {
                            # invalid value provided - error
                            my ErrorMsg MSG_DBG_ERR_NO_PARAMETER_VALUE_SPECIFIED
                        }
                        # remove vector element
                        set params [lassign $params param]
                    }
                    # cli::PutDebug "$command | $param | $params"

                    switch -regexp $command {
                        ^-?m(ode)?$ {
                            # set the mode

                            if {$param == "direct"} {
                                # requested mode is direct mode
                                set mode 7
                            }  elseif { [scan $param %u mode] != 1
                                || $mode > 7 || $mode == 6 } {

                                # invalid mode provided - error
                                my ErrorMsg MSG_DBG_ERR_REQUESTED_GPIO_MODE_IS_INVALID
                            }

                            if {$mode != 7} {
                                if { ![[lindex $gpioModes $pinNb] hasSignal $mode] } {
                                    my ErrorfMsg MSG_DBG_ERR_SIGNAL_IS_UNAVAILABLE_FOR_PIN_GPIO $pinNb $mode
                                }
                            }

                            # signal and mode are valid - change the mode
                            # my SetBits newCtrlRegister 0 2 $mode
                            set newCtrlRegister [expr { ( $newCtrlRegister & ~7) | $mode}]

                            # display action
                            my Putf  "%s = %d%s" [expr {
                                $mode == 7
                                    ? [MSG get MSG_DBG_NOTE_DIRECT_CONTROL]
                                    : [MSG get MSG_DBG_NOTE_MODE]
                            }] $mode [expr {
                                $mode == 7 ? ")" : ""
                            }]
                        }
                        ^-?data([oO]ut)?$ {
                            # setting data_out value
                            my Get {GPIO_DATA_OUT + (( $pinNb / 32) << 2)} oldData

                            set newData [my ScanBitParam $param]

                            # write new data
                            my Set {GPIO_DATA_OUT + (( $pinNb / 32) << 2)} [
                                my SET_GPIO_PIN $oldData $newData $pinNb
                            ]
                            my PutfMsg MSG_DBG_INFO_DATA_OUT $newData
                        }
                        ^-?inv[oO]utput$ {
                            set tempVal [my ScanBitParam $param]
                            my SetBit newCtrlRegister 14 $tempVal
                            my PutfMsg MSG_DBG_INFO_OUTUT_INV $tempVal
                        }
                        ^-?inv[dD]ata$ {
                            set tempVal [my ScanBitParam $param]
                            my SetBit newCtrlRegister 12 $tempVal
                            my PutfMsg MSG_DBG_INFO_DATA_INV $tempVal
                        }
                        ^-?pull$ {
                            my PrintMsg MSG_DBG_NOTE_PULL
                            set pupd 0
                            set msg MSG_DBG_NOTE_NONE
                            switch -glob $param {
                                n* - 0 {
                                    set pupd 0
                                    set msg MSG_DBG_NOTE_NONE
                                }
                                u* - 1 {
                                    set pupd 1
                                    set msg MSG_DBG_NOTE_UP
                                }
                                d* - 2 {
                                    set pupd 2
                                    set msg MSG_DBG_NOTE_DOWN

                                }
                                b* - 3 {
                                    set pupd 3
                                    set msg MSG_DBG_NOTE_BUS

                                }
                            }
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            lset GPIOPadRing $pinNb [
                                my Expr ( $pinPad & ~ PAD_PUPD_SEL ) | PAD_PUPD_SEL_$pupd
                            ]
                            my PrintMsg $msg
                        }

                        ^-?recv([eE]n)?$ {
                            set tempVal [my ScanBitParam $param]
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            lset GPIOPadRing $pinNb [
                                my Expr ( $pinPad & ~ PAD_REN ) | ( $tempVal << 7 )
                            ]
                            my PutfMsg MSG_DBG_INFO_RECVEN $tempVal
                        }

                        ^-?s(ch)?m(it)?t$ {
                            set tempVal [my ScanBitParam $param]
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            lset GPIOPadRing $pinNb [
                                my Expr ( $pinPad & ~ PAD_SCHMITT ) | ( $tempVal << 6 )
                            ]
                            my PutfMsg MSG_DBG_INFO_SCHMITT $tempVal
                        }
                        ^-?sle?w$ {
                            set tempVal [my ScanBitParam $param]
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            lset GPIOPadRing $pinNb [
                                my Expr ( $pinPad & ~ PAD_SLEW ) | ( $tempVal << 5 )
                            ]
                            my PutfMsg MSG_DBG_INFO_SLEW $tempVal
                        }

                        ^-?v(olt(age)?)?$ {
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            if [catch {
                                set msg [$DPlatform setVoltageValue \
                                    [self namespace]::DEF \
                                    [self namespace]::MSG \
                                    $param pinPad]
                                lset GPIOPadRing $pinNb $pinPad
                                my Puts $msg
                            } error ] {
                                my Error $error
                            }
                        }

                        ^-?drive$ {
                            set tmpVal 2
                            set drv 0
                            switch -glob $param {
                                2* {
                                    set tmpVal 2
                                    set drv 0
                                }
                                4* {
                                    set tmpVal 4
                                    set drv 1
                                }
                                6* {
                                    set tmpVal 6
                                    set drv 2
                                }
                                8* {
                                    set tmpVal 8
                                    set drv 3
                                }
                            }
                            set pinPad [lindex $GPIOPadRing $pinNb]
                            lset GPIOPadRing $pinNb [
                                my Expr ( $pinPad & ~ PAD_DRIVE ) | PAD_DRIVE_$drv
                            ]
                            my PutfMsg MSG_DBG_INFO_DRIVE $tmpVal
                        }

                        default {
                            my PrintGpioHelp "cfg"
                            my ErrorMsg MSG_DBG_ERR_INVALID_GPIO_CFG_PARAMETERS
                        }
                    }

                } ; # while

                # write pin configuration register
                my Set {GPIO_CTRL + ( $pinNb << 2 )} $newCtrlRegister

                # resynchronize pads
                if {$padSync} {
                    my SynchronizePadStep2 $pinNb
                }

            }

        }

        method PrintGpioHelp {args} {
            switch -glob [join $args " "] {
                *registerMap* - cfg* - configure* {
                    # help gpio cfg
                    my PrintfMsg MSG_DBG_NOTE_GPIO_CONFIG
                }
                *st* - *status* {
                    # help gpio status
                    my PrintfMsg MSG_DBG_NOTE_DISPLAY_INFORMATION_GPIO
                }
                *si* - signal* {
                    # help gpio signal
                    my PrintfMsg MSG_DBG_NOTE_GPIO_SIGNAL
                }
                default {
                    # help gpio
                    my PrintfMsg MSG_DBG_NOTE_GPIO_HELP
                }
            }
        }

        method PrintPinInfo {pinNr} {
            my Get {GPIO_CTRL + ( $pinNr << 2 )} gpioCtrl

            set gpioMode [my Expr $gpioCtrl & GPIO_MODE]
            set gpioDataInv [my Expr $gpioCtrl & GPIO_DATA_INV]
            set gpioOutEnable [my Expr $gpioCtrl & GPIO_DIR]
            set gpioEnInv [my Expr $gpioCtrl & GPIO_EN_INV]

            if [my Expr $gpioMode == GPIO_MODE_7] {
                # GPIO DIRECT control
                if {!( $gpioOutEnable ^ $gpioEnInv )} {
                    # is it output or input
                    my Get {GPIO_DATA_OUT + ( ( $pinNr / 32 ) << 2 )} gpioOut
                } else {
                    my Get {GPIO_DATA_INPUT + ( ( $pinNr / 32 ) << 2 )} gpioOut
                }
                set gpioOut [expr ( $gpioOut >> ( $pinNr & 0x1F ) ) & 0x01 ] ; # Data bit

                if { ( $gpioOutEnable ^ $gpioEnInv ) == 0 } {
                    set ioModeStr [MSG get MSG_DBG_NOTE_OUTPUT]
                } else {
                    if [my Expr ( [lindex $GPIOPadRing $pinNr] & PAD_REN ) != 0] {
                        set ioModeStr [MSG get MSG_DBG_NOTE_INPUT_PAD_REN]
                    } else {
                        set ioModeStr [MSG get MSG_DBG_NOTE_INPUT]
                    }
                }

                if { $gpioOutEnable & $gpioEnInv } {
                    set ioInvertStr [MSG get MSG_DBG_NOTE_INVERTED_INPUT]
                } else {
                    set ioInvertStr ""
                }

                set valueOnPin [
                    expr { ( $gpioOut ^ $gpioDataInv ) ? 1 : 0 }
                ]

                my PrintfMsg MSG_DBG_NOTE_DIRECT_ACCESS \
                    $pinNr $ioModeStr $ioInvertStr $valueOnPin

                if {$gpioDataInv} {
                    my PrintfMsg MSG_DBG_NOTE_INVERTED \
                        [expr $gpioOut ? 1 : 0]
                }

                set gpioPull [
                    my Expr [lindex $GPIOPadRing $pinNr] & PAD_PUPD_SEL
                ]

                if {$gpioPull} {
                    my PrintMsg MSG_DBG_INFO_PULL_

                    my Switch $gpioPull {
                        PAD_PUPD_SEL_3 {
                            my PrintMsg MSG_DBG_INFO_BUS_
                        }
                        PAD_PUPD_SEL_2 {
                            my PrintMsg MSG_DBG_INFO_DOWN_
                        }
                        PAD_PUPD_SEL_1 {
                            my PrintMsg MSG_DBG_INFO_UP_
                        }
                    }
                }

            } else {
                # GPIO mode 0..5
                set gpioModes [$DPlatform getGPIOModes]

                # get GPIO Data Input
                my Get {GPIO_DATA_INPUT + (( $pinNr / 32 ) << 2 )} gpioInput
                set gpioInput [
                    expr ( $gpioInput >> ( $pinNr & 0x1F ) ) & 0x01
                ] ; # implement inverted;

                # print name and mode
                my PrintfMsg MSG_DBG_INFO_GPIO_MODE \
                    $pinNr \
                    [[lindex $gpioModes $pinNr] getSignal $gpioMode] \
                    $gpioMode

                switch [[lindex $gpioModes $pinNr] getDirection $gpioMode] {
                    0 {
                        my PrintMsg MSG_DBG_INFO_UNUSED
                    }
                    1 {
                        if [my Expr ( [lindex $GPIOPadRing $pinNr] & PAD_REN ) != 0] {
                            my PrintMsg MSG_DBG_NOTE_INPUT_PAD_REN
                        } else {
                            my PrintMsg MSG_DBG_NOTE_INPUT
                        }
                    }
                    2 {
                        my PrintMsg MSG_DBG_NOTE_OUTPUT
                    }
                    3 {
                        if [my Expr ( [lindex $GPIOPadRing $pinNr] & PAD_REN ) != 0] {
                            my PrintMsg MSG_DBG_NOTE_DATA_PAD_REN
                        } else {
                            my PrintMsg MSG_DBG_NOTE_DATA
                        }
                    }
                }

                my Printf "=%s" \
                    [expr {( $gpioInput ^ $gpioDataInv ) != 0 ? 1 : 0 }]

                if {$gpioDataInv} {
                    my PrintfMsg MSG_DBG_NOTE_INVERTED \
                        [expr {$gpioInput != 0 ? 1 : 0}]
                }
                my Print ")"
            }

            #print raw value
            my Get { GPIO_DATA_INPUT_RAW + (( $pinNr / 32 ) << 2) } gpioRawValue
            set gpioRawValue [
                expr ( $gpioRawValue >> ( $pinNr & 0x1F ) ) & 0x01
            ]

            my Printf " raw %s: " [expr $gpioRawValue != 0 ? 1 : 0]

            my Printf { [%s %s] } [
                my Expr {[lindex $GPIOPadRing $pinNr] & PAD_SCHMITT ? "smt" : "---"}
            ] [
                my Expr {[lindex $GPIOPadRing $pinNr] & PAD_SLEW  ? "slw" : "---"}
            ]

            my Print [$DPlatform getVoltage $pinNr]

            # print current
            my Printf "  %dmA\n" [lindex {
                    2 4 6 8
                } [expr {
                    ( [lindex $GPIOPadRing $pinNr] >> 2) & 3
                } ]
            ]
        }

        method FindSignal {gpioModes sigName} {

            for {set pin 0} {$pin < $GPIONumber} {incr pin} {
                for {set mode 0} {$mode < 6} {incr mode} {
                    set modeOfPin [lindex $gpioModes $pin]
                    if {![$modeOfPin getDirection $mode]} {
                        continue
                    }
                    if {[$modeOfPin getSignal $mode] == $sigName} {
                        return [mdbg::gpio::Coordinates new $pin $mode $sigName]
                    }
                }
            }
            my ErrorfMsg MSG_DBG_ERR_UNKNOWN_SIGNAL_NAME $sigName
        }

        # ---------------------------


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
            upvar $varName var
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

        method ScanBitParam {param} {
            set value 0
            if {![string is integer -strict $param]} {
                return -level 2 -code error "expected integer, got \"$param\""
            }
            expr {$param != 0}
        }

        method SET_GPIO_PIN {oldVal newVal pinNumber} {
            expr {
                ((($oldVal) & (~(1 << (($pinNumber) & 0x1F)))) | ($newVal) << (($pinNumber) & 0x1F))
            }
        }

        method WAIT {ms} {
            ::mdbg::cli::Wait::ms $ms
        }

        method Expr {args} {
            DEF upexpr 1 {*}$args
        }

        method Def {name} {
            DEF get $name
        }

        method DefList {args} {
            DEF list $args
        }

        method Switch {expr body} {
            DEF upswitch 1 $expr $body
        }

        method Print {str} {
            puts -nonewline $str
        }

        method Puts {str} {
            puts $str
        }

        method Printf {format args} {
            my Print [format $format {*}$args]
        }

        method Putf {format args} {
            my Puts [format $format {*}$args]
        }

        method PrintMsg {name} {
            my Print [MSG get $name]
        }

        method PutMsg {name} {
            my Puts [MSG get $name]
        }

        method PrintfMsg {name args} {
            my Print [MSG format $name {*}$args]
        }

        method PutfMsg {name args} {
            my Puts [MSG format $name {*}$args]
        }

        method Error {str} {
            return -code error -level 2 $str
        }

        method ErrorMsg {name} {
            return -code error -level 2 [MSG get $name]
        }

        method ErrorfMsg {name args} {
            return -code error -level 2 [MSG format $name {*}$args]
        }

        method INIT_DEF {baseAddress} {

            mdbg::oo::DefineMap create DEF

            DEF add BASE_ADDR $baseAddress

            DEF add GPIO_DATA_OUT               BASE_ADDR + 0x0000
            DEF add GPIO_DATA_INPUT             BASE_ADDR + 0x0020
            DEF add GPIO_DATA_INPUT_RAW         BASE_ADDR + 0x0040
            DEF add GPIO_PAD_CTRL               BASE_ADDR + 0x0188
            DEF add GPIO_CTRL                   BASE_ADDR + 0x0200
            DEF add GPIOPad_RW_Data             BASE_ADDR + 0x0184

            DEF add GPIO_INT_MASK_1             0x20000
            DEF add GPIO_INT_MASK_0             0x10000
            DEF add GPIO_WAKE_MASK              0x08000
            DEF add GPIO_EN_INV                 0x04000
            DEF add GPIO_DATA_INV               0x01000
            DEF add GPIO_DIR                    0x00800 ;# input_enable

            # mode
            DEF add GPIO_MODE_7                 0x00007
            DEF add GPIO_MODE_5                 0x00005
            DEF add GPIO_MODE_4                 0x00004
            DEF add GPIO_MODE_3                 0x00003
            DEF add GPIO_MODE_2                 0x00002
            DEF add GPIO_MODE_1                 0x00001
            DEF add GPIO_MODE_0                 0x00000
            DEF add GPIO_MODE                   0x00007
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

        }

        method INIT_MSG {} {
            mdbg::oo::MessageMap create MSG

            MSG add MSG_DBG_NOTE_DISPLAY_INFORMATION_GPIO \
                {  Displays the information for a GPIO pin pin range or for all of the pins} "\n" \
                {  gpio st[atus] [all] (prints all pins)} "\n" \
                {  gpio st[atus] <pin> (prints the specified pin)} "\n" \
                {  gpio st[atus] <pin> <number> (prints <number> pins starting with <pin>)} "\n"

            MSG add MSG_DBG_NOTE_GPIO_CONFIG \
                {  Configures the behavior of a given GPIO pin.} "\n" \
                {  gpio {configure|cfg|registerMap|} <pin> <cfg1> [<cfg2> ...]} "\n" \
                {    <cfg> can be used for:} "\n" \
                {    MODE       : -m[ode] {0 | 1 | 2 | 3 | 4 | 5 | 7 | direct}} "\n" \
                {    DIRECTION  : -in[put] | -out[put]} "\n" \
                {    OUTPUT     : -data[out] {0 | 1}} "\n" \
                {    POLARITIES : -invoutput {0 | 1} | -invdata {0 | 1}} "\n" \
                {    PULL       : -pull {0 | 1 | 2 | 3 | none | up | down | bus }} "\n" \
                {    RECV       : -recv[en] {0 | 1}} "\n" \
                {    SCHMITT    : {-schmitt | -smt} {0 | 1}} "\n" \
                {    SLEW       : {-slew | -slw} {0 | 1}} "\n" \
                {    VOLTAGE    : {-v | -voltage} {1.8 2.5 3.3 }[vV]} "\n" \
                {    DRIVE      : {-drive} {2 | 4 | 6 | 8}} "\n"

            MSG add MSG_DBG_NOTE_GPIO_SIGNAL \
                {  Looks up and displays the mapping information for a given signal name.} "\n" \
                {  For a list of signals consult the IC technical reference manual} "\n" \
                {  gpio si[gnal] [<all>]              : prints all mapped signals if any} "\n" \
                {  gpio si[gnal] <name> [<name2> ...] : prints the associated pin(s) if mapped} "\n"

            MSG add MSG_DBG_NOTE_GPIO_HELP \
                {  Allows examination and/or alteration of the GPIO pins state and mode} "\n" \
                {  gpio st[atus] [<all> | <pin> | <pin> <number>]} "\n" \
                {  gpio {configure|cfg|registerMap} <pin> <options>} "\n" \
                {  gpio {si|sig|signal} [<all> | <pin> | <name>] } "\n"

            MSG add MSG_DBG_NOTE_DIRECT_CONTROL   "direct control (mode"
            MSG add MSG_DBG_NOTE_MODE             "Mode"
            MSG add MSG_DBG_NOTE_PULL             "Pull = "
            MSG add MSG_DBG_NOTE_NONE             "none\n"
            MSG add MSG_DBG_NOTE_UP               "up\n"
            MSG add MSG_DBG_NOTE_DOWN             "down\n"
            MSG add MSG_DBG_NOTE_BUS              "bus\n"
            MSG add MSG_DBG_NOTE_VOLTAGE          "Voltage = "
            MSG add MSG_DBG_NOTE_DIRECT_ACCESS    "gpio.%03d : direct access %s%s = %s"
            MSG add MSG_DBG_NOTE_OUTPUT           "output"
            MSG add MSG_DBG_NOTE_INPUT_PAD_REN    " input (PAD_REN = 1)"
            MSG add MSG_DBG_NOTE_INPUT            " input"
            MSG add MSG_DBG_NOTE_INVERTED_INPUT   "(inverted input)"
            MSG add MSG_DBG_NOTE_INVERTED         "(%s inverted)"
            MSG add MSG_DBG_NOTE_DATA_PAD_REN     " data (PAD_REN = 1)"
            MSG add MSG_DBG_NOTE_DATA             " data"
            MSG add MSG_DBG_INFO_OUTENABLE        "OutEnable = %d (active zero)."
            MSG add MSG_DBG_INFO_DATA_OUT         "DataOut = %d."
            MSG add MSG_DBG_INFO_OUTUT_INV        "OutputInv = %d."
            MSG add MSG_DBG_INFO_DATA_INV         "DataInv = %d."
            MSG add MSG_DBG_INFO_RECVEN           "RecvEn = %d."
            MSG add MSG_DBG_INFO_SCHMITT          "Schmitt = %d."
            MSG add MSG_DBG_INFO_SLEW             "Slew = %d."
            MSG add MSG_DBG_INFO_DRIVE            "Drive = %smA."
            MSG add MSG_DBG_INFO_PULL_            " pull"
            MSG add MSG_DBG_INFO_UP_              "up"
            MSG add MSG_DBG_INFO_DOWN_            "down"
            MSG add MSG_DBG_INFO_BUS_             "bus"
            MSG add MSG_DBG_INFO_GPIO_MODE        "gpio.%03d : %s\t(mode %d "

            MSG add MSG_DBG_ERR_GPIO_PINS_ARE_0   "GPIO pins are 0 to %d."
            MSG add MSG_DBG_ERR_TOO_MANY_ARGUMENTS_FOR_GPIO_ST_COMMAND     "Too many arguments for 'gpio st' command. Use 'gpio st help' for details."
            MSG add MSG_DBG_INFO_SIGNAL_IS_NOT_MAPPED                      "Signal %s is not mapped."
            MSG add MSG_DBG_ERR_UNKNOWN_SIGNAL_NAME                        "Unknown signal name: %s."

            MSG add MSG_DBG_INFO_PIN_GPIO_CONFIGURATION                             "Pin gpio.%d configuration:"

            MSG add MSG_DBG_ERR_NO_PARAMETER_VALUE_SPECIFIED                        "No parameter value specified."
            MSG add MSG_DBG_ERR_REQUESTED_GPIO_MODE_IS_INVALID                      "Requested GPIO mode is invalid."
            MSG add MSG_DBG_ERR_SIGNAL_IS_UNAVAILABLE_FOR_PIN_GPIO                  "Signal is unavailable for pin gpio.%d in mode %d."
            MSG add MSG_DBG_ERR_INVALID_GPIO_CFG_PARAMETERS                         "Invalid gpio cfg parameters."

            MSG add MSG_DBG_ERR_INVALID_VOLTAGE_VALUE                               "Invalid value for voltage. Only 1.8V is accepted."

        }; # method INIT_MESSAGES

        method UpdatePadRing {} {
            #set GPIOPadRing [lrepeat 142 0]
            set GPIOPadRing [lrepeat $GPIONumber 0]
            # $DPlatform _updatePadRing [self]
            my UpPad {*}[$DPlatform getUpdatePadRingParams]
        }

        method SynchronizePadStep1 {pin} {
            my WaitForThePreviousShiftToComplete value
            variable DPlatform

            # latch out all the values
            my Set GPIO_PAD_CTRL 0x004
            # $DPlatform _syncPad [self] $pin value
            my SyncPad $pin value {*}[$DPlatform getSyncPadParams $pin]
        }

        method SynchronizePadStep2 {pin} {
            my Set GPIOPad_RW_Data [lindex $GPIOPadRing $pin]
            # $DPlatform _dataShifting [self] [expr $pin + 1] value
            my DataShifting value {*}[$DPlatform getDataShiftingParams [expr $pin + 1]]
            my WaitForThePreviousShiftToComplete value
            my Set GPIO_PAD_CTRL 0x002
        }

        method WaitForThePreviousShiftToComplete {valueVar} {
            upvar 1 $valueVar value
            my Get GPIO_PAD_CTRL value
            while {$value & 0x08} {
                my WAIT 10
                my Get GPIO_PAD_CTRL value
            }
        }

        method UpPad {shiftValue maskValue triggerShift} {
            my WaitForThePreviousShiftToComplete value
            # latch out
            my Set GPIO_PAD_CTRL 0x004
            # shift with 3 register lengts
            my Set GPIO_PAD_CTRL $shiftValue

            for {set i 0} {$i < $GPIONumber} {incr i} {

                my WaitForThePreviousShiftToComplete value

                my Get GPIOPad_RW_Data value
                # get the val in the table
                lset GPIOPadRing [expr $GPIONumber - $i - 1] [expr $value & $maskValue]
                # shift again
                my Set GPIO_PAD_CTRL $triggerShift
            }
        }

        method SyncPad {pin valueVar gpioNum maskValue} {
            upvar 1 $valueVar value
            variable DPlatform

            # $DPlatform _dataShifting [self] $gpioNum value
            my DataShifting value {*}[$DPlatform getDataShiftingParams $gpioNum]

            my WaitForThePreviousShiftToComplete value
            my Get GPIOPad_RW_Data pinData

            variable GPIOPadRing
            lset GPIOPadRing $pin [expr $pinData & $maskValue]
        }

        method DataShifting {valueVar gpioNum shiftValue} {
            upvar 1 $valueVar value

            for {set i  0} {$i < $gpioNum} {incr i} {
                my WaitForThePreviousShiftToComplete value
                # shift
                my Set GPIO_PAD_CTRL $shiftValue
            }
        }

    }; # oo::class create GpioInterface

    Myriad2GpioPlatform create myriad2

}; # namespace eval mdbg::gpio

namespace eval mdbg {

    # create gpio command object
    gpio::GpioInterface create gpio mdbg::gpio::myriad2

    mdbg::shell::autocomplete::addScript {
        gpio
    } {
        if {![string length $PARAMS]} {
            tailcall ::mdbg::shell::autocomplete::filterList {
                status configure signal help
                registerMap cfg
            } $PREFIX*
        }
        switch -exact [lindex $PARAMS 0] {
            cfg - configure - registerMap {
                if {[llength $PARAMS] == 1} {
                    tailcall ::mdbg::shell::autocomplete::filterNumberRange 0 [
                        expr {[gpio GetGpioNumber] - 1}
                    ] $PREFIX*
                } else {
                    tailcall ::mdbg::shell::autocomplete::filterList [
                        switch -regexp [lindex $PARAMS end] {
                            ^-?m(ode)? {
                                list 0 1 2 3 4 5 7 direct
                            }
                            ^-?(data([oO]ut)?|inv([oO]ut|[dD]ata)|recv([eE]n)|s(ch)?m(it)?t|sle?w)$ {
                                list 0 1
                            }
                            ^-?pull$ {
                                list none up down bus  0 1 2 3
                            }
                            ^-?v(olt(age))?$ {
                                list 1.8 2.5 3.3
                            }
                            ^-?drive$ {
                                list 2mA 4mA 6mA 8mA
                            }
                            default {
                                set result [list]
                                foreach item {
                                    -mode -input -output -dataout -invoutput -invdata
                                    -pull -recven -schmitt -slew -voltage -drive
                                    -smt -slw -recv
                                } {
                                    if {$item ni $PARAMS} {
                                        lappend result $item
                                    }
                                }
                                set result
                            }
                        }
                    ] $PREFIX*
                }
            }
            st - status {
                tailcall concat [
                    ::mdbg::shell::autocomplete::filterList {
                        all help
                    } $PREFIX*
                ] [
                    ::mdbg::shell::autocomplete::filterNumberRange 0 [
                        expr {[gpio GetGpioNumber] - 1}
                    ] $PREFIX*
                ]
            }
            si - sig - signal {
                tailcall concat [
                    ::mdbg::shell::autocomplete::filterList {
                        all help
                    } $PREFIX*
                ] [
                    gpio GetSignalNames $PREFIX*
                ] [
                    ::mdbg::shell::autocomplete::filterNumberRange 0 [
                         expr {[gpio GetGpioNumber] - 1}
                    ] $PREFIX*
                ]
            }
            help {
                tailcall ::mdbg::shell::autocomplete::filterList {
                    status signal configure
                                  registerMap
                    st     sig    cfg
                           si
                } $PREFIX*
            }
        }

        mdbg::shell::autocomplete::addScript {
            gpio::myriad2
        } {
            if {![string length $PARAMS]} {
                tailcall ::mdbg::shell::autocomplete::listOfMethods $COMMAND $PREFIX*
            }
        }

    }

}
