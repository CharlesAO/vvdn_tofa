//boxFilter kernel test
//Asm function prototype:
//     void MaximumV9x4_asm( half** in, half** out, u32 width)
//
//Asm test function prototype:
//     void MaximumV9x4_asm_test(half** in, half** out, u32 width)
//
//C function prototype:
//     void MaximumV9x4(half** in, half** out, u32 width)
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
// width     - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "maximumV9x4.h"
#include "maximumV9x4_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA =0;

class MaximumV9x4KernelTest : public ::testing::TestWithParam< unsigned int > {
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

 TEST_F(MaximumV9x4KernelTest, TestUniformInputLines32)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 12, 0);
	inputGen.FillLine(inLines[0], width, 9.0f);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}

  TEST_F(MaximumV9x4KernelTest, TestAllValuesZero)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 12, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}

TEST_F(MaximumV9x4KernelTest, TestAllValues255)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 12, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}

TEST_F(MaximumV9x4KernelTest, TestMinimumWidthSize)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 12, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}

TEST_F(MaximumV9x4KernelTest, TestSimpleNotEqualInputLines)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 12, 1);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	for(int i = 1; i < 3; i++)
	{
		inputGen.FillLine(inLines[i], width, i + 1);
	}
	
	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}

TEST_F(MaximumV9x4KernelTest, TestRandomWidthRandomData)
{
	width = randGen->GenerateUInt(8, 1921, 8);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 12, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}


//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputsUnnormalized, MaximumV9x4KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(MaximumV9x4KernelTest, TestRandomData)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 12, 0, 255);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 4);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 4);

	mvcvMaximumV9x4(inLines, outLinesC, width);
	MaximumV9x4_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", MaximumV9x4CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 4, DELTA);
}  
