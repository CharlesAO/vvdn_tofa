//convolution7x7 kernel test
//Asm function prototype:
//     void mvcvConvolution7x7s2hhhh_asm(half** in, half** out, half conv[49], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution7x7s2hhhh_asm_test(half** in, half** out, half conv[49], unsigned int inWidth);
//
//C function prototype:
//     void mvcvConvolution7x7s2hhhh(half** in, half** out, half conv[49], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convolution7x7s2hhhh.h"
#include "convolution7x7s2hhhh_asm_test.h"
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
half DELTA = 3;//accepted tolerance between C and ASM results.
half DELTA2 = 0.1;//accepted tolerance between C and ASM results for smaller input values

class Convolution7x7s2hhhhKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half convMat[49];
    half **inLines;
    half **outLinesC;
    half **outLinesAsm;
    unsigned int width;
    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};


TEST_F(Convolution7x7s2hhhhKernelTest, TestRandomInputLinesRandomConvMatrix)
{
    width = randGen->GenerateUInt(16, 1921, 8);
    half **inLinesOffseted;
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 0.0, 100.0);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 2, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 2, 1);

    inLinesOffseted = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);

    inputGen.FillLine(convMat, SIZE*SIZE, 0.01f, 0.03f);

    mvcvConvolution7x7s2hhhh(inLinesOffseted, outLinesC, convMat, width);
    Convolution7x7s2hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution7x7s2hhhhCycleCount / (width / 2));

    outputChecker.CompareArrays(outLinesC, outLinesAsm, width / 2, 1, DELTA);
}

//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution7x7s2hhhhKernelTest,
        Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution7x7s2hhhhKernelTest, TestRandomInputLinesBlurFilter)
{
    unsigned int width = GetParam();

    inputGen.SelectGenerator("uniform");

    inputGen.FillLine(convMat, SIZE*SIZE, 0.02f);
    inputGen.SelectGenerator("random");
    inLines =  inputGen.GetLinesFloat16(width + PADDING, SIZE, 0, 100);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 2, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 2, 1);

    Convolution7x7s2hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution7x7s2hhhhCycleCount / (width / 2));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution7x7s2hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC,outLinesAsm, width / 2, 1, DELTA);
}

TEST_P(Convolution7x7s2hhhhKernelTest, TestRandomInputLinesRandomMatrix)
{
    unsigned int width = GetParam();
    inputGen.SelectGenerator("random");

    inputGen.FillLine(convMat, SIZE*SIZE, 0.0f, 0.07f);

    inputGen.SelectGenerator("random");
    inLines =  inputGen.GetLinesFloat16(width + PADDING, SIZE, 0.0, 1.0f);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 2, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 2, 1);

    Convolution7x7s2hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution7x7s2hhhhCycleCount / (width / 2));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution7x7s2hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC, outLinesAsm, width / 2, 1, DELTA2);
}
