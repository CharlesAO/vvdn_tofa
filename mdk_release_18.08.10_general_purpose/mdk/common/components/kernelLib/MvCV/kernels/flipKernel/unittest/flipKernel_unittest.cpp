
//Parameter description:
// mvcvFlipKernel - flips elements of a given line (first element becames last)
// @param[in] in             - pointer to input line
// @param[out] out           - pointer to output line
// @param[in] width          - width of the input line
// @return    Nothing
// 




#include "gtest/gtest.h"
#include <flipKernel.h>
#include "flipKernel_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 32

class mvcvFlipKernelKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *inLine1;
    unsigned char *outLineC;
    unsigned char *outLineAsm;
    unsigned int width;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvFlipKernelKernelTest, TestWidth16All0)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    outLineC   = inputGen.GetEmptyLine(width + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outLineC,  width + PADDING , 5);
    inputGen.FillLine(outLineAsm,width + PADDING , 5);
    
    
    inLine1 = inputGen.GetLine((width), 0);
    
    mvcvFlipKernel(inLine1, outLineC + 16, width);
    mvcvFlipKernel_asm_test(inLine1 , outLineAsm , width);
    
    RecordProperty("CyclePerPixel", mvcvFlipKernelCycleCount / (width ));

    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width * disparities);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width + PADDING);
    
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}

TEST_F(mvcvFlipKernelKernelTest, TestWidth16AllMAX)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    outLineC   = inputGen.GetEmptyLine(width + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outLineC,  width + PADDING , 5);
    inputGen.FillLine(outLineAsm,width + PADDING , 5);
    

    inLine1 = inputGen.GetLine((width), 255);
    
    mvcvFlipKernel(inLine1, outLineC + 16, width);
    mvcvFlipKernel_asm_test(inLine1 , outLineAsm , width);
    
    RecordProperty("CyclePerPixel", mvcvFlipKernelCycleCount / (width ));

    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width * disparities);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width + PADDING);
    
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}

TEST_F(mvcvFlipKernelKernelTest, TestRandomDATARandomWidth)
{
    width = randGen->GenerateUInt(8, 1280, 8); //max width allowed to not overwrite the stack
    
    inputGen.SelectGenerator("uniform");
    outLineC   = inputGen.GetEmptyLine(width + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outLineC,  width + PADDING , 5);
    inputGen.FillLine(outLineAsm,width + PADDING , 5);
    
    
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine((width), 0, 255);
    
    mvcvFlipKernel(inLine1, outLineC + 16, width);
    mvcvFlipKernel_asm_test(inLine1 , outLineAsm , width);
    
    RecordProperty("CyclePerPixel", mvcvFlipKernelCycleCount / (width ));

    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width * disparities);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width + PADDING);
    
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}

TEST_F(mvcvFlipKernelKernelTest, TestRandomDATARandomWidth2)
{
    width = randGen->GenerateUInt(960, 1280, 8); //max width allowed to not overwrite the stack
    //width = 16;
    
    inputGen.SelectGenerator("uniform");
    outLineC   = inputGen.GetEmptyLine(width + PADDING);
    outLineAsm = inputGen.GetEmptyLine(width + PADDING);
    inputGen.FillLine(outLineC,  width + PADDING , 5);
    inputGen.FillLine(outLineAsm,width + PADDING , 5);
    
    
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine((width), 0, 255);
    
    mvcvFlipKernel(inLine1, outLineC + 16, width);
    mvcvFlipKernel_asm_test(inLine1 , outLineAsm , width);
    
    RecordProperty("CyclePerPixel", mvcvFlipKernelCycleCount / (width ));

    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width * disparities);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width + PADDING);
    
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}
