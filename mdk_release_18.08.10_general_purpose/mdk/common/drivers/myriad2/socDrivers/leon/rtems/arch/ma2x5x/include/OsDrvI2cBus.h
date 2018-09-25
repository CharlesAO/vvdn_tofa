///
/// @file OsDrvI2cBus.h
/// 
/// 
/// @defgroup OsDrvI2cBus OsI2cBus Driver
/// @{
/// @brief API functions for I2cBus Driver. 
///

#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <rtems/libi2c.h>
#include <rtems.h>
#include "OsDrvI2cBusDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// This define should be equal to the system frequency in Khz
//#define I2C_GET_SYSTEM_FREQ_KHZ 19040
// Work out SCL High and Low based on system frequency in Khz and time desired in NS
//#define I2C_GET_SCL_UTILITY(timeNs_) ((timeNs_ * I2C_GET_SYSTEM_FREQ_KHZ + 500000)/ 1000000)

/// DECLARE_I2C_BUS description:
//      busName_ I2C Bus Name
//      busIndex_ I2C Bus Index. Can be 1, 2 or 3 for I2C1, I2C2 and I2C3
//      pin_cfg_ refer to the global myr_i2c_gpio_cfg for details
//      i2cDeviceSpeed_ can be 1, 2 or 3 for I2C Slow, Fast or High Speed
//      addr10bit_ can be 0 for 7 bits address or 1 for 10 bits address
//      interrupt_type_ check ICB for interrupt types
//      interruptPriority_ priority from 0 to 15
#define DECLARE_I2C_BUS(busName_, busIndex_, i2cDeviceSpeed_,   \
                        addr10bit_, interruptPriority_) \
myr2I2cBusEntry_t busName_ = \
{                                       \
    .bus =                              \
    {                                   \
        .ops = &myr2I2cOps,           \
        .size = sizeof(myr2I2cBusEntry_t)   \
    },                                         \
    .cfg = { .i2cDeviceSpeed = i2cDeviceSpeed_,     \
    .addr10bit = addr10bit_, \
    .interruptPriority = interruptPriority_ \
                            },    \
    .regs = (volatile myr2I2cRegs_t *) IIC##busIndex_##_BASE_ADR,   \
    .index = busIndex_ - 1,                                           \
    .vector = IRQ_IIC1 + busIndex_ - 1                                   \
};


typedef struct myr2I2cRegs
{
    volatile u32 control;
    volatile u32 targetAddr;
    volatile u32 slaveAddr;
    volatile u32 masterAddr;
    volatile u32 cmdData;
    volatile u32 ssSclHcnt;
    volatile u32 ssSclLcnt;
    volatile u32 fsSclHcnt;
    volatile u32 fsSclLcnt;
    volatile u32 hsSclHcnt;
    volatile u32 hsSclLcnt;
    volatile u32 intrStat;
    volatile u32 intrMask;
    volatile u32 rawIntrStat;
    volatile u32 rxThresh;
    volatile u32 txThresh;
    volatile u32 clearIntr;
    volatile u32 clearRxUnderIntr;
    volatile u32 clearRxOverIntr;
    volatile u32 clearTxOverIntr;
    volatile u32 clearRdReqIntr;
    volatile u32 clearTxAbrtIntr;
    volatile u32 clearRxDoneIntr;
    volatile u32 clearActivityIntr;
    volatile u32 clearStopDetIntr;
    volatile u32 clearStartDetIntr;
    volatile u32 clearGenCallIntr;
    volatile u32 enable;
    volatile u32 status;
    volatile u32 txLevel;
    volatile u32 rxLevel;
    volatile u32 sdaHold;
    volatile u32 txAbortSource;
    volatile u32 slvDataNackOnly;
    volatile u32 dmaCr;
    volatile u32 dmaTdlr;
    volatile u32 dmaRdlr;
    volatile u32 sdaSetup;
    volatile u32 ackGeneralCall;
    volatile u32 enableStatus;
    volatile u32 fsSpklen;
    volatile u32 hsSpklen;
    volatile u32 clrRestartDet;
    volatile u32 compParam1;
    volatile u32 compVersion;
    volatile u32 compType;
} myr2I2cRegs_t;


typedef struct myr2I2cBusEntry{
  rtems_libi2c_bus_t bus;
  volatile myr2I2cRegs_t *regs;
  i2cDeviceConfigs_t cfg;
  size_t index;
  rtems_vector_number vector;
  rtems_id taskId;
  u32 interruptStatus;
  u32 rxReq;
  u8 sendStart;
  u8 sendStop;
  u8 *volatile data;
  u8 *volatile end;
  u32 txAbrtSource;
} myr2I2cBusEntry_t;


typedef enum tyI2cControl
{
    OS_I2CM_GET_LAST_ERROR = 1,
}i2cControl_t;


extern const rtems_libi2c_bus_ops_t myr2I2cOps;

#ifdef __cplusplus
}
#endif


#endif

///@}
