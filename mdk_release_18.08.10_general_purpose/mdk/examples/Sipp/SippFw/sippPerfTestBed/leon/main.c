///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application to measure and report on the perfomance of one or
///            more SIPP pipelines potentially running concurrently
///            This test uses pipelines which have no conflicting SHAVE or
///            HW filter resource and therefore should be capable of running
///            concurrently

#include <sipp.h>
#include <sippTestCommon.h>
#include <string.h>

#ifndef SIPP_PC
#include "app_config.h"
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>
#endif

#include "perfTestbed.h"

/////////////////////////////////////////////////////
// Global Macros
/////////////////////////////////////////////////////

#define TESTBED_LOG_FILE "perfTestBedLog.txt"

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

volatile u32          testComplete = 0;
PerfTestEvent         tAppEvents[PERF_TESTBED_EVENT_HISTORY_SIZE];
u32                   appEventIdx = 0;
u8                    numPipesRunCtrl[PERF_TESTBED_NUM_PIPELINES + 1] = {0};
u8                    numPipesRunning = 0;

#ifndef SIPP_PC
u64 startT[PERF_TESTBED_NUM_PIPELINES], endT[PERF_TESTBED_NUM_PIPELINES], frameT[PERF_TESTBED_NUM_PIPELINES];
u64 testStart, testEnd;
#endif

static u8 perfTestbedFileDumpBuffer[FILE_DUMP_BUFFER_MAX_SIZE] SECTION(".ddr.bss") ALIGNED(8);

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////

void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData);

