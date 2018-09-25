//Bilateral5x5 kernel test

//Asm function prototype:
//	 	void bilateral5x5_asm(u16* input, u16* sigma, u16* output, u32 width);

//Asm test function prototype:
//    	void bilateral5x5_asm_test(unsigned short* input, unsigned short* sigma,unsigned short* output, unsigned int width);

//C function prototype:
//		void bilateral5x5(u16* input, u16* sigma, u16* output, u32 width);

//width		- width of input line
//input		- pointer to input lines
//sigma		- pointer to sigma values
//output	- pointer to output line

#include "gtest/gtest.h"
#include "bilateral5x5.h"
#include "bilateral5x5_asm_test.h"
#include "bilateral5x5_unittest.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


#define KERNEL_SIZE 5
const unsigned short DELTA = 0x000f;

class bilateral5x5Test : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned short* inLines;
    unsigned short* inSigma;
    unsigned short* outLineC;
    unsigned short* outLineAsm;
    unsigned int width;
    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(bilateral5x5Test, TestMaxWidthUniformInput)
{
    width = 1920;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLineU16(width * KERNEL_SIZE, 777);
    inSigma = inputGen.GetLineU16(width, 0);

    outLineC = inputGen.GetEmptyLineU16(width);
    outLineAsm = inputGen.GetEmptyLineU16(width);

    mvcvBilateral5x5(inLines, inSigma, width, outLineC);
    bilateral5x5_asm_test(inLines, inSigma, width, outLineAsm);
    RecordProperty("CyclePerPixel", bilateral5x5CycleCount / width);

    CompareArrays(outLineC, outLineAsm, width, DELTA);
}

TEST_F(bilateral5x5Test, TestRandomInput10bRandomWidth)
{
    width = randGen->GenerateUInt(8, 1921, 8);
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLineU16(width * KERNEL_SIZE, 0, 0x03ff);
    inSigma = inputGen.GetLineU16(width, 0, 0x00ff);


    outLineC = inputGen.GetEmptyLineU16(width);
    outLineAsm = inputGen.GetEmptyLineU16(width);

    mvcvBilateral5x5(&inLines[2*width], inSigma, width, outLineC);
    bilateral5x5_asm_test(inLines, inSigma, width, outLineAsm);
    RecordProperty("CyclePerPixel", bilateral5x5CycleCount / width);

    CompareArrays(outLineC, outLineAsm, width, DELTA);
}


TEST_F(bilateral5x5Test, TestRandomInput10bMaxWidth)
{
    width = 1920;
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLineU16(width * KERNEL_SIZE, 0, 0x03ff);
    inSigma = inputGen.GetLineU16(width, 0, 0x00ff);


    outLineC = inputGen.GetEmptyLineU16(width);
    outLineAsm = inputGen.GetEmptyLineU16(width);

    mvcvBilateral5x5(&inLines[2*width], inSigma, width, outLineC);
    bilateral5x5_asm_test(inLines, inSigma, width, outLineAsm);
    RecordProperty("CyclePerPixel", bilateral5x5CycleCount / width);

    CompareArrays(outLineC, outLineAsm, width, DELTA);
}
