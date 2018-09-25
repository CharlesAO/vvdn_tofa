//GaussVx2 kernel test
//Asm function prototype:
//     void mvcvGaussVx2(u8 **inLine,u8 *outLine,int width);
//
//Asm test function prototype:
//     void GaussVx2_test(unsigned char** in, unsigned char* out, unsigned int width);
//
//C function prototype:
//     void mvcvGaussVx2(u8 **inLine, u8 *outLine, int width);
//
//Parameter description:
// GaussVx2 filter - Apply downscale 2x vertical with gaussian filters with kernel 5x5. 
// inLine  - pointer for input line
// outLine - pointer for output line
// width   - width of input line


#include "gtest/gtest.h"
#include <gaussVx2.h>
#include "gaussVx2_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;


class GaussVx2KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **inLines;
	unsigned char *outLineC;
	unsigned char *outLineAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(GaussVx2KernelTest, TestUniformInputLines)
{
	width = 32;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 5, 10);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussVx2KernelTest, TestUniformInputLinesAllZero)
{
	width = 1920;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 5, 0);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussVx2KernelTest, TestUniformInputLinesAll255)
{
	width = 1280;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 5, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussVx2KernelTest, TestUniformInputLinesMinimumWidth)
{
	width = 16;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width, 5, 30);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussVx2KernelTest, TestRandomInputLines)
{
	width = 48;

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, 5, 0, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(GaussVx2KernelTest, TestRandomWidth)
{
	width =  randGen->GenerateUInt(0, 1920, 8);

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width, 5, 0, 255);

	outLineC = inputGen.GetEmptyLine(width);
	outLineAsm = inputGen.GetEmptyLine(width);

	mvcvGaussVx2(inLines, outLineC, width);
	GaussVx2_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width);
}
