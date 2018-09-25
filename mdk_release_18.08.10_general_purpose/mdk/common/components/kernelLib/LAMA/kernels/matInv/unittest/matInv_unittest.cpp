//MatInverse kernel test
//Asm function prototype:
//     void mvcvMatInverseLU_asm(float *A, float *B);
//
//Asm test function prototype:
//     void matInv_asm_test(float *A, float *B);
//
//C function prototype:
//     void mvcvMatInverseLU(float *A, float *B);
//
//Parameter description:
// A - input vector of size 1*K
// B - input matrix of size K*N

#include "gtest/gtest.h"
///
#include <matInv.h>
#include "matInv_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>
#include <cstdio>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA      5.0e-2f   //accepted tolerance between C and ASM results
#define EDGE 16
class matInvKernelTest : public ::testing::TestWithParam < unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned int width;
    unsigned int height;

    float *matA;
    float *matC_c;
    float *matC_asm;

    MAT *A;
    MAT *C_c;
    MAT *C_asm;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(matInvKernelTest, TestMatrix32x32)
{
    width = 32;
    height = 32;
    inputGen.SelectGenerator("random");

    A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

    A->max_m = 128;             C_c->max_m = 128;              C_asm->max_m  = 128;
    A->max_n = 128;             C_c->max_n = 128;              C_asm->max_n  = 128;
    A->m     = height;          C_c->m     = height;           C_asm->m      = height;
    A->n     = width;           C_c->n     = width;            C_asm->n      = width;

    A->base  = (float*)(inputGen.GetLineFloat(width * height, 0, 255));
    C_c->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));
    C_asm->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));

    memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));

    C_c   = mvcvMatInverseLU(A, C_c);
    C_asm =(MAT*) matInv_asm_test((MAT_test *)A, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matInvCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, DELTA);
}

TEST_F(matInvKernelTest, RandomInputAddition)
{
    width = 16;
    height = 16;

    inputGen.SelectGenerator("random");

    A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

    A->max_m = 128;              C_c->max_m = 128;               C_asm->max_m  = 128;
    A->max_n = 128;              C_c->max_n = 128;               C_asm->max_n  = 128;
    A->m     = height;           C_c->m     = height;            C_asm->m      = height;
    A->n     = width;            C_c->n     = width;             C_asm->n      = width;

    A->base  = (float*)(inputGen.GetLineFloat(width * height, 0, 255));
    C_c->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));
    C_asm->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));

    memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));

    C_c   = mvcvMatInverseLU(A, C_c);
    C_asm =(MAT*) matInv_asm_test((MAT_test *)A, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matInvCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, DELTA);
}

TEST_F(matInvKernelTest, TestMatrix10x10)
{
    width = 10;
    height = 10;

    inputGen.SelectGenerator("random");

    A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

    A->max_m = 128;             C_c->max_m = 128;              C_asm->max_m  = 128;
    A->max_n = 128;             C_c->max_n = 128;              C_asm->max_n  = 128;
    A->m     = height;          C_c->m     = height;           C_asm->m      = height;
    A->n     = width;           C_c->n     = width;            C_asm->n      = width;

    A->base  = (float*)(inputGen.GetLineFloat(width * height, 0, 255));
    C_c->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));
    C_asm->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));

    memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));

    C_c   = mvcvMatInverseLU(A, C_c);
    C_asm =(MAT*) matInv_asm_test((MAT_test *)A, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matInvCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, DELTA);
}


TEST_F(matInvKernelTest, TestMatrixRandom)
{
    width = randGen->GenerateUInt(1, 32);
    height = width;
    inputGen.SelectGenerator("random");

    A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
    C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

    A->max_m = 128;                C_c->max_m = 128;               C_asm->max_m  = 128;
    A->max_n = 128;                C_c->max_n = 128;               C_asm->max_n  = 128;
    A->m     = height;          C_c->m     = height;           C_asm->m      = height;
    A->n     = width;           C_c->n     = width;            C_asm->n      = width;

     A->base  = (float*)(inputGen.GetLineFloat(width * height, 0, 255));
    C_c->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));
    C_asm->base = (float*)(inputGen.GetLineFloat(width * height + EDGE, 0, 255));

    memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));

    C_c   = mvcvMatInverseLU(A, C_c);
    C_asm =(MAT*) matInv_asm_test((MAT_test *)A, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matInvCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, DELTA);
}


