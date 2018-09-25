#include <gtest/gtest.h>
#include <purpleFlare.h>
#include "purpleFlare_asm_test.h"
#include <InputGenerator.h>
#include <UniformGenerator.h>
#include <TestEventListener.h>
#include <RandomGenerator.h>
#include "TestEventListener.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include <ArrayChecker.h>

using ::testing::TestWithParam;
using ::testing::Values;

const unsigned char PADDING = 8;

class appTest : public ::testing::TestWithParam<unsigned int> {
 protected:
	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u16* inGreen;
	u16** inBlue;
	u16* outC;
	u16* outAsm;
	unsigned int width;
	unsigned char strength;
		
	InputGenerator inputGen;
	RandomGenerator randomGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};
/*
TEST_F(appTest, TestMinimumWidth)
{
	width = 8;
	strength = 4;
	inputGen.SelectGenerator("random");
	inGreen = inputGen.GetLineU16(width + PADDING, 0, 65535);
	inBlue = inputGen.GetLinesU16(width + PADDING, 3, 0, 65535);
	
	outC = inputGen.GetEmptyLineU16(width);
	outAsm = inputGen.GetEmptyLineU16(width);	

	purpleFlare_asm_test(inGreen, inBlue, outAsm, strength, width);
	RecordProperty("CyclePerPixel", PurpleFlareCycleCount / width);

	inGreen = inGreen + 4;
	inBlue = inputGen.GetOffsettedLines(inBlue, 3, 4);
	
	mvispPurpleFlare(inGreen, inBlue, outC, strength, width);
    outputChecker.CompareArrays(outC, outAsm, width);

}
*/
INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
		
);
/*
TEST_F(appTest , Debug)
{
	width = 32;
	inputGen.SelectGenerator("random");
	strength = randomGen.GenerateUChar(0, 255);
	inGreen = inputGen.GetLineU16(width + PADDING, 0, 65535);
	inBlue = inputGen.GetLinesU16(width + PADDING, 3, 0, 65535);
	
	outC = inputGen.GetEmptyLineU16(width);
	outAsm = inputGen.GetEmptyLineU16(width);	

	purpleFlare_asm_test(inGreen, inBlue, outAsm, strength, width);
	RecordProperty("CyclePerPixel", PurpleFlareCycleCount / width);
	
	outputChecker.DumpArrays("inGreen","inGreen.txt",inGreen, width + PADDING);
	outputChecker.DumpArrays("inBlue","inBlue.txt",inBlue, width + PADDING, 3);
	
	inGreen = inGreen + 4;
	inBlue = inputGen.GetOffsettedLines(inBlue, 3, 4);
	
	mvispPurpleFlare(inGreen, inBlue, outC, strength, width);
	
	
    outputChecker.CompareArrays(outC, outAsm, width);
	
	outputChecker.DumpArrays("outC","outC.txt",outC, width);
	outputChecker.DumpArrays("outAsm","outAsm.txt",outAsm, width);
	
}
*/
TEST_P(appTest , TestRandomInputRandomStrength)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");
	strength = randomGen.GenerateUChar(0, 255);
	inGreen = inputGen.GetLineU16(width + PADDING, 0, 65535);
	inBlue = inputGen.GetLinesU16(width + PADDING, 3, 0, 65535);
	
	
	outC = inputGen.GetEmptyLineU16(width);
	outAsm = inputGen.GetEmptyLineU16(width);
	
	purpleFlare_asm_test(inGreen, inBlue, outAsm, strength, width);
	RecordProperty("CyclePerPixel", PurpleFlareCycleCount / width);
	
	//outputChecker.DumpArrays("inGreen","inGreen.txt",inGreen, width);
	//outputChecker.DumpArrays("inBlue","inBlue.txt",inBlue, width, 3);
	
	inGreen = inGreen + 4;
	inBlue = inputGen.GetOffsettedLines(inBlue, 3, 4);
	
	mvispPurpleFlare(inGreen, inBlue, outC, strength, width);
	
	
    outputChecker.CompareArrays(outC, outAsm, width);
	
	//outputChecker.DumpArrays("outC","outC.txt",outC, width);
	//outputChecker.DumpArrays("outAsm","outAsm.txt",outAsm, width);
	
}
