//convSeparable3x3 Fp16ToFp16 kernel test
//Asm function prototype:
//     void convSeparable3x3Fp16ToFp16_asm(half* out, half** in, half conv[2], u32 inWidth);
//
//C function prototype:
//     void convSeparable3x3Fp16ToFp16(half* out, half** in, half conv[2], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convSeparable3x3Fp16ToFp16_asm_test.h"
#include "convSeparable3x3Fp16ToFp16.h"
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
half DELTA = 2; //accepted tolerance between C and ASM results

class appTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half *outC;
	half *outAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(appTest, Test1)
{
	width = 1920;
	half **inLinesOffseted;

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outC = inputGen.GetEmptyLineFp16(width);
	outAsm = inputGen.GetEmptyLineFp16(width);
	half conv[2];

	conv[0] =  (half) 0.166379085f;
	conv[1] =  (half) 0.667242834f;

	mvcvConvSeparable3x3Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable3x3Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 3);
	//outputChecker.DumpArrays("refInputC", "refInputC.c", inLinesOffseted, width + PADDING * 2 - 8, 3);
	//outputChecker.DumpArrays("outC", "outC.c", outC, width);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outAsm, width);
}




TEST_F(appTest, Test2All255)
{
	width = 1920;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outC = inputGen.GetEmptyLineFp16(width);
	outAsm = inputGen.GetEmptyLineFp16(width);
	half conv[2];

	conv[0] =  (half) 0.266379085f; // ensure overflow condition
	conv[1] =  (half) 0.867242834f;

	mvcvConvSeparable3x3Fp16ToFp16 (outC, inLinesOffseted, conv, width);
    convSeparable3x3Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}

TEST_F(appTest , TestRandomAll)
{
	half **inLinesOffseted;
	half conv[2];

	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;

	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outC = inputGen.GetEmptyLineFp16(width);
	outAsm = inputGen.GetEmptyLineFp16(width);

	inputGen.FillLine(conv, 2, 0.01f, 0.03f);

	mvcvConvSeparable3x3Fp16ToFp16 (outC, inLinesOffseted, conv, width);
    convSeparable3x3Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}


INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(appTest , TestRandomData)
{
	half **inLinesOffseted;
	half conv[2];

	inputGen.SelectGenerator("random");
	width = GetParam();

	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);

	outC = inputGen.GetEmptyLineFp16(width);
	outAsm = inputGen.GetEmptyLineFp16(width);

	inputGen.FillLine(conv, 2, 0.01f, 0.03f);

	mvcvConvSeparable3x3Fp16ToFp16 (outC, inLinesOffseted, conv, width);
    convSeparable3x3Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}
