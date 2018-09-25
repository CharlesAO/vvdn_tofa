//absoluteDiff test
//Asm function prototype:
//    void mvcvAbsoluteDiff_asm(u8** in1, u8** in2, u8** out, u32 width);
//
//Asm test function prototype:
//    void absoluteDiff_asm_test(unsigned char** in1, unsigned char** in2, unsigned char** out, unsigned int width);
//
//C function prototype:
//    void mvcvAbsoluteDiff(u8** in1, u8** in2, u8** out, u32 width);
//
//Parameter description:
// in1     - array of pointers to input lines of the first image
// in2     - array of pointers to input lines of the second image
// out     - array of pointers to output lines
// width   - width of the input line

#include "gtest/gtest.h"
#include "convert_fp16_u8.h"
#include "convert_fp16_u8_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include "half.h"

class Convert_fp16_u8Test : public ::testing::Test {
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



    half* inLine;
    u8* outLineC;
    u8* outLineASM;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(Convert_fp16_u8Test , TestWidth77)
{
    int width = 77; // 64 + 8 + 5
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLineFloat16(width, 0, 255);
	outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
	mvcvConvert_fp16_u8(inLine, outLineC, width);
	RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

	arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth137)
{
    int width = 137; // 64*2 + 8 + 1
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth136)
{
    int width = 136; // 64*2 + 8
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth199)
{
    int width = 199; // 64*3 + 7
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth64)
{
    int width = 64;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth25)
{
    int width = 25; // 8*3 + 1
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth8)
{
    int width = 8;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth1)
{
    int width = 1;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestValuesGreaterThan255)
{
    int width = 77;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 255, 1000);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestValuesLowerThan0)
{
    int width = 76;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, -500, 0);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

//    arrCheck.CompareArrays(outLineASM, outLineC, width);
//    arrCheck.DumpArrays("refInput", "refInput.c", inLine, width);
//    arrCheck.DumpArrays("refOutput", "refOutput.c", outLineC, width);
//    arrCheck.DumpArrays("asmOutput", "asmOutput.c", outLineASM, width);
}

TEST_F(Convert_fp16_u8Test , TestValues0)
{
    int width = 75;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 0);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth1920)
{
    int width = 1920;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_fp16_u8Test , TestWidth1307)
{
    int width = 1307;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLineFloat16(width, 0, 255);
    outLineC = inputGen.GetEmptyLine(width);
    outLineASM = inputGen.GetEmptyLine(width);

    Convert_fp16_u8_asm_test(inLine, outLineASM, width);
    mvcvConvert_fp16_u8(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_fp16_u8CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}
