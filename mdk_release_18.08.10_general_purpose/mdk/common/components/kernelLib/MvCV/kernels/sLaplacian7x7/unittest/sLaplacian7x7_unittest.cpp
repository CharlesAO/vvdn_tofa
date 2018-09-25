//sLaplacian7x7 kernel test
//Asm function prototype:
//     void mvcvSLaplacian7x7_asm(u8** in, u8** out, u32 inWidth);
//
//Asm test function prototype:
//     void sLaplacian7x7_asm_test(unsigned char** in, unsigned char** out, unsigned int inWidth);
//
//C function prototype:
//     void mvcvSLaplacian7x7(u8** in, u8** out, u32 inWidth);
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
#include "sLaplacian7x7.h"
#include "sLaplacian7x7_asm_test.h"
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

class sLaplacian7x7KernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(sLaplacian7x7KernelTest, TestUniformInputLinesWidth8)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 7, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

 TEST_F(sLaplacian7x7KernelTest, TestUniformInputLinesExpectedValues)
{
	
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 7, 0);
	
	outLinesC = inputGen.GetEmptyLines(width, 1);
	//outLinesExp = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	
	inLines[0][1] = 1;
	inLines[0][2] = 1;
	inLines[0][3] = 1;
	inLines[0][4] = 1;
	inLines[0][5] = 2;
	inLines[0][6] = 1;
	inLines[0][7] = 2;
	inLines[0][8] = 1;
	inLines[0][9] = 5;
	inLines[0][10] = 3;
	inLines[0][11] = 2;
	inLines[0][12] = 2;
	inLines[0][13] = 2;
	inLines[0][14] = 2;
	
	inLines[1][1]  = 2;
	inLines[1][2]  = 2;
	inLines[1][3]  = 2;
	inLines[1][4]  = 2;
	inLines[1][5]  = 3;
	inLines[1][6]  = 4;
	inLines[1][7]  = 1;
	inLines[1][8]  = 5;
	inLines[1][9]  = 7;
	inLines[1][10] = 5;	
	inLines[1][11] = 2;	
	inLines[1][12] = 2;	
	inLines[1][13] = 2;
	inLines[1][14] = 2;

	inLines[2][1]  = 3;
	inLines[2][2]  = 3;
	inLines[2][3]  = 3;
	inLines[2][4]  = 3;
	inLines[2][5]  = 0;
	inLines[2][6]  = 7;
	inLines[2][7]  = 8;
	inLines[2][8]  = 0;
	inLines[2][9]  = 7;
	inLines[2][10] = 4;
	inLines[2][11] = 7;
	inLines[2][12] = 7;
	inLines[2][13] = 7;
	inLines[2][14] = 7;
	
	inLines[3][1]  = 1;
	inLines[3][2]  = 1;
	inLines[3][3]  = 1;
	inLines[3][4]  = 1;
	inLines[3][5]  = 2;
	inLines[3][6]  = 6;
	inLines[3][7]  = 1;
	inLines[3][8]  = 1;
	inLines[3][9]  = 1;
	inLines[3][10] = 1;
	inLines[3][11] = 3;
	inLines[3][12] = 3;
	inLines[3][13] = 3;
	inLines[3][14] = 3;

	inLines[4][1]  = 5;
	inLines[4][2]  = 5;
	inLines[4][3]  = 5;
	inLines[4][4]  = 5;
	inLines[4][5]  = 3;
	inLines[4][6]  = 0;
	inLines[4][7]  = 5;
	inLines[4][8]  = 7;
	inLines[4][9]  = 4;
	inLines[4][10] = 0;
	inLines[4][11] = 1;
	inLines[4][12] = 1;
	inLines[4][13] = 1; 
	inLines[4][14] = 1; 
	
	inLines[5][1]  = 2;
	inLines[5][2]  = 2;
	inLines[5][3]  = 2;
	inLines[5][4]  = 2;
	inLines[5][5]  = 3;
	inLines[5][6]  = 0;
	inLines[5][7]  = 5;
	inLines[5][8]  = 7;
	inLines[5][9]  = 4;
	inLines[5][10] = 0;
	inLines[5][11] = 1;
	inLines[5][12] = 1;
	inLines[5][13] = 1; 
	inLines[5][14] = 1; 
	
	inLines[6][1]  = 5;
	inLines[6][2]  = 5;
	inLines[6][3]  = 5;
	inLines[6][4]  = 5;
	inLines[6][5]  = 3;
	inLines[6][6]  = 0;
	inLines[6][7]  = 5;
	inLines[6][8]  = 7;
	inLines[6][9]  = 4;
	inLines[6][10] = 0;
	inLines[6][11] = 3;
	inLines[6][12] = 3;
	inLines[6][13] = 3; 
	inLines[6][14] = 3; 
	
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
		
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);
	
	/* outLinesExp[0][0] = 0; 
	outLinesExp[0][1] = 0;
	outLinesExp[0][2] = 55;
	outLinesExp[0][3] = 63;
	outLinesExp[0][4] = 76;
	outLinesExp[0][5] = 38;
	outLinesExp[0][6] = 0;
	outLinesExp[0][7] = 32; */
	

	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);	
	//outputChecker.CompareArrays(outLinesC[0], outLinesExp[0], width, DELTA);
	//outputChecker.CompareArrays(outLinesAsm[0], outLinesExp[0], width, DELTA);
	outputChecker.CompareArrays(outLinesAsm[0], outLinesC[0], width, DELTA);
}

 TEST_F(sLaplacian7x7KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 7, 3);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
 
