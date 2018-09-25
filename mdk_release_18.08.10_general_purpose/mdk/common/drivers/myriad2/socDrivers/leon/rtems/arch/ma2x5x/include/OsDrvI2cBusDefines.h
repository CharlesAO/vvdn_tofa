///
/// @file OsDrvI2cBusDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvI2cBus
/// @{
/// @brief I2C Bus master generic driver header defines
///

#ifndef I2C_BUS_DEFINES_H
#define I2C_BUS_DEFINES_H
#include <rtems.h>
#include "OsCommon.h"
#include "mv_types.h"
// 1: Defines
// ----------------------------------------------------------------------------
//#define OS_DRV_I2C_L1
//#define OS_DRV_I2C_L2
//#define OS_DRV_I2C_L3

#ifdef OS_DRV_I2C_L1
    #define OS_I2C_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_I2C_DPRINTF1(...)
#endif

#ifdef OS_DRV_I2C_L2
    #define OS_I2C_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_I2C_DPRINTF2(...)
#endif

#ifdef OS_DRV_I2C_L3
    #define OS_I2C_DPRINTF3(...)            printf(__VA_ARGS__)
#else
    #define OS_I2C_DPRINTF3(...)
#endif

// Register offsets and bit definitions for I2C Blocks
#define IIC_CON (0x00)
#define IIC_TAR (0x04)
#define IIC_SAR (0x08)
#define IIC_HS_MADDR (0x0c)
#define IIC_DATA_CMD (0x10)
#define IIC_SS_HCNT (0x14)
#define IIC_SS_LCNT (0x18)
#define IIC_FS_HCNT (0x1c)
#define IIC_FS_LCNT (0x20)
#define IIC_HS_HCNT (0x24)
#define IIC_HS_LCNT (0x28)
#define IIC_INTR_STAT (0x2c)
#define IIC_INTR_MASK (0x30)
#define IIC_RAW_INTR_STAT (0x34)i2c_device_speed_
#define IIC_RX_TL (0x38)
#define IIC_TX_TL (0x3c)
#define IIC_CLR_INTR (0x40)
#define IIC_CLR_RX_UNDER (0x44)
#define IIC_CLR_RX_OVER (0x48)
#define IIC_CLR_TX_OVER (0x4c)
#define IIC_CLR_RD_REQ (0x50)
#define IIC_CLR_TX_ABRT (0x54)
#define IIC_CLR_RX_DONE (0x58)
#define IIC_CLR_ACTIVITY (0x5c)
#define IIC_CLR_STOP_DET (0x60)
#define IIC_CLR_START_DET (0x64)
#define IIC_CLR_GEN_CALL (0x68)
#define IIC_ENABLE (0x6c)
#define IIC_STATUS (0x70)
#define IIC_TXFLR (0x74)
#define IIC_RXFLR (0x78)
#define IIC_SDA_HOLD (0x7c)
#define IIC_TX_ABRT_SOURCE (0x80)
#define IIC_VERSION_ID (0xf8)
#define IIC_DMA_CR (0x88)
#define IIC_DMA_TDLR (0x8c)
#define IIC_DMA_RDLR (0x90)
#define IIC_COMP_PARAM_1 (0xf4)
#define IIC_COMP_VERSION (0xf8)
#define IIC_COMP_TYPE (0xfc)

/* IIC_RAW_INTR_STAT */
#define IIC_IRQ_GEN_CALL 0x0800
#define IIC_IRQ_START_DET 0x0400
#define IIC_IRQ_STOP_DET 0x0200
#define IIC_IRQ_ACTIVITY 0x0100
#define IIC_IRQ_RX_DONE 0x0080
#define IIC_IRQ_TX_ABRT 0x0040
#define IIC_IRQ_RD_REQ 0x0020
#define IIC_IRQ_TX_EMPTY 0x0010
#define IIC_IRQ_TX_OVER 0x0008
#define IIC_IRQ_RX_FULL 0x0004
#define IIC_IRQ_RX_OVER 0x0002
#define IIC_IRQ_RX_UNDER 0x0001

