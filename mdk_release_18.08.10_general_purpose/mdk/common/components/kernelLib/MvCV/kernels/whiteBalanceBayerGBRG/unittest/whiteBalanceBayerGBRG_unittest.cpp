//whiteBalanceBayerGBRG kernel test

//Asm function prototype:
//	 void whiteBalanceBayerGBRG_asm(u16** in, u16 awbCoef[3], 16** out, u16 clamp[1], u32 width, u8 line) 

//Asm test function prototype:
//void whiteBalanceBayerGBRG_asm_test(unsigned short **input, unsigned short awbCoef[3], unsigned short **out, unsigned short clamp[1], unsigned int width, unsigned char line);

//C function prototype:
//	 void whiteBalanceBayerGBRG(u16** in, u16 awbCoef[3], 16** out, u16 clamp[1], u32 width, u8 line) 


#include "gtest/gtest.h"
#include "whiteBalanceBayerGBRG.h"
///
#include "whiteBalanceBayerGBRG_asm_test.h"
#include "whiteBalanceBayerGBRG_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	

using ::testing::TestWithParam;
using ::testing::Values;


class whiteBalanceBayerGBRGTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned short** inLineGBRG;
	unsigned short** outLineC_GBRG;
	unsigned short** outLineAsm_GBRG;
	
	unsigned short awb[3];
	unsigned int width;
	unsigned char line;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};



/* TEST_F(whiteBalanceBayerGBRGTest, TestUniformInputWidth8LineGB)
{
	width = 8;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("uniform");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 4095);
	
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	inputGen.SelectGenerator("random");
	awb[0] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[1] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[2] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;

	clamp[0] = 16383;
	line = 2;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);

	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}

 TEST_F(whiteBalanceBayerGBRGTest, TestUniformInputWidth8LineRG)
{
	width = 8;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("uniform");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 4095);
	
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	inputGen.SelectGenerator("random");
	awb[0] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[1] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[2] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;

	clamp[0] = 16383;
	line = 1;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);

	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
} 
 */ 
 TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth24LineRG)
{
	width = 24;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 3;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);
	
	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}
 
TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth24LineGB)
{
	width = 24;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
		
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 2;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);

	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}
  
TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth32LineGB)
{
	width = 32;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 255);
	
	inputGen.SelectGenerator("random");
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
		
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 2;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);

	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}

TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth32LineRG)
{
	width = 32;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 255);
	
	inputGen.SelectGenerator("random");
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
		
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 3;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);

	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}

 TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth1920LineGB)
{
	width = 1920;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;

	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 2;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);
	
	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}  

TEST_F(whiteBalanceBayerGBRGTest, TestRandomInputWidth1920LineRG)
{
	width = 1920;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineGBRG = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	
	outLineC_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_GBRG = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;
	line = 3;
	
	mvcvWhiteBalanceBayerGBRG(inLineGBRG, awb, outLineC_GBRG, clamp, width, line);	
	whiteBalanceBayerGBRG_asm_test(inLineGBRG, awb, outLineAsm_GBRG, clamp, width, line);
	
	RecordProperty("CyclePerPixel", whiteBalanceBayerGBRGCycleCount / width);	
	CompareArrays(outLineC_GBRG[0], outLineAsm_GBRG[0], width, DELTA);
}



/* 	for (int i=0; i < width; i+=2 ){
		if(outLineC_GBRG[0][i] != outLineAsm_GBRG[0][i]) {
			if (((outLineC_GBRG[0][i] - outLineAsm_GBRG[0][i]) > DELTA) || ((outLineAsm_GBRG[0][i] - outLineC_GBRG[0][i]) > DELTA)) {
				printf("outLineC_R[0][%d] = %d, outLineAsm_R[0][%d]= %d\n",i, outLineC_GBRG[0][i], i, outLineAsm_GBRG[0][i]);
				EXPECT_EQ(2, 1);
				}
			}	
		if(outLineC_GBRG[0][i+1] != outLineAsm_GBRG[0][i+1]) {
			if (((outLineC_GBRG[0][i+1] - outLineAsm_GBRG[0][i+1]) > DELTA) || ((outLineAsm_GBRG[0][i+1] - outLineC_GBRG[0][i+1]) > DELTA)) {
				printf("outLineC_R[0][%d] = %d, outLineAsm_R[0][%d]= %d\n",i, outLineC_GBRG[0][i+1], i, outLineAsm_GBRG[0][i+1]);
				EXPECT_EQ(2, 1);
				}
			}
	} */