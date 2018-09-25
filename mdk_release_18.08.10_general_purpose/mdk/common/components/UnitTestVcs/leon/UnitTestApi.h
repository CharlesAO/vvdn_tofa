///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup UnitTestVcsApi VCS Unit Test API
/// @{
/// @brief     Unit Test API for VCS
///
/// Used for test reporting
///

#ifndef _UNIT_TEST_API_H
#define _UNIT_TEST_API_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "UnitTestDefines.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// @brief Initialise the Unit Test Framework
/// @note This function must be called before any other call in this API is valid
/// @return  void
///
void unitTestInit(void);

/// @brief Set expected verbosity of the unitTest library
///
/// @param[in] targetVerbosity - either VERBOSITY_SILENT,VERBOSITY_QUIET,VERBOSITY_DIFFS,VERBOSITY_ALL    
/// @return  void
///
void unitTestVerbosity(tyUnitVerbosity targetVerbosity);

/// @brief Assert that value passed is TRUE (non-zero)
/// @param[in] value - Error logged if value == 0 or contains xx's
/// @return   void
///
void unitTestAssert(int value);

/// @brief Compare 2 32 bit values, log error if they don't match
/// @param[in] actualValue   - actual value to be tested
/// @param[in] expectedValue - value for comparison
/// @return  void
///
void unitTestCompare(u32 actualValue, u32 expectedValue);


/// @brief Compare 2 64 bit values, log error if they don't match
/// @param[in] actualValue   - actual value to be tested
/// @param[in] expectedValue - value for comparison
/// @return  void
///
void unitTestCompare64(u64 actualValue, u64 expectedValue);

/// @brief Read a dword from memory and compare against expected value. 
/// Log a failure if values don't match
/// @note The Leon performs the actual READ
/// @param[in] dWordAddress  -  Address of dword to read
/// @param[in] expectedValue - 64 bit value to compare against
/// @return  void
///
void unitTestReadDWordCheck(void * dWordAddress, u64 expectedValue);


/// @brief Read a word from memory and compare against expected value. 
/// Log a failure if values don't match
/// @note The Leon performs the actual READ
/// @param[in] wordAddress   - Address of word to read
/// @param[in] expectedValue - 32 bit value to compare against
/// @return  void
///
void unitTestReadWordCheck(void * wordAddress, u32 expectedValue);

/// @brief Read a 16 bit value from memory and compare against expected value. 
/// Log a failure if values don't match
/// @note The Leon performs the actual READ
/// @param[in] address       - Address of word to read
/// @param[in] expectedValue - 16 bit value to compare against
/// @return  void
///
void unitTestReadHalfCheck(void * address, u16 expectedValue);

/// @brief Read a byte from memory and compare against expected value. 
/// Log a failure if values don't match
/// @note The Leon performs the actual READ
/// @param[in] address       - Address of word to read
/// @param[in] expectedValue - 8 bit value to compare against
/// @return  void
///
void unitTestReadByteCheck(void * address, u8 expectedValue);

/// @brief Read a word from memory and compare a number of bits from the value
/// against an expected result. Log a failure if values don't match
/// @note The Leon performs the actual READ
/// @param[in] wordAddress   - Address of word to read
/// @param[in] startBit      - First bit to compare against (0-31)
/// @param[in] numBits       - number of bits to compare
/// @param[in] expectedValue - numBits value to compare against [(numBits-1):0]
/// @return  void
///
void unitTestReadBitCheck(void * wordAddress, u32 startBit, u32 numBits, u32 expectedValue);

/// @brief Compare two memory buffers for a given number of bytes
///
/// Both pointers may be byte aligned and there are no limitations of lengthBytes
/// This is a high speed hardware accelerated compare (takes negligible sim time even for multiMB buffers)
/// The test will also fail if any xx's are found in either buffer.
/// note This function can also be used to compare words, short etc as the system is all LE
/// @note This function performs a raw read of the actual target memory!
///        As such, it is the responsibility of the caller to ensure cache coherency (i.e. flush before use)
/// @param[in] pActualStart   - Start  Pointer to buffer which should be checked
/// @param[in] pExpectedStart - Start  Pointer to buffer for comparison
/// @param[in] lengthBytes    - Number of bytes to compare
/// @return   void
///
void unitTestMemCompare(const void * pActualStart,const void * pExpectedStart, u32 lengthBytes);

