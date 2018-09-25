//convolution5x5s3hhhh kernel test
//Asm function prototype:
//     void Convolution5x5s3hhhh_asm(half** in, half** out, half conv[25], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution5x5s3hhhh_asm_test(half** in, half** out, half conv[25], unsigned int inWidth);
//
//C function prototype:
//     void Convolution5x5s3hhhh(half** in, half** out, half conv[25], u32 inWidth);
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
#include "convolution5x5s3hhhh.h"
#include "convolution5x5s3hhhh_asm_test.h"
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
half  DELTA = 1; //accepted tolerance between C and ASM results
#define SIZE 5
#define KERNEL_SIZE 25

class Convolution5x5s3hhhhKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half convMat[KERNEL_SIZE];
    half *cMat;

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

TEST_F(Convolution5x5s3hhhhKernelTest, TestUniformInputLinesMinimumWidth)
{
    width = 32;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 4);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    inputGen.FillLine(convMat, KERNEL_SIZE, 0.012345);


    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3, DELTA);
}

TEST_F(Convolution5x5s3hhhhKernelTest, TestUniformInputLinesAll0)
{
    width = 640;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 0);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    inputGen.FillLine(convMat, KERNEL_SIZE, 0.012345);

    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3);
}


TEST_F(Convolution5x5s3hhhhKernelTest, TestUniformInputLinesAll255)
{
    width = 160;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 255);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    inputGen.FillLine(convMat, KERNEL_SIZE, 0.012345);

    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3, DELTA);
}

TEST_F(Convolution5x5s3hhhhKernelTest, TestUniformInputLinesRandomConvMatrix)
{
    width = 640;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 3);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    inputGen.SelectGenerator("random");
    inputGen.FillLine(convMat, KERNEL_SIZE, 0.0f, 0.1f);

    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3, DELTA);
}


TEST_F(Convolution5x5s3hhhhKernelTest, TestRandomInputLinesRandomConvMatrix)
{
    width = 320;
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 0, 255);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    inputGen.FillLine(convMat, KERNEL_SIZE, 0.0f, 0.01f);

    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution5x5s3hhhhKernelTest,
        Values((unsigned int)24, (unsigned int)32, (unsigned int)40, (unsigned int)48, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution5x5s3hhhhKernelTest, TestRandomInputLinesBlurFilter)
{
    half   convMatC[KERNEL_SIZE];
    half    convMatAsm[KERNEL_SIZE];

    inputGen.SelectGenerator("uniform");

    inputGen.FillLine(convMat, KERNEL_SIZE, 1.0f / KERNEL_SIZE);

    unsigned int width = GetParam();

    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, SIZE, 0, 255);
    outLinesC = inputGen.GetEmptyLinesFp16(width / 3, 1);
    outLinesAsm = inputGen.GetEmptyLinesFp16(width / 3, 1);

    Convolution5x5s3hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s3hhhhCycleCount / (width / 3));

    inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
    mvcvConvolution5x5s3hhhh(inLines, outLinesC, convMat, width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width / 3, DELTA);
}
