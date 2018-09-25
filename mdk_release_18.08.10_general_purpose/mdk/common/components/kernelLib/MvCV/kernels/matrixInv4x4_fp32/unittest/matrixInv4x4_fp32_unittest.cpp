//matrixInv4x4_fp32 kernel test
//Asm function prototype:
//     void matrixInv4x4_fp32_asm(fp32 *in, fp32 *out);
//
//Asm test function prototype:
//     void matrixInv4x4_fp32_asm_test(fp32 *in, fp32 *out);
//
//C function prototype:
//     void matrixInv4x4_fp32(fp32 *in, fp32 *out);
//
//Parameter description:
// in         - array of pointers to input matrix
// out        - array of pointers to output matrix


#include "gtest/gtest.h"
#include "matrixInv4x4_fp32.h"
#include "matrixInv4x4_fp32_unittest.h"
///
#include "matrixInv4x4_fp32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <float.h>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 0.2f //accepted tolerance between C and ASM results
#define SIZE 4
class MatrixInv4x4_fp32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float *inMatrix;
	float *outMatrixC;
	float *outMatrixAsm;
	float *outMatrixExpected;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(MatrixInv4x4_fp32KernelTest, TestMatrixI)
{
  inMatrix = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixC = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixAsm = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixExpected = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));

  for(int i = 0; i < SIZE * SIZE; i++)
    inMatrix[i] = 0;
  
  inMatrix[0]  = 1;
  inMatrix[5]  = 1;
  inMatrix[10] = 1;
  inMatrix[15] = 1;  

	matrixInv4x4_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv4x4_fp32CycleCount);
	mvcvMatrixInv4x4_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC, outMatrixAsm, SIZE*SIZE, DELTA);  
}

TEST_F(MatrixInv4x4_fp32KernelTest, TestExpectedOutput)
{
  inMatrix = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixC = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixAsm = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixExpected = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));

  inMatrix[0] = 1.00000f;      inMatrix[4] = 0.50000f;      
  inMatrix[1] = 0.50000f;      inMatrix[5] = 0.33333f;      
  inMatrix[2] = 0.33333f;      inMatrix[6] = 0.25000f;          
  inMatrix[3] = 0.27000f;      inMatrix[7] = 0.87777f;      

  inMatrix[8] = 0.33333f;      inMatrix[12] = 0.33333f;
  inMatrix[9] = 0.25000f;      inMatrix[13] = 0.25000f;
  inMatrix[10] = 0.20000f;     inMatrix[14] = 0.20000f;
  inMatrix[11] = 0.43334f;     inMatrix[15] = 0.66666f;
    
  outMatrixExpected[0]  = 9.00000f;          outMatrixExpected[4]  = -36.000f;
  outMatrixExpected[1]  = -36.000f;          outMatrixExpected[5]  = 192.000f;
  outMatrixExpected[2]  = -39.3000f;         outMatrixExpected[6]  =  166.500f;
  outMatrixExpected[3]  =  69.3000f;         outMatrixExpected[7]  = -346.650f;
  
  outMatrixExpected[8]  = 30.0000f;          outMatrixExpected[12] = -0.0000f;
  outMatrixExpected[9]  = -180.10f;          outMatrixExpected[13] = -0.0000f;    
  outMatrixExpected[10] = -128.280f;         outMatrixExpected[14] = -4.28000f;
  outMatrixExpected[11] =  308.430f;         outMatrixExpected[15] =  4.28000f;

	matrixInv4x4_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv4x4_fp32CycleCount);
	mvcvMatrixInv4x4_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC, outMatrixAsm, SIZE*SIZE, DELTA);  
  outputChecker.CompareArrays(outMatrixExpected, outMatrixAsm, SIZE*SIZE, DELTA);
}

TEST_F(MatrixInv4x4_fp32KernelTest, TestRandomInRange0_1)
{
  inMatrix = inputGen.GetLineFloat(SIZE * SIZE, 0.0f, 1.0f);
  outMatrixC = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixAsm = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));

	matrixInv4x4_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv4x4_fp32CycleCount);
	mvcvMatrixInv4x4_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC, outMatrixAsm, SIZE*SIZE, DELTA);
}

TEST_F(MatrixInv4x4_fp32KernelTest, TestRandomInRange_minus1_1)
{
  inMatrix = inputGen.GetLineFloat(SIZE * SIZE, -1.0f, 1.0f);
  outMatrixC = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixAsm = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));

	matrixInv4x4_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv4x4_fp32CycleCount);
	mvcvMatrixInv4x4_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC, outMatrixAsm, SIZE*SIZE, DELTA);
}

TEST_F(MatrixInv4x4_fp32KernelTest, TestRandomInRange_minus255_255)
{
  inMatrix = inputGen.GetLineFloat(SIZE * SIZE, -255.0f, 255.0f);
  outMatrixC = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));
  outMatrixAsm = (float*)inputGen.GetEmptyLine(SIZE * SIZE * sizeof(float));

	matrixInv4x4_fp32_asm_test(inMatrix, outMatrixAsm);
	RecordProperty("CyclePerPixel", matrixInv4x4_fp32CycleCount);
	mvcvMatrixInv4x4_fp32(inMatrix, outMatrixC);

	outputChecker.CompareArrays(outMatrixC, outMatrixAsm, SIZE*SIZE, DELTA);
}
