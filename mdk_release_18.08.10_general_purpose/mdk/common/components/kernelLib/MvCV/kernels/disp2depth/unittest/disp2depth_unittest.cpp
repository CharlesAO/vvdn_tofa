///

#include "gtest/gtest.h"
#include "disp2depth.h"
#include "disp2depth_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include "half.h"

#define LUT_TABLE_WIDTH 256

class disp2depthTest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	void AlternativeCompare(unsigned int var1, unsigned int var2, u8** out, unsigned int length)
		{
			for(int i=0;i<(int)length-2;i+=2)
			{
				EXPECT_EQ(var1, out[0][i]);
			}
			for(int i=1;i<(int)length-2;i+=2)
			{
				EXPECT_EQ(var2, out[0][i]);
			}
		}



    u8* inLine;
    half* lut_table;
    half* outLineC;
    half* outLineASM;
    bool  flip_disp2depth;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(disp2depthTest , TestWidth16_1)
{
    int width = 16; // 64 + 8 + 5
    flip_disp2depth = 1;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    lut_table = inputGen.GetLineFloat16(LUT_TABLE_WIDTH, 0, 1000);

    inputGen.SelectGenerator("uniform");
    outLineC = inputGen.GetLineFloat16(width*2, 5);
    outLineASM = inputGen.GetLineFloat16(width*2, 5);

    disp2depth_asm_test(inLine, outLineASM, lut_table, flip_disp2depth, width);
    mvcvDisp2depth(inLine, &outLineC[width],  lut_table, flip_disp2depth, width);
    RecordProperty("CycleCount", disp2depthCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width * 2);
}

TEST_F(disp2depthTest , TestWidthRandom_1)
{
    int width;
    width = randGen->GenerateUInt(1280, 1921, 16);
    flip_disp2depth = 1;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    lut_table = inputGen.GetLineFloat16(LUT_TABLE_WIDTH, 0, 1000);

    inputGen.SelectGenerator("uniform");
    outLineC = inputGen.GetLineFloat16(width*2, 5);
    outLineASM = inputGen.GetLineFloat16(width*2, 5);

    disp2depth_asm_test(inLine, outLineASM, lut_table, flip_disp2depth, width);
    mvcvDisp2depth(inLine, &outLineC[width],  lut_table, flip_disp2depth, width);
    RecordProperty("CycleCount", disp2depthCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width * 2);
}

TEST_F(disp2depthTest , TestWidth16_0)
{
    int width = 16; // 64 + 8 + 5
    flip_disp2depth = 0;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    lut_table = inputGen.GetLineFloat16(LUT_TABLE_WIDTH, 0, 1000);

    inputGen.SelectGenerator("uniform");
    outLineC = inputGen.GetLineFloat16(width*2, 5);
    outLineASM = inputGen.GetLineFloat16(width*2, 5);

    disp2depth_asm_test(inLine, outLineASM, lut_table, flip_disp2depth, width);
    mvcvDisp2depth(inLine, &outLineC[width],  lut_table, flip_disp2depth, width);
    RecordProperty("CycleCount", disp2depthCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width * 2);
}

TEST_F(disp2depthTest , TestWidthRandom_0)
{
    int width;
    width = randGen->GenerateUInt(1280, 1921, 16);
    flip_disp2depth = 0;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    lut_table = inputGen.GetLineFloat16(LUT_TABLE_WIDTH, 0, 1000);

    inputGen.SelectGenerator("uniform");
    outLineC = inputGen.GetLineFloat16(width*2, 5);
    outLineASM = inputGen.GetLineFloat16(width*2, 5);

    disp2depth_asm_test(inLine, outLineASM, lut_table, flip_disp2depth, width);
    mvcvDisp2depth(inLine, &outLineC[width],  lut_table, flip_disp2depth, width);
    RecordProperty("CycleCount", disp2depthCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width * 2);
}