/* IIC_STATUS */
#define IIC_STATUS_ACTIVITY_SLAVE (0x40)
#define IIC_STATUS_ACTIVITY_MASTER (0x20)
#define IIC_STATUS_RX_FIFO_FULL (0x10)
#define IIC_STATUS_RX_FIFO_NOT_EMPTY (0x08)
#define IIC_STATUS_TX_FIFO_EMPTY (0x04)
#define IIC_STATUS_TX_FIFO_NOT_FULL (0x02)
#define IIC_STATUS_ACTIVITY (0x01)

/* IIC_DATA_CMD */
#define IIC_DATA_CMD_READ 8
#define IIC_DATA_CMD_STOP 9
#define IIC_DATA_CMD_RESTART 10

/* Offsets applied to SDA HOLD REGISTER */
#define IIC_SDAHOLD_RX 16
#define IIC_SDAHOLD_TX 0

/* Offsets applied to IC_CON REGISTER */
#define IIC_CON_MASTERMODE 0
#define IIC_CON_ICSLAVEDISABLE 6
#define IIC_CON_IC10BITADDRSLAVE 3
#define IIC_CON_IC10BITADDRMASTER 4
#define IIC_CON_ICRESTARTEN 5
#define IIC_CON_SPEED 1

/* Mask to apply to fields in IC_CON */
#define IIC_CON_MASTERMODE_MASK 1
#define IIC_CON_ICSLAVEDISABLE_MASK 1
#define IIC_CON_IC10BITADDRSLAVE_MASK 1
#define IIC_CON_IC10BITADDRMASTER_MASK 1
#define IIC_CON_ICRESTARTEN_MASK 1
#define IIC_CON_SPEED_MASK 3

/* Mask for enable register */
#define IIC_ENABLE_BITMASK 0x1

/* Offsets applied to IC_COMP_PARAM_1 */
#define IIC_COMP_PARAM_1_TXFIFO 16
#define IIC_COMP_PARAM_1_RXFIFO 8
/* Masks applied to IC_COMP_PARAM_1 */
#define IIC_COMP_PARAM_1_TXFIFOMASK 0xFF
#define IIC_COMP_PARAM_1_RXFIFOMASK 0xFF

/* Offsets applied to IC_TAR */
#define IIC_TAR_10BITADDR_MASTER 12

/* Current FIFO depth */
#define IIC_TX_FIFO_DEPTH 16
#define IIC_RX_FIFO_DEPTH 16

// 1: Defines
// ----------------------------------------------------------------------------
// Master status
#define I2C_MASTER_IDLE 0 // Master is Idle
#define I2C_MASTER_TRANSMISSION 1 // Transfer in progress
#define I2C_MASTER_RECEPTION 2 // Receive in progress

#define I2C_MASTER_MAX_OP_TIMEOUT 1000 // Maximum timeout

#define I2C_TX_ABORT_ERRORS_NR 17
#define OS_I2CM_STAT_ABRT_ENUM_OFFSET 1

#define MYR2_I2C_DEVICES 3
/// Availalbe gpio's for I2C0 device
#define I2C0_CFG0_SCL 60
#define I2C0_CFG0_SDA 61
#define I2C0_CFG0_MODE 0
#define I2C0_CFG1_SCL 10
#define I2C0_CFG1_SDA 11
#define I2C0_CFG1_MODE 1
#define I2C0_CFG2_SCL 20
#define I2C0_CFG2_SDA 21
#define I2C0_CFG2_MODE 4

/// Availalbe gpio's for I2C1 device
#define I2C1_CFG0_SCL 12
#define I2C1_CFG0_SDA 13
#define I2C1_CFG0_MODE 2
#define I2C1_CFG1_SCL 49
#define I2C1_CFG1_SDA 50
#define I2C1_CFG1_MODE 4
#define I2C1_CFG2_SCL 62
#define I2C1_CFG2_SDA 63
#define I2C1_CFG2_MODE 0
#define I2C1_CFG3_SCL 77
#define I2C1_CFG3_SDA 78
#define I2C1_CFG3_MODE 4

