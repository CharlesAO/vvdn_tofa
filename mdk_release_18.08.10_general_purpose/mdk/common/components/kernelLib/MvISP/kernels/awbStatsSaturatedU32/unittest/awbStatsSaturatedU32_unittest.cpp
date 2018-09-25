//genLumaFp16 test
//
// Test description:
//   generate luma from rgb input image
//

#include "gtest/gtest.h"
#include "awbStatsSaturatedU32_asm_test.h"
#include "awbStatsSaturatedU32.h"
#include "awbStatsSaturatedU32_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

unsigned char DELTA = 0;//accepted tolerance between C and ASM results
unsigned int OUT_BUF_CHECK = 16;
class awbStatsSaturatedU32Test : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	
	unsigned short *in;
	unsigned int *outLineC;
	unsigned int *outLineAsm;

	unsigned int width;
    unsigned int nPaxelX; 
    unsigned int gapPaxelX; 
    unsigned int widthPaxelX;
    unsigned int satPaxelX;    
    
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};



TEST_F(awbStatsSaturatedU32Test, TestFullRandom)
{
    inputGen.SelectGenerator("random");
    width = 4208;
    nPaxelX  = 20; 
    gapPaxelX = 8; 
    widthPaxelX = 6;
    satPaxelX = 200;      
   
	in = inputGen.GetLineU16(width, 0, 1023);
 
	outLineC = (unsigned int *)inputGen.GetLine((nPaxelX*2+OUT_BUF_CHECK) * 4, 0, 250, 0);
	outLineAsm = (unsigned int *)inputGen.GetLine((nPaxelX*2+OUT_BUF_CHECK) * 4, 0, 250, 0);
    memcpy(outLineAsm, outLineC, (nPaxelX*2+OUT_BUF_CHECK) * 4);

	mvispStatsAWBSatPixelsU32(outLineC, in, nPaxelX, gapPaxelX, widthPaxelX, satPaxelX);
    awbStatsSaturatedU32_asm_test(outLineAsm, in, nPaxelX, gapPaxelX, widthPaxelX, satPaxelX, width);
	
	CompareArrays(outLineC, outLineAsm, (nPaxelX*2+OUT_BUF_CHECK));
}

//------------ parameterized tests ----------------------------------------

 INSTANTIATE_TEST_CASE_P(UniformInputs, awbStatsSaturatedU32Test,
		Values((unsigned int)2, (unsigned int)4, (unsigned int)6, (unsigned int)8, (unsigned int)12, (unsigned int)16, (unsigned int)24, (unsigned int)26, (unsigned int)28, (unsigned int)32, (unsigned int)48, (unsigned int)64); //8
);

TEST_P(awbStatsSaturatedU32Test, TestRandomInputLinesAllWidth)
{
    inputGen.SelectGenerator("random");
    width = 4208;
    nPaxelX  = 64; 
    widthPaxelX = GetParam();
    gapPaxelX = randGen->GenerateUInt(widthPaxelX-1, (((unsigned int)(width/nPaxelX))&(~1)), 2);
    satPaxelX = randGen->GenerateUInt(20, 900, 1);     
//std::printf("\n width: %d -- nPaxelX: %d -- widthPaxelX: %d -- gapPaxelX: %d -- satPaxelX %d  \n",width, nPaxelX, widthPaxelX, gapPaxelX, satPaxelX);
    
	in = inputGen.GetLineU16(width, 0, 1023);
 
	outLineC = (unsigned int *)inputGen.GetLine((nPaxelX*2+OUT_BUF_CHECK) * 4, 0, 250, 0);
	outLineAsm = (unsigned int *)inputGen.GetLine((nPaxelX*2+OUT_BUF_CHECK) * 4, 0, 250, 0);
    memcpy(outLineAsm, outLineC, (nPaxelX*2+OUT_BUF_CHECK) * 4);

//std::printf("\n LineIN: %d, %d, %d, %d, %d, %d, \n",  outLineAsm[0], outLineAsm[1], outLineAsm[2], outLineAsm[3], outLineAsm[4], outLineAsm[5]);    
	mvispStatsAWBSatPixelsU32(outLineC, in, nPaxelX, gapPaxelX, widthPaxelX, satPaxelX);
    awbStatsSaturatedU32_asm_test(outLineAsm, in, nPaxelX, gapPaxelX, widthPaxelX, satPaxelX, width);
	
	CompareArrays(outLineC, outLineAsm, (nPaxelX*2+OUT_BUF_CHECK) );
//std::printf("\n LineC: %d, %d, %d, %d, %d, %d, \n",  outLineAsm[0], outLineAsm[1], outLineAsm[2], outLineAsm[3], outLineAsm[4], outLineAsm[5]);   
//std::printf("\n LineC: %d, %d, %d, %d, %d, %d, \n",  outLineC[0], outLineC[1], outLineC[2], outLineC[3], outLineC[4], outLineC[5]);   
}
