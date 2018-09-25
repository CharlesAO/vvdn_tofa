#include "gtest/gtest.h"
#include <mixMedian.h>
#include "mixMedian_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

const unsigned char  DELTA = 2;

class appTest : public ::testing::TestWithParam<unsigned int> {
 protected:
	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u8** in0Lines;
	u8** in1Lines;
	u8*  refLines;	
	u8** outC;
	u8** outAsm;
	unsigned int width;
	float offset;
	float slope;
	
	InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;	
    
    virtual void TearDown() {}
};

TEST_F(appTest , Test1)
{
	width = 16;
	offset = 0.2f;
	slope =  5.0f;	
	inputGen.SelectGenerator("random");
	in0Lines = inputGen.GetLines(width, 3, 0, 255);
	in1Lines = inputGen.GetLines(width, 3, 0, 255);
	refLines = inputGen.GetLine(width, 0, 255);
	outC = inputGen.GetEmptyLines(width, 3);
	outAsm = inputGen.GetEmptyLines(width, 3);	

	mvispMixMedian         (outC   , in0Lines, in1Lines, refLines, offset, slope, width);	
    mixMedian_asm_test(outAsm , in0Lines, in1Lines, refLines, offset, slope, width);

    //outputChecker.CompareArrays(outC, outAsm, width, 3, (u8)DELTA);
	//outputChecker.DumpArrays("outC","outC.h",outC, width, 3);
	//outputChecker.DumpArrays("outAsm","outAsm.h",outAsm, width, 3);
	//outputChecker.DumpArrays("in0Lines","in0Lines.h",in0Lines, width, 3);
	//outputChecker.DumpArrays("in1Lines","in1Lines.h",in1Lines, width, 3);
	//outputChecker.DumpArrays("refLines","refLines.h",refLines, width);
}

TEST_F(appTest , TestRandomAll)
{
	// width need to be multiple of 8
	width = ((randGen->GenerateUInt(8,1920))>>3)<<3;
	offset = randGen->GenerateFloat(-5.0f, 10.0f);
	slope =  randGen->GenerateFloat(-10.0f, 30.0f);
	inputGen.SelectGenerator("random");
	in0Lines = inputGen.GetLines(width, 3, 0, 255);
	in1Lines = inputGen.GetLines(width, 3, 0, 255);
	refLines = inputGen.GetLine(width, 0, 255);
	outC = inputGen.GetEmptyLines(width, 3);
	outAsm = inputGen.GetEmptyLines(width, 3);	

	mvispMixMedian         (outC   , in0Lines, in1Lines, refLines, offset, slope, width);	
    mixMedian_asm_test(outAsm , in0Lines, in1Lines, refLines, offset, slope, width);
}


INSTANTIATE_TEST_CASE_P(RandomInputs, appTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(appTest , TestRandomData)
{

	width = GetParam();
	offset = randGen->GenerateFloat(-1.0f, 2.0f);
	slope =  randGen->GenerateFloat(-1.0f, 20.0f);
	inputGen.SelectGenerator("random");
	in0Lines = inputGen.GetLines(width, 3, 0, 255);
	in1Lines = inputGen.GetLines(width, 3, 0, 255);
	refLines = inputGen.GetLine(width, 0, 255);
	outC = inputGen.GetEmptyLines(width, 3);
	outAsm = inputGen.GetEmptyLines(width, 3);	

	mvispMixMedian         (outC   , in0Lines, in1Lines, refLines, offset, slope, width);	
    mixMedian_asm_test(outAsm , in0Lines, in1Lines, refLines, offset, slope, width);
	
	outputChecker.CompareArrays(outC, outAsm, width, 3, (u8)DELTA);
}

