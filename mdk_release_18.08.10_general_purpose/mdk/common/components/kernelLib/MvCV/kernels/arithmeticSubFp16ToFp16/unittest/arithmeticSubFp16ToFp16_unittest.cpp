//arithmeticSubFp16ToFp16 kernel test

//Asm function prototype:
//	 void arithmeticSubFp16ToFp16_asm(half** in1, half** in2, half** out, u32 width);

//Asm test function prototype:
//    	void arithmeticSubFp16ToFp16_asm_test(half** input1, half** input2, half** output, unsigned int width);

//C function prototype:
//  	 void arithmeticSubFp16ToFp16(half** in1, half** in2, half** out, u32 width);

//width		- width of input lines and output line 
//in1		- first line
//in2		- second line
//out		- output line

#include "gtest/gtest.h"
#include "arithmeticSubFp16ToFp16.h"
#include "arithmeticSubFp16ToFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define EXCEEDED_BOUNDS 16


using ::testing::TestWithParam;
using ::testing::Values;
 
class ArithmeticSubFp16ToFp16Test : public ::testing::TestWithParam <unsigned int> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLine1;
	half** inLine2;
	half** outLineC;
	half** outLineAsm;
	half** outLineExpected;
	half** zeros;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(ArithmeticSubFp16ToFp16Test, TestUniformInput10Minus9)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesFloat16(width, 1, 10.0f);
	inLine2 = inputGen.GetLinesFloat16(width, 1, 9.0f);
	outLineExpected = inputGen.GetLinesFloat16(width, 1, 1.0f);
	outLineC = inputGen.GetLinesFloat16(width, 1, 0.0f);
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvArithmeticSubFp16ToFp16(inLine1, inLine2, outLineC, width);
	arithmeticSubFp16ToFp16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubFp16ToFp16CycleCount / width);		
		
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}

TEST_F(ArithmeticSubFp16ToFp16Test, TestUniformInput123Minus223)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesFloat16(width, 1, 123.0f);
	inLine2 = inputGen.GetLinesFloat16(width, 1, 223.0f);
	outLineExpected = inputGen.GetLinesFloat16(width, 1, -100.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);		
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvArithmeticSubFp16ToFp16(inLine1, inLine2, outLineC, width);
	arithmeticSubFp16ToFp16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubFp16ToFp16CycleCount / width);	
	
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}

//------------ parametrized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, ArithmeticSubFp16ToFp16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)40, (unsigned int)64, (unsigned int)512, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(ArithmeticSubFp16ToFp16Test, TestRandomInputParametrizedWidth)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLinesFloat16(width, 1, 0.0f, 255.0f);
	inLine2 = inputGen.GetLinesFloat16(width, 1, 0.0f, 255.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.SelectGenerator("uniform");
	outLineAsm = inputGen.GetLinesFloat16(width + EXCEEDED_BOUNDS*2, 1, 0.0f);		
	zeros = inputGen.GetLinesFloat16(EXCEEDED_BOUNDS, 1, 0.0f);
	
	mvcvArithmeticSubFp16ToFp16(inLine1, inLine2, outLineC, width);
	arithmeticSubFp16ToFp16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubFp16ToFp16CycleCount / width);	
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0] + EXCEEDED_BOUNDS, width);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0], EXCEEDED_BOUNDS);
	outputCheck.CompareArrays(zeros[0], outLineAsm[0] + width + EXCEEDED_BOUNDS, EXCEEDED_BOUNDS);
}