//GaussHx2 kernel test
//Asm function prototype:
//     void mvcvGaussHx2(u8 *inLine,u8 *outLine,int width);
//
//Asm test function prototype:
//     void GaussHx2_test(unsigned char* in, unsigned char* out, unsigned int width);
//
//C function prototype:
//     void mvcvGaussHx2(u8 *inLine, u8 *outLine, int width);
//
//Parameter description:
// GaussHx2 filter - downsample even lines and even cols
// inLine  - pointer for output line
// outLine - pointer for output line
// width   - width of input line

#include "gtest/gtest.h"
#include <gaussHx2.h>
#include "gaussHx2_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 4

class GaussHx2KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *inLine;
	unsigned char *outLineC;
	unsigned char *outLineAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(GaussHx2KernelTest, TestUniformInputLines)
{
	width = 64;

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLine(width * 2 + PADDING, 20);
	inLine[0] = inLine[1] = 0;
	inLine[width + 2] = inLine[width + 3] = 0;

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}


TEST_F(GaussHx2KernelTest, TestUniformInputLinesMinimumWidthSize)
{
	width = 24;

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLine(width * 2 + PADDING, 7);
	inLine[0] = inLine[1] = 0;
	inLine[width + 2] = inLine[width + 3] = 0;

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussHx2KernelTest, TestUniformInputLinesAll0)
{
	width = 1280;

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLine(width * 2 + PADDING, 0);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussHx2KernelTest, TestUniformInputLinesAll255)
{
	width = 1920;

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLine(width * 2 + PADDING, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}


TEST_F(GaussHx2KernelTest, TestIndividualValues)
{
	width = 64;

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLine(width * 2 + PADDING, 0);

	inLine[0] = 0; inLine[1] = 0;
	inLine[2] = 6; inLine[3] =  9; inLine[4] = 32;
	inLine[5] = 12; inLine[6] = 30; inLine[7] =  1;

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	EXPECT_EQ(6, outLineAsm[0]);
	EXPECT_EQ(19, outLineAsm[1]);
	EXPECT_EQ(16, outLineAsm[2]);
	EXPECT_EQ(2, outLineAsm[3]);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussHx2KernelTest, TestRandomInputLines)
{
	width = 48;

	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLine(width * 2 + PADDING, 0, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(TestDifferentInputSizes, GaussHx2KernelTest,
		Values((unsigned int)24, (unsigned int)32, (unsigned int)40, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)808, (unsigned int)1280, (unsigned int)1920);
);


TEST_P(GaussHx2KernelTest, TestRandom)
{
	width = GetParam();

	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLine(width * 2 + PADDING, 0, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussHx2(inLine + 2, outLineC, width);
	GaussHx2_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

