///matrix set block
/// @param[in]  in   - structure that contains info regarding width, height, pointer to first matrix
/// @param[out] out  - structure that contains info regarding width, height, pointer to output matrix
/// @param[in] start_row   - index of starting row of block to set
/// @param[in] start_col   - index of starting col of block to set
/// @param[in] length_row   - length of rows
/// @param[in] length_col   - length of cols

#include "gtest/gtest.h"
///
#include <matSet.h>
#include "matSet_asm_test.h"
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

class matSetKernelTest : public ::testing::TestWithParam< unsigned int > {
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
	
	unsigned int width2;
	unsigned int height2;
	
	float scalar;
	int start_row;
	int start_col;
	int length_row;
	int length_col;
	
	MAT *A_c;
	MAT *A_asm;
	MAT *C_c;
	MAT *C_asm;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(matSetKernelTest, TestMatrix32x32)
{
    width = 32;
    height = 32;
	
	if(width > height)
	{
		width2 = width;
		height2 = width;
	}
	else
	{
		width2 = height;
		height2 = height;		
	}
	
	scalar = randGen->GenerateFloat(0, 255);
	start_row = randGen->GenerateUInt(0, height - 1);
	start_col = randGen->GenerateUInt(0, width - 1);
	length_row =  randGen->GenerateUInt(1, height - start_row);
	length_col =  randGen->GenerateUInt(1, width - start_col);;

    inputGen.SelectGenerator("random");
	
	A_c       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	A_asm     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A_c->m    = height;           A_asm->m    = height;                C_c->m    = width;            C_asm->m    = width;        
	A_c->n    = width;            A_asm->n    = width;                 C_c->n    = height;           C_asm->n    = height;         
	
	A_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	A_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	C_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);

	memcpy(A_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(  C_c->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(C_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	
    C_c   = mvcvMatSet(scalar, A_c, start_row, start_col, length_row, length_col);
    C_asm = (MAT *)matSet_asm_test(scalar, (MAT_test *)A_asm, start_row, start_col, length_row, length_col);

    RecordProperty("CycleCount", matSetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width2 * height2 + EDGE, 0.0f);
}

TEST_F(matSetKernelTest, TestMatrix10x10)
{
    width = 10;
    height = 10;
	
	if(width > height)
	{
		width2 = width;
		height2 = width;
	}
	else
	{
		width2 = height;
		height2 = height;		
	}
	
	scalar = randGen->GenerateFloat(0, 255);
	start_row = randGen->GenerateUInt(0, height - 1);
	start_col = randGen->GenerateUInt(0, width - 1);
	length_row =  randGen->GenerateUInt(1, height - start_row);
	length_col =  randGen->GenerateUInt(1, width - start_col);;

    inputGen.SelectGenerator("random");
	
	A_c       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	A_asm     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A_c->m    = height;           A_asm->m    = height;                C_c->m    = width;            C_asm->m    = width;        
	A_c->n    = width;            A_asm->n    = width;                 C_c->n    = height;           C_asm->n    = height;              
	
	A_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	A_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	C_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);

	memcpy(A_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(  C_c->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(C_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	
    C_c   = mvcvMatSet(scalar, A_c, start_row, start_col, length_row, length_col);
    C_asm = (MAT *)matSet_asm_test(scalar, (MAT_test *)A_asm, start_row, start_col, length_row, length_col);

    RecordProperty("CycleCount", matSetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width2 * height2 + EDGE, 0.0f);
}

TEST_F(matSetKernelTest, TestMatrixRandom)
{
    width = randGen->GenerateUInt(1, 64);
    height = randGen->GenerateUInt(1, 64);

	if(width > height)
	{
		width2 = width;
		height2 = width;
	}
	else
	{
		width2 = height;
		height2 = height;		
	}
	
	scalar = randGen->GenerateFloat(0, 255);
	start_row = randGen->GenerateUInt(0, height - 1);
	start_col = randGen->GenerateUInt(0, width - 1);
	length_row =  randGen->GenerateUInt(1, height - start_row);
	length_col =  randGen->GenerateUInt(1, width - start_col);;

    inputGen.SelectGenerator("random");
	
	A_c       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	A_asm     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A_c->m    = height;           A_asm->m    = height;                C_c->m    = width;            C_asm->m    = width;        
	A_c->n    = width;            A_asm->n    = width;                 C_c->n    = height;           C_asm->n    = height;            
	
	A_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	A_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	C_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);

	memcpy(A_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(  C_c->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(C_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	
    C_c   = mvcvMatSet(scalar, A_c, start_row, start_col, length_row, length_col);
    C_asm = (MAT *)matSet_asm_test(scalar, (MAT_test *)A_asm, start_row, start_col, length_row, length_col);

    RecordProperty("CycleCount", matSetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width2 * height2 + EDGE, 0.0f);
}

TEST_F(matSetKernelTest, TestMatrix64x64)
{
    width =  64;
    height = 64;

	if(width > height)
	{
		width2 = width;
		height2 = width;
	}
	else
	{
		width2 = height;
		height2 = height;		
	}
	
	scalar = randGen->GenerateFloat(0, 255);
	start_row = randGen->GenerateUInt(0, height - 1);
	start_col = randGen->GenerateUInt(0, width - 1);
	length_row =  randGen->GenerateUInt(1, height - start_row);
	length_col =  randGen->GenerateUInt(1, width - start_col);;

    inputGen.SelectGenerator("random");
	
	A_c       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	A_asm     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_c   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	C_asm = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	
	A_c->m    = height;           A_asm->m    = height;                C_c->m    = width;            C_asm->m    = width;        
	A_c->n    = width;            A_asm->n    = width;                 C_c->n    = height;           C_asm->n    = height;            
	
	A_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	A_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);  
	C_c->base   = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255); 
	C_asm->base = (float*)inputGen.GetLineFloat(width2 * height2 + EDGE, 0, 255);

	memcpy(A_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(  C_c->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	memcpy(C_asm->base, A_c->base, sizeof(float) * (width2 * height2 + EDGE));
	
    C_c   = mvcvMatSet(scalar, A_c, start_row, start_col, length_row, length_col);
    C_asm = (MAT *)matSet_asm_test(scalar, (MAT_test *)A_asm, start_row, start_col, length_row, length_col);

    RecordProperty("CycleCount", matSetCycleCount);

	outputChecker.CompareArraysFloat(C_c->base, C_asm->base, width2 * height2 + EDGE, 0.0f);
}

