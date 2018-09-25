
//Parameter description:
// mvcvAggregateCostSGBM64 	- computes aggregated cost over a given path considering min2 and penaltyP2 previously computed
// @param[in] input1			- pointer to previous aggregated costs
// @param[in] input2			- pointer to current matching costs
// @param[out] output         	- pointer to output aggregated cost
// @param[in] min2          	- minimum value from the previous aggregated costs
// @param[in] penaltyP2       	- penalty value for the case where minimum cost is found at more than +/-1 disparity
// @return    Nothing



#include "gtest/gtest.h"
#include <aggregateCostSGBM64.h>
#include "aggregateCostSGBM64_asm_test.h"
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

class mvcvAggregateCostSGBM64KernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char penaltyP2;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvAggregateCostSGBM64KernelTest, TestAll0)
{
	min2 = 0;
	penaltyP2 = 0;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
	
	
	input1 = inputGen.GetLine((WIDTH), 0);
	input2 = inputGen.GetLine((WIDTH), 0);
	
	mvcvAggregateCostSGBM64(input1, input2, outputC + OFFSET, min2, penaltyP2);
	mvcvAggregateCostSGBM64_asm_test(input1, input2, outputAsm, min2, penaltyP2);
	
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64CycleCount / (WIDTH ));

	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
}

TEST_F(mvcvAggregateCostSGBM64KernelTest, TestAllMaxValues)
{
	min2 = 101; //max value for this variable
	penaltyP2 = 154; //max value for this variable
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
	
	
	input1 = inputGen.GetLine((WIDTH), 245); //245 comes from the max(u8) values minus 10 from the .c code
	input2 = inputGen.GetLine((WIDTH), 245);
	
	mvcvAggregateCostSGBM64(input1, input2, outputC + OFFSET, min2, penaltyP2);
	mvcvAggregateCostSGBM64_asm_test(input1, input2, outputAsm, min2, penaltyP2);
	
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64CycleCount / (WIDTH ));

	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
}

TEST_F(mvcvAggregateCostSGBM64KernelTest, TestAllRandomValues)
{

	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(WIDTH + PADDING);
    outputAsm = inputGen.GetEmptyLine(WIDTH + PADDING);
	inputGen.FillLine(outputC,  WIDTH + PADDING , 5);
	inputGen.FillLine(outputAsm,WIDTH + PADDING , 5);
	
	inputGen.SelectGenerator("random");
	min2 = randGen->GenerateUInt(0, 101, 1); //max value for this variable
	penaltyP2 = randGen->GenerateUInt(0, 154, 1); //max value for this variable
	input1 = inputGen.GetLine((WIDTH), 0, 245); //245 comes from the max(u8) values minus 10 from the .c code
	input2 = inputGen.GetLine((WIDTH), 0, 245);
	
	mvcvAggregateCostSGBM64(input1, input2, outputC + OFFSET, min2, penaltyP2);
	mvcvAggregateCostSGBM64_asm_test(input1, input2, outputAsm, min2, penaltyP2);

	
	
	RecordProperty("CyclePerPixel", mvcvAggregateCostSGBM64CycleCount / (WIDTH ));

	outputChecker.CompareArrays(outputC, outputAsm, WIDTH + PADDING);
	/* printf("min2 = %d\n", min2);
	printf("penaltyP2 = %d\n", penaltyP2);
	outputChecker.DumpArrays("input1", "input1.c", input1, WIDTH);
	outputChecker.DumpArrays("input2", "input2.c", input2, WIDTH);
	outputChecker.DumpArrays("outputC", "outputC.c", outputC, WIDTH + PADDING);
	outputChecker.DumpArrays("outputAsm", "outputAsm.c", outputAsm, WIDTH + PADDING);
	exit(0); */
	
}
