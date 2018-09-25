//Parameter description:
/// mvcvThresholdDisparityFp16   - filters and counts unreliable pixels in the input confidence map
/// @param[in] input             - confidence map
/// @param[in] countThresPixels  - number of pixels with unreliable disparity
/// @param[in] width             - image line width
/// @param[in] threshold         - pointer to a width x 32 array from path2
/// @param[in] setValue          - new value for unreliable pixels
/// @param[out] output           - disparity map in fp16 precision
/// @return    Nothing  

#include "gtest/gtest.h"
#include <thresholdDisparityFp16.h>
#include "thresholdDisparityFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
half DELTA=0.0f;

class mvcvThresholdDisparityFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
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
    unsigned int *countThresPixelsC;
    unsigned int *countThresPixelsAsm;
    unsigned int width;
    unsigned int threshold;
    half setValue;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvThresholdDisparityFp16KernelTest, Width16All20)
{
    width = 16;
    threshold = 0;
    setValue = 10.0f;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    inputGen.FillLine(outputC,  width*2 + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width*2 + PADDING , 5.0f);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
    input = inputGen.GetLine(width, 20);
    
    mvcvThresholdDisparityFp16(input, (half*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparityFp16_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityFp16CycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityFp16KernelTest, Width16All255)
{
    width = 16;
    threshold = 0;
    setValue = 10.0f;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    inputGen.FillLine(outputC,  width*2 + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width*2 + PADDING , 5.0f);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("uniform");
    input = inputGen.GetLine(width, 255);
    
    mvcvThresholdDisparityFp16(input, (half*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparityFp16_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityFp16CycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityFp16KernelTest, RandomWidthRandomDataSmall)
{
    width = randGen->GenerateUInt(16, 1280, 16);
    threshold = 0;
    setValue = 10.0f;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    inputGen.FillLine(outputC,  width*2 + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width*2 + PADDING , 5.0f);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparityFp16(input, (half*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparityFp16_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityFp16CycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING, DELTA);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityFp16KernelTest, RandomWidthRandomDataBig)
{
    width = randGen->GenerateUInt(1280, 1920, 16);
    threshold = 0;
    setValue = 10.0f;
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    inputGen.FillLine(outputC,  width*2 + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width*2 + PADDING , 5.0f);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparityFp16(input, (half*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparityFp16_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityFp16CycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING, DELTA);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

TEST_F(mvcvThresholdDisparityFp16KernelTest, RandomAll)
{
    width = randGen->GenerateUInt(16, 1920, 16);
    threshold = randGen->GenerateUInt(1, 255, 0);
    setValue = (half)((int)randGen->GenerateUInt(0, 255, 0));
    inputGen.FillLine((half*)&setValue, 1, 0.0f, 255.0f);
	
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    outputAsm = inputGen.GetEmptyLineFp16(width*2 + PADDING);
    inputGen.FillLine(outputC,  width*2 + PADDING , 5.0f);
    inputGen.FillLine(outputAsm,width*2 + PADDING , 5.0f);
	
    countThresPixelsC   = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    countThresPixelsAsm = (unsigned int*)inputGen.GetLine(PADDING * sizeof(unsigned int), 5);
    
	inputGen.SelectGenerator("random");
    input = inputGen.GetLine(width, 0, 255);
    
    mvcvThresholdDisparityFp16(input, (half*)&outputC[PADDING/2], (u32*)&countThresPixelsC[PADDING/2], width, threshold, setValue);
    mvcvThresholdDisparityFp16_asm_test(input, outputAsm, countThresPixelsAsm, width, threshold, setValue);
    
    RecordProperty("CyclePerPixel", mvcvThresholdDisparityFp16CycleCount / (width));

    outputChecker.CompareArrays(outputC, outputAsm, width*2 + PADDING, DELTA);
    outputChecker.CompareArrays(countThresPixelsC, countThresPixelsAsm, PADDING);
}

