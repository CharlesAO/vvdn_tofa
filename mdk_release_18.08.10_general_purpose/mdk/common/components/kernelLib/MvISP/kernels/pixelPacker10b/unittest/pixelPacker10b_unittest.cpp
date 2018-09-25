//mvispPixelPacker10b kernel test

//Asm function prototype:
//	void mvispPixelPacker10b_asm(u16** srcAddr, u32** ms8b, u8** ls2b, u32 width);

//Asm test function prototype:
// 	void pixelPacker10b_asm_test(unsigned short **input, unsigned long **output32, unsigned char **output8, unsigned int width);

//C function prototype:
// 	void mvispPixelPacker10b(u16** srcAddr, u32** ms8b, u8** ls2b, u32 width);

//width		- width of input line
//in		- input line
//ms8b		- the output line with MS 8 bits
//ls2b		- the output line with LS 2 bits

#include "gtest/gtest.h"
#include "pixelPacker10b.h"
#include "pixelPacker10b_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

class pixelPacker10bTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u16* inLine;
	u8 *outLine8C, *outLine8Asm, *expectedLine8;
	u32 *outLine32C, *outLine32Asm, *expectedLine32;
	u32 width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(pixelPacker10bTest, TestWithExpectedOutput)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine 		= inputGen.GetLineU16(width, 0x0000);
	outLine8C 		= inputGen.GetEmptyLine(width);
	outLine32C 		= (u32*)inputGen.GetLineFloat(width, 1, 0);		
	outLine8Asm 	= inputGen.GetEmptyLine(width);
	outLine32Asm 	= (u32*)inputGen.GetLineFloat(width, 0);
	expectedLine8 	= inputGen.GetEmptyLine(width);
	expectedLine32 	= (u32*)inputGen.GetLineFloat(width, 0);
	
	inLine[0] = 0x031f;
	inLine[1] = 0x02ab;
	inLine[2] = 0x0134;
	inLine[3] = 0x00e2;
	inLine[4] = 0x03ff;
	inLine[5] = 0x029a;
	inLine[6] = 0x0255;
	inLine[7] = 0x018e;
	
	expectedLine32[0] = 0xe234ab1f;
	expectedLine32[1] = 0x8e559aff;
	
	expectedLine8[0] = 0xde;
	expectedLine8[1] = 0x89;
	
	mvispPixelPacker10b(inLine, outLine32C, outLine8C, width);
	pixelPacker10b_asm_test(inLine, outLine32Asm, outLine8Asm, width);

	RecordProperty("CyclePerPixel", pixelPacker10bCycleCount / width);
	
	outputCheck.CompareArrays(outLine8C, outLine8Asm, width / 4);
	outputCheck.CompareArrays(outLine32C, outLine32Asm, width / 4);
	outputCheck.CompareArrays(expectedLine8, outLine8Asm, width / 4);
	outputCheck.CompareArrays(expectedLine32, outLine32Asm, width / 4);
}



//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, pixelPacker10bTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(pixelPacker10bTest, TestRandomInputParameterizedWidth)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLineU16(width, 0x0000, 0xffff);
	outLine8C = inputGen.GetEmptyLine(width);
	outLine32C = (u32*)inputGen.GetLineFloat(width, 0);		
	outLine8Asm = inputGen.GetEmptyLine(width);
	outLine32Asm = (u32*)inputGen.GetLineFloat(width, 0);
	
	
	mvispPixelPacker10b(inLine, outLine32C, outLine8C, width);
	pixelPacker10b_asm_test(inLine, outLine32Asm, outLine8Asm, width);
	
	RecordProperty("CyclePerPixel", pixelPacker10bCycleCount / width);
			
	outputCheck.CompareArrays(outLine8C, outLine8Asm, width / 4);
	outputCheck.CompareArrays(outLine32C, outLine32Asm, width / 4);
}
