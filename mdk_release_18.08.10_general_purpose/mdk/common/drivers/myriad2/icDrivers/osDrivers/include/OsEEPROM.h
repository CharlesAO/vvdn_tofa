///   
/// @file                             
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     MV0182 R4 =< EEPROM driver
/// 
/// 
///
#ifndef _OSEEPROM_H
#define _OSEEPROM_H

#include <rtems.h>
#include <rtems/libi2c.h>
#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    s32 address_length; // Length of the address
    u32 eeprom_size; // Size of the memory in bytes 
} osEEPROMDev_t;

// Driver table to be exported
extern const rtems_libi2c_drv_t eepromDrvTbl;

#ifdef __cplusplus
}
#endif

#endif
