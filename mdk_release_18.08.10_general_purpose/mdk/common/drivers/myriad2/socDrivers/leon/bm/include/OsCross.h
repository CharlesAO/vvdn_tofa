///
/// @file OsCross.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsCross OsCross Driver
/// @ingroup OsCross
/// @{
/// @brief     General OSAL Layer Implementation
///


#include <OsCommon.h>
#include <OsDrvI2cBus.h>
#include "swcWhoAmI.h"
#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RTEMS__
/*
* Initialization staff Has to be called after setting up clocks !!!
*/
int osCrossInit(void);
/*
 * IO Manager
*/

int osCrossLookupDriver(osCrossDriverAddressTable_t * driver_table);
int OsCrossIOUnRegisterDriver(int major);
int OsCrossIORegisterDriver(uint32_t major, const osCrossDriverAddressTable_t * driver_table, uint32_t *registered_major);
int OsCrossIOOpenDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossIOCloseDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossIOInitDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossIOWriteDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossIOReadDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossIOCtldDriver(uint32_t major, uint32_t minor, void *arg);
int OsCrossWaitFlags(uint32_t events, uint32_t mode, uint32_t timeout, uint32_t *eventsout, volatile uint32_t *shared);
int OsCrossInterruptRegister(uint32_t irq, irq_handler isr);
int OsCrossInterruptUnregister(uint32_t irq);
int OsCrossCheckStatusCode(uint32_t sc, char *msg);
int OsCrossGetTicksperSecond(void);
void OsCrossUSleep(u32 timeout);

// Look for already registered bus
int osCrossLibI2CBusLookup(osCrossLibI2CBusEntry_t *bus);
int OsCrossLibI2CRegisterDevice(char *name, osCrossLibI2CDrv_t *desc, uint32_t bus, uint32_t addr);
int OsCrossLibI2CSendStart(uint32_t minor);
int OsCrossLibI2CSendStop(uint32_t minor);
int OsCrossLibI2CSendAddr(uint32_t minor , uint32_t rw);
int OsCrossLibI2CReadBytes(uint32_t minor, u8 *buffer, uint32_t count);
int OsCrossLibI2CWriteBytes(uint32_t minor, u8 *buffer, uint32_t count);

#ifdef __cplusplus
}
#endif

#endif

///@}
