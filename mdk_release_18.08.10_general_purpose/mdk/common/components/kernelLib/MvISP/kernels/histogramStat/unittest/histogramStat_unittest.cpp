//HistogramStatTest
//Asm function prototype:
//   void mvcvHistogramStat_asm(u8 *intputR, u8 *inputG, u8 *inputB, u32 *histR, u8 *histG, u32 *histB, u32 width, u32 step);
//
//Asm test function prototype:
//   void histogramStat_asm_test(unsigned char *intputR, unsigned char *inputG, unsigned char *inputB, unsigned int *histR, unsigned int *histG, unsigned int *histB, unsigned int width, unsigned int step);
//
//C function prototype:
//   void mvcvHistogramStat(u8 *intputR, u8 *inputG, u8 *inputB, u32 *histR, u32 *histG, u32 *histB, u32 width, u32 step);
//
//Parameter description:
// intputR     	- pointer to input R line
// intputG  	- pointer to input G line
// intputB 		- pointer to input B line
// histR      	- pointer to R histogram
// histG      	- pointer to G histogram
// histB      	- pointer to B histogram
// width     	- width of input line
// step			- step*8 will be the number of skipped pixeles

#include "gtest/gtest.h"
#include <histogramStat.h>
#include "histogramStat_asm_test.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define HIST_WIDTH 64

using ::testing::TestWithParam;
using ::testing::Values;

class HistogramStatTest :  public ::testing::TestWithParam< unsigned int >  {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned int width;
    InputGenerator inputGen;
    RandomGenerator ranGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    u8 *inLineR;
    u8 *inLineG;
    u8 *inLineB;
	
    u32 *histCR;
	u32 *histCG;
    u32 *histCB;
	
	u32 *histAsmR;
    u32 *histAsmG;
    u32 *histAsmB;
   
    ArrayChecker outputChecker;
    
    virtual void TearDown()
    {
    }
};



TEST_F(HistogramStatTest, TestRandomInputHist0MaxWidthStep0)
{
    inputGen.SelectGenerator("random");
    width = 1920;
    inLineR = inputGen.GetLine(width, 0, 255);
    inLineG = inputGen.GetLine(width, 0, 255);
    inLineB = inputGen.GetLine(width, 0, 255);
	
	inputGen.SelectGenerator("uniform");
    histCR = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histCG = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histCB = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
	histAsmR = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histAsmG = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histAsmB = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
	
	mvispHistogramStat(inLineR, inLineG, inLineB, histCR, histCG, histCB, width, 0);
    histogramStat_asm_test(inLineR, inLineG, inLineB, histAsmR, histAsmG, histAsmB, width, 0);
	
    RecordProperty("CycleCount", histogramStatCycleCount);

    outputChecker.CompareArrays(histCR, histAsmR, HIST_WIDTH);	
    outputChecker.CompareArrays(histCG, histAsmG, HIST_WIDTH);
    outputChecker.CompareArrays(histCB, histAsmB, HIST_WIDTH);
}
/*
//---------------- parameterized tests -------------------------
INSTANTIATE_TEST_CASE_P(RandomInputs, HistogramStatTest,
		Values(1, 2, 3, 4, 7, 12, 25);
);

TEST_P(HistogramStatTest, TestRandom)
{

	int param = GetParam();
	width = 1920;
    inputGen.SelectGenerator("random");    
    inLineR = inputGen.GetLine(width, 0, 255);
    inLineG = inputGen.GetLine(width, 0, 255);
    inLineB = inputGen.GetLine(width, 0, 255);
	
	inputGen.SelectGenerator("uniform");
    histCR = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histCG = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histCB = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
	histAsmR = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histAsmG = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
    histAsmB = (u32*)inputGen.GetLineFloat(HIST_WIDTH, 0);
	
	for(int i = 0; i < HIST_WIDTH; i++)
	{
		histAsmR[i] = i + param * 2;
		histCR[i] = histAsmR[i];
		histAsmG[i] = 7*i + param;
		histCG[i] = histAsmG[i];
		histAsmB[i] = 3*i + param / 2;
		histCB[i] = histAsmB[i];
	}
	
	mvispHistogramStat(inLineR, inLineG, inLineB, histCR, histCG, histCB, width, param);
    histogramStat_asm_test(inLineR, inLineG, inLineB, histAsmR, histAsmG, histAsmB, width, param);
	
    RecordProperty("CycleCount", histogramStatCycleCount);

    outputChecker.CompareArrays(histCR, histAsmR, HIST_WIDTH);	
    outputChecker.CompareArrays(histCG, histAsmG, HIST_WIDTH);
    outputChecker.CompareArrays(histCB, histAsmB, HIST_WIDTH);
}
*/