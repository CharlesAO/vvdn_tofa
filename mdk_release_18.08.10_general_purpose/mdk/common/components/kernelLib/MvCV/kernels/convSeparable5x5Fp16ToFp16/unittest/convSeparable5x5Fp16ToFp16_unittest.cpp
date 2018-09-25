//convSeparable5x5 kernel test
//Asm function prototype:
//     void convSeparable5x5Fp16ToFp16_asm(half** in, half** out, half conv[3], u32 inWidth);
//
//Asm test function prototype:
//     void convSeparable5x5Fp16ToFp16_asm_test(half** in, half** out, half conv[3], unsigned int inWidth);
//
//C function prototype:
//     void convSeparable5x5Fp16ToFp16(half** in, half** out, half conv[3], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 2 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convSeparable5x5Fp16ToFp16.h"
#include "convSeparable5x5Fp16ToFp16_asm_test.h"
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

class convSeparable5x5Fp16ToFp16Test : public ::testing::TestWithParam< unsigned int > {
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

 TEST_F(convSeparable5x5Fp16ToFp16Test, Test0Width8)
{
	width = 8;
	half **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 8.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);

	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	half conv[3];
        inputGen.FillLine(conv, 3, 0.01f, 0.03f);
	
	
	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm, inLines, conv, width);
	
	
	outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
} 
  
TEST_F(convSeparable5x5Fp16ToFp16Test, Test1Width32)
{
	width = 32;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 10.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
		
	half conv[3];
        inputGen.FillLine(conv, 3, 0.01f, 0.03f);
	
	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm , inLines, conv, width);
	
	
	outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
} 


 
TEST_F(convSeparable5x5Fp16ToFp16Test, Test2All255)
{
	width = 32;
	half **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	half conv[3];
        inputGen.FillLine(conv, 3, 0.01f, 0.03f);
	
	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm , inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
} 
 
TEST_F(convSeparable5x5Fp16ToFp16Test , Test3RandomAll)
{
	half **inLinesOffseted;
	half conv[3];
	
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;

	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(conv, 3, 0.01f, 0.03f);

	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

    outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
} 

  TEST_F(convSeparable5x5Fp16ToFp16Test, Test4Width1920)
{
	width = 1920;
	half **inLinesOffseted;
	half conv[3];
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	inputGen.SelectGenerator("uniform");
	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);
	
	inputGen.FillLine(conv, 3, 0.01f, 0.03f);
	
	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm, inLines, conv, width);
	
	outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
}

INSTANTIATE_TEST_CASE_P(RandomInputs, convSeparable5x5Fp16ToFp16Test,
 		 Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
 );

  TEST_P(convSeparable5x5Fp16ToFp16Test, TestRandomData)
{

	half **inLinesOffseted;
	half conv[3];

	width = GetParam();
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 5, 0.0f, 255.0f);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);

	outC = inputGen.GetEmptyLinesFp16(width, 1);
	outAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(conv, 3, 0.01f, 0.03f);

	mvcvConvSeparable5x5Fp16ToFp16(outC, inLinesOffseted, conv, width);
    convSeparable5x5Fp16ToFp16_asm_test(outAsm, inLines, conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, 1, DELTA);
}


