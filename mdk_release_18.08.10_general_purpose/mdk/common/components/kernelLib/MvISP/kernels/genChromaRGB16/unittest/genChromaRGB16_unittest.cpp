//genChromaRGB16 kernel test

//Asm function prototype:
// ; void mvispGenChromaRGB16(u16** inRGB, u8 *inY, u8** outC, int width, int eps, int nBits)

//Asm test function prototype:
// void genChromaRGB16_asm_test(unsigned short **input, unsigned char *inputY, unsigned char **output, unsigned int width, unsigned int eps, unsigned int nBits)

//C function prototype:
//	void mvispGenChromaRGB16(u16** inRGB, u8 *inY, u8** outC, int width, int eps, int nBits)


#include "gtest/gtest.h"
#include <genChromaRGB16.h>
#include "genChromaRGB16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	

using ::testing::TestWithParam;
using ::testing::Values;

class genChromaRGB16Test : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned short** inLine;
	unsigned char*  inLineY;
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	unsigned char** outLineExp;
	float chromaScale[3];
	
	unsigned int eps;
	unsigned int nBits;
	unsigned int width;
	InputGenerator inputGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(genChromaRGB16Test, TestRandomInputWidth8)
{
	 width = 8;
	chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	inputGen.SelectGenerator("random");
	 inLine = inputGen.GetLinesU16(width, 3, 0, 16383);
	 inLineY = inputGen.GetLine(width, 1, 0, 255);
	
	  nBits = randGen->GenerateUInt(8, 14);
	  eps =  randGen->GenerateUInt(0, 255);
	
	 inputGen.SelectGenerator("uniform");
	 outLineC = inputGen.GetEmptyLines(width, 3);
	 outLineAsm = inputGen.GetEmptyLines(width, 3);
		
	mvispGenChromaRGB16(inLine, inLineY, outLineC, width, eps, nBits, chromaScale);	
	genChromaRGB16_asm_test(inLine, inLineY, outLineAsm, width, eps, nBits, chromaScale);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	
}

TEST_F(genChromaRGB16Test, TestRandomInputWidth48)
{
	 width = 48;
		chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	inputGen.SelectGenerator("random");
	 inLine = inputGen.GetLinesU16(width, 3, 0, 16383);
	 inLineY = inputGen.GetLine(width, 1, 0, 255);
	
	 nBits = randGen->GenerateUInt(8, 14);
	 eps =  randGen->GenerateUInt(0, 255);
		
	inputGen.SelectGenerator("uniform");
	 outLineC = inputGen.GetLines(width, 3, 0);
	 outLineAsm = inputGen.GetLines(width, 3, 0);
	
	mvispGenChromaRGB16(inLine, inLineY, outLineC, width, eps, nBits, chromaScale);	
	genChromaRGB16_asm_test(inLine, inLineY, outLineAsm, width, eps, nBits, chromaScale);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	
}


TEST_F(genChromaRGB16Test, TestRandomInputWidth1920)
{
	width = 1920;
		chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	inputGen.SelectGenerator("random");
	 inLine = inputGen.GetLinesU16(width, 3, 0, 16383);
	 inLineY = inputGen.GetLine(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	 outLineC = inputGen.GetEmptyLines(width, 3);
	 outLineAsm = inputGen.GetEmptyLines(width, 3);
	
	 nBits=14;
	 eps=2;
		
	mvispGenChromaRGB16(inLine, inLineY, outLineC, width, eps, nBits, chromaScale);	
	genChromaRGB16_asm_test(inLine, inLineY, outLineAsm, width, eps, nBits, chromaScale);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	 
} 


TEST_F(genChromaRGB16Test, TestRandomInput)
{
	unsigned int width = 1920;
		chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	inputGen.SelectGenerator("random");
	 inLine = inputGen.GetLinesU16(width, 3, 0, 16383);
	 inLineY = inputGen.GetLine(width, 1, 0, 255);
	
	nBits = randGen->GenerateUInt(8, 14);
	eps =  randGen->GenerateUInt(0, 255);
	
	inputGen.SelectGenerator("uniform");
	 outLineC = inputGen.GetEmptyLines(width, 3);
	 outLineAsm = inputGen.GetEmptyLines(width, 3);
			
	mvispGenChromaRGB16(inLine, inLineY, outLineC, width, eps, nBits, chromaScale);	
	genChromaRGB16_asm_test(inLine, inLineY, outLineAsm, width, eps, nBits, chromaScale);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
	
	/* outputCheck.DumpArrays("inline1", "inLine1.c", inLine[0], width);	
	outputCheck.DumpArrays("inline2", "inLine2.c", inLine[1], width);	
	outputCheck.DumpArrays("inline3", "inLine3.c", inLine[2], width);	
	outputCheck.DumpArrays("inlineY", "inLineY.c", inLineY, width);	
	
	printf("nBits=%d\n", nBits);
	printf("eps=%d\n", eps); */
		
}

 INSTANTIATE_TEST_CASE_P(TestRandomData, genChromaRGB16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(genChromaRGB16Test, TestRandomData)
{

	inputGen.SelectGenerator("random");
	width = GetParam();
		chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	nBits = randGen->GenerateUInt(8, 14);
	 eps =  randGen->GenerateUInt(0, 255);
	
	 inLine = inputGen.GetLinesU16(width, 3, 0, 16383);
	 inLineY = inputGen.GetLine(width, 1, 0, 255);
	
	 outLineC = inputGen.GetEmptyLines(width, 3);
	 outLineAsm = inputGen.GetEmptyLines(width, 3);
			
	mvispGenChromaRGB16(inLine, inLineY, outLineC, width, eps, nBits, chromaScale);	
	genChromaRGB16_asm_test(inLine, inLineY, outLineAsm, width, eps, nBits, chromaScale);	

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);
	outputCheck.CompareArrays(outLineC[1], outLineAsm[1], width, DELTA);
	outputCheck.CompareArrays(outLineC[2], outLineAsm[2], width, DELTA);
}   