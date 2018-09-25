//MatVecMul kernel test
//Asm function prototype:
//     void mvcvMatVecMul_asm(float *A, float *B, float *C, int K, int N);
//
//Asm test function prototype:
//     void matVecMul_asm_test(float *A, float *B, float *C, int K, int N);
//
//C function prototype:
//     void mvcvMatVecMul(float *A, float *B, float *C, int K, int N);
//
//Parameter description:
// A - input vector of size 1*K
// B - input matrix of size K*N
// C - input/output vector of size 1*N
// K - width of A vector
// N - number of columns of matrix B

#include "gtest/gtest.h"
///
#include <matVecMul.h>
#include "matVecMul_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA          5.0e-2f //accepted tolerance between C and ASM results
#define MIN_VALUE    -10.0f
#define MAX_VALUE     10.0f
#define MIN_SCALAR     1.0f
#define MAX_SCALAR     2.0f

class matVecMulKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    void FillPanel(float *in, float *out, int size);
    void TransposeMatrix(float *matrix, int size);
    void InverseMatrix(float *in, float *out, int size);

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

    s32 k;
    s32 n;
    float *A;
    float *B;
    float *C_C;
    float *C_Asm;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(matVecMulKernelTest, TestMatrix128x128)
{
    k = 128;
    n = 128;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}


TEST_F(matVecMulKernelTest, TestMatrix16x1)
{
    k = 16;
    n = 1;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}

TEST_F(matVecMulKernelTest, TestMatrix64x1)
{
    k = 64;
    n = 1;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}

TEST_F(matVecMulKernelTest, TestMatrix32x7)
{
    k = 32;
    n = 7;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}

TEST_F(matVecMulKernelTest, TestMatrix32x128)
{
    k = 32;
    n = 128;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}

TEST_F(matVecMulKernelTest, TestMatrix128x32)
{
    k = 128;
    n = 32;

    inputGen.SelectGenerator("random");
    A   = inputGen.GetLineFloat(k, MIN_VALUE, MAX_VALUE);
    B   = inputGen.GetLineFloat(k * n, MIN_VALUE, MAX_VALUE);
    C_C = inputGen.GetLineFloat(n, MIN_VALUE, MAX_VALUE);

    C_Asm = (float*)inputGen.GetEmptyLine(n * sizeof(float));
    CopyLine(C_Asm, C_C, n);

    mvcvMatVecMul(A, B, C_C, k, n);
    matVecMul_asm_test(A, B, C_Asm, k, n);
    RecordProperty("CycleCount", matVecMulCycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, n, DELTA);
}