void appConfigurePipelines ( );

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appInit                                                      //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appInit ()
{
    memset (perfTestbedPipeList, 0x0, sizeof(perfTestbedPipeList));
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLookupHandle                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appLookupHandle (SippPipeline * pPipeline)
{
    u32 idx;

    for (idx = 0; idx < PERF_TESTBED_NUM_PIPELINES; idx++)
    {
        PerfTestbedPipe * pipe = &perfTestbedPipeList[idx];

        if (pipe->sippFwHandle == pPipeline)
        {
            return idx;
        }
    }

    return 0xFF;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCreatePipelines                                           //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appCreatePipelines ( )
{
    u32 idx;
    for (idx = 0; idx < PERF_TESTBED_NUM_PIPELINES; idx++ )
    {
        PerfTestbedPipe * pipe = &perfTestbedPipeList[idx];

        pipe->sippFwHandle = sippCreatePipeline (sliceFirst[idx],
                                                 sliceLast[idx],
                                                 SIPP_MBIN(mbinImgSipp));

        if (pipe->sippFwHandle == (SippPipeline *)NULL)
        {
            printf ("*** Pipeline creation failure\n*** Test will terminate early\n");
            exit (0x0);
        }

        // Register callback for async API
        sippRegisterEventCallback (pipe->sippFwHandle,
                                   appSippCallback);
    }
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appPipeLogEvent                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appPipeLogEvent (PerfTestbedPipe *  pipe,
                      PERF_TB_PIPE_EVENT event,
                      u64                data)
{
    UNUSED(data);

    pipe->eventList[pipe->eventListWrPtr++] = event;
    if (pipe->eventListWrPtr == PERF_TESTBED_EVENT_PIPE_HISTORY_SIZE) pipe->eventListWrPtr = 0;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appProcessEvent                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

PERF_TB_PIPE_EVENT appProcessEvent (PerfTestbedPipe * pipe)
{
    PERF_TB_PIPE_EVENT retVal = PERF_TB_PIPE_EVENT_NONE;

    if (pipe->eventListWrPtr != pipe->eventListRdPtr )
    {
        retVal = pipe->eventList[pipe->eventListRdPtr++];
        if (pipe->eventListRdPtr == PERF_TESTBED_EVENT_PIPE_HISTORY_SIZE) pipe->eventListRdPtr = 0;

    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appSippCallback                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    #ifndef SIPP_PC
    // Find pipeline
    u32               pipeIdx = appLookupHandle (pPipeline);
    PerfTestbedPipe * pipe    = &perfTestbedPipeList[pipeIdx];

    // Use critical sections to future proof against this happening in an RTOS thread
    u32 storedIL = swcLeonGetPIL ();
    swcLeonSetPIL (15);
    #endif

    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        #ifdef SIPP_PERF_TB_DBG_PRINT
        printf ("Pipe %u completes frame\n", (unsigned int)pipeIdx);
        #endif

        #ifndef SIPP_PC
        DrvTimerGetSystemTicks64 (&endT[pipeIdx]);
        tAppEvents[appEventIdx].pipeIdx     = (u8)pipeIdx;
        tAppEvents[appEventIdx].event       = PERF_TB_PIPE_EVENT_FRAME_DONE;
        tAppEvents[appEventIdx++].eventData = endT[pipeIdx];

        if (appEventIdx == PERF_TESTBED_EVENT_HISTORY_SIZE)
        {
            printf ("Error : Extend pipe history size - early termination\n");
            exit (0x0);
        }

        appPipeLogEvent (pipe,
                         PERF_TB_PIPE_EVENT_FRAME_DONE,
                         endT[pipeIdx]);

        #endif
    }
    else if (eEvent == eSIPP_PIPELINE_STARTED)
    {
        #ifndef SIPP_PC
        // Mark time
        DrvTimerGetSystemTicks64 (&startT[pipeIdx]);
        tAppEvents[appEventIdx].pipeIdx     = (u8)pipeIdx;
        tAppEvents[appEventIdx].event       = PERF_TB_PIPE_EVENT_FRAME_STARTED;
        tAppEvents[appEventIdx++].eventData = startT[pipeIdx];

        if (appEventIdx == PERF_TESTBED_EVENT_HISTORY_SIZE)
        {
            printf ("Error : Extend pipe history size - early termination\n");
            exit (0x0);
        }

        #endif
    }

    #ifndef SIPP_PC
    swcLeonSetPIL (storedIL);
    #endif

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCheckComplete                                             //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appCheckComplete ( )
{
    u32 retVal = 0x1;
    u32 idx;

    for (idx = 0; idx < PERF_TESTBED_NUM_PIPELINES; idx++ )
    {
        PerfTestbedPipe * pipe = &perfTestbedPipeList[idx];
        if (pipe->eState != PERF_TB_PIPE_STATE_COMPLETE)
        {
            retVal = 0x0;
            break;
        }
    }

    return retVal;
}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appTask                                                      //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appTask ()
{
    u32 pipeIdx;

    #ifndef SIPP_PC
    DrvTimerGetSystemTicks64 (&testStart);
    #endif

    while (testComplete == 0)
    {
        for (pipeIdx = 0x0; pipeIdx < PERF_TESTBED_NUM_PIPELINES; pipeIdx++)
        {
            PerfTestbedPipe * pipe = &perfTestbedPipeList[pipeIdx];

            switch (pipe->eState)
            {
                case PERF_TB_PIPE_STATE_UNINIT   : {
                                                       pipe->eState = PERF_TB_PIPE_STATE_IDLE;
                                                   } break;
                case PERF_TB_PIPE_STATE_IDLE     : {
                                                       if (pipe->framesComplete < PERF_TESTBED_FRAMES_PER_PIPE)
                                                       {
                                                           #ifdef SIPP_RUN_ASYNC

                                                           sippProcessFrameNB (pipe->sippFwHandle);
                                                           pipe->eState = PERF_TB_PIPE_STATE_RUNNING;
                                                           numPipesRunning++;
                                                           if (numPipesRunning > 2) printf("I should not be!!!\n");
                                                           #else

                                                           u32 errCode      = (u32)E_BLOCK_CALL_REJECTED;
                                                           u32 errMaskIdx   = errCode >> 5;
                                                           u32 errMaskEntry = 0x1 << (errCode & 0x1F);

                                                           // We wish to call the sync API
                                                           sippProcessFrame (pipe->sippFwHandle);

                                                           // Check if the call worked
                                                           // Interrogate the error
                                                           if (pipe->sippFwHandle->errorStatus[errMaskIdx] & errMaskEntry)
                                                           {
                                                               printf ("Pipe %lu failed in call to blocking API sippProcessFrame due to active and/or pending operations\n", pipeIdx);
                                                               // Clear the error
                                                               pipe->sippFwHandle->errorStatus[errMaskIdx] &= (~errMaskEntry);
                                                           }
                                                           else
                                                           {
                                                               printf ("Pipe %lu successfully called blocking API sippProcessFrame\n", pipeIdx);
                                                               pipe->framesComplete++;
                                                               pipe->eState = PERF_TB_PIPE_STATE_IDLE;
                                                           }

                                                           #endif
                                                       }
                                                       else
                                                       {
                                                           #ifdef SIPP_PERF_TB_DBG_PRINT
                                                           printf ("Pipe %u completed run of %u images\n", (unsigned int)pipeIdx, (unsigned int)pipe->framesComplete);
                                                           #endif

                                                           sippFrameworkRun ();
                                                           pipe->eState = PERF_TB_PIPE_STATE_COMPLETE;
                                                       }

                                                   } break;
                case PERF_TB_PIPE_STATE_RUNNING  : {
                                                       PERF_TB_PIPE_EVENT event;
                                                       while ((event = appProcessEvent (pipe)) != PERF_TB_PIPE_EVENT_NONE)
                                                       {
                                                           switch (event)
                                                           {
                                                               case PERF_TB_PIPE_EVENT_FRAME_DONE : {
                                                                                                        pipe->framesComplete++;
                                                                                                        numPipesRunning--;
                                                                                                        pipe->eState = PERF_TB_PIPE_STATE_IDLE;
                                                                                                    } break;
                                                               default : break;
                                                           }
                                                       }
                                                   } break;
                case PERF_TB_PIPE_STATE_COMPLETE : {
                                                       if (appCheckComplete())
                                                       {
                                                           testComplete = 1;
                                                       }
                                                   } break;
                default                          : {
                                                       printf ("Unknown state on pipe %u - Terminating early\n", (unsigned int)pipeIdx);
                                                       exit (0x0);
                                                   } break;
            }
        }
    }

    #ifndef SIPP_PC
    DrvTimerGetSystemTicks64 (&testEnd);
    printf("TestCycles = %llx\n\n", testEnd - testStart);

    #ifdef SIPP_RUNTIME_PROFILING
    for (pipeIdx = 0x0; pipeIdx < PERF_TESTBED_NUM_PIPELINES; pipeIdx++)
    {
        PerfTestbedPipe * pipe = &perfTestbedPipeList[pipeIdx];

        printf("Run long-pole identification algo on Pipe %lu\n-----------------------------------------------\n\n", pipeIdx);
        // Print Runtime statistics for the last frame processed on each pipeline
        sippDbgParseRTStats (pipe->sippFwHandle, SIPP_RT_STATS_PRINT_CTRL);
    }
    #endif
    #endif

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCreateLog                                                 //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appCreateLog ()
{
    #ifndef SIPP_PC
    char               lineBuffer [MAX_LINE_LENGTH];
    u32                idx, idx2;
    u32                pipeIdx;
    PERF_TB_PIPE_EVENT event;
    u64                eventData;
    u64                startCycle[PERF_TESTBED_NUM_PIPELINES] = {0};
    u8                 runStatus[PERF_TESTBED_NUM_PIPELINES];
    u8                 runTally[PERF_TESTBED_NUM_PIPELINES];
    u64                cycleTime;
    u32                prtFrameTime;

    perfTestbedSetFileBuffer (perfTestbedFileDumpBuffer);

    PERF_TB_DUMP_LINE(lineBuffer, "Test Log : Total test cycles %llu LPI %d\n", testEnd - testStart, PERF_TESTBED_PIPE0_LINES_PER_ITER);
    PERF_TB_DUMP_LINE(lineBuffer, "<   Time   >|");

    for (idx = 0; idx < PERF_TESTBED_NUM_PIPELINES; idx++)
    {
        PERF_TB_DUMP_LINE(lineBuffer, "<   Str %lu  >|", idx);
        runStatus[idx] = 0;
        runTally[idx]  = 0;
    }
    PERF_TB_DUMP_LINE(lineBuffer, "\n------------|");
    for (idx = 0; idx < PERF_TESTBED_NUM_PIPELINES; idx++)
    {
        PERF_TB_DUMP_LINE(lineBuffer, "------------|");
    }
    PERF_TB_DUMP_LINE(lineBuffer, "\n");

    for (idx = 0; idx < appEventIdx; idx++)
    {
        pipeIdx   = tAppEvents[idx].pipeIdx;
        event     = tAppEvents[idx].event;
        eventData = tAppEvents[idx].eventData;

        prtFrameTime    = 0x0;
        cycleTime = eventData - testStart;
        PERF_TB_DUMP_LINE(lineBuffer, "%11llu |", cycleTime);

        idx2 = 0;
        while (idx2 < pipeIdx)
        {
            if (runStatus[idx2])
            {
                PERF_TB_DUMP_LINE(lineBuffer, "      V     |");
            }
            else
            {
                PERF_TB_DUMP_LINE(lineBuffer, "            |");
            }
            idx2++;
        }

        if (event == PERF_TB_PIPE_EVENT_FRAME_DONE)
        {
            PERF_TB_DUMP_LINE(lineBuffer, "   F %4d C |", runTally[pipeIdx]);
            runTally[pipeIdx]++;
            runStatus[pipeIdx] = 0x0;
            prtFrameTime       = 0x1;
        }
        else if (event == PERF_TB_PIPE_EVENT_FRAME_STARTED)
        {
            PERF_TB_DUMP_LINE(lineBuffer, "   F %4d S |", runTally[pipeIdx]);
            runStatus[pipeIdx]  = 0x1;
            startCycle[pipeIdx] = cycleTime;
        }
        else if (event == PERF_TB_PIPE_POWER_MEAS_STARTED)
        {
            PERF_TB_DUMP_LINE(lineBuffer, "         Start power measurement");
        }
        else if (event == PERF_TB_PIPE_POWER_MEAS_DONE)
        {
            PERF_TB_DUMP_LINE(lineBuffer, "         Power measurement done");
        }

        idx2 = pipeIdx + 1;
        while (idx2 < PERF_TESTBED_NUM_PIPELINES)
        {
            if (runStatus[idx2])
            {
                PERF_TB_DUMP_LINE(lineBuffer, "      V     |");
            }
            else
            {
                PERF_TB_DUMP_LINE(lineBuffer, "            |");
            }
            idx2++;
        }

        if (prtFrameTime)
        {
            PERF_TB_DUMP_LINE(lineBuffer, "         Str %lu last frame cycle count : %11llu", pipeIdx, cycleTime - startCycle[pipeIdx]);
        }
        PERF_TB_DUMP_LINE(lineBuffer, "\n");
    }

    perfTestbedDumpFile (TESTBED_LOG_FILE);

    #endif
}

////////////////////////////////////////////////////////////////////////////////
// MAIN

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    main                                                         //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    #ifndef SIPP_PC
    unitTestInit ();
    initClocksAndMemory ();

    u32 freq =  DrvCprGetClockFreqKhz(SYS_CLK, NULL) / 1000;

    printf("\nBenchmark scenario\n");
    printf("===============================================================\n");
    printf("Frequency:  %luMHz\n\n", freq);

    #endif

    sippInitialize ();

    #ifndef SIPP_PC
    DrvTimerInit ();
    #endif

    #ifndef SIPP_PC
    #ifdef SIPP_RUN_ASYNC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
    }
    else
    {
    	printf ("App Starting on LEON RT\n");
    }
    #endif // SIPP_RUN_ASYNC
    #endif // SIPP_PC

    #ifdef SIPP_RUN_ASYNC
    sippPlatformInitAsync ();
    #endif

    appInit ();
    appCreatePipelines ();
    appConfigurePipelines ();

    // Load input data for pipes
    appTask ();

    // Write output data / run checks etc...
    #ifndef SIPP_PC
    unitTestFinalReport();
    appCreateLog ();
    #endif

   return 0;
}
