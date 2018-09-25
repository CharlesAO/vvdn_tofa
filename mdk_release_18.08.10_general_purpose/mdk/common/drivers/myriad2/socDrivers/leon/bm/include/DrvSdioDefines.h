///
/// @file DrvSdioDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSdio
/// @{
///


/// 
/// 
/// 
///
/// @brief     SDIO Driver defines 
///


#ifndef _DRVSDIODEFINES_H_
#define _DRVSDIODEFINES_H_
#include <DrvCommon.h>
#include "mv_types.h"
// DRVSDIO Debug mode 
//#define DRVSDIO_DEBUG
// Number of cards supported 
#define DRVSDIO_MAXDRIVERCARDS                               2
// Max/Min DMA transaction sizes
#define DRVSDIO_MAXDMATRANSACTIONSIZE                        4096
#define DRVSDIO_MINDMATRANSACTIONSIZE                        64//512
#define DRVSDIO_MAXDEVIATIONKHZ                              1000
#define DRVSDIO_MAXINPUTFREQUENCYKHZ                         260000
//----------------------------------------------------------------------------//
// REGISTER UTILITIES 
//----------------------------------------------------------------------------//

// INTMASK 
#define DRVSDIO_INTMASK_SDIO15                      31
#define DRVSDIO_INTMASK_SDIO14                      30
#define DRVSDIO_INTMASK_SDIO13                      29
#define DRVSDIO_INTMASK_SDIO12                      28
#define DRVSDIO_INTMASK_SDIO11                      27
#define DRVSDIO_INTMASK_SDIO10                      26
#define DRVSDIO_INTMASK_SDIO9                       25
#define DRVSDIO_INTMASK_SDIO8                       24
#define DRVSDIO_INTMASK_SDIO7                       23
#define DRVSDIO_INTMASK_SDIO6                       22
#define DRVSDIO_INTMASK_SDIO5                       21
#define DRVSDIO_INTMASK_SDIO4                       20
#define DRVSDIO_INTMASK_SDIO3                       19
#define DRVSDIO_INTMASK_SDIO2                       18
#define DRVSDIO_INTMASK_SDIO1                       17
#define DRVSDIO_INTMASK_SDIO0                       16
#define DRVSDIO_INTMASK_EBE                         15
#define DRVSDIO_INTMASK_ACD                         14
#define DRVSDIO_INTMASK_BCI                         13
#define DRVSDIO_INTMASK_HLE                         12
#define DRVSDIO_INTMASK_FRUN                        11
#define DRVSDIO_INTMASK_HTO                         10
#define DRVSDIO_INTMASK_DRTO                        9
#define DRVSDIO_INTMASK_RTO                         8
#define DRVSDIO_INTMASK_DCRC                        7
#define DRVSDIO_INTMASK_RCRC                        6
#define DRVSDIO_INTMASK_RXDR                        5
#define DRVSDIO_INTMASK_TXDR                        4
#define DRVSDIO_INTMASK_DTO                         3
#define DRVSDIO_INTMASK_CDONE                       2
#define DRVSDIO_INTMASK_RE                          1
#define DRVSDIO_INTMASK_CD                          0
#define DRVSDIO_INTMASK_SDIOALL                     (0xFFFF0000)
#define DRVSDIO_INTMASK_EVENTALL                    (0x0000FFFF)
#define DRVSDIO_INTMASK_ALL                         (0xFFFFFFFF)

// RAW INT STATUS 
#define DRVSDIO_RAWINTSTATUS_SDIO15                     31
#define DRVSDIO_RAWINTSTATUS_SDIO14                     30
#define DRVSDIO_RAWINTSTATUS_SDIO13                     29
#define DRVSDIO_RAWINTSTATUS_SDIO12                     28
#define DRVSDIO_RAWINTSTATUS_SDIO11                     27
#define DRVSDIO_RAWINTSTATUS_SDIO10                     26
#define DRVSDIO_RAWINTSTATUS_SDIO9                      25
#define DRVSDIO_RAWINTSTATUS_SDIO8                      24
#define DRVSDIO_RAWINTSTATUS_SDIO7                      23
#define DRVSDIO_RAWINTSTATUS_SDIO6                      22
#define DRVSDIO_RAWINTSTATUS_SDIO5                      21
#define DRVSDIO_RAWINTSTATUS_SDIO4                      20
#define DRVSDIO_RAWINTSTATUS_SDIO3                      19
#define DRVSDIO_RAWINTSTATUS_SDIO2                      18
#define DRVSDIO_RAWINTSTATUS_SDIO1                      17
#define DRVSDIO_RAWINTSTATUS_SDIO0                      16
#define DRVSDIO_RAWINTSTATUS_EBE                        15
#define DRVSDIO_RAWINTSTATUS_ACD                        14
#define DRVSDIO_RAWINTSTATUS_BCI                        13
#define DRVSDIO_RAWINTSTATUS_HLE                        12
#define DRVSDIO_RAWINTSTATUS_FRUN                       11
#define DRVSDIO_RAWINTSTATUS_HTO                        10
#define DRVSDIO_RAWINTSTATUS_VOLTSW                     10  // Shared with HTO
#define DRVSDIO_RAWINTSTATUS_DRTO                       9
#define DRVSDIO_RAWINTSTATUS_RTO                        8
#define DRVSDIO_RAWINTSTATUS_DCRC                       7
#define DRVSDIO_RAWINTSTATUS_RCRC                       6
#define DRVSDIO_RAWINTSTATUS_RXDR                       5
#define DRVSDIO_RAWINTSTATUS_TXDR                       4
#define DRVSDIO_RAWINTSTATUS_DTO                        3
#define DRVSDIO_RAWINTSTATUS_CDONE                      2
#define DRVSDIO_RAWINTSTATUS_RE                         1
#define DRVSDIO_RAWINTSTATUS_CD                         0
#define DRVSDIO_RAWINTSTATUS_SDIOALL                    (0xFFFF0000)
#define DRVSDIO_RAWINTSTATUS_EVENTALL                   (0x0000FFFF)
#define DRVSDIO_RAWINTSTATUS_ALL                        (0xFFFFFFFF)

