///This kernel does bilinear interpolation when there is a pattern 
///between bilinear factors
///if the horizontal scalefactor(srcWidth/destWidth)
///has one decimal point precision (for example: 1.4, 555.5, 0.1, 0.4) then
///the 4 factors that are used in calculation will repeat at least after every
///10 iteration, so we can create a lookup table which has the size of 10x10x4*2bytes
///the acces pattern will also respect the equidistant locality
///principle so every 10th pixel's coordinate will differ by a constant
///which is 10 * scaleHor
///bilinear interpolation is applied on 2 lines
///@param[in]   line1 - input elements, 8 bits unsigned chars
///@param[in]   line2 - input elements, 8 bits unsigned chars
///@param[out]  output     - Output line, 8 bits unsigned chars
///@param[in]   precalcPos - the first 10 pixels' adress (these with accesPatternStep)
///                     defines the acces pattern. 10x2 bytes
///@param[in]   lut - lookup table that holds the precalculated factors. 10x10x4x2bytes
///@param[in]   stopW - the pixel's number until we apply bilinear interpolation
///             should be higher than 20
///@param[in]   accesPatternStep - the acces pattern is defined by the horizontal scale
///             which is after every 10th iteration (at least) differs by 10 * scaleHor

#include "gtest/gtest.h"
#include "bilinearInterpolation.h"
#include "bilinearInterpolation_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#include <stdio.h>



class bilinearInterpolationTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

    u8 *line1;
    u8 *line2;
    u8 *dstC;
    u8 *dstAsm;
    u16 precalcPos[10];
    u16 lut[40];
    int stopW;
    int accesPatternStep;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


void init(int srcWidth, int destWidth, int *stopW, int *accesPatternStep,
        int fY0, u16 *lut, u16 *precalcPos)
{

    float scaleHor = (float)srcWidth / destWidth;

    *stopW =
            (scaleHor > 1) ?
                (destWidth - 1) :
                ((((float) (srcWidth - 1) / scaleHor)) + 1 - 0.5f * scaleHor);

    *accesPatternStep = 10 * scaleHor;
    float accX = 0.5f * scaleHor;

    int i, k;
    for (i = k = 0; i < 10; i++, accX = (i + .5f) * scaleHor, k += 4)
    {
        int FX0 = (accX - (int) accX) * 255.f + 0.5f;
        int FX1 = 255 - FX0;

        lut[k+2] = (fY0 * FX1) >> 8;
        lut[k+3] = (fY0 * FX0) >> 8;
        lut[k]   = (FX1 - lut[k+2]);
        lut[k+1] = (FX0 - lut[k+3]);    

        precalcPos[i] = accX;
    }

}

TEST_F(bilinearInterpolationTest, TestWin_0)
{

    init(80, 200, &stopW, &accesPatternStep, 20, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}


TEST_F(bilinearInterpolationTest, TestWin_1)
{
    init(500, 2300, &stopW, &accesPatternStep, 56, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}


TEST_F(bilinearInterpolationTest, TestWin_2)
{
    init(5000, 1000, &stopW, &accesPatternStep, 10, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}


TEST_F(bilinearInterpolationTest, TestWin_3)
{
    init(1920, 1280, &stopW, &accesPatternStep, 98, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

TEST_F(bilinearInterpolationTest, TestWin_4)
{
    init(4000, 320, &stopW, &accesPatternStep, 255, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    line1 = inputGen.GetLine(MAX_SIZE, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

TEST_F(bilinearInterpolationTest, TestWin_5)
{
    init(3800, 2000, &stopW, &accesPatternStep, 173, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    line1 = inputGen.GetLine(MAX_SIZE, 0);
    line2 = inputGen.GetLine(MAX_SIZE, 0);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}


TEST_F(bilinearInterpolationTest, TestWin_6)
{
    inputGen.SelectGenerator("random");
    int tmp = randGen->GenerateUChar(0, 255);
    init(1120, 2800, &stopW, &accesPatternStep, tmp, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

TEST_F(bilinearInterpolationTest, TestWin_7)
{
    inputGen.SelectGenerator("random");

    int tmp = randGen->GenerateUChar(0, 255);

    int srcWidth, destWidth;
    float scale;

    do
    {
        srcWidth = randGen->GenerateUShort(30, 5000);
        destWidth = randGen->GenerateUShort(30, 5000);
        scale = (float)srcWidth / destWidth;
    }while((scale*10 == (int)(scale*10)));

    init(srcWidth, destWidth, &stopW, &accesPatternStep, tmp, lut, precalcPos);

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    line1 = inputGen.GetLine(MAX_SIZE, 0, 255);
    line2 = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolation_asm_test(line1, line2, dstAsm, precalcPos, lut, stopW, accesPatternStep);
    //RecordProperty("CyclePerPixel", bilinearInterpolationCycleCount / (stopW));
    bilinearInterpolation(line1, line2, dstC, precalcPos, lut, stopW, accesPatternStep);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

