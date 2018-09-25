
//Parameter description:
// mvcvCensusMatching 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <convertYUV400ToYUV422.h>
#include "convertYUV400ToYUV422_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16

class mvcvConvertYUV400ToYUV422KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *input;
	unsigned short *outLineC;
	unsigned short *outLineAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvConvertYUV400ToYUV422KernelTest, TestWidth16All0)
{
    
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	input = inputGen.GetLine(width, 0);
	
	

	outLineC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
    outLineAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
	
	inputGen.FillLine(outLineC,  width  + PADDING , 5);
	inputGen.FillLine(outLineAsm,width  + PADDING , 5);
	

	mvcvConvertYUV400ToYUV422(input, outLineC + PADDING/2, width);
	convertYUV400ToYUV422_asm_test(input , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", convertYUV400ToYUV422CycleCount / (width ));
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}

TEST_F(mvcvConvertYUV400ToYUV422KernelTest, TestWidth16AllMAX)
{
	width = 16;
	
	inputGen.SelectGenerator("uniform");
	input = inputGen.GetLine(width, 255);
	
	

	outLineC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
    outLineAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
	
	inputGen.FillLine(outLineC,  width  + PADDING , 5);
	inputGen.FillLine(outLineAsm,width  + PADDING , 5);
	

	mvcvConvertYUV400ToYUV422(input, outLineC + PADDING/2, width);
	convertYUV400ToYUV422_asm_test(input , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", convertYUV400ToYUV422CycleCount / (width ));
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}


TEST_F(mvcvConvertYUV400ToYUV422KernelTest, TestRandomDATARandomWidth)
{
    
	width = randGen->GenerateUInt(8, 1920, 8);
	
	inputGen.SelectGenerator("random");
	input = inputGen.GetLine(width, 0,  255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
    outLineAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
	
	inputGen.FillLine(outLineC,  width  + PADDING , 5);
	inputGen.FillLine(outLineAsm,width  + PADDING , 5);
	

	mvcvConvertYUV400ToYUV422(input, outLineC + PADDING/2, width);
	convertYUV400ToYUV422_asm_test(input , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", convertYUV400ToYUV422CycleCount / (width ));
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}


TEST_F(mvcvConvertYUV400ToYUV422KernelTest, TestRandomDATARandom1Width)
{
	width = randGen->GenerateUInt(1280, 1920, 8);
	
	inputGen.SelectGenerator("random");
	input = inputGen.GetLine(width, 0,  255);
	
	inputGen.SelectGenerator("uniform");
	outLineC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
    outLineAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING) * sizeof(short));
	
	inputGen.FillLine(outLineC,  width  + PADDING , 5);
	inputGen.FillLine(outLineAsm,width  + PADDING , 5);
	

	mvcvConvertYUV400ToYUV422(input, outLineC + PADDING/2, width);
	convertYUV400ToYUV422_asm_test(input , outLineAsm , width);
	
	RecordProperty("CyclePerPixel", convertYUV400ToYUV422CycleCount / (width ));
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING);
}