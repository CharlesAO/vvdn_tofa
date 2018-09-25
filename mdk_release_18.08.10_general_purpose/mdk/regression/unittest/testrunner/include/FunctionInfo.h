#ifndef __FUNCTION_INFO_CLASS__
#define __FUNCTION_INFO_CLASS__

#include <numeric>
#include <memory>
#include <vector>
#include "gtest/gtest.h"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

class FunctionInfo
{
public:
	static FunctionInfo& Instance()
	{
		if( instance.get() == 0 )
		{
			instance.reset( new FunctionInfo() );
		}
		return *instance;
	}

	void AddCyclePerPixelInfo(float value)
	{
		cyclePerPixel.push_back(value);
	}

	void ResetCyclePerPixelInfo()
	{
		cyclePerPixel.clear();
	}

	float GetAverageCyclesPerPixel()
	{
		return accumulate(cyclePerPixel.begin(), cyclePerPixel.end(), 0.0) / (float)cyclePerPixel.size();
	}

	static unsigned int getRandSeed()
	{
		return randSeed;
	}

	static unsigned int getUserSeed()
	{
		return useUserSeed;
	}

	static void setRandSeed(unsigned int userSeed)
	{
		randSeed = userSeed;
	}

	static void setUserSeed(unsigned int userSeed)
	{
		useUserSeed = userSeed;
	}
	static void setExpectedCycles(float defCC)
	{
		expectedCycles = defCC;
	}
	static float getExpectedCycles(void)
	{
		return expectedCycles;
	}

private:
	FunctionInfo();
//	FunctionInfo(FunctionInfo const&);            // Don't Implement
	void operator=(FunctionInfo const&); // Don't implement

	std::vector<float> cyclePerPixel;
	static unsigned int useUserSeed;
	static unsigned int randSeed;
	static float expectedCycles;
	static std::auto_ptr<FunctionInfo> instance;
};



#endif //__FUNCTION_INFO_CLASS__
