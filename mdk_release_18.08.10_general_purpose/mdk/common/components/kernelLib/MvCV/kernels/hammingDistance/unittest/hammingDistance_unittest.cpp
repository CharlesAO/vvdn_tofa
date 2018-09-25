//mvcvHammingDistance test
//Asm function prototype:
//    void mvcvHammingDistance_asm(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances);
//
//Asm test function prototype:
//    void mvcvHammingDistance_asm_test(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances);
//
//C function prototype:
//    void mvcvHammingDistance(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances);
//
//Parameter description:
// d1              - pointer to first descriptor
// d2              - pointer to second descriptor
// array_size      - size of array used to compute the distances
// descriptor_size - size of the descriptor (32, 64 or 128 bytes)
// out             - pointer to distances

#include "gtest/gtest.h"
///
#include "hammingDistance_asm_test.h"
#include "hammingDistance.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"


using ::testing::TestWithParam;
using ::testing::Values;

class mvcvHammingDistanceTest : public ::testing::TestWithParam< unsigned int > {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

    u8  *inLine1;
    u8  *inLine2;
    u16 *outLineC;
    u16 *outLineASM;
    int array_size;
    int descriptor_size;
    int minus_bytes;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

   virtual void TearDown() {}
};

/* Test ASM with all input 0 */
TEST_F(mvcvHammingDistanceTest, TestAllValues0_32)
{
    descriptor_size = 32;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 0);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
    //arrCheck.DumpArrays("refInput1", "refInput1.c", inLine1, descriptor_size);
    //arrCheck.DumpArrays("refInput2", "refInput2.c", inLine2, descriptor_size * array_size);
    //arrCheck.DumpArrays("refCOutput", "refCOutput.c", outLineC, array_size);
    //arrCheck.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineASM, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestAllValues0_64)
{
    descriptor_size = 64;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 0);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestAllValues0_128)
{
    descriptor_size = 128;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 0);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

/* Test ASM with maximum distance */
TEST_F(mvcvHammingDistanceTest, TestMaxDistances_32)
{
    descriptor_size = 32;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
    //arrCheck.DumpArrays("refInput1", "refInput1.c", inLine1, descriptor_size);
    //arrCheck.DumpArrays("refInput2", "refInput2.c", inLine2, descriptor_size * array_size);
    //arrCheck.DumpArrays("refCOutput", "refCOutput.c", outLineC, array_size);
    //arrCheck.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineASM, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestMaxDistances_64)
{
    descriptor_size = 64;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestMaxDistances_128)
{
    descriptor_size = 128;
    array_size      = 512;
    inputGen.SelectGenerator("uniform");
    inLine1 = inputGen.GetLine(descriptor_size, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

/* Test with random input */
TEST_F(mvcvHammingDistanceTest, TestRandomInput_32)
{
    descriptor_size = 32;
    array_size      = 512;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
	inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

	outLineC = inputGen.GetEmptyLineU16(array_size);
	outLineASM = inputGen.GetEmptyLineU16(array_size);
	mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
	RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

	arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestRandomInput_64)
{
    descriptor_size = 64;
    array_size      = 512;
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_F(mvcvHammingDistanceTest, TestRandomInput_128)
{
    descriptor_size = 128;
    array_size      = 512;
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

/* Test with array_size not divisible with descriptor_size */
INSTANTIATE_TEST_CASE_P(UniformInputs, mvcvHammingDistanceTest,
        Values((unsigned int)1, (unsigned int)2, (unsigned int)3, (unsigned int)4, (unsigned int)5, (unsigned int)6, (unsigned int)7);
);
TEST_P(mvcvHammingDistanceTest, TestRandomArraySize_32)
{
    minus_bytes = GetParam();
    descriptor_size = 32;
    array_size      = 512 - minus_bytes;
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_P(mvcvHammingDistanceTest, TestRandomArraySize_64)
{
    minus_bytes = GetParam();
    descriptor_size = 64;
    array_size      = 512 - minus_bytes;
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

TEST_P(mvcvHammingDistanceTest, TestRandomArraySize_128)
{
    minus_bytes = GetParam();
    descriptor_size = 128;
    array_size      = 512 - minus_bytes;
    inputGen.SelectGenerator("random");
    inLine1 = inputGen.GetLine(descriptor_size, 0, 255);
    inLine2 = inputGen.GetLine(descriptor_size * array_size, 0, 255);

    outLineC = inputGen.GetEmptyLineU16(array_size);
    outLineASM = inputGen.GetEmptyLineU16(array_size);
    mvcvHammingDistance(inLine1, inLine2, array_size, descriptor_size, outLineC);
    mvcvHammingDistance_asm_test(inLine1, inLine2, array_size, descriptor_size, outLineASM);
    RecordProperty("CycleCount", mvcvHammingDistanceCycleCount);

    arrCheck.CompareArrays(outLineASM, outLineC, array_size);
}

