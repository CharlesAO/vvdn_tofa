
//Parameter description:
// mvcvComputeADPyrOnePosWindow     - computes aggregated cost over a given path considering min2 and penaltyP2 previously computed
// @param[in] input1            - pointer to previous aggregated costs
// @param[in] input2            - pointer to current matching costs
// @param[out] output             - pointer to output aggregated cost
// @param[in] min2              - minimum value from the previous aggregated costs
// @param[in] penaltyP2           - penalty value for the case where minimum cost is found at more than +/-1 disparity
// @return    Nothing



#include "gtest/gtest.h"
#include <computeADPyrOnePosWindow.h>
#include "computeADPyrOnePosWindow_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;
    
#define PADDING 16
#define OFFSET PADDING/2

class mvcvComputeADPyrOnePosWindowKernelTest : public ::testing::TestWithParam< unsigned int > {
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
    unsigned int disparities;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvComputeADPyrOnePosWindowKernelTest, TestAllRandomValues)
{

    width = randGen->GenerateUInt(1280, 1921, 8);
    //width = 32;
    disparities = 5;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
        outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
    inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
    inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input1 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
    input2 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
    
    mvcvComputeADPyrOnePosWindow(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
    mvcvComputeADPyrOnePosWindow_asm_test(input1, input2, outputAsm, width);
    
        //outputChecker.DumpArrays("input1", "input1.c", input1, (width * disparities + PADDING));
        //outputChecker.DumpArrays("input2", "input2.c", input2, (width * disparities + PADDING));
        //outputChecker.DumpArrays("outputC", "outputC.c", outputC, (width * disparities + PADDING));

    RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosWindowCycleCount / (width * disparities ));

    outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvComputeADPyrOnePosWindowKernelTest, TestAll0)
{
    width = 320;
    disparities = 5;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
    inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
    inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
    
    
    input1 = inputGen.GetLine((width * disparities + PADDING), 0);
    input2 = inputGen.GetLine((width * disparities + PADDING), 0);
    
    mvcvComputeADPyrOnePosWindow(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
    mvcvComputeADPyrOnePosWindow_asm_test(input1, input2, outputAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosWindowCycleCount / (width * disparities ));

    outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvComputeADPyrOnePosWindowKernelTest, TestAllMaxValues)
{
    width = 320;
    disparities = 5;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
    inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
    inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
    
    
    input1 = inputGen.GetLine((width * disparities + PADDING), 255);
    input2 = inputGen.GetLine((width * disparities + PADDING), 255);
    
    mvcvComputeADPyrOnePosWindow(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
    mvcvComputeADPyrOnePosWindow_asm_test(input1, input2, outputAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosWindowCycleCount / (width * disparities ));

    outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvComputeADPyrOnePosWindowKernelTest, TestAllRandomValuesWidth8)
{
    width = 8;
    disparities = 5;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
        outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
    inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
    inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
    
    inputGen.SelectGenerator("random");
    input1 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
    input2 = inputGen.GetLine((width * disparities + PADDING), 0, 255);
    
    mvcvComputeADPyrOnePosWindow(input1 + OFFSET, input2 + OFFSET, outputC + OFFSET, width);
    mvcvComputeADPyrOnePosWindow_asm_test(input1, input2, outputAsm, width);
    
        //outputChecker.DumpArrays("input1", "input1.c", input1, (width * disparities + PADDING));
        //outputChecker.DumpArrays("input2", "input2.c", input2, (width * disparities + PADDING));
        //outputChecker.DumpArrays("outputC", "outputC.c", outputC, (width * disparities + PADDING));

    RecordProperty("CyclePerPixel", mvcvComputeADPyrOnePosWindowCycleCount / (width * disparities ));

    outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}  
