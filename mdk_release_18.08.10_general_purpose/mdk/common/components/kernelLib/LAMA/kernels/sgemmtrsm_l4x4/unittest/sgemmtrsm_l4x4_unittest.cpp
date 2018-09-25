//Asm function prototype:
//     void sgemmtrsm_l4x4_asm(s32 k, float *alpha, float *a10, float *a11, float *b01, float *b11, float *c11, s32 rs_c, s32 cs_c);
//
//Asm test function prototype:
//     void sgemmtrsm_l4x4_asm_test(s32 k, float *alpha, float *a, float *b, float *b11, float *c, s32 rs_c, s32 cs_c);
//
//C function prototype:
//     void sgemmtrsm_l4x4(s32 k, float *alpha, float *a10, float *a11, float *b01, float *b11, float *c11, s32 rs_c, s32 cs_c);
//
//Parameter description:
// k     - the number of columns of a10 and rows of b01
// alpha - the address of a scalar to be applied to b11
// a10   - 4 x k submatrix of the packed (column-stored) micro-panel of A, situated to the left of the 4 x 4 triangular submatrix a11
// a11   - 4 x 4 lower triangular submatrix within the packed (column-stored) micro-panel of matrix A, situated to the right of a10
// b01   - k x 4 submatrix of the packed (row-stored) micro-panel of B, situated above the 4 x 4 block b11
// b11   - 4 x 4 submatrix of the packed (row-stored) micro-panel of B, situated below b01
// c11   - 4 x 4 submatrix of matrix C, stored according to rsc and csc
// rs_c  - row stride of c11
// cs_c  - column stride of c11

#include "gtest/gtest.h"
///
#include "sgemmtrsm_l4x4.h"
#include "sgemmtrsm_l4x4_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA       1.0e-5f //accepted tolerance between C and ASM results
#define MIN_VALUE -10.0f
#define MAX_VALUE  10.0f
#define MIN_SCALAR  1.0f
#define MAX_SCALAR  2.0f

