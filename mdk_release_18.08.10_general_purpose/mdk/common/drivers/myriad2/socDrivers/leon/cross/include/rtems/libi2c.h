///
/// @file libi2c.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup libi2c libi2c Driver
/// @{
/// @brief     Cross LIBI2C
///

#ifndef __LIBI2C_H__
#define __LIBI2C_H__

#include <mv_types.h>
#include <rtems/libio.h>

// LIBI2C return values
#define OSCROSSLIBI2CNOBUSAVAILABLE																	-1
#define OSCROSSLIBI2CNODEVICEAVAILABLE																-2

typedef struct osCrossLibI2CBus_t_
{
  const struct osCrossLibI2CBusOps_ *ops;
  int size;                     
} osCrossLibI2CBus_t;

typedef struct osCrossLibI2CBusOps_
{  
  uint32_t (*init) (osCrossLibI2CBus_t * bushdl);
  uint32_t (*send_start) (osCrossLibI2CBus_t * bushdl);  
  uint32_t (*send_stop) (osCrossLibI2CBus_t * bushdl);  
  uint32_t (*send_addr) (osCrossLibI2CBus_t * bushdl, uint32_t addr, int rw);  
  int (*read_bytes) (osCrossLibI2CBus_t * bushdl, unsigned char *bytes, int nbytes);  
  int (*write_bytes) (osCrossLibI2CBus_t * bushdl, unsigned char *bytes, int nbytes);  
  int (*ioctl) (osCrossLibI2CBus_t * bushdl, int   cmd, void *buffer);
} osCrossLibI2CBusOps_t;


typedef struct osCrossLibI2CDrv_t_
{
  const osCrossDriverAddressTable_t *ops;      /* the driver ops */
  int size;                     /* size of whole structure (including appended private data) */
} osCrossLibI2CDrv_t;
#endif
///@}
