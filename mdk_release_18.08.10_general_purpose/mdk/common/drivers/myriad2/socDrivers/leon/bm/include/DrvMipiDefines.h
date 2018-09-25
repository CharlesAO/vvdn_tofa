///
/// @file DrvMipiDefines.h
/// 
/// 
/// @ingroup DrvMipi
/// @{
///

#ifndef DRV_MIPI_DEFINES_H
#define DRV_MIPI_DEFINES_H

#include <mv_types.h>

// capabilities
#define DRV_MIPI_NO_CTRL   (6)
#define DRV_MIPI_NO_DPHY   (6)

#define DRV_MIPI_C0 (0)
#define DRV_MIPI_C1 (1)
#define DRV_MIPI_C2 (2)
#define DRV_MIPI_C3 (3)
#define DRV_MIPI_C4 (4)
#define DRV_MIPI_C5 (5)

#define DRV_MIPI_P0 (0)
#define DRV_MIPI_P1 (1)
#define DRV_MIPI_P2 (2)
#define DRV_MIPI_P3 (3)
#define DRV_MIPI_P4 (4)
#define DRV_MIPI_P5 (5)


// number of D-PHYs  connected to each controller
#define DRV_MIPI_C0_NO_RX  (4)
#define DRV_MIPI_C1_NO_RX  (1)
#define DRV_MIPI_C2_NO_RX  (2)
#define DRV_MIPI_C3_NO_RX  (3)
#define DRV_MIPI_C4_NO_RX  (2)
#define DRV_MIPI_C5_NO_RX  (1)

#define DRV_MIPI_C0_NO_TX  (0)
#define DRV_MIPI_C1_NO_TX  (0)
#define DRV_MIPI_C2_NO_TX  (0)
#define DRV_MIPI_C3_NO_TX  (2)
#define DRV_MIPI_C4_NO_TX  (0)
#define DRV_MIPI_C5_NO_TX  (2)


// which D-PHYs are available for each controller, saved as bit positions
#define DRV_MIPI_C0_RX_DPHYS  ((1<<0) | (1<<1) | (1<<2) | (1<<3))  //  DPHYs 0,1,2,3
#define DRV_MIPI_C1_RX_DPHYS  ((1<<1))                             //  DPHY  1
#define DRV_MIPI_C2_RX_DPHYS  ((1<<2) | (1<<3))                    //  DPHYs 2,3
#define DRV_MIPI_C3_RX_DPHYS  ((1<<3) | (1<<4) | (1<<5))           //  DPHYs 3, 4, 5
#define DRV_MIPI_C4_RX_DPHYS  ((1<<4) | (1<<5))                    //  DPHYs 4, 5
#define DRV_MIPI_C5_RX_DPHYS  ((1<<5))                             //  DPHY  5

#define DRV_MIPI_C0_TX_DPHYS  (0)
#define DRV_MIPI_C1_TX_DPHYS  (0)
#define DRV_MIPI_C2_TX_DPHYS  (0)
#define DRV_MIPI_C3_TX_DPHYS  ((1<<2) | (1<<3))                    //  DPHYs 2,3
#define DRV_MIPI_C4_TX_DPHYS  (0)
#define DRV_MIPI_C5_TX_DPHYS  ((1<<4) | (1<<5))                    //  DPHYs 4,5





// D-PHY associations also have some rules, besides the defines
// RULE:
// for RX ctrl X, lane 0 is on D-PHY X

// For tx not so simple :
// for TX ctrl 3, lane 0 is on D-PHY 2
// for TX ctrl 5, lane 0 is on D-PHY 5, and as lane number increases, D-PHY number decreases

// number of lanes, all possible settings for each controller, saved as bit positions; ex: if it works with 1 and 2 lanes only, value stored is 0x06;
#define DRV_MIPI_C0_RX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4) | (1<<6) | (1<<8)) // 1, 2, 4, 6, 8
#define DRV_MIPI_C1_RX_LANE_CFG  ((1<<1) | (1<<2))                            // 1, 2
#define DRV_MIPI_C2_RX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4))                   // 1, 2, 4
#define DRV_MIPI_C3_RX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4) | (1<<6))          // 1, 2, 4, 6
#define DRV_MIPI_C4_RX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4))                   // 1, 2, 4
#define DRV_MIPI_C5_RX_LANE_CFG  ((1<<1) | (1<<2))                            // 1, 2

#define DRV_MIPI_C0_TX_LANE_CFG  (0) // no Tx
#define DRV_MIPI_C1_TX_LANE_CFG  (0) // no Tx
#define DRV_MIPI_C2_TX_LANE_CFG  (0) // no Tx
#define DRV_MIPI_C3_TX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4))                   // 1, 2, 4
#define DRV_MIPI_C4_TX_LANE_CFG  (0) // no Tx
#define DRV_MIPI_C5_TX_LANE_CFG  ((1<<1) | (1<<2) | (1<<4))                   // 1, 2, 4

#define D_DRV_MIPI_CTRL_MIN_TRAN_RATE_MBPS (80)
#define D_DRV_MIPI_CTRL_MAX_TRAN_RATE_MBPS (1500)

#define D_DRV_MIPI_PLL_LOCK_TIMEOUT (50000)           // 0 means wait forever 

