///   
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved. 
///            For License Warranty see: common/license.txt   
///
/// @brief     Software test library
/// 
/// 
/// 

// 1: Includes
// ----------------------------------------------------------------------------
#include "swcTestUtils.h"
#include "registersMyriad.h"
#include "stdio.h"
#include "DrvSvuDefines.h"
#include "DrvSvu.h"
#include "DrvTimer.h"
#include <DrvRegUtilsDefines.h>
#include <dbgTracerApi.h>


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data 
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
    tyTimeStamp execTime;
tyProcessorType swcGetProcessorType(void)
{
    tyProcessorType processorType ;
    
    if (IS_PLATFORM_SILICON)
    {
        processorType = MVI_IC;
    }
    else if (IS_PLATFORM_VCS)
    {
        processorType = MVI_VCS;
    }
    else if (IS_PLATFORM_FPGA)
    {
        processorType = MVI_FPGA;
    }
    else if (IS_PLATFORM_MOVISIM)
    {
        processorType = MVI_FSIM;
    }
    else
    {
        processorType = MVI_UNKNOWN;
    }
    return processorType;
}

void swcShaveProfInit(performanceStruct *perfStruct)
{
    //resets the structure's elements

    perfStruct->perfCounterStall = -1;
    perfStruct->perfCounterExec  = -1;
    perfStruct->perfCounterClkCycles  = -1;
    perfStruct->perfCounterBranch = -1;
    perfStruct->perfCounterTimer  = 0;
    perfStruct->countShCodeRun = 0;
    perfStruct->stallsTypes = PC_DEFAULT;

}


void swcShaveProfStartGathering(u32 shaveNumber, performanceStruct *perfStruct)
{
    perfStruct->countShCodeRun++;
    switch (perfStruct->countShCodeRun)
    {
    case 1:
        DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_EX_IN_EN);
        DrvSvuEnablePerformanceCounter(shaveNumber, 1, perfStruct->stallsTypes);
        DrvSvuEnablePerformanceCounter(shaveNumber, 2, PC_BR_TAKEN_EN);
        DrvSvuEnablePerformanceCounter(shaveNumber, 3, PC_CLK_CYC_EN);
        break;
    case 2:
        DrvTimerStartTicksCount(&perfStruct->executionTimer);
        break;
    default:
        break;
    }
    SET_REG_WORD(DCU_PC3(shaveNumber), 0);
    SET_REG_WORD(DCU_PC2(shaveNumber), 0);
    SET_REG_WORD(DCU_PC1(shaveNumber), 0);
    SET_REG_WORD(DCU_PC0(shaveNumber), 0);

}


int swcShaveProfGatheringDone(performanceStruct *perfStruct)
{

    int done = 0;
    if (perfStruct->countShCodeRun == 2){
        done = 1;
    }
    else
        done = 0;

    return done;
}


void swcShaveProfStopGathering(u32 shaveNumber, performanceStruct *perfStruct)
{
    u32 count1, count2;
    count1 = GET_REG_WORD_VAL(DCU_PC0(shaveNumber));
    // get the stalls number
    count2 = GET_REG_WORD_VAL(DCU_PC1(shaveNumber));

    if(perfStruct->countShCodeRun == 1)
    {
    	perfStruct->perfCounterExec = count1;
        perfStruct->perfCounterStall = count2;

        // Get the branches taken number
        perfStruct->perfCounterBranch = DrvSvuGetPerformanceCounter2(shaveNumber);
        // Get the clock cycles
        perfStruct->perfCounterClkCycles = DrvSvuGetPerformanceCounter3(shaveNumber);
    }
    if(perfStruct->countShCodeRun == 2)
        DrvTimerGetElapsedTicks(&perfStruct->executionTimer, &perfStruct->perfCounterTimer);

}


void swcShaveProfPrint(u32 shaveNumber, performanceStruct *perfStruct)
{
    (void) shaveNumber; // "use" the variables to hush the compiler warning.

    switch(perfStruct->countShCodeRun)
    {
    case 2:
        printf("\nLeon executed %lu cycles in %06lu micro seconds ([%lu ms])\n",(u32)(perfStruct->perfCounterTimer)
                ,(u32)(DrvTimerTicksToMs(perfStruct->perfCounterTimer)*1000)
                ,(u32)(DrvTimerTicksToMs(perfStruct->perfCounterTimer)));

    case 1:
        printf("\nShave executed clock cycle count: %ld\n", perfStruct->perfCounterClkCycles);
        printf("\nNumber of branches taken: %ld\n", perfStruct->perfCounterBranch);
        printf("\nShave measured stall cycle count: %ld\n", perfStruct->perfCounterStall);
        printf("\nShave measured instruction count: %ld\n", perfStruct->perfCounterExec);
    default:{
        if (perfStruct->countShCodeRun>1){
            printf("\nMeasuring over multiple runs. Small disparities are expected.\n");
        }
    }
    }
}

