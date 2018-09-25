///
/// @file OsCross.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsCross
/// @{
/// @brief     General OSAL Layer Implementation
///


#include <rtems.h>
#include <OsDrvI2cBus.h>
#include "swcWhoAmI.h"

//#define OS_CROSS_DEBUG

#ifdef OS_CROSS_DEBUG
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

#ifndef __RTEMS__
// Support for IO Manager 
static struct{
	uint32_t major;
	uint32_t currentMajor;
	const osCrossDriverAddressTable_t * driverTable[OSCROSSIOMANAGERTABLEMAXDRIVERS];	
} osCrossIOManagerInfo;

// Support for BSP CLOCK
osCrossBSPClockConfig_t osCrossBSPClockConfiguration = {12000, 0, 0, 0, 12000, 0, 0, 0, 0, 0};
// Clock Ticks
volatile u32 Clock_driver_ticks;

// Support LibI2C
static struct {		
	osCrossLibI2CBusEntry_t * buses[DRVCROSSLIBI2CMANAGERDEPTH];	
	osCrossLibI2CDrv_t * devices[DRVCROSSLIBI2CMANAGERDEPTH][DRVCROSSLIBI2CDEVICESDEPTH];	
	uint32_t busMajor[DRVCROSSLIBI2CMANAGERDEPTH];
	uint32_t i2cAddr[DRVCROSSLIBI2CMANAGERDEPTH][DRVCROSSLIBI2CDEVICESDEPTH];
} osCrossLibI2CManagerInfo;

// Default LibI2C Driver Table for the trick
static int osCrossLibI2CDefaultRead(uint32_t major, uint32_t minor, void *arg);
static int osCrossLibI2CDefaultWrite(uint32_t major, uint32_t minor, void *arg);

static const osCrossDriverAddressTable_t osCrossLibI2CDefaultDriverTable =
{
	.read_entry = (rtems_status_code (*)(uint32_t, uint32_t, void *)) osCrossLibI2CDefaultRead,
	.write_entry = (rtems_status_code (*)(uint32_t, uint32_t, void *)) osCrossLibI2CDefaultWrite,
};

/*
 * Private definitions
*/
// Private Register Bitfield descriptions
#define D_TIMER_GENCFG_FREE_CNT_EN         (1 << 0)
#define D_TIMER_GENCFG_PRESCALER_ENABLE    (1 << 2)

/*
* Initialization staff Has to be called after setting up clocks !!!
*/
int osCrossInit(void)
{
	uint32_t timerBaseAddress;
	if (swcWhoAmI() == PROCESS_LEON_OS)
        timerBaseAddress = TIM0_BASE_ADR;
    else
        timerBaseAddress = TIM1_BASE_ADR;        
	// Enable the free running clock, and the scaler
    SET_REG_BITS_MASK(timerBaseAddress + TIM_GEN_CONFIG_OFFSET ,   D_TIMER_GENCFG_FREE_CNT_EN | D_TIMER_GENCFG_PRESCALER_ENABLE); 
	SET_REG_WORD(timerBaseAddress + TIM_SCALER_RELOAD_OFFSET,0);
	// Enable Traps 
	swcLeonEnableTraps();
	// Set PIL 
	swcLeonSetPIL(0);
	return OS_MYR_DRV_SUCCESS;
}

/*
 * IO Manager 
*/

int osCrossLookupDriver(const osCrossDriverAddressTable_t * driver_table)
{
	int i = 0;
	do {
		if (osCrossIOManagerInfo.driverTable[i] == driver_table)
			break;
		i ++;
	} while(i < OSCROSSIOMANAGERTABLEMAXDRIVERS);
	return i;
}

