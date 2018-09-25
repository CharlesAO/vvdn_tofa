
//Parameter description:
/// mvcvCensusMinSubpixel3Window   - Compute minimum disparity with subpixel precision considering its left and right neighbour in the cost vector
/// @param[in] in            - input matching cost
/// @param[out] out          - output disparity (minimum position) with subpixel precision
/// @param[out] outm         - output confidence (minimum value)
/// @param[in] width         - line width
/// @return    Nothing
    

#include "gtest/gtest.h"
#include <censusMinSubpixel3Window.h>
#include "censusMinSubpixel3Window_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define DISPARITIES 5
#define OFFSET PADDING/2
half DELTA = 0.0f;

class mvcvCensusMinSubpixel3WindowKernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestUniformData0)
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
    
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestUniformData255)
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
    
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestRandomDataWidth640)
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
    


    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    //outputChecker.DumpArrays("input", "input.c", input, (width * DISPARITIES + PADDING));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestRandomDataWidth320)
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
 
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestRandomDataWidth8)
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
    
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestRandomDataWidth16)
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
    
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

TEST_F(mvcvCensusMinSubpixel3WindowKernelTest, TestRandomWidthRandomData)
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
 
    mvcvCensusMinSubpixel3Window(input, outputC + OFFSET, outputmC + OFFSET, width);
    mvcvCensusMinSubpixel3Window_asm_test(input, outputAsm, outputmAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinSubpixel3WindowCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width + PADDING, DELTA);
    outputChecker.CompareArrays(outputmC, outputmAsm, width + PADDING);
}

