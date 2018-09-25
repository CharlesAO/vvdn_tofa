
//Parameter description:
// mvcvAggregateCostSGBM64clampkernel 	- computes aggregated cost over a given path considering min2, penaltyP1 and penaltyP2 previously computed
// @param[in] input1					- pointer to previous aggregated costs with SGBM
// @param[in] input2					- pointer to current disparity costs
// @param[out] output         			- pointer to aggregated cost computed from previous aggregated and current disparity costs
// @param[in] min2          			- minimum cost from previous aggregated costs
// @param[in] penaltyP1       			- penalty applied when minimum cost is located at +/-1 disparity
// @param[in] penaltyP2       			- penalty applied when minimum cost is located at more than +/-1 disparity
// @param[in] activateClamp    			- clamp or not u32 values to u8 
// @return    Nothing



#include "gtest/gtest.h"
#include <aggregateCostSGBM64_clamp.h>
#include "aggregateCostSGBM64_clamp_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define WIDTH 64
#define OFFSET PADDING/2

class mvcvAggregateCostSGBM64_clampKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char min2;
	unsigned short penaltyP1;
	unsigned short penaltyP2;
	unsigned char activateClamp;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};
TEST_F(mvcvAggregateCostSGBM64_clampKernelTest, TestAll0)
{
	penaltyP1 = 0;
	penaltyP2 = 0;
	min2 = 0;
	activateClamp = 0;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
		
	input1 = inputGen.GetLine((WIDTH), 0);
	input2 = inputGen.GetLine((WIDTH), 0);
	
	mvcvAggregateCostSGBM64_clamp(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, min2, activateClamp);
	mvcvAggregateCostSGBM64_clamp_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, min2, activateClamp);
	
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64clampCycleCount / (WIDTH ));
	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
}

TEST_F(mvcvAggregateCostSGBM64_clampKernelTest, TestAllBigValues)
{
	penaltyP1 = 2000; 
	penaltyP2 = 2000; 
	min2 = 255; 
	activateClamp = 1;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
	
	
	input1 = inputGen.GetLine((WIDTH), 255); 
	input2 = inputGen.GetLine((WIDTH), 255);
	
	mvcvAggregateCostSGBM64_clamp(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, min2, activateClamp);
	mvcvAggregateCostSGBM64_clamp_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, min2, activateClamp);
	
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64clampCycleCount / (WIDTH ));
	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
}


TEST_F(mvcvAggregateCostSGBM64_clampKernelTest, TestAllRandomValues)
{
		
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
	
	inputGen.SelectGenerator("random");
	activateClamp = randGen->GenerateUInt(0, 1, 1);
	min2 = randGen->GenerateUInt(0, 101, 1); 
	penaltyP2 = randGen->GenerateUInt(0, 154, 1);
	penaltyP1 = randGen->GenerateUInt(0, 154, 1); 
	input1 = inputGen.GetLine((WIDTH), 0, 255); 
	input2 = inputGen.GetLine((WIDTH), 0, 255);
	
	
	mvcvAggregateCostSGBM64_clamp(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, min2, activateClamp);
	mvcvAggregateCostSGBM64_clamp_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, min2, activateClamp);
	
    
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64clampCycleCount / (WIDTH ));
	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
}

