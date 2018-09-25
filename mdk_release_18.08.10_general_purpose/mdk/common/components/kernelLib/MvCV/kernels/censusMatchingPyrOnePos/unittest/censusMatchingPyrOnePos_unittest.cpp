
//Parameter description:
// mvcvCensusMatching 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[in] predicted      - pointer to predicted disparities 
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <censusMatchingPyrOnePos.h>
#include "censusMatchingPyrOnePos_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 32

class mvcvCensusMatchingPyrOnePosKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char *predicted;
	unsigned char *outLineAsm;
	unsigned int width;
	unsigned int disparities;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvCensusMatchingPyrOnePosKernelTest, TestWidth16All0)
{
    disparities = 3;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0);
	predicted = inputGen.GetLine((width + PADDING), 0);
	
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatchingPyrOnePos(inLine1 + PADDING, inLine2 + PADDING, predicted, outLineC + PADDING/2, width);
	mvcvCensusMatchingPyrOnePos_asm_test(inLine1 , inLine2, predicted , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatchingPyrOnePosCycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatchingPyrOnePosKernelTest, TestWidth16AllMAX)
{
    disparities = 3;
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 255);
	predicted = inputGen.GetLine((width + PADDING), 6);
	

	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatchingPyrOnePos(inLine1 + PADDING, inLine2 + PADDING, predicted, outLineC + PADDING/2, width);
	mvcvCensusMatchingPyrOnePos_asm_test(inLine1 , inLine2, predicted , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatchingPyrOnePosCycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatchingPyrOnePosKernelTest, TestRandomDATARandomWidth)
{
    disparities = 3;
	width = randGen->GenerateUInt(8, 1280, 8);
	//width = 16;
	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	predicted = inputGen.GetLine((width + PADDING), 0, 6);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatchingPyrOnePos(inLine1 + PADDING, inLine2 + PADDING, predicted, outLineC + PADDING/2, width);
	mvcvCensusMatchingPyrOnePos_asm_test(inLine1 , inLine2, predicted , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatchingPyrOnePosCycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("predicted", "predicted.c", predicted, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}

TEST_F(mvcvCensusMatchingPyrOnePosKernelTest, TestRandomDATARandom1Width)
{
    disparities = 3;
	width = randGen->GenerateUInt(960, 1280, 8);
	
	inputGen.SelectGenerator("random");
	inLine1 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	inLine2 = (unsigned int *)inputGen.GetLine((width + PADDING)*4, 0, 255);
	predicted = inputGen.GetLine((width + PADDING), 0, 6);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outLineC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outLineAsm,width * disparities + PADDING , 5);
	

	mvcvCensusMatchingPyrOnePos(inLine1 + PADDING, inLine2 + PADDING, predicted, outLineC + PADDING/2, width);
	mvcvCensusMatchingPyrOnePos_asm_test(inLine1 , inLine2, predicted , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", mvcvCensusMatchingPyrOnePosCycleCount / (width * disparities));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width * disparities + PADDING);
}