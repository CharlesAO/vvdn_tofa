//CensusTransform11x11 kernel test
//Asm function prototype:
//     void mvcvCensusTransform11x11_asm(u8 **in, u32 *out, u32 width);
//
//Asm test function prototype:
//     void censusTransform11x11_asm_test(unsigned char** in, unsigned int* out, unsigned int width);
//
//C function prototype:
//     void mvcvCensusTransform11x11(u8 **in, u32 *out, u32 width);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// inWidth    - width of input line


#include "gtest/gtest.h"
#include <censusTransform11x11.h>
#include "censusTransform11x11_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define SIZE 11

class CensusTransform11x11KernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}
	
    unsigned short **inLines;
	unsigned char *outLinesC;
	unsigned char *outLinesAsm;
	unsigned int width;
    ArrayChecker outputChecker;
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;


	virtual void TearDown() {}
};

TEST_F(CensusTransform11x11KernelTest, TestUniformInputLinesWidth64)
{
    width = 64;
    unsigned short** inLinesOffseted;
    inputGen.SelectGenerator("uniform");
    
    inLines = inputGen.GetLinesU16(width + PADDING, SIZE, 100);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
	outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		

    mvcvCensusTransform11x11(inLinesOffseted, outLinesC, width);
    censusTransform11x11_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform11x11CycleCount / (width*16));
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11KernelTest, TestMinimumWidthRandomData)
{
    width = 8;
    unsigned short** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLinesU16(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		
    
    mvcvCensusTransform11x11(inLinesOffseted, outLinesC, width);
    censusTransform11x11_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform11x11CycleCount / (width*16));
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11KernelTest, TestMaximumWidthRandomData)
{
    width = 1920;
    unsigned short** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLinesU16(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		
    
    mvcvCensusTransform11x11(inLinesOffseted, outLinesC, width);
    censusTransform11x11_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform11x11CycleCount / (width*16));
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11KernelTest, TestRandomDataRandomWidth)
{
    unsigned short** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    width = randGen->GenerateUInt(0, 1921, 8);
    inLines = inputGen.GetLinesU16(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		
    
    mvcvCensusTransform11x11(inLinesOffseted, outLinesC, width);
    censusTransform11x11_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform11x11CycleCount / (width*16));
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}