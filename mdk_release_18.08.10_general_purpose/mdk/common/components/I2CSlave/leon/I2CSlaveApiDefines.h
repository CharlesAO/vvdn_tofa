///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup I2CSlaveApiDefines I2C Slave API Defines
/// @ingroup  I2CSlave
/// @{
/// @brief     Definitions and types needed by the I2C Slave
///
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for the I2C Slave component
///

#ifndef _I2C_SLAVE_DEFINES_H
#define _I2C_SLAVE_DEFINES_H

// System Includes
// ----------------------------------------------------------------------------
#include "DrvI2cDefines.h"
#include "mv_types.h"

// Application Includes
// ----------------------------------------------------------------------------


// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#ifndef I2C_SLAVE_INT_LEVEL
#define I2C_SLAVE_INT_LEVEL 14
#endif

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

/// @brief Callback function executed when I2C masters reads data from slave
/// @param[in]  reg 		   	- current register address
/// @param[out] valPtr  	- pointer to the region in register memory at current register address
/// @param[out] readCount - number of bytes to be send to I2C master
typedef void (i2cReadAction)(u32 reg, void *valPtr, u32* readCount);

/// @brief Callback function executed when I2C masters writes data to slave
/// @param[in]  reg 			  - register address read from I2C master
/// @param[in]  val 		    - the value to be read from I2C master
/// @param[out] valPtr 	  - pointer to the region in register memory starting at register read from master
/// @param[in]  valByteNo - number of bytes read from I2C master
typedef void (i2cWriteAction)(u32 reg, u32 val, void* valPtr, u32 valByteNo);

/// @brief Structure that holds I2C slave configuration
typedef struct i2cSlaveConfig_t
{
	/// @brief The base address of the I2C slave device (can be on of IIC1_BASE_ADR, IIC2_BASE_ADR)
    u32 device; 
	/// @brief GPIO number corresponding to I2C clock pin
    u8  sclPin; 
	/// @brief GPIO number corresponding to I2C data pin
    u8  sdaPin; 
	/// @brief I2C operating speed in KHz
    u32 speedKhz; 
	/// @brief Contains device's address size: can be 7 bits (ADDR_7BIT) or 10 bits (ADDR_10BIT)
    u32 addressSize; 
}i2cSlaveConfig_t;

/// @brief List of I2C slave actions
typedef enum i2cSlaveAction_t
{
	/// @brief Read from I2C slave
    READ_I2C,
	/// @brief Write to I2C slave
    WRITE_I2C,
	/// @brief End I2C connection
    END_I2C
}i2cSlaveAction_t;

/// @brief 
typedef struct i2cSlaveHandle_t
{
    I2CM_Device *i2cDevHandle;
    i2cSlaveConfig_t i2cConfig;
    i2cReadAction* cbReadAction;
    i2cWriteAction* cbWriteAction;
    u32 irqSource;
}i2cSlaveHandle_t;

// contains information about I2C slave register memory
typedef struct i2cSlaveAddrCfg_t
{
    u32 slvAddr; ///< I2C slave device address
    u32 regMemAddr; ///< starting address of register memory
    u32 regMemByteSize; ///< size in bytes of register memory
    u32 dataByteNo; ///< size in bytes for each register in register memory
}i2cSlaveAddrCfg_t;

// states for I2C slave state machine
typedef enum i2cSlaveState_t
{
    I2CSLAVE_ADDRESS,
    I2CSLAVE_WRITE,
    I2CSLAVE_READ,
    I2CSLAVE_STOPPED,
}i2cSlaveState_t;


typedef struct i2cStat_t
{
    i2cSlaveHandle_t *i2cHndl;
    u8 *regSpaceMemory;
    u32 regMemByteSize;
    volatile u32 addrReg;
    volatile u32 callbackAddressReg;
    u32 addrSize;
    u32 dataSize;
    volatile i2cSlaveState_t i2cState;
    volatile u32 receivedBytes;
}i2cStat_t;


enum
{
    NO_CMD = -1,
    READ_CMD = 0,
    WRITE_CMD = 1,
};

enum
{
    I2C_BLOCK_1 = 0,
    I2C_BLOCK_2 = 1,
    I2C_BLOCK_3 = 2,
};

#define I2C_SPEED_SLOW_KHZ  100
#define I2C_SPEED_FAST_KHZ  400
#define I2C_SPEED_HIGH_KHZ 3400

/// @}
#endif