// Flags to check waiting for the completion of a command apart from command done
#define DRVSDIO_COMMANDDONE_ERROR_FLAGS                 ((1 << DRVSDIO_RAWINTSTATUS_RE) |  (1 << DRVSDIO_RAWINTSTATUS_RCRC) | (1 << DRVSDIO_RAWINTSTATUS_RTO))
#define DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS             ((1 << DRVSDIO_RAWINTSTATUS_DCRC) | (1 << DRVSDIO_RAWINTSTATUS_DRTO) | (1 << DRVSDIO_RAWINTSTATUS_BCI) | (1 << DRVSDIO_RAWINTSTATUS_EBE) | \
                                                        (1 << DRVSDIO_RAWINTSTATUS_HTO) | (1 << DRVSDIO_RAWINTSTATUS_FRUN) | (1 << DRVSDIO_R1_ERROR_FLAG))
#define DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS             ((1 << DRVSDIO_RAWINTSTATUS_DCRC) | (1 << DRVSDIO_RAWINTSTATUS_DRTO) | (1 << DRVSDIO_RAWINTSTATUS_EBE) | \
                                                        (1 << DRVSDIO_RAWINTSTATUS_HTO) | (1 << DRVSDIO_RAWINTSTATUS_FRUN) | (1 << DRVSDIO_R1_ERROR_FLAG))



// CONTROL REGISTER
#define DRVSDIO_CTRL_USE_IDMAC                          25
#define DRVSDIO_CTRL_ENABLE_OD_PULLUP                   24
#define DRVSDIO_CTRL_CARD_VOLTAGE_B                     20
#define DRVSDIO_CTRL_CARD_VOLTAGE_A                     16
#define DRVSDIO_CTRL_CEATA_INT_STATUS                   11
#define DRVSDIO_CTRL_SEND_AUTO_STOP_CCSD                10
#define DRVSDIO_CTRL_SEND_CCSD                          9
#define DRVSDIO_CTRL_ABORT_READ_DATA                    8
#define DRVSDIO_CTRL_SEND_IRQ_RESPONSE                  7
#define DRVSDIO_CTRL_READ_WAIT                          6
#define DRVSDIO_CTRL_DMA_ENABLE                         5
#define DRVSDIO_CTRL_INT_ENABLE                         4
#define DRVSDIO_CTRL_DMA_RESET                          2
#define DRVSDIO_CTRL_FIFO_RESET                         1
#define DRVSDIO_CTRL_CONTROLLER_RESET                   0

// CLOCK DIVIDER 
#define DRVSDIO_CLKDIV_DIVIDER3                         24
#define DRVSDIO_CLKDIV_DIVIDER2                         16
#define DRVSDIO_CLKDIV_DIVIDER1                         8
#define DRVSDIO_CLKDIV_DIVIDER0                         0

// CLOCK SOURCE 
#define DRVSDIO_CLKSOURCE_SDIO15                        30
#define DRVSDIO_CLKSOURCE_SDIO14                        28
#define DRVSDIO_CLKSOURCE_SDIO13                        26
#define DRVSDIO_CLKSOURCE_SDIO12                        24
#define DRVSDIO_CLKSOURCE_SDIO11                        22
#define DRVSDIO_CLKSOURCE_SDIO10                        20
#define DRVSDIO_CLKSOURCE_SDIO9                         18
#define DRVSDIO_CLKSOURCE_SDIO8                         16
#define DRVSDIO_CLKSOURCE_SDIO7                         14
#define DRVSDIO_CLKSOURCE_SDIO6                         12
#define DRVSDIO_CLKSOURCE_SDIO5                         10
#define DRVSDIO_CLKSOURCE_SDIO4                         8
#define DRVSDIO_CLKSOURCE_SDIO3                         6
#define DRVSDIO_CLKSOURCE_SDIO2                         4
#define DRVSDIO_CLKSOURCE_SDIO1                         2
#define DRVSDIO_CLKSOURCE_SDIO0                         0

