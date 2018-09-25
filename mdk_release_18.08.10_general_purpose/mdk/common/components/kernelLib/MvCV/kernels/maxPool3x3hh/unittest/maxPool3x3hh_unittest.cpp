// maxPool3x3hh kernel test

//Asm function prototype:
//    void mvcvMaxPool3x3hh_asm(half** in, half** out, u32 width);

//Asm test function prototype:
//    void maxPool3x3hh_asm_test(half **input, half **output, unsigned int width);

//C function prototype:
//    void mvcvMaxPool3x3hh(half** in, half** out, u32 width);

//width     - width of output line
//in        - input line
//out       - the output line

#include "gtest/gtest.h"
#include "maxPool3x3hh.h"
#include "maxPool3x3hh_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define POOL_SIZE 3
#define PADDING 8

class maxPool3x3hhTest : public ::testing::Test {
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
    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(maxPool3x3hhTest, TestWidth16RandomInput)
{
    width = 16;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -2000, 2000);

    outLineC = inputGen.GetEmptyLinesFp16(width, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

    maxPool3x3hh_asm_test(inLine, outLineAsm, width);
    RecordProperty("CyclePerPixel", maxPool3x3hhCycleCount / width);

    mvcvMaxPool3x3hh(inLine, outLineC, width);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(maxPool3x3hhTest, TestWidth24RandomInput)
{
    width = 24;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -500, 500);

    outLineC = inputGen.GetEmptyLinesFp16(width, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

    maxPool3x3hh_asm_test(inLine, outLineAsm, width);
    RecordProperty("CyclePerPixel", maxPool3x3hhCycleCount / width);

    mvcvMaxPool3x3hh(inLine, outLineC, width);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(maxPool3x3hhTest, TestWidth1920RandomInput)
{
    width = 1920;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, POOL_SIZE, -1000, 1000);

    outLineC = inputGen.GetEmptyLinesFp16(width, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);

    maxPool3x3hh_asm_test(inLine, outLineAsm, width);
    RecordProperty("CyclePerPixel", maxPool3x3hhCycleCount / width);

    mvcvMaxPool3x3hh(inLine, outLineC, width);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