#define D_DRV_MIPI_CTRL_INITILISED (0x68797869)   // used to mark that the init struct was used to initialize the controller.
/* --------------------------------------------------------------------------------------------------------------------------------------
                                                             test commands for the PHY
------------------------------------------------------------------------------------------------------------------------------------------ */
#define D_MIPI_TCMD_NORMAL                0x00  // No test; Normal Operation
#define D_MIPI_TCMD_LP_WUP                0x01  // 0x01 LP Driver Wake Up Timer Counter after Exiting ULPS
#define D_MIPI_TCMD_LP_TXDRV2EN_LP11      0x02  // 0x02 Delay between LP TX Driver Enable and Start Driving LP = 11 during Initialization Timer Control
#define D_MIPI_TCMD_EN2START_CAL          0x03  // 0x03 Delay between Enable Calibration and Starting Calibration Timer Control
#define D_MIPI_TCMD_REL_CTRL_PHY_TX       0x04  // 0x04 Relinquish Control if PHY is Configured as TX
#define D_MIPI_TCMD_TTA_GO_TIM_CTR        0x05  // 0x05 Tta-go Timer Counter
#define D_MIPI_TCMD_TTA_SURE_TIM_CTR      0x06  // 0x06 Tta-sure Timer Counter
#define D_MIPI_TCMD_TURN_REQ_DLY_CTRL     0x07  // 0x07 Turnaround Request Delay Control
#define D_MIPI_TCMD_DLY_HS_PON_EN_CAL     0x08  //0x08 Delay between HS Receiver Power On and Enabling Calibration Timer Control
#define D_MIPI_TCMD_REL_CTRL_PHY_RX       0x09  // 0x09 Relinquish Control when PHY is Configured as RX
#define D_MIPI_TCMD_SS_WDOG_TIM_CTRL      0x0A  // 0x0A Stop State Watchdog Timer Control
#define D_MIPI_TCMD_SS_WDOG_TIM_EN_CD     0x0C  // 0x0C Stop State Watchdog Timer Enable and Contention Detection
#define D_MIPI_TCMD_BIST                  0x0D  // 0x0D BIST Mode
#define D_MIPI_TCMD_PLL_CFG1              0x10  // 0x10 PLL Bias Current Selector/Filter Capacitance Control/VCO Control
#define D_MIPI_TCMD_PLL_CFG2              0x11  // 0x11 PLL CP Control / PLL Lock Bypass for Initialization and for ULP
#define D_MIPI_TCMD_PLL_CFG3              0x12  // 0x12 PLL LPF and CP Control
#define D_MIPI_TCMD_PLL_DIG_TEST          0x13  // 0x13 PLL Digital Testability
#define D_MIPI_TCMD_PLL_PH_ER_CTRL        0x14  // 0x14 PLL Phase Error Control
#define D_MIPI_TCMD_PLL_LOCKING_FIL       0x15  // 0x15 PLL Locking Filter
#define D_MIPI_TCMD_PLL_UNLOCKING_FIL     0x16  // 0x16 PLL Unlocking Filter
#define D_MIPI_TCMD_PLL_IN_DIV            0x17  // 0x17 PLL Input Divider Ratio
#define D_MIPI_TCMD_PLL_LOOP_DIV          0x18  // 0x18 PLL Loop Divider Ratio
#define D_MIPI_TCMD_PLL_IN_LOOP_DIV       0x19  // 0x19 PLL Input and Loop Divider Ratios Control
#define D_MIPI_TCMD_BANDGAP_BIAS_CTRL     0x20  // 0x20 Bandgap and Bias Control
#define D_MIPI_TCMD_TERM_REZI_CTRL        0x21  // 0x21 Termination Resistor Control
#define D_MIPI_TCMD_AFE_BIAS_BGAP_PROG    0x22  // 0x22 AFE/BIAS/Bandgap Analog Programmability
#define D_MIPI_TCMD_HS_TX_BPC_CL          0x30  // 0x30 HS TX and Bias Power on Control of Clock Lane
#define D_MIPI_TCMD_LP_RX_CTRL_CL         0x31  // 0x31 LP RX Control of Clock Lane
#define D_MIPI_TCMD_LP_TX_CTRL_CL_0       0x32  // 0x32 LP TX Control of Clock Lane
#define D_MIPI_TCMD_LP_TX_CTRL_CL_1       0x33  // 0x33 LP TX Control of Clock Lane
#define D_MIPI_TCMD_HS_RX_CTRL_CL         0x34  // 0x34 HS RX Control of Clock Lane
#define D_MIPI_TCMD_SWAP_CLKP_CLKN        0x35  // 0x35 CLKP/CLKN Swap for Clock Lane + Tclk_miss Control
#define D_MIPI_TCMD_CAL_M_OUT_CL          0x36  // 0x36 Calibration Machine Outputs Observability of Clock Lane
#define D_MIPI_TCMD_HS_TX_BPC_L0          0x40  // 0x40 HS TX and Bias Power on Control of Lane 0
#define D_MIPI_TCMD_LP_RX_CTRL_L0         0x41  // 0x41 LP RX Control of Lane 0
#define D_MIPI_TCMD_LP_TX_CTRL_L0_0       0x42  // 0x42 LP TX Control of Lane 0
#define D_MIPI_TCMD_LP_TX_CTRL_L0_1       0x43  // 0x43 LP TX Control of Lane 0
#define D_MIPI_TCMD_HS_RX_CTRL_L0         0x44  // 0x44 HS RX Control of Lane 0
#define D_MIPI_TCMD_SWAP_DATAP_DATAN_L0   0x45  // 0x45 DATAP/DATAN Swap for Lane 0
#define D_MIPI_TCMD_CAL_M_OUT_L0          0x46  // 0x46 HS RX Lane 0 Outputs and Calibration Errors Observability
#define D_MIPI_TCMD_BIST_L0               0x47  // 0x47 BIST Control and Observability on Lane 0
#define D_MIPI_TCMD_HS_TX_BPC_L1          0x50  // 0x50 HS TX and Bias Power on Control of Lane 1
#define D_MIPI_TCMD_LP_RX_CTRL_L1         0x51  // 0x51 LP RX Control of Lane 1
#define D_MIPI_TCMD_LP_TX_CTRL_L1_0       0x52  // 0x52 LP TX Control of Lane 1
#define D_MIPI_TCMD_LP_TX_CTRL_L1_1       0x53  //   0x53 LP TX Control of Lane 1
#define D_MIPI_TCMD_HS_RX_CTRL_L1         0x54  // 0x54 HS RX Control of Lane 1
#define D_MIPI_TCMD_SWAP_DATAP_DATAN_L1   0x55  // 0x55 DATAP/DATAN Swap for Lane 1
#define D_MIPI_TCMD_CAL_M_OUT_L1          0x56  // 0x56 HS RX Lane 1 Outputs and Calibration Errors Observability
#define D_MIPI_TCMD_BIST_L1               0x57  // 0x57 BIST Control and Observability on Lane 1
#define D_MIPI_TCMD_HS_TX_CL_REQ_ST       0x60  // 0x60 HS TX Clock Lane Request State Time (TLP) Control
#define D_MIPI_TCMD_HS_TX_CL_PREP_ST      0x61  // 0x61 HS TX Clock Lane Prepare State Time (TCLK-prepare) Control
#define D_MIPI_TCMD_HS_TX_CL_HS_ZERO_ST   0x62  // 0x62 HS TX Clock Lane HS-Zero State Time (TCLK-zero) Control
#define D_MIPI_TCMD_HS_TX_CL_TRL_ST       0x63  // 0x63 HS TX Clock Lane Trail State Time (TCLKtrail) Control
#define D_MIPI_TCMD_HS_TX_CL_EXT_ST       0x64  // 0x64 HS TX Clock Lane Exit State Time (THS-exit) Control
#define D_MIPI_TCMD_HS_TX_CL_CLK_PST_T    0x65  // 0x65 HS TX Clock Lane Clock Post Time (TCLKpost) Control
#define D_MIPI_TCMD_HS_TX_DAT_REQ_ST      0x70  // 0x70 HS TX Data Lane Request State Time (TLP) Control
#define D_MIPI_TCMD_HS_TX_DAT_PREP_ST     0x71  // 0x71 HS TX Data Lanes Prepare State Time (THSprepare) Control
#define D_MIPI_TCMD_HS_TX_DAT_HS_ZERO_ST  0x72  // 0x72 HS TX Data Lanes HS-Zero State Time (THS-zero) Control
#define D_MIPI_TCMD_HS_TX_DAT_TRL_ST      0x73  // 0x73 HS TX Data Lanes Trail State Time (THS-trail) Control
#define D_MIPI_TCMD_HS_TX_DAT_EXT_ST      0x74  // 0x74 HS TX Data Lanes Exit State Time (THS-exit) Control
#define D_MIPI_TCMD_HS_TX_DAT_SATTLE_ST   0x75  // 0x75 HS RX Data Lanes Settle State Time (THSsettle) Control
#define D_MIPI_TCMD_HS_TX_PROG_OUT_D_CTRL 0x76  // 0x76 HS TX Programmable Output Delay Control


