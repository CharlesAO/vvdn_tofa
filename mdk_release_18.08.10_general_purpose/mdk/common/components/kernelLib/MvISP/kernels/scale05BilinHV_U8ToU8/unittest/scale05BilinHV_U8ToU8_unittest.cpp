//scale05BilinHV_U8ToU8 kernel test

//Asm function prototype:
//	void mvispScale05BilinHV_U8ToU8_asm(u8 **in, u8 *out, u32 width)

//Asm test function prototype:
//	void scale05BilinHV_U8ToU8_asm_test(unsigned char **in, unsigned char *out, unsigned int width);

//C function prototype:
//	void mvispScale05BilinHV_U8ToU8(u8 **in, u8 *out, u32 width)

// in         - array of pointers to input lines
// out        - pointer to output line
// inWidth    - width of input line

#include "gtest/gtest.h"
#include "scale05BilinHV_U8ToU8.h"
#include "scale05BilinHV_U8ToU8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define OUT_PADDING 16

using ::testing::TestWithParam;
using ::testing::Values;

class scale05BilinHV_U8ToU8Test : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u8 **inLines;
	u8 *outLineC;
	u8 *outLineAsm;
	u8 *outLineExpected;
	u32 width;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(scale05BilinHV_U8ToU8Test, TestExpectedOutputMinWidth)
{
	width = 8;	
	inputGen.SelectGenerator("uniform");	
	inLines = inputGen.GetEmptyLines(width * 2, 2);	
	outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);		
	outLineExpected = inputGen.GetLine(width + 2 * OUT_PADDING, 0);		
	
	inLines[0][0] = 1; 		inLines[0][1] = 3;
	inLines[1][0] = 10;		inLines[1][1] = 30; 
	outLineExpected[0 + OUT_PADDING] = 11;
	
	inLines[0][2] = 5;		inLines[0][3] = 6;
	inLines[1][2] = 50;		inLines[1][3] = 60; 
	outLineExpected[1 + OUT_PADDING] = 30;
	
	inLines[0][4] = 7;		inLines[0][5] = 2;
	inLines[1][4] = 3;		inLines[1][5] = 8;
	outLineExpected[2 + OUT_PADDING] = 5;
	
	inLines[0][6] = 15;		inLines[0][7] = 2;
	inLines[1][6] = 5;		inLines[1][7] = 2; 
	outLineExpected[3 + OUT_PADDING] = 6;
	
	inLines[0][8] = 12;		inLines[0][9] = 6;
	inLines[1][8] = 18;		inLines[1][9] = 5;	
	outLineExpected[4 + OUT_PADDING] = 10;
	
	inLines[0][10] = 12;	inLines[0][11] = 0;
	inLines[1][10] = 10;	inLines[1][11] = 18; 
	outLineExpected[5 + OUT_PADDING] = 10;
	
	inLines[0][12] = 255;	inLines[0][13] = 255;
	inLines[1][12] = 255;	inLines[1][13] = 255; 
	outLineExpected[6 + OUT_PADDING] = 255;	
	
	inLines[0][14] = 0;		inLines[0][15] = 1;
	inLines[1][14] = 0;		inLines[1][15] = 1; 
	outLineExpected[7 + OUT_PADDING] = 0;	
	
	mvispScale05BilinHV_U8ToU8(inLines, &outLineC[OUT_PADDING], width);
	scale05BilinHV_U8ToU8_asm_test(inLines, outLineAsm, width);
	
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING);	
	outputCheck.CompareArrays(outLineExpected, outLineAsm, width + 2 * OUT_PADDING);	
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scale05BilinHV_U8ToU8Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880, (unsigned int)1816, (unsigned int)1920);
);

TEST_P(scale05BilinHV_U8ToU8Test, TestRandomInputParameterizedWidthRandomShift)
{
	width = GetParam();

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width * 2, 2, 0, 255);		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);		
	
	mvispScale05BilinHV_U8ToU8(inLines, &outLineC[OUT_PADDING], width);
	scale05BilinHV_U8ToU8_asm_test(inLines, outLineAsm, width);

	/*
	outputCheck.DumpArrays("inLines[0]", "input[0].c", inLines[0], width * 2);
	outputCheck.DumpArrays("inLines[1]", "input[1].c", inLines[1], width * 2);
	outputCheck.DumpArrays("outLineC", "outputC.c", outLineC, width);
	outputCheck.DumpArrays("outLineAsm", "outputAsm.c", outLineAsm, width);
	*/

	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING);	
}
