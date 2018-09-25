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
#include "assert.h"
#include "UnitTestApi.h"
#include "registersMyriad.h"
#include "stdio.h"
// UnitTestVcs is dependent on VcsHooks component
#include <VcsHooksApi.h>
#include <swcWhoAmI.h>
#include "string.h"

#ifdef PROD_TEST_DLT_ENABLE
#  include "DltUtilsApi.h"     // Used when test is part of the production test program
#endif

#ifdef EXPECT_FREQ_MHZ
#ifdef __RTEMS__
#  include "OsDrvCpr.h"  // Used to get system clock frequency
#else
#  include "DrvCpr.h"  // Used to get system clock frequency
#endif

#  ifndef BUP_PLL0_FREQ
#    define BUP_PLL0_FREQ (EXPECT_FREQ_MHZ*1000)
#  endif

#  ifndef BUP_MCLK_DIV_NUM
#    define BUP_MCLK_DIV_NUM (DONT_TOUCH_MASTER_DIVIDER)
#  endif

#  ifndef BUP_MCLK_DIV_DEN
#    define BUP_MCLK_DIV_DEN (DONT_TOUCH_MASTER_DIVIDER)
#  endif
#endif

#define ENABLE_LEON_CHECKING (0)
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ---------------------------------------------------------------------------


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


#ifdef LEON_RT
u32 lrt_numPass;
u32 lrt_numFail;
tyTestType lrt_globalTestType;
#endif

tyTestType globalTestType;
u32 numPass;
static u32 verbosity=VERBOSITY_QUIET; // Default to summary info including pass and fails
u32 numFail;
static int cacheBypass=0;             // Default to not bypassing caches
//#endif

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static inline void convertAddrIfCacheBypass(void **addr)
{
    if (VCSHOOKS_USE_SOFTWARE && cacheBypass)
    {
        *addr = DrvRegGetUncacheableAddr(*addr);
    }
}

#ifdef LEON_RT
// this function should not be called from LEON RT

void lrt_unitTestInit(void)
{
    unitTestInit();
}
#endif

void unitTestInit(void)
{
    unitTestVerbosity(verbosity); // This signals the simulator to use our default verbosity level
    numPass = 0;
    numFail = 0;
#ifdef LEON_RT

    lrt_numPass = 0;
    lrt_numFail = 0;
    lrt_globalTestType = EXPECT_TESTS_TO_PASS;
#endif
    globalTestType = EXPECT_TESTS_TO_PASS;

#ifdef PROD_TEST_DLT_ENABLE
    DltUtilsSignalEvent(DLT_EVENT_TEST_INIT,0);
#endif
}

char* resultFail="FAIL";
char* resultPass="PASS";

#pragma GCC diagnostic ignored "-Wunused-function"
static char *passFail(int pass)
{
    if (globalTestType == EXPECT_TESTS_TO_FAIL)
    {
        pass = !pass;
    }
    char *result = resultFail;
    if (pass)
    {
        numPass++;
        result = resultPass;
    }
    else
    {
        numFail++;
#ifdef UNITTEST_FAIL_FAST
        unitTestFinalReport();
        assert(0);
#endif
    }
    return result;
}

#ifdef LEON_RT
void lrt_unitTestVerbosity(tyUnitVerbosity targetVerbosity)
{
    unitTestVerbosity(targetVerbosity);
}
#endif

void unitTestVerbosity(tyUnitVerbosity targetVerbosity)
{
    verbosity = targetVerbosity;
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_SET_VERBOSITY,(u32)targetVerbosity,0,0,0); // Signal the hardware that the verbosity is changed
    }
    else
    {
        switch (verbosity)
        {
            case VERBOSITY_QUIET:
                prefixed_printf("unitTestVerbosity()    : VERBOSITY_QUIET\n");
                break;
            case VERBOSITY_SILENT:
                prefixed_printf("unitTestVerbosity()    : VERBOSITY_SILENT\n");
                break;
            case VERBOSITY_DIFFS:
                prefixed_printf("unitTestVerbosity()    : VERBOSITY_DIFFS\n");
                break;
            case VERBOSITY_ALL:
                prefixed_printf("unitTestVerbosity()    : VERBOSITY_ALL\n");
                break;
            default:
                prefixed_printf("unitTestVerbosity()    : VERBOSITY_???\n");
        }
    }
}

