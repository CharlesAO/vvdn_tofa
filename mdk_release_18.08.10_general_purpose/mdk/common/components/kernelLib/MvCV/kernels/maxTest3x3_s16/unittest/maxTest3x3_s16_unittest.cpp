#include "gtest/gtest.h"
#include "maxTest3x3_s16_asm_test.h"
#include "maxTest3x3_s16.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"

#include <ctime>
#include <memory>


class maxTest3x3_s16 : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        int i;
        maxCountOutC = 0;
        maxCountOutAsm = 0;
        maxCountIn = 0;
        for (i = 0; i<320; i++)
        {
            maxLocationsIn[i] = 0;
            maxLocationsOutC[i] = 0;
            maxLocationsOutAsm[i] = 0;
        }
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }


    InputGenerator inputGen;
    RandomGenerator dataGenerator;
    ArrayChecker outputChecker;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    s16* inBuffer[3];
    s16* cand;

    unsigned int maxLocationsIn[320];
    unsigned int maxLocationsOutC[320];
    unsigned int maxLocationsOutAsm[320];
    unsigned int maxCountIn;
    unsigned int maxCountOutC;
    unsigned int maxCountOutAsm;
  //virtual void TearDown() {}
};

TEST_F(maxTest3x3_s16, firstElementmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 2);
  maxCountIn = 1;
  maxLocationsIn[0]= 1;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);

}

TEST_F(maxTest3x3_s16, lastElementmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);
  
  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);

}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos1)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[0][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos2)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[0][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos3)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[0][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos4)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[1][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos5)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[1][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
 
  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos6)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[1][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos7)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[2][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos8)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[2][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, lastElementCheckButNotmaxPos9)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[2][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, threeElementsCheck)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  inBuffer[0][100] = 1;
  maxCountIn = 3;
  maxLocationsIn[0]= 99;
  maxLocationsIn[1]= 100;
  maxLocationsIn[2]= 101;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, test300elements3notmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0.5);
  cand = (short*)inputGen.GetLineU16(320, 1);
  int i;
  maxCountIn = 300;
  for (i = 0; i<300; i++)
  {
      maxLocationsIn[i]= i+1;
  }
  inBuffer[0][100]= 1;

  maxTest3x3_s16_asm_test(cand,inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);

  //inBuffer[0] = inBuffer[0] + PADDING/2;
  //inBuffer[1] = inBuffer[1] + PADDING/2;
  //inBuffer[2] = inBuffer[2] + PADDING/2;
  mvcvMaxTest3x3_s16(cand,inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);
}

TEST_F(maxTest3x3_s16, testRandom)
{
  inputGen.SelectGenerator("random");
  inBuffer[0] = (short*)inputGen.GetLineU16(320, 0, 255);
  inBuffer[1] = (short*)inputGen.GetLineU16(320, 0, 255);
  inBuffer[2] = (short*)inputGen.GetLineU16(320, 0, 255);

  cand = (short*)inputGen.GetLineU16(320, 0, 255);

  maxCountIn = 300;
  for(int i = 0; i < 300; i++)
    maxLocationsIn[i] = dataGenerator.GenerateUInt(1, 255);

  maxTest3x3_s16_asm_test(cand, inBuffer, maxLocationsIn, maxLocationsOutAsm, maxCountIn, &maxCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMaxTest3x3_s16(cand, inBuffer, maxLocationsIn, maxLocationsOutC, maxCountIn, &maxCountOutC);

  EXPECT_EQ(maxCountOutC, maxCountOutAsm);
  outputChecker.CompareArrays(&maxLocationsOutC[0], &maxLocationsOutAsm[0], maxCountOutC);

}