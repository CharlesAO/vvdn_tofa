//matrixVectorMultfp16x4 kernel test
//Asm function prototype:
//     void matrixVectorMultfp16x4_asm(half **inArrayA, half *inVecB, u32 num_elements, half *outVec);
//
//Asm test function prototype:
//     void matrixVectorMultfp16x4_asm(half **inArrayA, half *inVecB, u32 num_elements, half *outVec);
//
//C function prototype:
//     void matrixVectorMultfp16x4(half **inArrayA, half *inVecB, u32 num_elements, half *outVec);
//
//Parameter description:
// inArrayA     - Input rows of the 4xN matrix
// inVecB       - Nx1 vector
// num_elements - N - number of elements
// outVec       - 4x1 result vector

#include "gtest/gtest.h"
///
#include "matrixVectorMultfp16x4.h"
#include "matrixVectorMultfp16x4_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = 1;//accepted tolerance between C and ASM results

class matrixVectorMultfp16x4KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

    half **inLinesA;
    half* inLineB;
    half* outLineC;
    half* outLineAsm;
    unsigned int width;
    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(matrixVectorMultfp16x4KernelTest, InputAIsNull)
{
    width = TEST_ARRAY_SIZE;
    inputGen.SelectGenerator("uniform");
    inLinesA   = inputGen.GetLinesFloat16(width, 4, 0.0f);
    inLineB    = inputGen.GetLineFloat16(width, 1.0f);
    outLineC   = inputGen.GetLineFloat16(4, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(4, 0.0f);

    matrixVectorMultfp16x4_asm_test(inLinesA, inLineB, width, outLineAsm);
    mvcvmatrixVectorMultfp16x4(inLinesA, inLineB, width, outLineC);
    RecordProperty("CyclePerPixel", matrixVectorMultfp16x4CycleCount / width);

    outputCheck.CompareArrays(outLineC, outLineAsm, 4);
}

TEST_F(matrixVectorMultfp16x4KernelTest, InputBIsNull)
{
    width = TEST_ARRAY_SIZE;
    inputGen.SelectGenerator("uniform");
    inLinesA   = inputGen.GetLinesFloat16(width, 4, 1.0f);
    inLineB    = inputGen.GetLineFloat16(width, 0.0f);
    outLineC   = inputGen.GetLineFloat16(4, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(4, 0.0f);

    mvcvmatrixVectorMultfp16x4(inLinesA, inLineB, width, outLineC);
    matrixVectorMultfp16x4_asm_test(inLinesA, inLineB, width, outLineAsm);
    RecordProperty("CyclePerPixel", matrixVectorMultfp16x4CycleCount / width);

    outputCheck.CompareArrays(outLineC, outLineAsm, 4);
}

TEST_F(matrixVectorMultfp16x4KernelTest, TestUniformInput)
{
    width = TEST_ARRAY_SIZE;
    inputGen.SelectGenerator("uniform");
    inLinesA   = inputGen.GetLinesFloat16(width, 4, 1.0f);
    inLineB    = inputGen.GetLineFloat16(width, 2.0f);
    outLineC   = inputGen.GetLineFloat16(4, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(4, 0.0f);

    mvcvmatrixVectorMultfp16x4(inLinesA, inLineB, width, outLineC);
    matrixVectorMultfp16x4_asm_test(inLinesA, inLineB, width, outLineAsm);
    RecordProperty("CyclePerPixel", matrixVectorMultfp16x4CycleCount / width);

    outputCheck.CompareArrays(outLineC, outLineAsm, 4);
}

TEST_F(matrixVectorMultfp16x4KernelTest, TestRandomInput)
{
    width = TEST_ARRAY_SIZE;
    inputGen.SelectGenerator("random");
    inLinesA   = inputGen.GetLinesFloat16(width, 4, -2.0f, 2.0f);
    inLineB    = inputGen.GetLineFloat16(width, -2.0f, 2.0f);
    outLineC   = inputGen.GetLineFloat16(4, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(4, 0.0f);

    mvcvmatrixVectorMultfp16x4(inLinesA, inLineB, width, outLineC);
    matrixVectorMultfp16x4_asm_test(inLinesA, inLineB, width, outLineAsm);
    RecordProperty("CyclePerPixel", matrixVectorMultfp16x4CycleCount / width);

    outputCheck.CompareArrays(outLineC, outLineAsm, 4, DELTA);
}

TEST_F(matrixVectorMultfp16x4KernelTest, TestRandomInputRandomWidth)
{
	inputGen.SelectGenerator("random");
    width = randomGen.GenerateUInt(96, TEST_ARRAY_SIZE, 16);
    inLinesA   = inputGen.GetLinesFloat16(width, 4, 0.0f, 1.0f);
    inLineB    = inputGen.GetLineFloat16(width, 0.0f, 1.0f);
    outLineC   = inputGen.GetLineFloat16(4, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(4, 0.0f);

    mvcvmatrixVectorMultfp16x4(inLinesA, inLineB, width, outLineC);
    matrixVectorMultfp16x4_asm_test(inLinesA, inLineB, width, outLineAsm);
    RecordProperty("CyclePerPixel", matrixVectorMultfp16x4CycleCount / width);

    outputCheck.CompareArrays(outLineC, outLineAsm, 4, DELTA);
}
