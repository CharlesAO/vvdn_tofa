//censusTransformAverageRefMask7x7 kernel test
/// CENSUSTRANSFORMAVERAGEREF7X7 kernel - this kernel performs comparision between the average of windows size of (7x7)
/// and pixels in windows of 7x7 based on a mask that takes in consideration only the odd pixels
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

#include "gtest/gtest.h"
#include "censusTransformAverageRefMask7x7_asm_test.h"
#include "censusTransformAverageRefMask7x7.h"
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

class censusTransformAverageRefMask7x7KernelTest : public ::testing::TestWithParam< unsigned long > {
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

TEST_F(censusTransformAverageRefMask7x7KernelTest, TestUniformInputLinesWidth64)
{
    width = 64;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("uniform");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 100);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);		
    outLinesAsm = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);			
    
    mvcvCensusTransformAverageRefMask7x7(inLinesOffseted, (u32*)outLinesC, width);
    censusTransformAverageRefMask7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransformAverageRefMask7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(censusTransformAverageRefMask7x7KernelTest, TestMinimumWidthRandomData)
{
    width = 16;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);		
    outLinesAsm = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);	
    
    mvcvCensusTransformAverageRefMask7x7(inLinesOffseted, (u32*)outLinesC, width);
    censusTransformAverageRefMask7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransformAverageRefMask7x7CycleCount / width);

    //outputChecker.DumpArrays("matInput", "matInput.c", mat1, 49 , 1);
    //outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING , 7);    
    //outputChecker.DumpArrays("refOutput", "refOutput.c", (unsigned char*)outLinesC, width* sizeof(unsigned int));
    //outputChecker.DumpArrays("asmOutput", "asmOutput.c", (unsigned char*)outLinesAsm, width* sizeof(unsigned int));

    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(censusTransformAverageRefMask7x7KernelTest, TestMaximumWidthRandomData)
{
    width = 1920;
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);		
    outLinesAsm = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);
    
    mvcvCensusTransformAverageRefMask7x7(inLinesOffseted, (u32*)outLinesC, width);
    censusTransformAverageRefMask7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransformAverageRefMask7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(censusTransformAverageRefMask7x7KernelTest, TestRandomDataRandomWidth)
{
    unsigned char** inLinesOffseted;
    inputGen.SelectGenerator("random");
    
    width = randGen->GenerateUInt(16, 1921, 16);
    inLines = inputGen.GetLines(width + PADDING, SIZE, 0, 255);
    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    
    outLinesC = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);		
    outLinesAsm = (unsigned int*)inputGen.GetLine(width * sizeof(unsigned int), 0);		
    
    mvcvCensusTransformAverageRefMask7x7(inLinesOffseted, (u32*)outLinesC, width);
    censusTransformAverageRefMask7x7_asm_test(inLines, outLinesAsm, width);
    RecordProperty("CyclePerPixel", censusTransformAverageRefMask7x7CycleCount / width);
    
    outputChecker.CompareArrays(outLinesC, outLinesAsm, width);
}

