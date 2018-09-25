//genLumaFp16 test
//
// Test description:
//   generate luma from rgb input image
//

#include "gtest/gtest.h"
#include "genLumaFp16_asm_test.h"
#include <genLumaFp16.h>
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = (half)0.00000f;//accepted tolerance between C and ASM results

class appKernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float normVal;
	unsigned short *inR;
    unsigned short *inG;
    unsigned short *inB;
	half *outLineC;
	half *outLineAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(appKernelTest, Test1)
{
	width = 16;
    normVal = 0.0033f;
	inputGen.SelectGenerator("random");
	inR = inputGen.GetLineU16(width, 0, 16383);
    inG = inputGen.GetLineU16(width, 0, 16383);
    inB = inputGen.GetLineU16(width, 0, 16383);
    

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);
    
	mvispGenLumaFp16(outLineC, inR, inG, inB, normVal, width);
	genLumaFp16_asm_test(outLineAsm, inR, inG, inB, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);

    //outputChecker.DumpArrays("refInputR", "refInputR.c", inR, width);
    //outputChecker.DumpArrays("refInputG", "refInputG.c", inG, width);
    //outputChecker.DumpArrays("refInputBS", "refInputB.c", inB, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width);
}


TEST_F(appKernelTest, TestFullRandom)
{
    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(0, 1921, 8);
    int inputBpp = randGen->GenerateUInt(8, 14, 1);
    normVal = (float)1.0f/(float)(1<<inputBpp);

	inR = inputGen.GetLineU16(width, 0, (unsigned short)((1<<inputBpp)-1));
    inG = inputGen.GetLineU16(width, 0,  (unsigned short)((1<<inputBpp)-1));
    inB = inputGen.GetLineU16(width, 0,  (unsigned short)((1<<inputBpp)-1));    
    

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispGenLumaFp16(outLineC, inR, inG, inB, normVal, width);
	genLumaFp16_asm_test(outLineAsm, inR, inG, inB, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, appKernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(appKernelTest, TestRandomInputLinesAllWidth)
{

    inputGen.SelectGenerator("random");
    width = GetParam();
    
    int inputBpp = randGen->GenerateUInt(8, 14, 1);
    normVal = (float)1.0f/(float)(1<<inputBpp);    
    
  	inR = inputGen.GetLineU16(width, 0, (unsigned short)((1<<inputBpp)-1));
    inG = inputGen.GetLineU16(width, 0,  (unsigned short)((1<<inputBpp)-1));
    inB = inputGen.GetLineU16(width, 0,  (unsigned short)((1<<inputBpp)-1));    
    

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispGenLumaFp16(outLineC, inR, inG, inB, normVal, width);
	genLumaFp16_asm_test(outLineAsm, inR, inG, inB, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}


