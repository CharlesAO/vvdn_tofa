///matrix transpose
/// @param[in]  A    - structure that contains info regarding width, height, pointer to first matrix
/// @param[out] out  - structure that contains info regarding width, height, pointer to output matrix

#include "gtest/gtest.h"
///
#include <matTranspose.h>
#include "matTranspose_asm_test.h"
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
#define EDGE 16

class matTransposeKernelTest : public ::testing::TestWithParam< unsigned int > {
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

TEST_F(matTransposeKernelTest, TestMatrix32x32)
{
    width = 32;
    height = 32;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matTransposeKernelTest, TestMatrix10x10)
{
    width = 10;
    height = 10;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         

	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matTransposeKernelTest, TestMatrixRandom)
{
    width = randGen->GenerateUInt(1, 64);
    height = randGen->GenerateUInt(1, 64);

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
    
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matTransposeKernelTest, TestMatrix64x64)
{
    width =  64;
    height = 64;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}
//special non multiple of 4 cases
TEST_F(matTransposeKernelTest, TestMatrix2x2)
{
    width =  2;
    height = 2;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}
TEST_F(matTransposeKernelTest, TestMatrix3x2)
{
    width =  3;
    height = 2;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}


TEST_F(matTransposeKernelTest, TestMatrix13x5)
{
    width =  13;
    height = 5;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matTransposeKernelTest, TestMatrix7x17)
{
    width =  7;
    height = 17;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matTransposeKernelTest, TestMatrix21x55)
{
    width =  21;
    height = 55;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = width;            C_asm->m    = width;        
	A->n    = width;           C_c->n    = height;             C_asm->n    = height;         
 
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatTranspose(A, C_c);
    C_asm = (MAT *)matTranspose_asm_test((MAT_test *)A, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matTransposeCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}
