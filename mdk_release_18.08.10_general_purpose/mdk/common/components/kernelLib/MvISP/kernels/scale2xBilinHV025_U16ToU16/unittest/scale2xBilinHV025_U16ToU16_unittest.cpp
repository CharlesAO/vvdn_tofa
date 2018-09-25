//Scale2xBilinHV025_U16ToU16 kernel test

//Asm function prototype:
//	void mvispScale2xBilinHV025_U16ToU16_asm(u16 **in, u16 **out, u32 width)

//Asm test function prototype:
//	void scale2xBilinHV025_U16ToU16_asm_test(unsigned char **in, unsigned char **out, unsigned int width);

//C function prototype:
//	void mvispScale2xBilinHV025_U16ToU16(u16 **in, u16 **out, u32 width)

// in         - array of pointers to input lines
// out        - pointer to output line
// inWidth    - width of output line

#include "gtest/gtest.h"
#include "scale2xBilinHV025_U16ToU16.h"
#include "scale2xBilinHV025_U16ToU16_asm_test.h"
#include "scale2xBilinHV025_U16ToU16_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
const unsigned short DELTA = 1;

using ::testing::TestWithParam;
using ::testing::Values;

class scale2xBilinHV025_U16ToU16Test : public ::testing::TestWithParam <unsigned long> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u16** inLines;
	u16** inLinesOffseted;
	u16** outLineC;
	u16** outLineAsm;
	u16** outLineExpected;
	u32 width;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(scale2xBilinHV025_U16ToU16Test, TestUniformInputMinWidthMaxValue)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width/2 + PADDING, 2 , 65535);		
	outLineC = inputGen.GetLinesU16(width, 1, 0);
	outLineAsm = inputGen.GetLinesU16(width, 1, 0);		
 
  inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);  
	mvispScale2xBilinHV025_U16ToU16(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U16ToU16_asm_test(inLines, outLineAsm, width);

	CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

TEST_F(scale2xBilinHV025_U16ToU16Test, TestRandomInputMaxWidth)
{
	width = 1920;

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width/2 + PADDING, 2 , 0, 255);		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLinesU16(width, 1, 0);
	outLineAsm = inputGen.GetLinesU16(width, 1, 0);		
 
  inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);  
	mvispScale2xBilinHV025_U16ToU16(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U16ToU16_asm_test(inLines, outLineAsm, width);

	CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scale2xBilinHV025_U16ToU16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880);
);

TEST_P(scale2xBilinHV025_U16ToU16Test, TestRandomInputParameterizedWidth)
{
	width = GetParam() * 2;
  inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width/2 + PADDING, 2 , 0, 255);			
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLinesU16(width, 1, 0);
	outLineAsm = inputGen.GetLinesU16(width, 1, 0);		
 
  inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);  
	mvispScale2xBilinHV025_U16ToU16(inLinesOffseted, outLineC, width);
	scale2xBilinHV025_U16ToU16_asm_test(inLines, outLineAsm, width);

	CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	

	/*
	outputCheck.DumpArrays("inLines[0]", "input[0].c", inLines[0], width/2);
	outputCheck.DumpArrays("inLines[1]", "input[1].c", inLines[1], width/2);
	outputCheck.DumpArrays("outLineC", "outputC.c", outLineC, width);
	outputCheck.DumpArrays("outLineAsm", "outputAsm.c", outLineAsm, width);
*/
	
}
