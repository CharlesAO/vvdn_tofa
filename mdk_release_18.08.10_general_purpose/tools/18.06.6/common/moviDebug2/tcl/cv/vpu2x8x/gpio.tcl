# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : gpio.tcl
# Description: gpio command for MA2x8x
# Created on : Jul 6, 2017
# Author     : Daraban Luminita (luminita.daraban@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::gpio {

    oo::class create Myriad2x8xGpioPlatform {
        # Gpio modes
        variable GPIOModes
        constructor {} {
            ::mdbg::oo::DefineMap create DEF

            DEF add ADDRESS_START_GPIO 0x20250000
            DEF add GPIO_NUMBER 64

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

        method INIT_GPIO_MODES {} {
            set GPIOModes [list]
            # { 3 2 2 2 2 0 2} from documentation: direction of pin: 0-doesn't exist; 1-input; 2-output; 3-input/output
            # documention - trunk-> ...->specification->fragrak_v3_pin_out.xls
            my AddGPIOMode { "spi0_dio_0_mosi"    "cam_mclk"          "lcd_data_16"        "sd_hst1_clk"       "sd_hst0_pwr_a"        ""                  "lcd_data_en"   } { 3 2 2 2 2 0 2}  ;# 0
            my AddGPIOMode { "spi0_dio_1_miso"    "cam_pclk"          "lcd_data_17"        "sd_hst1_cmd"       "sd_hst0_pwr_b"        ""                  "lcd_pclk"      } { 3 1 2 3 2 0 3}  ;# 1
            my AddGPIOMode { "spi0_dio_2"         "cam_vsync"         "lcd_data_18"        "sd_hst1_dat_0"     "spi1_ss_1"            "eth_mdc"           "lcd_vsync"     } { 3 3 2 3 3 2 2}  ;# 2
            my AddGPIOMode { "spi0_dio_3"         "cam_hsync"         "lcd_data_19"        "sd_hst1_dat_1"     "spi1_ss_2"            "eth_mdio"          "lcd_hsync"     } { 3 3 2 3 3 3 2}  ;# 3
            my AddGPIOMode { "spi0_sclk"          "cam_data0"         "lcd_data_20"        "sd_hst1_dat_2"     "sd_hst0_cd"           ""                  "lcd_data_0"    } { 3 1 2 3 1 0 2}  ;# 4
            my AddGPIOMode { "spi0_ss_0"          "cam_data1"         "lcd_data_21"        "sd_hst1_dat_3"     "sd_hst0_wp"           ""                  "lcd_data_1"    } { 3 1 2 3 1 0 2}  ;# 5
            my AddGPIOMode { "spi1_dio_0_mosi"    "cam_data2"         "lcd_data_22"        ""                  "sd_hst0_irq"          ""                  "lcd_data_2"    } { 3 1 2 3 1 0 2}  ;# 6
            my AddGPIOMode { "spi1_dio_1_miso"    "cam_data3"         "lcd_data_23"        ""                  "sd_hst0_mmc_rstn"     ""                  "lcd_data_3"    } { 3 1 2 3 2 0 2}  ;# 7
            my AddGPIOMode { "spi1_dio_2"         "cam_data4"         "lcd_data_24"        ""                  "spi0_ss_1"            "uart0_rts_n"       "lcd_data_4"    } { 3 1 2 3 3 2 2}  ;# 8
            my AddGPIOMode { "spi1_dio_3"         "cam_data5"         "spi3_dio_0_mosi"    ""                  "spi0_ss_2"            "lcd_data_24"       "lcd_data_5"    } { 3 1 3 3 3 2 2}  ;# 9
            my AddGPIOMode { "spi1_sclk"          "cam_data6"         "spi3_dio_1_miso"    "i2s1_sck"          "sd_hst1_cd"           "lcd_data_25"       "lcd_data_6"    } { 3 1 3 3 1 2 2}  ;# 10
            my AddGPIOMode { "spi1_ss_0"          "cam_data7"         "spi3_dio_2"         "i2s1_ws"           "sd_hst1_wp"           "lcd_data_26"       "lcd_data_7"    } { 3 1 3 3 1 2 2}  ;# 11
            my AddGPIOMode { "spi2_dio_0_mosi"    "cam_data8"         "spi3_dio_3"         "i2s1_in_sd0"       "sd_hst1_irq"          "lcd_data_27"       "lcd_data_8"    } { 3 1 3 1 1 2 2}  ;# 12
            my AddGPIOMode { "spi2_dio_1_miso"    "cam_data9"         "spi3_sclk"          "i2s1_out_sd0"      "sd_hst1_mmc_rstn"     "lcd_data_28"       "lcd_data_9"    } { 3 1 3 2 2 2 2}  ;# 13
            my AddGPIOMode { "spi2_dio_2"         "cam_data10"        "spi3_ss_0"          "i2s1_out_sd1"      "spi0_ss_3"            "lcd_data_29"       "eth_mdc"       } { 3 1 3 2 3 2 2}  ;# 14
            my AddGPIOMode { "spi2_dio_3"         "cam_data11"        "uart0_sout"         "i2s1_out_sd2"      "spi1_ss_3"            "lcd_data_30"       "eth_mdio"      } { 3 1 2 2 3 2 3}  ;# 15
            my AddGPIOMode { "spi2_sclk"          "cam_data12"        "uart0_sin"          "i2s1_out_sd3"      "sd_hst1_pwr_a"        "lcd_data_31"       "pcie_clkreq_n" } { 3 1 1 2 2 2 2}  ;# 16
            my AddGPIOMode { "spi2_ss_0"          "cam_data13"        "uart0_cts_n"        "uart0_sout"        "sd_hst1_pwr_b"        "lcd_data_32"       "lcd_data_10"   } { 3 1 1 2 2 2 2}  ;# 17
            my AddGPIOMode { "i2c0_scl"           "cam_data14"        "uart0_rts_n"        "i2s1_in_sd1"       "sd_hst1_pwr_c"        "lcd_data_33"       "lcd_data_11"   } { 2 1 2 1 2 2 2}  ;# 18
            my AddGPIOMode { "i2c0_sda"           "cam_data15"        "uart0_sir_in"       "i2s1_in_sd2"       "sd_hst0_pwr_c"        "lcd_data_34"       "lcd_data_12"   } { 2 1 1 1 2 2 2}  ;# 19
            my AddGPIOMode { "i2c1_scl"           "i2c0_scl"          "uart0_sir_out_n"    "i2s1_in_sd3"       "cam_data16"           "lcd_data_35"       "lcd_data_13"   } { 2 3 2 1 1 2 2}  ;# 20
            my AddGPIOMode { "i2c1_sda"           "i2c0_sda"          "pwm_5"              "spi3_ss_1"         "cam_data17"           "lcd_pwm_0"         "lcd_data_14"   } { 2 3 2 3 1 2 2}  ;# 21
            my AddGPIOMode { "i2c2_scl"           "i2c1_scl"          ""                   "spi3_ss_2"         "cam_data18"           "lcd_pwm_1"         "lcd_data_15"   } { 2 3 0 3 1 2 2}  ;# 22
            my AddGPIOMode { "i2c2_sda"           "i2c1_sda"          "cpr_io_out_clk_0"   "spi3_ss_3"         "cam_data19"           "lcd_pwm_2"         "pcie_wake_n"   } { 2 3 2 3 1 2 2}  ;# 23
            my AddGPIOMode { "i2c3_scl"           "spi0_sclk"         "i2s1_sck"           "spi2_ss_1"         "cam_data20"           ""                  "debug_0"       } { 2 3 3 0 1 0 2}  ;# 24
            my AddGPIOMode { "i2c3_sda"           "spi0_ss_0"         "i2s1_ws"            "spi2_ss_2"         "cam_data21"           ""                  "debug_1"       } { 2 3 3 3 1 0 2}  ;# 25
            my AddGPIOMode { "i2c4_scl"           "spi0_dio_0_mosi"   "i2s1_in_sd0"        "spi2_ss_3"         "cam_data22"           "uart0_sout"        "debug_2"       } { 2 3 1 3 1 2 2}  ;# 26
            my AddGPIOMode { "i2c4_sda"           "spi0_dio_1_miso"   "cpr_io_out_clk_1"   "i2s1_out_sd0"      "cam_data23"           "uart0_sin"         "debug_3"       } { 2 3 2 2 1 1 2}  ;# 27
            my AddGPIOMode { "uart3_sout"         "spi0_dio_2"        "i2s2_sck"           "i2c4_scl"          "i2s3_sck"             "eth_mdc"           "debug_4"       } { 2 3 3 2 3 2 2}  ;# 28
            my AddGPIOMode { "uart3_sin"          "spi0_dio_3"        "i2s2_ws"            "i2c4_sda"          "i2s3_ws"              "eth_mdio"          "debug_5"       } { 1 3 3 2 3 3 2}  ;# 29
            my AddGPIOMode { "uart0_cts_n"        "uart1_sout"        "i2s2_out_sd0"       "lcd_data_22"       "i2s3_in_sd0"          "uart3_cts_n"       "debug_6"       } { 1 2 2 2 1 1 2}  ;# 30
            my AddGPIOMode { "uart0_rts_n"        "uart1_sin"         "i2s2_in_sd0"        "lcd_data_23"       "i2s3_in_sd1"          "uart3_rts_n"       "debug_7"       } { 2 1 1 2 1 2 2}  ;# 31
            my AddGPIOMode { "sd_hst0_clk"        "cpr_io_out_clk_2"  "i2s1_out_sd0"       "spi2_dio_0_mosi"   "i2s3_in_sd2"          "i2c2_sda"          "eth_mdc"       } { 2 2 2 3 1 2 2}  ;# 32
            my AddGPIOMode { "sd_hst0_cmd"        "cpr_io_out_clk_3"  "i2c3_scl"           "spi2_dio_1_miso"   "i2s3_in_sd3"          ""                  "eth_mdio"      } { 3 2 2 3 1 0 3}  ;# 33
            my AddGPIOMode { "sd_hst0_dat_0"      "pwm_0"             "i2c3_sda"           "spi2_dio_2"        "i2s3_out_sd0"         "uart3_sout"        "spi2_sclk"     } { 3 2 2 3 2 2 3}  ;# 34
            my AddGPIOMode { "sd_hst0_dat_1"      "i2c2_scl"          "uart1_cts_n"        "spi2_dio_3"        "i2s3_out_sd1"         "uart3_sin"         "spi2_ss_0"     } { 3 2 1 3 2 1 3}  ;# 35
            my AddGPIOMode { "sd_hst0_dat_2"      "i2c2_sda"          "uart1_rts_n"        "spi2_sclk"         "i2s3_out_sd2"         "i2c0_scl"          "debug_8"       } { 3 2 2 3 2 2 2}  ;# 36
            my AddGPIOMode { "sd_hst0_dat_3"      "pwm_3"             "uart3_sout"         "spi2_ss_0"         "i2s3_out_sd3"         "i2c0_sda"          "debug_9"       } { 3 2 2 3 2 2 2}  ;# 37
            my AddGPIOMode { "sd_hst0_dat_4"      "sd_hst1_clk"       "acif_ext_fsync"     "spi1_dio_0_mosi"   "lcd_data_16"          "cam_data30"        "debug_10"      } { 3 2 3 3 2 1 2}  ;# 38
            my AddGPIOMode { "sd_hst0_dat_5"      "sd_hst1_cmd"       "acif_clk"           "spi1_dio_1_miso"   "lcd_data_17"          "cam_data31"        "debug_11"      } { 3 3 2 3 2 1 2}  ;# 39
            my AddGPIOMode { "sd_hst0_dat_6"      "sd_hst1_dat_0"     "acif_data_0"        "spi1_dio_2"        "lcd_data_18"          "cam_data32"        "debug_12"      } { 3 3 1 3 2 1 2}  ;# 40
            my AddGPIOMode { "sd_hst0_dat_7"      "sd_hst1_dat_1"     "acif_data_1"        "spi1_dio_3"        "lcd_data_19"          "cam_data33"        "debug_13"      } { 3 3 1 3 2 1 2}  ;# 41
            my AddGPIOMode { "uart1_sout"         "sd_hst1_dat_2"     "acif_cmd"           "spi1_sclk"         "lcd_data_20"          "cam_data34"        "debug_14"      } { 2 3 2 3 2 1 2}  ;# 42
            my AddGPIOMode { "uart1_sin"          "sd_hst1_dat_3"     "acif_cal"           "spi1_ss_0"         "lcd_data_21"          "cam_data35"        "debug_15"      } { 1 3 2 3 2 1 2}  ;# 43
            my AddGPIOMode { "sd_hst1_clk"        "lcd_data_en"       "eth_mdc"            "lcd_hsync"         "cpr_io_out_clk_0"     "cam_data24"        "cam_pclk"      } { 2 3 2 2 2 1 1}  ;# 44
            my AddGPIOMode { "sd_hst1_cmd"        "lcd_pclk"          "pwm_2"              "uart2_sout"        "cpr_io_out_clk_2"     "cam_data25"        "cam_vsync"     } { 3 2 2 2 2 1 3}  ;# 45
            my AddGPIOMode { "sd_hst1_dat_0"      "lcd_vsync"         "pwm_3"              "uart2_sin"         "cpr_io_out_clk_3"     "cam_data26"        "cam_hsync"     } { 3 2 2 1 2 1 3}  ;# 46
            my AddGPIOMode { "sd_hst1_dat_1"      "lcd_hsync"         "eth_mdio"           "lcd_data_en"       "cpr_io_out_clk_1"     "cam_data27"        "cam_data0"     } { 3 2 3 2 2 1 1}  ;# 47
            my AddGPIOMode { "sd_hst1_dat_2"      "lcd_data_0"        "pwm_4"              "uart2_cts_n"       "i2s2_sck"             "cam_data28"        "cam_data1"     } { 3 2 2 1 3 1 1}  ;# 48
            my AddGPIOMode { "sd_hst1_dat_3"      "lcd_data_1"        "pwm_5"              "uart2_rts_n"       "i2s2_ws"              "cam_data29"        "cam_data2"     } { 3 2 2 2 3 1 1}  ;# 49
            my AddGPIOMode { "i2s0_sck"           "lcd_data_2"        ""                   "cam_data16"        "i2s2_in_sd0"          "cam_pwm0"          "cam_data3"     } { 3 2 3 1 1 2 1}  ;# 50
            my AddGPIOMode { "i2s0_ws"            "lcd_data_3"        ""                   "cam_data17"        "i2s2_in_sd1"          "cam_pwm1"          "cam_data4"     } { 3 2 3 1 1 2 1}  ;# 51
            my AddGPIOMode { "i2s0_in_sd0"        "lcd_data_4"        ""                   "cam_data18"        "i2s2_in_sd2"          "cam_pwm2"          "cam_data5"     } { 1 2 3 1 1 2 1}  ;# 52
            my AddGPIOMode { "i2s0_in_sd1"        "lcd_data_5"        ""                   "cam_data19"        "i2s2_in_sd3"          "eth_mdc"           "cam_data6"     } { 1 2 3 1 1 2 1}  ;# 53
            my AddGPIOMode { "i2s0_in_sd2"        "lcd_data_6"        "uart3_sout"         "cam_data20"        "i2s2_out_sd0"         "eth_mdio"          "cam_data7"     } { 1 2 2 1 2 3 1}  ;# 54
            my AddGPIOMode { "i2s0_in_sd3"        "lcd_data_7"        "uart3_sin"          "cam_data21"        "i2s2_out_sd1"         "pcie_clkreq_n"     "cam_data8"     } { 1 2 1 1 2 2 1}  ;# 55
            my AddGPIOMode { "i2s0_out_sd0"       "lcd_data_8"        "uart3_cts_n"        "cam_data22"        "i2s2_out_sd2"         "pcie_wake_n"       "cam_data9"     } { 2 2 1 1 2 2 1}  ;# 56
            my AddGPIOMode { "i2s0_out_sd1"       "lcd_data_9"        "uart3_rts_n"        "cam_data23"        "i2s2_out_sd3"         "eth_mdc"           "cam_data10"    } { 2 2 2 1 2 2 1}  ;# 57
            my AddGPIOMode { "i2s0_out_sd2"       "lcd_data_10"       "cpr_io_out_clk_3"   "pwm_5"             "spi3_ss_1"            "eth_mdio"          "cam_data11"    } { 2 2 2 2 3 3 1}  ;# 58
            my AddGPIOMode { "i2s0_out_sd3"       "lcd_data_11"       "pwm_2"              "cpr_io_out_clk_2"  "i2c0_cur_src_en"      "spi0_ss_0"         "cam_mclk"      } { 2 2 2 2 2 3 1}  ;# 59
            my AddGPIOMode { "cpr_io_out_clk_0"   "lcd_data_12"       "pwm_3"              "cpr_io_out_clk_3"  "i2c1_cur_src_en"      "spi0_ss_1"         "dsu_active"    } { 2 2 2 2 2 3 2}  ;# 60
            my AddGPIOMode { "cpr_io_out_clk_1"   "lcd_data_13"       "pwm_4"              "spi1_ss_1"         "i2c2_cur_src_en"      "spi0_ss_2"         "dsu_tstop"     } { 2 2 2 3 2 3 2}  ;# 61
            my AddGPIOMode { "pwm_0"              "lcd_data_14"       "cpr_io_out_clk_2"   "spi1_ss_2"         "i2c3_cur_src_en"      "spi0_ss_3"         "rt_dsu_active" } { 2 2 2 3 2 3 2}  ;# 62
            my AddGPIOMode { "pwm_1"              "lcd_data_15"       "cpr_io_out_clk_3"   "spi1_ss_3"         "i2c4_cur_src_en"      "spi2_ss_1"         "rt_dsu_tstop"  } { 2 2 2 3 2 3 2}  ;# 63
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

        method getGPIOModes {} {
            return $GPIOModes
        }
    }; # oo::class Myriad2GpioPlatform

    oo::class create GpioInterface {

        superclass MA2x5x+_GpioInterfaceBase

        method getVoltage {pinNumber} {
            # info: AON_BLOCK_BASE_ADDR = 0x20260000
            set AON_REG0 0x20260000
            set AON_REG1 [expr {$AON_REG0 + 4}]
            my Get $AON_REG1 aonReg1
            #0-31,38-63  AON_CFG1[8:7] = 3   -> 1.8 V (Default State)
            #32-37 AON_CFG1[10:9] = 0        -> 3.3 V (Default State)
            #      AON_CFG1[10:9] = 3        -> 1.8 V (Optinal config)
            if {($pinNumber >= 32) && ($pinNumber <= 37)} {
                set padsVoltage [expr { ($aonReg1 >> 9) & 0x3}]
            } else {
                set padsVoltage [expr { ($aonReg1 >> 7) & 0x3}]
            }
            if {$padsVoltage == 0} {
                return "3.3V"
            } else {
                return "1.8V"
            }
        }

        method setVoltage {pinNumber value} {
            #0-31,38-63  AON_CFG1[8:7] = 3   -> 1.8 V (Default State)
            #32-37 AON_CFG1[10:9] = 0        -> 3.3 V (Default State)
            #      AON_CFG1[10:9] = 3        -> 1.8 V (Optinal config)
            set AON_REG0 0x20260000
            set AON_REG1 [expr {$AON_REG0 + 4}]
            my Get $AON_REG1 aonReg1
            if {$value == 1.8} {
                set bitValue 3
                if {($pinNumber >= 32) && ($pinNumber <= 37)} {
                    set mask [expr { $bitValue << 9}]
                } else {
                    set mask [expr { $bitValue << 7}]
                }
                set newVal [expr {$aonReg1 | $mask }]
                my Set $AON_REG1 $newVal
            } elseif {$value == 3.3} {
                set bitValue 0
                if {($pinNumber >= 32) && ($pinNumber <= 37)} {
                    set mask [expr { ~(3 << 9)}]
                } else {
                    set mask [expr { ~(3 << 7)}]
                  }
                set newVal [expr {$aonReg1 & $mask }]
                my Set $AON_REG1 $newVal
            } else {
                return -code error "invalid value: must be 1.8 or 3.3"
            }
        }
    }; # oo::class create GpioInterface

    Myriad2x8xGpioPlatform create myriad2

    GpioInterface create interface mdbg::gpio::myriad2

}; # namespace eval mdbg::gpio
