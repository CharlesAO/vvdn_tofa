//scale05BilinHV_U16ToU16 kernel test

//Asm function prototype:
//	void mvispScale05BilinHV_U16ToU16_asm(u16 **in, u16 *out, u32 width)

//Asm test function prototype:
//	void scale05BilinHV_U16ToU16_asm_test(unsigned short **in, unsigned short *out, unsigned int width);

//C function prototype:
//	void mvispScale05BilinHV_U16ToU16(u16 **in, u16 *out, u32 width)

// in         - array of pointers to input lines
// out        - pointer to output line
// inWidth    - width of input line

#include "gtest/gtest.h"
#include "scale05BilinHV_U16ToU16.h"
#include "scale05BilinHV_U16ToU16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "scale05BilinHV_U16ToU16_unittest.h"


#define OUT_PADDING 16
#define PADDING_VALUE 28
const unsigned short DELTA = 1;

using ::testing::TestWithParam;
using ::testing::Values;

class scale05BilinHV_U16ToU16Test : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u16 **inLines;
	u16 *outLineC;
	u16 *outLineAsm;
	u16 *outLineExpected;
	u32 width;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(scale05BilinHV_U16ToU16Test, TestExpectedOutputMinWidth)
{
	width = 8;	
	inputGen.SelectGenerator("uniform");	
	inLines = inputGen.GetLinesU16(width * 2, 2, 0);	
	outLineC = inputGen.GetLineU16(width  + 2 * OUT_PADDING, PADDING_VALUE);
	outLineAsm = inputGen.GetLineU16(width + 2 * OUT_PADDING, PADDING_VALUE);		
	outLineExpected = inputGen.GetLineU16(width + 2 * OUT_PADDING, PADDING_VALUE);		
	
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
	
	mvispScale05BilinHV_U16ToU16(inLines, &outLineC[OUT_PADDING], width);
	scale05BilinHV_U16ToU16_asm_test(inLines, outLineAsm, width);
	
	CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING, DELTA);	
	CompareArrays(outLineExpected, outLineAsm, width + 2 * OUT_PADDING, DELTA);	
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scale05BilinHV_U16ToU16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)24, (unsigned int)40, (unsigned int)64, (unsigned int)512, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(scale05BilinHV_U16ToU16Test, TestRandomInputParameterizedWidth)
{

	width = GetParam();	
	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLineU16(width  + 2 * OUT_PADDING, PADDING_VALUE);
	outLineAsm = inputGen.GetLineU16(width + 2 * OUT_PADDING, PADDING_VALUE);
	
	inputGen.SelectGenerator("random");	
	inLines = inputGen.GetLinesU16(width * 2, 2, 0, 65535);	
		
	
	mvispScale05BilinHV_U16ToU16(inLines, &outLineC[OUT_PADDING], width);
	scale05BilinHV_U16ToU16_asm_test(inLines, outLineAsm, width);

	/*
	outputCheck.DumpArrays("inLines[0]", "input[0].c", inLines[0], width * 2);
	outputCheck.DumpArrays("inLines[1]", "input[1].c", inLines[1], width * 2);
	outputCheck.DumpArrays("outLineC", "outputC.c", outLineC, width);
	outputCheck.DumpArrays("outLineAsm", "outputAsm.c", outLineAsm, width);
	*/

	CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING, DELTA);	
}
