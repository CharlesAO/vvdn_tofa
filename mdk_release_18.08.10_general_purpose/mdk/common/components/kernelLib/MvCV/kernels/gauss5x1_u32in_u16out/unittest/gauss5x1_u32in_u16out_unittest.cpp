//Gauss5x1_u32in_u16out kernel test
//Asm function prototype:
//     void mvcvGauss5x1_u32in_u16out_asm(u8** in, u8** out, u32 width);
//
//Asm test function prototype:
//    void Gauss5x1_u32in_u16out_asm_test(unsigned char** in, unsigned char** out, unsigned int width);
//
//C function prototype:
//     void mvcvGauss5x1_u32in_u16out(u8** in, u8** out, u32 width)
//    
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// width     - width of input line (min. 8 )
//


#include "gtest/gtest.h"
#include <gauss5x1_u32in_u16out.h>
#include "gauss5x1_u32in_u16out_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdlib.h>

#define DELTA 1

class Gauss5x1_u32in_u16outKernelTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned int **inLines;
    unsigned short **outLinesC;
    unsigned short **outLinesAsm;
    unsigned int width;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

 
//1

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestUniformInputLines32)
{
    int width=32;
    
    inputGen.SelectGenerator("uniform");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 1);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
    
}

//2

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestUniformInputLinesAllZero)
{
    int width=32;
    
    inputGen.SelectGenerator("uniform");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
    
}

//3

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestUniformInputLinesAllMax)
{
    int width=32;
    
    inputGen.SelectGenerator("uniform");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}

//4

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength8)
{
    int width=16;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
    
}

//5

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength240)
{
    int width=240;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}

//6

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength320)
{
    int width=320;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}

//7

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength640)
{
    int width=640;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}

//8

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength800)
{
    int width=800;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}

//9

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomInputLineLength1280)
{
    int width=1280;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
            
}


//10

TEST_F(Gauss5x1_u32in_u16outKernelTest, TestEdges)
{
    int width=320;
    int j;
    
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
    for(j=0; j<3;j++)
        EXPECT_NEAR(outLinesC[0][j], outLinesAsm[0][j], DELTA);
    for(j=317; j<320;j++)
        EXPECT_NEAR(outLinesC[0][j], outLinesAsm[0][j], DELTA);
}

    
TEST_F(Gauss5x1_u32in_u16outKernelTest, TestRandomWidthRandomDataRandomWidth)
{
    int width;
    
    width = randGen->GenerateUInt(8, 1921,8);
    inputGen.SelectGenerator("random");
    inLines = (unsigned int **)inputGen.GetLines(width * sizeof(unsigned int), 5, 0, 255);
    
    outLinesC = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short), 1);
    outLinesAsm = (unsigned short **)inputGen.GetEmptyLines(width * sizeof(unsigned short),1);

    mvcvGauss5x1_u32in_u16out(inLines, outLinesC, width);
    Gauss5x1_u32in_u16out_asm_test(inLines, outLinesAsm,width);
    RecordProperty("CyclePerPixel", Gauss5x1_u32in_u16outCycleCount / width);

    outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}