#define  DRVSDIO_CLKSOURCE_DIV0                         0
#define  DRVSDIO_CLKSOURCE_DIV1                         1
#define  DRVSDIO_CLKSOURCE_DIV2                         2
#define  DRVSDIO_CLKSOURCE_DIV3                         3

// CLK ENABLE
#define DRVSDIO_CLKENA_LOWPOWER_SDIO15                  31
#define DRVSDIO_CLKENA_LOWPOWER_SDIO14                  30
#define DRVSDIO_CLKENA_LOWPOWER_SDIO13                  29
#define DRVSDIO_CLKENA_LOWPOWER_SDIO12                  28
#define DRVSDIO_CLKENA_LOWPOWER_SDIO11                  27
#define DRVSDIO_CLKENA_LOWPOWER_SDIO10                  26
#define DRVSDIO_CLKENA_LOWPOWER_SDIO9                   25
#define DRVSDIO_CLKENA_LOWPOWER_SDIO8                   24
#define DRVSDIO_CLKENA_LOWPOWER_SDIO7                   23
#define DRVSDIO_CLKENA_LOWPOWER_SDIO6                   22
#define DRVSDIO_CLKENA_LOWPOWER_SDIO5                   21
#define DRVSDIO_CLKENA_LOWPOWER_SDIO4                   20
#define DRVSDIO_CLKENA_LOWPOWER_SDIO3                   19
#define DRVSDIO_CLKENA_LOWPOWER_SDIO2                   18
#define DRVSDIO_CLKENA_LOWPOWER_SDIO1                   17
#define DRVSDIO_CLKENA_LOWPOWER_SDIO0                   16
#define DRVSDIO_CLKENA_LOWPOWER_ALL                     (0xFFFF0000)

#define DRVSDIO_CLKENA_ENABLE_SDIO15                    15
#define DRVSDIO_CLKENA_ENABLE_SDIO14                    14
#define DRVSDIO_CLKENA_ENABLE_SDIO13                    13
#define DRVSDIO_CLKENA_ENABLE_SDIO12                    12
#define DRVSDIO_CLKENA_ENABLE_SDIO11                    11
#define DRVSDIO_CLKENA_ENABLE_SDIO10                    10
#define DRVSDIO_CLKENA_ENABLE_SDIO9                     9
#define DRVSDIO_CLKENA_ENABLE_SDIO8                     8
#define DRVSDIO_CLKENA_ENABLE_SDIO7                     7
#define DRVSDIO_CLKENA_ENABLE_SDIO6                     6
#define DRVSDIO_CLKENA_ENABLE_SDIO5                     5
#define DRVSDIO_CLKENA_ENABLE_SDIO4                     4
#define DRVSDIO_CLKENA_ENABLE_SDIO3                     3
#define DRVSDIO_CLKENA_ENABLE_SDIO2                     2
#define DRVSDIO_CLKENA_ENABLE_SDIO1                     1
#define DRVSDIO_CLKENA_ENABLE_SDIO0                     0
#define DRVSDIO_CLKENA_ENABLE_ALL                       (0x0000FFFF)
#define DRVSDIO_CLKENA_DISABLE_ALL                      (0x00000000)


// STATUS REGISTER
#define DRVSDIO_STATUS_DMAREQ                           31
#define DRVSDIO_STATUS_DMAACK                           30
#define DRVSDIO_STATUS_FIFOCOUNT                        17
#define DRVSDIO_STATUS_RESPINDEX                        11
#define DRVSDIO_STATUS_DATASTATEMCBUSY                  10
#define DRVSDIO_STATUS_DATABUSY                         9
#define DRVSDIO_STATUS_DATA3STATUS                      8
#define DRVSDIO_STATUS_FSMSTATE                         4
#define DRVSDIO_STATUS_FIFOFULL                         3
#define DRVSDIO_STATUS_FIFOEMPTY                        2
#define DRVSDIO_STATUS_FIFOTXWATERMARK                  1
#define DRVSDIO_STATUS_FIFORXWATERMARK                  0

// CMD REGISTER 
#define DRVSDIO_CMD_START_CMD                           31
#define DRVSDIO_CMD_USE_HOLDREGISTER                    29
#define DRVSDIO_CMD_VOLT_SWITCH                         28
#define DRVSDIO_CMD_BOOT_MODE                           27
#define DRVSDIO_CMD_DISABLE_BOOT                        26
#define DRVSDIO_CMD_EXPECT_BOOT_ACK                     25
#define DRVSDIO_CMD_ENABLE_BOOT                         24
#define DRVSDIO_CMD_CCS_EXPECTED                        23
#define DRVSDIO_CMD_READ_CEATA_DEVICE                   22
#define DRVSDIO_CMD_UPDATE_CLK_REG                      21
#define DRVSDIO_CMD_CARD_NUMBER                         16
#define DRVSDIO_CMD_SEND_INIT                           15
#define DRVSDIO_CMD_STOP_ABORT_CMD                      14
#define DRVSDIO_CMD_WAIT_PREVDATA_CMPL                  13
#define DRVSDIO_CMD_SEND_AUTO_STOP                      12
#define DRVSDIO_CMD_TRANSFER_MODE                       11
#define DRVSDIO_CMD_READ_WRITE                          10
#define DRVSDIO_CMD_DATA_EXPECTED                       9
#define DRVSDIO_CMD_CHECK_RESP_CRC                      8
#define DRVSDIO_CMD_RESP_LENGTH                         7
#define DRVSDIO_CMD_RESP_EXPECT                         6
#define DRVSDIO_CMD_CMD_INDEX                           0