TEST_F(sLaplacian7x7KernelTest, TestUniformInputLinesAllValues0)
{
	width = 128;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 7, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(sLaplacian7x7KernelTest, TestUniformInputLinesAllValues255)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, 7, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(sLaplacian7x7KernelTest, TestRandomLines)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 7, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


 TEST_F(sLaplacian7x7KernelTest, TestColumnAscendingInputSize7x7)
{
	width = 8;
	height = 7;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

    inLines[0][3] = 1;
	inLines[0][4] = 2;
	inLines[0][5] = 7;
	inLines[0][6] = 7;
	inLines[0][7] = 3;
	inLines[0][8] = 2;
	inLines[0][9] = 8;	
	inLines[1][3] = 8;
	inLines[1][4] = 9;
	inLines[1][5] = 3;
	inLines[1][6] = 5;
	inLines[1][7] = 4;
	inLines[1][8] = 7;
	inLines[1][9] = 9;	
	inLines[2][3] = 7;
	inLines[2][4] = 6;
	inLines[2][5] = 5;
	inLines[2][6] = 4;
	inLines[2][7] = 3;
	inLines[2][8] = 2;
	inLines[2][9] = 1;	
	inLines[3][3] = 1;
	inLines[3][4] = 2;
	inLines[3][5] = 3;
	inLines[3][6] = 4;
	inLines[3][7] = 5;
	inLines[3][8] = 6;
	inLines[3][9] = 7;	
	inLines[4][3] = 1;
	inLines[4][4] = 3;
	inLines[4][5] = 5;
	inLines[4][6] = 7;
	inLines[4][7] = 9;
	inLines[4][8] = 0;
	inLines[4][9] = 0;	
	inLines[5][3] = 9;
	inLines[5][4] = 8;
	inLines[5][5] = 7;
	inLines[5][6] = 6;
	inLines[5][7] = 5;
	inLines[5][8] = 4;
	inLines[5][9] = 3;	
	inLines[6][3] = 0;
	inLines[6][4] = 0;
	inLines[6][5] = 3;
	inLines[6][6] = 0;
	inLines[6][7] = 0;
	inLines[6][8] = 9;
	inLines[6][9] = 9;
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

TEST_F(sLaplacian7x7KernelTest, TestRandomLinesMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, 7, 0, 255);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7(inLinesPadd, outLinesC, width);
	sLaplacian7x7_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}
 