#include "sampleApp.h"
#include "sampleApp_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

class SampleAppTest :  public ::testing::TestWithParam< unsigned int > {
 protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *input;
    unsigned char *outAsm;
    unsigned char *outC;
    unsigned char value;
    unsigned char operation;
    unsigned char pxToChange;
    unsigned int width;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker outputCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(SampleAppTest , RandomInputAddition)
{

    inputGen.SelectGenerator("random");
    width = 64;
    input  = inputGen.GetLine(width, 0, 255);
    value  = randGen->GenerateUChar(0,255);
    outAsm = inputGen.GetEmptyLine(width);
    outC = inputGen.GetEmptyLine(width);
    pxToChange = 16;
    operation = 1; //addition
    SampleApp_asm_test(input, width, value, operation, pxToChange, outAsm);
    sampleApp(input, width, value, operation, pxToChange, outC);
    RecordProperty("CycleCount", SampleAppCycleCount);
    outputCheck.CompareArrays(outC, outAsm, width);
}

TEST_F(SampleAppTest , RandomInputSubtraction)
{

    inputGen.SelectGenerator("random");
    width = 64;
    input  = inputGen.GetLine(width, 0, 255);
    value  = randGen->GenerateUChar(0,255);
    outAsm = inputGen.GetEmptyLine(width);
    outC = inputGen.GetEmptyLine(width);
    pxToChange = 16;
    operation = 0; //subtraction
    SampleApp_asm_test(input, width, value, operation, pxToChange, outAsm);
    sampleApp(input, width, value, operation, pxToChange, outC);
    RecordProperty("CycleCount", SampleAppCycleCount);
    outputCheck.CompareArrays(outC, outAsm, width);
}

TEST_F(SampleAppTest , UniformInputAddition)
{

    inputGen.SelectGenerator("uniform");
    width = 64;
    input  = inputGen.GetLine(width, 50);
    value  = randGen->GenerateUChar(0,255);
    outAsm = inputGen.GetEmptyLine(width);
    outC = inputGen.GetEmptyLine(width);
    pxToChange = 16;
    operation = 1; //addition
    SampleApp_asm_test(input, width, value, operation, pxToChange, outAsm);
    sampleApp(input, width, value, operation, pxToChange, outC);
    RecordProperty("CycleCount", SampleAppCycleCount);
    outputCheck.CompareArrays(outC, outAsm, width);
}

TEST_F(SampleAppTest , UniformInputSubtraction)
{

    inputGen.SelectGenerator("uniform");
    width = 64;
    input  = inputGen.GetLine(width, 50);
    value  = randGen->GenerateUChar(0,255);
    outAsm = inputGen.GetEmptyLine(width);
    outC = inputGen.GetEmptyLine(width);
    pxToChange = 16;
    operation = 0; //subtraction
    SampleApp_asm_test(input, width, value, operation, pxToChange, outAsm);
    sampleApp(input, width, value, operation, pxToChange, outC);
    RecordProperty("CycleCount", SampleAppCycleCount);
    outputCheck.CompareArrays(outC, outAsm, width);
}

//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, SampleAppTest,
                        Values(8u, 32u, 320u, 640u, 800u, 1280u, 1920u);
);

TEST_P(SampleAppTest, TestWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = GetParam();
    input  = inputGen.GetLine(width, 0, 255);
    value  = randGen->GenerateUChar(0,255);
    outAsm = inputGen.GetEmptyLine(width);
    outC = inputGen.GetEmptyLine(width);
    pxToChange = (width == 8) ? 8 : randGen->GenerateUInt(8,width, 8);
    operation = 0; //subtraction
    SampleApp_asm_test(input, width, value, operation, pxToChange, outAsm);
    sampleApp(input, width, value, operation, pxToChange, outC);
    RecordProperty("CycleCount", SampleAppCycleCount);
    outputCheck.CompareArrays(outC, outAsm, width);
}
