//extractDescriptor64_brisk kernel test
//Asm function prototype:
//     void mvcvExtractDescriptor64_brisk_asm(unsigned short *in_pixel_array,  unsigned char *out_descriptor);
//
//Asm test function prototype:
//     void extractDescriptor64_brisk_asm_test(unsigned short *in_pixel_array,  unsigned char *out_descriptor)
//
//C function prototype:
//     void mvcvExtractDescriptor64_brisk(unsigned short *in_pixel_array,  unsigned char *out_descriptor);
//
//Parameter description:
// @param[in] in        - smoothed values from a Gaussian filter from all pixels from a given pattern (brisk)
// @param[out] out      - binary value that describe the corner using a patern (binary descriptor)
//


#include "gtest/gtest.h"
#include <extractDescriptor64_brisk.h>
#include "extractDescriptor64_brisk_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA 0

class extractDescriptor64_briskKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short *inLine;
	unsigned char  *outLineC;
	unsigned char  *outLineAsm;
	
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};



TEST_F(extractDescriptor64_briskKernelTest, TestUniformInputLines32)
{

	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLineU16(64, 32);

	outLineC = inputGen.GetEmptyLine(64);
	outLineAsm = inputGen.GetEmptyLine(64);

	mvcvExtractDescriptor64_brisk(inLine, outLineC);
	extractDescriptor64_brisk_asm_test(inLine, outLineAsm);

	RecordProperty("CyclePerPixel", extractDescriptor64_briskCycleCount);

	outputChecker.CompareArrays(outLineC, outLineAsm, 64, DELTA);
}


TEST_F(extractDescriptor64_briskKernelTest, TestAllValuesZero)
{
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLineU16(64, 0);

	outLineC = inputGen.GetEmptyLine(64);
	outLineAsm = inputGen.GetEmptyLine(64);

	mvcvExtractDescriptor64_brisk(inLine, outLineC);
	extractDescriptor64_brisk_asm_test(inLine, outLineAsm);

	RecordProperty("CyclePerPixel", extractDescriptor64_briskCycleCount);

	outputChecker.CompareArrays(outLineC, outLineAsm, 64, DELTA);
}

TEST_F(extractDescriptor64_briskKernelTest, TestAllValues255)
{
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLineU16(64, 255);

	outLineC = inputGen.GetEmptyLine(64);
	outLineAsm = inputGen.GetEmptyLine(64);

	mvcvExtractDescriptor64_brisk(inLine, outLineC);
	extractDescriptor64_brisk_asm_test(inLine, outLineAsm);

	RecordProperty("CyclePerPixel", extractDescriptor64_briskCycleCount);

	outputChecker.CompareArrays(outLineC, outLineAsm, 64, DELTA);
}



TEST_F(extractDescriptor64_briskKernelTest, TestRandomWidthRandomData)
{
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLineU16(64, 0, 255);

	outLineC = inputGen.GetLine(64, 0);
	outLineAsm = inputGen.GetLine(64, 0);

	mvcvExtractDescriptor64_brisk(inLine, outLineC);
	extractDescriptor64_brisk_asm_test(inLine, outLineAsm);

	RecordProperty("CyclePerPixel", extractDescriptor64_briskCycleCount);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, 64, DELTA);
}
