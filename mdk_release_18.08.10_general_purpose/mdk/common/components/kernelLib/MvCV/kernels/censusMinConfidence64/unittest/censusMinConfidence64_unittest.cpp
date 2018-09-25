
//Parameter description:
// mvcvCensusMin 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <censusMinConfidence64.h>
#include "censusMinConfidence64_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 32

class mvcvCensusMinConfidence64KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *inLine;
    unsigned char *minPosC;
    unsigned char *minPosAsm;
    unsigned char *minValC;
    unsigned char *minValAsm;
    unsigned int width;
    unsigned int disparities;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvCensusMinConfidence64KernelTest, TestWidth16All0)
{
    width = 16;
    disparities = 64;
    inputGen.SelectGenerator("uniform");
    minValC   = inputGen.GetLine(width + PADDING, 5);
    minValAsm = inputGen.GetLine(width + PADDING, 5);
	
    minPosC   = inputGen.GetLine(width + PADDING, 5);
    minPosAsm = inputGen.GetLine(width + PADDING, 5);

	inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLine(((width+ PADDING) * disparities), 0);
    
    mvcvCensusMinConfidence64((unsigned char *)&inLine[16], (unsigned char *)&minPosC[16],(unsigned char *)&minValC[16], width);
    mvcvCensusMinConfidence64_asm_test(inLine , minPosAsm, minValAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64CycleCount / (width ));
    outputChecker.CompareArrays(minValC, minValAsm, width + PADDING);
    outputChecker.CompareArrays(minPosC, minPosAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64KernelTest, TestWidth16All255)
{
    width = 16;
    disparities = 64;
    inputGen.SelectGenerator("uniform");
    minValC   = inputGen.GetLine(width + PADDING, 5);
    minValAsm = inputGen.GetLine(width + PADDING, 5);
	
    minPosC   = inputGen.GetLine(width + PADDING, 5);
    minPosAsm = inputGen.GetLine(width + PADDING, 5);

	inputGen.SelectGenerator("uniform");
    inLine = inputGen.GetLine(((width+ PADDING) * disparities), 255);
    
    mvcvCensusMinConfidence64((unsigned char *)&inLine[16], (unsigned char *)&minPosC[16],(unsigned char *)&minValC[16], width);
    mvcvCensusMinConfidence64_asm_test(inLine , minPosAsm, minValAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64CycleCount / (width ));
    outputChecker.CompareArrays(minValC, minValAsm, width + PADDING);
    outputChecker.CompareArrays(minPosC, minPosAsm, width + PADDING);
}


TEST_F(mvcvCensusMinConfidence64KernelTest, TestMinWidth)
{
    width = 8;
    disparities = 64;
    inputGen.SelectGenerator("uniform");
    minValC   = inputGen.GetLine(width + PADDING, 5);
    minValAsm = inputGen.GetLine(width + PADDING, 5);
	
    minPosC   = inputGen.GetLine(width + PADDING, 5);
    minPosAsm = inputGen.GetLine(width + PADDING, 5);

	inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(((width+ PADDING) * disparities), 0, 255);
    
    mvcvCensusMinConfidence64((unsigned char *)&inLine[16], (unsigned char *)&minPosC[16],(unsigned char *)&minValC[16], width);
    mvcvCensusMinConfidence64_asm_test(inLine , minPosAsm, minValAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64CycleCount / (width ));
    outputChecker.CompareArrays(minValC, minValAsm, width + PADDING);
    outputChecker.CompareArrays(minPosC, minPosAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64KernelTest, TestRandomDataSmallWidth)
{
	width = randGen->GenerateUInt(8, 320);
    disparities = 64;
    inputGen.SelectGenerator("uniform");
    minValC   = inputGen.GetLine(width + PADDING, 5);
    minValAsm = inputGen.GetLine(width + PADDING, 5);
	
    minPosC   = inputGen.GetLine(width + PADDING, 5);
    minPosAsm = inputGen.GetLine(width + PADDING, 5);

	inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(((width+ PADDING) * disparities), 0, 255);
    
    mvcvCensusMinConfidence64((unsigned char *)&inLine[16], (unsigned char *)&minPosC[16],(unsigned char *)&minValC[16], width);
    mvcvCensusMinConfidence64_asm_test(inLine , minPosAsm, minValAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64CycleCount / (width ));
    outputChecker.CompareArrays(minValC, minValAsm, width + PADDING);
    outputChecker.CompareArrays(minPosC, minPosAsm, width + PADDING);
}

TEST_F(mvcvCensusMinConfidence64KernelTest, TestRandomDataBigWidth)
{
	width = randGen->GenerateUInt(320, 640);
    disparities = 64;
    inputGen.SelectGenerator("uniform");
    minValC   = inputGen.GetLine(width + PADDING, 5);
    minValAsm = inputGen.GetLine(width + PADDING, 5);
	
    minPosC   = inputGen.GetLine(width + PADDING, 5);
    minPosAsm = inputGen.GetLine(width + PADDING, 5);

	inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(((width+ PADDING) * disparities), 0, 255);
    
    mvcvCensusMinConfidence64((unsigned char *)&inLine[16], (unsigned char *)&minPosC[16],(unsigned char *)&minValC[16], width);
    mvcvCensusMinConfidence64_asm_test(inLine , minPosAsm, minValAsm, width);
    
    RecordProperty("CyclePerPixel", mvcvCensusMinConfidence64CycleCount / (width ));
    outputChecker.CompareArrays(minValC, minValAsm, width + PADDING);
    outputChecker.CompareArrays(minPosC, minPosAsm, width + PADDING);
}




