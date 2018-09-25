//sLaplacian7x7Fp16ToFp16 kernel test
//Asm function prototype:
//     void mvcvSLaplacian7x7Fp16ToFp16_asm(u8** in, u8** out, u32 inWidth);
//
//Asm test function prototype:
//     void sLaplacian7x7Fp16ToFp16_asm_test(unsigned char** in, unsigned char** out, unsigned int inWidth);
//
//C function prototype:
//     void mvcvSLaplacian7x7Fp16ToFp16(u8** in, u8** out, u32 inWidth);
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
#include "sLaplacian7x7Fp16ToFp16.h"
#include "sLaplacian7x7Fp16ToFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 16
half DELTA=1; //accepted tolerance between C and ASM results

class sLaplacian7x7Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half **inLinesPadd;
	half **outLinesExp;
	half **outLinesC;
	half **outLinesAsm;
	unsigned int width;
	unsigned char height;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestUniformInputLinesWidth8)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7, 0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

 TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7, 3);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestUniformInputLinesAllValues0)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7,0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
	//outputChecker.DumpArrays("OutC","outLinesC",outLinesC,width,1);
	//outputChecker.DumpArrays("OutAsm","outLinesAsm",outLinesAsm,width,1);
	//outputChecker.DumpArrays("In","inLines",inLines,width+PADDING,7);
}


TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestUniformInputLinesAllValues200)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7, 200);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}

TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestRandomLines)
{
	width = 16;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7, 0, 1.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);

//	outputChecker.DumpArrays("OutputC", "OutputC.c", outLinesC, width,1);
	//outputChecker.DumpArrays("OutputAsm", "OutputAsm.c", outLinesAsm, width,1);
//	outputChecker.DumpArrays("inLines", "inLines.c", inLines, width+PADDING,7);
}


 TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestColumnAscendingInputSize7x7Fp16ToFp16)
{
	width = 8;
	height = 7;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, height, 0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

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
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

TEST_F(sLaplacian7x7Fp16ToFp16KernelTest, TestRandomLinesMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 7, 0, 1.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 7, PADDING / 2);
	
	mvcvSLaplacian7x7Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian7x7Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian7x7Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}


