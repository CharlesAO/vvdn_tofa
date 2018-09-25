///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippTestCommon.h>

#ifdef SIPP_TEST_APP

#if defined(__sparc) && defined(MYRIAD2)

#include <registersMyriad.h>
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>
#include <swcLeonUtils.h>

#ifdef SIPP_SCRT_ENABLE_OPIPE
#include <DrvCmxDma.h>
#ifdef USE_CMX_DMA_NEW_DRIVER
#include <DrvCdma.h>
#endif
#endif

#elif defined(SIPP_PC)
#include <stdint.h>
u8 * mbinImgSipp = 0; // dummy (unused on PC)
#endif

#endif // SIPP_TEST_APP

//##########################################################################
// Myriad initialization function
// Undocumented - for internal test cases only!
void sippPlatformInit ()
{

    #ifdef SIPP_TEST_APP
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    #if SIPP_RTOS == SIPP_NO_RTOS
    DrvCprStartAllClocks ();
    DrvCprInit ();
    DrvTimerInit ();
    unitTestInit ();
    DrvLL2CInitWriteThrough ();
    // Need this with current heap init code which will attempt to init the DDR heap on setup
    DrvDdrInitialise (NULL);

    // Initialise the cmx dma driver
    #ifdef USE_CMX_DMA_NEW_DRIVER
    DrvCmxDmaInitialize ((DrvCmxDmaSetupStruct *)NULL);
    #else
    DrvCmxDmaInitDefault ();
    #endif

    #endif
    #endif
    #endif  // SIPP_TEST_APP

    // Bypass CMXDMA SW reset
    sippSetInitMask (0x0);

    if (sippInitialize () == FALSE)
    {
        // Hard core kill as this function is for test prototyping only
        exit (0x0);
    }
}


//##########################################################################
// Myriad initialization function
// Undocumented - for internal test cases only!
void sippPlatformInitAsync (void)
{
    #ifndef SIPP_PC
    #ifndef __RTEMS__
    // Enable interrupts
    swcLeonSetPIL (0);
    #endif
    #endif

}