void unitTestLogPass(void)
{
    unitTestAssert(1);
}

void unitTestLogFail(void)
{
   unitTestAssert(0);
}

void unitTestAssert(int value)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_ASSERT,value,0,0,0);
    }
    else
    {
        prefixed_printf("unitTestAssert()       : (value:0x%08x)                            => %s\n", value, passFail(value));
    }
}

void unitTestCompare(u32 actualValue, u32 expectedValue)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_WORD_CMP,actualValue,expectedValue,0,0);
    }
    else
    {
        prefixed_printf("unitTestCompare()      : (0x%08lx,0x%08lx)                       => %s\n", actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestCompare64(u64 actualValue, u64 expectedValue)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        u32 actualUpper;
        u32 actualLower;
        u32 expectedUpper;
        u32 expectedLower;
        actualUpper   = (actualValue>>32)  & 0xFFFFFFFF ;
        actualLower   = (actualValue>>0)   & 0xFFFFFFFF ;
        expectedUpper = (expectedValue>>32)& 0xFFFFFFFF ;
        expectedLower = (expectedValue>>0) & 0xFFFFFFFF ;
        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_DWORD_CMP,actualUpper,actualLower,expectedUpper,expectedLower,0,0); // Perform comparison in hardware
    }
    else
    {
        prefixed_printf("unitTestCompare64()    : (0x%016llx,0x%016llx)       => %s\n", actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestReadDWordCheck(void * dWordAddress, u64 expectedValue)
{
    convertAddrIfCacheBypass(&dWordAddress);
    u64 actualValue = GET_REG_DWORD_VAL(dWordAddress);
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        u32 actualUpper;
        u32 actualLower;
        u32 expectedUpper;
        u32 expectedLower;

        actualUpper   = (actualValue>>32)  & 0xFFFFFFFF ;
        actualLower   = (actualValue>>0)   & 0xFFFFFFFF ;
        expectedUpper = (expectedValue>>32)& 0xFFFFFFFF ;
        expectedLower = (expectedValue>>0) & 0xFFFFFFFF ;

        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_WORD_CMP_ADR,VCS_HOOK_ID_UNIT_TEST_DWORD_CHK,actualUpper,actualLower,expectedUpper,expectedLower,(u32)dWordAddress);
    }
    else
    {
        prefixed_printf("unitTestReadDWordChk()  : (addr:0x%08lu 0x%016llx,0x%016llx)       => %s\n", (u32)dWordAddress, actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestReadWordCheck(void * wordAddress, u32 expectedValue)
{
    convertAddrIfCacheBypass(&wordAddress);
    u32 actualValue = GET_REG_WORD_VAL(wordAddress);
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_WORD_CMP_ADR,VCS_HOOK_ID_UNIT_TEST_WORD_CHK,0,actualValue,0,expectedValue,(u32)wordAddress);
    }
    else
    {
        prefixed_printf("unitTestReadWordCheck() : (addr:0x%08lu 0x%08lx,0x%08lx)       => %s\n", (u32)wordAddress, actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestReadHalfCheck(void * address, u16 expectedValue)
{
    convertAddrIfCacheBypass(&address);
    u16 actualValue = GET_REG_HALF_VAL(address);
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_WORD_CMP_ADR,VCS_HOOK_ID_UNIT_TEST_HALF_CHK,0,(u32)actualValue,0,(u32)expectedValue,(u32)address);
    }
    else
    {
        prefixed_printf("unitTestReadHalfCheck() : (addr:0x%08lu 0x%04x,0x%04x)       => %s\n", (u32)address, actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestReadByteCheck(void * address, u8 expectedValue)
{
    convertAddrIfCacheBypass(&address);
    u8 actualValue = GET_REG_BYTE_VAL(address);
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_WORD_CMP_ADR,VCS_HOOK_ID_UNIT_TEST_BYTE_CHK,0,(u32)actualValue,0,(u32)expectedValue,(u32)address);
    }
    else
    {
        prefixed_printf("unitTestReadByteCheck() : (addr:0x%08lu 0x%02x,0x%02x)       => %s\n", (u32)address, actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestReadBitCheck(void * address, u32 startBit, u32 numBits, u32 expectedValue)
{
    u32 actualValue;
    u32 bitMask = ( numBits >= 32U ? 0xFFFFFFFF : ((1U << numBits) - 1U) );

    convertAddrIfCacheBypass(&address);
    actualValue = GET_REG_WORD_VAL(address);
    actualValue = actualValue >> startBit;
    actualValue &= bitMask;
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam6(VCS_HOOK_FUNC_WORD_CMP_ADR,VCS_HOOK_ID_UNIT_TEST_BIT_CHK,0,(u32)actualValue,0,(u32)expectedValue,(u32)address);
    }
    else
    {
        prefixed_printf("unitTestReadBitCheck()  : (addr:0x%08lu 0x%lx,0x%lx)       => %s\n", (u32)address, actualValue, expectedValue, passFail(actualValue == expectedValue));
    }
}

void unitTestCrcCheck(const void * pStart, u32 lengthBytes, u32 expectedCrc)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_CRC_VERIFY,(u32)pStart,lengthBytes,expectedCrc,0);
    }
    else
    {
        convertAddrIfCacheBypass((void**)&pStart);

        const u32 CRC32_POLY = 0x04C11DB7;
        u32 crc32 = 0xffffffff;
        u32 temp = 0;
        u32 i;
        u32 bitNum;
        for (i=0;i<lengthBytes;i++)
        {
            u8 current_byte = GET_REG_BYTE_VAL(((int)pStart) + i);
            for (bitNum = 0; bitNum < 8; bitNum++)
            {
                int msb = (crc32 >> 31) & 1;
                crc32 = crc32 << 1;
                if (msb != ((current_byte >> bitNum) & 1))
                {
                    crc32 ^= CRC32_POLY;
                    crc32 |= 1;
                }
            }
        }
        for (bitNum = 0; bitNum < 32; bitNum++)
        {
            if ((crc32 >> bitNum) & 1)
            {
                temp |= 1 << (31 - bitNum);
            }
        }
        crc32 = ~temp;
        prefixed_printf("unitTestCrcCheck() : (addr:0x%08x len:0x%lx actualCrc:%08lx expectedCrc: %08lx)  => %s\n", (int)pStart, lengthBytes, crc32, expectedCrc, passFail(crc32 == expectedCrc));
    }
}

void unitTestMemCompare(const void * pActualStart,const void * pExpectedStart, u32 lengthBytes)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_COMPARE,(u32)pActualStart,(u32)pExpectedStart,lengthBytes,0);
    }
    else
    {
        convertAddrIfCacheBypass((void**)&pActualStart);
        convertAddrIfCacheBypass((void**)&pExpectedStart);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
        int does_it_pass = !memcmp(pActualStart, pExpectedStart, lengthBytes);
#pragma GCC diagnostic ignored "-Wunused-function"
        prefixed_printf("unitTestMemCompare()    : (0x%08x vs 0x%08x l:0x%08x       => %s\n", (int)pActualStart, (int)pExpectedStart, (int)lengthBytes, passFail(does_it_pass));
    }
}




