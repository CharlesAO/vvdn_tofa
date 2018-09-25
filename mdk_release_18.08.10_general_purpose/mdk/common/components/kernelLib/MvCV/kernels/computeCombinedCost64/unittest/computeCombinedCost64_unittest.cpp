
//Parameter description:
// mvcvComputeCombinedCost64 	- computes average sum from 4 paths (width x 64 arrays)
// @param[in] path0			- pointer to a width x 64 array from path0
// @param[in] path1			- pointer to a width x 64 array from path1
// @param[in] path2			- pointer to a width x 64 array from path2
// @param[in] path3			- pointer to a width x 64 array from path3
// @param[out] output         	- pointer to output result
// @param[in] width          	- line width
// @return    Nothing


#include "gtest/gtest.h"
#include "computeCombinedCost64.h"
#include "computeCombinedCost64_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DISPARITIES 64
#define DELTA 1

class mvcvComputeCombinedCost64KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *disparityCostC;
	unsigned char *disparityCostAsm;
	unsigned char *adCost;

	unsigned int alpha;
	unsigned int beta;
	unsigned int width;
	unsigned int normFactor;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvComputeCombinedCost64KernelTest, TestAll0)
{
	width = 320;
	
	inputGen.SelectGenerator("random");
	alpha  = randGen->GenerateUInt(1, 5, 1);
	beta  = randGen->GenerateUInt(1, 5, 1);
	normFactor = randGen->GenerateUInt(1, 5, 1);
	
	inputGen.SelectGenerator("uniform");
	
	disparityCostC = inputGen.GetLine((width * DISPARITIES), 0);
	disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 0);
	
	memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
	
	adCost = inputGen.GetLine((width * DISPARITIES), 0);

	
	mvcvComputeCombinedCost64(disparityCostC, adCost, alpha, beta, width, normFactor);
	mvcvComputeCombinedCost64_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
	
	RecordProperty("CyclePerPixel", mvcvComputeCombinedCost64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

TEST_F(mvcvComputeCombinedCost64KernelTest, TestAllMaxValues)
{
	width = 320;
	
	inputGen.SelectGenerator("random");
	alpha  = randGen->GenerateUInt(1, 5, 1);
	beta  = randGen->GenerateUInt(1, 5, 1);
	normFactor = randGen->GenerateUInt(1, 5, 1);
	
	inputGen.SelectGenerator("uniform");
	
	disparityCostC = inputGen.GetLine((width * DISPARITIES), 255);
	disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 255);
	
	memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
	
	adCost = inputGen.GetLine((width * DISPARITIES), 255);

	
	mvcvComputeCombinedCost64(disparityCostC, adCost, alpha, beta, width, normFactor);
	mvcvComputeCombinedCost64_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
	
	RecordProperty("CyclePerPixel", mvcvComputeCombinedCost64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

TEST_F(mvcvComputeCombinedCost64KernelTest, TestAllRandomValues)
{
	width = 320;
	
	inputGen.SelectGenerator("random");
	alpha  = randGen->GenerateUInt(1, 5, 1);
	beta  = randGen->GenerateUInt(1, 5, 1);
	normFactor = randGen->GenerateUInt(1, 5, 1);
	
	
	inputGen.SelectGenerator("random");
	
	disparityCostC = inputGen.GetLine((width * DISPARITIES), 0, 25);
	disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 0, 255);
	
	memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
	
	adCost = inputGen.GetLine((width * DISPARITIES), 0, 255);

	
	mvcvComputeCombinedCost64(disparityCostC, adCost, alpha, beta, width, normFactor);
	mvcvComputeCombinedCost64_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
	
	RecordProperty("CyclePerPixel", mvcvComputeCombinedCost64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

 