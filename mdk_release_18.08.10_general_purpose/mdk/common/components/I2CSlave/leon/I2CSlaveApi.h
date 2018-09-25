///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup I2CSlaveApi I2C Slave API
/// @ingroup  I2CSlaveApi
/// @{
/// @brief     I2C Slave Functions API.
/// 
///  @par Component Usage
///
///  In order to use the component the following steps are ought to be done:
///   -# Declare a variable of i2cSlaveAddrCfg_t type
///   -# Initialize the members of the variable above declared
///   -# Declare a variable of i2cSlaveHandle_t type (handler)
///   -# Initialize a member of the handler declared, i2cConfig_t
///   -# Call I2CSlaveSetupCallbacks() function, having as parameters address of handler declared,
///                       result of i2cRead function, result of i2cRead function
///   -# Call I2CSlaveInit() function, having as parameters the address of the handler declared,
///                       I2C block, and the address of I2CSlaveAddrCfg variable
///
/// i2cReadAction() callback is called every time a RD_REQ interrupt is triggered, meaning
/// that the master has issued a read request and it is waiting for data. The function
/// can be used to prepare the data to be send to the master.
///
/// i2cWriteAction() callback is called when the slave has finished reading the information sent
/// by the master and also a stop bit has been received, signaling that the master has finished
/// the transfer. It provides the data that was received from master.
///

#ifndef _I2C_SLAVE_API_H_
#define _I2C_SLAVE_API_H_

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include "I2CSlaveApiDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global function prototypes
// ----------------------------------------------------------------------------
/// This will initialize the I2C Slave Component
/// @param[in] hndl   - pointer to the I2C slave handler
/// @param[in] config - pointer to I2CSlaveAddr configuration
/// @return	   s32	  - 0 - on success
///
s32 I2CSlaveInit(i2cSlaveHandle_t *hndl, i2cSlaveAddrCfg_t *config);

/// Set callback functions for the I2C component
/// @param[in] hndl          - pointer to I2C slave handler
/// @param[in] cbReadAction  - pointer to read function handler
/// @param[in] cbWriteAction - pointer to write function handler
/// @return    void
///
void I2CSlaveSetupCallbacks(i2cSlaveHandle_t *hndl, i2cReadAction* cbReadAction,
                            i2cWriteAction* cbWriteAction);
							
/// }@
#ifdef __cplusplus
}
#endif


#endif