#ifdef LEON_RT
void lrt_unitTestSetTestType(tyTestType testType)
{
    unitTestSetTestType(testType);
}
#endif

void unitTestSetTestType(tyTestType testType)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_SET_TEST_TYPE,(u32)testType,0,0,0);
    }
    else
    {
        globalTestType = testType;
#ifdef LEON_RT
        lrt_globalTestType = testType;
#endif
        if (testType == EXPECT_TESTS_TO_FAIL)
        {
            prefixed_printf("- Negative Testing Enabled.. Tests Are Expected to FAIL! -----------------\n");
        }
        else if (testType == EXPECT_TESTS_TO_PASS)
        {
            prefixed_printf("- Positive Testing Enabled.. Tests Are Expected to PASS! -----------------\n");
        }
    }

}

void unitTestEnableCacheBypassMode(int enableCacheBypass)
{
    cacheBypass = enableCacheBypass;
}

void unitTestExecutionWithinRange(float actual, float expected, float percentageError)
{
    float minVal;
    float maxVal;

    //Early exit if we have negative numbers
    if (actual < 0.0f)
    {
        unitTestAssert(0); // Mark a fail if incorrectly used with negative numbers
        return;
    }

    minVal = expected * ((100.0 - percentageError) / 100.0);
    maxVal = expected * ((100.0 + percentageError) / 100.0);

    unitTestAssert( ((actual > minVal) && (actual < maxVal)) || (actual==0.0f) );
}

