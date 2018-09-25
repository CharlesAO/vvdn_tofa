///
/// @file      sippError.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework Error management
///            With a concurrent pipeline API, the minimum aim is that an error
///            on one pipeline does NOT effect other pipelines also running at
///            that time
///
///            Platform(s) supported : MA2150
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////
u32 gSippErrCode[SIPP_ERROR_HISTORY_SIZE];
u32 gSippErrWrIdx, gSippErrRdIdx;

////////////////////////////////////////////////////
// Static variables
////////////////////////////////////////////////////
u32 gSippFatalErrList[SIPP_ERROR_MASK_SIZE];

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Internal functions
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippErrorInit                                                    //
//                                                                                //
//  DESCRIPTION: Sipp framework error handling initialisation                     //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippErrorInit ( )
{
    gSippErrWrIdx = gSippErrRdIdx = 0;

    // These errors have no graceful handling mechanisms at present so make them fatal
    sippErrorSetFatal (E_CDMA_QU_OVERFLOW);
    sippErrorSetFatal (E_INVLD_SLICE_WIDTH);
    sippErrorSetFatal (E_PC_CMX_MEM_ALLOC_ERR);
    sippErrorSetFatal (E_OSE_CREATION_ERROR);
    sippErrorSetFatal (E_DATA_NOT_FOUND);
    sippErrorSetFatal (E_PRECOMP_SCHED);
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippError                                                        //
//                                                                                //
//  DESCRIPTION: Sipp framework error resolution                                  //
//                                                                                //
//  INPUTS:      errCode - The error found                                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Calls to this on SOC may be bad for your application's health.   //
//               If the error code is considered fatal this will cause a TRAP to  //
//               the reset vector entry                                           //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippError (u32 * errStatusMask, u32 errCode)
{
    u32 errMaskIdx   = errCode >> 5;
    u32 errMaskEntry = 0x1 << (errCode & 0x1F);

    gSippErrCode[gSippErrWrIdx++] = errCode;

    if (gSippErrWrIdx == SIPP_ERROR_HISTORY_SIZE) gSippErrWrIdx = 0;

    if (errStatusMask)
    {
        errStatusMask[errMaskIdx] |= errMaskEntry;
    }

    #if defined(SIPP_PC)
    // Assume all errors are fatal in model builds
    sippPalTrace (SIPP_CORE_TL_ERROR, "FATAL_ERROR: %d !\n", errCode);
    abort ();
    #elif defined(__sparc)
    #ifdef SIPP_ERROR_ALL_FATAL
    asm volatile( "set 1, %g1; ta 0\n" );
    #else
    u32 errFatal = gSippFatalErrList[errMaskIdx];
    if (errFatal & errMaskEntry)
    {
        asm volatile( "set 1, %g1; ta 0\n" );
    }
    #endif
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAssert                                                       //
//                                                                                //
//  DESCRIPTION: Sipp framework assert condition                                  //
//                                                                                //
//  INPUTS:      errCode                                                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Pass is that condition is true, otherwise error.                 //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippAssert (u32 condition, u32 errCode)
{
    if (condition == 0)
    {
        sippError ((u32 *)NULL, errCode);
    }
}

////////////////////////////////////////////////////
// API functions
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippErrorSetFatal                                                //
//                                                                                //
//  DESCRIPTION: Sipp framework error resolution set error type as being fatal    //
//                                                                                //
//  INPUTS:      errCode                                                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippErrorSetFatal (u32 errCode)
{
    #ifndef SIPP_ERROR_ALL_FATAL

    gSippFatalErrList[(errCode >> 5)] |= (0x1 << (errCode & 0x1F));

    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetLastError                                                 //
//                                                                                //
//  DESCRIPTION: Sipp framework error API returning last error recorded           //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     The last error code recorded                                     //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGetLastError ( )
{
    if (gSippErrWrIdx == 0) return gSippErrCode[SIPP_ERROR_HISTORY_SIZE - 0x1];
    else                    return gSippErrCode[gSippErrWrIdx - 0x1];
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPipeGetErrorStatus                                           //
//                                                                                //
//  DESCRIPTION: Sipp framework error API returning all current recorded errors   //
//               on a specific pipeline                                           //
//                                                                                //
//  INPUTS:      pPipe - PipeLine to check                                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x1 if any error code recorded                                   //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippPipeGetErrorStatus (SippPipeline * pPipe)
{
    u32 retCode = 0x0;
    u32 idx;

    for (idx = 0; idx < SIPP_ERROR_MASK_SIZE; idx++)
    {
        if (pPipe->errorStatus[idx])
        {
            retCode = 0x1;
            break;
        }
    }

    return retCode;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetErrorHistory                                              //
//                                                                                //
//  DESCRIPTION: Sipp framework error API returning the last errors recorded      //
//               up to a maximum of the cfg define SIPP_ERROR_HISTORY_SIZE        //
//               This is numbered from boot or the last call to this function     //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     ptrErrList - Array of last errors recorded                       //
//                                                                                //
//  RETURNS:     The number of valid errors in the list                           //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGetErrorHistory (u32 * ptrErrList)
{
    u32 errIdx = 0;
    while (gSippErrWrIdx != gSippErrRdIdx)
    {
        ptrErrList[errIdx++] = gSippErrCode[gSippErrRdIdx++];
        if (gSippErrRdIdx == SIPP_ERROR_HISTORY_SIZE) gSippErrRdIdx = 0;
    }

    return errIdx;
}
