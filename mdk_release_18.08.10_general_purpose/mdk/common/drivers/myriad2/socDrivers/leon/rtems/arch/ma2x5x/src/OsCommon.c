///
/// @file OsCommon.c
/// 
/// 
/// @ingroup OsCommon
/// @{
///

#include <limits.h>
#include <rtems.h>
#include <OsCommon.h>
#include <stdio.h>
#include "OsCross.h"


rtems_status_code OsCommonEventReceive(rtems_event_set event_in, int waitforany, int wait, rtems_interval ticks, rtems_event_set *event_out, volatile u32 *shared, rtems_event_set driver_event)
{
	rtems_option option_set = 0;
	u32 timeout = UINT_MAX, tmp = 0, read = 0, maxticks = 0, satisfied = FALSE, ticksPassed = 0;

	if (waitforany)
		option_set = RTEMS_EVENT_ANY;
	else
		option_set = RTEMS_EVENT_ALL;

	// Work out maximum ticks
	if (wait) {
		if (ticks == RTEMS_NO_TIMEOUT)
			maxticks = UINT_MAX;
		else
			maxticks = rtems_clock_get_ticks_since_boot() + ticks;
	}

	// Build option_set
	if (wait == 0)
		option_set |= RTEMS_NO_WAIT;
	else
		option_set |= RTEMS_WAIT;

	// If no timeout is selected there is no need to wait for event
	if (wait == 0)
		read = (*shared & event_in);
	else { // Timeout was selected
		do {
			read = (*shared & event_in);
			if (((read & event_in) == event_in) || ((waitforany) && (read & event_in)))
				satisfied = TRUE;
			else {
				// Work out remaining ticks
				ticksPassed = rtems_clock_get_ticks_since_boot();
				if (ticksPassed < maxticks)
					rtems_event_receive(driver_event, option_set, maxticks - ticksPassed, &tmp);
				else
					timeout = 0;
			}
		}while ((!satisfied) && (timeout));
	}

	// Need to code return code
	if (satisfied)
		tmp = RTEMS_SUCCESSFUL;
	else {
		if ((timeout == 0) && (waitforany == 0))
			tmp = RTEMS_TIMEOUT;
		else
			tmp = RTEMS_UNSATISFIED;
	}

	// Clear already read flags only is successful or unsatisfied
	if ((read) && ((tmp == RTEMS_UNSATISFIED) || (tmp == RTEMS_SUCCESSFUL))){
		rtems_interrupt_disable(wait);
		*shared &= ~read;
		rtems_interrupt_enable(wait);
		*event_out = read;
	}
	return (rtems_status_code) tmp;
}

rtems_status_code OsDrvCommonInterruptSendEvent(rtems_id id, rtems_event_set event_in, volatile u32 *sharedInfo, rtems_event_set driver_event, int protect)
{
	rtems_status_code res;
	int status;
	if (protect) {
		rtems_interrupt_disable(status);
		*sharedInfo |= event_in;
		rtems_interrupt_enable(status);
	}
	else
		*sharedInfo |= event_in;
	// Release semaphore
	res = rtems_event_send(id, driver_event);
	return res;
}

void OsDrvCheckDynaInfrStatusCode(int code)
{
    switch(code)
    {
    case OS_MYR_DYN_INFR_SUCCESS:
        printf("OS Myriad dynamic loading infrastructure: operation successful. \n");
        break;
    case OS_MYR_DYN_INFR_ERROR:
        printf("OS Myriad dynamic loading infrastructure: operation error. \n");
        break;
    case OS_MYR_DYN_INFR_MULTIPLE_SHAVE_ALLOCATION:
        printf("OS Myriad dynamic loading infrastructure: operation error. Shave allocated to multiple applications. \n");
        break;
    case OS_MYR_DYN_INFR_INVALID_PARAMETERS:
        printf("OS Myriad dynamic loading infrastructure: operation error. Invalid function parameter(s). \n");
        break;
    case OS_MYR_DYN_INFR_CANNOT_ALLOC_HEAP:
        printf("OS Myriad dynamic loading infrastructure: operation error. Cannot allocate heap memory. \n");
        break;
    case OS_MYR_DYN_INFR_CANNOT_ALLOC_GROUPDATA:
        printf("OS Myriad dynamic loading infrastructure: operation error. Cannot allocate group data memory. \n");
        break;
    case OS_MYR_DYN_INFR_RUNALGO_SHAVES_BUSY:
        printf("OS Myriad dynamic loading infrastructure: operation error when running shave algo. Target shaves are busy or invalid function parameter.  \n");
        break;
    case OS_MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID:
        printf("OS Myriad dynamic loading infrastructure: operation error when running shave algo on assigned shave. Target shave busy or not assigned. \n");
        break;
    case OS_MYR_DYN_INFR_INVALID_SAVED_CONFIG:
        printf("OS Myriad dynamic loading infrastructure: operation error at cleanup. Invalid saved configuration. \n");
        break;
    case OS_MYR_DRV_RESOURCE_BUSY:
        printf("OS Myriad dynamic loading infrastructure: the shave cannot be accessed because it is used by another thread. \n");
        break;
    case OS_MYR_DRV_NOTOPENED:
        printf("OS Myriad dynamic loading infrastructure: driver still not opened. \n");
        break;
    case OS_MYR_DRV_TIMEOUT:
        printf("OS Myriad dynamic loading infrastructure: timeout while accessing the resource. \n");
        break;
    case OS_MYR_DRV_ERROR:
        printf("OS Myriad dynamic loading infrastructure: error in driver. \n");
        break;
    case OS_MYR_RESOURCE_NOT_OWNED:
        printf("OS Myriad dynamic loading infrastructure: trying to access a resource that is owned by other thread. \n");
        break;
    case OS_MYR_DRV_ALREADY_OPENED:
        printf("OS Myriad dynamic loading infrastructure: driver already opened. \n");
        break;
    case OS_MYR_DRV_ALREADY_INITIALIZED:
        printf("OS Myriad dynamic loading infrastructure: driver already initialized. \n");
        break;
    case OS_MYR_DYN_INFR_ERROR_STACK_OVERFLOW:
        printf("OS Myriad dynamic loading infrastructure: operation error when running shave algo. Stack overflow occured \n");
        break;
    case OS_MYR_DYN_INFR_NUMBER_OF_FREE_SHAVE_NOT_AVAILABLE:
        printf("OS Myriad dynamic loading infrastructure: free shaves number requested not available \n");
        break;
    case OS_MYR_DYN_INFR_SHAVE_NOT_FOUND_IN_CONTEXT_LIST:
        printf("OS Myriad dynamic loading infrastructure: shave not assigned to the Context structure \n");
        break;
    case OS_MYR_DYN_INFR_HEAP_NOT_SELECTED:
        printf("OS Myriad dynamic loading infrastructure: aplication does not require heap but heap size is set \n");
        break;
    case OS_MYR_DYN_INFR_HEAP_SIZE_NOT_SET:
        printf("OS Myriad dynamic loading infrastructure: application require heap but heap size is not set \n");
        break;
    case OS_MYR_DYN_INFR_FUNCTION_NOT_FOUND:
        printf("OS Myriad dynamic loading infrastructure: application entry could not be found into its own IAT \n");
        break;
    default:
        printf("OS Myriad dynamic loading infrastructure: unknown return code. Check return value description of causing function. \n");
        break;
    }
}

///@}
