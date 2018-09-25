//greyDesat kernel test

//Asm function prototype:
// ; void greyDesat(u8* in[3], u8* out[3], int offset, int slope, int grey[3], u32 width)

//Asm test function prototype:
// void greyDesat_asm_test(unsigned char **input, unsigned char **output, int offset,  int slope, int grey[3], unsigned int width)

//C function prototype:
//	void greyDesat(u8* in[3], u8* out[3], int offset, int slope, int grey[3], u32 width)


#include "gtest/gtest.h"
#include <greyDesat.h>
#include "greyDesat_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	
const unsigned short OUTPUT_PADDING = 16;	

using ::testing::TestWithParam;
using ::testing::Values;

class greyDesatTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine;
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	unsigned char** outLineExp;
	
	int slope;
	int offset;
	int grey[3];
	int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


 TEST_F(greyDesatTest, TestRandomInput1)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 3, 0, 255);
	
	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
	
	offset=-2;
	slope=30;
	
	grey[0] = 128;
	grey[1] = 120;
	grey[2] = 112;
	
	
	mvispGreyDesat(inLine, outLineC, offset, slope, grey, width);	
	greyDesat_asm_test(inLine, outLineAsm, offset, slope, grey, width);	

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);

} 

  TEST_F(greyDesatTest, TestUniformInput255)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLines(width, 3,255);
	
	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
	
	offset=-255;
	slope=32;
	
	grey[0] = 255;
	grey[1] = 255;
	grey[2] = 255;
	
	
	mvispGreyDesat(inLine, outLineC, offset, slope, grey, width);	
	greyDesat_asm_test(inLine, outLineAsm, offset, slope, grey, width);	

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	
}

 TEST_F(greyDesatTest, TestRandomInputSlope40)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetEmptyLines(width, 3);
	
	unsigned char** outLineC = inputGen.GetEmptyLines(width, 3);
	unsigned char** outLineAsm = inputGen.GetEmptyLines(width, 3);
	
	inLine[0][0] = 126;   inLine[1][0] = 113;      inLine[2][0] = 119;
	inLine[0][1] = 128;   inLine[1][1] = 115;     inLine[2][1] = 123;
	inLine[0][2] = 249;   inLine[1][2] = 46;      inLine[2][2] = 75;
	inLine[0][3] = 1;     inLine[1][3] = 184;     inLine[2][3] = 221;
	inLine[0][4] = 129;   inLine[1][4] = 223;     inLine[2][4] = 86;
	inLine[0][5] = 177;   inLine[1][5] = 116;     inLine[2][5] = 214;
	inLine[0][6] = 2;     inLine[1][6] = 153;     inLine[2][6] = 127;
	inLine[0][7] = 44;    inLine[1][7] = 84;      inLine[2][7] = 33;
		
	offset=-16;
	slope=40;
	
	grey[0] = 128;
	grey[1] = 120;
	grey[2] = 112;
	
	mvispGreyDesat(inLine, outLineC, offset, slope, grey, width);	
	greyDesat_asm_test(inLine, outLineAsm, offset, slope, grey, width);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
				
	//outputCheck.DumpArrays("inline1", "inLine1.c", inLine[0], width);	
	//outputCheck.DumpArrays("inline2", "inLine2.c", inLine[1], width);	
	//outputCheck.DumpArrays("inline3", "inLine3.c", inLine[2], width);	

}

 
TEST_F(greyDesatTest, TestRandomInputGrey0)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 3, 0, 255);
	
	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
	
	offset = 255;
	slope  = 32;
	
	grey[0] = 0;
	grey[1] = 0;
	grey[2] = 0;
	
	
	mvispGreyDesat(inLine, outLineC, offset, slope, grey, width);	
	greyDesat_asm_test(inLine, outLineAsm, offset, slope, grey, width);	

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	
	//outputCheck.DumpArrays("inline1", "inLine1.c", inLine[0], width);	
	//outputCheck.DumpArrays("inline2", "inLine2.c", inLine[1], width);	
	//outputCheck.DumpArrays("inline3", "inLine3.c", inLine[2], width);	

}  



INSTANTIATE_TEST_CASE_P(RandomInputs, greyDesatTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);


 TEST_P(greyDesatTest, TestRandomData)
{

	inputGen.SelectGenerator("random");
	width = GetParam();
	
	offset = randGen->GenerateUInt(0, 255);
	slope = randGen->GenerateUInt(0, 32);
	grey[0] = randGen->GenerateUInt(0, 255);
	grey[1] = randGen->GenerateUInt(0, 255);
	grey[2] = randGen->GenerateUInt(0, 255);
	
	inLine = inputGen.GetLines(width, 3, 0, 255);
	outLineC   = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
		
	mvispGreyDesat(inLine, outLineC, offset, slope, grey, width);	
	greyDesat_asm_test(inLine, outLineAsm, offset, slope, grey, width);	

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
}    