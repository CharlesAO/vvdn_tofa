//CensusTransform11x11u8 kernel test
//Asm function prototype:
//     void mvcvCensusTransform11x11u8_asm(u8 **in, u32 *out, u32 width);
//
//Asm test function prototype:
//     void censusTransform11x11u8_asm_test(unsigned char** in, unsigned int* out, unsigned int width);
//
//C function prototype:
//     void mvcvCensusTransform11x11u8(u8 **in, u32 *out, u32 width);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// inWidth    - width of input line


#include "gtest/gtest.h"
#include <censusTransform11x11u8.h>
#include "censusTransform11x11u8_asm_test.h"
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

class CensusTransform11x11u8KernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}
	
    unsigned char **inLines;
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
	
    void showDescriptors(u8* descriptors, u32 width)
    {
	for (unsigned int i = 0; i < width; i++)
        {
	    printf("%02d ", i);
	    for (int j = 0; j < 16; j++)
                printf("%02x", descriptors[i * 16 + j]);
	    printf("\n");
        }
    }
};

TEST_F(CensusTransform11x11u8KernelTest, TestUniformInputLinesWidth64)
{
    width = 64;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("uniform");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 100);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
	outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		

    mvcvCensusTransform11x11u8(inLinesOffseted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11u8KernelTest, TestMinimumWidthRandomData)
{
    width = 16;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		
    
    mvcvCensusTransform11x11u8(inLinesOffseted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING, 11);
	//outputChecker.DumpArrays("refOut", "refOut.c", outLinesC, width * 16);
}

TEST_F(CensusTransform11x11u8KernelTest, TestMaximumWidthRandomData)
{
    width = 1920;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		

    mvcvCensusTransform11x11u8(inLinesOffseted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11u8KernelTest, TestRandomDataRandomWidth)
{
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    width = randGen->GenerateUInt(16, 1921, 16);
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = inputGen.GetLine(width * 16, 0);		
	outLinesAsm = inputGen.GetLine(width * 16, 0);		
    
    mvcvCensusTransform11x11u8(inLinesOffseted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width*16);
}

TEST_F(CensusTransform11x11u8KernelTest, TestMinWidthAllPixelsSaturated)
{
    width = 16;
    unsigned char** inLinesOffsetted;

    inLines = inputGen.GetLines(width + PADDING, SIZE, 254, 255);
    inLinesOffsetted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

    outLinesC = inputGen.GetLine(width * 16, 0);
    outLinesAsm = inputGen.GetLine(width * 16, 0);

    mvcvCensusTransform11x11u8(inLinesOffsetted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);

    // If the centers and the rest of the pixels have the same value, all
    // the descriptor bits should be 0
    outputChecker.ExpectAllEQ<u8>(outLinesC, width * 16, 0);
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width * 16);
}

TEST_F(CensusTransform11x11u8KernelTest, TestMinWidthAllPixelsSaturatedButCenters)
{
    width = 16;
    unsigned char** inLinesOffsetted;

    inLines = inputGen.GetLines(width + PADDING, SIZE, 254, 255);
    inLinesOffsetted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

    memset(inLines[5], 0, width);

    outLinesC = inputGen.GetLine(width * 16, 0);
    outLinesAsm = inputGen.GetLine(width * 16, 0);

    mvcvCensusTransform11x11u8(inLinesOffsetted, outLinesC, width);
    censusTransform11x11u8_asm_test(inLines, outLinesAsm, width);

    outputChecker.CompareArrays(outLinesC, outLinesAsm, width * 16);

    //showDescriptors(outLinesC, width);
    //showDescriptors(outLinesAsm, width);
}