#define DRVSDIO_CMD_CARD_NUMBER_MASK                    0x1F

// TMOUT REGISTER
#define DRVSDIO_TMOUT_DATATIMEOUT                       8
#define DRVSDIO_TMOUT_RESPONSETIMEOUT                   0
#define DRVSDIO_TMOUT_DATATIMEOUT_MASK                  0x00FFFFFF
#define DRVSDIO_TMOUT_RESPONSETIMEOUTMASK               0x000000FF
#define DRVSDIO_TMOUT_DEFAULT_DATATIMEOUT               0x00FFFFFF // 3-ok for CMX ; 37..39- DDR
#define DRVSDIO_TMOUT_DEFAULT_RESPONSETIMEOUT           0x000000FF

// CTYPE REGISTER
#define DRVSDIO_CTYPE_8BIT                              16
#define DRVSDIO_CTYPE_4BIT                              0

// FIFOTH REGISTER
#define DRVSDIO_FIFOTH_DMA_MULTIPLETRAN                 28
#define DRVSDIO_FIFOTH_RX_WATERMARK                     16
#define DRVSDIO_FIFOTH_TX_WATERMARK                     0
#define DRVSDIO_FIFOTH_DMA_MULTIPLETRAN_MASK            0x7
#define DRVSDIO_FIFOTH_RX_WATERMARK_MASK                0x7FF
#define DRVSDIO_FIFOTH_TX_WATERMARK_MASK                0xFFF

// BMOD REGISTER
#define DRVSDIO_BMOD_PBL                                8
#define DRVSDIO_BMOD_DE                                 7
#define DRVSDIO_BMOD_DSL                                2
#define DRVSDIO_BMOD_FB                                 1
#define DRVSDIO_BMOD_SWR                                0

// IDINTEN REGISTER
#define DRVSDIO_IDINTEN_AI                              9
#define DRVSDIO_IDINTEN_NI                              8
#define DRVSDIO_IDINTEN_CES                             5
#define DRVSDIO_IDINTEN_DU                              4
#define DRVSDIO_IDINTEN_FBE                             2
#define DRVSDIO_IDINTEN_RI                              1
#define DRVSDIO_IDINTEN_TI                              0
#define DRVSDIO_IDINTEN_ALL                             ((1 << DRVSDIO_IDINTEN_AI) | (1 << DRVSDIO_IDINTEN_NI) | (1 << DRVSDIO_IDINTEN_CES) | (1 << DRVSDIO_IDINTEN_DU) | (1 << DRVSDIO_IDINTEN_FBE) | (1 << DRVSDIO_IDINTEN_RI) | (1 << DRVSDIO_IDINTEN_TI))


// UHS_REG
#define DRVSDIO_UHS_REG_DDR_REG                         16
#define DRVSDIO_UHS_REG_VOLT_REG                        0
#define DRVSDIO_UHS_REG_DDR_REG_MASK                    (0x0000FFFF)
#define DRVSDIO_UHS_REG_VOLT_REG_MASK                   (0x0000FFFF)

// UHS_REG_EXT
#define DRVSDIO_UHS_REG_EXT_EXT_CLK_MUX_CTRL            30
#define DRVSDIO_UHS_REG_EXT_CLK_DRV_PHASE_CTRL          23
#define DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL         16
#define DRVSDIO_UHS_REG_EXT_MMC_VOLT_REG                0

// UHS_REG_EXT
//#define DRVSDIO_UHS_REG_EXT_EXT_CLK_MUX_CTRL          16
//#define DRVSDIO_UHS_REG_EXT_CLK_DRV_PHASE_CTRL        9
//#define DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL       2
//#define DRVSDIO_UHS_REG_EXT_MMC_VOLT_REG              0

// RST_N REGISTER
#define DRVSDIO_RST_N_RESETCARD15                       15
#define DRVSDIO_RST_N_RESETCARD14                       14
#define DRVSDIO_RST_N_RESETCARD13                       13
#define DRVSDIO_RST_N_RESETCARD12                       12
#define DRVSDIO_RST_N_RESETCARD11                       11
#define DRVSDIO_RST_N_RESETCARD10                       10
#define DRVSDIO_RST_N_RESETCARD9                        9
#define DRVSDIO_RST_N_RESETCARD8                        8
#define DRVSDIO_RST_N_RESETCARD7                        7
#define DRVSDIO_RST_N_RESETCARD6                        6
#define DRVSDIO_RST_N_RESETCARD5                        5
#define DRVSDIO_RST_N_RESETCARD4                        4
#define DRVSDIO_RST_N_RESETCARD3                        3
#define DRVSDIO_RST_N_RESETCARD2                        2
#define DRVSDIO_RST_N_RESETCARD1                        1
#define DRVSDIO_RST_N_RESETCARD0                        0
#define DRVSDIO_RST_N_RESETCARDALL                      (0x00000000)
#define DRVSDIO_RST_N_ACTIVECARDALL                     (0x0000FFFF)

