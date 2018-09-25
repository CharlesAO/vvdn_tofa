#include "gtest/gtest.h"
#include "interpolatePixelBilinear_asm_test.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"


#include <math.h>
#include <ctime>
#include <memory>
#define WIDTH 320

class interpolatePixelBilinearTest : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        result = 0;
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    half* line1;
    half* line2;
    float x;
    float y;
    half result;

};

TEST_F(interpolatePixelBilinearTest, Test1)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.5;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(12, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test2)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.25;
    y = 14.5;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(11.5, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test3)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.75;
    y = 14.5;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(12.5, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test4)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.25;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(11.5, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test5)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.75;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(12.5, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test6)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.25;
    y = 14.25;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(11, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test7)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.75;
    y = 14.75;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(13, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test8)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.0;
    y = 14.0;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(10, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test9)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.0;
    y = 14.5;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(11, (float)result);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(interpolatePixelBilinearTest, Test10)
{
    inputGen.SelectGenerator("uniform");
    line1 = inputGen.GetLineFloat16(WIDTH, 10);
    line2 = inputGen.GetLineFloat16(WIDTH, 12);
    line1[13] = 12;
    line2[13] = 14;
    x = 12.5;
    y = 14.0;
    interpolatePixelBilinear_asm_test(line1, line2, x, y, &result, WIDTH);
    EXPECT_EQ(11, (float)result);
    RecordProperty("CycleCount", cycleCount);
}

