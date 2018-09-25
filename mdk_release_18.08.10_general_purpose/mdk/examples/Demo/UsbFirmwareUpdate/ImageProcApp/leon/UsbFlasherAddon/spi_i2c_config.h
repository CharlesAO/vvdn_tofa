///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///
///
///
///
///

#ifndef _SPI_I2C_CONFIG_H
#define _SPI_I2C_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------

// Include board specific config functions
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise
int i_myr2_register_libi2c_spi_bus(void);

#endif // _SPI_I2C_CONFIG_H

/* End of File */