/* --------------------------------------------------------------------------------------------------------------------------------------
                                                             D-PHY CONTROL REGISTERS
------------------------------------------------------------------------------------------------------------------------------------------ */


// ============================================================= MIPI_DPHY_INIT_CTRL
//bit 0
#define D_MIPI_DPHY_INIT_CTRL_EN_0   (1 << 0)
#define D_MIPI_DPHY_INIT_CTRL_EN_1   (1 << 1)
#define D_MIPI_DPHY_INIT_CTRL_EN_2   (1 << 2)
#define D_MIPI_DPHY_INIT_CTRL_EN_3   (1 << 3)
#define D_MIPI_DPHY_INIT_CTRL_EN_4   (1 << 4)
#define D_MIPI_DPHY_INIT_CTRL_EN_5   (1 << 5)
//bits 6 - 7 reserved
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_0   (1 << 8)
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_1   (1 << 9)
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_2   (1 << 10)
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_3   (1 << 11)
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_4   (1 << 12)
#define D_MIPI_DPHY_INIT_CTRL_RSTZ_5   (1 << 13)
//bits 14-15 reserved
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_0   (1 << 16)
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_1   (1 << 17)
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_2   (1 << 18)
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_3   (1 << 19)
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_4   (1 << 20)
#define D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_5   (1 << 21)
//bits 22-23 reserved
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_0   (1 << 24)
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_1   (1 << 25)
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_2   (1 << 26)
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_3   (1 << 27)
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_4   (1 << 28)
#define D_MIPI_DPHY_INIT_CTRL_CLK_CONT_5   (1 << 29)

// ============================================================= MIPI_DPHY_FORCE_CTRL
//bit 0
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L0     (1<<0)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L1     (1<<1)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L2     (1<<2)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L3     (1<<3)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L4     (1<<4)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L5     (1<<5)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L6     (1<<6)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L7     (1<<7)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L8     (1<<8)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L9     (1<<9)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L10    (1<<10)
#define D_MIPI_DPHY_FORCE_CTRL_FTXSTOP_L11    (1<<11)
//bits 12- 15 reserved - doc is wrong
#define D_MIPI_DPHY_FORCE_CTRL_FRX_0    (1<<16)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_1    (1<<17)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_2    (1<<18)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_3    (1<<19)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_4    (1<<20)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_5    (1<<21)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_6    (1<<22)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_7    (1<<23)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_8    (1<<24)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_9    (1<<25)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_10   (1<<26)
#define D_MIPI_DPHY_FORCE_CTRL_FRX_11   (1<<27)
// 28- 29 reserved
#define D_MIPI_DPHY_FORCE_CTRL_FPLL_0    (1<<30)
#define D_MIPI_DPHY_FORCE_CTRL_FPLL_1    (1<<31)

// =============================================================  MIPI_DPHY_TURN_CTRL
//bit 0
#define D_MIPI_DPHY_TURN_CTRL_L0  (1<<0)
#define D_MIPI_DPHY_TURN_CTRL_L1  (1<<1)
#define D_MIPI_DPHY_TURN_CTRL_L2  (1<<2)
#define D_MIPI_DPHY_TURN_CTRL_L3  (1<<3)
#define D_MIPI_DPHY_TURN_CTRL_L4  (1<<4)
#define D_MIPI_DPHY_TURN_CTRL_L5  (1<<5)
#define D_MIPI_DPHY_TURN_CTRL_L6  (1<<6)
#define D_MIPI_DPHY_TURN_CTRL_L7  (1<<7)
#define D_MIPI_DPHY_TURN_CTRL_L8  (1<<8)
#define D_MIPI_DPHY_TURN_CTRL_L9  (1<<9)
#define D_MIPI_DPHY_TURN_CTRL_L10 (1<<10)
#define D_MIPI_DPHY_TURN_CTRL_L11 (1<<11)

// ============================================================= MIPI_DPHY_STAT_DPHY01
// MIPI_DPHY_STAT_DPHY23
// MIPI_DPHY_STAT_DPHY45
//bit 0
#define D_MIPI_DPHY_STAT_P0_CLK_ULPS      (1<<0)
#define D_MIPI_DPHY_STAT_P0_CLK_STOPS     (1<<1)
#define D_MIPI_DPHY_STAT_P0_L0_ULPS       (1<<2)
#define D_MIPI_DPHY_STAT_P0_L1_ULPS       (1<<3)
#define D_MIPI_DPHY_STAT_P0_L0_STOPS      (1<<4)
#define D_MIPI_DPHY_STAT_P0_L1_STOPS      (1<<5)
#define D_MIPI_DPHY_STAT_P0_L0_DIR        (1<<6)
#define D_MIPI_DPHY_STAT_P0_L1_DIR        (1<<7)
// bit 8 - the other phy
#define D_MIPI_DPHY_STAT_P1_CLK_ULPS      (1<<8)
#define D_MIPI_DPHY_STAT_P1_CLK_STOPS     (1<<9)
#define D_MIPI_DPHY_STAT_P1_L0_ULPS       (1<<10)
#define D_MIPI_DPHY_STAT_P1_L1_ULPS       (1<<11)
#define D_MIPI_DPHY_STAT_P1_L0_STOPS      (1<<12)
#define D_MIPI_DPHY_STAT_P1_L1_STOPS      (1<<13)
#define D_MIPI_DPHY_STAT_P1_L0_DIR        (1<<14)
#define D_MIPI_DPHY_STAT_P1_L1_DIR        (1<<15)

