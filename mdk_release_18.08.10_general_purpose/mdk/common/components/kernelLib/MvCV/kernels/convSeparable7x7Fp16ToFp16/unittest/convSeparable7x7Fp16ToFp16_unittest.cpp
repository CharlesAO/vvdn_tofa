//convSeparable7x7Fp16ToFp16 kernel test
//Asm function prototype:
//     void convSeparable7x7Fp16ToFp16_asm(u8** in, u8** out, float conv[4], u32 inWidth);
//
//Asm test function prototype:
//     void convSeparable7x7Fp16ToFp16_asm_test(unsigned char** in, unsigned char** out, float conv[4], unsigned int inWidth);
//
//C function prototype:
//     void convSeparable7x7Fp16ToFp16(u8** in, u8** out, float conv[4], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 3 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convSeparable7x7Fp16ToFp16.h"
#include "convSeparable7x7Fp16ToFp16_asm_test.h"
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
const half DELTA = 1; //accepted tolerance between C and ASM results

class convSeparable7x7Fp16ToFp16Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	half **inLines;
	half **outC;
	half **outAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(convSeparable7x7Fp16ToFp16Test, Test1Width8)
{
	width = 8;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 1.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	half conv[4];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.87248894f;
 	conv[3] = 0.97248894f;
	
	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted,  conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
}

 TEST_F(convSeparable7x7Fp16ToFp16Test, Test2Width48)
{
	width = 48;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 11.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	half conv[4];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.87248894f;
 	conv[3] = 0.97248894f;
	
	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);
	
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);

} 
 
TEST_F(convSeparable7x7Fp16ToFp16Test, Test2All255)
{
	width = 48;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	half conv[4];
	conv[0] = 0.266379085f; // ensure overflow condition
	conv[1] = 0.867242834f;
	conv[2] = 0.567242834f;
	conv[3] = 0.167242834f;
	
	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted,  conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

 TEST_F(convSeparable7x7Fp16ToFp16Test , Test3RandomAll)
{
	 half **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;
	
	half conv[4];
	inputGen.FillLine(conv, 4, 0.01f, 0.03f);
	
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
}
 

 TEST_F(convSeparable7x7Fp16ToFp16Test , Test3RandomInputWidth312)
{
	 half **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = 312;
	
	half conv[4];
	inputGen.FillLine(conv, 4, 0.01f, 0.03f);
	
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

 
  TEST_F(convSeparable7x7Fp16ToFp16Test, Test4Width1920)
{
	width = 1920;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	inputGen.SelectGenerator("uniform");
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	half conv[4];
	inputGen.FillLine(conv, 4, 0.01f, 0.03f);
	
	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

 INSTANTIATE_TEST_CASE_P(RandomInputs, convSeparable7x7Fp16ToFp16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(convSeparable7x7Fp16ToFp16Test , TestRandomData)
{
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	half conv[4];
	inputGen.FillLine(conv, 4, 0.01f, 0.03f);
	
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvConvSeparable7x7Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable7x7Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
}