/// Availalbe gpio's for I2C2 device
#define I2C2_CFG0_SCL 47
#define I2C2_CFG0_SDA 48
#define I2C2_CFG0_MODE 4
#define I2C2_CFG1_SCL 79
#define I2C2_CFG1_SDA 80
#define I2C2_CFG1_MODE 2

// THis values are used by the I2C SCL Low and HIgh timing calculation
#define I2C_SCL_FALL_TIME_NS 100
#define I2C_SCL_RISE_TIME_NS 100
#define I2C_SPKLEN_SS_FS_FSP 5
#define I2C_SPKLEN_HS        1

#define I2C_SS_MOD           1
#define I2C_FS_MOD           2
#define I2C_HS_MOD           3

// 4700+5200 ~= 1/100  ~= 10000ns
#define I2C_SS_HCNT 4700
#define I2C_SS_LCNT 5200
// 1160+1330 ~= 1/400  ~= 2500ns
#define I2C_FS_HCNT 1190
#define I2C_FS_LCNT 1350
// 500 +500  ~= 1/1000 ~= 1000ns
#define I2C_FSP_HCNT 500
#define I2C_FSP_LCNT 500
// 165 +125  ~= 1/3400 ~= 290ns
#define I2C_HS_HCNT 165
#define I2C_HS_LCNT 125

enum i2cSpeed
{
    I2C_SPEED_SS  = 0, //100Kbits/s
    I2C_SPEED_FS  = 1, //400Kbits/s
    I2C_SPEED_FSP = 2, //1.0Mbits/s
    I2C_SPEED_HS  = 3  //3.4Mbits/s
};

#define I2C_SPEED_MODES   4

typedef struct
{
    u32 hcnt;
    u32 lcnt;
    u32 mode;
}i2cConfigModes;

typedef struct i2cDeviceConfigs
{
    enum i2cSpeed i2cDeviceSpeed;
    u32 addr10bit;
    u32 interruptPriority;
} i2cDeviceConfigs_t;

typedef enum 
{
    OS_MYR_DRV_I2C_INVALID_CLOCK = MYR_DRV_CUSTOM_START_CODE
} OS_MYR_DRV_I2C_STATUS_CODE;

typedef enum
{
    OS_I2CM_STAT_ABRT_NO_ERROR_RECEIVED    = 0,
    OS_I2CM_STAT_ABRT_7B_ADDR_NOACK        = 1,
    OS_I2CM_STAT_ABRT_10ADDR1_NOACK        = 2,
    OS_I2CM_STAT_ABRT_10ADDR2_NOACK        = 3,
    OS_I2CM_STAT_ABRT_TXDATA_NOACK         = 4,
    OS_I2CM_STAT_ABRT_GCALL_NOACK          = 5,
    OS_I2CM_STAT_ABRT_GCALL_READ           = 6,
    OS_I2CM_STAT_ABRT_HS_ACKDET            = 7,
    OS_I2CM_STAT_ABRT_SBYTE_ACKDET         = 8,
    OS_I2CM_STAT_ABRT_HS_NORSTRT           = 9,
    OS_I2CM_STAT_ABRT_SBYTE_NORSTRT        = 10,
    OS_I2CM_STAT_ABRT_10B_RD_NORSTRT       = 11,
    OS_I2CM_STAT_ABRT_MASTER_DIS           = 12,
    OS_I2CM_STAT_ABRT_LOST                 = 13,
    OS_I2CM_STAT_ABRT_SLV_FLUSH_TXFIFO     = 14,
    OS_I2CM_STAT_ABRT_SLV_ARBLOST          = 15,
    OS_I2CM_STAT_ABRT_SLVDR_INTX           = 16,
    OS_I2CM_STAT_ABRT_USER                 = 17
} i2cTxAbrtSource;

#endif

///@}