int OsCrossIOUnRegisterDriver(uint32_t major)
{
	int res;
	if (osCrossIOManagerInfo.major > major) {
		osCrossIOManagerInfo.driverTable[major] = NULL;
		res = OS_MYR_DRV_SUCCESS;
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIORegisterDriver(uint32_t major, const osCrossDriverAddressTable_t * driver_table, uint32_t *registered_major)
{
    UNUSED(major); // hush the compiler warning.

	int res = OS_MYR_DRV_SUCCESS;
	if (osCrossIOManagerInfo.major < OSCROSSIOMANAGERTABLEMAXDRIVERS) {
		if (osCrossLookupDriver(driver_table) == OSCROSSIOMANAGERTABLEMAXDRIVERS) {
			osCrossIOManagerInfo.driverTable[osCrossIOManagerInfo.major] = driver_table;
			*registered_major = osCrossIOManagerInfo.major;			
			osCrossIOManagerInfo.major ++;	
			if (osCrossIOManagerInfo.driverTable[*registered_major]->initialization_entry != NULL)
				(*osCrossIOManagerInfo.driverTable[*registered_major]->initialization_entry)(*registered_major, 0, NULL);							
		}
		else
			res = RTEMS_RESOURCE_IN_USE;
	}
	else
		res = RTEMS_TOO_MANY;	
	return res;
}

int OsCrossIOOpenDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res = OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {
		if (osCrossIOManagerInfo.driverTable[major]->open_entry != NULL)
			res = (*osCrossIOManagerInfo.driverTable[major]->open_entry)(major, minor, arg);
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIOCloseDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res = OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {	
		if (osCrossIOManagerInfo.driverTable[major]->close_entry != NULL)
			res = (*osCrossIOManagerInfo.driverTable[major]->close_entry)(major, minor, arg);
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIOInitDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res = OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {	
		
		if (osCrossIOManagerInfo.driverTable[major]->initialization_entry != NULL) 
			res = (*osCrossIOManagerInfo.driverTable[major]->initialization_entry)(major, minor, arg);
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIOWriteDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res = OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {		
		if (osCrossIOManagerInfo.driverTable[major]->write_entry != NULL)
			res = (*osCrossIOManagerInfo.driverTable[major]->write_entry)(major, minor, arg);
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIOReadDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res = OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {			
		if (osCrossIOManagerInfo.driverTable[major]->read_entry != NULL)
			res = (*osCrossIOManagerInfo.driverTable[major]->read_entry)(major, minor, arg);
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

int OsCrossIOCtldDriver(uint32_t major, uint32_t minor, void *arg)
{
	int res =  OS_MYR_DRV_SUCCESS;
	osCrossIOManagerInfo.currentMajor = major;
	if ((osCrossIOManagerInfo.major > major) && osCrossIOManagerInfo.driverTable[major] != NULL) {	
		if (osCrossIOManagerInfo.driverTable[major]->control_entry != NULL)
			res = (*osCrossIOManagerInfo.driverTable[major]->control_entry)(major, minor, arg);
		
	}
	else
		res = RTEMS_INVALID_NUMBER;
	return res;
}

static u64 osCrossGetFreeRunningCounter(void)
{
    u32 timerBaseAddress, currentTicksH, currentTicksL;
    if (swcWhoAmI() == PROCESS_LEON_OS)
        timerBaseAddress = TIM0_BASE_ADR;
    else
        timerBaseAddress = TIM1_BASE_ADR; 
    
    #ifdef MA2X8X
    currentTicksL = GET_REG_WORD_VAL(timerBaseAddress + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
    currentTicksH = GET_REG_WORD_VAL(timerBaseAddress + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
    #else
    currentTicksH = GET_REG_WORD_VAL(timerBaseAddress + TIM_FREE_CNT1_OFFSET);  // On this read the low value is latched
    currentTicksL = GET_REG_WORD_VAL(timerBaseAddress + TIM_FREE_CNT0_OFFSET);  // previously latched low value read here
    #endif
    return (((u64)currentTicksH << 32) | ((u64)currentTicksL));
}

static void osCrossInitTimeStamp(tyTimeStamp * pTimeStamp, u32 intervalUs)
{
	*pTimeStamp =  osCrossGetFreeRunningCounter() + ((u64)intervalUs * (u64) osCrossBSPClockConfiguration.currentSysClockKhz);
}

static int osCrossCheckTimeStamp(tyTimeStamp * pTimeStamp)
{			
	if ( osCrossGetFreeRunningCounter() > (*pTimeStamp) )
        return 1;
    else
        return 0;
}

/*
 * rtems_event_receive substitution
*/
int OsCrossWaitFlags(uint32_t events, uint32_t mode, uint32_t timeout, uint32_t *eventsout, volatile uint32_t *shared)
{	
	tyTimeStamp pTimeStamp;
	u32 keepon = TRUE, expired = FALSE;
	u32 readevents = 0, interrupts = 0;
	int res;	
		
	// Init timestamp
	if (mode & RTEMS_NO_WAIT)
		osCrossInitTimeStamp(&pTimeStamp, 0);
	else
		osCrossInitTimeStamp(&pTimeStamp, timeout);
		
	do {
		readevents = *shared;
		if (mode & RTEMS_EVENT_ANY) {
			if (readevents & events)
				keepon = FALSE;
		}
		else {
			if ((readevents & events) == events)
				keepon = FALSE;
		}		
		expired = osCrossCheckTimeStamp(&pTimeStamp);			
	} while((keepon == TRUE) && (expired == FALSE));	
	// If any of the flags was set we need to clear it for next iteration
	if (readevents) {
		rtems_interrupt_disable(interrupts);		
		*shared &= ~readevents;	
		rtems_interrupt_enable(interrupts);
	}	
	// We have got what we expected 
	if (!keepon)
		res = OS_MYR_DRV_SUCCESS;
	else {
	// we haven't got what we expected 
		if (mode & RTEMS_NO_WAIT)
			res = RTEMS_UNSATISFIED;
		else
			res = RTEMS_TIMEOUT;		
	}	
	// Output caught events
	*eventsout = readevents;	
	return res;
}

/*
 * rtems_interrupt_disable, rtems_interrupt_enable substitution
*/

int OsCrossInterruptRegister(uint32_t irq, irq_handler isr)	
{
	DrvIcbIrqClear(irq);
	DrvIcbSetIrqHandler(irq, isr);
	DrvIcbEnableIrq(irq);
	return OS_MYR_DRV_SUCCESS;																							
}

int OsCrossInterruptUnregister(uint32_t irq)
{
	DrvIcbIrqClear(irq);
	return OS_MYR_DRV_SUCCESS;																							
}

/*
 * Debugging messages staff
*/

int OsCrossCheckStatusCode(uint32_t sc, char *msg)
{
    UNUSED(msg); // hush the compiler warning.

	if (sc != OS_MYR_DRV_SUCCESS)
	{
		DPRINTF1("ERROR: %d, %s\n", sc, msg);
	}
	else
	{
		DPRINTF1("OK: %d, %s\n", sc, msg);
	}
	return sc;
}

/*
 * Time staff
*/
int OsCrossGetTicksperSecond(void)
{	
	return (osCrossBSPClockConfiguration.currentSysClockKhz);
}

void OsCrossUSleep(u32 timeout)
{	
	tyTimeStamp pTimeStamp;
	osCrossInitTimeStamp(&pTimeStamp, timeout);	
	while(osCrossCheckTimeStamp(&pTimeStamp) == 0){}	
}

/*
 * LibI2C staff
*/

static int osCrossLibI2CSearchMajorInInternalTable(uint32_t major)
{
	int i;
	for (i = 0; i < DRVCROSSLIBI2CMANAGERDEPTH; i ++) {
		if (osCrossLibI2CManagerInfo.busMajor[i] == major)
			break;
	}
	return i;
}

static int osCrossLibI2CDefaultRead(uint32_t major, uint32_t minor, void *arg)
{
	int res = RTEMS_INVALID_NUMBER;
	int pos = osCrossLibI2CSearchMajorInInternalTable(major);
	if (pos != DRVCROSSLIBI2CMANAGERDEPTH) {
		if (osCrossLibI2CManagerInfo.devices[major][minor] != NULL) {
			if (osCrossLibI2CManagerInfo.devices[major][minor]->ops->read_entry != NULL) {
				res = (*osCrossLibI2CManagerInfo.devices[major][minor]->ops->read_entry)(major, minor, arg);
			} 
		}
	}
	return res;
}

static int osCrossLibI2CDefaultWrite(uint32_t major, uint32_t minor, void *arg)
{
	int res = RTEMS_INVALID_NUMBER;
	int pos = osCrossLibI2CSearchMajorInInternalTable(major);
	if (pos != DRVCROSSLIBI2CMANAGERDEPTH) {
		if (osCrossLibI2CManagerInfo.devices[major][minor] != NULL) {
			if (osCrossLibI2CManagerInfo.devices[major][minor]->ops->write_entry != NULL) {
				res = (*osCrossLibI2CManagerInfo.devices[major][minor]->ops->write_entry)(major, minor, arg);
			} 
		}
	}
	return res;
}

// Look for already registered bus
int osCrossLibI2CBusLookup(osCrossLibI2CBusEntry_t *bus)
{
	int i;
	for (i = 0; i < DRVCROSSLIBI2CMANAGERDEPTH; i ++) {
		if (osCrossLibI2CManagerInfo.buses[i] == bus)
			break;
	}
	return i;
}

int OsCrossLibI2CRegisterBus(char *name, osCrossLibI2CBusEntry_t *bus)
{
    UNUSED(name); // hush the compiler warning.

	uint32_t res = OSCROSSLIBI2CNOBUSAVAILABLE , i = 0, busn = 0; 
	if ((osCrossLibI2CBusLookup(bus) == DRVCROSSLIBI2CMANAGERDEPTH)) {
		do {
			if (osCrossLibI2CManagerInfo.buses[i] == NULL) {
				// Save function pointers
				osCrossLibI2CManagerInfo.buses[i] = bus;	
				// Call init function				
				if (bus->bus.ops->init != NULL)
                    (*bus->bus.ops->init)(&bus->bus);
				// Register driver with default table				
				if (OsCrossIORegisterDriver(i, &osCrossLibI2CDefaultDriverTable, &busn) == OS_MYR_DRV_SUCCESS) {
					res = busn;				
					osCrossLibI2CManagerInfo.busMajor[i] = busn;
				}
				break;
			}
			i ++;
		}while(i < DRVCROSSLIBI2CMANAGERDEPTH);		
	}
	return res;
}

int OsCrossLibI2CRegisterDevice(char *name, osCrossLibI2CDrv_t *desc, uint32_t bus, uint32_t addr)
{	
    UNUSED(name); // hush the compiler warning.

	int i = 0, res = OSCROSSLIBI2CNODEVICEAVAILABLE;
	if ((bus < DRVCROSSLIBI2CMANAGERDEPTH) && (osCrossLibI2CManagerInfo.buses[i] != NULL)) {
		do {
			if (osCrossLibI2CManagerInfo.devices[bus][i] == NULL) {
				// Save function pointers
				osCrossLibI2CManagerInfo.devices[bus][i] = desc;
				// Save addr 
				osCrossLibI2CManagerInfo.i2cAddr[bus][i] = addr;	
				break;
			}
			i ++;
		}while(i < DRVCROSSLIBI2CDEVICESDEPTH);
		if (i < DRVCROSSLIBI2CDEVICESDEPTH) {
			res = i;					
		}
	}	
	return res;
}
    
int OsCrossLibI2CSendStart(uint32_t minor)															
{	
    UNUSED(minor); // hush the compiler warning.

	return (osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus.ops->send_start)(&osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus);			
}

int OsCrossLibI2CSendStop(uint32_t minor)
{	
    UNUSED(minor); // hush the compiler warning.

	return (osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus.ops->send_stop)(&osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus);			
}

int OsCrossLibI2CSendAddr(uint32_t minor , uint32_t rw)
{	
    return (osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus.ops->send_addr)(&osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus, osCrossLibI2CManagerInfo.i2cAddr[osCrossIOManagerInfo.currentMajor][minor], rw);				
}

int OsCrossLibI2CReadBytes(uint32_t minor, u8 *buffer, uint32_t count)
{	
    UNUSED(minor); // hush the compiler warning.

	return (osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus.ops->read_bytes)(&osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus, buffer, count);			
}

int OsCrossLibI2CWriteBytes(uint32_t minor, u8 *buffer, uint32_t count)
{	
    UNUSED(minor); // hush the compiler warning.

    return (osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus.ops->write_bytes)(&osCrossLibI2CManagerInfo.buses[osCrossIOManagerInfo.currentMajor]->bus, buffer, count);				
}
#endif

///@}
