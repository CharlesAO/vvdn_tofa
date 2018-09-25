
//Parameter description:
// mvcvCensusMatching 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <censusMatching65.h>
#include "censusMatching65_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 65

class mvcvCensusMatching65KernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(mvcvCensusMatching65KernelTest, TestWidth16All0)
{
    disparities = 65;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching65(inLine1 + PADDING, inLine2 + PADDING, outLineC + 16, width);
	mvcvCensusMatching65_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching65CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching65KernelTest, TestWidth16AllMAX)
{
    disparities = 65;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching65(inLine1 + PADDING, inLine2 + PADDING, outLineC + 16, width);
	mvcvCensusMatching65_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching65CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching65KernelTest, TestRandomDATARandomWidth)
{
    disparities = 65;
	width = randGen->GenerateUInt(8, 1000, 8); //max width allowed to not overwrite the stack
	
	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching65(inLine1 + PADDING, inLine2 + PADDING, outLineC + 16, width);
	mvcvCensusMatching65_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching65CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatching65KernelTest, TestRandomDATARandomWidth2)
{
    disparities = 65;
	width = randGen->GenerateUInt(960, 1000, 8); //max width allowed to not overwrite the stack

	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatching65(inLine1 + PADDING, inLine2 + PADDING, outLineC + 16, width);
	mvcvCensusMatching65_asm_test(inLine1 , inLine2 , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatching65CycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}