// ============================================================= MIPI_DPHY_ERR_STAT_DPHY01
// MIPI_DPHY_ERR_STAT_DPHY23
// MIPI_DPHY_ERR_STAT_DPHY45
#define D_MIPI_DPHY_ERR_STAT_P0_CONTLP_L0_1  (1<<0)
#define D_MIPI_DPHY_ERR_STAT_P0_CONTLP_L1_1  (1<<1)
#define D_MIPI_DPHY_ERR_STAT_P0_CONTLP_L0_0  (1<<2)
#define D_MIPI_DPHY_ERR_STAT_P0_CONTLP_L1_0  (1<<3)
#define D_MIPI_DPHY_ERR_STAT_P0_CTRL_L0      (1<<4)
#define D_MIPI_DPHY_ERR_STAT_P0_CTRL_L1      (1<<5)
#define D_MIPI_DPHY_ERR_STAT_P0_SYNCESC_L0   (1<<6)
#define D_MIPI_DPHY_ERR_STAT_P0_SYNCESC_L1   (1<<7)
#define D_MIPI_DPHY_ERR_STAT_P0_ESC_L0       (1<<8)
#define D_MIPI_DPHY_ERR_STAT_P0_ESC_L1       (1<<9)
#define D_MIPI_DPHY_ERR_STAT_P0_SOTSYNCHS_L0 (1<<10)
#define D_MIPI_DPHY_ERR_STAT_P0_SOTSYNCHS_L1 (1<<11)
#define D_MIPI_DPHY_ERR_STAT_P0_SOTHS_L0     (1<<12)
#define D_MIPI_DPHY_ERR_STAT_P0_SOTHS_L1     (1<<13)
//bits  14
#define D_MIPI_DPHY_ERR_STAT_P1_CONTLP_L0_1  (1<<14)
#define D_MIPI_DPHY_ERR_STAT_P1_CONTLP_L1_1  (1<<15)
#define D_MIPI_DPHY_ERR_STAT_P1_CONTLP_L0_0  (1<<16)
#define D_MIPI_DPHY_ERR_STAT_P1_CONTLP_L1_0  (1<<17)
#define D_MIPI_DPHY_ERR_STAT_P1_CTRL_L0      (1<<18)
#define D_MIPI_DPHY_ERR_STAT_P1_CTRL_L1      (1<<19)
#define D_MIPI_DPHY_ERR_STAT_P1_SYNCESC_L0   (1<<20)
#define D_MIPI_DPHY_ERR_STAT_P1_SYNCESC_L1   (1<<21)
#define D_MIPI_DPHY_ERR_STAT_P1_ESC_L0       (1<<22)
#define D_MIPI_DPHY_ERR_STAT_P1_ESC_L1       (1<<23)
#define D_MIPI_DPHY_ERR_STAT_P1_SOTSYNCHS_L0 (1<<24)
#define D_MIPI_DPHY_ERR_STAT_P1_SOTSYNCHS_L1 (1<<25)
#define D_MIPI_DPHY_ERR_STAT_P1_SOTHS_L0     (1<<26)
#define D_MIPI_DPHY_ERR_STAT_P1_SOTHS_L1     (1<<27)

// ============================================================= MIPI_DPHY_TEST_CLR
// MIPI_DPHY_TEST_EN
// MIPI_DPHY_TEST_CLK
#define D_MIPI_DPHY_TEST_P0 (1<<0)
#define D_MIPI_DPHY_TEST_P1 (1<<1)
#define D_MIPI_DPHY_TEST_P2 (1<<2)
#define D_MIPI_DPHY_TEST_P3 (1<<3)
#define D_MIPI_DPHY_TEST_P4 (1<<4)
#define D_MIPI_DPHY_TEST_P5 (1<<5)

// =============================================================  MIPI_DPHY_TEST_DOUT_DPHY01
// MIPI_DPHY_TEST_DOUT_DPHY23
// MIPI_DPHY_TEST_DOUT_DPHY45
#define MIPI_DPHY_TEST_DOUT_P0      (1   <<0)
#define MIPI_DPHY_TEST_DOUT_P0_MASK (0xFF<<0)
#define MIPI_DPHY_TEST_DOUT_P1      (1   <<8)
#define MIPI_DPHY_TEST_DOUT_P1_MASK (0xFF<<8)
// =============================================================  MIPI_DPHY_PLL_LOCK
#define MIPI_DPHY_PLL_LOCK_0   (1<<5)  // DPHY 5
#define MIPI_DPHY_PLL_LOCK_1   (1<<2)  // DPHY 2


/* --------------------------------------------------------------------------------------------------------------------------------------
                                                             HS  CONTROL REGISTERS
------------------------------------------------------------------------------------------------------------------------------------------ */



// =============================================================  MIPI_HS_CTRL
#define  D_MIPI_HS_CTRL_EN         (1<<0)

#define  D_MIPI_HS_CTRL_MODE_TX    (1<<1)
#define  D_MIPI_HS_CTRL_MODE_RX    (0<<1)
#define  D_MIPI_HS_CTRL_DIR_POS    (1)