// CARDTHRCTL REGISTER
#define DRVSDIO_CARDTHRCTL_CARDRDTHREN                  0
#define DRVSDIO_CARDTHRCTL_BSYCLRINTEN                  1
#define DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD              16
#define DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD_MASK         27

// CTYPE REGISTER

#define DRVSDIO_CTYPE_SD0_4BITS                         0
#define DRVSDIO_CTYPE_SD1_4BITS                         1
#define DRVSDIO_CTYPE_SD2_4BITS                         2
#define DRVSDIO_CTYPE_SD3_4BITS                         3
#define DRVSDIO_CTYPE_SD4_4BITS                         4
#define DRVSDIO_CTYPE_SD5_4BITS                         5
#define DRVSDIO_CTYPE_SD6_4BITS                         6
#define DRVSDIO_CTYPE_SD7_4BITS                         7
#define DRVSDIO_CTYPE_SD8_4BITS                         8
#define DRVSDIO_CTYPE_SD9_4BITS                         9
#define DRVSDIO_CTYPE_SD10_4BITS                        10
#define DRVSDIO_CTYPE_SD11_4BITS                        11
#define DRVSDIO_CTYPE_SD12_4BITS                        12
#define DRVSDIO_CTYPE_SD13_4BITS                        13
#define DRVSDIO_CTYPE_SD14_4BITS                        14
#define DRVSDIO_CTYPE_SD15_4BITS                        15
#define DRVSDIO_CTYPE_SD0_8BITS                         16
#define DRVSDIO_CTYPE_SD1_8BITS                         17
#define DRVSDIO_CTYPE_SD2_8BITS                         18
#define DRVSDIO_CTYPE_SD3_8BITS                         19
#define DRVSDIO_CTYPE_SD4_8BITS                         20
#define DRVSDIO_CTYPE_SD5_8BITS                         21
#define DRVSDIO_CTYPE_SD6_8BITS                         22
#define DRVSDIO_CTYPE_SD7_8BITS                         23
#define DRVSDIO_CTYPE_SD8_8BITS                         24
#define DRVSDIO_CTYPE_SD9_8BITS                         25
#define DRVSDIO_CTYPE_SD10_8BITS                        26
#define DRVSDIO_CTYPE_SD11_8BITS                        27
#define DRVSDIO_CTYPE_SD12_8BITS                        28
#define DRVSDIO_CTYPE_SD13_8BITS                        29
#define DRVSDIO_CTYPE_SD14_8BITS                        30
#define DRVSDIO_CTYPE_SD15_8BITS                        31

// All cards are supposed to be SD Cards 
#define DRVSDIO_CTYPE_SD0_SD15_4BITS                    0xFFFF

//----------------------------------------------------------------------------//
// DESCRIPTOR  
//----------------------------------------------------------------------------//
#define DRVSDIO_DES0_OWN                                31
#define DRVSDIO_DES0_CES                                30
#define DRVSDIO_DES0_ER                                 5
#define DRVSDIO_DES0_CH                                 4
#define DRVSDIO_DES0_FS                                 3
#define DRVSDIO_DES0_LD                                 2
#define DRVSDIO_DES0_DIC                                1

#define DRVSDIO_DES1_BS2                                13
#define DRVSDIO_DES1_BS1                                0
#define DRVSDIO_DES1_BS_MASK                            (0x1FFF)
//----------------------------------------------------------------------------//
// Argument Utilities 
//----------------------------------------------------------------------------//
#define DRVSDIO_CMD8_ARG_PATTERN                        0
#define DRVSDIO_CMD8_ARG_VOLTAGE                        8
#define DRVSDIO_CMD8_ARG_V_2_7__3_6                     1
#define DRVSDIO_CMD8_ARG_V_LOW_RESERVED                 2
#define DRVSDIO_CMD8_ARG_DEFAULT_PATTERN                0xAA

#define DRVSDIO_ACMD41_ARG_OCR                          8
#define DRVSDIO_ACMD41_ARG_S18R                         24
#define DRVSDIO_ACMD41_ARG_XPC                          28
#define DRVSDIO_ACMD41_ARG_HCS                          30
#define DRVSDIO_ACMD41_ARG_BUSY                         31
#define DRVSDIO_ACMD41_ARG_OCR_ALL                      (0xFFFF)
#define DRVSDIO_ACMD41_ARG_OCR_3_2__3_3                 20

#define DRVSDIO_R3_OCR                                  8
#define DRVSDIO_R3_S18A                                 24
#define DRVSDIO_R3_UHSII                                29
#define DRVSDIO_R3_CCS                                  30
#define DRVSDIO_R3_BUSY                                 31

#define DRVSDIO_R6_RCA                                  16

