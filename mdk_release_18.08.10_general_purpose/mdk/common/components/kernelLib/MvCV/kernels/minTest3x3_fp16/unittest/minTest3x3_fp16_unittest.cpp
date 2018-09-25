#include "gtest/gtest.h"
#include "mv_types.h"

#include "minTest3x3_fp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "FunctionInfo.h"
#include "ArrayChecker.h"


#include <ctime>
#include <memory>

class localMaxMin3x3Test : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        int i;
        minCountOut = 0;
        minCountIn = 0;
        for (i = 0; i<320; i++)
        {
            minLocationsIn[i] = 0;
            minLocationsOut[i] = 0;
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
    unsigned int minLocationsIn[320];
    unsigned int minLocationsOut[320];
    unsigned int minCountIn;
    unsigned int minCountOut;
  //virtual void TearDown() {}
};

TEST_F(localMaxMin3x3Test, firstElementMin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  minCountIn = 1;
  minLocationsIn[0]= 1;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)1, minCountOut);
  EXPECT_EQ((unsigned int)1, minLocationsOut[0]);
  EXPECT_EQ((unsigned int)0, minLocationsOut[1]);
}
TEST_F(localMaxMin3x3Test, lastElementMin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);
  
  EXPECT_EQ((unsigned int)1, minCountOut);
  EXPECT_EQ((unsigned int)318, minLocationsOut[0]);
  EXPECT_EQ((unsigned int)0, minLocationsOut[1]);
}

TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos1)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[0][317] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos2)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[0][318] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos3)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[0][319] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos4)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[1][317] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos5)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[1][318] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos6)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[1][319] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos7)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[2][317] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos8)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[2][318] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, lastElementCheckButNotMinPos9)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[2][319] = -1;
  minCountIn = 1;
  minLocationsIn[0]= 318;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}
TEST_F(localMaxMin3x3Test, threeElementsCheck)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  inBuffer[0][100] = -1;
  minCountIn = 3;
  minLocationsIn[0]= 99;
  minLocationsIn[1]= 100;
  minLocationsIn[2]= 101;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)0, minCountOut);
  EXPECT_EQ((unsigned int)0, minLocationsOut[0]);
}


TEST_F(localMaxMin3x3Test, test300elements3notMin)
{
  inputGen.SelectGenerator("uniform");
  inBuffer[0] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[1] = inputGen.GetLineFloat16(320, 0.5);
  inBuffer[2] = inputGen.GetLineFloat16(320, 0.5);
  cand = inputGen.GetLineFloat16(320, -1);
  int i;
  minCountIn = 300;
  for (i = 0; i<300; i++)
  {
      minLocationsIn[i]= i+1;
  }
  inBuffer[0][100]= -1;
  minTest3x3_fp16_asm_test(cand,inBuffer, 320, minLocationsIn, minLocationsOut, minCountIn, &minCountOut);
  RecordProperty("CycleCount", cycleCount);

  EXPECT_EQ((unsigned int)297, minCountOut);
  EXPECT_EQ((unsigned int)1, minLocationsOut[0]);
  EXPECT_EQ((unsigned int)98, minLocationsOut[97]);
  EXPECT_EQ((unsigned int)102, minLocationsOut[98]);
}
