//         SINGULAR VALUE DECOMPOSITION kernel test
//Asm function prototype:
//     void mvcvSVD_asm(float **A, float *S, float **V, int rank);
//
//Asm test function prototype:
//     void svd_asm_test(float **A, float *S, float **V, int rank);
//
//C function prototype:
//     void mvcvSVD(float **A, float *S, float **V, int rank);
//
//Parameter description:
//!@{
/// mvcvSVD 	- Performs a singular value decomposition A = USV' of square matrix.
///               No restrictions to size or contents of the matrix.
///               This version of SVD works on square matrices only.
///               The assembly code for size 3 and size 9 is optimized particularly.
///
/// @param[in]   A  rank x rank matrix - on calling contains the matrix to be decomposed	
/// @param[out]  A  rank x rank matrix - on return contains the product US	
/// @param[out]  S  vector returns the square of the singular values. 
/// @param[out]  V  rank x rank matrix - on return contains V(not V')   	
/// @param[in]   rank -number of rows/columns in matrix A           	 
/// @return      Nothing


#include "gtest/gtest.h"
#include <svd.h>
#include "svd_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdio.h> 
using ::testing::TestWithParam;
using ::testing::Values;

#define TEST_RANK 3

class SVDTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

    float **A_C;    
	float **A_ASM;
    float **V_C;
    float **V_ASM;
    float *S_C;
    float *S_ASM;
    int rank;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(SVDTest, TestAllRandomValues)
{
	
	int i,j;
	rank =  TEST_RANK;	
	
	inputGen.SelectGenerator("random");
	
	A_C = inputGen.GetLinesFloat(TEST_RANK, TEST_RANK, 2.00, 10.00);
	A_ASM =	inputGen.GetLinesFloat(TEST_RANK, TEST_RANK, 10.00, 10.00);
	for (i = 0; i < rank; i++)	{
		for (j = 0; j < rank; j++)
			A_ASM[i][j] = A_C[i][j] ; 
	}

	S_C   = (float*)inputGen.GetEmptyLineFp16(TEST_RANK*2);
    S_ASM = (float*)inputGen.GetEmptyLineFp16(TEST_RANK*2);
	
	V_C = inputGen.GetLinesFloat(TEST_RANK, TEST_RANK, 0.00, 0.00);
	V_ASM = inputGen.GetLinesFloat(TEST_RANK, TEST_RANK, 0.00, 0.00);

	
    mvcvSVD(A_C, S_C, V_C, rank);
    svd_asm_test(A_ASM, S_ASM, V_ASM, rank);

	RecordProperty("cycleCount", SVDCycleCount);
	
    outputChecker.CompareArraysFloat(S_C, S_ASM, TEST_RANK, 0.001);
    outputChecker.CompareArraysFloat(A_C, A_ASM, TEST_RANK, TEST_RANK, 0.001);
	outputChecker.CompareArraysFloat(V_C, V_ASM, TEST_RANK, TEST_RANK, 0.001);

}

