//Parameter description:
// mvcvThresholdDisparity   - filters and counts unreliable pixels in the input confidence map
// @param[in] input             - confidence map
// @param[in] countThresPixels  - number of pixels with unreliable disparity
// @param[in] width             - image line width
// @param[in] threshold         - pointer to a width x 32 array from path2
// @param[in] setValue          - new value for unreliable pixels
// @param[out] output           - disparity map
// @return    Nothing    

#include "gtest/gtest.h"
#include <thresholdDisparity.h>
#include "thresholdDisparity_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16

class mvcvThresholdDisparityKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *input;
    unsigned char *outputC;
    unsigned char *outputAsm;
    unsigned int *countThresPixelsC;
    unsigned int *countThresPixelsAsm;
    unsigned int width;
    unsigned int threshold;
    unsigned int setValue;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvThresholdDisparityKernelTest, Width16All20)
{
    width = 16;
	threshold = 0;
	setValue = 10;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLine(width*2 + PADDING, 5);
    outputAsm = inputGen.GetLine(width*2 + PADDING, 5);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
    input = inputGen.GetLine(width, 20);
    
    mvcvThresholdDisparity(input, (u8*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparity_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityKernelTest, Width16All255)
{
    width = 16;
	threshold = 0;
	setValue = 10;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLine(width*2 + PADDING, 5);
    outputAsm = inputGen.GetLine(width*2 + PADDING, 5);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
    input = inputGen.GetLine(width, 255);
    
    mvcvThresholdDisparity(input, (u8*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparity_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityKernelTest, RandomWidthRandomDataSmall)
{
    width = randGen->GenerateUInt(16, 1280, 16);
	threshold = 0;
	setValue = 10;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLine(width*2 + PADDING, 5);
    outputAsm = inputGen.GetLine(width*2 + PADDING, 5);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparity(input, (u8*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparity_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityKernelTest, RandomWidthRandomDataBig)
{
    width = randGen->GenerateUInt(1280, 1920, 16);
	threshold = 0;
	setValue = 10;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLine(width*2 + PADDING, 5);
    outputAsm = inputGen.GetLine(width*2 + PADDING, 5);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparity(input, (u8*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparity_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityKernelTest, RandomAll)
{
    width = randGen->GenerateUInt(16, 1920, 16);
	threshold = randGen->GenerateUInt(1, 255, 0);
	setValue = randGen->GenerateUInt(0, 255, 0);
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLine(width*2 + PADDING, 5);
    outputAsm = inputGen.GetLine(width*2 + PADDING, 5);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparity(input, (u8*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparity_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityCycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

