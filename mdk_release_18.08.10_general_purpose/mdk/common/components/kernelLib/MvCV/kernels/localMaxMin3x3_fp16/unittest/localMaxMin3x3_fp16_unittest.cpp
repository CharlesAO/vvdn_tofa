#include "gtest/gtest.h"
#include "localMaxMin3x3_fp16_asm_test.h"
#include "localMaxMin3x3_fp16.h"
#include "mv_types.h"

#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"


#include <ctime>
#include <memory>
#define CAND_X    9
#define PADDING 16

class localMaxMin3x3Test : public ::testing::Test {
protected:

    virtual void SetUp()
    {

        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }



    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
	half* inLines[3];
    half* inLines_c[3];



    u32 width;
    unsigned int maxLocationList_c[640];
    unsigned int minLocationList_c[640];
    unsigned int minCount_c;
    unsigned int maxCount_c;

    unsigned int maxLocationList_asm[640];
    unsigned int minLocationList_asm[640];
    unsigned int minCount_asm;
    unsigned int maxCount_asm;
    //virtual void TearDown() {}
protected:
    unsigned int checkIfExists(unsigned int* values, unsigned int x, unsigned int len)
    {
        unsigned int i;
        for (i = 0; i < len ; i++)
            if (values[i] == x)
                return 1;
        return 0;
    }
};

TEST_F(localMaxMin3x3Test, TestUniformInputLine)
{
	 width = 16;
     minCount_c = 0;
     maxCount_c = 0;
     minCount_asm = 0;
     maxCount_asm = 0;
    inputGen.SelectGenerator("uniform");

	inLines[0] = inputGen.GetLineFloat16(width + PADDING, 0.5);
    inLines[1] = inputGen.GetLineFloat16(width + PADDING, 0.5);
    inLines[2] = inputGen.GetLineFloat16(width + PADDING, 0.5);
    inLines_c[0] = inLines[0] + PADDING/2;
    inLines_c[1] = inLines[1] + PADDING/2;
    inLines_c[2] = inLines[2] + PADDING/2;


    memset(minLocationList_asm,5,width);
    memset(maxLocationList_asm,5,width);

    memset(minLocationList_c,5,width);
    memset(maxLocationList_c,5,width);
    inLines[1][PADDING/2 + 1] = 0;
    (inLines[1][PADDING/2 + width/2 - 2]) = 10;

    localMaxMin3x3_fp16_asm_test(inLines, width/2, minLocationList_asm, maxLocationList_asm, &minCount_asm, &maxCount_asm);
    mvcvLocalMaxMin3x3_fp16(inLines_c, width/2, minLocationList_c,  maxLocationList_c, &minCount_c, &maxCount_c);
    RecordProperty("CycleCount", cycleCount);

    arrCheck.CompareArrays(maxLocationList_asm, maxLocationList_c, width / 2);

    arrCheck.CompareArrays(minLocationList_asm, minLocationList_c, width / 2);

    EXPECT_EQ(minCount_c, minCount_asm);
    EXPECT_EQ(maxCount_c, maxCount_asm);

}

TEST_F(localMaxMin3x3Test, TestUniformInputLine2)
{
    width = 320;
     minCount_c = 0;
     maxCount_c = 0;
     minCount_asm = 0;
     maxCount_asm = 0;
    inputGen.SelectGenerator("uniform");

    inLines[0] = inputGen.GetLineFloat16(width + PADDING, 10);
    inLines[1] = inputGen.GetLineFloat16(width + PADDING, 10);
    inLines[2] = inputGen.GetLineFloat16(width + PADDING, 10);
    inLines_c[0] = inLines[0] + PADDING/2;
    inLines_c[1] = inLines[1] + PADDING/2;
    inLines_c[2] = inLines[2] + PADDING/2;

    memset(minLocationList_asm,5,width);
    memset(maxLocationList_asm,5,width);

    memset(minLocationList_c,5,width);
    memset(maxLocationList_c,5,width);
    inLines[1][PADDING/2 + 1] = 0;
    (inLines[1][PADDING/2 + width/2 - 2]) = 10;

    localMaxMin3x3_fp16_asm_test(inLines, width/2, minLocationList_asm, maxLocationList_asm, &minCount_asm, &maxCount_asm);
    mvcvLocalMaxMin3x3_fp16(inLines_c, width/2, minLocationList_c,  maxLocationList_c, &minCount_c, &maxCount_c);
    RecordProperty("CycleCount", cycleCount);

    arrCheck.CompareArrays(maxLocationList_asm, maxLocationList_c, width / 2);

    arrCheck.CompareArrays(minLocationList_asm, minLocationList_c, width / 2);

    EXPECT_EQ(minCount_c, minCount_asm);
    EXPECT_EQ(maxCount_c, maxCount_asm);

}

