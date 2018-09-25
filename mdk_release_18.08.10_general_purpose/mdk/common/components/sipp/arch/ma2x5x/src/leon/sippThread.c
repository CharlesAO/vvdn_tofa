///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine, RTOS thread
///

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

#if SIPP_RTOS != SIPP_NO_RTOS

/////////////////////////////////////////////////////////////////////////////////
//  Local Macros
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  Storage Allocation Globals
/////////////////////////////////////////////////////////////////////////////////
static SIPP_PAL_THREAD gSippThread;

///////////////////////////////////////////////////////////////////////
// Code

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippThreadCreate                                                 //
//                                                                                //
//  DESCRIPTION: Create the SIPP thread - the entry point is defined elsewhere    //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

bool sippThreadCreate ()
{
    bool bRetVal = FALSE;

    if (eSIPP_STATUS_OK == sippPalThreadCreate ((SIPP_PAL_PFNTHREAD)sippAccessSchedulerControl,
                                                0,
                                                (void **)NULL,
                                                SIPP_THREAD_STACK_SIZE,
                                                SIPP_THREAD_PRIORITY,
                                                SIPP_THREAD_NAME,
                                                &gSippThread))
    {
        bRetVal = TRUE;
    }

    return bRetVal;

}

#endif

