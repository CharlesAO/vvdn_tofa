
//Parameter description:
// mvcvMinKernel64 	- computes aggregated cost over a given path considering min2 and penaltyP2 previously computed
// @param[in] input1			- pointer to previous aggregated costs
// @param[in] input2			- pointer to current matching costs
// @param[out] output         	- pointer to output aggregated cost
// @param[in] min2          	- minimum value from the previous aggregated costs
// @param[in] penaltyP2       	- penalty value for the case where minimum cost is found at more than +/-1 disparity
// @return    Nothing



#include "gtest/gtest.h"
#include <minKernel64.h>
#include "minKernel64_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define WIDTH 64

class mvcvMinKernel64KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *input;
	unsigned char *minC;
	unsigned char *minAsm;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvMinKernel64KernelTest, TestAllRandomValues)
{

	inputGen.SelectGenerator("uniform");
	minC   = inputGen.GetEmptyLine(1);
    minAsm = inputGen.GetEmptyLine(1);
	
	inputGen.SelectGenerator("random");
	input = inputGen.GetLine((WIDTH), 0, 255);
	
	mvcvMinKernel64(input, minC);
	mvcvMinKernel64_asm_test(input, minAsm);

	RecordProperty("CyclePerPixel", mvcvMinKernel64CycleCount);

	outputChecker.CompareArrays(minC, minAsm, 1);
	
}
