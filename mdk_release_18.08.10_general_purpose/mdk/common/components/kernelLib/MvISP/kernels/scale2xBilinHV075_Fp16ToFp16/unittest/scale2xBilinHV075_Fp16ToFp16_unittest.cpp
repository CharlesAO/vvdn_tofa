//Scale2xBilinHV075_Fp16ToFp16 kernel test
// input interval is suposed to be subunitary

//Asm function prototype:
//	void mvispScale2xBilinHV075_Fp16ToFp16_asm(half **in, half **out, u32 width)

//Asm test function prototype:
//	void scale2xBilinHV075_Fp16ToFp16_asm_test(half **in, half **out, unsigned int width);

//C function prototype:
//	void mvispScale2xBilinHV075_Fp16ToFp16(half **in, half **out, u32 width)

// in         - array of pointers to input lines
// out        - pointer to output line
// inWidth    - width of output line

#include "gtest/gtest.h"
#include "scale2xBilinHV075_Fp16ToFp16.h"
#include "scale2xBilinHV075_Fp16ToFp16_asm_test.h"
#include "scale2xBilinHV075_Fp16ToFp16_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
const half DELTA = 1.0f;

using ::testing::TestWithParam;
using ::testing::Values;

class scale2xBilinHV075_Fp16ToFp16Test : public ::testing::TestWithParam <unsigned long> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLines;
	half** inLinesOffseted;
	half** outLineC;
	half** outLineAsm;
	half** outLineExpected;
	u32 width;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(scale2xBilinHV075_Fp16ToFp16Test, TestRandomInput_minus1_1_MaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width/2 + PADDING, 2, -1.0f, 1.0f);		
  inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLinesFloat16(width, 1, 0);
	outLineAsm = inputGen.GetLinesFloat16(width, 1, 0);		
 
  inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);  
	mvispScale2xBilinHV075_Fp16ToFp16(inLinesOffseted, outLineC, width);
	scale2xBilinHV075_Fp16ToFp16_asm_test(inLines, outLineAsm, width);

	CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, scale2xBilinHV075_Fp16ToFp16Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)240, (unsigned int)512, (unsigned int)880);
);

TEST_P(scale2xBilinHV075_Fp16ToFp16Test, TestRandomInputParameterizedWidth)
{
	width = GetParam() * 2;
  inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width/2 + PADDING, 2, -255.0f, 255.0f);			
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetLinesFloat16(width, 1, 0);
	outLineAsm = inputGen.GetLinesFloat16(width, 1, 0);		
 
  inLinesOffseted = inputGen.GetOffsettedLines(inLines, 2, PADDING/2);  
	mvispScale2xBilinHV075_Fp16ToFp16(inLinesOffseted, outLineC, width);
	scale2xBilinHV075_Fp16ToFp16_asm_test(inLines, outLineAsm, width);

	CompareArrays(outLineC[0], outLineAsm[0], width, DELTA);	

	// outputCheck.DumpArrays("inLines[0]", "input[0].c", inLines[0], width/2);
	// outputCheck.DumpArrays("inLines[1]", "input[1].c", inLines[1], width/2);
	// outputCheck.DumpArrays("outLineC", "outputC.c", outLineC, width);
	// outputCheck.DumpArrays("outLineAsm", "outputAsm.c", outLineAsm, width);

}
