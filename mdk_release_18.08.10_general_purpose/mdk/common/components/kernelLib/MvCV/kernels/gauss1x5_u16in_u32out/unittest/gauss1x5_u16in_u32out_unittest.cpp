//Gauss1x5_u16in_u32out kernel test
//Asm function prototype:
//     void mvcvGauss1x5_u16in_u32out_asm(u8** in, u8** out, u32 width);
//
//Asm test function prototype:
//	void gauss1x5_u16in_u32out_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvGauss1x5_u16in_u32out(u8** in, u8** out, u32 width)
//	
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// width     - width of input line (min. 8 )
//


#include "gtest/gtest.h"
#include <gauss1x5_u16in_u32out.h>
#include "gauss1x5_u16in_u32out_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdlib.h>

#define PADDING 16

class Gauss1x5_u16in_u32outKernelTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short **inLines;
	unsigned int **outLinesC;
	unsigned int **outLinesAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

 
//1

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestUniformInputLines32)
{
	int width=32;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 10);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);	
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestUniformInputLinesAllZero)
{
	int width=32;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);	
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestUniformInputLinesAll255)
{
	int width=32;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength8)
{
	int width=16;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
	
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength240)
{
	int width=240;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0,255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);
	
	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength320)
{
	int width=320;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength640)
{
	int width=640;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}


TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength800)
{
	int width=800;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength1280)
{
	int width=1280;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomInputLineLength1920)
{
	int width=1920;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);			
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestEdges)
{
	int j;
	width = 320;
	unsigned short **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
	for(j=0; j<3;j++)
		EXPECT_EQ(outLinesC[0][j], outLinesAsm[0][j]);
	for(j=317; j<320;j++)
		EXPECT_EQ(outLinesC[0][j], outLinesAsm[0][j]);
}

TEST_F(Gauss1x5_u16in_u32outKernelTest, TestRandomWidthRandomDataRandomWidth)
{
	unsigned short **inLinesOffseted;
	width = randGen->GenerateUInt(8, 1921, 8);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 1, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 1, PADDING/2);
	outLinesC = (unsigned int**)inputGen.GetEmptyLines(width * 4, 1);
	outLinesAsm = (unsigned int**)inputGen.GetEmptyLines(width * 4,1);

	mvcvGauss1x5_u16in_u32out(inLinesOffseted, outLinesC, width);
	gauss1x5_u16in_u32out_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gauss1x5_u16in_u32outCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}