#include "gtest/gtest.h"
#include <localTM.h>
#include "localTM_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"


using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 8
half DELTA = 0.01;//accepted tolerance between C and ASM results

class localTMKernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half curves[160];
	half** luma_in;
	unsigned char** bg8;
	half** outputC; 
	half** outputAsm; 
	unsigned int width;
	unsigned int run_no;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(localTMKernelTest, TestRandomDataBigWidth)
{
	width = randGen->GenerateUInt(1281, 1920, 8);
	int i;
	run_no = randGen->GenerateUInt(0, 1920, 1);
	unsigned char **bg8Offseted;
	inputGen.SelectGenerator("random");
	luma_in = inputGen.GetLinesFloat16(width, 1, 0.0, 1);
	bg8     = inputGen.GetLines(width/2 + PADDING * 2, 2, 0, 255);
	bg8Offseted = inputGen.GetOffsettedLines(bg8, 2 , PADDING);
	outputC   = inputGen.GetEmptyLinesFp16(width + 16, 1);
	outputAsm = inputGen.GetEmptyLinesFp16(width + 16, 1);

	for (i = 0; i<(int)(width + 16);i++)
	{
		outputC[0][i] = (half)1.0f;
		outputAsm[0][i] = (half)1.0f;
	}
	
	inputGen.FillLine(curves, 160, 0.0f, 1.0f);

	mvispLocalTM(luma_in, bg8Offseted, outputC, curves, width, run_no);
	localTM_asm_test(luma_in, bg8, outputAsm, curves, width, run_no);
	RecordProperty("CyclePerPixel", localTMCycleCount / width);
	

	
	for (i=0;i<(int)(width+16);i++)
	{	
		EXPECT_NEAR(outputC[0][i], outputAsm[0][i], DELTA) << "Line position: " << i << "/" << width
				<< ", Line 1 val: " << outputC[0][i]
				<< ", Line 2 val: " << outputAsm[0][i] << std::endl;;
	}
	
	//outputChecker.CompareArrays(outC, outAsm, width, DELTA);

	//outputChecker.DumpArrays("refInput", "refInputL.c", luma_in, width, 1);
	//outputChecker.DumpArrays("refInput", "refInputB.c", bg8 , width/2 + PADDING * 2, 2);
	//half* mat = curves;
	//half** mat1 = &mat;
	//outputChecker.DumpArrays("matInput", "matInput.c", mat1, 160 , 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outputC, width + 16 , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outputAsm, width +16, 1);
}

TEST_F(localTMKernelTest, TestRandomDataSmallWidth)
{
	width = randGen->GenerateUInt(0, 1280, 8);
	int i;
	run_no = randGen->GenerateUInt(0, 1920, 1);
	unsigned char **bg8Offseted;
	inputGen.SelectGenerator("random");
	luma_in = inputGen.GetLinesFloat16(width, 1, 0.0, 1);
	bg8     = inputGen.GetLines(width/2 + PADDING * 2, 2, 0, 255);
	bg8Offseted = inputGen.GetOffsettedLines(bg8, 2 , PADDING);
	outputC   = inputGen.GetEmptyLinesFp16(width + 16, 1);
	outputAsm = inputGen.GetEmptyLinesFp16(width + 16, 1);

	for (i = 0; i<(int)(width + 16);i++)
	{
		outputC[0][i] = (half)1.0f;
		outputAsm[0][i] = (half)1.0f;
	}
	
	inputGen.FillLine(curves, 160, 0.0f, 1.0f);

	mvispLocalTM(luma_in, bg8Offseted, outputC, curves, width, run_no);
	localTM_asm_test(luma_in, bg8, outputAsm, curves, width, run_no);
	RecordProperty("CyclePerPixel", localTMCycleCount / width);
	

	
	for (i=0;i<(int)(width+16);i++)
	{	
		EXPECT_NEAR(outputC[0][i], outputAsm[0][i], DELTA) << "Line position: " << i << "/" << width
				<< ", Line 1 val: " << outputC[0][i]
				<< ", Line 2 val: " << outputAsm[0][i] << std::endl;;
	}
	
}
