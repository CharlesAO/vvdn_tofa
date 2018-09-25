//convSeparable7x7 kernel test
//Asm function prototype:
//     void convSeparable7x7_asm(u8** in, u8** out, float conv[4], u32 inWidth);
//
//Asm test function prototype:
//     void convSeparable7x7_asm_test(unsigned char** in, unsigned char** out, float conv[4], unsigned int inWidth);
//
//C function prototype:
//     void convSeparable7x7(u8** in, u8** out, float conv[4], u32 inWidth);
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
#include "convSeparable7x7.h"
#include "convSeparable7x7_asm_test.h"
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
const unsigned char DELTA = 1; //accepted tolerance between C and ASM results

class convSeparable7x7Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	unsigned char **inLines;
	unsigned char **outC;
	unsigned char **outAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(convSeparable7x7Test, Test1Width8)
{
	width = 8;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[4];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.87248894f;
 	conv[3] = 0.97248894f;
	
	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);
	
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
	
}

 TEST_F(convSeparable7x7Test, Test2Width48)
{
	width = 48;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 11);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[4];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.87248894f;
 	conv[3] = 0.97248894f;
	
	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);
	
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);

} 
 
TEST_F(convSeparable7x7Test, Test2All255)
{
	width = 48;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[4];
	conv[0] = 0.266379085f; // ensure overflow condition
	conv[1] = 0.867242834f;
	conv[2] = 0.567242834f;
	conv[3] = 0.167242834f;
	
	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

 TEST_F(convSeparable7x7Test , Test3RandomAll)
{
	unsigned char **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;
	
	float conv[4];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[3] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
}
 

 TEST_F(convSeparable7x7Test , Test3RandomInputWidth312)
{
	unsigned char **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = 312;
	
	float conv[4];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[3] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

 
  TEST_F(convSeparable7x7Test, Test4Width1920)
{
	width = 1920;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	inputGen.SelectGenerator("uniform");
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[4];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[3] = randGen->GenerateFloat(0.0f, 1.0f);
	
	mvcvConvSeparable7x7(outC   , inLinesOffseted,  conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);
	

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
	
} 

 INSTANTIATE_TEST_CASE_P(RandomInputs, convSeparable7x7Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(convSeparable7x7Test , TestRandomData)
{
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	float conv[4];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[3] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 7, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	mvcvConvSeparable7x7(outC   , inLinesOffseted, conv, width);	
    convSeparable7x7_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
}