/// @brief Perform 32 bit CRC on the buffer of lengthBytes and compare against expectedCrc
///
/// The buffer may be byte aligned and there are no limitations on lengthBytes
/// This is a high speed hardware accelerated CRC (takes negligible sim time even for multiMB buffers)
/// The test will also fail if any xx's are found in the buffer.
/// @note This function performs a raw read of the actual target memory!
///        As such, it is the responsibility of the caller to ensure cache coherency (i.e. flush before use)
/// @param[in] pStart 	   - Start Pointer to buffer which should be checked
/// @param[in] expectedCrc - Start Pointer to buffer for CRC comparison
/// @param[in] lengthBytes - Number of bytes to compare
/// @return  void
///
void unitTestCrcCheck(const void * pStart, u32 lengthBytes, u32 expectedCrc);

/// @brief Checks if a floating point value is within an acceptable margin
/// expected value
/// @note unitTestExecutionWithinRange(actual,100, 10) passes if actual >90 and less than 110
/// @param[in] actual          - actual execution time
/// @param[in] expected        - expected execution time
/// @param[in] percentageError - Accepted execution time error
/// @return
///
void unitTestExecutionWithinRange(float actual, float expected, float percentageError);

/// @brief Signal unit test Framework that testing is complete and generate report
/// @attention This function MUST be called at the end of testing!
/// 
/// @note This API supports returning the number of failures found
/// but this is only valid when the API is used in "software mode"
/// i.e. Where M2_SW_UNIT_TEST=yes makefile flag is specified
/// In other cases the test system has no way to report back the number of 
/// failures and defaults to returning a fail
///
/// @return numFailures
///
u32 unitTestFinalReport(void);

/// @brief Set Test Type for either Positive (default) or Negative testing
///
/// The normal default mode is that we expect all tests to PASS
/// however this function allows us to invert that setting in the test logic and
/// declare that we expect the following tests to FAIL
/// While the testType == EXPECT_TESTS_TO_FAIL any tests that fail are effectively marked as passing
/// and any tests that pass are effectively marked as failing
/// @param[in] testType - (EXPECT_TESTS_TO_PASS,EXPECT_TESTS_TO_FAIL)
/// @return  void
///
void unitTestSetTestType(tyTestType testType);

/// @brief Set memory access mode to bypass the caches
///
/// When running in "software mode" (built with M2_SW_UNIT_TEST=yes), this can enable
/// unitTestRead*/unitTestMemCompare/unitTestCrcCheck to bypass Leon caches for read
/// operations. Intended for tests using HW engines for writing data (DMA/SIPP/etc)
/// - cache invalidation before compare no longer needed (operation otherwise required
/// even if Leon didn't touch the buffers, when sharing a cache line with other data).
/// @param[in] enableCacheBypass - TRUE (non-zero) for cache-bypassed accesses
/// @return  void
///
void unitTestEnableCacheBypassMode(int enableCacheBypass);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Backwards compatibilty API.. Not recommended ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// @brief  Log a Pass in the Unit test framework
///
/// This function call is provided for backwards compatiblity
/// It allows for software tests which do their own checking.
/// However it is strongly recommended to avoid this method
/// as it does not allow for checking for xx's etc.
///
void unitTestLogPass(void);

/// @brief  Log a Fail in the Unit test framework
///
/// This function call is provided for backwards compatiblity
/// It allows for software tests which do their own checking.
/// However it is strongly recommended to avoid this method
/// as it does not allow for checking for xx's etc.
///
void unitTestLogFail(void);
/// @}
#ifdef __cplusplus
}
#endif


#endif // _UNIT_TEST_API_H
