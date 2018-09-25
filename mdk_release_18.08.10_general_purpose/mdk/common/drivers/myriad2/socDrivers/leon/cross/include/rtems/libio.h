///
/// @file libio.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup libio libio Driver
/// @{
/// @brief     Cross IO
///

#ifndef __LIBIO_H__
#define __LIBIO_H__

/*
 * IO Manager implementation
*/

typedef uint32_t (*osCrossDeviceDriverEntry)(uint32_t, uint32_t, void *);

typedef struct 
{
	osCrossDeviceDriverEntry initialization_entry;
	osCrossDeviceDriverEntry open_entry;
	osCrossDeviceDriverEntry close_entry;
	osCrossDeviceDriverEntry read_entry;
	osCrossDeviceDriverEntry write_entry;
	osCrossDeviceDriverEntry control_entry;	
} osCrossDriverAddressTable_t;

typedef struct osCrossLibIORWArgs_t_{
	char                   *buffer;
	uint32_t                count;
	uint32_t                bytes_moved;
}osCrossLibIORWArgs_t;

#endif
///@}
