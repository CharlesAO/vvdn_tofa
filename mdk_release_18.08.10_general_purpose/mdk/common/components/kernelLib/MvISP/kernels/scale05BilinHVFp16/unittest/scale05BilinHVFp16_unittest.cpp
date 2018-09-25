//scale05BilinHVFp16 kernel test

//Asm function prototype:
//	void mvispScale05BilinHVFp16_asm(half** in, half** out, u32 width);

//Asm test function prototype:
// 	void scale05BilinHVFp16_asm_test(half** input, half** output, unsigned int width);

//C function prototype:
//  void mvispScale05BilinHVFp16(half** in, half** out, u32 width);

//width		- width of input and output line 
//in		- input line
//out		- output line

#include "gtest/gtest.h"
#include "scale05BilinHVFp16.h"
#include "scale05BilinHVFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;
 
half DELTA = 1;
 
class scale05BilinHVFp16Test : public ::testing::TestWithParam <unsigned long> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLine;
	half** outLineC;
	half** outLineAsm;
	half** outLineExpected;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(scale05BilinHVFp16Test, TestUniformInput)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLinesFloat16(width, 2, 10.0f);
	outLineExpected = inputGen.GetLinesFloat16(width, 1, 10.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);		
	
	mvispScale05BilinHVFp16(inLine, outLineC, width);
	scale05BilinHVFp16_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale05BilinHVFp16CycleCount / width);	
	
	outputCheck.CompareArrays(outLineExpected[0], outLineC[0], width / 2);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width / 2);
}

TEST_F(scale05BilinHVFp16Test, TestRandomDATASmallWidth)
{
	width = randGen->GenerateUInt(8, 1281, 8);
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLinesFloat16(width, 2, 0.0f, 255.0f);
	outLineExpected = inputGen.GetLinesFloat16(width, 1, 10.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);		
	
	mvispScale05BilinHVFp16(inLine, outLineC, width);
	scale05BilinHVFp16_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale05BilinHVFp16CycleCount / width);	
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width / 2, DELTA);
}

TEST_F(scale05BilinHVFp16Test, TestRandomDATABigWidth)
{
	width = randGen->GenerateUInt(1280, 1921, 8);
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLinesFloat16(width, 2, 0.0f, 255.0f);
	outLineExpected = inputGen.GetLinesFloat16(width, 1, 10.0f);
	outLineC = inputGen.GetEmptyLinesFp16(width, 1);
	outLineAsm = inputGen.GetEmptyLinesFp16(width, 1);		
	
	mvispScale05BilinHVFp16(inLine, outLineC, width);
	scale05BilinHVFp16_asm_test(inLine, outLineAsm, width);
	RecordProperty("CyclePerPixel", scale05BilinHVFp16CycleCount / width);	
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width / 2, DELTA);
}

