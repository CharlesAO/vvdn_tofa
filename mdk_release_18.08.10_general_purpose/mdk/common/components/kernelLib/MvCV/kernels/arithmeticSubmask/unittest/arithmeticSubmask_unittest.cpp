//SAD5x5 kernel test

//Asm function prototype:
//	 void sarithmeticSubmask_asm(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//Asm test function prototype:
//    	void arithmeticSubmask_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned char **mask,unsigned int width);

//C function prototype:
//  	 void sarithmeticSubmask(u8** in1, u8** in2, u8** out, u8** mask, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "arithmeticSubmask.h"
#include "arithmeticSubmask_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>





class arithmeticSubmaskTest : public ::testing::Test {
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
	unsigned char** mask;
	unsigned char** outLineAsm;
	unsigned char** outLineExp;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};
 
TEST_F(arithmeticSubmaskTest, TestWidth240)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 2);
	inLine2 = inputGen.GetLines(width, 1, 4);

	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);	
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
		
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 

}
 
TEST_F(arithmeticSubmaskTest, TestWidth72)
{
	width = 72;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 45);
	inLine2 = inputGen.GetLines(width, 1, 120);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 

	
} 

TEST_F(arithmeticSubmaskTest, TestWidth32)
{
	width = 32;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 89);
	inLine2 = inputGen.GetLines(width, 1, 45);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
	
}

TEST_F(arithmeticSubmaskTest, TestWidth24)
{
	width = 24;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 48);
	inLine2 = inputGen.GetLines(width, 1, 10);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 
	
}

TEST_F(arithmeticSubmaskTest, TestMinimWidth8)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 77);
	inLine2 = inputGen.GetLines(width, 1, 129);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width); 

	
}

TEST_F(arithmeticSubmaskTest, TestWidth64)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 155);
	inLine2 = inputGen.GetLines(width, 1, 123);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
				
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
} 


TEST_F(arithmeticSubmaskTest, TestValues0)
{
	width = 280;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 1);
	inLine2 = inputGen.GetLines(width, 1, 1);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);	
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	
 
	
}
 
TEST_F(arithmeticSubmaskTest, TestValues255)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 255);
	inLine2 = inputGen.GetLines(width, 1, 255);
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);

	
	}

TEST_F(arithmeticSubmaskTest, TestExpectedValue)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width, 1, 6);
	inLine2 = inputGen.GetLines(width, 1, 3);
	
	
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);			
	outLineExp = inputGen.GetLines(width, 1, 0);			
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0);
	
	
	for(int i = 0; i < 64; i+=8){
	mask[0][i + 0] = 1;	
	mask[0][i + 1] = 1;	
	mask[0][i + 2] = 0;	
	mask[0][i + 3] = 0;	
	mask[0][i + 4] = 1;	
	mask[0][i + 5] = 1;	
	mask[0][i + 6] = 1;	
	mask[0][i + 7] = 0;	
	}
	
	for(int i = 0; i < 64; i+=8){
	outLineExp[0][i + 0] = 3;	
	outLineExp[0][i + 1] = 3;	
	outLineExp[0][i + 2] = 0;	
	outLineExp[0][i + 3] = 0;	
	outLineExp[0][i + 4] = 3;	
	outLineExp[0][i + 5] = 3;	
	outLineExp[0][i + 6] = 3;	
	outLineExp[0][i + 7] = 0;	
	}
	
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
			
	outputCheck.CompareArrays(outLineExp[0], outLineC[0], width);
	outputCheck.CompareArrays(outLineExp[0], outLineAsm[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);

} 
	
	
 
TEST_F(arithmeticSubmaskTest, TestMaximValue)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width, 1, 0, 255);
	inLine2 = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("random");
	mask = inputGen.GetLines(width, 1, 0, 2);
	
	
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);		
	
	arithmeticSubmask_asm_test(inLine1, inLine2, outLineAsm, mask, width);
	RecordProperty("CyclePerPixel", arithmeticSubmaskCycleCount / width);
	
	mvcvArithmeticSubmask(inLine1, inLine2, outLineC, mask, width);
			
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}
 