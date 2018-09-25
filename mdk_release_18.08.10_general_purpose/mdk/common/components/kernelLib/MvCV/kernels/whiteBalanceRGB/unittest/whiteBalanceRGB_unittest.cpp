//cvtColorKernelRGBtoUV420 kernel test

//Asm function prototype:
//	 void whiteBalanceRGB_asm(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], 16** oR, u16** oG, u16** oB, u16 clamp[1], u32 width) 

//Asm test function prototype:
//void whiteBalanceRGB_asm_test(unsigned short **inputR, unsigned short **inputG, unsigned short **inputB, unsigned short awbCoef[3], unsigned short **Rout, unsigned short **Gout, unsigned short **Bout, unsigned short clamp[1], unsigned int width);

//C function prototype:
//	 void whiteBalanceRGB(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], 16** oR, u16** oG, u16** oB, u16 clamp[1], u32 width) 


#include "gtest/gtest.h"
#include "whiteBalanceRGB.h"
///
#include "whiteBalanceRGB_asm_test.h"
#include "whiteBalanceRGB_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	

using ::testing::TestWithParam;
using ::testing::Values;

class whiteBalanceRGBTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned short** inLineR;
	unsigned short** inLineG;
	unsigned short** inLineB;
	
	unsigned short** outLineC_R;
	unsigned short** outLineC_G;
	unsigned short** outLineC_B;

	unsigned short** outLineAsm_R;
	unsigned short** outLineAsm_G;
	unsigned short** outLineAsm_B;

	unsigned short awb[3];
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};



TEST_F(whiteBalanceRGBTest, TestUniformInputWidth8)
{
	width = 8;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLinesU16(width, 1, 16383);
	inLineG = inputGen.GetLinesU16(width, 1, 16383);
	inLineB = inputGen.GetLinesU16(width, 1, 16383);
	
	outLineC_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_B = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_B = inputGen.GetEmptyLinesU16(width, 1);
	
	inputGen.SelectGenerator("random");
	awb[0] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[1] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	awb[2] = (randGen->GenerateFloat(0.8f, 1.2f)) * 32768;
	clamp[0] = 16383;

	mvcvWhiteBalanceRGB(inLineR, inLineG, inLineB, awb, outLineC_R, outLineC_G, outLineC_B, clamp, width);	
	whiteBalanceRGB_asm_test(inLineR, inLineG, inLineB, awb, outLineAsm_R, outLineAsm_G, outLineAsm_B, clamp, width);

	CompareArrays(outLineC_R[0], outLineAsm_R[0], width, DELTA);
	CompareArrays(outLineC_G[0], outLineAsm_G[0], width, DELTA);
	CompareArrays(outLineC_B[0], outLineAsm_B[0], width, DELTA);

}
 
TEST_F(whiteBalanceRGBTest, TestRandomInputWidth1920Clamp16383)
{
	width = 1920;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineG = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineB = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	
	outLineC_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_B = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_B = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;

	mvcvWhiteBalanceRGB(inLineR, inLineG, inLineB, awb, outLineC_R, outLineC_G, outLineC_B, clamp, width);	
	whiteBalanceRGB_asm_test(inLineR, inLineG, inLineB, awb, outLineAsm_R, outLineAsm_G, outLineAsm_B, clamp, width);
	RecordProperty("CyclePerPixel", whiteBalanceRGBCycleCount / width);	
	
	CompareArrays(outLineC_R[0], outLineAsm_R[0], width, DELTA);
	CompareArrays(outLineC_G[0], outLineAsm_G[0], width, DELTA);
	CompareArrays(outLineC_B[0], outLineAsm_B[0], width, DELTA);
		
}

TEST_F(whiteBalanceRGBTest, TestRandomInputWidth1920Clamp255)
{
	width = 1920;
	
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineG = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineB = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	
	outLineC_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_B = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_B = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;

	mvcvWhiteBalanceRGB(inLineR, inLineG, inLineB, awb, outLineC_R, outLineC_G, outLineC_B, clamp, width);	
	whiteBalanceRGB_asm_test(inLineR, inLineG, inLineB, awb, outLineAsm_R, outLineAsm_G, outLineAsm_B, clamp, width);
	RecordProperty("CyclePerPixel", whiteBalanceRGBCycleCount / width);	
	
	CompareArrays(outLineC_R[0], outLineAsm_R[0], width, DELTA);
	CompareArrays(outLineC_G[0], outLineAsm_G[0], width, DELTA);
	CompareArrays(outLineC_B[0], outLineAsm_B[0], width, DELTA);
	
}

INSTANTIATE_TEST_CASE_P(RandomInputs, whiteBalanceRGBTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(whiteBalanceRGBTest , TestRandomData)
{
	unsigned short clamp[1];
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	inLineR = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineG = inputGen.GetLinesU16(width, 1, 0, 16383);
	inLineB = inputGen.GetLinesU16(width, 1, 0, 16383);
	
	awb[0] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[1] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;
	awb[2] = randGen->GenerateFloat(0.8f, 1.2f) * 32768;

	outLineC_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineC_B = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_R = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_G = inputGen.GetEmptyLinesU16(width, 1);
	outLineAsm_B = inputGen.GetEmptyLinesU16(width, 1);
	
	clamp[0] = 16383;

	mvcvWhiteBalanceRGB(inLineR, inLineG, inLineB, awb, outLineC_R, outLineC_G, outLineC_B, clamp, width);	
	whiteBalanceRGB_asm_test(inLineR, inLineG, inLineB, awb, outLineAsm_R, outLineAsm_G, outLineAsm_B, clamp, width);
	RecordProperty("CyclePerPixel", whiteBalanceRGBCycleCount / width);	
	
	CompareArrays(outLineC_R[0], outLineAsm_R[0], width, DELTA);
	CompareArrays(outLineC_G[0], outLineAsm_G[0], width, DELTA);
	CompareArrays(outLineC_B[0], outLineAsm_B[0], width, DELTA);
	
} 


	/* for (int i=0; i < width; i++ ){
		if(outLineC_R[0][i] != outLineAsm_R[0][i]) {
			if (((outLineC_R[0][i] - outLineAsm_R[0][i]) > DELTA) || ((outLineAsm_R[0][i] - outLineC_R[0][i]) > DELTA)) {
				printf("outLineC_R[0][%d] = %d, outLineAsm_R[0][%d]= %d\n",i, outLineC_R[0][i], i, outLineAsm_R[0][i]);
				EXPECT_EQ(2, 1);
				}
			}
		if(outLineC_G[0][i] != outLineAsm_G[0][i]) {
			if (((outLineC_G[0][i] - outLineAsm_G[0][i]) > DELTA) || ((outLineAsm_G[0][i] - outLineC_G[0][i]) > DELTA)) {
				printf("outLineC_G[0][%d] = %d, outLineAsm_G[0][%d]= %d\n",i, outLineC_G[0][i], i, outLineAsm_G[0][i]);
				EXPECT_EQ(2, 1); 
				}
			}
		if(outLineC_B[0][i] != outLineAsm_B[0][i]) {
			if (((outLineC_R[0][i] - outLineAsm_R[0][i]) > DELTA) || ((outLineAsm_B[0][i] - outLineC_B[0][i]) > DELTA)) {
				printf("outLineC_B[0][%d] = %d, outLineAsm_B[0][%d]= %d\n",i , outLineC_B[0][i], i, outLineAsm_B[0][i]);
				EXPECT_EQ(2, 1);
				}
			}	
	} */