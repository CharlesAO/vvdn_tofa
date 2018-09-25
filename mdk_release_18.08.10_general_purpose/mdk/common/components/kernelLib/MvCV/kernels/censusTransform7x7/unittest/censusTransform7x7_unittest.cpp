//CensusTransform7x7 kernel test
//Asm function prototype:
//     void mvcvCensusTransform7x7_asm(u8 **in, u32 *out, u32 width);
//
//Asm test function prototype:
//     void censusTransform7x7_asm_test(unsigned char** in, unsigned int* out, unsigned int width);
//
//C function prototype:
//     void mvcvCensusTransform7x7(u8 **in, u32 *out, u32 width);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// inWidth    - width of input line

#include "gtest/gtest.h"
#include "censusTransform7x7_asm_test.h"
#include "censusTransform7x7.h"
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
#define SIZE 7

class CensusTransform7x7KernelTest : public ::testing::TestWithParam< unsigned long > {
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
    unsigned long long *outLinesC;
    unsigned long long *outLinesAsm;
    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;
    
    virtual void TearDown() {}
};

TEST_F(CensusTransform7x7KernelTest, TestUniformInputLinesWidth64)
{
    width = 64;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("uniform");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 100);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);		
    outLinesAsm = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);			
    
    mvcvCensusTransform7x7(inLinesOffseted, (u64*)outLinesC, width);
    censusTransform7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform7x7KernelTest, TestMinimumWidthRandomData)
{
    width = 16;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);		
    outLinesAsm = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);	
    
    mvcvCensusTransform7x7(inLinesOffseted, (u64*)outLinesC, width);
    censusTransform7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform7x7KernelTest, TestMaximumWidthRandomData)
{
    width = 1920;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);		
    outLinesAsm = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);
    
    mvcvCensusTransform7x7(inLinesOffseted, (u64*)outLinesC, width);
    censusTransform7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(CensusTransform7x7KernelTest, TestRandomDataRandomWidth)
{
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    width = randGen->GenerateUInt(8, 1921, 16);
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);		
    outLinesAsm = (unsigned long long*)inputGen.GetLine(width * sizeof(unsigned long long), 0);		
    
    mvcvCensusTransform7x7(inLinesOffseted, (u64*)outLinesC, width);
    censusTransform7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransform7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}
