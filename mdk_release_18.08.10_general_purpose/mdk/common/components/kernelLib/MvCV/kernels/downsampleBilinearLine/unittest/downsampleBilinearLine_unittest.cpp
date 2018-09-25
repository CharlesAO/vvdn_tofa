
//Parameter description:
// mvcvDownsampleBilinearLine 
// @param[in] inLine           - input line
// @param[out] tempLine        - output buffer for even lines, input buffer for odd lines
// @param[out] outLine         - output result for odd lines
// @param[in] width            - width of the input lines
// @param[in] select           - select on wich line the computation will occure (o-even, 1-odd) 
// @return    Nothing




#include "gtest/gtest.h"
#include <downsampleBilinearLine.h>
#include "downsampleBilinearLine_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16

class mvcvDownsampleBilinearLineKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned short *inLine;
    unsigned int *tempLineC;
    unsigned int *tempLineAsm;
    unsigned short *outLineC;
    unsigned short *outLineAsm;
    unsigned int inWidth;
    unsigned int select;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvDownsampleBilinearLineKernelTest, TestWidth16All0_0)
{
    inWidth = 16;
    select = 0;
    
    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineU16(inWidth, 0);
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    
    
    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

 TEST_F(mvcvDownsampleBilinearLineKernelTest, TestWidth16AllMAX_0)
{
    inWidth = 16;
    select = 0;
    
    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineU16(inWidth, 65535);
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 65535);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 65535);
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    
    
    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

TEST_F(mvcvDownsampleBilinearLineKernelTest, TestWidth16All0_1)
{
    inWidth = 16;
    select = 1;
    
    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineU16(inWidth, 0);
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    memcpy ( (unsigned char *)tempLineAsm, (unsigned char *)tempLineC, inWidth*4 );
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    
    
    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

 TEST_F(mvcvDownsampleBilinearLineKernelTest, TestWidth16AllMAX_1)
{
    inWidth = 16;
    select = 1;
    
    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineU16(inWidth, 65535);
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 65535);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 65535);
    memcpy ( (unsigned char *)tempLineAsm, (unsigned char *)tempLineC, inWidth*4 );
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    
    
    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

TEST_F(mvcvDownsampleBilinearLineKernelTest, TestRandomDATARandomWidth_0)
{
    inWidth = randGen->GenerateUInt(16, 1280, 16);
    select = 0;
    
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineU16(inWidth, 0, 65535);

    
    inputGen.SelectGenerator("uniform");
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 0);
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    

    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

TEST_F(mvcvDownsampleBilinearLineKernelTest, TestRandomDATARandomWidth_1)
{
    inWidth = randGen->GenerateUInt(16, 1280, 16);
    select = 1;
    
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineU16(inWidth, 0, 65535);

    inputGen.SelectGenerator("uniform");
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    memcpy ( (unsigned char *)tempLineAsm, (unsigned char *)tempLineC, inWidth*4 );
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    

    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
}

 TEST_F(mvcvDownsampleBilinearLineKernelTest, TestRandomDATARandom1Width_0)
{
    inWidth = randGen->GenerateUInt(960, 1920, 16);
    select = 0;
    
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineU16(inWidth, 0, 65535);

    inputGen.SelectGenerator("uniform");
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    memcpy ( (unsigned char *)tempLineAsm, (unsigned char *)tempLineC, inWidth*4 );
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    

    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
} 

 TEST_F(mvcvDownsampleBilinearLineKernelTest, TestRandomDATARandom1Width_1)
{
    inWidth = randGen->GenerateUInt(960, 1920, 16);
    select = 1;
    
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineU16(inWidth, 0, 65535);

    inputGen.SelectGenerator("uniform");
    tempLineC = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    tempLineAsm = (unsigned int *)inputGen.GetLineU16(inWidth*2, 10000);
    memcpy ( (unsigned char *)tempLineAsm, (unsigned char *)tempLineC, inWidth*4 );
    outLineC = inputGen.GetLineU16(inWidth, 5);
    outLineAsm = inputGen.GetLineU16(inWidth, 5);
    

    mvcvDownsampleBilinearLine(inLine, tempLineC, outLineC, inWidth, select);
    mvcvDownsampleBilinearLine_asm_test(inLine, tempLineAsm, outLineAsm, inWidth, select);
    
    RecordProperty("CyclePerPixel", mvcvDownsampleBilinearLineCycleCount / (inWidth));    
    
    outputChecker.CompareArrays(tempLineC, tempLineAsm, inWidth);
    outputChecker.CompareArrays(outLineC, outLineAsm, inWidth);
} 