//boxFilter kernel test
//Asm function prototype:
//     void mvcvThresholdBinaryU8(u8** in, u8** out, u8 threshold, u32 width)
//
//Asm test function prototype:
//     void thresholdBinaryU8_asm_test(unsigned char** in, unsigned char** out, unsigned char threshold, unsigned char  width)
//
//C function prototype:
//     void mvcvThresholdBinaryU8(u8** in, u8** out, u8 threshold, u32 width)
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// threshold - parameter 
// width     - width of input line
//


#include "gtest/gtest.h"
#include "thresholdBinaryU8.h"
#include "thresholdBinaryU8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 1

class thresholdBinaryU8KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char threshold;
	unsigned char **inLines;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


// ------------- normalized variant -------------------

TEST_F(thresholdBinaryU8KernelTest, TestUniformInputMinimWidth8)
{
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 175);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 188, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 188, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth16)
{
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 45);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 12, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 12, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth32)
{
	width = 32;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 59);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 12, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 12, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth48)
{
	width = 48;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 0);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 0, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 0, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestRandomInputWidth64)
{
	width = 64;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 0, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 0, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth88)
{
	width = 88;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 156);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 123, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 123, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth320)
{
	width = 320;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 255);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 0, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 0, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(thresholdBinaryU8KernelTest, TestUniformInputWidth408)
{
	width = 408;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 1, 85);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 23, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 23, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(thresholdBinaryU8KernelTest, TestRndomInputWidth280)
{
	width = 280;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 45, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 45, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
	//for(int i=0;i < width;i++) {
	//printf("%d--  in:%d  outC:%d  outAsm:%d  \n", i, inLines[0][i],outLinesC[0][i], outLinesAsm[0][i] );
	//}
	
}

TEST_F(thresholdBinaryU8KernelTest, TestRandomInputWidth1920)
{
	width = 1920;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, 1,0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	mvcvThresholdBinaryU8(inLines, outLinesC, 123, width);
	thresholdBinaryU8_asm_test(inLines, outLinesAsm, 123, width);

	RecordProperty("CyclePerPixel", thresholdBinaryU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}