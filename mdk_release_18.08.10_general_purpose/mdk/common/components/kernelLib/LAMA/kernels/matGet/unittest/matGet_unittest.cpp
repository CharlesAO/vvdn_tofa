/// matrix get block
/// @param[in]  in         - structure that contains info regarding width, height, pointer to input matrix
/// @param[out] out       - structure that contains info regarding width, height, pointer to output matrix
/// @param[in]  start_row - index of row where to start from
/// @param[in]  start_col - index of col where to start from

#include "gtest/gtest.h"
///
#include <matGet.h>
#include "matGet_asm_test.h"
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

class matGetKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	
	unsigned int length;
	
	unsigned int  width_out;
	unsigned int height_out;
	
	int start_row;
    int start_col;
	
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

TEST_F(matGetKernelTest, TestMatrix32x32)
{
    width = 32;
    height = 32;
	
	length = width;
	if(height > width)
		length = height;
	
	if(height == 1){
	start_row = 0;	
	}
	else{
	start_row = (int)randGen->GenerateUInt(0, height - 1);
	}
	if(width == 1){
	start_col = 0;	
	}
	else{
	start_col = (int)randGen->GenerateUInt(0, width - 1);
	}
	
	 height_out = height - start_row;
	width_out = width - start_col;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = height_out;            C_asm->m    = height_out;     
	A->n    = width;           C_c->n    = width_out;             C_asm->n    = width_out;      

	A->base = (float*)inputGen.GetLineFloat(length * length, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (length * length + EDGE));
	
    C_c   = mvcvMatGet(A, C_c, start_row, start_col);
    C_asm = (MAT *)matGet_asm_test((MAT_test *)A, (MAT_test *)C_asm, start_row, start_col);
	
    RecordProperty("CycleCount", matGetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, C_c->n * C_c->m + EDGE, 0.0f);
}

TEST_F(matGetKernelTest, TestMatrix10x10)
{
    width = 10;
    height = 10;

	length = width;
	if(height > width)
		length = height;
	
	if(height == 1){
	start_row = 0;	
	}
	else{
	start_row = (int)randGen->GenerateUInt(0, height - 1);
	}
	if(width == 1){
	start_col = 0;	
	}
	else{
	start_col = (int)randGen->GenerateUInt(0, width - 1);
	}
	
	 height_out = height - start_row;
	width_out = width - start_col;

    inputGen.SelectGenerator("uniform");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = height_out;            C_asm->m    = height_out;     
	A->n    = width;           C_c->n    = width_out;             C_asm->n    = width_out;      

	A->base = (float*)inputGen.GetLineFloat(length * length, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (length * length + EDGE));
	
    C_c   = mvcvMatGet(A, C_c, start_row, start_col);
    C_asm = (MAT *)matGet_asm_test((MAT_test *)A, (MAT_test *)C_asm, start_row, start_col);
	
    RecordProperty("CycleCount", matGetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, C_c->n * C_c->m + EDGE, 0.0f);

}

TEST_F(matGetKernelTest, TestMatrixRandom)
{
    width = randGen->GenerateUInt(1, 64);
    height = randGen->GenerateUInt(1, 64);
	length = width;
	if(height > width)
		length = height;
	
	if(height == 1){
	start_row = 0;	
	}
	else{
	start_row = (int)randGen->GenerateUInt(0, height - 1);
	}
	if(width == 1){
	start_col = 0;	
	}
	else{
	start_col = (int)randGen->GenerateUInt(0, width - 1);
	}

	height_out = height - start_row;
	width_out = width - start_col;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = height_out;            C_asm->m    = height_out;     
	A->n    = width;           C_c->n    = width_out;             C_asm->n    = width_out;      
 
	A->base = (float*)inputGen.GetLineFloat(length * length, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (length * length + EDGE));
	
    C_c   = mvcvMatGet(A, C_c, start_row, start_col);
    C_asm = (MAT *)matGet_asm_test((MAT_test *)A, (MAT_test *)C_asm, start_row, start_col);
	
    RecordProperty("CycleCount", matGetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, C_c->n * C_c->m + EDGE, 0.0f);
}

TEST_F(matGetKernelTest, TestMatrix64x64)
{
    width =  64;
    height = 64;

	length = width;
	if(height > width)
		length = height;
	
	if(height == 1){
	start_row = 0;	
	}
	else{
	start_row = (int)randGen->GenerateUInt(0, height - 1);
	}
	if(width == 1){
	start_col = 0;	
	}
	else{
	start_col = (int)randGen->GenerateUInt(0, width - 1);
	}
	
	 height_out = height - start_row;
	width_out = width - start_col;

    inputGen.SelectGenerator("random");
	
	A     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A->m    = height;          C_c->m    = height_out;            C_asm->m    = height_out;     
	A->n    = width;           C_c->n    = width_out;             C_asm->n    = width_out;      
 
	A->base = (float*)inputGen.GetLineFloat(length * length, 0, 255);  
	C_c->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(length * length + EDGE, 0, 255);
	
	memcpy(C_asm->base, C_c->base, sizeof(float) * (length * length + EDGE));
	
    C_c   = mvcvMatGet(A, C_c, start_row, start_col);
    C_asm = (MAT *)matGet_asm_test((MAT_test *)A, (MAT_test *)C_asm, start_row, start_col);
	
    RecordProperty("CycleCount", matGetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, C_c->n * C_c->m + EDGE, 0.0f);
}

