//monoImbalance kernel test
//Asm test function prototype:
//    void MonoImbalance_asm_test(u16** in, half** out,int thrDark, int thrBright, int inputBits, u32 width)
//
//C function prototype:
//    void mvcvMonoImbalance(u16** in, half** out,int thrDark, int thrBright, int inputBits, u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// thrDark
// thrBright
// inputBits
// inWidth    - width of input line
//


#include "gtest/gtest.h"
#include "monoImbalance.h"
///
#include "monoImbalance_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"
#include <stdio.h> 

using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 8

half delta=1.0;//accepted tolerance between C and ASM results
#define SIZE 3
class MonoImbalanceKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	unsigned short **inLines;
	half **outLinesC;
	half **outLinesAsm;
	unsigned int width;
	int thrDark,thrBright,inputBits;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};



TEST_F(MonoImbalanceKernelTest, TestExperimental)
{

	width = 1920;
	thrDark =16; 
	thrBright = 81;
	inputBits = 10;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesU16(width + PADDING, 3, 0x0,0x3fff);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	MonoImbalance_asm_test(inLines, outLinesAsm, thrDark, thrBright, inputBits, width);
	RecordProperty("CyclePerPixel", monoImbalanceCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2); 
	mvcvMonoImbalance(inLines, outLinesC,  thrDark, thrBright, inputBits, width);
		
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width, delta);
  

}