#ifdef LEON_RT
u32 lrt_unitTestFinalReport(void)
{
   return unitTestFinalReport();
}
#endif

u32 unitTestFinalReport(void)
{
    u32 numFailsResult=999; // Default to assuming failure just in case other code paths don't get exercised.

#if defined(EXPECT_FREQ_MHZ) && !defined(DISABLE_FREQ_CHECK)
    u32 expectedFreqKhz;
    u32 actualFreqKhz;

    #ifdef __RTEMS__
    actualFreqKhz = OsDrvCprGetSysClockKhz();
    #else
    actualFreqKhz = DrvCprGetSysClockKhz();
    #endif

    // Define passed with -D with no value is defaulted to 1
    if (EXPECT_FREQ_MHZ != 1)
    {
        expectedFreqKhz = EXPECT_FREQ_MHZ * 1000;
    }
    else
    {
#if BUP_MCLK_DIV_DEN
        if (BUP_MCLK_DIV_NUM && (BUP_MCLK_DIV_NUM != BUP_MCLK_DIV_DEN))
            expectedFreqKhz = BUP_PLL0_FREQ * BUP_MCLK_DIV_NUM / BUP_MCLK_DIV_DEN;
        else
#else
            expectedFreqKhz = BUP_PLL0_FREQ;
#endif
    }
    unitTestCompare(actualFreqKhz, expectedFreqKhz);
#endif

    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_DISPLAY_RESULT,0,0,0,0);
    }
    else
    {
#ifdef LEON_RT
     if (numFail + lrt_numFail != 0)
#else
   	  if (numFail != 0)
#endif
        {
#ifdef LEON_RT
            prefixed_printf("moviUnitTest:FAILED %lu + %lu error(s)\n", numFail, lrt_numFail);
            numFailsResult = numFail + lrt_numFail;

#else
            prefixed_printf("moviUnitTest:FAILED %lu error(s)\n", numFail);
            numFailsResult = numFail;
#endif
        }
        else
        {
            prefixed_printf("moviUnitTest:PASSED\n");
            numFailsResult = 0;
        }
    }

#ifdef PROD_TEST_DLT_ENABLE
    DltUtilsSignalEvent(DLT_EVENT_SIGNAL_RESULT,numFailsResult);
#endif

    // Some testbenches make use of this global flag to signal the end of
    // test
    unitTestTestComplete = TRUE;
    unitTestNumFailures  = numFailsResult;

    // Write also to the uncacheable address. In case L2 is enabled in copy-back mode,
    // this will ensure that the test script reads the updated value
    *(volatile u32*)DrvRegGetUncacheableAddr((void*)&unitTestTestComplete) = TRUE;
    *(volatile u32*)DrvRegGetUncacheableAddr((void*)&unitTestNumFailures)  = numFailsResult;

    // Note: If this isn't called with VCSHOOKS_USE_SOFTWARE (i.e. vcs mode) then this function will always return a fail
    // i.e. You are misusing the API if you care about the result when run in VCS mode as this is not possible.
    // Previously we defaulted to zero in this case, but it ran the risk of people assuming test passed even though
    // it was run in a way that was not valid.
    return numFailsResult;
}