#define  D_MIPI_HS_CTRL_MODE_CSI   (1<<2)
#define  D_MIPI_HS_CTRL_MODE_DSI   (0<<2)
#define  D_MIPI_HS_CTRL_MODE_POS   (2)
// bit 3 not used
#define  D_MIPI_HS_CTRL_1_LANE     (0<<4)
#define  D_MIPI_HS_CTRL_2_LANE     (1<<4)
#define  D_MIPI_HS_CTRL_4_LANE     (3<<4)
#define  D_MIPI_HS_CTRL_6_LANE     (5<<4)
#define  D_MIPI_HS_CTRL_8_LANE     (7<<4)
#define  D_MIPI_HS_CTRL_LANE_MASK  (7<<4)
#define  D_MIPI_HS_CTRL_LANE_POS   (4)
// bit 7 not used
#define D_MIPI_HS_CTRL_DATA_MODE_MASK    (3<<8)
#define D_MIPI_HS_CTRL_DATA_MODE_0       (0<<8)
#define D_MIPI_HS_CTRL_DATA_MODE_1       (1<<8)
#define D_MIPI_HS_CTRL_DATA_MODE_2       (2<<8)
#define D_MIPI_HS_CTRL_DATA_MODE_3       (3<<8)
#define D_MIPI_HS_CTRL_DATA_MODE_POS     (8)
//bits 10
#define D_MIPI_HS_CTRL_RX_IGNORE_DATA_TYPE (1<<10)
//bits 11
#define D_MIPI_HS_CTRL_TX_DSI_EOTP_EN      (1<<11)
//bits 12
#define D_MIPI_HS_CTRL_TX_DSI_CMD_HFP_EN   (1<<12)
//bits 16 ->
#define  D_MIPI_HS_CTRL_HS_EXIT_CNT_POS     (16)
#define  D_MIPI_HS_CTRL_HS_CLK_IDLE_CNT_POS (24)
#define  D_MIPI_HS_CTRL_HS_EXIT_CNT_MASK     (0xFF << D_MIPI_HS_CTRL_HS_EXIT_CNT_POS)
#define  D_MIPI_HS_CTRL_HS_CLK_IDLE_CNT_MASK (0xFF << D_MIPI_HS_CTRL_HS_CLK_IDLE_CNT_POS)
//===============================================================  MIPI_HS_SYNC_CFG_ADDR
#define D_MIPI_HS_SYNC_CFG_LINE_PKT_EN            (1<<0)
#define D_MIPI_HS_SYNC_CFG_FR_CNT_EN              (1<<1)
#define D_MIPI_HS_SYNC_CFG_LINE_CNT               (1<<2)

#define D_MIPI_HS_SYNC_CFG_DSI_VBL_HS             (1<<4)
#define D_MIPI_HS_SYNC_CFG_DSI_VBL_LP             (0<<4)

#define D_MIPI_HS_SYNC_CFG_DSI_HSA_BL_HS          (1<<5)
#define D_MIPI_HS_SYNC_CFG_DSI_HSA_BL_LP          (0<<5)

#define D_MIPI_HS_SYNC_CFG_DSI_HBP_BL_HS          (1<<6)
#define D_MIPI_HS_SYNC_CFG_DSI_HBP_BL_LP          (0<<6)

#define D_MIPI_HS_SYNC_CFG_DSI_HFP_BL_HS          (1<<7)
#define D_MIPI_HS_SYNC_CFG_DSI_HFP_BL_LP          (0<<7)

#define D_MIPI_HS_SYNC_CFG_DSI_SYNC_EVENT         (0<<8)
#define D_MIPI_HS_SYNC_CFG_DSI_SYNC_PULSE         (1<<8)

#define D_MIPI_HS_SYNC_CFG_DSI_LP_FIRST_VSA_LINE  (1<<9)

#define D_MIPI_HS_SYNC_CFG_DSI_LP_LAST_VFP_LINE  (1<<10)

/* --------------------------------------------------------------------------------------------------------------------------------------
                                                              MAIN STATUS REGISTERS
------------------------------------------------------------------------------------------------------------------------------------------ */
#define D_MIPI_IRQ_STAT_CFG_ERR       (1<<0)

#define D_MIPI_IRQ_STAT_DPHY_ERR_0    (1<<1)
#define D_MIPI_IRQ_STAT_DPHY_ERR_1    (1<<2)
#define D_MIPI_IRQ_STAT_DPHY_ERR_2    (1<<3)
#define D_MIPI_IRQ_STAT_DPHY_ERR_3    (1<<4)
#define D_MIPI_IRQ_STAT_DPHY_ERR_4    (1<<5)
#define D_MIPI_IRQ_STAT_DPHY_ERR_5    (1<<6)
#define D_MIPI_IRQ_STAT_DPHY_ERR (D_MIPI_IRQ_STAT_DPHY_ERR_0 | D_MIPI_IRQ_STAT_DPHY_ERR_1 | D_MIPI_IRQ_STAT_DPHY_ERR_2 |\
                                  D_MIPI_IRQ_STAT_DPHY_ERR_3 | D_MIPI_IRQ_STAT_DPHY_ERR_4 | D_MIPI_IRQ_STAT_DPHY_ERR_5)

#define D_MIPI_IRQ_STAT_HS_ERR_0      (1<< 7)
#define D_MIPI_IRQ_STAT_HS_ERR_1      (1<< 8)
#define D_MIPI_IRQ_STAT_HS_ERR_2      (1<< 9)
#define D_MIPI_IRQ_STAT_HS_ERR_3      (1<<10)
#define D_MIPI_IRQ_STAT_HS_ERR_4      (1<<11)
#define D_MIPI_IRQ_STAT_HS_ERR_5      (1<<12)
#define D_MIPI_IRQ_STAT_HS_ERR (D_MIPI_IRQ_STAT_HS_ERR_0 | D_MIPI_IRQ_STAT_HS_ERR_1 | D_MIPI_IRQ_STAT_HS_ERR_2 | \
                                D_MIPI_IRQ_STAT_HS_ERR_3 | D_MIPI_IRQ_STAT_HS_ERR_4 | D_MIPI_IRQ_STAT_HS_ERR_5)

#define D_MIPI_IRQ_STAT_LP_EVENT      (1<<13)

#define D_MIPI_IRQ_STAT_HS_RX_EVENT_0 (1<<14)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT_1 (1<<15)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT_2 (1<<16)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT_3 (1<<17)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT_4 (1<<18)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT_5 (1<<19)
#define D_MIPI_IRQ_STAT_HS_RX_EVENT   (D_MIPI_IRQ_STAT_HS_RX_EVENT_0 | D_MIPI_IRQ_STAT_HS_RX_EVENT_1 | D_MIPI_IRQ_STAT_HS_RX_EVENT_2 |\
                                       D_MIPI_IRQ_STAT_HS_RX_EVENT_3 | D_MIPI_IRQ_STAT_HS_RX_EVENT_4 | D_MIPI_IRQ_STAT_HS_RX_EVENT_5)