#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP1                0
#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP2                4
#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP3                8
#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP4                12
#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP5                16
#define DRVSDIO_CMD6_ARG_FUNCTION_GROUP6                20
#define DRVSDIO_CMD6_ARG_RESERVED                       24
#define DRVSDIO_CMD6_ARG_OPERATION_MODE                 31
#define DRVSDIO_CMD6_OPERATION_MODE_CHECK               0
#define DRVSDIO_CMD6_OPERATION_MODE_SWITCH              1
#define DRVSDIO_CMD6_STATUS_SUPPORT_FUNC_FG1            400
#define DRVSDIO_CMD6_STATUS_SUPPORT_FUNC_FG2            416
#define DRVSDIO_CMD6_STATUS_SUPPORT_FUNC_FG3            432
#define DRVSDIO_CMD6_STATUS_SUPPORT_FUNC_FG4            448
// Functions in 3.3V mode
#define DRVSDIO_CMD6_FUNCTION_DEFAULT                   0
#define DRVSDIO_CMD6_FUNCTION_HIGH_SPEED                1
// Functions in 1.8V mode
#define DRVSDIO_CMD6_FUNCTION_SDR_12                    0
#define DRVSDIO_CMD6_FUNCTION_SDR_25                    1
#define DRVSDIO_CMD6_FUNCTION_SDR_50                    2
#define DRVSDIO_CMD6_FUNCTION_SDR_104                   3
#define DRVSDIO_CMD6_FUNCTION_DDR_50                    4
//----------------------------------------------------------------------------//
// Hardware constrained constants 
//----------------------------------------------------------------------------//
#define DRVSDIO_MAXNUMCARDS                             16
#define DRVSDIO_MAXNUMCARDS_MASK                        (0x0000FFFF)
#define DRVSDIO_FIFO_DEPTH                              256
#define DRVSDIO_FIFO_DATA_WIDTH                         32
#define DRVSDIO_STARTFREQ                               400 // KHz
#define SYSTEMFREQ_CONFIG                               100000

//----------------------------------------------------------------------------//
// SD COMMANDS 
//----------------------------------------------------------------------------//
#define DRVSDIO_CMD0                        0
#define DRVSDIO_CMD1                        1
#define DRVSDIO_CMD2                        2
#define DRVSDIO_CMD3                        3
#define DRVSDIO_CMD4                        4
#define DRVSDIO_CMD5                        5
#define DRVSDIO_CMD6                        6
#define DRVSDIO_ACMD6                       6
#define DRVSDIO_CMD7                        7
#define DRVSDIO_CMD8                        8
#define DRVSDIO_CMD9                        9
#define DRVSDIO_CMD11                       11
#define DRVSDIO_CMD12                       12
#define DRVSDIO_CMD13                       13
#define DRVSDIO_CMD16                       16
#define DRVSDIO_CMD17                       17
#define DRVSDIO_CMD18                       18
#define DRVSDIO_CMD19                       19
#define DRVSDIO_CMD23                       23
#define DRVSDIO_CMD24                       24
#define DRVSDIO_CMD25                       25
#define DRVSDIO_CMD32                       32
#define DRVSDIO_CMD33                       33
#define DRVSDIO_CMD38                       38
#define DRVSDIO_CMD41                       41
#define DRVSDIO_ACMD51                      51
#define DRVSDIO_CMD55                       55

//----------------------------------------------------------------------------------//
// CARD UTILITIES
//----------------------------------------------------------------------------------//
#define DRVSDIO_CARD_STATUS_OUT_OF_RANGE        31
#define DRVSDIO_CARD_STATUS_ADDRESS_ERROR       30
#define DRVSDIO_CARD_STATUS_BLOCK_LEN_ERROR     29
#define DRVSDIO_CARD_STATUS_ERASE_SEQ_ERROR     28
#define DRVSDIO_CARD_STATUS_ERASE_PARAM         27
#define DRVSDIO_CARD_STATUS_WP_VIOLATION        26
#define DRVSDIO_CARD_STATUS_CARD_LOCKED         25
#define DRVSDIO_CARD_STATUS_CARD_UNLOCK_FAILED  24
#define DRVSDIO_CARD_STATUS_COM_CRC_ERROR       23
#define DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND     22
#define DRVSDIO_CARD_STATUS_CARD_ECC_FAILED     21
#define DRVSDIO_CARD_STATUS_CC_ERROR            20
#define DRVSDIO_CARD_STATUS_ERROR               19
#define DRVSDIO_CARD_STATUS_CSD_OVERWRITE       16
#define DRVSDIO_CARD_STATUS_WP_ERASE_SKIP       15
#define DRVSDIO_CARD_STATUS_CARD_ECC_DISABLED   14
#define DRVSDIO_CARD_STATUS_ERASE_RESET         13
#define DRVSDIO_CARD_STATUS_CURRENT_STATE       9
#define DRVSDIO_CARD_STATUS_READY_FOR_DATA      8
#define DRVSDIO_CARD_STATUS_APP_CMD             5
#define DRVSDIO_CARD_STATUS_AKE_SEQ_ERROR       3

