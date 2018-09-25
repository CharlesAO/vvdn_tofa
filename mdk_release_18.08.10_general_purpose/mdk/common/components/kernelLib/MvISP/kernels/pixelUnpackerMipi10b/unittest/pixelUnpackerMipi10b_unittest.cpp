//pixelUnpackerMipi10b kernel test

//Asm function prototype:
//	void pixelUnpackerMipi10b_asm(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround);

//Asm test function prototype:
//	vvoid pixelUnpackerMipi10b_asm_test(unsigned int *in, unsigned short *output, unsigned int width, unsigned int lineNo, unsigned long coefs[4], int noMipiRxWorkaround);

//C function prototype:
//		void pixelUnpackerMipi10b(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround);

// in		          - pointer to input line
// out		          - pointer to output line
// width	          - number of processed pixels
// lineNo	          - line number
// corfs	          - coefficients for black correction
// noMipiRxWorkaround - if not set, do black-level subtraction only

#include "gtest/gtest.h"
#include "pixelUnpackerMipi10b.h"
#include "pixelUnpackerMipi10b_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define OUT_PADDING 16

using ::testing::TestWithParam;
using ::testing::Values;

class PixelUnpackerMipi10bTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned int *inLine;
	u16 *outLineC;
	u16 *outLineAsm;
	u16 *outExpected;
	unsigned int width;
	unsigned int coefs[4];
	unsigned int lineNo;
	int noMipiRxWorkaround;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, PixelUnpackerMipi10bTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880, (unsigned int)1816, (unsigned int)1920);
);


TEST_P(PixelUnpackerMipi10bTest, TestRandomInputParameterizedWidthMipiRxWorkaround)
{
	noMipiRxWorkaround = 0;
	width = GetParam();	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLineU16(width + 2 * OUT_PADDING, 0);
	outLineAsm = inputGen.GetLineU16(width + 2 * OUT_PADDING, 0);
	inputGen.SelectGenerator("random");
	inLine = (u32*)inputGen.GetLineFloat(width / 2, 0, 0xffffffff);	
	coefs[0] = 8;		coefs[1] = 3;
	coefs[2] = 2;		coefs[3] = 7;	
	lineNo = randomGen.GenerateUInt(0, 10);

	PixelUnpackerMipi10b_asm_test(inLine, outLineAsm, width, lineNo, coefs, noMipiRxWorkaround);
	mvispPixelUnpackerMipi10b(inLine, &outLineC[OUT_PADDING], width, lineNo, coefs, noMipiRxWorkaround);

 	/*outputCheck.DumpArrays("input", "input.c", inLine, width/2 , 1);
 	outputCheck.DumpArrays("outLineC", "outLineC.c", outLineC, width , 1);
	outputCheck.DumpArrays("outLineAsm", "outLineAsm.c", outLineAsm, width , 1);
 	*/

	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING);	
}

TEST_P(PixelUnpackerMipi10bTest, TestRandomInputParameterizedWidthNoMipiRxWorkaround)
{
	noMipiRxWorkaround = 1;
	width = GetParam();	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLineU16(width + 2 * OUT_PADDING, 0);
	outLineAsm = inputGen.GetLineU16(width + 2 * OUT_PADDING, 0);
	inputGen.SelectGenerator("random");
	inLine = (u32*)inputGen.GetLineFloat(width / 2, 0, 0xffffffff);	
	coefs[0] = 8;		coefs[1] = 3;
	coefs[2] = 2;		coefs[3] = 7;	
	lineNo = randomGen.GenerateUInt(0, 10);

	PixelUnpackerMipi10b_asm_test(inLine, outLineAsm, width, lineNo, coefs, noMipiRxWorkaround);
	mvispPixelUnpackerMipi10b(inLine, &outLineC[OUT_PADDING], width, lineNo, coefs, noMipiRxWorkaround);

	/*outputCheck.DumpArrays("input", "input.c", inLine, width/2 , 1);
	outputCheck.DumpArrays("outLineC", "outLineC.c", outLineC, width , 1);
	outputCheck.DumpArrays("outLineAsm", "outLineAsm.c", outLineAsm, width , 1);
	*/

	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2 * OUT_PADDING);
}

