#include "gtest/gtest.h"
#include "minTest3x3_s16_asm_test.h"
#include "minTest3x3_s16.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"

#include <ctime>
#include <memory>


class minTest3x3_s16 : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        int i;
        minCountOutC = 0;
        minCountOutAsm = 0;
        minCountIn = 0;
        for (i = 0; i<320; i++)
        {
            minLocationsIn[i] = 0;
            minLocationsOutC[i] = 0;
            minLocationsOutAsm[i] = 0;
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

    unsigned int minLocationsIn[320];
    unsigned int minLocationsOutC[320];
    unsigned int minLocationsOutAsm[320];
    unsigned int minCountIn;
    unsigned int minCountOutC;
    unsigned int minCountOutAsm;
  //virtual void TearDown() {}
};

TEST_F(minTest3x3_s16, firstElementmin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 2);
  minCountIn = 1;
  minLocationsIn[0]= 1;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);

}

TEST_F(minTest3x3_s16, lastElementmin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);
  
  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);

}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos1)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[0][317] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos2)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[0][318] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos3)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[0][319] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos4)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[1][317] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos5)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[1][318] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
 
  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos6)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[1][319] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos7)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[2][317] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos8)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[2][318] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, lastElementCheckButNotminPos9)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[2][319] = 1;
  minCountIn = 1;
  minLocationsIn[0]= 318;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, threeElementsCheck)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  inBuffer[0][100] = 1;
  minCountIn = 3;
  minLocationsIn[0]= 99;
  minLocationsIn[1]= 100;
  minLocationsIn[2]= 101;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);
}

TEST_F(minTest3x3_s16, test300elements3notmin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0.5);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0.5);
  cand = (short *)inputGen.GetLineU16(320, 1);
  int i;
  minCountIn = 300;
  for (i = 0; i<300; i++)
  {
      minLocationsIn[i]= i+1;
  }
  inBuffer[0][100]= 1;

  minTest3x3_s16_asm_test(cand,inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand,inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);

}

TEST_F(minTest3x3_s16, testRandom)
{
  inputGen.SelectGenerator("random");
  inBuffer[0] = (short *)inputGen.GetLineU16(320, 0, 255);
  inBuffer[1] = (short *)inputGen.GetLineU16(320, 0, 255);
  inBuffer[2] = (short *)inputGen.GetLineU16(320, 0, 255);

  cand = (short *)inputGen.GetLineU16(320, 0, 255);

  minCountIn = 300;
  for(int i = 0; i < 300; i++)
    minLocationsIn[i] = dataGenerator.GenerateUInt(1, 255);

  minTest3x3_s16_asm_test(cand, inBuffer, minLocationsIn, minLocationsOutAsm, minCountIn, &minCountOutAsm);
  RecordProperty("CycleCount", cycleCount);
  mvcvMinTest3x3_s16(cand, inBuffer, minLocationsIn, minLocationsOutC, minCountIn, &minCountOutC);

  EXPECT_EQ(minCountOutC, minCountOutAsm);
  outputChecker.CompareArrays(&minLocationsOutC[0], &minLocationsOutAsm[0], minCountOutC);

}
