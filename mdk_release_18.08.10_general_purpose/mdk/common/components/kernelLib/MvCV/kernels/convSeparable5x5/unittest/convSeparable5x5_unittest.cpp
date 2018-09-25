//convSeparable5x5 kernel test
//Asm function prototype:
//     void convSeparable5x5_asm(u8** in, u8** out, float conv[3], u32 inWidth);
//
//Asm test function prototype:
//     void convSeparable5x5_asm_test(unsigned char** in, unsigned char** out, float conv[3], unsigned int inWidth);
//
//C function prototype:
//     void convSeparable5x5(u8** in, u8** out, float conv[3], u32 inWidth);
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
#include "convSeparable5x5.h"
#include "convSeparable5x5_asm_test.h"
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

class convSeparable5x5Test : public ::testing::TestWithParam< unsigned int > {
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

 TEST_F(convSeparable5x5Test, Test0Width8)
{
	width = 8;
	unsigned char **inLinesOffseted;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 8);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);

	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	float conv[3];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.67248894f;
	
	
	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);
	
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
	
} 
  
TEST_F(convSeparable5x5Test, Test1Width32)
{
	width = 32;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 10);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
		
	float conv[3];
	conv[0] = 0.166379085f;
	conv[1] = 0.667242834f;
 	conv[2] = 0.67248894f;
	
	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);
	
	
	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
	
} 


 
TEST_F(convSeparable5x5Test, Test2All255)
{
	width = 32;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[3];
	conv[0] = 0.266379085f; // ensure overflow condition
	conv[1] = 0.867242834f;
	conv[2] = 0.567242834f;
	
	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 
 
TEST_F(convSeparable5x5Test , Test3RandomAll)
{
	unsigned char **inLinesOffseted;
	
	// width need to be multiple of 8
	inputGen.SelectGenerator("random");
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;
	
	float conv[3];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 

  TEST_F(convSeparable5x5Test, Test4Width1920)
{
	width = 1920;
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	inputGen.SelectGenerator("uniform");
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);
	
	float conv[3];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	
	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);
	

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);

}

 INSTANTIATE_TEST_CASE_P(RandomInputs, convSeparable5x5Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(convSeparable5x5Test , TestRandomData)
{
	unsigned char **inLinesOffseted;
	
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	float conv[3];
	conv[0] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[1] = randGen->GenerateFloat(0.0f, 1.0f);
	conv[2] = randGen->GenerateFloat(0.0f, 1.0f);
	
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	
	outC = inputGen.GetEmptyLines(width,1);
	outAsm = inputGen.GetEmptyLines(width,1);

	mvcvConvSeparable5x5(outC   , inLinesOffseted, conv, width);	
    convSeparable5x5_asm_test(outAsm , inLines        , conv, width);

	outputChecker.CompareArrays(outC[0], outAsm[0], width, DELTA);
} 
