//convolution7x7 kernel test
//Asm function prototype:
//     void Convolution7x7_asm(u8** in, u8** out, half conv[][7], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution7x7_asm(unsigned char** in, unsigned char** out, half conv[][7], unsigned int inWidth);
//
//C function prototype:
//     void Convolution7x7(u8** in, u8** out, float conv[][7], u32 inWidth);
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
#include "convSeparable3x3.h"
#include "convSeparable3x3_asm_test.h"
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
const unsigned char DELTA = 2; //accepted tolerance between C and ASM results

class appTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float conv[2];
	unsigned char **inLines;
	unsigned char *outC;
	unsigned char *outAsm;
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
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 3, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);
	
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
	
	mvcvConvSeparable3x3 (outC   , inLinesOffseted, conv, width);	
    convSeparable3x3_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 3);
	//outputChecker.DumpArrays("outC", "outC.c", outC, width);	
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outAsm, width);	
}




TEST_F(appTest, Test2All255)
{
	width = 16;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 3, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);
	
	conv[0] = 0.266379085f; // ensure overflow condition
	conv[1] = 0.867242834f;
	
	mvcvConvSeparable3x3 (outC   , inLinesOffseted, conv, width);	
    convSeparable3x3_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}

TEST_F(appTest , TestRandomAll)
{
	unsigned char **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 3, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);

	mvcvConvSeparable3x3 (outC   , inLinesOffseted, conv, width);	
    convSeparable3x3_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}


INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(appTest , TestRandomData)
{
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	width = GetParam();
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 3, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 3, 8);
	
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);

	mvcvConvSeparable3x3 (outC   , inLinesOffseted, conv, width);	
    convSeparable3x3_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC, outAsm, width, DELTA);
}