TEST_F(localMaxMin3x3Test, TestRandomInputLine)
{
    width = 320;
     minCount_c = 0;
     maxCount_c = 0;
     minCount_asm = 0;
     maxCount_asm = 0;
    inputGen.SelectGenerator("random");

    inLines[0] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[1] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[2] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines_c[0] = inLines[0] + PADDING/2;
    inLines_c[1] = inLines[1] + PADDING/2;
    inLines_c[2] = inLines[2] + PADDING/2;

    memset(minLocationList_asm,5,width);
    memset(maxLocationList_asm,5,width);

    memset(minLocationList_c,5,width);
    memset(maxLocationList_c,5,width);
    inLines[1][PADDING/2 + 1] = 0;
    (inLines[1][PADDING/2 + width/2 - 2]) = 10;

    localMaxMin3x3_fp16_asm_test(inLines, width/2, minLocationList_asm, maxLocationList_asm, &minCount_asm, &maxCount_asm);
    mvcvLocalMaxMin3x3_fp16(inLines_c, width/2, minLocationList_c,  maxLocationList_c, &minCount_c, &maxCount_c);
    RecordProperty("CycleCount", cycleCount);

    arrCheck.CompareArrays(maxLocationList_asm, maxLocationList_c, width / 2);

    arrCheck.CompareArrays(minLocationList_asm, minLocationList_c, width / 2);

    EXPECT_EQ(minCount_c, minCount_asm);
    EXPECT_EQ(maxCount_c, maxCount_asm);

}



TEST_F(localMaxMin3x3Test, TestRandomInputLineRandomWidth)
{
    width = randGen->GenerateUInt(16, 640, 8);
     minCount_c = 0;
     maxCount_c = 0;
     minCount_asm = 0;
     maxCount_asm = 0;
    inputGen.SelectGenerator("random");

    inLines[0] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[1] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[2] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines_c[0] = inLines[0] + PADDING/2;
    inLines_c[1] = inLines[1] + PADDING/2;
    inLines_c[2] = inLines[2] + PADDING/2;

    memset(minLocationList_asm,5,width);
    memset(maxLocationList_asm,5,width);

    memset(minLocationList_c,5,width);
    memset(maxLocationList_c,5,width);
    inLines[1][PADDING/2 + 1] = 0;
    (inLines[1][PADDING/2 + width/2 - 2]) = 10;

    localMaxMin3x3_fp16_asm_test(inLines, width/2, minLocationList_asm, maxLocationList_asm, &minCount_asm, &maxCount_asm);
    mvcvLocalMaxMin3x3_fp16(inLines_c, width/2, minLocationList_c,  maxLocationList_c, &minCount_c, &maxCount_c);
    RecordProperty("CycleCount", cycleCount);

    arrCheck.CompareArrays(maxLocationList_asm, maxLocationList_c, width / 2);

    arrCheck.CompareArrays(minLocationList_asm, minLocationList_c, width / 2);

    EXPECT_EQ(minCount_c, minCount_asm);
    EXPECT_EQ(maxCount_c, maxCount_asm);

}

TEST_F(localMaxMin3x3Test, TestRandomInputLineWidth640)
{
    width = 640;
     minCount_c = 0;
     maxCount_c = 0;
     minCount_asm = 0;
     maxCount_asm = 0;
    inputGen.SelectGenerator("random");

    inLines[0] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[1] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines[2] = inputGen.GetLineFloat16(width + PADDING, -255, 255);
    inLines_c[0] = inLines[0] + PADDING/2;
    inLines_c[1] = inLines[1] + PADDING/2;
    inLines_c[2] = inLines[2] + PADDING/2;

    memset(minLocationList_asm,5,width);
    memset(maxLocationList_asm,5,width);

    memset(minLocationList_c,5,width);
    memset(maxLocationList_c,5,width);
    inLines[1][PADDING/2 + 1] = 0;
    (inLines[1][PADDING/2 + width/2 - 2]) = 10;

    localMaxMin3x3_fp16_asm_test(inLines, width/2, minLocationList_asm, maxLocationList_asm, &minCount_asm, &maxCount_asm);
    mvcvLocalMaxMin3x3_fp16(inLines_c, width/2, minLocationList_c,  maxLocationList_c, &minCount_c, &maxCount_c);
    RecordProperty("CycleCount", cycleCount);

    arrCheck.CompareArrays(maxLocationList_asm, maxLocationList_c, width / 2);

    arrCheck.CompareArrays(minLocationList_asm, minLocationList_c, width / 2);

    EXPECT_EQ(minCount_c, minCount_asm);
    EXPECT_EQ(maxCount_c, maxCount_asm);

}
