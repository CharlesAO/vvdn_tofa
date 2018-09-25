    
//Parameter description:
/// mvcvMatmulBT_xxii      - matrix multiplication
/// @param[in] A            - first array
/// @param[in] B            - second array
/// @param[out] C           - resulted array
/// @param[in] m            - number of lines
/// @param[in] k            - common dimension
/// @param[in] n            - number of columns
/// @param[in] wA            - rows of array A
/// @param[in] wB            - columns of array B
/// @param[in] wC            - rows of array C
/// @return    Nothing




#include "gtest/gtest.h"
#include <matmulBT_xxii.h>
#include "matmulBT_xxii_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 32

class mvcvMatmulBT_xxiiKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *matA;
    unsigned char *matB;
    unsigned int *matC;
    unsigned int *matCc;

    unsigned int m; //number of lines for out Matrix
    unsigned int k; //common dimmension between matrix
    unsigned int n; //number of columns for out Matrix
    unsigned int wA; //number of lines for matA 
    unsigned int wB; //number of columns for matB
    unsigned int wC; //number of lines for matC


    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestAll5)
{
    m = 16; 
    k = 32;
    n = 24;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("uniform");
    matA = inputGen.GetLine( m * k, 5);
    matB = inputGen.GetLine( k * n, 5);
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 5);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 5);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestAll255)
{
    m = 16; 
    k = 48;
    n = 32;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("uniform");
    matA = inputGen.GetLine( m * k, 255);
    matB = inputGen.GetLine( k * n, 255);
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestWidth64AllRandom)
{
    m = 64; 
    k = 64;
    n = 64;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("random");
    matA = inputGen.GetLine( m * k, 0, 255);
    matB = inputGen.GetLine( k * n, 0, 255);
    inputGen.SelectGenerator("uniform");
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestWidth80AllRandom)
{
    m = 80; 
    k = 80;
    n = 80;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("random");
    matA = inputGen.GetLine( m * k, 0, 255);
    matB = inputGen.GetLine( k * n, 0, 255);
    inputGen.SelectGenerator("uniform");
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestWidth32AllRandom)
{
    m = 32; 
    k = 32;
    n = 32;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("random");
    matA = inputGen.GetLine( m * k, 0, 255);
    matB = inputGen.GetLine( k * n, 0, 255);
    inputGen.SelectGenerator("uniform");
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestWidth16AllRandom)
{
    m = 16; 
    k = 16;
    n = 16;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("random");
    matA = inputGen.GetLine( m * k, 0, 255);
    matB = inputGen.GetLine( k * n, 0, 255);
    inputGen.SelectGenerator("uniform");
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

TEST_F(mvcvMatmulBT_xxiiKernelTest, TestWidth96AllRandom)
{
    m = 96; 
    k = 96;
    n = 96;
    wA = m;
    wB = n;
    wC = n;
    
    inputGen.SelectGenerator("random");
    matA = inputGen.GetLine( m * k, 0, 255);
    matB = inputGen.GetLine( k * n, 0, 255);
    inputGen.SelectGenerator("uniform");
    matC = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    matCc = (unsigned int *)inputGen.GetLine( (m * n) * sizeof(unsigned int), 255);
    
    mvcvMatmulBT_xxii(matA, matB, matCc, m, k, n, wA, wB, wC);
    mvcvMatmulBT_xxii_asm_test(matA, matB, matC, m, k, n, wA, wB, wC);
    
    RecordProperty("CyclePerPixel", mvcvMatmulBT_xxiiCycleCount / (m * n));
    //debug purpose
    //outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
    //outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
    //outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
    //outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
    //
    //
    outputChecker.CompareArrays(matCc, matC, m * n);
}

