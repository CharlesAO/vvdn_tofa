/*
 * This is a component to initialize memory structure for dynamic allocation
 *
 * @File      MyriadMemInit.c
 * @Brief     Movidius Myriad Memory Manager Initializer
 * @copyright All code copyright Movidius Ltd 2017, all rights reserved
 *            For License Warranty see: common/license.txt
 */

#include <stdio.h>
#include "registersMyriad.h"

#include "MyriadMemInit.h"

#ifdef MA2X8X
#include "OsDrvMutex.h"
#else
#include "DrvMutex.h"
#endif

#include "swcShaveLoader.h"
#include <assert.h>

#define MVLOG_UNIT_NAME MyriadMemInit
#include "mvLog.h"

static void* memorySections[][5] =
{
    { (void*)CMX_SLICE_0_BASE_ADR, __shave0_cmx_text_start, __shave0_cmx_text_end, __shave0_cmx_data_start, __shave0_cmx_data_end },
    { (void*)CMX_SLICE_1_BASE_ADR, __shave1_cmx_text_start, __shave1_cmx_text_end, __shave1_cmx_data_start, __shave1_cmx_data_end },
    { (void*)CMX_SLICE_2_BASE_ADR, __shave2_cmx_text_start, __shave2_cmx_text_end, __shave2_cmx_data_start, __shave2_cmx_data_end },
    { (void*)CMX_SLICE_3_BASE_ADR, __shave3_cmx_text_start, __shave3_cmx_text_end, __shave3_cmx_data_start, __shave3_cmx_data_end },
    { (void*)CMX_SLICE_4_BASE_ADR, __shave4_cmx_text_start, __shave4_cmx_text_end, __shave4_cmx_data_start, __shave4_cmx_data_end },
    { (void*)CMX_SLICE_5_BASE_ADR, __shave5_cmx_text_start, __shave5_cmx_text_end, __shave5_cmx_data_start, __shave5_cmx_data_end },
    { (void*)CMX_SLICE_6_BASE_ADR, __shave6_cmx_text_start, __shave6_cmx_text_end, __shave6_cmx_data_start, __shave6_cmx_data_end },
    { (void*)CMX_SLICE_7_BASE_ADR, __shave7_cmx_text_start, __shave7_cmx_text_end, __shave7_cmx_data_start, __shave7_cmx_data_end },
    { (void*)CMX_SLICE_8_BASE_ADR, __shave8_cmx_text_start, __shave8_cmx_text_end, __shave8_cmx_data_start, __shave8_cmx_data_end },
    { (void*)CMX_SLICE_9_BASE_ADR, __shave9_cmx_text_start, __shave9_cmx_text_end, __shave9_cmx_data_start, __shave9_cmx_data_end },
    { (void*)CMX_SLICE_10_BASE_ADR, __shave10_cmx_text_start, __shave10_cmx_text_end, __shave10_cmx_data_start, __shave10_cmx_data_end },
    { (void*)CMX_SLICE_11_BASE_ADR, __shave11_cmx_text_start, __shave11_cmx_text_end, __shave11_cmx_data_start, __shave11_cmx_data_end },
#ifdef MA2X8X
    { (void*)CMX_SLICE_12_BASE_ADR, __shave12_cmx_text_start, __shave12_cmx_text_end, __shave12_cmx_data_start, __shave12_cmx_data_end },
    { (void*)CMX_SLICE_13_BASE_ADR, __shave13_cmx_text_start, __shave13_cmx_text_end, __shave13_cmx_data_start, __shave13_cmx_data_end },
    { (void*)CMX_SLICE_14_BASE_ADR, __shave14_cmx_text_start, __shave14_cmx_text_end, __shave14_cmx_data_start, __shave14_cmx_data_end },
    { (void*)CMX_SLICE_15_BASE_ADR, __shave15_cmx_text_start, __shave15_cmx_text_end, __shave15_cmx_data_start, __shave15_cmx_data_end },
#endif /* MA2x8x */
    {__ddrSpaceStart, __ddrSpaceStart, __ddrSpaceEnd, __ddrSpaceEnd, __ddrSpaceEnd},    /*DDR*/
};

