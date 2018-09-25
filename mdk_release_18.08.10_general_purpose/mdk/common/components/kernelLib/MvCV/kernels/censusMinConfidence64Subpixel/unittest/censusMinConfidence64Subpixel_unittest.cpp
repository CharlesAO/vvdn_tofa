
//Parameter description:
/// mvcvCensusMinConfidence64Subpixel   - Compute minimum disparity with subpixel precision considering its left and right neighbour in the cost vector
/// @param[in] in            - input matching cost
/// @param[out] out          - output disparity (minimum position) with subpixel precision
/// @param[out] outm         - output confidence (minimum value)
/// @param[in] width         - line width
/// @return    Nothing
    

#include "gtest/gtest.h"
#include <censusMinConfidence64Subpixel.h>
#include "censusMinConfidence64Subpixel_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define DISPARITIES 64
#define OFFSET PADDING/2
half DELTA = 0.04f;

class mvcvCensusMinConfidence64SubpixelKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *input;
    half *outputC;
    half *outputAsm;
    unsigned char *outputmC;
    unsigned char *outputmAsm;
    unsigned int width;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestUniformData0)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestUniformData255)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 255);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestRandomDataWidth640)
{
    width = 32;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    

    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    //outputChecker.DumpArrays("refInput1", "refInput1.c", input, width * DISPARITIES + PADDING);
    //outputChecker.DumpArrays("outputC", "outputC.c", outputC, width + PADDING);
    //outputChecker.DumpArrays("outputmC", "outputmC.c", outputmC, width + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width + PADDING);

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestRandomDataWidth320)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestRandomDataWidth8)
{
    width = 8;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestRandomDataWidth16)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64SubpixelKernelTest, TestRandomWidthRandomData)
{
    width = randGen->GenerateUInt(8, 640, 8);
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width + PADDING);
    inputGen.FillLine(outputC,  width + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width + PADDING , 5.0f);

    outputmC   = inputGen.GetEmptyLine(width + PADDING);
    outputmAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outputmC,  width + PADDING , 5);
    inputGen.FillLine(outputmAsm,width + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvCensusMinConfidence64Subpixel(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinConfidence64Subpixel_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64SubpixelCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

