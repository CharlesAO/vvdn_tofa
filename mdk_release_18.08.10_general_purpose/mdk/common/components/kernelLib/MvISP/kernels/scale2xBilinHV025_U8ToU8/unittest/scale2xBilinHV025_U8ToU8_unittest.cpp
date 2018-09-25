//Scale2xBilinHV025_U8ToU8 kernel test

//Asm function prototype:
//	void mvispScale2xBilinHV025_U8ToU8_asm(u8 **in, u8 **out, u32 width)

//Asm test function prototype:
//	void scale2xBilinHV025_U8ToU8_asm_test(unsigned char **in, unsigned char **out, unsigned int width);

//C function prototype:
//	void mvispScale2xBilinHV025_U8ToU8(u8 **in, u8 **out, u32 width)

// in         - array of pointers to input lines
// out        - pointer to output line
// inWidth    - width of output line

#include "gtest/gtest.h"
#include "scale2xBilinHV025_U8ToU8.h"
#include "scale2xBilinHV025_U8ToU8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
#define DELTA 1

using ::testing::TestWithParam;
using ::testing::Values;

class scale2xBilinHV025_U8ToU8Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char **inLines;
    unsigned char **inLinesOffseted;
	unsigned char **outLineC;
	unsigned char **outLineAsm;
	u32 width;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(scale2xBilinHV025_U8ToU8Test, TestUniformInput)
{
	width = 32;
 
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 20);		
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

TEST_F(scale2xBilinHV025_U8ToU8Test, TestMinimumWidthSize)
{
	width = 8;
 
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 0, 100);		
	inputGen.SelectGenerator("uniform");
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

TEST_F(scale2xBilinHV025_U8ToU8Test, TestMaximumWidthSize)
{
	width = 1920;
 
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 0, 100);		
	inputGen.SelectGenerator("uniform");
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

TEST_F(scale2xBilinHV025_U8ToU8Test, TestUniformInputLinesAll0)
{
	width = 64;
 
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 0);		
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

TEST_F(scale2xBilinHV025_U8ToU8Test, TestUniformInputLinesAll255)
{
	width = 64;
 
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 255);		
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}


//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scale2xBilinHV025_U8ToU8Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880, (unsigned int)1816, (unsigned int)1920);
);


TEST_P(scale2xBilinHV025_U8ToU8Test, TestRandomInputParameterizedWidth)
{
	width = GetParam();

    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width/2 + PADDING, 2 , 0, 255);		
	inLinesOffseted = inputGen.GetLines(width/2,  2, 0);	
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
   
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
  
	mvispScale2xBilinHV025_U8ToU8(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U8ToU8_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale2xBilinHV025_U8ToU8CycleCount / width);

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

