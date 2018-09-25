///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Unit Test Framework
///
/// Basic utility functions for unit testing
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>
#include <registersMyriad.h>
#include <DrvTimer.h>
#include "string.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// This is a global flag that is used in some testBench code as a way to determine
// that the test has reached completion
// Also have a global variable that allows us to check number of failures from debugger
// after execution
volatile u32 unitTestTestComplete = 0;
volatile u32 unitTestNumFailures  = 8888; // This intial default should always be ovewritten by code before being read, otherwise assume failure


// 4: Static Local Data
// ----------------------------------------------------------------------------
static atomic_ulong numTestsPassed;
static atomic_ulong numTestsRan;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

int unitTestInit(void)
{
    atomic_init(&numTestsPassed, 0);
    atomic_init(&numTestsRan, 0);
    return 0;
}

int unitTestLogPass(void)
{
    atomic_fetch_add(&numTestsRan, 1);     //numTestsRan++;
    atomic_fetch_add(&numTestsPassed, 1);  //numTestsPassed++;
    return 0;
}

int unitTestLogFail(void)
{
    atomic_fetch_add(&numTestsRan, 1);
    return 0;
}

int unitTestAssert(int value)
{
    atomic_fetch_add(&numTestsRan, 1);
    if (value)
        atomic_fetch_add(&numTestsPassed, 1);

    return 0;
}

int unitTestExecutionWithinRange(float actual, float expected, float percentageError)
{
    float minVal;
    float maxVal;

    atomic_fetch_add(&numTestsRan, 1);

    //Early exit if we have negative numbers
    if (actual<0.0f){
        return 0;
    }

    minVal = expected * ((100.0 - percentageError) / 100.0);
    maxVal = expected * ((100.0 + percentageError) / 100.0);
    //Also, if actual number is 0.0f then this means the program ran tocompletion very fast
    //which probably means it passed :)
    if ( ((actual > minVal) && (actual < maxVal)) || (actual==0.0f) )
        atomic_fetch_add(&numTestsPassed, 1);

    return 0;
}

int unitTestFloatWithinRange(float actual, float expected, float percentageError)
{
    float difference,absErrorAccepted;

    atomic_fetch_add(&numTestsRan, 1);
    difference=actual-expected;
    if (difference<0) difference=-difference;

    //Considering that a deviation of 1.0f is 100% error
    absErrorAccepted=percentageError/100.0f;
    if (difference<=absErrorAccepted){
        atomic_fetch_add(&numTestsPassed, 1);
    }

    return 0;
}

int unitTestFloatAbsRangeCheck(float actual, float expected, float AbsError)
{
    float difference;
    difference=actual-expected;
    atomic_fetch_add(&numTestsRan, 1);
    if (difference<0) difference=-difference;
    if (difference<=AbsError){
        atomic_fetch_add(&numTestsPassed, 1);
    }
    return 0;
}

void unitTestMemCompare(const void * pActualStart,const void * pExpectedStart, u32 lengthBytes)
{
    int does_it_pass = 0;
    atomic_fetch_add(&numTestsRan, 1);

    does_it_pass = memcmp(pActualStart, pExpectedStart, lengthBytes);
    if(does_it_pass == 0)
        atomic_fetch_add(&numTestsPassed, 1);
}

void unitTestMemCompareDeltaU8(u8 * pActualStart,u8 * pExpectedStart, u32 lengthBytes, u8 delta)
{
    unsigned int i;
    int does_it_pass = 0;
    int negDelta = -delta, diff;
    atomic_fetch_add(&numTestsRan, 1);

    for(i=0; i < lengthBytes; i++)
    {
        diff = pActualStart[i] - pExpectedStart[i];
       if( ( diff > delta) || ( diff < negDelta) )
       {
            does_it_pass++;
            break;
       }
    }
    if(does_it_pass == 0)
        atomic_fetch_add(&numTestsPassed, 1);
}

void unitTestCompare(u32 actualValue, u32 expectedValue)
{
    atomic_fetch_add(&numTestsRan, 1);
    if (actualValue == expectedValue)
    {
        atomic_fetch_add(&numTestsPassed, 1);
    }
}

int unitTestLogResults(int passes,int fails)
{
    atomic_fetch_add(&numTestsRan, passes);
    atomic_fetch_add(&numTestsRan, fails);
    atomic_fetch_add(&numTestsPassed, passes);
    return 0;
}

u32 unitTestCheckSectionFail(char *sectionName)
{
    static u32 lastFailNb = 0;
    u32 failNb = (u32)atomic_load(&numTestsRan) - (u32)atomic_load(&numTestsPassed) - lastFailNb;
    if (failNb)
    {
        if (sectionName)
            printf("moviUnitTest: %ld failure(s) in %s section\n", (long int) failNb, sectionName);
        lastFailNb += failNb;
    }
    return failNb;
}

int unitTestFinalReport(void)
{
    if (atomic_load(&numTestsPassed) == atomic_load(&numTestsRan))
    {
        printf("\nmoviUnitTest:PASSED\n");
    }
    else
    {
        printf("\nmoviUnitTest:FAILED : %ld failures\n",(long int)(atomic_load(&numTestsRan) - atomic_load(&numTestsPassed)));
    }

    // Some testbenches make use of this global flag to signal the end of
    // test
    unitTestTestComplete = TRUE;
    unitTestNumFailures  = (u32)(atomic_load(&numTestsRan) - atomic_load(&numTestsPassed));

    return 0;
}