// MIPI_RX_LP_EVENT
#define D_MIPI_RX_LP_EVENT_L0  (1<<0)
#define D_MIPI_RX_LP_EVENT_L1  (1<<1)
#define D_MIPI_RX_LP_EVENT_L2  (1<<2)
#define D_MIPI_RX_LP_EVENT_L3  (1<<3)
#define D_MIPI_RX_LP_EVENT_L4  (1<<4)
#define D_MIPI_RX_LP_EVENT_L5  (1<<5)
#define D_MIPI_RX_LP_EVENT_L6  (1<<6)
#define D_MIPI_RX_LP_EVENT_L7  (1<<7)
#define D_MIPI_RX_LP_EVENT_L8  (1<<8)
#define D_MIPI_RX_LP_EVENT_L9  (1<<9)
#define D_MIPI_RX_LP_EVENT_L10 (1<<10)
#define D_MIPI_RX_LP_EVENT_L11 (1<<11)

#define D_MIPI_RX_LP_EVENT_L0_TRIG  (1<<12)
#define D_MIPI_RX_LP_EVENT_L1_TRIG  (1<<13)
#define D_MIPI_RX_LP_EVENT_L2_TRIG  (1<<14)
#define D_MIPI_RX_LP_EVENT_L3_TRIG  (1<<15)
#define D_MIPI_RX_LP_EVENT_L4_TRIG  (1<<16)
#define D_MIPI_RX_LP_EVENT_L5_TRIG  (1<<17)
#define D_MIPI_RX_LP_EVENT_L6_TRIG  (1<<18)
#define D_MIPI_RX_LP_EVENT_L7_TRIG  (1<<19)
#define D_MIPI_RX_LP_EVENT_L8_TRIG  (1<<20)
#define D_MIPI_RX_LP_EVENT_L9_TRIG  (1<<21)
#define D_MIPI_RX_LP_EVENT_L10_TRIG (1<<22)
#define D_MIPI_RX_LP_EVENT_L11_TRIG (1<<23)

// MIPI_RX_ULPS_EVENT
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY0 (1<<0)
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY1 (1<<1)
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY2 (1<<2)
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY3 (1<<3)
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY4 (1<<4)
#define D_MIPI_RX_ULPS_EVENT_CLK_DPHY5 (1<<5)

#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY0_L0  (1<<6)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY0_L1  (1<<7)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY1_L0  (1<<8)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY1_L1  (1<<9)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY2_L0  (1<<10)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY2_L1  (1<<11)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY3_L0  (1<<12)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY3_L1  (1<<13)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY4_L0  (1<<14)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY4_L1  (1<<15)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY5_L0  (1<<16)
#define D_MIPI_RX_ULPS_ENTER_EVENT_DPHY5_L1  (1<<17)

// MIPI_RX_ULPS_EXIT
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY0 (1<<0)
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY1 (1<<1)
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY2 (1<<2)
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY3 (1<<3)
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY4 (1<<4)
#define D_MIPI_RX_ULPS_EXIT_EVENT_CLK_DPHY5 (1<<5)

#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY0_L0  (1<<6)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY0_L1  (1<<7)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY1_L0  (1<<8)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY1_L1  (1<<9)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY2_L0  (1<<10)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY2_L1  (1<<11)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY3_L0  (1<<12)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY3_L1  (1<<13)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY4_L0  (1<<14)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY4_L1  (1<<15)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY5_L0  (1<<16)
#define D_MIPI_RX_ULPS_EXIT_EVENT_DPHY5_L1  (1<<17)

// MIPI_TX_HS_SECTx_PH
#define D_MIPI_TX_HS_PH_NO                     (4)                                     // number of different packet headers that can be sent.
#define D_MIPI_TX_HS_PH_WORD_COUNT_POS         (0)
#define D_MIPI_TX_HS_PH_WORD_COUNT_MASK        (0xFFFF << D_MIPI_TX_HS_PH_WORD_COUNT_POS)
#define D_MIPI_TX_HS_PH_DATA_IDENTIFIER_POS    (16)
#define D_MIPI_TX_HS_PH_DATA_IDENTIFIER_MASK   (0xFF << D_MIPI_TX_HS_PH_DATA_IDENTIFIER_POS)
#define D_MIPI_TX_HS_PH_DATA_MODE_POS          (24)
#define D_MIPI_TX_HS_PH_DATA_MODE_MASK         (0x3 << D_MIPI_TX_HS_PH_DATA_MODE_POS)




/* --------------------------------------------------------------------------------------------------------------------------------------
                                                           PACKET HEADER TYPES
------------------------------------------------------------------------------------------------------------------------------------------ */


//=============================================================== DSI DATA FORMATS
#define D_MIPI_DSI_SP_DT_RESERVED_00               (0x00 << 16)
#define D_MIPI_DSI_SP_DT_VSYNC_START               (0x01 << 16)
#define D_MIPI_DSI_SP_DT_COLOR_MODE_OFF            (0x02 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_SHORT_WR          (0x03 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_RD                (0x04 << 16)
#define D_MIPI_DSI_SP_DT_DCS_SHORT_WR              (0x05 << 16)
#define D_MIPI_DSI_SP_DT_DCS_RD                    (0x06 << 16)
#define D_MIPI_DSI_SP_DT_EOTP                      (0x08 << 16)
#define D_MIPI_DSI_LP_DT_NULL                      (0x09 << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_0A               (0x0a << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_0B               (0x0b << 16)
#define D_MIPI_DSI_LP_DT_LPPS_YCBCR422_20B         (0x0c << 16)
#define D_MIPI_DSI_LP_DT_PPS_RGB101010_30B         (0x0d << 16)
#define D_MIPI_DSI_LP_DT_PPS_RGB565_16B            (0x0e << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_0F               (0x0f << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_10               (0x10 << 16)
#define D_MIPI_DSI_SP_DT_VSYNC_END                 (0x11 << 16)
#define D_MIPI_DSI_SP_DT_COLOR_MODE_ON             (0x12 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_SHORT_WR_1PAR     (0x13 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_RD_1PAR           (0x14 << 16)
#define D_MIPI_DSI_SP_DT_DCS_SHORT_WR_1PAR         (0x15 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_16               (0x16 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_17               (0x17 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_18               (0x18 << 16)
#define D_MIPI_DSI_LP_DT_BLANK                     (0x19 << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_1A               (0x1a << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_1B               (0x1b << 16)
#define D_MIPI_DSI_LP_DT_PPS_YCBCR422_24B          (0x1c << 16)
#define D_MIPI_DSI_LP_DT_PPS_RGB121212_36B         (0x1d << 16)
#define D_MIPI_DSI_LP_DT_PPS_RGB666_18B            (0x1e << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_1F               (0x1f << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_20               (0x20 << 16)
#define D_MIPI_DSI_SP_DT_HSYNC_START               (0x21 << 16)
#define D_MIPI_DSI_SP_DT_SHUT_DOWN_PERIPH_CMD      (0x22 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_SHORT_WR_2PAR     (0x23 << 16)
#define D_MIPI_DSI_SP_DT_GENERIC_RD_2PAR           (0x24 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_25               (0x25 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_26               (0x26 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_27               (0x27 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_28               (0x28 << 16)
#define D_MIPI_DSI_LP_DT_GENERIC_LONG_WR           (0x29 << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_2A               (0x2a << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_2B               (0x2b << 16)
#define D_MIPI_DSI_LP_DT_PPS_YCBCR422_16B          (0x2c << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_2D               (0x2d << 16)
#define D_MIPI_DSI_LP_DT_LPPS_RGB666_18B           (0x2e << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_2F               (0x2f << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_30               (0x30 << 16)
#define D_MIPI_DSI_SP_DT_HSYNC_END                 (0x31 << 16)
#define D_MIPI_DSI_SP_DT_TURN_ON_PERIPH_CMD        (0x32 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_33               (0x33 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_34               (0x34 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_35               (0x35 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_36               (0x36 << 16)
#define D_MIPI_DSI_SP_DT_SET_MAX_RETURN_PKT_SIZE   (0x37 << 16)
#define D_MIPI_DSI_SP_DT_RESERVED_38               (0x38 << 16)
#define D_MIPI_DSI_LP_DT_DSC_LONG_WR               (0x39 << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_3A               (0x3a << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_3B               (0x3b << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_3C               (0x3c << 16)
#define D_MIPI_DSI_LP_DT_PPS_YCBCR420_12B          (0x3d << 16)
#define D_MIPI_DSI_LP_DT_PPS_RGB888_24B            (0x3e << 16)
#define D_MIPI_DSI_LP_DT_RESERVED_3F               (0x3f << 16)



