///
/// @file OsDrvShaveL2Cache.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvShaveL2Cache
/// @{
/// @brief     Shave low level functionality for RTEMS.
///



/// 1: Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <bsp.h>
#include "OsDrvShaveL2Cache.h"
#include "dbgTracerApi.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Lock/unlock utilities
#define OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK()           lockCriticalSection(osDrvShaveL2cacheControl.controlMutex, true, RTEMS_NO_TIMEOUT)
#define OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK()         unlockCriticalSection(osDrvShaveL2cacheControl.controlMutex)

//#define OS_DRV_SHAVE_L2CACHE_L1
//#define OS_DRV_SHAVE_L2CACHE_L2

#ifdef OS_DRV_SHAVE_L2CACHE_L1
    #define OS_DPRINTF1(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF1(...)
#endif

#ifdef OS_DRV_SHAVE_L2CACHE_L2
    #define OS_DPRINTF2(...)            printf(__VA_ARGS__)
#else
    #define OS_DPRINTF2(...)
#endif


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static struct {
    OS_MYRIAD_DRIVER_STATUS rtStatus;
    u32 allocated;
    s32 partitionCount;
    rtems_id controlMutex;
}osDrvShaveL2cacheControl;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/* Enter Critical Section */
static rtems_status_code lockCriticalSection(rtems_id sem, u32 wait, u32 wait_ticks)
{
    rtems_status_code rtStatus;

    if (wait)
        rtStatus = rtems_semaphore_obtain(sem, RTEMS_WAIT, wait_ticks);
    else
        rtStatus = rtems_semaphore_obtain(sem, RTEMS_NO_WAIT, 0);

    OS_DPRINTF2("lockCriticalSection(): rtStatus = %d, wait = %d\n", rtStatus, wait);
    return rtStatus;
}

/* Abandon Critical Section */
static rtems_status_code unlockCriticalSection(rtems_id sem)
{
    rtems_status_code rtStatus = rtems_semaphore_release(sem);

    OS_DPRINTF2("unlockCriticalSection(): rtStatus = %d\n", rtStatus);
    return rtStatus;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CacheInit(osDrvShaveL2CacheModes_t l2cMode)
{
    rtems_status_code rtStatus;

    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_UNINITIALIZED)
    {
        osDrvShaveL2cacheControl.rtStatus = OS_DRV_STAT_OPENED; //fast reservation, before actually opening; will be undone if something goes wrong below; still vulnerable up to here

        rtStatus = rtems_semaphore_create(rtems_build_name('S', 'L', '2', 'C'), 1,
                RTEMS_BINARY_SEMAPHORE,0,(rtems_id *) &osDrvShaveL2cacheControl.controlMutex);
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            DrvShaveL2CacheClearPartitions();
            osDrvShaveL2cacheControl.allocated = FALSE;
            osDrvShaveL2cacheControl.partitionCount = 0;
            DrvShaveL2CacheSetMode(l2cMode);  //TODO test the return code? the errors are asserted in BM driver
            osDrvReturnResult = OS_MYR_DRV_SUCCESS;
        }
        else
        {
            osDrvShaveL2cacheControl.rtStatus = OS_DRV_STAT_UNINITIALIZED; //revert
            osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
        }
    }
    else
        osDrvReturnResult = OS_MYR_DRV_ALREADY_INITIALIZED;

    OS_DPRINTF1("OsDrvShaveL2CacheInit() result = %d\n", osDrvReturnResult);
    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CResetPartitions(void)
{

    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED) {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL) {
            DrvShaveL2CacheClearPartitions();
            // Release allocation so that we can setup and allocate again
            osDrvShaveL2cacheControl.allocated = FALSE;
            osDrvShaveL2cacheControl.partitionCount = 0;
            OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }
        osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }

    OS_DPRINTF1("OsDrvShaveL2CResetPartitions() %d\n",osDrvReturnResult);

    return osDrvReturnResult;
}

OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CSetNonWindowedPartition(shaveId_t shaveNum,u32 partNum, osDrvShaveL2CachePartitionType_t partitionType)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            switch(partitionType)
            {
                case (NON_WINDOWED_DATA_PARTITION):
                    DrvShaveL2CacheSetLSUPartId(shaveNum, partNum);
                    break;
                case (NON_WINDOWED_INSTRUCTIONS_PARTITION):
                    DrvShaveL2CacheSetInstrPartId(shaveNum, partNum);
                    break;
                default:
                    osDrvReturnResult = OS_MYR_DRV_ERROR;
            }

            rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }

        if ( osDrvReturnResult != OS_MYR_DRV_ERROR )
            osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CSetNonWindowedPartition(shave %d, partNum %d, partType %d) result = %d\n", shaveNum, partNum, partitionType, osDrvReturnResult);

    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CSetWindowPartition(shaveId_t shaveNum, drvShaveL2CacheWindowType_t winId, u32 partNum)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            DrvShaveL2CacheSetWinPartId(shaveNum, winId, partNum);

            rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }

        osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CSetWindowPartition(shave %d, winId %d, partNum %d) result = %d\n", shaveNum, winId, partNum, osDrvReturnResult);

    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CachePartitionFlush(u32 partNumber, osDrvShaveL2CacheInvalidate_t invalidate)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            if ( invalidate == DO_NOT_PERFORM_INVALIDATION )
                DrvShaveL2CachePartitionFlush(partNumber);
            else  //PERFORM_INVALIDATION
                DrvShaveL2CachePartitionFlushAndInvalidate(partNumber);

            rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }

        osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CachePartitionFlush(partNumber %d) result = %d\n", partNumber, osDrvReturnResult);

    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CachePartitionInvalidate(u32 partNumber)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            DrvShaveL2CachePartitionInvalidate(partNumber);

            rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }

        osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CachePartitionInvalidate(partNumber %d) result = %d\n", partNumber, osDrvReturnResult);

    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE OsDrvShaveL2CGetPartition(osDrvShaveL2CachePartitionSizes_enum_t partitionSize, int* partitionId)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL) {
            if (!osDrvShaveL2cacheControl.allocated)
            {
                if (osDrvShaveL2cacheControl.partitionCount < (MAX_SHAVE_L2C_PARTITIONS-1))
                {
                    osDrvShaveL2cacheControl.partitionCount = DrvShaveL2CacheSetupPartition(partitionSize);
                    if (partitionId != NULL)
                    	*partitionId = osDrvShaveL2cacheControl.partitionCount;
                }
                else
                {
                    osDrvReturnResult = OS_MYR_DRV_ERROR;
                    if (partitionId != NULL)
                    	*partitionId = MAX_SHAVE_L2C_PARTITIONS;
                }
            }
            else
                osDrvReturnResult = OS_MYR_DRV_ERROR;
            OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }
        else
            osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CacheSetupPartition(partSize %d) usedPartitions = %d, result = %d\n", partitionSize, *nbOfUsedPartitions, osDrvReturnResult);

    return osDrvReturnResult;
}


OS_MYRIAD_DRIVER_STATUS_CODE  OsDrvShaveL2CacheAllocateSetPartitions(void)
{
    rtems_status_code rtStatus;
    OS_MYRIAD_DRIVER_STATUS_CODE osDrvReturnResult = OS_MYR_DRV_SUCCESS;
    u32 bmResult;

    if (osDrvShaveL2cacheControl.rtStatus == OS_DRV_STAT_OPENED)
    {
        rtStatus = OS_DRV_SHAVE_L2CACHE_MUTEX_LOCK();
        if (rtStatus == RTEMS_SUCCESSFUL)
        {
            if (!osDrvShaveL2cacheControl.allocated) {
                bmResult = DrvShaveL2CacheAllocateSetPartitions();
                // Verify bm result
                if (bmResult != 0)
                    osDrvReturnResult = OS_MYR_DRV_ERROR;
                else // Mark partitions as allocated
                    osDrvShaveL2cacheControl.allocated = TRUE;
            }
            else 
                osDrvReturnResult = OS_MYR_DRV_ERROR;
            // Release semaphore
            OS_DRV_SHAVE_L2CACHE_MUTEX_UNLOCK();
        }
        else
            osDrvReturnResult = (OS_MYRIAD_DRIVER_STATUS_CODE) rtStatus;
    }
    else
        osDrvReturnResult = OS_MYR_DRV_NOTOPENED;

    OS_DPRINTF1("OsDrvShaveL2CacheAllocateSetPartitions() result = %d\n", osDrvReturnResult);

    return osDrvReturnResult;
}


//OS_MYRIAD_DRIVER_STATUS_CODE  OsDrvShaveL2CacheSetMode(void)  //not implemented, for now


///@}

