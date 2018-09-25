# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : gpio.tcl
# Description: gpio command for MA2x5x
# Created on : Feb 10, 2016
# Author     : Daraban Luminita (luminita.daraban@movidius.com)
# ***************************************************************************

package require TclOO

namespace eval mdbg::gpio {

    oo::class create Myriad2x5xGpioPlatform {
        # Gpio modes
        variable GPIOModes
        constructor {} {
            ::mdbg::oo::DefineMap create DEF

            DEF add ADDRESS_START_GPIO 0x20E30000
            DEF add GPIO_NUMBER 85

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

            my AddGPIOMode { "cam_mclk"            "i2s1_sck"            "debug_0"               "acif_ext_fsync"      "eth_125_ref_i"       ""                 "Reserved"  "Direct GPIO"} { 2 3 2 3 1 0 3 3}  1  ;# 0
            my AddGPIOMode { "cam_pclk"            "i2s1_ws"             "debug_1"               ""                    "eth_tx_clk"          ""                 "Reserved"  "Direct GPIO"} { 3 3 2 0 1 0 3 3}  2  ;# 1
            my AddGPIOMode { "cam_vsync"           "i2s1_in_sd0"         "debug_2"               "uart_apb_sout"       "eth_tx_en"           ""                 "Reserved"  "Direct GPIO"} { 3 1 2 2 2 0 3 3}  2  ;# 2
            my AddGPIOMode { "cam_hsync"           "i2s1_in_sd1"         "debug_3"               "uart_apb_sin"        "eth_tx_err"          ""                 "Reserved"  "Direct GPIO"} { 3 1 2 1 2 0 3 3}  3  ;# 3
            my AddGPIOMode { "cam_data0"           "i2s1_in_sd2"         "debug_4"               ""                    "eth_rx_clk"          ""                 "Reserved"  "Direct GPIO"} { 1 1 2 0 1 0 3 3}  4  ;# 4
            my AddGPIOMode { "cam_data1"           ""                    "debug_5"               "spi2_ss_out_in_1"    "eth_rx_dv"           ""                 "Reserved"  "Direct GPIO"} { 1 0 2 3 1 0 3 3}  5  ;# 5
            my AddGPIOMode { "cam_data2"           "i2s1_out_sd0"        "debug_6"               "spi2_ss_out_in_2"    "eth_rx_er"           ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 1 0 3 3}  5  ;# 6
            my AddGPIOMode { "cam_data3"           "i2s1_out_sd1"        "debug_7"               "spi2_ss_out_in_3"    "eth_rx_col"          ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 1 0 3 3}  5  ;# 7
            my AddGPIOMode { "cam_data4"           "i2s1_out_sd2"        "debug_8"               "spi2_mosi"           "eth_rx_crs"          ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 1 0 3 3}  5  ;# 8
            my AddGPIOMode { "cam_data5"           "i2c0_cur_src_en"     "debug_9"               "spi2_miso"           "eth_mdio"            ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 3 0 3 3}  6  ;# 9
            my AddGPIOMode { "cam_data6"           "i2c0_scl"            "debug_10"              "spi2_sclk_out"       "eth_mdc"             ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 2 0 3 3}  6  ;# 10
            my AddGPIOMode { "cam_data7"           "i2c0_sda"            "debug_11"              "spi2_ss_out_in_0"    "eth_txd_0"           ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 2 0 3 3}  6  ;# 11
            my AddGPIOMode { "cam_data8"           "uart_apb_rts_n"      "i2c1_scl"              "spi1_mosi"           "eth_txd_1"           ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 2 0 3 3}  1  ;# 12
            my AddGPIOMode { "cam_data9"           "uart_apb_cts_n"      "i2c1_sda"              "spi1_miso"           "eth_txd_2"           ""                 "Reserved"  "Direct GPIO"} { 1 1 2 3 2 0 3 3}  1  ;# 13
            my AddGPIOMode { "cam_data10"          "uart_apb_sin"        "uart_apb_sir_in"       "spi1_sclk_out"       "eth_txd_3"           ""                 "Reserved"  "Direct GPIO"} { 1 1 1 3 2 0 3 3}  1  ;# 14
            my AddGPIOMode { "cam_data11"          "uart_apb_sout"       "uart_apb_sir_out_n"    "spi1_ss_out_in_0"    "eth_txd_4"           ""                 "Reserved"  "Direct GPIO"} { 1 2 2 3 2 0 3 3}  7  ;# 15
            my AddGPIOMode { "cam_data12"          "eth_125_ref_i"       "lcd_data_24"           "sd_hst1_dat_3"       "spi0_ss_out_in_0"    "lcd_data_13"      "Reserved"  "Direct GPIO"} { 1 1 2 3 3 2 3 3}  7  ;# 16
            my AddGPIOMode { "cam_data13"          "eth_tx_clk"          "lcd_data_25"           "sd_hst1_clk"         "spi0_sclk_out"       "lcd_data_14"      "Reserved"  "Direct GPIO"} { 1 1 2 2 3 2 3 3}  7  ;# 17
            my AddGPIOMode { "cam_data14"          "eth_tx_en"           "lcd_data_26"           "sd_hst1_cmd"         "spi0_mosi"           "lcd_data_15"      "Reserved"  "Direct GPIO"} { 1 2 2 3 3 2 3 3}  8  ;# 18
            my AddGPIOMode { "cam_data15"          "eth_tx_err"          "lcd_data_27"           "sd_hst1_dat_0"       "spi0_miso"           "lcd_data_16"      "Reserved"  "Direct GPIO"} { 1 2 2 3 3 2 3 3}  8  ;# 19
            my AddGPIOMode { "cam_data16"          "eth_rx_clk"          "lcd_data_28"           "sd_hst1_dat_1"       "i2c0_scl"            "lcd_data_17"      "Reserved"  "Direct GPIO"} { 1 1 2 3 2 2 3 3}  8  ;# 20
            my AddGPIOMode { "cam_data17"          "eth_rx_dv"           "lcd_data_29"           "sd_hst1_dat_2"       "i2c0_sda"            "lcd_data_18"      "Reserved"  "Direct GPIO"} { 1 1 2 3 2 2 3 3}  7  ;# 21
            my AddGPIOMode { "cam_data18"          "eth_rx_er"           "lcd_data_30"           "sd_hst1_cd"          "eth_gtx_clk"         "lcd_data_19"      "Reserved"  "Direct GPIO"} { 1 1 2 1 2 2 3 3}  8  ;# 22
            my AddGPIOMode { "cam_data19"          "eth_rx_col"          "lcd_data_31"           "sd_hst1_dat_4"       "i2s0_sck"            "lcd_data_20"      "Reserved"  "Direct GPIO"} { 1 1 2 3 3 2 3 3}  8  ;# 23
            my AddGPIOMode { "cam_data20"          "eth_rx_crs"          "lcd_data_32"           "sd_hst1_dat_5"       "i2s0_ws"             "lcd_data_21"      "Reserved"  "Direct GPIO"} { 1 1 2 3 3 2 3 3}  9  ;# 24
            my AddGPIOMode { "cam_data21"          "eth_mdio"            "lcd_data_33"           "sd_hst1_dat_6"       "i2s0_in_sd0"         "lcd_data_22"      "Reserved"  "Direct GPIO"} { 1 3 2 3 1 2 3 3}  9  ;# 25
            my AddGPIOMode { "cam_data22"          "eth_mdc"             "lcd_data_34"           "sd_hst1_dat_7"       "i2s0_out_sd0"        "lcd_data_23"      "Reserved"  "Direct GPIO"} { 1 2 2 3 2 2 3 3}  9  ;# 26
            my AddGPIOMode { "cam_data23"          "eth_txd_0"           "lcd_data_35"           "sd_hst1_wp"          "i2s0_in_sd1"         "pwm_out_1"        "Reserved"  "Direct GPIO"} { 1 2 2 1 1 2 3 3}  9  ;# 27
            my AddGPIOMode { "lcd_mclk"            "eth_gtx_clk"         "cam_mclk"              "i2s0_sck"            "i2s0_out_sd1"        "debug_0"          "Reserved"  "Direct GPIO"} { 2 2 2 3 2 2 3 3} 10  ;# 28
            my AddGPIOMode { "lcd_pclk"            ""                    "cam_pclk"              "i2s0_ws"             ""                    "debug_1"          "Reserved"  "Direct GPIO"} { 3 0 3 3 0 2 3 3} 11  ;# 29
            my AddGPIOMode { "lcd_vsync"           ""                    "cam_vsync"             "i2s0_in_sd0"         ""                    "debug_2"          "Reserved"  "Direct GPIO"} { 2 0 3 1 0 2 3 3} 11  ;# 30
            my AddGPIOMode { "lcd_hsync"           ""                    "cam_hsync"             "i2s0_in_sd1"         "uart_apb_sout"       "debug_3"          "Reserved"  "Direct GPIO"} { 2 0 3 1 2 2 3 3} 10  ;# 31
            my AddGPIOMode { "lcd_data_en"         ""                    "cam_data0"             "i2s0_in_sd2"         ""                    "debug_4"          "Reserved"  "Direct GPIO"} { 2 0 1 1 0 2 3 3} 10  ;# 32
            my AddGPIOMode { "lcd_pwm_0"           ""                    "cam_data1"             "pwm_out_0"           "uart_apb_sin"        "debug_5"          "Reserved"  "Direct GPIO"} { 2 0 1 2 1 2 3 3} 10  ;# 33
            my AddGPIOMode { "lcd_pwm_1"           "acif_ext_fsync"      "cam_data2"             "i2s0_out_sd0"        "uart_apb_sir_in"     "debug_6"          "Reserved"  "Direct GPIO"} { 2 3 1 2 1 2 3 3} 10  ;# 34
            my AddGPIOMode { "lcd_pwm_2"           "eth_gtx_clk"         "cam_data3"             "i2s0_out_sd1"        "uart_apb_sir_out_n"  "debug_7"          "Reserved"  "Direct GPIO"} { 2 2 1 2 2 2 3 3} 12  ;# 35
            my AddGPIOMode { "lcd_data_0"          ""                    "cam_data4"             "i2s0_out_sd2"        ""                    "debug_8"          "Reserved"  "Direct GPIO"} { 2 0 1 2 0 2 3 3} 12  ;# 36
            my AddGPIOMode { "lcd_data_1"          ""                    "cam_data5"             ""                    ""                    "debug_9"          "Reserved"  "Direct GPIO"} { 2 0 1 0 0 2 3 3} 13  ;# 37
            my AddGPIOMode { "lcd_data_2"          ""                    "cam_data6"             "eth_txd_5"           ""                    "debug_10"         "Reserved"  "Direct GPIO"} { 2 0 1 2 0 2 3 3} 14  ;# 38
            my AddGPIOMode { "lcd_data_3"          ""                    "cam_data7"             "eth_txd_6"           "cpr_io_out_clk_1"    "debug_11"         "Reserved"  "Direct GPIO"} { 2 0 1 2 2 2 3 3} 15  ;# 39
            my AddGPIOMode { "lcd_data_4"          "acif_clk"            "cam_data8"             "eth_txd_7"           "cpr_io_out_clk_2"    "debug_12"         "Reserved"  "Direct GPIO"} { 2 2 1 2 2 2 3 3} 12  ;# 40
            my AddGPIOMode { "lcd_data_5"          "acif_cmd"            "cam_data9"             "eth_rxd_0"           "cpr_io_out_clk_3"    "debug_13"         "Reserved"  "Direct GPIO"} { 2 2 1 1 2 2 3 3} 12  ;# 41
            my AddGPIOMode { "lcd_data_6"          "acif_data_0"         "cam_data10"            "eth_rxd_1"           "spi0_sclk_out"       "debug_14"         "Reserved"  "Direct GPIO"} { 2 1 1 1 3 2 3 3} 12  ;# 42
            my AddGPIOMode { "lcd_data_7"          "acif_data_1"         "cam_data11"            "eth_rxd_2"           "uart_apb_cts_n"      "debug_15"         "Reserved"  "Direct GPIO"} { 2 1 1 1 1 2 3 3} 15  ;# 43
            my AddGPIOMode { "lcd_data_8"          "acif_cal"            "cpr_io_out_clk_0"      "eth_rxd_3"           "uart_apb_rts_n"      "pwm_out_2"        "Reserved"  "Direct GPIO"} { 2 2 2 1 2 2 3 3} 14  ;# 44
            my AddGPIOMode { "lcd_data_9"          "sd_hst0_cd"          "pwm_out_0"             "eth_rxd_4"           "spi0_mosi"           ""                 "Reserved"  "Direct GPIO"} { 2 1 2 1 3 0 3 3} 16  ;# 45
            my AddGPIOMode { "lcd_data_10"         "sd_hst0_clk"         "pwm_out_1"             "eth_rxd_5"           "spi0_miso"           ""                 "Reserved"  "Direct GPIO"} { 2 2 2 1 3 0 3 3} 16  ;# 46
            my AddGPIOMode { "lcd_data_11"         "sd_hst0_cmd"         "i2s2_sck"              "eth_rxd_6"           "i2c2_scl"            ""                 "Reserved"  "Direct GPIO"} { 2 3 3 1 2 0 3 3} 12  ;# 47
            my AddGPIOMode { "lcd_data_12"         "sd_hst0_dat_0"       "i2s2_ws"               "eth_rxd_7"           "i2c2_sda"            ""                 "Reserved"  "Direct GPIO"} { 2 3 3 1 2 0 3 3} 12  ;# 48
            my AddGPIOMode { "lcd_data_13"         "sd_hst0_dat_1"       "i2s2_in_sd0"           "pwm_out_3"           "i2c1_scl"            ""                 "Reserved"  "Direct GPIO"} { 2 3 1 2 2 0 3 3} 17  ;# 49
            my AddGPIOMode { "lcd_data_14"         "sd_hst0_dat_2"       "i2s2_in_sd1"           "pwm_out_4"           "i2c1_sda"            ""                 "Reserved"  "Direct GPIO"} { 2 3 1 2 2 0 3 3} 17  ;# 50
            my AddGPIOMode { "lcd_data_15"         "sd_hst0_dat_3"       "i2s2_in_sd2"           "pwm_out_5"           "uart_apb_sout"       ""                 "Reserved"  "Direct GPIO"} { 2 3 1 2 2 0 3 3} 17  ;# 51
            my AddGPIOMode { "lcd_data_16"         "sd_hst0_dat_4"       "cpr_io_out_clk_3"      "acif_clk"            "i2c1_cur_src_en"     ""                 "Reserved"  "Direct GPIO"} { 2 3 2 2 0 0 3 3} 17  ;# 52
            my AddGPIOMode { "lcd_data_17"         "sd_hst0_dat_5"       "i2s2_out_sd0"          "acif_cmd"            "spi2_ss_out_in_1"    ""                 "Reserved"  "Direct GPIO"} { 2 3 2 2 3 0 3 3} 18  ;# 53
            my AddGPIOMode { "lcd_data_18"         "sd_hst0_dat_6"       "i2s2_out_sd1"          "acif_data_0"         "spi0_ss_out_in_1"    ""                 "Reserved"  "Direct GPIO"} { 2 3 2 1 3 0 3 3} 18  ;# 54
            my AddGPIOMode { "lcd_data_19"         "sd_hst0_dat_7"       "i2s2_out_sd2"          "acif_data_1"         "spi2_ss_out_in_2"    ""                 "Reserved"  "Direct GPIO"} { 2 3 2 1 3 0 3 3} 19  ;# 55
            my AddGPIOMode { "lcd_data_20"         "sd_hst0_wp"          ""                      "acif_cal"            "spi0_ss_out_in_3"    ""                 "Reserved"  "Direct GPIO"} { 2 1 0 2 3 0 3 3} 19  ;# 56
            my AddGPIOMode { "lcd_data_21"         "sd_hst0_irq"         "cpr_io_out_clk_1"      "i2c2_cur_src_en"     "spi0_ss_out_in_2"    ""                 "Reserved"  "Direct GPIO"} { 2 1 2 0 3 0 3 3} 18  ;# 57
            my AddGPIOMode { "lcd_data_22"         "sd_hst0_mmc_rstn"    "cpr_io_out_clk_2"      "spi2_ss_out_in_3"    "spi0_sclk_out"       ""                 "Reserved"  "Direct GPIO"} { 2 2 2 3 3 0 3 3} 18  ;# 58
            my AddGPIOMode { "lcd_data_23"         "sd_hst0_pwr_a"       "spi1_ss_out_in_1"      "cpr_io_out_clk_0"    "spi0_ss_out_in_0"    ""                 "Reserved"  "Direct GPIO"} { 2 2 3 2 3 0 3 3} 18  ;# 59
            my AddGPIOMode { "i2c0_scl"            "sd_hst0_pwr_b"       "spi1_ss_out_in_2"      "spi2_mosi"           "eth_gtx_clk"         ""                 "Reserved"  "Direct GPIO"} { 2 2 3 3 2 0 3 3} 18  ;# 60
            my AddGPIOMode { "i2c0_sda"            "sd_hst0_pwr_c"       "spi1_ss_out_in_3"      "spi2_miso"           ""                    ""                 "Reserved"  "Direct GPIO"} { 2 2 3 3 0 0 3 3} 20  ;# 61
            my AddGPIOMode { "i2c1_scl"            "eth_txd_1"           "sd_hst0_cd"            "spi2_sclk_out"       "pwm_out_3"           ""                 "Reserved"  "Direct GPIO"} { 2 2 1 3 2 0 3 3} 20  ;# 62
            my AddGPIOMode { "i2c1_sda"            "eth_txd_2"           "sd_hst0_clk"           "spi2_ss_out_in_0"    "pwm_out_4"           ""                 "Reserved"  "Direct GPIO"} { 2 2 2 3 2 0 3 3} 21  ;# 63
            my AddGPIOMode { "i2s0_sck"            "eth_txd_3"           "sd_hst0_cmd"           "lcd_mclk"            "cpr_io_out_clk_0"    ""                 "Reserved"  "Direct GPIO"} { 3 2 3 2 2 0 3 3} 21  ;# 64
            my AddGPIOMode { "i2s0_ws"             "eth_txd_4"           "sd_hst0_dat_0"         "lcd_pclk"            "cam_data12"          ""                 "Reserved"  "Direct GPIO"} { 3 2 3 3 1 0 3 3} 22  ;# 65
            my AddGPIOMode { "i2s0_in_sd0"         "eth_txd_5"           "sd_hst0_dat_1"         "lcd_vsync"           "cam_data13"          ""                 "Reserved"  "Direct GPIO"} { 1 2 3 2 1 0 3 3} 23  ;# 66
            my AddGPIOMode { "i2s0_in_sd1"         "eth_txd_6"           "sd_hst0_dat_2"         "lcd_hsync"           "cam_data14"          ""                 "Reserved"  "Direct GPIO"} { 1 2 3 2 1 0 3 3} 24  ;# 67
            my AddGPIOMode { "i2s0_in_sd2"         "eth_txd_7"           "sd_hst0_dat_3"         "lcd_data_en"         "cam_data15"          ""                 "Reserved"  "Direct GPIO"} { 1 2 3 2 1 0 3 3} 24  ;# 68
            my AddGPIOMode { "i2c1_sda"            "eth_rxd_0"           "sd_hst0_dat_4"         "lcd_pwm_0"           "cam_data16"          ""                 "Reserved"  "Direct GPIO"} { 2 1 3 2 1 0 3 3} 25  ;# 69
            my AddGPIOMode { "i2s0_out_sd0"        "eth_rxd_1"           "sd_hst0_dat_5"         "lcd_pwm_1"           "cam_data17"          "rt_dsu_active"    "Reserved"  "Direct GPIO"} { 2 1 3 2 1 2 3 3} 25  ;# 70
            my AddGPIOMode { "i2s0_out_sd1"        "eth_rxd_2"           "sd_hst0_dat_6"         "lcd_pwm_2"           "cam_data18"          "rt_dsu_tstop"     "Reserved"  "Direct GPIO"} { 2 1 3 2 1 2 3 3} 26  ;# 71
            my AddGPIOMode { "i2s0_out_sd2"        "eth_rxd_3"           "sd_hst0_dat_7"         "lcd_data_0"          "cam_data19"          "dsu_active"       "Reserved"  "Direct GPIO"} { 2 1 3 2 1 2 3 3} 26  ;# 72
            my AddGPIOMode { "spi1_ss_out_in_0"    "eth_rxd_4"           "sd_hst0_wp"            "lcd_data_1"          "cam_data20"          "dsu_tstop"        "Reserved"  "Direct GPIO"} { 3 1 1 2 1 2 3 3} 27  ;# 73
            my AddGPIOMode { "spi0_mosi"           "eth_rxd_5"           "sd_hst0_irq"           "lcd_data_2"          "cam_data21"          "pwm_out_4"        "Reserved"  "Direct GPIO"} { 3 1 1 2 1 2 3 3} 28  ;# 74
            my AddGPIOMode { "spi0_miso"           "eth_rxd_6"           "sd_hst0_mmc_rstn"      "lcd_data_3"          "cam_data22"          "pwm_out_5"        "Reserved"  "Direct GPIO"} { 3 1 2 2 1 2 3 3} 28  ;# 75
            my AddGPIOMode { "spi0_sclk_out"       "eth_rxd_7"           "sd_hst0_pwr_a"         "lcd_data_4"          "cam_data23"          ""                 "Reserved"  "Direct GPIO"} { 3 1 2 2 1 0 3 3} 29  ;# 76
            my AddGPIOMode { "spi0_ss_out_in_0"    "pwm_out_0"           "sd_hst0_pwr_b"         "lcd_data_5"          "i2c1_scl"            "uart_apb_sin"     "Reserved"  "Direct GPIO"} { 3 2 2 2 2 1 3 3} 29  ;# 77
            my AddGPIOMode { "spi1_mosi"           "sd_hst1_irq"         "sd_hst0_pwr_c"         "lcd_data_6"          "i2c1_sda"            "spi0_ss_out_in_1" "Reserved"  "Direct GPIO"} { 3 2 2 2 2 3 3 3} 30  ;# 78
            my AddGPIOMode { "spi1_miso"           "sd_hst1_mmc_rstn"    "i2c2_scl"              "lcd_data_7"          "debug_12"            "spi0_ss_out_in_2" "Reserved"  "Direct GPIO"} { 3 2 2 2 2 3 3 3} 30  ;# 79
            my AddGPIOMode { "spi1_sclk_out"       "sd_hst1_pwr_a"       "i2c2_sda"              "lcd_data_8"          "debug_13"            ""                 "Reserved"  "Direct GPIO"} { 3 2 2 2 2 0 3 3} 31  ;# 80
            my AddGPIOMode { "spi1_ss_out_in_0"    "sd_hst1_pwr_b"       "cpr_io_out_clk_1"      "lcd_data_9"          "spi0_ss_out_in_3"    ""                 "Reserved"  "Direct GPIO"} { 3 2 0 2 3 0 3 3} 32  ;# 81
            my AddGPIOMode { "spi1_ss_out_in_1"    "sd_hst1_pwr_c"       "cam_pwm_0"             "lcd_data_10"         "pwm_out_3"           "i2s2_out_sd2"     "Reserved"  "Direct GPIO"} { 3 2 2 2 2 2 3 3} 33  ;# 82
            my AddGPIOMode { "spi1_ss_out_in_2"    "cpr_io_out_clk_0"    "cam_pwm_1"             "lcd_data_11"         "debug_14"            "i2s2_out_sd0"     "Reserved"  "Direct GPIO"} { 3 2 2 2 2 2 3 3} 34  ;# 83
            my AddGPIOMode { "spi1_ss_out_in_3"    "pwm_out_2"           "cam_pwm_2"             "lcd_data_12"         "debug_15"            "i2s2_out_sd1"     "Reserved"  "Direct GPIO"} { 3 2 2 2 2 2 3 3} 35  ;# 84
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
            # info: AON_BLOCK_BASE_ADDR = 0x20ef0000
            set AON_REG0 0x20ef0000
            my Get $AON_REG0 aonReg0
            if {($pinNumber >= 16) && ($pinNumber <= 21)} {
                set padsVoltage [expr { ($aonReg0 >> 23) & 0x1}]
            } else {
                set padsVoltage [expr { ($aonReg0 >> 22) & 0x1}]
            }
            if {$padsVoltage == 1} {
                return "1.8V"
            } elseif {$padsVoltage == 0} {
                return "3.3V"
            }
        }

        method setVoltage {pinNumber value} {
            if {$value == 1.8} {
                set bitValue 1
            } elseif {$value == 3.3} {
                set bitValue 0
            }
            set AON_REG0 0x20ef0000
            my Get $AON_REG0 aonReg0
            if {($pinNumber >= 16) && ($pinNumber <= 21)} {
                my SetBit aonReg0 23 $bitValue
                my Set $AON_REG0 $aonReg0
            } else {
                my SetBit aonReg0 22 $bitValue
                my Set $AON_REG0 $aonReg0
            }
        }
    }; # oo::class create GpioInterface

    Myriad2x5xGpioPlatform create myriad2

    GpioInterface create interface mdbg::gpio::myriad2

}; # namespace eval mdbg::gpio
