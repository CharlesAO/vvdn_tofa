#include "gtest/gtest.h"
#include "fastAtan2Positive_asm_test.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"


#include <math.h>
#include <ctime>
#include <memory>
#define MATH_2PI                6.283185307179586
#define ERROR_THRESHOLD         0.01
typedef struct atanCoord_t
{
    half x;
    half y;
}atanCoord_t;

class fastAtanPositiveTest : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        result = 5.6;
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }
    half localAtanPositive(half y, half x)
    {
        half val = atan2(y,x);
        if (val < (half)0.0)
        {
            val = ((half)MATH_2PI + val);
        }
        return val;
    }
    void testValues (atanCoord_t *values, unsigned int len)
    {
        unsigned int i;
        for (i = 0; i < len; i++)
        {
            half val = localAtanPositive(values[i].y,values[i].x);
            fastAtan2Positive_asm_test(values[i].y,values[i].x, &result);
            EXPECT_LT(fabs(val - result), ERROR_THRESHOLD);
        }
    }


    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    half result;

};

TEST_F(fastAtanPositiveTest, TestSpecialCaseY0)
{
    atanCoord_t values[3]=
    {
        {0.0,-1.0},
        {0.0, 1.0},
        {0.0, 0.5}
    };
    testValues(values, 3);

    RecordProperty("CycleCount", cycleCount);


}
TEST_F(fastAtanPositiveTest, TestSpecialCaseX0)
{
    atanCoord_t values[4]=
    {
        {1.0, 0.0},
        {-1.0, 0.0},
        {-0.5, 0.0},
        {2.0, 0.0}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestSpecialCaseX0Y0)
{
    atanCoord_t values[1]=
    {
        {0.0, 0.0},
    };
    testValues(values, 1);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestSpecialCaseYDivX1)
{
    atanCoord_t values[4]=
    {
        {1.0, 1.0},
        {-1.0, -1.0},
        {-0.5, -0.5},
        {2.125, 2.125}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestSpecialCaseYDivXMinus1)
{
    atanCoord_t values[4]=
    {
        {1.0, -1.0},
        {1.0, -1.0},
        {-0.5, 0.5},
        {-3.0, 3.0}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestYDivX_0_1)
{
    atanCoord_t values[4]=
    {
        {1.0, 2.0},
        {2.0, 3.0},
        {-0.5, -0.8},
        {-3.0, -5.0}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestYDivX_Minus1_0)
{
    atanCoord_t values[4]=
    {
        {1.0, -2.0},
        {-2.0, 3.0},
        {-0.5, 0.8},
        {-3.0, 5.0}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestYDivX_0_inf)
{
    atanCoord_t values[4]=
    {
        {-7.125, -0.875},
        {-80.0, -30.0},
        {15.5, 3.8},
        {-3.75, -1.25}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, TestYDivX_MinusInf_0)
{
    atanCoord_t values[4]=
    {
        {40.0, -2.0},
        {80.0, -30.0},
        {-15.5, 3.8},
        {-3.75, 1.25}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, Test_ResultInFirstQuarter)
{
    atanCoord_t values[4]=
    {
        {2.0625, 9.25},
        {0.1875, 9.25},
        {26.375, 18.75},
        {16.25, 21.0}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, Test_ResultInSecondQuarter)
{
    atanCoord_t values[4]=
    {
        {6.375, -6.125},
        {7.625, -15.0},
        {1.875, -37.75},
        {26.375, -0.375}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, Test_ResultInThirdQuarter)
{
    atanCoord_t values[4]=
    {
        {-4.375, -2.875},
        {-4.875, -1.625},
        {-4.25, -6.5},
        {-26.375, -0.375}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
TEST_F(fastAtanPositiveTest, Test_ResultInFourthQuarter)
{
    atanCoord_t values[4]=
    {
        {-4.375, 2.875},
        {-4.875, 1.625},
        {-4.25, 6.5},
        {-26.375, 0.375}
    };
    testValues(values, 4);
    RecordProperty("CycleCount", cycleCount);
}
