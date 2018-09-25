//boxFilter kernel test
//Asm function prototype:
//     void scaleFp16_asm( half** in, half** out, u32 width)
//
//Asm test function prototype:
//     void scaleFp16_asm_test(half** in, half** out, u32 width)
//
//C function prototype:
//     void scaleFp16(half** in, half** out, u32 width)
//
//Parameter description:
// This kernel performs 1 maximum line from 3 input lines
// @param[in] in        - 3 Input lines      
// @param[out] out      - 1 Output line    
// @param[in] width     - Width of input line


#include "gtest/gtest.h"
///
#include "scaleFp16.h"
#include "scaleFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define EXCEEDED_BOUNDS 16
half DELTA = 0.01f;

class scaleFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half **outLinesC;
	half **outLinesAsm;
	half** zeros;
	unsigned int width;
	half scale;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


// ------------- normalized variant -------------------
 TEST_F(scaleFp16KernelTest, TestUniformInputLines32)
{
	width = 32;
	scale = randGen->GenerateFloat16(0.01, 255.0);
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 1, 0);
	inputGen.FillLine(inLines[0], width, 9.0f);

	outLinesC   = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS, 1, 0.0f);
    zeros       = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);

	mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);
	
	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
    outputChecker.CompareArrays(zeros[0], outLinesAsm[0] + width, EXCEEDED_BOUNDS);
}

  TEST_F(scaleFp16KernelTest, TestAllValuesZero)
{
	width = 320;
	scale = randGen->GenerateFloat16(0.01, 255.0);
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 1, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
    outLinesAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS, 1, 0.0f);
    zeros       = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);

	mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);

	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
    outputChecker.CompareArrays(zeros[0], outLinesAsm[0] + width, EXCEEDED_BOUNDS);
}

TEST_F(scaleFp16KernelTest, TestAllValues255)
{
	width = 1280;
	scale = randGen->GenerateFloat16(0.01, 255.0);
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 1, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
    outLinesAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS, 1, 0.0f);
    zeros       = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);

	mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);

	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
    outputChecker.CompareArrays(zeros[0], outLinesAsm[0] + width, EXCEEDED_BOUNDS);
}

TEST_F(scaleFp16KernelTest, TestMinimumWidthSize)
{
	width = 4;
	scale = randGen->GenerateFloat16(0.01, 255.0);
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 1, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
    outLinesAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS, 1, 0.0f);
    zeros       = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);

	mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);

	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
    outputChecker.CompareArrays(zeros[0], outLinesAsm[0] + width, EXCEEDED_BOUNDS);
}

TEST_F(scaleFp16KernelTest, TestRandomWidthRandomData)
{
	width = randGen->GenerateUInt(8, 1921, 4);
	width = (width >> 2)<<2; //(line width have to be mutiple of 4)
	scale = randGen->GenerateFloat16(0.01, 255.0);
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 1, 0, 255);

    inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
    outLinesAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS, 1, 0.0f);
    zeros       = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);

    mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);

	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
    outputChecker.CompareArrays(zeros[0], outLinesAsm[0] + width, EXCEEDED_BOUNDS);
}

//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputsUnnormalized, scaleFp16KernelTest,
           Values((unsigned int)16, (unsigned int)128, (unsigned int)512, (unsigned int)1024);
);

TEST_P(scaleFp16KernelTest, TestRandomData)
{
	width = GetParam();
	scale = randGen->GenerateFloat16(0.01, 255.0);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 1, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvScaleFp16(inLines, outLinesC, scale, width);
	scaleFp16_asm_test(inLines, outLinesAsm, scale, width);

	RecordProperty("CyclePerPixel", scaleFp16CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}
