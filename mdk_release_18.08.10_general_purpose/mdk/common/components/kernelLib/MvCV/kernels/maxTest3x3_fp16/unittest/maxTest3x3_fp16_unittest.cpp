#include "gtest/gtest.h"
#include "maxTest3x3_fp16_asm_test.h"
#include "mv_types.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"


#include <ctime>
#include <memory>

class maxTest3x3_fp16 : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        int i;
        maxCountOut = 0;
        maxCountIn = 0;
        for (i = 0; i<320; i++)
        {
            maxLocationsIn[i] = 0;
            maxLocationsOut[i] = 0;
        }
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }



    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    half* inBuffer[3];
    half* cand;

    u32 width;
    unsigned int maxLocationsIn[320];
    unsigned int maxLocationsOut[320];
    unsigned int maxCountIn;
    unsigned int maxCountOut;
  //virtual void TearDown() {}
};

TEST_F(maxTest3x3_fp16, firstElementmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 2);
  maxCountIn = 1;
  maxLocationsIn[0]= 1;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)1, maxCountOut);
  EXPECT_EQ((unsigned int)1, maxLocationsOut[0]);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[1]);
}
TEST_F(maxTest3x3_fp16, lastElementmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);
  
  EXPECT_EQ((unsigned int)1, maxCountOut);
  EXPECT_EQ((unsigned int)318, maxLocationsOut[0]);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[1]);
}

TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos1)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[0][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos2)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[0][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos3)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[0][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos4)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[1][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos5)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[1][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos6)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[1][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos7)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[2][317] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos8)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[2][318] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, lastElementCheckButNotmaxPos9)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[2][319] = 1;
  maxCountIn = 1;
  maxLocationsIn[0]= 318;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}
TEST_F(maxTest3x3_fp16, threeElementsCheck)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  inBuffer[0][100] = 1;
  maxCountIn = 3;
  maxLocationsIn[0]= 99;
  maxLocationsIn[1]= 100;
  maxLocationsIn[2]= 101;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, maxCountOut);
  EXPECT_EQ((unsigned int)0, maxLocationsOut[0]);
}


TEST_F(maxTest3x3_fp16, test300elements3notmax)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, 1);
  int i;
  maxCountIn = 300;
  for (i = 0; i<300; i++)
  {
      maxLocationsIn[i]= i+1;
  }
  inBuffer[0][100]= 1;
  maxTest3x3_fp16_asm_test(cand,inBuffer, 320, maxLocationsIn, maxLocationsOut, maxCountIn, &maxCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)297, maxCountOut);
  EXPECT_EQ((unsigned int)1, maxLocationsOut[0]);
  EXPECT_EQ((unsigned int)98, maxLocationsOut[97]);
  EXPECT_EQ((unsigned int)102, maxLocationsOut[98]);
}
