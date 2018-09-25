///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup UnitTestApi Unit Test API
/// @ingroup  UnitTestApi
/// @{
/// @brief     Unit Test API
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
/// @brief Initiate a new unit test
///
int unitTestInit(void);

/// @brief Checks if a floating point value is within a certain percentage of
/// expected value
/// @note actual+1 is considered 100% deviation, actual is 0% deviation
/// @param[in] actual          - actual floating point value
/// @param[in] expected        - expected floating point value
/// @param[in] percentageError - acceptable percent deviation as explained above
/// @return 0
///
int unitTestFloatWithinRange(float actual, float expected, float percentageError);

/// @brief Checks if a floating point value is within an acceptable margin
/// expected value
/// @note unitTestExecutionWithinRange(actual,100, 10) passes if actual >90 and less than 110
/// @param[in] actual          - actual execution time
/// @param[in] expected        - expected execution time
/// @param[in] percentageError - Accepted execution time error
/// @return 0
///
int unitTestExecutionWithinRange(float actual, float expected, float percentageError);

/// @brief Checks if a floating point value is within an acceptable margin
/// expected value
/// @param[in] actual   - existing float
/// @param[in] expected - expected float
/// @param[in] AbsError - accepted absolute error
/// @return 0
///
int unitTestFloatAbsRangeCheck(float actual, float expected, float AbsError);

/// @brief Check if a logical condition is true or not
/// @param[in] value - the value to be checked
/// @return 0
///
int unitTestAssert(int value);

/// @brief Log a passed test
/// @return 0
///
int unitTestLogPass(void);

/// @brief Log a failed test
/// @return 0
///
int unitTestLogFail(void);

/// @brief Set up the passed and failed tests results
/// @param[in] passes - number of passed tests
/// @param[in] fails  - number of failed tests
/// @return 0
///
int unitTestLogResults(int passes,int fails);

/// @brief Compares two values
/// @param[in] pActualStart    - Pointer to the actual value
/// @param[in] pExpectedStart  - Pointer to the expected value
/// @param[in] lengthBytes     - Value length in bytes
/// @return void
///
void unitTestMemCompare(const void * pActualStart,const void * pExpectedStart, u32 lengthBytes);

/// @brief Compares two values, with threshold
/// @param[in] pActualStart    - Pointer to the actual value
/// @param[in] pExpectedStart  - Pointer to the expected value
/// @param[in] lengthBytes     - Value length in bytes
/// @param[in] delta           - threshold
/// @return void
///
void unitTestMemCompareDeltaU8(u8 * pActualStart,u8 * pExpectedStart, u32 lengthBytes, u8 delta);

/// @@brief Compares two values
/// @param[in] actualValue   - Actual value
/// @param[in] expectedValue - Expected value
/// @return void
///
void unitTestCompare(u32 actualValue, u32 expectedValue);

/// @@brief Check if there has been any failures in the last test section
///
/// The first section starts at the beginning of the test. When this function is
/// called, it checks if there has been any failures in the previous tests and
/// switches to a new a section. Any call of this function will only report
/// failures in the tests run since the previous call of this function.
///
/// This functions prints a message if any failure has occurred, indicating the
/// name of the section (passed as a parameter) and the number of failures. If
/// the user does not want this message to be printed, set sectionName to NULL.
///
/// @param[in] section_name - the name of the test section being checked
///            /!\ This string has to be '\0' terminated.
///            If set to NULL, no error message is printed.
///
/// @return the number of tests failed in the last section
///
int unitTestCheckSectionFail(char *sectionName);

/// @brief Print the final results of the unit testing
/// @return 0
///
int unitTestFinalReport(void);
/// @}
#ifdef __cplusplus
}
#endif


#endif // _UNIT_TEST_API_H
