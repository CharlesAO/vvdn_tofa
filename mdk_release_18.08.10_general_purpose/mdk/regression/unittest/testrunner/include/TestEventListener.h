#ifndef TEST_EVENT_LISTENER_
#define TEST_EVENT_LISTENER_

#include <stdio.h>

#include "gtest/gtest.h"
#include "FunctionInfo.h"
#include "internalDebug.h"

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestCase;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

namespace {

// Provides alternative output mode which produces minimal amount of
// information about tests.
class TestEventListener : public EmptyTestEventListener {
 private:
  // Called before any test activity starts.
  virtual void OnTestProgramStart(const UnitTest& /* unit_test */) {}

  // Called after all test activities have ended.
  virtual void OnTestProgramEnd(const UnitTest& /*unit_test*/)
  {
	FunctionInfo info = FunctionInfo::Instance();
	float cyclesPerPixel = info.GetAverageCyclesPerPixel();
	printf("Average measured cycles per pixel: %f\n", cyclesPerPixel);
	float expectedCyclesPerPixel = info.getExpectedCycles();
    if (0 == expectedCyclesPerPixel) {
      // this means that this number was not completed. Not necessary a check
    }
    else {
#ifdef INT_DDR
      // not assert fail, but inform. Normaly not have to increase cc/pixel, as both cache are on, and hope to not be very big impact, as code and datas are small
      if (cyclesPerPixel > (expectedCyclesPerPixel * (1+(float)TOLERANCE_MVCV_CC/100.0f))) {
          printf("The cycles per pixel increase. Expected value: %f, test FAIL \n", expectedCyclesPerPixel);
      }          
#else    
      if (cyclesPerPixel > (expectedCyclesPerPixel * (1+(float)TOLERANCE_MVCV_CC/100.0f))) {
          printf("The cycles per pixel increase. Expected value: %f, test FAIL \n", expectedCyclesPerPixel);
          FAIL();
      }
#endif      
    }
  }

  // Called before a test starts.
  virtual void OnTestStart(const TestInfo& /*test_info*/)
  {

  }

  // Called after a failed assertion or a SUCCEED() invocation.
  virtual void OnTestPartResult(const TestPartResult& test_part_result)
  {
	  // show random seed value if test fail
	  if (test_part_result.failed())
	  {
		  FunctionInfo info = FunctionInfo::Instance();
		  printf("seed used = %d \n", info.getRandSeed());
	  }
  }

  // Called after a test ends.
  virtual void OnTestEnd(const TestInfo& /*test_info*/)
  {
      SAVE_ELF_FILE;
  }

};  // class TestEventListener

}  // namespace

#endif