void swcShaveProfStartGatheringFields(u32 shaveNumber, performanceCounterDef perfDefines)
{
    performanceCounterDef perfInfoType;
    perfInfoType = perfDefines;
    if (perfInfoType == PERF_INSTRUCT_COUNT)
    {
        DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_EX_IN_EN);
    }
    if (perfInfoType == PERF_STALL_COUNT)
    {
         DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_DEFAULT);
    }
    if (perfInfoType == PERF_CLK_CYCLE_COUNT)
    {
        DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_CLK_CYC_EN);
    }
    if (perfInfoType == PERF_BRANCH_COUNT)
    {
        DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_BR_TAKEN_EN);
    }
    if (perfInfoType == PERF_TIMER_COUNT)
    {
        DrvTimerStartTicksCount(&execTime);
    }
    SET_REG_WORD(DCU_PC0(shaveNumber), 0);
}

void swcShaveProfStopFieldsGathering(u32 shaveNumber, performanceCounterDef perfDefines)
{
    unsigned long long timeCount;
    int perfGathered = perfDefines;
#ifdef NO_PRINT
    UNUSED(shaveNumber);
#else
    u32 count = GET_REG_WORD_VAL(DCU_PC0(shaveNumber));
    u32 result;
    if (perfGathered == PERF_STALL_COUNT)
    {
        result = count;
        printf("\nShave measured stall cycle count: %ld\n", result);
    }
    if (perfGathered == PERF_INSTRUCT_COUNT)
    {
        result = count;
        printf("\nShave measured instruction count: %ld\n", result);
    }
    if (perfGathered == PERF_CLK_CYCLE_COUNT)
    {
        result = count;
        printf("\nShave executed clock cycle count: %ld\n", result);
    }
    if (perfGathered == PERF_BRANCH_COUNT)
    {
        result = count;
        printf("\nNumber of branches taken: %ld\n", result);
    }
#endif
    if (perfGathered == PERF_TIMER_COUNT)
    {
        DrvTimerGetElapsedTicks(&execTime, &timeCount);
        printf("\nLeon executed %lu cycles in %06lu micro seconds ([%lu ms])\n",(u32)(timeCount)
            ,(u32)(DrvTimerTicksToMs(timeCount)*1000)
            ,(u32)(DrvTimerTicksToMs(timeCount)));
    }
}

void swcShaveProfStopFieldsGatehering(u32 shaveNumber, performanceCounterDef perfDefines) 
{
    swcShaveProfStopFieldsGathering(shaveNumber, perfDefines);
}

void swcShaveProfileCyclesStart(u32 shaveNumber)
{

    DrvSvuEnablePerformanceCounter(shaveNumber, 0, PC_EX_IN_EN);
    DrvSvuEnablePerformanceCounter(shaveNumber, 1, PC_STALL_EN);

	DrvTimerStartTicksCount(&execTime);

    SET_REG_WORD(DCU_PC0(shaveNumber), 0);
    SET_REG_WORD(DCU_PC1(shaveNumber), 0);
}


void swcShaveProfileCyclesStop(u32 shaveNumber)
{
#ifdef NO_PRINT
    UNUSED(shaveNumber);
#else
    u32 count0, count1;
    unsigned long long timeCount;

    count1 = GET_REG_WORD_VAL(DCU_PC1(shaveNumber));
    DrvTimerGetElapsedTicks(&execTime, &timeCount);
    count0 = GET_REG_WORD_VAL(DCU_PC0(shaveNumber));

    printf("\nMeasuring at different offsets: \n");
    printf("\nShave measured instruction count: %ld\n", count0);
    printf("\nShave measured stall cycle count: %ld\n", count1);
    printf("\nLeon executed %lu cycles in %06lu micro seconds ([%lu ms])\n",(u32)(timeCount)
        ,(u32)(DrvTimerTicksToMs(timeCount)*1000)
        ,(u32)(DrvTimerTicksToMs(timeCount)));
#endif
}
