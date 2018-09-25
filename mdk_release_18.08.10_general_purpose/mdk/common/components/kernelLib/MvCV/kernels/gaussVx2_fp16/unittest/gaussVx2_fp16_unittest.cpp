//GaussVx2_fp16 kernel test
//Asm function prototype:
//     void mvcvGaussVx2_fp16(u8 **inLine,u8 *outLine,int width);
//
//Asm test function prototype:
//     void GaussVx2_fp16_test(unsigned char** in, unsigned char* out, unsigned int width);
//
//C function prototype:
//     void mvcvGaussVx2_fp16(u8 **inLine, u8 *outLine, int width);
//
//Parameter description:
// GaussVx2_fp16 filter - downsample even lines and even cols
// inLine  - array of pointers to input lines
// outLine - pointer for output line
// width   - width of input line

#include "gtest/gtest.h"
#include <gaussVx2_fp16.h>
#include "gaussVx2_fp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = 1;

class GaussVx2_fp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half **outLineC;
	half **outLineAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(GaussVx2_fp16KernelTest, TestUniformInputLines)
{
	width = 32;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 5, 2);

	outLineC =  inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm =  inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}


TEST_F(GaussVx2_fp16KernelTest, TestUniformInputLinesMinimumWidthSize)
{
	width = 16;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 5, 7);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}


TEST_F(GaussVx2_fp16KernelTest, TestUniformInputLinesAll0)
{
	width = 328;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 5, 0);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}


TEST_F(GaussVx2_fp16KernelTest, TestUniformInputLinesAll255)
{
	width = 1024;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 5, 255);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}

TEST_F(GaussVx2_fp16KernelTest, TestIndividualValues)
{
	width = 64;

	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width, 5, 0);

	inLines[0][0] = 2; inLines[0][1] = 12; inLines[0][2] = 6;
	inLines[0][3] = 9; inLines[0][4] = 32; inLines[0][5] = 7;

	inLines[1][0] = 8; inLines[1][1] = 0; inLines[1][2] = 24;
	inLines[1][3] = 40; inLines[1][4] = 5; inLines[1][5] = 18;

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);


	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}


TEST_F(GaussVx2_fp16KernelTest, TestRandomInputLines)
{
	width = 48;

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 5, 0, 255);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}

TEST_F(GaussVx2_fp16KernelTest, TestRandomDATASmallWidth)
{
	width = randGen->GenerateUInt(8, 1281, 8);

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 5, 0.0, 255.0);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
}

TEST_F(GaussVx2_fp16KernelTest, TestRandomDATABigWidth)
{
	width = randGen->GenerateUInt(1280, 1921, 8);

	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, 5, 0.0, 255.0);

	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvGaussVx2_fp16(inLines, outLineC, width);
	GaussVx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussVx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width, 1, DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width, 5);

	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLineC, width , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLineAsm, width , 1);
}