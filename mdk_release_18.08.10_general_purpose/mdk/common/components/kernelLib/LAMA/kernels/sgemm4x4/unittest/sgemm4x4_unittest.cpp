//sgemm4x4 kernel test
//Asm function prototype:
//     void sgemm4x4_asm(s32 k, float *alpha, float *A, float *B, float *beta, float *C_Asm, s32 rs_c, s32 cs_c);
//
//Asm test function prototype:
//     void sgemm4x4_asm(s32 k, float *alpha, float *A, float *B, float *beta, float *C_Asm, s32 rs_c, s32 cs_c);
//
//C function prototype:
//     void sgemm4x4(s32 k, float *alpha, float *A, float *B, float *beta, float *C_C, s32 rs_c, s32 cs_c);
//
//Parameter description:
// k     - number of columns of A and rows of B
// alpha - address of a scalar to the A * B product.
// A     - 4 x k "micro-panel" matrix stored in packed (column-stored)
// B     - k x 4 "micro-panel" matrix in packed (row-stored) format
// beta  - address of a scalar to the input value of matrix C
// C_C   - address of a matrix C of dimension 4 x 4, stored according to rsc and csc, used for C function
// C_Asm - address of a matrix C of dimension 4 x 4, stored according to rsc and csc, used for Asm function
// rs_c  - row stride of matrix C
// cs_c  - column stride of matrix C

#include "gtest/gtest.h"
///
#include <sgemm4x4.h>
#include "sgemm4x4_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA          0.7f //accepted tolerance between C and ASM results
#define MIN_VALUE    -10.0f
#define MAX_VALUE     10.0f
#define MIN_SCALAR     1.0f
#define MAX_SCALAR     2.0f

class sgemm4x4KernelTest : public ::testing::TestWithParam< unsigned int > {
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

	s32   k = 0;
	s32   k_step = 0;
    float *alpha;
    float *A;
    float *B;
    float *beta;
    float *C_C;
    float *C_Asm;
	s32   rs_c;
	s32   cs_c;
	InputGenerator inputGen;
    RandomGenerator randomGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(sgemm4x4KernelTest, TestMatrixAIsNull)
{
    k    = 512;
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    alpha = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    beta  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    B     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    A     = inputGen.GetLineFloat(4 * k, 0.0);

    C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(C_Asm, C_C, 4 * 4);

    mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
    sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemm4x4CycleCount);

    //outputChecker.ExpectAllEQ<float>(C_Asm, 4 * 4, 2.0);
   // CompareResults(C_C, C_Asm, 4 * 4, DELTA);
     outputChecker.CompareArraysFloat(C_C, C_Asm, 4 * 4, 0.0);
}

TEST_F(sgemm4x4KernelTest, TestMatrixBIsNull)
{
    k    = 512;
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    alpha = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    beta  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
	B     = inputGen.GetLineFloat(4 * k, 0.0);

	C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(C_Asm, C_C, 4 * 4);

	mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
	sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemm4x4CycleCount);

	 outputChecker.CompareArraysFloat(C_C, C_Asm, 4 * 4, 0.0);
}

TEST_F(sgemm4x4KernelTest, TestAlphaIsNull)
{
    k    = 512;
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    beta  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    alpha = inputGen.GetLineFloat(1, 0.0);

    C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(C_Asm, C_C, 4 * 4);

    mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
    sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemm4x4CycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, 4 * 4, 0.0);
}

TEST_F(sgemm4x4KernelTest, TestBetaIsNull)
{
    k    = 512;
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    alpha  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    C_Asm = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    beta = inputGen.GetLineFloat(1, 0.0);

    mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
    sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);
    RecordProperty("CycleCount", sgemm4x4CycleCount);

     CompareResults(C_C, C_Asm, 4 * 4, DELTA);
}

    TEST_F(sgemm4x4KernelTest, TestKZero)
{
    k = 0;
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    alpha = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    beta  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(C_Asm, C_C, 4 * 4);

    mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
    sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);

    RecordProperty("CycleCount", sgemm4x4CycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, 4 * 4, 0.0);

}

TEST_F(sgemm4x4KernelTest, TestDifferentK)
{
    k_step = randGen->GenerateUInt(2, 11, 1);
    k = pow(2, k);
    rs_c = 4;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    alpha = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    beta  = inputGen.GetLineFloat(1, MIN_SCALAR, MAX_SCALAR);
    A     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    B     = inputGen.GetLineFloat(4 * k, MIN_VALUE, MAX_VALUE);
    C_C   = inputGen.GetLineFloat(4 * 4, MIN_VALUE, MAX_VALUE);
    C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));
    CopyLine(C_Asm, C_C, 4 * 4);

    mvcvsgemm4x4(k, alpha, A, B, beta, C_C, rs_c, cs_c);
    sgemm4x4_asm_test(k, alpha, A, B, beta, C_Asm, rs_c, cs_c);

    RecordProperty("CycleCount", sgemm4x4CycleCount);

    outputChecker.CompareArraysFloat(C_C, C_Asm, 4 * 4, 0.0);
}


//-------------------- parameterized tests -------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, sgemm4x4KernelTest,
        Values((unsigned int)4, (unsigned int)8, (unsigned int)16);
);



