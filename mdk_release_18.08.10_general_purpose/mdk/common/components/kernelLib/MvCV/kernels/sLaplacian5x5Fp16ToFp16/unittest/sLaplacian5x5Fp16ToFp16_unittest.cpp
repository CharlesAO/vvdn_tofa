//sLaplacian5x5Fp16ToFp16 kernel test
//Asm function prototype:
//     void mvcvSLaplacian5x5Fp16ToFp16_asm(u8** in, u8** out, u32 inWidth);
//
//Asm test function prototype:
//     void sLapplacian5x5Fp16ToFp16_asm_test(unsigned char** in, unsigned char** out, unsigned int inWidth);
//
//C function prototype:
//     void mvcvSLaplacian5x5Fp16ToFp16(u8** in, u8** out, u32 inWidth);
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
#include "sLaplacian5x5Fp16ToFp16.h"
#include "sLaplacian5x5Fp16ToFp16_asm_test.h"
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
half DELTA=0.2; //accepted tolerance between C and ASM results

class sLaplacian5x5Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(sLaplacian5x5Fp16ToFp16KernelTest, TestRandomDATABigWidth)
{
	width = randGen->GenerateUInt(1280, 1921, 16);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 0.0, 1.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvSLaplacian5x5Fp16ToFp16(inLinesOffseted, outLinesC, width);
	sLaplacian5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, width);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 5);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLinesC, width , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLinesAsm, width , 1);

	RecordProperty("CyclePerPixel", sLaplacian5x5Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(sLaplacian5x5Fp16ToFp16KernelTest, TestRandomDATASmallWidth)
{
	width = randGen->GenerateUInt(8, 1281, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 0.0, 1.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvSLaplacian5x5Fp16ToFp16(inLinesOffseted, outLinesC, width);
	sLaplacian5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, width);

	RecordProperty("CyclePerPixel", sLaplacian5x5Fp16ToFp16CycleCount / width);
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}
