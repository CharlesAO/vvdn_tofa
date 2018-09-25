//This kernel performs a vertical average on 49 elements
//@param[in]	srcAddr	- input elements 16-bits floating point - 49 lines
//@param[out]	destAddr	- Output line, 16-bits floating point
//@param[in] 	width		- Width of input line

#include "gtest/gtest.h"
#include "avgPool7x7xk.h"
#include "avgPool7x7xk_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define EXCEEDED_BOUNDS 16

half delta = 1;

using ::testing::TestWithParam;
using ::testing::Values;
 
class AvgPool7x7xkTest : public ::testing::TestWithParam <unsigned int> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLine1;
	half** outLineC;
	half** outLineAsm;
	half** zeros;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(AvgPool7x7xkTest, TestUniformInput10Minus9)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesFloat16(width, 49, 10.0f);
	outLineC = inputGen.GetLinesFloat16(width, 1, 0.0f);
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvAvgPool7x7xk(inLine1, outLineC, width);
	avgPool7x7xk_asm_test(inLine1, outLineAsm, width);
	RecordProperty("CyclePerPixel", avgPool7x7xkCycleCount / width);		
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width, delta);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}

TEST_F(AvgPool7x7xkTest, TestUniformInput123Minus223)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesFloat16(width, 49, 123.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);		
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvAvgPool7x7xk(inLine1, outLineC, width);
	avgPool7x7xk_asm_test(inLine1, outLineAsm, width);
	RecordProperty("CyclePerPixel", avgPool7x7xkCycleCount / width);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width, delta);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}

//------------ parametrized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, AvgPool7x7xkTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)40, (unsigned int)64, (unsigned int)240, (unsigned int)240, (unsigned int)240);
);

TEST_P(AvgPool7x7xkTest, TestRandomInputParametrizedWidth)
{
	width = GetParam();

	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLinesFloat16(width, 49, 0.0f, 255.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.SelectGenerator("uniform");
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);		
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvAvgPool7x7xk(inLine1, outLineC, width);
	avgPool7x7xk_asm_test(inLine1, outLineAsm, width);
	RecordProperty("CyclePerPixel", avgPool7x7xkCycleCount / width);	
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width, delta);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}