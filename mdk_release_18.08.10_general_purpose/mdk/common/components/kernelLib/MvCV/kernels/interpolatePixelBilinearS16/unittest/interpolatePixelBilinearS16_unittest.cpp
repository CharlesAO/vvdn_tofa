#include "gtest/gtest.h"
#include "interpolatePixelBilinearS16.h"
#include "interpolatePixelBilinearS16_asm_test.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"

#define WIDTH 320

class interpolatePixelBilinearS16Test : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        resultC = 0;
        resultAsm = 0;
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    short* line1;
    short* line2;
    float x;
    float y;
    float resultC;
    float resultAsm;

};

TEST_F(interpolatePixelBilinearS16Test, Test1)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.5;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(12, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test2)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.25;
    y = 14.5;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(11.5, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test3)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.75;
    y = 14.5;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(12.5, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test4)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.25;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(11.5, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test5)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.75;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(12.5, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test6)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.25;
    y = 14.25;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(11, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test7)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.75;
    y = 14.75;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(13, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test8)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.0;
    y = 14.0;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(10, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test9)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.0;
    y = 14.5;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(11, resultC);
    EXPECT_EQ(resultAsm, resultC);
}

TEST_F(interpolatePixelBilinearS16Test, Test10)
{
    inputGen.SelectGenerator("uniform");
    line1 = (short*)inputGen.GetLineU16(WIDTH, 10);
    line2 = (short*)inputGen.GetLineU16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.0;

    resultC = mvcvInterpolatePixelBilinearS16(line1, line2, x, y);
    interpolatePixelBilinearS16_asm_test(line1, line2, x, y, &resultAsm, WIDTH);
    RecordProperty("CycleCount", cycleCount);

    EXPECT_EQ(11, resultC);
    EXPECT_EQ(resultAsm, resultC);
}
