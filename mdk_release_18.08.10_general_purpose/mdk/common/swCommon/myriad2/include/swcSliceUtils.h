///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup SliceUtils Slice Utils
/// @{
/// @brief     Slice Utilities API 
///
/// Used for manipulating slice functionalities
///

#ifndef SWCSLICEUTILS_H_
#define SWCSLICEUTILS_H_

#include <swcDmaTypes.h>
#include <mv_types.h>
#include <DrvIcb.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Function that releases a certain hardware mutex
/// @param[in] mutexNo mutex to release
/// @return  void
///
void swcSliceReleaseMutex(unsigned int mutexNo);

/// @brief Function that requests a certain hardware mutex
/// @param  mutexNo    - mutex number: [0,31]
/// @param  autoRetry  - If the mutex requested is available, it will be taken , otherwise:
///                      - autoRetry=2: :the application  will be blocked until the mutex will be taken
///                      - autoRetry=1: only the request will be locked , the user may come later to check if the mutex has been taken
///						 - autoRetry=0: it will exit the function
/// @return 1 if the mutex has been taken, 0 otherwise
///
int swcSliceRequestMutex(unsigned int mutexNo,  int requestOption);

/// @brief Function that requests a certain hardware mutex
/// @param[in] mutexHandler handler function
/// @param[in] intMask mask to mutex's used
/// @return void
///
void swcSetMutexInterrupt(irq_handler mutexHandler, int intMask);

/// @brief Checks if a mutex is free
/// @param  mutexNo - mutex number: [0,31]
/// @return 1 if the mutex is free and  0 if it is in use
///
int swcSliceIsMutexFree(unsigned int mutexNo);
/// @}
#ifdef __cplusplus
}
#endif

#endif /* SWCSLICEUTILS_H_ */
