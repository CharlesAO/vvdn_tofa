//matrixInv3x3_fp32 kernel test
//Asm function prototype:
//     void matrixInv3x3_fp32_asm(fp32 **in, fp32 **out);
//
//Asm test function prototype:
//     void matrixInv3x3_fp32_asm_test(fp32 **in, fp32 **out);
//
//C function prototype:
//     void matrixInv3x3_fp32(fp32 **in, fp32 **out);
//
//Parameter description:
// in         - array of pointers to input matrix
// out        - array of pointers to output matrix


#include "gtest/gtest.h"
#include "matrixInv3x3_fp32.h"
#include "matrixInv3x3_fp32_unittest.h"
///
#include "matrixInv3x3_fp32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;


#define DELTA 0.2f //accepted tolerance between C and ASM results
#define SIZE 3
class MatrixInv3x3_fp32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float **inMatrix;
	float **outMatrixC;
	float **outMatrixAsm;
	float **outMatrixExpected;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(MatrixInv3x3_fp32KernelTest, TestExpectedOutput)
{
  inMatrix = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixC = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixAsm = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixExpected = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);

  inMatrix[0][0] = 1.00000f;      inMatrix[0][1] = 0.50000f;      inMatrix[0][2] = 0.33333f;
  inMatrix[1][0] = 0.50000f;      inMatrix[1][1] = 0.33333f;      inMatrix[1][2] = 0.25000f;
  inMatrix[2][0] = 0.33333f;      inMatrix[2][1] = 0.25000f;      inMatrix[2][2] = 0.20000f;

  outMatrixExpected[0][0] = 9.00000f;      outMatrixExpected[0][1] = -36.000f;      outMatrixExpected[0][2] =  30.0000f;
  outMatrixExpected[1][0] = -36.000f;      outMatrixExpected[1][1] = 192.000f;      outMatrixExpected[1][2] = -180.000f;
  outMatrixExpected[2][0] = 30.0000f;      outMatrixExpected[2][1] = -180.00f;      outMatrixExpected[2][2] =  180.000f;

	matrixInv3x3_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv3x3_fp32CycleCount);
	mvcvMatrixInv3x3_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC[0], outMatrixAsm[0], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[1], outMatrixAsm[1], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[2], outMatrixAsm[2], SIZE, DELTA);

  outputChecker.CompareArrays(outMatrixExpected[0], outMatrixAsm[0], SIZE, DELTA);
  outputChecker.CompareArrays(outMatrixExpected[1], outMatrixAsm[1], SIZE, DELTA);
  outputChecker.CompareArrays(outMatrixExpected[2], outMatrixAsm[2], SIZE, DELTA);
}

TEST_F(MatrixInv3x3_fp32KernelTest, TestRandomInRange0_1)
{
	inputGen.SelectGenerator("random");
  inMatrix = inputGen.GetLinesFloat(SIZE, SIZE, 0.0f, 1.0f);
  outMatrixC = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixAsm = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);

	matrixInv3x3_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv3x3_fp32CycleCount);
	mvcvMatrixInv3x3_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC[0], outMatrixAsm[0], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[1], outMatrixAsm[1], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[2], outMatrixAsm[2], SIZE, DELTA);
}

TEST_F(MatrixInv3x3_fp32KernelTest, TestRandomInRange_minus1_1)
{
	inputGen.SelectGenerator("random");
  inMatrix = inputGen.GetLinesFloat(SIZE, SIZE, -1.0f, 1.0f);
  outMatrixC = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixAsm = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);

	matrixInv3x3_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv3x3_fp32CycleCount);
	mvcvMatrixInv3x3_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC[0], outMatrixAsm[0], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[1], outMatrixAsm[1], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[2], outMatrixAsm[2], SIZE, DELTA);
}


TEST_F(MatrixInv3x3_fp32KernelTest, TestRandomInRange0_255)
{
	inputGen.SelectGenerator("random");
  inMatrix = inputGen.GetLinesFloat(SIZE, SIZE, 0.0f, 255.0f);
  outMatrixC = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);
  outMatrixAsm = (float**)inputGen.GetEmptyLines(SIZE * sizeof(float), SIZE);

	matrixInv3x3_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv3x3_fp32CycleCount);
	mvcvMatrixInv3x3_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC[0], outMatrixAsm[0], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[1], outMatrixAsm[1], SIZE, DELTA);
	outputChecker.CompareArrays(outMatrixC[2], outMatrixAsm[2], SIZE, DELTA);
}
