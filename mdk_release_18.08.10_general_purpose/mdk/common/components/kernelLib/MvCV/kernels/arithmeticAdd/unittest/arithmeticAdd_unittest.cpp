//SAD5x5 kernel test

//Asm function prototype:
//	 void sarithmeticAdd_asm(u8** in1, u8** in2, u8** out, u32 width);

//Asm test function prototype:
//    	void arithmeticAdd_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width);

//C function prototype:
//  	 void sarithmeticAdd(u8** in1, u8** in2, u8** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "arithmeticAdd.h"
#include "arithmeticAdd_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class arithmeticAddTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine1;
	unsigned char** inLine2;
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	unsigned char** outLineExp	;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(arithmeticAddTest, TestWidth240)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 2);
	inLine2 = inputGen.GetLines(width, 1, 4);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticAddTest, TestWidth72)
{
	width = 72;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 45);
	inLine2 = inputGen.GetLines(width, 1, 120);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticAddTest, TestWidth32)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 5);
	inLine2 = inputGen.GetLines(width, 1, 20);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticAddTest, TestWidthMinimWidth)
{
	width = 8;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1,0, 255);
	inLine2 = inputGen.GetLines(width, 1,0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
}

TEST_F(arithmeticAddTest, TestWidth64)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 155);
	inLine2 = inputGen.GetLines(width, 1, 123);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
				
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
TEST_F(arithmeticAddTest, TestValues0)
{
	width = 280;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 0);
	inLine2 = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}

TEST_F(arithmeticAddTest, TestValues255)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 255);
	inLine2 = inputGen.GetLines(width, 1, 255);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(arithmeticAddTest, TestExpectedValue)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 0);
	inLine2 = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	outLineExp = inputGen.GetEmptyLines(width, 1);		
	
	for(int i = 0; i < 64; i++){
	inLine1[0][i] = i + 1;
	inLine2[0][i] = i + 3;
	}
	
	for(int i = 2; i <= 65; i++){
	outLineExp[0][i-2] = 2 * i;
	}
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineExp[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineExp[0], outLineAsm[0], width);
}

TEST_F(arithmeticAddTest, TestMaximValue)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	arithmeticAdd_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", arithmeticAddCycleCount / width);
	
	mvcvArithmeticAdd(inLine1, inLine2, outLineC, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