#ifdef MEM_MGR_DDR_SIZE
// DDR_MGR_SECTION uint8_t __MemMgrReservedSpace[MEM_MGR_DDR_SIZE];    /*allocate memory for DDR management*/
#endif /* DDR_MGR_SECTION */

/*******MEMORY STRUCTURE*******/
__attribute__((section(".ddr_direct.data"))) MemManag __MemManager;
/******************************/

////////////////////////////////////////////////////////////////////////////
//////////////////////////////////INTERNAL//////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static void addAllocator(MemArea *area, unsigned int allocIndex,
    void* address, size_t size, allocState state)
{
    area->allocators[allocIndex].address   = address;
    area->allocators[allocIndex].size      = size;
    area->allocators[allocIndex].state     = state;

    if (state == USED)
        area->usage += size;

    area->nmbOfAllocs++;
}

static size_t getMemAreaSize(int sliceIndex)
{
    int32_t FreeMemBefText, FreeMemAfterText, FreeMemAfterData, retVal;

    if (sliceIndex == DDR_AREA){
        /*DDR area size requested*/
        assert(memorySections[sliceIndex][0] <= memorySections[sliceIndex][4]);
        retVal = memorySections[sliceIndex][4] - memorySections[sliceIndex][0];
        goto exit;
    }

    if( (((uint32_t)(memorySections[sliceIndex][3]) & 0xF0000000) == 0x70000000)
        && (((uint32_t)(memorySections[sliceIndex][1]) & 0xF0000000) == 0x70000000) )
    {   // if cmx.text.start is in CMX and cmx.data.start is in CMX
        FreeMemBefText   = (int32_t)(memorySections[sliceIndex][1] - memorySections[sliceIndex][0]);
        FreeMemAfterText = (int32_t)(memorySections[sliceIndex][3] - memorySections[sliceIndex][2]);
        FreeMemAfterData = (int32_t)(((int32_t)&memorySections[sliceIndex][0] + CMX_SLICE_SIZE) - (int32_t)&memorySections[sliceIndex][4]);
        FreeMemAfterData -= swcGetShaveStackSize(sliceIndex); // remove stored stack size for the shave appl.

        retVal = FreeMemAfterData;
        goto exit;
    }
    else
    {
        if( (((uint32_t)(memorySections[sliceIndex][3]) & 0xF0000000) == 0x70000000)
            && (((uint32_t)(memorySections[sliceIndex][1]) & 0xF0000000) != 0x70000000) )
        {  // if cmx.text.start is NOT in CMX and cmx.data.start is in CMX
            FreeMemBefText = (int32_t)(memorySections[sliceIndex][3] - memorySections[sliceIndex][0]);
            FreeMemAfterText = 0;
            FreeMemAfterData = (int32_t)(((int32_t)&memorySections[sliceIndex][0] + CMX_SLICE_SIZE) - (int32_t)&memorySections[sliceIndex][4]);
            FreeMemAfterData -= swcGetShaveStackSize(sliceIndex); // remove stored stack size for the shave appl.

            retVal = FreeMemBefText + FreeMemAfterData;
            goto exit;
        }
        else
        {
            if( (((uint32_t)(memorySections[sliceIndex][3]) & 0xF0000000) != 0x70000000)
                && (((uint32_t)(memorySections[sliceIndex][1]) & 0xF0000000) == 0x70000000) )
            { // if cmx.text.start is in CMX and cmx.data.start is NOT in CMX
                FreeMemBefText   = (int32_t)(memorySections[sliceIndex][1] - memorySections[sliceIndex][0]);
                FreeMemAfterText = (int32_t)(((int32_t)&memorySections[sliceIndex][0] + CMX_SLICE_SIZE) - (int32_t)&memorySections[sliceIndex][2]);
                FreeMemAfterText -= swcGetShaveStackSize(sliceIndex); // remove stored stack size for the shave appl.
                FreeMemAfterData = 0;

                retVal = FreeMemAfterText;
                goto exit;
            }
            else
            {  // text and data sections are not in CMX, CMX is free, only used by stack.
                FreeMemBefText = 0;
                FreeMemAfterText = 0;
                FreeMemAfterData = CMX_SLICE_SIZE - swcGetShaveStackSize(sliceIndex); // remove stored stack size for the shave appl.

                retVal = FreeMemAfterData;
                goto exit;
            }
        }
    }
    exit:
        return retVal;
}

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////EXTERNAL///////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void MemMgrInitialize(void)
{
    void *areaBase, *areaTextStart, *areaTextEnd, *areaDataStart, *areaDataEnd;
    MemArea *localArea;

    if (__MemManager.isInitialized)
    {
        /*Initialization function was already called*/
        goto exit;
    }

#ifdef MA2X8X
    // mutex handler
    OsDrvMutexHandler hndl;
    // driver interface status code
    rtems_status_code sc;

    // initialize mutex driver
    OsDrvMutexInit(OS_DRV_MUTEX_DEFAULT_INT_LEVEL);

    // opening a mutex will return a handle used for further operations
    sc = OsDrvMutexOpen(&hndl, MEM_MGR_MUTEX);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexOpen() failed" );

    // obtain the mutex immediately or block until free
    sc = OsDrvMutexObtain(&hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexObtain() failed" );
#else
    DrvMutexLock(MEM_MGR_MUTEX);
#endif

    mvLog(MVLOG_DEBUG, "Initialized flag is: %d!", __MemManager.isInitialized);

    __MemManager.version = 0;
    // Init CMX memory areas
    for(MemMgrAreas area = CMX_AREA0; area <= CMX_TOTAL_COUNT; area++)
    {
        localArea = &__MemManager.areas[area];

        areaBase        = memorySections[area][0];
        areaTextStart   = memorySections[area][1];
        areaTextEnd     = memorySections[area][2];
        areaDataStart   = memorySections[area][3];
        areaDataEnd     = memorySections[area][4];

        localArea->address      = areaBase;
        localArea->allocRate    = DEFAULT_ALLOC_RATE;
        localArea->size         = getMemAreaSize(area);
        localArea->usage        = 0;
        localArea->nmbOfAllocs  = 0;

        if(area == DDR_AREA)
        {
            addAllocator(localArea,
                        localArea->nmbOfAllocs,
                        areaBase,
                        areaDataStart - areaBase,
                        FREE);
        }
        else if ( (CMX_SLICE_SIZE >= (int32_t)(areaTextStart - areaBase)) &&
            ((int32_t)(areaTextStart - areaBase) >= 0) )
        {
            /*TEXT-SECTION STATICALY ALLOCATED IN CMX SLICE*/
            if (areaTextEnd - areaTextStart)
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            areaTextStart,
                            areaTextEnd - areaTextStart,
                            STATIC);

            if ( (CMX_SLICE_SIZE >= (int32_t)(areaDataStart - areaBase)) &&
                ((int32_t)(areaDataStart - areaBase) >= 0) )
            {
                /*DATA-SECTION STATICALY ALLOCATED IN CMX SLICE*/
                // Add free space between cmx.text and cmx.data if size > 0
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            areaTextEnd,
                            areaDataStart - areaTextEnd,
                            FREE);

                if(CMX_SLICE_SIZE >= (areaDataEnd - areaDataStart))
                {
                    /*DATA-SECTION FITS IN CMX SLICE*/
                    if (areaDataEnd - areaDataStart)
                        addAllocator(localArea,
                                    localArea->nmbOfAllocs,
                                    areaDataStart,
                                    areaDataEnd - areaDataStart,
                                    STATIC);
                    // Assert execution if stack space overwrites data section
                    assert( (((int32_t)(areaBase + CMX_SLICE_SIZE) - (int32_t)areaDataEnd - (int32_t)swcGetShaveStackSize(area))>=0)
                        && "ERROR: No more space left for stack, check stack usage and adjust reserved stack !!!" );

                    // Add free space after cmx.data section if it's size is grater then 0
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                areaDataEnd,
                                (areaBase + CMX_SLICE_SIZE) - areaDataEnd - swcGetShaveStackSize(area),
                                FREE);
                    // Add used stack space
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                (areaBase + CMX_SLICE_SIZE) - swcGetShaveStackSize(area),
                                swcGetShaveStackSize(area),
                                STACK);

                }
                else
                {
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                areaDataStart,
                                (areaBase + CMX_SLICE_SIZE) - areaDataStart,
                                STATIC);
                }
            }
            else
            {
                /*DATA-SECTION STATICALY ALLOCATED OUTSIDE CMX SLICE*/
                // Add free space between cmx.text and slice end - stack if size > 0
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            areaTextEnd,
                            (areaBase + CMX_SLICE_SIZE) - swcGetShaveStackSize(area) - areaTextEnd,
                            FREE);
                // Add used stack space
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            (areaBase + CMX_SLICE_SIZE) - swcGetShaveStackSize(area),
                            swcGetShaveStackSize(area),
                            STACK);
            }
        }
        else
        {
            /*TEXT-SECTION STATICALY ALLOCATED OUTSIDE CMX SLICE*/
            if ( (CMX_SLICE_SIZE >= (int32_t)(areaDataStart - areaBase)) &&
                ((int32_t)(areaDataStart - areaBase) >= 0) )
            {
                /*DATA-SECTION STATICALY ALLOCATED IN CMX SLICE*/
                // Add free space between SLICE start and cmx.data if size > 0
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            areaBase,
                            areaDataStart - areaBase,
                            FREE);
                if(CMX_SLICE_SIZE >= (areaDataEnd - areaDataStart))
                {
                    /*DATA-SECTION FITS IN CMX SLICE*/
                    // Add static cmx.data section if size is > 0
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                areaDataStart,
                                areaDataEnd - areaDataStart,
                                STATIC);
                    // Assert execution if stack space overwrites data section
                    assert( (((int32_t)(areaBase + CMX_SLICE_SIZE) - (int32_t)areaDataEnd - (int32_t)swcGetShaveStackSize(area))>=0)
                            && "ERROR: No more space left for stack, check stack usage and adjust reserved stack !!!" );

                    // Add free space after cmx.data section if it's size is grater then 0
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                areaDataEnd,
                                (areaBase + CMX_SLICE_SIZE) - areaDataEnd - swcGetShaveStackSize(area),
                                FREE);
                }
                else
                {
                    addAllocator(localArea,
                                localArea->nmbOfAllocs,
                                areaDataStart,
                                (areaBase + CMX_SLICE_SIZE) - areaDataStart,
                                STATIC);
                }

            }
            else
            {
                /*DATA-SECTION STATICALY ALLOCATED OUTSIDE CMX SLICE*/
                // Add free space between cmx.text and slice end - stack if size > 0
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            areaBase,
                            CMX_SLICE_SIZE - swcGetShaveStackSize(area),
                            FREE);
                // Add used stack space
                addAllocator(localArea,
                            localArea->nmbOfAllocs,
                            (areaBase + CMX_SLICE_SIZE) - swcGetShaveStackSize(area),
                            swcGetShaveStackSize(area),
                            STACK);
            }
        }

        mvLog(MVLOG_DEBUG, "MemArea-%d address (%p) size (%d) usage (%d)!",
               area,
               localArea->address,
               localArea->size,
               localArea->usage);
    }

    __MemManager.isInitialized = INITIALIZED;

#ifdef MA2X8X
    // make the mutex available for others to use
    sc = OsDrvMutexRelease(&hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexRelease() failed" );

    // release the resources associated with the mutex
    sc = OsDrvMutexClose(&hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexClose() failed" );
#else
    DrvMutexUnlock(MEM_MGR_MUTEX);
#endif

    exit:
        return;
}
