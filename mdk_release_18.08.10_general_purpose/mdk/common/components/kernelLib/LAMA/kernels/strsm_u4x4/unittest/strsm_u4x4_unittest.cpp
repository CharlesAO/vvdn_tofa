//Asm function prototype:
//     void lamaStrsm_u4x4_asm(float *a, float *b, float *c, inc_t rs_c, inc_t cs_c)
//
//Asm test function prototype:
//     void strsm_u4x4_asm_test(float *a, float *b, float *c, inc_t rs_c, inc_t cs_c)
//
//C function prototype:
//     void lamaStrsm_u4x4(float *a, float *b, float *c, inc_t rs_c, inc_t cs_c)
//
//Parameter description:
// a     - address of input matrix (upper triangular)
// b     - address of input matrix
// c     - address of the output matrix
// rs_c  - row stride
// cs_c  - column stride

#include "gtest/gtest.h"
///
#include "strsm_u4x4.h"
#include "strsm_u4x4_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA      0.5
#define MIN_VALUE -10.0f
#define MAX_VALUE  10.0f

class strsm_u4x4KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    float *a;
    float *b;
    float *c_C;
    float *c_Asm;
    int rs_c;
    int cs_c;
    int outputSize;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(strsm_u4x4KernelTest, TestRowStride4ColumnStride1)
{
    rs_c = 4;
    cs_c = 1;
    outputSize = 3 * (rs_c + cs_c) + 1;

    inputGen.SelectGenerator("random");
    a   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    b   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    c_C = inputGen.GetLineFloat(outputSize, 0.0);
    c_Asm = inputGen.GetLineFloat(outputSize, 0.0);

    for(int i = 1; i < 4; i++)
        for(int j = 0; j < i; j++)
            a[i*4+j] = 0.0f;
        
    lamaStrsm_u4x4(a, b, c_C, rs_c, cs_c);
    strsm_u4x4_asm_test(a, b, c_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", strsm_u4x4CycleCount);

    outputChecker.CompareArraysFloat(c_C, c_Asm, outputSize, DELTA);
}


//-------------------- parameterized tests -------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, strsm_u4x4KernelTest,
        Values((unsigned int)4, (unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)64);
);

TEST_P(strsm_u4x4KernelTest, TestColumnStride4RowStrideParameterized)
{
    rs_c = GetParam() * 4;
    cs_c = 4;
    outputSize = 3 * (rs_c + cs_c) + 1;

    inputGen.SelectGenerator("random");
    a = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    b = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    c_C = inputGen.GetLineFloat(outputSize, 0.0);
    c_Asm = inputGen.GetLineFloat(outputSize, 0.0);

    for(int i = 1; i < 4; i++)
        for(int j = 0; j < i; j++)
            a[i*4+j] = 0.0f;

    lamaStrsm_u4x4(a, b, c_C, rs_c, cs_c);
    strsm_u4x4_asm_test(a, b, c_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", strsm_u4x4CycleCount);

    outputChecker.CompareArraysFloat(c_C, c_Asm, outputSize, DELTA);
}

TEST_P(strsm_u4x4KernelTest, TestColumnStrideParameterizedRowStride256)
{
    rs_c = 256;
    cs_c = GetParam();
    outputSize = 3 * (rs_c + cs_c) + 1;

    inputGen.SelectGenerator("random");
    a = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    b = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    c_C = inputGen.GetLineFloat(outputSize, 0.0);
    c_Asm = inputGen.GetLineFloat(outputSize, 0.0);

    for(int i = 1; i < 4; i++)
        for(int j = 0; j < i; j++)
            a[i*4+j] = 0.0f;

    lamaStrsm_u4x4(a, b, c_C, rs_c, cs_c);
    strsm_u4x4_asm_test(a, b, c_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", strsm_u4x4CycleCount);

    outputChecker.CompareArraysFloat(c_C, c_Asm, outputSize, DELTA);
}
