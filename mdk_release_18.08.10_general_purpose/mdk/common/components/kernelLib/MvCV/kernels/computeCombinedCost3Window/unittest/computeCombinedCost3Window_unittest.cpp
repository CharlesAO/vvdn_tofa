
//Parameter description:
// mvcvCensusMin 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <computeCombinedCost3Window.h>
#include "computeCombinedCost3Window_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DISPARITIES 5
#define DELTA 1

class mvcvComputeCombinedCost3WindowKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *disparityCostC;
    unsigned char *disparityCostAsm;
    unsigned char *adCost;

    unsigned int alpha;
    unsigned int beta;
    unsigned int width;
    unsigned int normFactor;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};


TEST_F(mvcvComputeCombinedCost3WindowKernelTest, TestAll0)
{
    width = 320;    
    inputGen.SelectGenerator("random");
    alpha  = randGen->GenerateUInt(1, 5, 1);
    beta  = randGen->GenerateUInt(1, 5, 1);
    normFactor = randGen->GenerateUInt(1, 5, 1);
    
    inputGen.SelectGenerator("uniform");
    disparityCostC = inputGen.GetLine((width * DISPARITIES), 0);
    disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 0);
    memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
    adCost = inputGen.GetLine((width * DISPARITIES), 0);

    
    mvcvComputeCombinedCost3Window(disparityCostC, adCost, alpha, beta, width, normFactor);
    mvcvComputeCombinedCost3Window_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
    RecordProperty("CyclePerPixel", mvcvComputeCombinedCost3WindowCycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

TEST_F(mvcvComputeCombinedCost3WindowKernelTest, TestAllMAX)
{
    width = 1280;
    inputGen.SelectGenerator("random");
    alpha  = randGen->GenerateUInt(1, 5, 1);
    beta  = randGen->GenerateUInt(1, 5, 1);
    normFactor = randGen->GenerateUInt(1, 5, 1);
    
    inputGen.SelectGenerator("uniform");
    disparityCostC = inputGen.GetLine((width * DISPARITIES), 255);
    disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 255);
    memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
    adCost = inputGen.GetLine((width * DISPARITIES), 255);

    mvcvComputeCombinedCost3Window(disparityCostC, adCost, alpha, beta, width, normFactor);
    mvcvComputeCombinedCost3Window_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
    RecordProperty("CyclePerPixel", mvcvComputeCombinedCost3WindowCycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

TEST_F(mvcvComputeCombinedCost3WindowKernelTest, TestRandomDdataRandomWidth)
{
    width = randGen->GenerateUInt(8, 1920, 8); 
    inputGen.SelectGenerator("random");
    alpha  = randGen->GenerateUInt(1, 5, 1);
    beta  = randGen->GenerateUInt(1, 5, 1);
    normFactor = randGen->GenerateUInt(1, 5, 1);
    
    inputGen.SelectGenerator("random");
    disparityCostC = inputGen.GetLine((width * DISPARITIES), 0, 255);
    disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 0);
    memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
    adCost = inputGen.GetLine((width * DISPARITIES), 0, 255);
    
    mvcvComputeCombinedCost3Window(disparityCostC, adCost, alpha, beta, width, normFactor);
    mvcvComputeCombinedCost3Window_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
    RecordProperty("CyclePerPixel", mvcvComputeCombinedCost3WindowCycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}

TEST_F(mvcvComputeCombinedCost3WindowKernelTest, TestRandomDataRandomWidth2)
{
    width = randGen->GenerateUInt(960, 1920, 8);
    inputGen.SelectGenerator("random");
    alpha  = randGen->GenerateUInt(1, 5, 1);
    beta  = randGen->GenerateUInt(1, 5, 1);
    normFactor = randGen->GenerateUInt(1, 5, 1);
    
    inputGen.SelectGenerator("random");
    disparityCostC = inputGen.GetLine((width * DISPARITIES), 0, 255);
    disparityCostAsm = inputGen.GetLine((width * DISPARITIES), 0);
    memcpy ( disparityCostAsm, disparityCostC, width * DISPARITIES);
    adCost = inputGen.GetLine((width * DISPARITIES), 0, 255);
    
    mvcvComputeCombinedCost3Window(disparityCostC, adCost, alpha, beta, width, normFactor);
    mvcvComputeCombinedCost3Window_asm_test(disparityCostAsm, adCost, alpha, beta, width, normFactor);
    RecordProperty("CyclePerPixel", mvcvComputeCombinedCost3WindowCycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(disparityCostC, disparityCostAsm, width * DISPARITIES, DELTA);
}
