// maxPool3x3s2hh kernel test

//Asm function prototype:
//    void mvcvMaxPool3x3s2hh_asm(half** in, half** out, u32 outputWidth);

//Asm test function prototype:
//    void maxPool3x3s2hh_asm_test(half **input, half **output, unsigned int outputWidth);

//C function prototype:
//    void mvcvMaxPool3x3s2hh(half** in, half** out, u32 outputWidth);

//width     - width of output line
//in        - input line
//out       - the output line

#include "gtest/gtest.h"
#include "maxPool3x3s2hh.h"
#include "maxPool3x3s2hh_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define STRIDE 2
#define POOL_SIZE 3
#define PADDING 8

class maxPool3x3s2hhTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half** inLine;
    half** outLineC;
    half** outLineAsm;
    unsigned int width;
    unsigned int outputWidth;
    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(maxPool3x3s2hhTest, TestWidth16RandomInput)
{
    width = 32;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -2000, 2000);

    outLineC = inputGen.GetEmptyLinesFp16(outputWidth, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(outputWidth, 1);

    maxPool3x3s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool3x3s2hhCycleCount / outputWidth);

    mvcvMaxPool3x3s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}

TEST_F(maxPool3x3s2hhTest, TestWidth24RandomInput)
{
    width = 40;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -500, 500);

    outLineC = inputGen.GetEmptyLinesFp16(outputWidth, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(outputWidth, 1);

    maxPool3x3s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool3x3s2hhCycleCount / outputWidth);

    mvcvMaxPool3x3s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}

TEST_F(maxPool3x3s2hhTest, TestWidth1920RandomInput)
{
    width = 1920;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -1000, 1000);

    outLineC = inputGen.GetEmptyLinesFp16(outputWidth, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(outputWidth, 1);

    maxPool3x3s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool3x3s2hhCycleCount / outputWidth);

    mvcvMaxPool3x3s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}
