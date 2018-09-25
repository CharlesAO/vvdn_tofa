//genLumaFp16 test
//
// Test description:
//   generate luma from rgb input image
//

#include "gtest/gtest.h"
#include "genLumaU8Fp16_asm_test.h"
#include <genLumaU8Fp16.h>
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = (half)0.30000f;//accepted tolerance between C and ASM results

class genLumaU8Fp16Test : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	
	unsigned char *inR;
    unsigned char *inG;
    unsigned char *inB;
	half *outLineC;
	half *outLineAsm;
	half *outLineExp;
	half coefs[3];
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};



TEST_F(genLumaU8Fp16Test, TestFullRandom)
{
    inputGen.SelectGenerator("random");
    width = 1920;
    coefs[0] = (half)0.25;
    coefs[1] = (half)0.50;
    coefs[3] = (half)0.25;
	inR = inputGen.GetLine(width, 0, 255);
	inG = inputGen.GetLine(width, 0, 255);
	inB = inputGen.GetLine(width, 0, 255);

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	
	mvispGenLumaU8Fp16(inR, inG, inB, outLineC, coefs, width);
	genLumaU8Fp16_asm_test(inR, inG, inB, outLineAsm, coefs, width);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
	
}

//------------ parameterized tests ----------------------------------------

 INSTANTIATE_TEST_CASE_P(UniformInputs, genLumaU8Fp16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)24, (unsigned int)32, (unsigned int)320, (unsigned int)376, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920); //8
);

TEST_P(genLumaU8Fp16Test, TestRandomInputLinesAllWidth)
{

    inputGen.SelectGenerator("random");
    width = GetParam();
    coefs[0] = (half)0.25;
    coefs[1] = (half)0.50;
    coefs[3] = (half)0.25;
 	inR = inputGen.GetLine(width, 0, 255);
	inG = inputGen.GetLine(width, 0, 255);
	inB = inputGen.GetLine(width, 0, 255);   
 
	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispGenLumaU8Fp16(inR, inG, inB, outLineC, coefs, width);
	genLumaU8Fp16_asm_test(inR, inG, inB, outLineAsm, coefs, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}
