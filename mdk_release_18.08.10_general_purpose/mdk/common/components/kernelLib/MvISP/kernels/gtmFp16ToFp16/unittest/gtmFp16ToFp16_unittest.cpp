
#include "gtest/gtest.h"
#include "gtmFp16ToFp16_asm_test.h"
#include "gtmFp16ToFp16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

#define PADDING 16

class gtmFp16ToFp16Test : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	
	half *in;
	half *outLineC;
	half *outLineAsm;
    half *scurve;
	unsigned int width;
    int nPoints; 
    
	RandomGenerator dataGenerator;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

half DELTA = 0;//accepted tolerance between C and ASM results

TEST_F(gtmFp16ToFp16Test, TestRandomInput)
{
	inputGen.SelectGenerator("random");
    width = 1024;
    nPoints  = 4; 
     
	in = inputGen.GetLineFloat16(width + PADDING, 3, 255);
	scurve = inputGen.GetLineFloat16(width + PADDING, 1, 10);
	
	outLineC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outLineAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispGtmFp16ToFp16(in, outLineC, width, scurve, nPoints);
	gtmFp16ToFp16_asm_test(in, outLineAsm, width, scurve, nPoints);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING,DELTA);
}

TEST_F(gtmFp16ToFp16Test, TestRandomInputMinWidth)
{
	inputGen.SelectGenerator("random");
    width = 8;
    nPoints  = 4; 
     
	in = inputGen.GetLineFloat16(width + PADDING, 0, 2);
	scurve = inputGen.GetLineFloat16(width + PADDING, 1, 10);
	
	outLineC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outLineAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispGtmFp16ToFp16(in, outLineC, width, scurve, nPoints);
	gtmFp16ToFp16_asm_test(in, outLineAsm, width, scurve, nPoints);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING,DELTA);
}

TEST_F(gtmFp16ToFp16Test, TestRandomInputMaxWidth)
{
	inputGen.SelectGenerator("random");
    width = 1920;
    nPoints  = 4; 
     
	in = inputGen.GetLineFloat16(width + PADDING, 0, 480);
	scurve = inputGen.GetLineFloat16(width + PADDING, 0, 255);
	
	outLineC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outLineAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispGtmFp16ToFp16(in, outLineC, width, scurve, nPoints);
	gtmFp16ToFp16_asm_test(in, outLineAsm, width, scurve, nPoints);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING,DELTA);
}

TEST_F(gtmFp16ToFp16Test, TestRandomInputRandomData)
{
	inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(8, 1920, 8);
    nPoints  = randGen->GenerateUInt(8, width, 8);
     
	in = inputGen.GetLineFloat16(width + PADDING, 0, (half)((float)(width)/((float)(nPoints))));
	scurve = inputGen.GetLineFloat16(width + PADDING, 0, 255);
	
	outLineC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outLineAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispGtmFp16ToFp16(in, outLineC, width, scurve, nPoints);
	gtmFp16ToFp16_asm_test(in, outLineAsm, width, scurve, nPoints);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width + PADDING,DELTA);
}