class sgemmtrsm_l4x4KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());

        rs_c = 4;
        cs_c = 1;
    }

    void CopyLine(float *dest, float *src, int size)
    {
        int i;
        for(i = 0; i < size; i++)
        {
            dest[i] = src[i];
        }
    }

    void CompareResults(float *expected, float *actual, int size, float delta)
    {
        int   i;
        float diff, a, b;

        for(i = 0; i < size; i++)
        {
            a = expected[i];
            b = actual[i];
            if(a != b)
            {
                if(a > b)
                    diff = fabs(a - b);
                else
                    diff = fabs(b - a);
                if(fabs(a) > 1.0)
                    diff = fabs(diff/a);
                else if(fabs(a) > 1.0)
                    diff = fabs(diff/b);
                EXPECT_LE(diff, delta);
            }
        }
    }

    s32   k;
    float *alpha;
    float *A10;
    float *A11_C;
    float *A11_Asm;
    float *B01;
    float *B11_C;
    float *B11_Asm;
    float *C11_C;
    float *C11_Asm;
    s32   rs_c;
    s32   cs_c;
    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(sgemmtrsm_l4x4KernelTest, TestMatrixAIsNull)
{
    k       = 512;
    inputGen.SelectGenerator("random");
    alpha   = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    B01     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B11_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    B11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(B11_Asm, B11_C, 4 * 4);
    inputGen.SelectGenerator("uniform");
    A10     = inputGen.GetLineFloat(4 * k, 0.0);
    A11_C   = inputGen.GetLineFloat(4 * 4, 0.0);
    A11_Asm = inputGen.GetLineFloat(4 * 4, 0.0);

    C11_C   = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    C11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));

    mvcvsgemmtrsm_l4x4(k, alpha, A10, A11_C, B01, B11_C, C11_C, rs_c, cs_c);
    sgemmtrsm_l4x4_asm_test(k, alpha, A10, A11_Asm, B01, B11_Asm, C11_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemmtrsm_l4x4CycleCount);

    outputChecker.ExpectAllEQ<float>(C11_Asm, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(B11_C, B11_Asm, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(C11_C, C11_Asm, 4 * 4, 0.0);
}

TEST_F(sgemmtrsm_l4x4KernelTest, TestMatrixBIsNull)
{
    k     = 512;
    inputGen.SelectGenerator("random");
    alpha = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A10     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    A11_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    A11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(A11_Asm, A11_C, 4 * 4);
    inputGen.SelectGenerator("uniform");
    B01     = inputGen.GetLineFloat(4 * k, 0.0);
    B11_C   = inputGen.GetLineFloat(4 * 4, 0.0);
    B11_Asm = inputGen.GetLineFloat(4 * 4, 0.0);

    C11_C   = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    C11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));

    mvcvsgemmtrsm_l4x4(k, alpha, A10, A11_C, B01, B11_C, C11_C, rs_c, cs_c);
    sgemmtrsm_l4x4_asm_test(k, alpha, A10, A11_Asm, B01, B11_Asm, C11_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemmtrsm_l4x4CycleCount);

    outputChecker.ExpectAllEQ<float>(C11_Asm, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(B11_C, B11_Asm, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(C11_C, C11_Asm, 4 * 4, 0.0);
}

TEST_F(sgemmtrsm_l4x4KernelTest, TestAlphaIsNull)
{
    k       = 512;
    inputGen.SelectGenerator("random");
    A10     = inputGen.GetLineFloat(4 * k, 1/MIN_VALUE, 1/MAX_VALUE);
    A11_C   = inputGen.GetLineFloat(4 * 4, 1/MIN_VALUE, 1/MAX_VALUE);
    A11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(A11_Asm, A11_C, 4 * 4);
    B01     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B11_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    B11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(B11_Asm, B11_C, 4 * 4);
    inputGen.SelectGenerator("uniform");
    alpha   = inputGen.GetLineFloat(1, 0.0);

    C11_C   = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    C11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));

    mvcvsgemmtrsm_l4x4(k, alpha, A10, A11_C, B01, B11_C, C11_C, rs_c, cs_c);
    sgemmtrsm_l4x4_asm_test(k, alpha, A10, A11_Asm, B01, B11_Asm, C11_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemmtrsm_l4x4CycleCount);

    outputChecker.CompareArraysFloat(C11_C, B11_C, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(C11_Asm, B11_Asm, 4 * 4, 0.0);
    CompareResults(C11_C, C11_Asm, 4 * 4, DELTA);
}
//-------------------- parameterized tests -------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, sgemmtrsm_l4x4KernelTest,
        Values((unsigned int)0, (unsigned int)4, (unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)64, (unsigned int)128, (unsigned int)512, (unsigned int)1024, (unsigned int)2048);
);

TEST_P(sgemmtrsm_l4x4KernelTest, TestDifferentK)
{
    k = GetParam();
    inputGen.SelectGenerator("random");
    alpha   = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A10     = inputGen.GetLineFloat(4 * k, 1/MIN_VALUE, 1/MAX_VALUE);
    A11_C   = inputGen.GetLineFloat(4 * 4, 1/MIN_VALUE, 1/MAX_VALUE);
    A11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(A11_Asm, A11_C, 4 * 4);
    B01     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B11_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    B11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(B11_Asm, B11_C, 4 * 4);

    C11_C   = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    C11_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));

    mvcvsgemmtrsm_l4x4(k, alpha, A10, A11_C, B01, B11_C, C11_C, rs_c, cs_c);
    sgemmtrsm_l4x4_asm_test(k, alpha, A10, A11_Asm, B01, B11_Asm, C11_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemmtrsm_l4x4CycleCount);

    outputChecker.CompareArraysFloat(C11_C, B11_C, 4 * 4, 0.0);
    outputChecker.CompareArraysFloat(C11_Asm, B11_Asm, 4 * 4, 0.0);
    CompareResults(C11_C, C11_Asm, 4 * 4, DELTA);
}