TEST_P(sgemm4x4KernelTest, TestMultiplyAxBWithDifferentK)
{
    float *A_Asm, *B_Asm, *C;
    float *a1, *b1, *c1, *c11;
    float sum;
    int   i, j, i_c, j_c;

    k    = GetParam();
    rs_c = k;
    cs_c = 1;
    inputGen.SelectGenerator("random");
    A     = inputGen.GetLineFloat(k * k, MIN_VALUE, MAX_VALUE);
    inputGen.SelectGenerator("uniform");
    alpha = inputGen.GetLineFloat(1, 1.0);
    beta  = inputGen.GetLineFloat(1, 0.0);

    B = (float*)inputGen.GetEmptyLine(k * k * sizeof(float));
    C = (float*)inputGen.GetEmptyLine(k * k * sizeof(float));
    A_Asm = (float*)inputGen.GetEmptyLine(k * 4 * sizeof(float));
    B_Asm = (float*)inputGen.GetEmptyLine(k * 4 * sizeof(float));
    C_Asm = (float*)inputGen.GetEmptyLine(4 * 4 * sizeof(float));

    InverseMatrix(A, B, k);
    TransposeMatrix(A, k);

    b1 = B;
    c1 = C;

    /* Loop over the n dimension (4 columns at a time). */
    for(j = 0; j < k/4; j++)
    {
        a1  = A;
        c11 = c1;

        /* Fill B_Asm with corresponding panel from B */
        FillPanel(b1, B_Asm, k);

        /* Loop over the m dimension (MR rows at a time). */
        for(i = 0; i < k/4; i++)
        {
            /* Fill A_Asm with corresponding panel from A */
            FillPanel(a1, A_Asm, k);

            /* Invoke the gemm micro-kernel. */
            sgemm4x4_asm_test(k, alpha, A_Asm, B_Asm, beta, C_Asm, 4, 1);

            /* Fill corresponding panel of C with C_Asm */
            for(i_c = 0; i_c < 4; i_c++)
            {
                for(j_c = 0; j_c < 4; j_c++)
                {
                    c11[i_c*rs_c + j_c*cs_c] = C_Asm[4*i_c + j_c];
                }
            }
            a1  += 4;
            c11 += 4 * rs_c;
        }
        b1 += 4;
        c1 += 4 * cs_c;
    }

    sum = 0.0;
    for(i = 0; i < k; i++)
    {
        sum += C[i*k + i];
    }

    EXPECT_NEAR(sum, (float)k, 2);

    RecordProperty("CycleCount", sgemm4x4CycleCount);
}

void sgemm4x4KernelTest::FillPanel(float *in, float *out, int size)
{
    int i, j;

    for(i = 0; i < size; i++)
    {
        for(j = 0; j < 4; j++)
        {
            out[i*4 + j] = in[i*size + j];
        }
    }
}

void sgemm4x4KernelTest::TransposeMatrix(float *matrix, int size)
{
    int   i, j;
    float temp;

    for(i = 1; i < size; i++)
    {
        for(j = 0; j < i; j++)
        {
            temp               = matrix[i*size + j];
            matrix[i*size + j] = matrix[j*size + i];
            matrix[j*size + i] = temp;
        }
    }
}

#define SMALLEST 1.0e-12

void sgemm4x4KernelTest::InverseMatrix(float *in, float *out, int size)
{
    /* Loop variables */
    int   i, j, k;
    /* Sum variables */
    float sum, x;

    /*  Copy the input matrix to output matrix */
    for(i = 0; i < size*size; i++)
    {
        out[i] = in[i];
    }
    /* Add small value to diagonal if diagonal is zero */
    for(i = 0; i < size; i++)
    {
        j = i*size + i;
        if((out[j] < SMALLEST) && (out[j] > -SMALLEST))
        {
            out[j] = SMALLEST;
        }
    }
    /* Matrix size must be larger than one */
    if (size <= 1)
        return;
    for (i = 1; i < size; i++)
    {
        out[i] /= out[0]; /* normalize row 0 */
    }
    for (i = 1; i < size; i++)
    {
        for (j = i; j < size; j++)
        { /* do a column of L */
            sum = 0.0;
            for (k = 0; k < i; k++)
            {
                sum += out[j*size+k] * out[k*size+i];
            }
            out[j*size+i] -= sum;
        }
        if (i == size - 1)
            continue;
        for (j = i + 1; j < size; j++)
        {  /* do a row of U */
            sum = 0.0;
            for (k = 0; k < i; k++)
            {
                sum += out[i*size+k]*out[k*size+j];
            }
            if(out[i*size+i] == 0)
            {
                printf("\nInverse of Entered Matrix is not possible\n");
                return;
            }
            out[i*size+j] = (out[i*size+j]-sum) / out[i*size+i];
        }
    }
    for ( i = 0; i < size; i++ )  /* invert L */
    {
        for ( j = i; j < size; j++ )
        {
            x = 1.0;
            if ( i != j )
            {
                x = 0.0;
                for ( k = i; k < j; k++ )
                {
                    x -= out[j*size+k]*out[k*size+i];
                }
            }
            if(out[j*size+j] == 0)
            {
                printf("\nInverse of Entered Matrix is not possible\n");
                return;
            }
            out[j*size+i] = x / out[j*size+j];
        }
    }
    for ( i = 0; i < size; i++ ) /* invert U */
    {
        for ( j = i; j < size; j++ )
        {
            if ( i == j )
                continue;
            sum = 0.0;
            for ( k = i; k < j; k++ )
            {
                sum += out[k*size+j]*( (i == k) ? (float)1.0 : out[i*size+k] );
            }
            out[i*size+j] = -sum;
        }
    }
    for ( i = 0; i < size; i++ ) /* final inversion */
    {
        for ( j = 0; j < size; j++ )
        {
            sum = 0.0;
            for ( k = ((i>j)?i:j); k < size; k++ )
            {
                sum += ((j==k)?1.0:out[j*size+k])*out[k*size+i];
            }
            out[j*size+i] = sum;
        }
    }
}
