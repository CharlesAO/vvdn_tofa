//sLaplacian3x3Fp16ToFp16 kernel test
//Asm function prototype:
//     void mvcvSLaplacian3x3Fp16ToFp16_asm(half** in, half** out, u32 inWidth);
//
//Asm test function prototype:
//     void sLapplacian3x3_asm_test(half** in, half** out, unsigned int inWidth);
//
//C function prototype:
//     void mvcvSLaplacian3x3Fp16ToFp16(half** in, half** out, u32 inWidth);
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
#include "sLaplacian3x3Fp16ToFp16.h"
#include "sLaplacian3x3Fp16ToFp16_asm_test.h"
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
//half DELTA=3 ;//accepted tolerance between C and ASM results
half DELTA=2;
class sLaplacian3x3Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
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
	half height;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

 TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestUniformInputLinesWidth8)
{
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 1);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
		
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);	
		
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

 TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 2);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	//outLinesExp = inputGen.GetEmptyLinesFp16(width, 1);
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
			
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestUniformInputLinesAllValues0)
{
	width = 128;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0);

	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);	
	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestUniformInputLinesAllValues255)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 255.0);
	outLinesC = inputGen.GetEmptyLinesFp16(width + PADDING, 255.0);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);	
	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
}


TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestRandomLines)
{
	width = 640;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3,0, 255);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
}

 
 TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestUniformInputLinesExpValue)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	//outLinesExp = inputGen.GetEmptyLinesFp16(width, 1);

	inLines[0][3] = 2;
	inLines[0][4] = 2;
	inLines[0][5] = 1;
	inLines[0][6] = 7;
	inLines[0][7] = 5;
	inLines[0][8] = 8;
	inLines[0][9] = 8;
	inLines[0][10] = 6;
	inLines[0][11] = 7;
	inLines[0][12] = 7;
	
	inLines[1][3] = 7;
	inLines[1][4] = 7;
	inLines[1][5] = 5;
	inLines[1][6] = 3;
	inLines[1][7] = 2;
	inLines[1][8] = 2;
	inLines[1][9] = 9;
	inLines[1][10]= 1;
	inLines[1][11] = 4;
	inLines[1][12] = 4;
	
	
	inLines[2][3] = 1;
	inLines[2][4] = 1;
	inLines[2][5] = 2;
	inLines[2][6] = 3;
	inLines[2][7] = 5;
	inLines[2][8] = 7;
	inLines[2][9] = 7;
	inLines[2][10]= 4;
	inLines[2][11] = 2;
	inLines[2][12] = 2;
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
		
	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	 
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
	//outputChecker.CompareArrays(outLinesAsm[0], outLinesExp[0], width, DELTA);
	}  



 TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestColumnAscendingInputSize3x3)
{
	width = 8;
	height = 3;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, height, 0);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	
	inLines[0][1] = 101;
	inLines[0][2] = 104;
	inLines[0][3] = 107;
	
	inLines[1][1] = 102;
	inLines[1][2] = 105;
	inLines[1][3] = 108;
	
	inLines[2][1] = 103;
	inLines[2][2] = 106;
	inLines[2][3] = 109;
	
	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
	
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestRandomLinesWidth1912)
{
	width = 1912;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING,3 ,0 ,255);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
} 

TEST_F(sLaplacian3x3Fp16ToFp16KernelTest, TestRandomLinesMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0.0, 255.0);
	
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inLinesPadd = inputGen.GetOffsettedLines(inLines, 3, PADDING / 2);
	mvcvSLaplacian3x3Fp16ToFp16(inLinesPadd, outLinesC, width);
	sLaplacian3x3Fp16ToFp16_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", sLaplacian3x3Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays<half>(outLinesC[0], outLinesAsm[0], width, DELTA);
}
     