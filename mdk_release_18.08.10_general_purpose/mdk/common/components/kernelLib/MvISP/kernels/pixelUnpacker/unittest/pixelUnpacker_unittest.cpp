//pixelUnpacker kernel test

//Asm function prototype:
//	void mvispPixelUnpacker_asm(u32* ls8b, u8* ms2b, u16 *output, u32 width, u8 shift)

//Asm test function prototype:
//	void pixelUnpacker_asm_test(unsigned long *input32, unsigned char *input8, unsigned short output, unsigned int width, unsigned char shift);

//C function prototype:
//	void mvispPixelUnpacker(u32* ls8b, u8* ms2b, u16 *output, u32 width, u8 shift)

//ms8b		- array of pointers to input line containing the 8 LSBs
//ls2b		- array of pointers to input line containing the 2 MSBs
//output	- array of pointers for output line 
//width		- width of input line
//shift		- number of bits for shifting the result to left
//awbCoef	- coefs used for WB
//line		- line number

#include "gtest/gtest.h"
#include "pixelUnpacker.h"
#include "pixelUnpacker_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

class pixelUnpackerTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u8 shift;
	u32 *inLine32;
	u8 *inLine8;
	u16 *outLineC;
	u16 *outLineAsm;
	u32 width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(pixelUnpackerTest, TestRandomInputMinWidthEvenLineShift0)
{
	width = 8;
	shift = 0;
	inputGen.SelectGenerator("random");
	inLine8 = inputGen.GetLine(width/4, 0, 255);
	float *aux = inputGen.GetLineFloat(width/4, 0, 255);	
	inputGen.SelectGenerator("uniform");
	inLine32 = (u32*)inputGen.GetLineFloat(width/4, 0);
	for(int i = 0; i < (int)width/4; i++)
	{
		inLine32[i] = (u32)aux[i] * 9876543;		
	}

	outLineC = inputGen.GetLineU16(width, 0);
	outLineAsm = inputGen.GetLineU16(width, 0);		
	
	mvispPixelUnpacker(inLine32, inLine8, outLineC, width, shift);
	pixelUnpacker_asm_test(inLine32, inLine8, outLineAsm, width, shift);
	
	outputCheck.CompareArrays(outLineC, outLineAsm, width);	
}

TEST_F(pixelUnpackerTest, TestRandomInputMinWidthOddLineShift2)
{
	width = 8;
	shift = 2;
	inputGen.SelectGenerator("random");
	inLine8 = inputGen.GetLine(width/4, 0, 255);
	float *aux = inputGen.GetLineFloat(width/4, 0, 255);	
	inputGen.SelectGenerator("uniform");
	inLine32 = (u32*)inputGen.GetLineFloat(width/4, 0);
	for(int i = 0; i < (int)width/4; i++)
	{	
		inLine32[i] = (u32)aux[i] * 9876543;				
	}
		
	outLineC = inputGen.GetLineU16(width, 0);
	outLineAsm = inputGen.GetLineU16(width, 0);		

	
	mvispPixelUnpacker(inLine32, inLine8, outLineC, width, shift);
	pixelUnpacker_asm_test(inLine32, inLine8, outLineAsm, width, shift);
	
	outputCheck.CompareArrays(outLineC, outLineAsm, width);	
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, pixelUnpackerTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880, (unsigned int)1816, (unsigned int)1920);
);

TEST_P(pixelUnpackerTest, TestRandomInputParameterizedWidthRandomShift)
{
	width = GetParam();
	shift = width%7;
	inputGen.SelectGenerator("random");
	inLine8 = inputGen.GetLine(width/4, 0, 255);
	float *aux = inputGen.GetLineFloat(width/4, 0, 0xffffffff);	
	inputGen.SelectGenerator("uniform");
	inLine32 = (u32*)inputGen.GetLineFloat(width/4, 0);

	for(int i = 0; i < (int)width/4; i++)
	{
		inLine32[i] = (u32)aux[i] * 9876543;	
	}
			
	outLineC = inputGen.GetLineU16(width, 0);
	outLineAsm = inputGen.GetLineU16(width, 0);		

	
	mvispPixelUnpacker(inLine32, inLine8, outLineC, width, shift);
	pixelUnpacker_asm_test(inLine32, inLine8, outLineAsm, width, shift);
	
	/*outputCheck.DumpArrays("input32", "input32.c", inLine32, width/4 , 1);
	outputCheck.DumpArrays("input8", "input8.c", inLine8, width/4 , 1);
	outputCheck.DumpArrays("outLineC", "outLineC.c", outLineC, width , 1);
	outputCheck.DumpArrays("outLineAsm", "outLineAsm.c", outLineAsm, width , 1);*/

	outputCheck.CompareArrays(outLineC, outLineAsm, width);	
}
