//thresholdBinaryRange kernel test

//Asm function prototype:
//	 void mvcvThresholdBinaryRange(u8** in, u8** out, u8 lowerValue, u8 upperValue, u32 width);

//Asm test function prototype:
//    	void thresholdBinaryU8_asm_test(unsigned char** in, unsigned char** out, unsigned char threshold, unsigned int width);

//C function prototype:
//  	 void mvcvThresholdBinaryRange(u8** in, u8** out, u8 lowerValue, u8 upperValue, u32 width);



#include "gtest/gtest.h"
#include "thresholdBinaryRange.h"
#include "thresholdBinaryRange_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class thresholdBinaryRangeTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine;
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	
	unsigned char lowerValue;
	unsigned char upperValue;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth8)
{
	width = 8;
	lowerValue = 81;
	upperValue = 210;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	
}

TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth64)
{
	width = 64;
	lowerValue = 31;
	upperValue = 210;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);

}


TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth280)
{
	width = 280;
	lowerValue = 131;
	upperValue = 255;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);	
}

TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth512Range0)
{
	width = 512;
	lowerValue = 0;
	upperValue = 0;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	
		
}

TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth16ExpectedValue)
{
	width = 16;
	lowerValue = 30;
	upperValue = 190;
	
	
	unsigned char** outLineExpected;
		
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	outLineExpected = inputGen.GetEmptyLines(width, 1);		
	
	
	inLine[0][0] = 27;     outLineExpected[0][0] = 0;
	inLine[0][1] = 56;     outLineExpected[0][1] = 255;
	inLine[0][2] = 224;    outLineExpected[0][2] = 0;
	inLine[0][3] = 89;     outLineExpected[0][3] = 255;
	inLine[0][4] = 156;    outLineExpected[0][4] = 255;
	inLine[0][5] = 23;     outLineExpected[0][5] = 0;
	inLine[0][6] = 115;    outLineExpected[0][6] = 255;
	inLine[0][7] = 49;     outLineExpected[0][7] = 255; 
	inLine[0][8] = 127;    outLineExpected[0][8] = 255;
	inLine[0][9] = 96;     outLineExpected[0][9] = 255;
	inLine[0][10] = 124;   outLineExpected[0][10] = 255;
	inLine[0][11] = 78;    outLineExpected[0][11] = 255;
	inLine[0][12] = 16;    outLineExpected[0][12] = 0;
	inLine[0][13] = 93;    outLineExpected[0][13] = 255;
	inLine[0][14] = 15;    outLineExpected[0][14] = 0;
	inLine[0][15] = 149;   outLineExpected[0][15] = 255;
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineAsm[0], outLineExpected[0], width);
			
}



TEST_F(thresholdBinaryRangeTest, TestRandomInputWidth408Range255)
{
	width = 408;
	lowerValue = 255;
	upperValue = 255;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);

}


TEST_F(thresholdBinaryRangeTest, TestRandomInputMaximWidth)
{
	width = 1920;
	lowerValue = 31;
	upperValue = 200;
	
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLines(width, 1, 0, 255);
		
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);		
	
	mvcvThresholdBinaryRange(inLine, outLineC, lowerValue, upperValue, width);
	thresholdBinaryRange_asm_test(inLine, outLineAsm, lowerValue, upperValue, width);
	
	RecordProperty("CyclePerPixel", thresholdBinaryRangeCycleCount / width);
			
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	
	
}
