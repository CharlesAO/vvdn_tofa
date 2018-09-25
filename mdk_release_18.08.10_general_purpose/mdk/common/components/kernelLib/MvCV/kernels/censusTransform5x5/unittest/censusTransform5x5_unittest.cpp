//CensusTransform5x5 kernel test
//Asm function prototype:
//     void mvcvCensusTransform5x5_asm(u8 **in, u32 *out, u32 width);
//
//Asm test function prototype:
//     void censusTransform5x5_asm_test(unsigned char** in, unsigned int* out, unsigned int width);
//
//C function prototype:
//     void mvcvCensusTransform5x5(u8 **in, u32 *out, u32 width);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// inWidth    - width of input line


#include "gtest/gtest.h"
#include "censusTransform5x5_asm_test.h"
#include "censusTransform5x5.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define SIZE 5

class CensusTransform5x5KernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}
	
	unsigned int width;
    unsigned char** inLines;
	unsigned int *outLinesC;
	unsigned int *outLinesAsm;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(CensusTransform5x5KernelTest, TestUniformInputLinesWidth16)
{
	width = 64;
	unsigned char** inLinesOffseted;
	inputGen.SelectGenerator("uniform");

	inLines = inputGen.GetLines(width + PADDING, 5, 10);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

	outLinesC = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		
	outLinesAsm = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		

	mvcvCensusTransform5x5(inLinesOffseted, outLinesC, width);
	censusTransform5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", censusTransform5x5CycleCount / width);
    
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform5x5KernelTest, TestMinimumWidthRandomData)
{
	width = 16;
	unsigned char** inLinesOffseted;
	inputGen.SelectGenerator("random");

	inLines = inputGen.GetLines(width + PADDING, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

	outLinesC = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		
	outLinesAsm = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		

	mvcvCensusTransform5x5(inLinesOffseted, outLinesC, width);
	censusTransform5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", censusTransform5x5CycleCount / width);
   
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform5x5KernelTest, TestMaximumWidthRandomData)
{
	width = 1920;
	unsigned char** inLinesOffseted;
	inputGen.SelectGenerator("random");

	inLines = inputGen.GetLines(width + PADDING, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

	outLinesC = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		
	outLinesAsm = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		

	mvcvCensusTransform5x5(inLinesOffseted, outLinesC, width);
	censusTransform5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", censusTransform5x5CycleCount / width);
    
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform5x5KernelTest, TestRandomDataRandomWidth)
{
	unsigned char** inLinesOffseted;
	inputGen.SelectGenerator("random");

	width = randGen->GenerateUInt(0, 1921, 16);
	inLines = inputGen.GetLines(width + PADDING, 5, 0, 255);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

	outLinesC = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		
	outLinesAsm = (unsigned int *)inputGen.GetLine(width * sizeof(unsigned int), 0);		

	mvcvCensusTransform5x5(inLinesOffseted, outLinesC, width);
	censusTransform5x5_asm_test(inLines, outLinesAsm, width);
	RecordProperty("CyclePerPixel", censusTransform5x5CycleCount / width);
    
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}