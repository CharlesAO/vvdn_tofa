
//Parameter description:
// mvcvComputeADPyrOnePos 	- computes aggregated cost over a given path considering min2 and penaltyP2 previously computed
// @param[in] input1			- pointer to previous aggregated costs
// @param[in] input2			- pointer to current matching costs
// @param[out] output         	- pointer to output aggregated cost
// @param[in] min2          	- minimum value from the previous aggregated costs
// @param[in] penaltyP2       	- penalty value for the case where minimum cost is found at more than +/-1 disparity
// @return    Nothing



#include "gtest/gtest.h"
#include <computeADPyrOnePos.h>
#include "computeADPyrOnePos_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define OFFSET PADDING/2

class mvcvComputeADPyrOnePosKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *input1;
	unsigned char *input2;
	unsigned char *outputAsm;
	unsigned char *outputC;
	unsigned int width;
	unsigned int disparities;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvComputeADPyrOnePosKernelTest, TestAll0)
{
	width = 320;
	disparities = 3;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	
	input1 = inputGen.GetLine((width * disparities + PADDING), 0);
	input2 = inputGen.GetLine((width * disparities + PADDING), 0);
	
	mvcvComputeADPyrOnePos(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
	mvcvComputeADPyrOnePos_asm_test(input1, input2, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosCycleCount / (width * disparities ));

	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvComputeADPyrOnePosKernelTest, TestAllMaxValues)
{
	width = 320;
	disparities = 3;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	
	input1 = inputGen.GetLine((width * disparities + PADDING), 255);
	input2 = inputGen.GetLine((width * disparities + PADDING), 255);
	
	mvcvComputeADPyrOnePos(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
	mvcvComputeADPyrOnePos_asm_test(input1, input2, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosCycleCount / (width * disparities ));

	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvComputeADPyrOnePosKernelTest, TestAllRandomValues)
{

	width = randGen->GenerateUInt(1280, 1921, 8);
	width = 16;
	disparities = 3;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	inputGen.SelectGenerator("random");
	input1 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
	input2 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
	
	mvcvComputeADPyrOnePos(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
	mvcvComputeADPyrOnePos_asm_test(input1, input2, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosCycleCount / (width * disparities ));

	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
} 