//=============================================================== CSI DATA FORMATS
#define D_MIPI_CSI_DFORM_RGB888        (0x24 << 16)
#define D_MIPI_CSI_DFORM_RGB666        (0x23 << 16)
#define D_MIPI_CSI_DFORM_RGB565        (0x22 << 16)
#define D_MIPI_CSI_DFORM_RGB555        (0x21 << 16)
#define D_MIPI_CSI_DFORM_RGB444        (0x20 << 16)
#define D_MIPI_CSI_DFORM_Y420_8        (0x18 << 16)
#define D_MIPI_CSI_DFORM_Y420_8_CSPS   (0x1C << 16)
#define D_MIPI_CSI_DFORM_Y420_8_L      (0x1A << 16)
#define D_MIPI_CSI_DFORM_Y422_8        (0x1E << 16)
#define D_MIPI_CSI_DFORM_Y422_10       (0x1F << 16)
#define D_MIPI_CSI_DFORM_Y420_10       (0x19 << 16)
#define D_MIPI_CSI_DFORM_Y420_10_CSPS  (0x1D << 16)
#define D_MIPI_CSI_DFORM_RAW6          (0x28 << 16)
#define D_MIPI_CSI_DFORM_RAW7          (0x29 << 16)
#define D_MIPI_CSI_DFORM_RAW8          (0x2A << 16)
#define D_MIPI_CSI_DFORM_RAW10         (0x2B << 16)
#define D_MIPI_CSI_DFORM_RAW12         (0x2C << 16)
#define D_MIPI_CSI_DFORM_RAW14         (0x2D << 16)
#define D_MIPI_CSI_DFORM_NULL          (0x10 << 16)
#define D_MIPI_CSI_DFORM_BLANK         (0x11 << 16)
#define D_MIPI_CSI_DFORM_EMBEDDED      (0x12 << 16)
#define D_MIPI_CSI_DFORM_USER0         (0x30 << 16)
#define D_MIPI_CSI_DFORM_USER1         (0x31 << 16)
#define D_MIPI_CSI_DFORM_USER2         (0x32 << 16)
#define D_MIPI_CSI_DFORM_USER3         (0x33 << 16)
#define D_MIPI_CSI_DFORM_USER4         (0x34 << 16)
#define D_MIPI_CSI_DFORM_USER5         (0x35 << 16)
#define D_MIPI_CSI_DFORM_USER6         (0x36 << 16)
#define D_MIPI_CSI_DFORM_USER7         (0x37 << 16)

#define D_MIPI_DFORM_MASK (0x3F)        // data type field of CSI/DSI


#define DRV_MIPI_NO_TX_PACKETS (4)      // number of different TX packets that can be transmitted on a TX controller

enum {
    eMipiCfgErrCantBeTx,
    eMipiCfgErrWrongNoLanes,

};

typedef void (* tDrvMipiCallBack)( u32, u32 );  // takes two parameters, depending on the type of event they could mean different things

typedef struct drvMipiCallBackStruct
{
    /*  function gets two parameters:
           first param given is the lane number:
                           0 - 11    lane x has LPDT data, see second param for the LPDT value
                           12 - 23 lane x - 12 has a  Trig value, see second param for the trig value
           second param given is the LPDT data or the trig value, or 0 if ULPS event
           */
    tDrvMipiCallBack LpdtEvent;

    /*
           first param given is the lane number:
                           0 - 5 CLK lane of D-PHY (x) entered ULPS state
                           6 - 17 lane x-6 entered ULPS state
           second param  is 0
           */
    tDrvMipiCallBack UlpsEnter;

    /*
           first param given is the lane number:
                           0 - 5 CLK lane of D-PHY (x) exited ULPS state
                           6 - 17 lane x-6 entered ULPS state
           second param  is 0
           */
    tDrvMipiCallBack UlpsExit;

    /*
           first param given is the ctrl number:
           second param  contains the packet header received by this controller
           */
    tDrvMipiCallBack RxHsEvent;

    /*
           first param given is the ctrl number:
           second param  is the LLP error register content
           */
    tDrvMipiCallBack HsTxError;
    tDrvMipiCallBack HsRxError;

    /*
           first param given is the dphy number:
           second param  contains the error bits for that dphy
           */
    tDrvMipiCallBack DphyError;

    /*
           first param given is the controller number
           second param  is a code meaning:
                   eMipiCfgErrCantBeTx - ctrl was set as tx but it can only be RX
                   eMipiCfgErrWrongNoLanes - ctrl is configured with a wrong number of lanes
           */
    tDrvMipiCallBack HsCfgError;

} * tPtrDrvMipiCallBackStruct, tDrvMipiCallBackStruct;

