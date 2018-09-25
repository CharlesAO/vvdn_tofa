//sLaplacian5x5 kernel test
//Asm function prototype:
//     void mvcvSLaplacian5x5_asm(u8** in, u8** out, u32 inWidth);
//
//Asm test function prototype:
//     void sLapplacian5x5_asm_test(unsigned char** in, unsigned char** out, unsigned int inWidth);
//
//C function prototype:
//     void mvcvSLaplacian5x5(u8** in, u8** out, u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
//
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "sLaplacian5x5.h"
#include "sLaplacian5x5_asm_test.h"
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
#define DELTA 1 //accepted tolerance between C and ASM results

class sLaplacian5x5KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **inLines;
	unsigned char **inLinesPadd;
	unsigned char **outLinesExp;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;
	unsigned char height;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(sLaplacian5x5KernelTest, TestUniformInputLinesWidth8)
{
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 5, 1);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);	
	
		
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(sLaplacian5x5KernelTest, TestUniformInputLinesExpectedValues)
{
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 5, 0);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesExp = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	
	inLines[0][2] = 1;
	inLines[0][3] = 1;
	inLines[0][4] = 1;
	inLines[0][5] = 8;
	inLines[0][6] = 9;
	inLines[0][7] = 6;
	inLines[0][8] = 1;
	inLines[0][9] = 5;
	inLines[0][10] = 3;
	inLines[0][11] = 2;
	inLines[0][12] = 2;
	inLines[0][13] = 2;
	
	inLines[1][2]  = 2;
	inLines[1][3]  = 2;
	inLines[1][4]  = 2;
	inLines[1][5]  = 3;
	inLines[1][6]  = 4;
	inLines[1][7]  = 1;
	inLines[1][8]  = 8;
	inLines[1][9]  = 7;
	inLines[1][10] = 5;	
	inLines[1][11] = 9;	
	inLines[1][12] = 9;	
	inLines[1][13] = 9;

	inLines[2][2]  = 3;
	inLines[2][3]  = 3;
	inLines[2][4]  = 3;
	inLines[2][5]  = 0;
	inLines[2][6]  = 7;
	inLines[2][7]  = 8;
	inLines[2][8]  = 9;
	inLines[2][9]  = 7;
	inLines[2][10] = 4;
	inLines[2][11] = 7;
	inLines[2][12] = 7;
	inLines[2][13] = 7;
	
	inLines[3][2]  = 8;
	inLines[3][3]  = 8;
	inLines[3][4]  = 8;
	inLines[3][5]  = 2;
	inLines[3][6]  = 6;
	inLines[3][7]  = 1;
	inLines[3][8]  = 1;
	inLines[3][9]  = 1;
	inLines[3][10] = 1;
	inLines[3][11] = 3;
	inLines[3][12] = 3;
	inLines[3][13] = 3;

	inLines[4][2]  = 5;
	inLines[4][3]  = 5;
	inLines[4][4]  = 5;
	inLines[4][5]  = 3;
	inLines[4][6]  = 0;
	inLines[4][7]  = 5;
	inLines[4][8]  = 7;
	inLines[4][9]  = 4;
	inLines[4][10] = 0;
	inLines[4][11] = 7;
	inLines[4][12] = 7;
	inLines[4][13] = 7; 
	
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	
	outLinesExp[0][0] = 0; 
	outLinesExp[0][1] = 0;
	outLinesExp[0][2] = 48;
	outLinesExp[0][3] = 55;
	outLinesExp[0][4] = 67;
	outLinesExp[0][5] = 31;
	outLinesExp[0][6] = 0;
	outLinesExp[0][7] = 25;
	

	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);	
	outputChecker.CompareArrays(outLinesC[0], outLinesExp[0], width, DELTA);
	outputChecker.CompareArrays(outLinesAsm[0], outLinesExp[0], width, DELTA);

}

TEST_F(sLaplacian5x5KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 5, 3);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(sLaplacian5x5KernelTest, TestUniformInputLinesAllValues0)
{
	width = 320;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 5, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(sLaplacian5x5KernelTest, TestUniformInputLinesAllValues255)
{
	width = 128;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 5, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(sLaplacian5x5KernelTest, TestRandomLinesWidth640)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

 TEST_F(sLaplacian5x5KernelTest, TestColumnAscendingInputSize5x5)
{
	width = 8;
	height = 5;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLines[0][4] = 7;
	inLines[0][5] = 8;
	inLines[0][6] = 11;
	inLines[0][7] = 5;
	inLines[0][8] = 1;
	inLines[0][9] = 1;
	inLines[0][10] = 1;
	inLines[0][11] = 1;
	
	inLines[1][4]  = 2;
	inLines[1][5]  = 3;
	inLines[1][6]  = 4;
	inLines[1][7]  = 6;
	inLines[1][8]  = 8;
	inLines[1][9]  = 8;
	inLines[1][10] = 8;
	inLines[1][11] = 8;
	
	inLines[2][4]  = 8;
	inLines[2][5]  = 3;
	inLines[2][6]  = 9;
	inLines[2][7]  = 0;
	inLines[2][8]  = 2;
	inLines[2][9]  = 2;
	inLines[2][10] = 2;
	inLines[2][11] = 2;
	
	inLines[3][4]  = 2;
	inLines[3][5]  = 2;
	inLines[3][6]  = 2;
	inLines[3][7]  = 2;
	inLines[3][8]  = 2;
	inLines[3][9]  = 7;
	inLines[3][10] = 7;
	inLines[3][11] = 2;
	
	inLines[4][4]  = 7;
	inLines[4][5]  = 7;
	inLines[4][6]  = 3;
	inLines[4][7]  = 4;
	inLines[4][8]  = 4;
	inLines[4][9]  = 4;
	inLines[4][10] = 4;
	inLines[4][11] = 5;
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

TEST_F(sLaplacian5x5KernelTest, TestRandomLinesMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 5, PADDING / 2);
		
	mvcvSLaplacian5x5(inLinesPadd, outLinesC, width);
	sLaplacian5x5_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian5x5CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
 