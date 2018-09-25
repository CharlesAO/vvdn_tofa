//boxFilter kernel test
//Asm function prototype:
//     void MaximumV2_asm( half** in, half** out, u32 width)
//
//Asm test function prototype:
//     void MaximumV2_asm_test(half** in, half** out, u32 width)
//
//C function prototype:
//     void MaximumV2(half** in, half** out, u32 width)
//
//Parameter description:
// This kernel performs 1 maximum line from 3 input lines
// @param[in] in        - 3 Input lines      
// @param[out] out      - 1 Output line    
// @param[in] width     - Width of input line


#include "gtest/gtest.h"
///
#include "maximumV2.h"
#include "maximumV2_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define NUM_INPUT_LINES 	2
using ::testing::TestWithParam;
using ::testing::Values;

half DELTA =0;

class MaximumV2KernelTest : public ::testing::TestWithParam< unsigned int > {
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
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


// ------------- normalized variant -------------------

 TEST_F(MaximumV2KernelTest, TestUniformInputLines32)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 0);
	inputGen.FillLine(inLines[0], width, 9.0f);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

  TEST_F(MaximumV2KernelTest, TestAllValuesZero)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(MaximumV2KernelTest, TestAllValues255)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(MaximumV2KernelTest, TestMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(MaximumV2KernelTest, TestSimpleNotEqualInputLines)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 1);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	for(int i = 1; i < 2; i++)
	{
		inputGen.FillLine(inLines[i], width, i + 1);
	}
	
	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(MaximumV2KernelTest, TestRandomWidthRandomData)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}



//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputsUnnormalized, MaximumV2KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(MaximumV2KernelTest, TestRandomData)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, NUM_INPUT_LINES, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvMaximumV2(inLines, outLinesC, width);
	MaximumV2_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV2CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}  
