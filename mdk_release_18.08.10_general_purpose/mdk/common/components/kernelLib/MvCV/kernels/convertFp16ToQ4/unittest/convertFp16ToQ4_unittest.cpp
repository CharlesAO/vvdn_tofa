/// convert fp16 values to Q4 (fixed point with 4 fractional bits)
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

#include "gtest/gtest.h"
#include "convertFp16ToQ4.h"
#include "convertFp16ToQ4_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include "half.h"


class ConvertFp16ToQ4Test : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half *inLine;
    unsigned short *outputC;
    unsigned short *outputAsm;
    unsigned int width;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(ConvertFp16ToQ4Test , TestRandomDataWidth8)
{
    width = 8;

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestRandomDataWidth16)
{
    width = 16;

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestRandomDataWidth64)
{
    width = 64;

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestData0Width320)
{
    width = 320;

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineFloat16(width, 0);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestData255Width640)
{
    width = 640;

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLineFloat16(width, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestRandomDataSmallWidth)
{
    width = randGen->GenerateUInt(8, 1280, 8);
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}

TEST_F(ConvertFp16ToQ4Test , TestRandomDataBigWidth)
{
    width = randGen->GenerateUInt(1280, 1920, 8);

    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetLineU16(width, 5);
    outputAsm = inputGen.GetLineU16(width, 5);

    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);

    ConvertFp16ToQ4_asm_test(inLine, outputAsm, width);
    mvcvConvertFp16ToQ4(inLine, outputC, width);
    
    RecordProperty("CycleCount", ConvertFp16ToQ4CycleCount);
    outputChecker.CompareArrays(outputAsm, outputC, width);
}