#define DRVSDIO_CARD_STATUS_CURRENT_STATE_MASK  0x0F
#define DRVSDIO_CARD_STATUS_ERASESEQ_FLAGS_MASK ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) | (1 << DRVSDIO_CARD_STATUS_ERASE_SEQ_ERROR) | (1 << DRVSDIO_CARD_STATUS_ERASE_PARAM) | (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) | \
                                                (1 << DRVSDIO_CARD_STATUS_CC_ERROR) | (1 << DRVSDIO_CARD_STATUS_ERROR) | (1 << DRVSDIO_CARD_STATUS_WP_ERASE_SKIP) | (1 << DRVSDIO_CARD_STATUS_ERASE_RESET))
#define DRVSDIO_CSDLENGTH                       4
#define DRVSDIO_CIDLENGTH                       4

//----------------------------------------------------------------------------------//
// HOST VOLTAGE RANGES 
//----------------------------------------------------------------------------------//
#define DRVSDIO_VDD_16_17                 (1u << 4)
#define DRVSDIO_VDD_17_18                 (1u << 5)
#define DRVSDIO_VDD_18_19                 (1u << 6)
#define DRVSDIO_VDD_19_20                 (1u << 7)
#define DRVSDIO_VDD_20_21                 (1u << 8)
#define DRVSDIO_VDD_21_22                 (1u << 9)
#define DRVSDIO_VDD_22_23                 (1u << 10)
#define DRVSDIO_VDD_23_24                 (1u << 11)
#define DRVSDIO_VDD_24_25                 (1u << 12)
#define DRVSDIO_VDD_25_26                 (1u << 13)
#define DRVSDIO_VDD_26_27                 (1u << 14)
#define DRVSDIO_VDD_27_28                 (1u << 15)
#define DRVSDIO_VDD_28_29                 (1u << 16)
#define DRVSDIO_VDD_29_30                 (1u << 17)
#define DRVSDIO_VDD_30_31                 (1u << 18)
#define DRVSDIO_VDD_31_32                 (1u << 19)
#define DRVSDIO_VDD_32_33                 (1u << 20)
#define DRVSDIO_VDD_33_34                 (1u << 21)
#define DRVSDIO_VDD_34_35                 (1u << 22)
#define DRVSDIO_VDD_35_36                 (1u << 23)
#define DRVSDIO_CARD_BUSY                 (1u << 31)

#define DRVSDIO_HOST_VOLT_RANGE    (DRVSDIO_VDD_27_28  |\
                                    DRVSDIO_VDD_28_29  |\
                                    DRVSDIO_VDD_29_30  |\
                                    DRVSDIO_VDD_30_31  |\
                                    DRVSDIO_VDD_31_32  |\
                                    DRVSDIO_VDD_32_33)
//----------------------------------------------------------------------------------//
// R1 Response 
//----------------------------------------------------------------------------------//

// Flag used to mark an error coming from the response to a command
#define DRVSDIO_R1_ERROR_FLAG                       31
//----------------------------------------------------------------------------------//
// MACROS 
//----------------------------------------------------------------------------------//
#define DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(x)       ((x) & (DRVSDIO_COMMANDDONE_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_HLE)))
#define DRVSDIO_TUNING_ISTHERE_COMMAND_DONE_ERROR(x)       ((x) & ((1 << DRVSDIO_RAWINTSTATUS_RE) | (1 << DRVSDIO_RAWINTSTATUS_RCRC) | (1 << DRVSDIO_RAWINTSTATUS_HLE)))
#define DRVSDIO_CSD_GET_CSD_STRUCTURE(csd) ((csd) [0] >> 6)
#define DRVSDIO_CSD_GET_SPEC_VERS(csd) (((csd) [0] >> 2) & 0xf)
#define DRVSDIO_CSD_GET_TAAC(csd) ((csd) [1])
#define DRVSDIO_CSD_GET_NSAC(csd) ((u32) (csd) [2])
#define DRVSDIO_CSD_GET_TRAN_SPEED(csd) ((csd) [3])
#define DRVSDIO_CSD_GET_READ_BLK_LEN(csd) ((u32) (csd) [5] & 0xf)
#define DRVSDIO_CSD_GET_WRITE_BLK_LEN(csd) ((((u32) (csd) [12] & 0x3) << 2) + ((((u32) (csd) [13]) >> 6) & 0x3))
#define DRVSDIO_CSD_GET_C_SIZE(csd) ((((u64) (csd) [7] & 0x3f) << 16) + (((u64) (csd) [8]) << 8) + (u64) (csd) [9])


#define DRVSDIO_GETFLAGS                            DrvSdio_RInterrupts
#define DRVSDIO_CLEARFLAGS(flags)                       do { DrvIcbDisableIrq(IRQ_SDIO);  DrvSdio_RInterrupts &= ~(flags); DrvIcbEnableIrq(IRQ_SDIO); } while (0)

// Get the value to program the divider with based on clock input and desired clock output
#define DRVSDIO_GET_DIV_VALUE(cclk_in_Khz, sdfreq_Khz)      ((cclk_in_Khz)/ ((sdfreq_Khz) << 1))

