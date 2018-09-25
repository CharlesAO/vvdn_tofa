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
#include <gaussHx2_fp16.h>
#include "gaussHx2_fp16_asm_test.h"
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
half DELTA = 1;

class GaussHx2_fp16KernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(GaussHx2_fp16KernelTest, TestRandomDATASmallWidth)
{
	width = randGen->GenerateUInt(16, 1281, 16);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING*2, 1, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, 8);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvGaussHx2_fp16(inLinesOffseted, outLineC, width);
	GaussHx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width/2, 1, DELTA);
}


TEST_F(GaussHx2_fp16KernelTest, TestRandomDATABigWidth)
{
	width = randGen->GenerateUInt(1280, 1921, 16);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING*2, 1, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, 8);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);


	mvcvGaussHx2_fp16(inLinesOffseted, outLineC, width);
	GaussHx2_fp16_asm_test(inLines, outLineAsm, width);
	RecordProperty("CyclePerPixel", GaussHx2_fp16CycleCount / width);

	outputChecker.CompareArrays(outLineC, outLineAsm, width/2, 1, DELTA);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING*2, 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLineC, width/2 , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLineAsm, width/2 , 1);
}