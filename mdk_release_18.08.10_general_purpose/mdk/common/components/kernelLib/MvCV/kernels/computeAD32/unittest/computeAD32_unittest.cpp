
//Parameter description:
/// mvcvComputeAD32             - compute absolute difference between each pixel from left image and 32 left pixels from right image
/// @param[in] inputLeft        - pointer to left image
/// @param[in] inputRight       - pointer to right image
/// @param[out] output             - pointer to output - absolute difference
/// @param[in] width            - image width
/// @return    Nothing



#include "gtest/gtest.h"
#include <computeAD32.h>
#include "computeAD32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 32
#define DISPARITIES 32

class mvcvComputeAD32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *input1;
    unsigned char *input2;
    unsigned char *outputAsm;
    unsigned char *outputC;
    unsigned int width;
    unsigned int flag;
    unsigned int disparities;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvComputeAD32KernelTest, TestAll0)
{
    width = 320;
    flag = 0;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 0);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 0);

    mvcvComputeAD32(input1, input2 + PADDING, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);
    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
}

TEST_F(mvcvComputeAD32KernelTest, TestAllMaxValues)
{
    width = 960;
    flag = 0;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);    
    
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 255);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 255);
    
    mvcvComputeAD32(input1, input2 + PADDING, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);
    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
}

TEST_F(mvcvComputeAD32KernelTest, TestAllRandomValues)
{
    width = randGen->GenerateUInt(1280, 1921, 8);
    flag = 0;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    
    inputGen.SelectGenerator("random");
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvComputeAD32(input1, input2 + PADDING, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);

    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
} 

TEST_F(mvcvComputeAD32KernelTest, TestAll0_flag)
{
    width = 320;
    flag = 1;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 0);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 0);

    mvcvComputeAD32(input1, input2, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);
    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
}

TEST_F(mvcvComputeAD32KernelTest, TestAllMaxValues_flag)
{
    width = 960;
    flag = 1;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);    
    
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 255);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 255);
    
    mvcvComputeAD32(input1, input2, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);
    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
}

TEST_F(mvcvComputeAD32KernelTest, TestAllRandomValues_flag)
{
    width = randGen->GenerateUInt(1280, 1921, 8);
    flag = 1;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    
    inputGen.SelectGenerator("random");
    input1 = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    input2 = inputGen.GetLine((width * DISPARITIES + PADDING), 0, 255);
    
    mvcvComputeAD32(input1, input2, outputC, width, flag);
    mvcvComputeAD32_asm_test(input1, input2, outputAsm, width, flag);

    RecordProperty("CyclePerPixel", mvcvComputeAD32CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES);
} 

