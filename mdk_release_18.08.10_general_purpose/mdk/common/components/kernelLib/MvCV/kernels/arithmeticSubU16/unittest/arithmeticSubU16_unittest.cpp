//kernel test

//Asm function prototype:
//	 void arithmeticSubU16_asm(u16** in1, u16** in2, s16** out, u32 width);

//Asm test function prototype:
//    	void arithmeticSubU16_asm_test(unsigned short **input1, unsigned short **input2, unsigned short **output, unsigned int width);

//C function prototype:
//  	 void arithmeticSubU16(u16** in1, u16** in2, s16** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "arithmeticSubU16.h"
#include "arithmeticSubU16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>





class arithmeticSubU16Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned short** inLine1;
	unsigned short** inLine2;
	short** outLineC;
	short** outLineAsm;
	short** outLineExp;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(arithmeticSubU16Test, TestWidth240)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 2);
	inLine2 = inputGen.GetLinesU16(width, 1, 4);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticSubU16Test, TestWidth72)
{
	width = 72;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 45);
	inLine2 = inputGen.GetLinesU16(width, 1, 120);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticSubU16Test, TestWidth32)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 55);
	inLine2 = inputGen.GetLinesU16(width, 1, 20);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticSubU16Test, TestMinimWidth8)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLinesU16(width, 1, 0, 255);
	inLine2 = inputGen.GetLinesU16(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}


TEST_F(arithmeticSubU16Test, TestWidth64)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 155);
	inLine2 = inputGen.GetLinesU16(width, 1, 123);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
				
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
TEST_F(arithmeticSubU16Test, TestValues0)
{
	width = 280;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 0);
	inLine2 = inputGen.GetLinesU16(width, 1, 0);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(arithmeticSubU16Test, TestValues255)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 255);
	inLine2 = inputGen.GetLinesU16(width, 1, 255);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(arithmeticSubU16Test, TestExpectedValue)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLinesU16(width, 1, 0);
	inLine2 = inputGen.GetLinesU16(width, 1, 0);
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineExp = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	for(int i = 0; i < 64; i++){
	inLine1[0][i] = i + 3;
	inLine2[0][i] = i + 1;
	}
	
	for(int i = 2; i <= 65; i++){
	outLineExp[0][i-2] = 2;
	}
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineExp[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineExp[0], outLineAsm[0], width);
}

TEST_F(arithmeticSubU16Test, TestRandomWidthRandomValues)
{
	width = randGen->GenerateUInt(16, 1921, 8);;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLinesU16(width, 1, 0, 255);
	inLine2 = inputGen.GetLinesU16(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(arithmeticSubU16Test, TestMaximWidthValue)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLinesU16(width, 1, 0, 255);
	inLine2 = inputGen.GetLinesU16(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	outLineAsm = (short**)inputGen.GetEmptyLines(width * sizeof(short), 1);
	
	arithmeticSubU16_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticSubU16CycleCount / width);
	
	mvcvArithmeticSubU16(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