// Useful macro to test result WR TRUE success, FALSE error
#define DRVSDIO_VERIFY_WR_BLOCK_RESULT(flags)           (((flags) & (DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS | DRVSDIO_COMMANDDONE_ERROR_FLAGS)) ? FALSE : TRUE)
// Useful macro to test result TRUE success, FALSE error
#define DRVSDIO_VERIFY_RD_BLOCK_RESULT(flags)           (((flags) & (DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | DRVSDIO_COMMANDDONE_ERROR_FLAGS)) ? FALSE : TRUE)
// Useful macro to test result TRUE success, FALSE error
#define DRVSDIO_VERIFY_ERASE_RESULT(flags)              (((flags) & DRVSDIO_COMMANDDONE_ERROR_FLAGS) ? FALSE: TRUE)
#define DRVSDIO_SWAP_BYTES(flags)                       ((u16)(((flags) << 8) | ((flags) >> 8)))

#define SDIO_INVALID_SPEED_MODE 0xFF

#define DRVSDIO_TUNING_BLOCK_SZ 64
#define DRVSDIO_TUNING_MAX_LOOP 40
#define DRVSDIO_TUNING_BLOCK_TX_SZ 64
#define DRVSDIO_TUNING_DELAY_RANGE 16
#define DRVSDIO_TUNING_RX_WMARK (DRVSDIO_TUNING_BLOCK_SZ - 1)
#define DRVSDIO_TUNING_TX_WMARK DRVSDIO_TUNING_BLOCK_SZ
#define DRVSDIO_TUNING_DMASIZE  3
#define DRVSDIO_TIMEOUT 0xFFFF

#define DRVSDIO_TUNING_PATTERN { 0x00FF0FFF, 0xCCC3CCFF, 0xFFCC3CC3, 0xEFFEFFFE, \
                                 0xDDFFDFFF, 0xFBFFFBFF, 0xFF7FFFBF, 0xEFBDF777, \
                                 0xF0FFF0FF, 0x3CCCFC0F, 0xCFCC33CC, 0xEEFFEFFF, \
                                 0xFDFFFDFF, 0xFFBFFFDF, 0xFFF7FFBB, 0xDE7B7FF7 }

#define SDIO_HIGHEST_SPEEDMODE_SUPPORTED SDIO_SDR50
#define DRVSDIO_1v8_VALUE 1800
#define DRVSDIO_3v3_VALUE 3300

//----------------------------------------------------------------------------//
// STATUS FLAGS
//----------------------------------------------------------------------------//
// CARDS
#define DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED           0

// DRIVER
#define DRVSDIO_DRIVER_INITIALIZED                        0
//----------------------------------------------------------------------------//
// DATA TYPES
//----------------------------------------------------------------------------//
typedef struct
{
    u32 buffer;
    u32 size;
}tyDrvSdio_Transaction;

typedef enum{
    DRVSDIO_1v8,
    DRVSDIO_3v3
} tyDrvPmicVolt;

typedef struct
{
    u32 descriptor0;
    u32 descriptor1;
    u32 descriptor2;
    u32 descriptor3;
}DrvSdio_Descriptors;

typedef enum
{
    //3.3V
    SDIO_DEFAULT_SPEED = 0,
    SDIO_HIGH_SPEED    = 1,
    // 1.8V
    SDIO_UHS_I = 2,
    SDIO_SDR12 = SDIO_UHS_I,
    SDIO_SDR25,
    SDIO_SDR50,
    SDIO_SDR104, // not yet supported
    SDIO_DDR50   // not yet supported
} tyDrvSdioSpeedMode;

// Voltage switch callback
typedef int (*DrvSdioVoltSwitchFunc)(tyDrvPmicVolt voltage);

typedef struct
{
    DrvSdioVoltSwitchFunc callback;
    tyDrvSdioSpeedMode max_speed_mode;
} DrvSdioOpts;

typedef struct
{
    u8 useHoldRegister; // Use hold register
    u8 busWidth; // Bus Width
    u8 sdhc; // HC card
    u16 statusFlags;
    u16 blockSize; // BlockSize
    u16 relativeCardAddress; // RCA
    u32 csd[DRVSDIO_CSDLENGTH]; // CSD Register
    u32 cid[DRVSDIO_CIDLENGTH]; // CSI Register
    DrvSdioOpts card_opts;
}tyDrvSdioCardConfig;

// TBD Move this ENUM to DrvCommon.h when it is updated
typedef enum
{
    DRVSDIO_SUCCESS = MYR_DRV_SUCCESS,
    DRVSDIO_ERROR = MYR_DRV_ERROR,
    DRVSDIO_NOT_INITIALIZED = MYR_DRV_NOT_INITIALIZED,
    DRVSDIO_CARDSLOT_OUT_OF_RANGE = MYR_DRV_CUSTOM_CODE_START_OFFSET,
    DRVSDIO_CARD_TYPE_NOT_SUPPORTED

} DRVSDIO_RETURN_TYPE;

#endif

///@}
