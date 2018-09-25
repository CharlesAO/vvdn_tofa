///matrix addition
/// @param[in]  A   - structure that contains info regarding width, height, pointer to first matrix
/// @param[in]  B   - structure that contains info regarding width, height, pointer to second matrix
/// @param[out] out - structure that contains info regarding width, height, pointer to output matrix

#include "gtest/gtest.h"
///
#include <matAdd.h>
#include "matAdd_asm_test.h"
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

class matAddKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }


    float *matA;
    float *matB;
    float *matC_C;
    float *matC_Asm;
	unsigned int width;
	unsigned int height;
	
	MAT *A;
	MAT *B;
	MAT *C_c;
	MAT *C_asm;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(matAddKernelTest, TestMatrix32x32)
{
    width = 32;
    height = 32;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	B     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;           B->m    = height;           C_c->m    = height;            C_asm->m    = height;        
	A->n    = width;            B->n    = width;            C_c->n    = width;             C_asm->n    = width;         

	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	B->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
	
    C_c   = mvcvMatAdd(A, B, C_c);
    C_asm = (MAT *)matAdd_asm_test((MAT_test *)A, (MAT_test *)B, (MAT_test *)C_asm);
	
    RecordProperty("CycleCount", matAddCycleCount);


	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
	
}

TEST_F(matAddKernelTest, TestMatrix10x10)
{
    width = 10;
    height = 10;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	B     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;           B->m    = height;           C_c->m    = height;            C_asm->m    = height;        
	A->n    = width;            B->n    = width;            C_c->n    = width;             C_asm->n    = width;         
	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	B->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
    C_c   = mvcvMatAdd(A, B, C_c);
    C_asm = (MAT *)matAdd_asm_test((MAT_test *)A, (MAT_test *)B, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matAddCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);

}

TEST_F(matAddKernelTest, TestMatrixRandom)
{
    width = randGen->GenerateUInt(1, 64);
    height = randGen->GenerateUInt(1, 64);

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	B     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;           B->m    = height;           C_c->m    = height;            C_asm->m    = height;        
	A->n    = width;            B->n    = width;            C_c->n    = width;             C_asm->n    = width;         

	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	B->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
    C_c   = mvcvMatAdd(A, B, C_c);
    C_asm = (MAT *)matAdd_asm_test((MAT_test *)A, (MAT_test *)B, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matAddCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
}

TEST_F(matAddKernelTest, TestMatrix64x64)
{
    width =  64;
    height = 64;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	B     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;           B->m    = height;           C_c->m    = height;            C_asm->m    = height;        
	A->n    = width;            B->n    = width;            C_c->n    = width;             C_asm->n    = width;         

	A->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);  
	B->base = (float*)inputGen.GetLineFloat(width * height, 0, 255);
	C_c->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width * height + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (width * height + EDGE));
    C_c   = mvcvMatAdd(A, B, C_c);
    C_asm = (MAT *)matAdd_asm_test((MAT_test *)A, (MAT_test *)B, (MAT_test *)C_asm);

    RecordProperty("CycleCount", matAddCycleCount);

    outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width * height + EDGE, 0.0f);
	
}

