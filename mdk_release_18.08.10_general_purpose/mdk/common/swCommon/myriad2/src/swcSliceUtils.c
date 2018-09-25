///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API manipulating Slice functionalities
///
///

#include "swcSliceUtils.h"
#include <mv_types.h>
#include <registersMyriad.h>
#include <DrvRegUtilsDefines.h>
#include <DrvMutex.h>



/// Unlocks a specified mutex
/// @param  mutex             mutex number: [0,31]
/// @return void
///
void swcSliceReleaseMutex(unsigned int mutexNo)
{
    // release access to mutex
    DrvMutexUnlock(mutexNo);
    return;
}


/// Requests a specified mutex
/// @param  mutexNo      : mutex number: [0,31]
/// @param  autoRetry    If the mutex requested is available, it will be taken , otherwise
///                      autoRetry=2: :the application  will be blocked until the mutex will be taken
///                      autoRetry=1: only the request will be locked , the user may come later to check if the mutex has been taken
///						 autoRetry=0: it will exit the function
/// @return: 1: if the mutex has been taken, 0 otherwise
///
int swcSliceRequestMutex(unsigned int mutexNo,  int requestOption)
{
	int x = 0;

	switch(requestOption)
	{
	case 2:
			DrvMutexLock(mutexNo);
			return 1;

	case 1:
			//Request a hardware mutex with auto repetition.
			x = 1- DrvMutexLockRequest(mutexNo);
			break;

	case 0:
			//Request a hardware mutex without auto repetition.
			x =  DrvMutexTryLock(mutexNo);
			break;

	default:
			break;
	}

	return x;
}

/// Checks if a mutex is free
/// @param  mutexNo      : mutex number: [0,31]
/// @return 1 if  the mutex is free and  0 if it is in use
///
int swcSliceIsMutexFree(unsigned int mutexNo)
{
    unsigned int index;
    // request access to mutex
    index = DrvMutexGetStatus(mutexNo);
    if ((index & 0x100) == 0x0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/// Sets interrupt - Not use it if there are other IRQ_CMX interrupts that need be set
/// For complex projects, please define in you project handler icb operations
/// @param   mutexHandler - pointer to the handler function
/// @param   intMask      - mask  that indicates which mutex will generate the  interrupt
/// @return  void
///
void swcSetMutexInterrupt(irq_handler mutexHandler, int intMask)
{
    DrvMutexSetInterrupts(mutexHandler, intMask);
}
