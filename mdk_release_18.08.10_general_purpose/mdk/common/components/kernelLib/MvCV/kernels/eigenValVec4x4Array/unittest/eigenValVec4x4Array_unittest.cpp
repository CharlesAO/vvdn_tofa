//eigenValVec4x4Array kernel test
//Asm function prototype:
//     void mvcveigenValVec4x4Array_asm( float** matrix, float* eigvals, float** eigvec, u32 num_matrix)
//
//Asm test function prototype:
//     void eigenValVec4x4Array_asm_test( float** matrix, float* eigvals, float** eigvec, u32 num_matrix)
//
//C function prototype:
//     void mvcvEigenValVec4x4Array( float** matrix, float* eigvals, float** eigvec, u32 num_matrix)
//
//Parameter description:
// @param[in] in        - smoothed values from a Gaussian filter from all pixels from a given pattern (brisk)
// @param[out] out      - binary value that describe the corner using a patern (binary descriptor)
//


#include "gtest/gtest.h"
#include <eigenValVec4x4Array.h>
#include "eigenValVec4x4Array_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

float DELTA = 0.01;

class eigenValVec4x4ArrayKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float  **matrix;
	float  *eigvals_Asm;
	float  **eigvec_Asm;
	
	float  *eigvals_C;
	float  **eigvec_C;
	float  threshold;
	unsigned int num_mat;
	
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};



TEST_F(eigenValVec4x4ArrayKernelTest, TestUniformInputLines32)
{
	
	num_mat = 100;
	unsigned int row_width = 16;
	unsigned int i =0;
	threshold = 0.0005;
	inputGen.SelectGenerator("uniform");

	matrix = (float **)inputGen.GetLines(row_width * 4, num_mat, 0);
	eigvals_Asm  = (float *)inputGen.GetEmptyLine(num_mat * 4);
	eigvals_C    = (float *)inputGen.GetEmptyLine(num_mat * 4);
	
	eigvec_Asm   = (float **)inputGen.GetEmptyLines((row_width)*4, num_mat);
	eigvec_C     = (float **)inputGen.GetEmptyLines((row_width)*4, num_mat);
	
	for ( i = 0; i< num_mat; i+=2)
	{
		matrix[i][0]   =  2.005345 ; matrix[i][1]   = 0.001696 ; matrix[i][2]   = 1.105084 ; matrix[i][3]   = -0.232994 ;
		matrix[i][4]   =  0.001696 ; matrix[i][5]   = 1.999420 ; matrix[i][6]   = 0.896327 ; matrix[i][7]   =  0.197473 ;
		matrix[i][8]   =  1.105084 ; matrix[i][9]   = 0.896327 ; matrix[i][10]  = 1.010365 ; matrix[i][11]  = -0.44224 ;
		matrix[i][12]  = -0.232994 ; matrix[i][13]  = 0.197473 ; matrix[i][14]  = -0.44224 ; matrix[i][15]  =  0.093171 ;
		
		matrix[i+1][0]  =  2.005345 ; matrix[i+1][1]  = 0.001696 ; matrix[i+1][2]  = 1.105084 ; matrix[i+1][3]  = -0.232994 ;
		matrix[i+1][4]  =  0.001696 ; matrix[i+1][5]  = 1.999420 ; matrix[i+1][6]  = 0.896327 ; matrix[i+1][7]  =  0.197473 ;
		matrix[i+1][8]  =  1.105084 ; matrix[i+1][9]  = 0.896327 ; matrix[i+1][10] = 1.010365 ; matrix[i+1][11] = -0.44224  ;
		matrix[i+1][12] = -0.232994 ; matrix[i+1][13] = 0.197473 ; matrix[i+1][14] = -0.44224 ; matrix[i+1][15] =  0.093171 ;
	}
	
	
	mvcvEigenValVec4x4Array(matrix, eigvals_C, eigvec_C, num_mat, threshold);
	eigenValVec4x4Array_asm_test(matrix, eigvals_Asm, eigvec_Asm, num_mat, threshold);

	RecordProperty("CyclePerPixel", eigenValVec4x4ArrayCycleCount);

	outputChecker.CompareArrays(eigvals_C, eigvals_Asm, num_mat, DELTA);
	outputChecker.CompareArrays(eigvec_C, eigvec_Asm, row_width, num_mat, DELTA);
}

