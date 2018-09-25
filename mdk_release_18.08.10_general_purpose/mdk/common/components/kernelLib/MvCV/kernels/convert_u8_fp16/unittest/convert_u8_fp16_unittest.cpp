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
#include "convert_u8_fp16.h"
#include "convert_u8_fp16_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include "half.h"

class Convert_u8_fp16Test : public ::testing::Test {
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
    half* outLineC;
    half* outLineASM;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(Convert_u8_fp16Test , TestWidth77)
{
    int width = 77; // 64 + 8 + 5
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLine(width, 0, 255);
	outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
	mvcvConvert_u8_fp16(inLine, outLineC, width);
	RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

	arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth137)
{
    int width = 137; // 64*2 + 8 + 1
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth136)
{
    int width = 136; // 64*2 + 8
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth23)
{
    int width = 23; // 8*2 + 7
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth199)
{
    int width = 199; // 64*3 + 7
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth64)
{
    int width = 64;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth25)
{
    int width = 25; // 8*3 + 1
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth8)
{
    int width = 8;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth1)
{
    int width = 1;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestValues0)
{
    int width = 75;
    int i;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetEmptyLine(width);

    for(i = 0; i < width; i++)
        inLine[i] = 0;

    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth1920)
{
    int width = 1920;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
    arrCheck.CompareArrays(outLineASM, outLineC, width);
    //arrCheck.DumpArrays("refInput", "refInput.c", inLine, width);
    //arrCheck.DumpArrays("refOutput", "refOutput.c", outLineC, width);
    //arrCheck.DumpArrays("asmOutput", "asmOutput.c", outLineASM, width);
}

TEST_F(Convert_u8_fp16Test , TestWidth1307)
{
    int width = 1307;
    inputGen.SelectGenerator("random");
    inLine = inputGen.GetLine(width, 0, 255);
    outLineC = inputGen.GetEmptyLineFp16(width);
    outLineASM = inputGen.GetEmptyLineFp16(width);

    Convert_u8_fp16_asm_test(inLine, outLineASM, width);
    mvcvConvert_u8_fp16(inLine, outLineC, width);
    RecordProperty("CycleCount", Convert_u8_fp16CycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, width);
}

