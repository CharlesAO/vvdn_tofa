///
/// @file DrvI2cMaster.h
/// @copyright All code copyright Movidius Srl 2012, Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvI2cMaster I2cMaster Driver
/// @{
/// @brief I2C Master Driver
///
///  This module is a general purpose I2C master driver
///  It is a unified driver supporting both the Myriad I2C 
///  hardware module AND the use of GPIOs to bit bash.
///  The module aims to improve on existing Myriad I2C drivers
///  in the following ways
///  - Same API supports HW and Bitbash
///  - Support for error detection and reporting
///  - Option for unified error handler callback
///  - No need to re-init to change target slave
///  - Protocol driven to avoid the need to have 20 different I2C functions
///

#ifndef _BRINGUP_SABRE_IIC_MASTER_H_
#define _BRINGUP_SABRE_IIC_MASTER_H_


/******************************************************************************
 1: Included types first then APIs from other modules
******************************************************************************/

#include "DrvI2cMasterDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

                        
/******************************************************************************
 2:  Exported variables --- these should be avoided at all costs
******************************************************************************/

/******************************************************************************
 3:  Exported Functions
******************************************************************************/

/**  Initialise the I2C Driver Module and configure its parameters

 @param     dev Pointer to an #I2CM_Device structure which will be used to store the device state
 @param     module Enum #I2CMModuleOption to specify choice of hardware block or bitbash
 @param     sclPin GPIO number of the pin to be used for the SCL line
 @param     sdaPin GPIO number of the pin to be used for the SDA line
 @param     speedKhz Target speed of the I2C bus in Khz
 @param     addrType Enum #I2CMAddrType to specify 7 or 10 bit addressing
 @return    #I2CM_StatusType to report either success (0) or give error code.
 @note      The #I2CM_Device structure is to be treated by the programmer as an opaque structure
            No access should be made to its members except via recognised I2CM driver API calls
*/
I2CM_StatusType DrvI2cMInit(I2CM_Device * dev,I2CMModuleOption module, u32 sclPin,u32 sdaPin,u32 speedKhz,I2CMAddrType addrType);


/** Initialise the I2C Driver Module and configure its parameters basesd on a tyI2cConfig structure

 @param     dev Pointer to an #I2CM_Device structure which will be used to store the device state
 @param     config pointer to a tyI2cConfig structure that contains the configuration parameters
 @return    #I2CM_StatusType to report either success (0) or give error code.
 @note      The #I2CM_Device structure is to be treated by the programmer as an opaque structure
            No access should be made to its members except via recognised I2CM driver API calls
*/

I2CM_StatusType DrvI2cMInitFromConfig(I2CM_Device *dev, const tyI2cConfig *config);

/**  Perform an I2C Master Transaction to either Read or Write data to an I2C bus

 @param     devHandle Pointer to a preinitialised #I2CM_Device handle 
 @param     slaveAddr I2C Slave address of the target device in **7-BIT** Format 
 @param     regAddr NOT COMMENTED!
 @param     proto Array of u8 values which defines the target protocol to be used for this transaction.
 See \#I2CM_Usage for a description of how this array is used. 
 @param     dataBuffer u8 * to dataBuffer which is either source or target of operation depending on proto
 @param     dataBufferSize Size of data to be read/written in bytes. (Read/Write depends on proto)
 @return    #I2CM_StatusType to report either success (0) or give error code. 
*/
I2CM_StatusType DrvI2cMTransaction(I2CM_Device * devHandle, u32 slaveAddr,u32 regAddr,u8 * proto, u8 * dataBuffer, u32 dataBufferSize);

/**  Specify a callback function which is to be used to collect all errors returned.
 By default the I2CMaster driver Transaction will return an error code
 In order to support legacy code which does not handle such errors
 or code which is unable to handle such errors, it is possible to specify
 a callback function which will be used when an error occurs.
 Typical usage might include:
 - Raising of an assert in debug builds.
 - Keeping a simple error count. 
 - Handling the error, by reportin it to the user.
 @note The callback function is of the type #I2CErrorHandler. This function also returns an #I2CM_StatusType which 
 may either be the original error value or can be overridden to #I2CM_STAT_OK so that the callee doesn't see the error.
 @param    dev     devHandle Pointer to a preinitialised I2CM_Device handle 
 @param    handler Function pointer of type #I2CErrorHandler for callback to be used 
 @return    I2CM_StatusType to report either success (0) or give error code.
 @note   Only the #DrvI2cMTransaction function will trigger the callback as DrvI2cMInit cannot do so.
*/
I2CM_StatusType DrvI2cMSetErrorHandler(I2CM_Device * dev,I2CErrorHandler handler);


/// Simple wrapper for I2C transaction that reads 1 byte from a slave using
/// default 8 bit read
/// @param[in] dev Handle to target I2C device
/// @param[in] slaveAddr 7 bit I2C Slave Address of the target device
/// @param[in] regAddr address if the register to read within the slave device 
/// @return    
u8   DrvI2cMReadByte(I2CM_Device * dev,u32 slaveAddr,int regAddr);

/// Simple wrapper for I2C transaction that writes 1 byte to a slave using
/// default 8 bit write
/// @param[in] dev Handle to target I2C device
/// @param[in] slaveAddr 7 bit I2C Slave Address of the target device
/// @param[in] regAddr address if the register to read within the slave device 
/// @param[in] value 8 bit value to be written to the register
/// @return    
void DrvI2cMWriteByte(I2CM_Device * dev,u32 slaveAddr,u32 regAddr, u8 value);

/// Retrieve a copy of the settings which were used to configure this I2C device
///
/// This can be useful if there is a need to temporarily change I2C config and 
/// restore a backup of the config.
///
/// @param[in] dev - device handle pointer 
/// @param[in] currentConfig - pointer to storage into which the results will be placed
/// @return    
void DrvI2cMGetCurrentConfig(I2CM_Device * dev,tyI2cConfig * currentConfig);

#ifdef __cplusplus
}
#endif

#endif // _BRINGUP_SABRE_IIC_MASTER_H_

/// @}