typedef enum drvMipiCtrlNo
{
    MIPI_CTRL_0 = 0 , MIPI_CTRL_1, MIPI_CTRL_2, MIPI_CTRL_3, MIPI_CTRL_4, MIPI_CTRL_5
} eDrvMipiCtrlNo;
typedef enum drvMipiType { MIPI_DSI = 0, MIPI_CSI = 1} eDrvMipiType;
typedef enum drvMipiDir  { MIPI_RX = 0, MIPI_TX = 1 } eDrvMipiDir;
//CSI MIPI modes. Look for these in table "CSI-2 RAW data formats" from the Myriad2 datasheet
//And all of the similar CSI-2 * data formats tables in the surrounding paragpahs
//column "Data Mode"
typedef enum drvMipiDataMode {MIPI_D_MODE_0 = 0, MIPI_D_MODE_1 = 1, MIPI_D_MODE_2 = 2, MIPI_D_MODE_3 = 3} eDrvMipiDataMode;

typedef enum drvMipiDataType
{
   // csi long packet data types
    CSI_YUV_420_B8       = 0x18,
    CSI_YUV_420_B10      = 0x19,
    CSI_YUV_420_B8_L     = 0x1A,
    CSI_YUV_420_B8_CSPS  = 0x1C,
    CSI_YUV_420_B10_CSPS = 0x1D,
    CSI_YUV_422_B8       = 0x1E,
    CSI_YUV_422_B10      = 0x1F,
    CSI_RGB_444          = 0x20,
    CSI_RGB_555          = 0x21,
    CSI_RGB_565          = 0x22,
    CSI_RGB_666          = 0x23,
    CSI_RGB_888          = 0x24,
    CSI_RAW_6            = 0x28,
    CSI_RAW_7            = 0x29,
    CSI_RAW_8            = 0x2A,
    CSI_RAW_10           = 0x2B,
    CSI_RAW_12           = 0x2C,
    CSI_RAW_14           = 0x2D,

    //dsi long packet data types, bit 8 set to 1 not to interfere with CSI Data type, bit 8 will have to be removed in the driver for DSI
    DSI_LPPS_YUV_422_B20 = 0x10C, // LPPS - Loosely PPS
    DSI_PPS_RGB_101010   = 0x10D,
    DSI_PPS_RGB_565      = 0x10E,
    DSI_PPS_YUV_422_B24  = 0x11C,
    DSI_PPS_RGB_121212   = 0x11D,
    DSI_PPS_RGB_666      = 0x11E,
    DSI_PPS_YUV_422_B16  = 0x12C,
    DSI_LPPS_RGB_666     = 0x12E,
    DSI_PPS_YUV_420_B12  = 0x13D,  // PPS - Packet pixel stream
    DSI_PPS_RGB_888      = 0x13E,
} eDrvMipiDataType;


typedef enum drvMipiInitFuncRet
{
    DRV_MIPI_CFG_DONE=0,                     // configuration was done with no errors
    DRV_MIPI_CFG_INVALID_CTRL,               // controller X does not exist; if X not in {0..5}
    DRV_MIPI_CFG_INVALID_DIR,                // Parameter is neither RX, nor TX
    DRV_MIPI_CFG_INVALID_TYPE,               // Parameter is neither RX, nor TX
    DRV_MIPI_CFG_NO_TX,                      // Controller has no Tx capabilities
    DRV_MIPI_CFG_INVALID_LANE_NO,            // To many lanes selected, or an odd number of lanes selected (exception 1 lane is possible )
    DRV_MIPI_CFG_INVALID_RATE,               // Rate has to be in the range [80, 1500] Mbps
    DRV_MIPI_CFG_INVALID_RX_DATA_TYPE,       // RX Data type is not a 6 bit field.
    DRV_MIPI_CFG_INVALID_TX_DATA_TYPE,       // TX Data type is not a 6 bit field.
    DRV_MIPI_CFG_INVALID_RX_DATA_MODE,       // RX Data mode is invalid, not a 2 bit filed
    DRV_MIPI_CFG_INVALID_TX_DATA_MODE,       // TX Data mode is invalid, not a 2 bit filed
    DRV_MIPI_CFG_PLL_NOT_LOCKED              // PLL is not locked
} eDrvMipiInitFuncRet;

typedef struct drvMipiTxPacketHeader
{
    eDrvMipiDataMode    dataMode;          // This works with data type, to set the bus between the MIPI ctrl and CIF, SIPP or LCD
    eDrvMipiDataType    dataType;          // Csi and dsi long packets data types
    u32                 noPackets;         // Number of packets to be transmitted (not needed for rx) with this data types
    u32                 bitsPerPixel;     // Number of bytes per pixel 
} tDrvMipiTxPacketHeaderStruct, * tPtrDrvMipiTxPacketHeaderStruct;

typedef struct drvMipiRxPacketHeader
{
    eDrvMipiDataMode    dataMode;          // This works with data type, to set the bus between the MIPI ctrl and CIF, SIPP or LCD
    eDrvMipiDataType    dataType;          // Csi and dsi long packets data types
} tDrvMipiRxPacketHeaderStruct, * tPtrDrvMipiRxPacketHeaderStruct;


typedef struct drvMipiControllerConfigStruct
{
    eDrvMipiCtrlNo      controllerNo;      // Controller number 0 to 5
    eDrvMipiType        type;              // Is controller set as DSI or CSI
    eDrvMipiDir         direction;         // Is controller set as Rx or Tx
    u32                 noLanes;           // Number of lanes to be used with controller controllerNo
    u32                 laneRateMbps;      // Link rate in Mbps, on one data lane
    union                                  // packet header stuff
    {
        tDrvMipiTxPacketHeaderStruct txPacket[DRV_MIPI_NO_TX_PACKETS];
        tDrvMipiRxPacketHeaderStruct rxPacket;
    };
    u32                 width;             // Width in pixels
    u32                 height;             // Height in rows

    u32                 refClkKHz;         // Ref clock, needed for tx PLL
    // TO DO
    //  pointer to callback
} *tPtrDrvMipiControllerConfigStruct, tDrvMipiControllerConfigStruct;

typedef enum drvMipiTermResMode 
{
    DRV_MIPI_TERM_RES_DISABLE = 0,
    DRV_MIPI_TERM_RES_ENABLE 

} eDrvMipiTermResMode;



#endif

///@}
