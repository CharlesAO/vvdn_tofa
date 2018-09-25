
//Parameter description:
// mvcvCensusMatching 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <censusMatching16.h>
#include "censusMatching16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16

class mvcvCensusMatching16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned int *inLine1;
	unsigned int *inLine2;
	unsigned char *outLineC;
	unsigned char *outLineAsm;
	unsigned int width;
	unsigned int disparities;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvCensusMatching16KernelTest, TestWidth16All0)
{
    disparities = 16;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching16(inLine1 + PADDING, inLine2 + PADDING, outLineC + PADDING/2, width);
	mvcvCensusMatching16_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching16CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching16KernelTest, TestWidth16AllMAX)
{
    disparities = 16;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching16(inLine1 + PADDING, inLine2 + PADDING, outLineC + PADDING/2, width);
	mvcvCensusMatching16_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching16CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching16KernelTest, TestRandomDATARandomWidth)
{
    disparities = 16;
	width = randGen->GenerateUInt(8, 1280, 8);
	
	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching16(inLine1 + PADDING, inLine2 + PADDING, outLineC + PADDING/2, width);
	mvcvCensusMatching16_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching16CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching16KernelTest, TestRandomDATARandom1Width)
{
    disparities = 16;
	width = randGen->GenerateUInt(960, 1280, 8);
	
	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching16(inLine1 + PADDING, inLine2 + PADDING, outLineC + PADDING/2, width);
	mvcvCensusMatching16_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching16CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}