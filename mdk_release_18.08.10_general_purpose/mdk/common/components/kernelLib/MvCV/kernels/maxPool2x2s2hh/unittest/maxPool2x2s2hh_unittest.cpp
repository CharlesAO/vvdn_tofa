// maxPool2x2s2hh kernel test

//Asm function prototype:
//    void mvcvMaxPool2x2s2hh_asm(half** in, half** out, u32 outputWidth);

//Asm test function prototype:
//    void maxPool2x2s2hh_asm_test(half **input, half **output, unsigned int width);

//C function prototype:
//    void mvcvMaxPool2x2s2hh(half** in, half** out, u32 outputWidth);

//width     - width of output line
//in        - input line
//out       - the output line

#include "gtest/gtest.h"
#include "maxPool2x2s2hh.h"
#include "maxPool2x2s2hh_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define STRIDE 2
#define SIZE 2
#define PADDING 8

class maxPool2x2s2hhTest : public ::testing::Test {
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

TEST_F(maxPool2x2s2hhTest, TestWidth16RandomInput)
{
    width = 16;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, SIZE, -2000, 2000);

    outLineC = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);

    maxPool2x2s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool2x2s2hhCycleCount / outputWidth);

    mvcvMaxPool2x2s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}

TEST_F(maxPool2x2s2hhTest, TestWidth24RandomInput)
{
    width = 24;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, SIZE, -500, 500);

    outLineC = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);

    maxPool2x2s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool2x2s2hhCycleCount / outputWidth);

    mvcvMaxPool2x2s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}

TEST_F(maxPool2x2s2hhTest, TestWidth1920RandomInput)
{
    width = 1920;
    outputWidth = width / STRIDE;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLinesFloat16(width + PADDING, SIZE, -1000, 1000);

    outLineC = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);
    outLineAsm = inputGen.GetEmptyLinesFp16(width / STRIDE, 1);

    maxPool2x2s2hh_asm_test(inLine, outLineAsm, outputWidth);
    RecordProperty("CyclePerPixel", maxPool2x2s2hhCycleCount / outputWidth);

    mvcvMaxPool2x2s2hh(inLine, outLineC, outputWidth);

    outputCheck.CompareArrays(outLineC[0], outLineAsm[0], outputWidth);
}
