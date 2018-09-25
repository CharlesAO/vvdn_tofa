//Gauss kernel test
//Asm function prototype:
//     void mvcvGauss_asm(u8** in, u8** out, u32 width);
//
//Asm test function prototype:
//	void gauss_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvGauss(u8** in, u8** out, u32 width)
//	
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// width     - width of input line (min. 8 )
//


#include "gtest/gtest.h"
#include <gauss.h>
#include "gauss_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdlib.h>

#define PADDING 8
#define DELTA 1

class GaussKernelTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **inLines;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

 
//1

TEST_F(GaussKernelTest, TestUniformInputLines32)
{
	int width=32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

//2

TEST_F(GaussKernelTest, TestUniformInputLinesAllZero)
{
	int width=32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING*2, 5, 0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

//3

TEST_F(GaussKernelTest, TestUniformInputLinesAll255)
{
	int width=32;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING*2, 5, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
			
}

//4

TEST_F(GaussKernelTest, TestRandomInputLineLength8)
{
	int width=16;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
}

//5

TEST_F(GaussKernelTest, TestRandomInputLineLength240)
{
	int width=240;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 5, 0,255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);
	
	
	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
	
	
			
}

//6

TEST_F(GaussKernelTest, TestRandomInputLineLength320)
{
	int width=320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 5,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
			
}

//7

TEST_F(GaussKernelTest, TestRandomInputLineLength640)
{
	int width=640;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 5,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
			
}

//8

TEST_F(GaussKernelTest, TestRandomInputLineLength800)
{
	int width=800;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 5,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
			
}

//9

TEST_F(GaussKernelTest, TestRandomInputLineLength1280)
{
	int width=1280;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING*2, 5,0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
			
}


//10

TEST_F(GaussKernelTest, TestEdges)
{
	int j;
	width = 320;
	unsigned char **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING*2, 5, 1);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width,1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width,DELTA);
	for(j=0; j<3;j++)
		EXPECT_NEAR(outLinesC[0][j], outLinesAsm[0][j], DELTA);
	for(j=317; j<320;j++)
		EXPECT_NEAR(outLinesC[0][j], outLinesAsm[0][j], DELTA);
}


TEST_F(GaussKernelTest, TestRandomWidthRandomDataRandomWidth)
{
	unsigned char **inLinesOffseted;
	width = randGen->GenerateUInt(0, 1921);
	width = (width >> 3)<<3; //(line width have to be mutiple of 8)
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING * 2, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 5, 8);

	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width , 1);

	mvcvGauss(inLinesOffseted, outLinesC, width);
	gauss_asm_test(inLines, outLinesAsm,width);
	RecordProperty("CyclePerPixel", gaussCycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, DELTA);
}