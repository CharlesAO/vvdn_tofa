
//Parameter description:
/// mvcvAggregateFivePaths64 	- computes average sum from 5 paths (width x 64 arrays)
/// @param[in] path0			- pointer to a width x 64 array from path0
/// @param[in] path1			- pointer to a width x 64 array from path1
/// @param[in] path2			- pointer to a width x 64 array from path2
/// @param[in] path3			- pointer to a width x 64 array from path3
/// @param[in] path4			- pointer to a width x 64 array from path4
/// @param[out] output         	- pointer to output result
/// @param[in] width          	- line width
/// @return    Nothing


#include "gtest/gtest.h"
#include <aggregateFivePaths64.h>
#include "aggregateFivePaths64_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define DISPARITIES 64
#define OFFSET PADDING/2

class mvcvAggregateFivePaths64KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *path0;
	unsigned char *path1;
	unsigned char *path2;
	unsigned char *path3;
	unsigned char *path4;
	unsigned char *outputAsm;
	unsigned char *outputC;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvAggregateFivePaths64KernelTest, Test0)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
        outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
	inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
	inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);
	
	path0 = inputGen.GetLine((width * DISPARITIES), 0);
	path1 = inputGen.GetLine((width * DISPARITIES), 0);
	path2 = inputGen.GetLine((width * DISPARITIES), 0);
	path3 = inputGen.GetLine((width * DISPARITIES), 0);
	path4 = inputGen.GetLine((width * DISPARITIES), 0);
	
	mvcvAggregateFivePaths64(path0, path1, path2, path3, path4, outputC + OFFSET, width);
	mvcvAggregateFivePaths64_asm_test(path0, path1, path2, path3, path4, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvAggregateFivePaths64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}

TEST_F(mvcvAggregateFivePaths64KernelTest, TestMaxValues)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
        outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
	inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
	inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);
	
	path0 = inputGen.GetLine((width * DISPARITIES), 255);
	path1 = inputGen.GetLine((width * DISPARITIES), 255);
	path2 = inputGen.GetLine((width * DISPARITIES), 255);
	path3 = inputGen.GetLine((width * DISPARITIES), 255);
	path4 = inputGen.GetLine((width * DISPARITIES), 255);
	
	mvcvAggregateFivePaths64(path0, path1, path2, path3, path4, outputC + OFFSET, width);
	mvcvAggregateFivePaths64_asm_test(path0, path1, path2, path3, path4, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvAggregateFivePaths64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}

TEST_F(mvcvAggregateFivePaths64KernelTest, TestRandomValues)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
        outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
	inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
	inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1, 320, 1);
	path0 = inputGen.GetLine((width * DISPARITIES), 0, 255);
	path1 = inputGen.GetLine((width * DISPARITIES), 0, 255);
	path2 = inputGen.GetLine((width * DISPARITIES), 0, 255);
	path3 = inputGen.GetLine((width * DISPARITIES), 0, 255);
	path4 = inputGen.GetLine((width * DISPARITIES), 0, 255);
	
	mvcvAggregateFivePaths64(path0, path1, path2, path3, path4, outputC + OFFSET, width);
	mvcvAggregateFivePaths64_asm_test(path0, path1, path2, path3, path4, outputAsm, width);
	
	RecordProperty("CyclePerPixel", mvcvAggregateFivePaths64CycleCount / (width * DISPARITIES ));

	outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}

