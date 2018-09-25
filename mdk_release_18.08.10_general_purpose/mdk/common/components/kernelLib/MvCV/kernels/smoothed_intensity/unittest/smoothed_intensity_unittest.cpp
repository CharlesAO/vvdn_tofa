//mvcvSmoothed_intensity test

#include "gtest/gtest.h"
#include "smoothed_intensity.h"
#include "smoothed_intensity_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

class mvcvSmoothed_intensityTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    float kp_x;
    float kp_y;
    u8* integral;
    int integral_stride;
    float* pattern_pnt_lst;
    int pattern_pnt_sz;
    unsigned short* intensitiesC;
    unsigned short* intensitiesAsm;
    unsigned int width;
    unsigned int height;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    // virtual void TearDown() {}
};

TEST_F(mvcvSmoothed_intensityTest , TestFixedValues)
{
    pattern_pnt_sz = 44;
    inputGen.SelectGenerator("random");
    width = 380;
    height = 4;

    integral = inputGen.GetLine(width * (height+4) * 4, 0, 255);
    pattern_pnt_lst = inputGen.GetLineFloat(pattern_pnt_sz * 3, 0);

    kp_x = 64.518532;
    kp_y = 1.713143;

    integral_stride = width * 4;

    int i = 0;
    for (i=0;i<pattern_pnt_sz*3; i+=3)
    {
        pattern_pnt_lst[i+2] = randGen->GenerateFloat(0.0f, 1.0f);
        pattern_pnt_lst[i] = randGen->GenerateFloat( - kp_x , width - kp_x - pattern_pnt_lst[i+2]);
        pattern_pnt_lst[i+1] = randGen->GenerateFloat( - kp_y , height - kp_y - pattern_pnt_lst[i+2]);
    }
    inputGen.SelectGenerator("uniform");
    intensitiesC = inputGen.GetLineU16(pattern_pnt_sz, 5);
    intensitiesAsm = inputGen.GetLineU16(pattern_pnt_sz, 5);

    mvcvSmoothed_intensity(kp_x, kp_y, (u8*)(integral+ integral_stride * 2), integral_stride , (mvcvPatternPoint*)pattern_pnt_lst, pattern_pnt_sz, intensitiesC);
    mvcvSmoothed_intensity_asm_test(kp_x, kp_y, integral, integral_stride, pattern_pnt_lst, pattern_pnt_sz, intensitiesAsm, width, height);

    //arrCheck.DumpArrays("integral", "integral.c", &integral, width * (height+2) * 4, 1);
    //arrCheck.DumpArrays("intensitiesC", "intensitiesC.c", &intensitiesC, 5, 1);
    //arrCheck.DumpArrays("pattern_pnt_lst", "pattern_pnt_lst.c", &pattern_pnt_lst, pattern_pnt_sz * 3, 1);
    RecordProperty("CycleCount", mvcvSmoothed_intensityCycleCount);

    arrCheck.CompareArrays(intensitiesC,intensitiesAsm,pattern_pnt_sz);
}

TEST_F(mvcvSmoothed_intensityTest , TestMaxTestDataSize)
{
    pattern_pnt_sz = 44;
    inputGen.SelectGenerator("random");
    width  = 640;//randGen->GenerateUInt(0, 640, 1);
    height = 10;//randGen->GenerateUInt(0, 10, 1);

    integral = inputGen.GetLine(width * (height+4) * 4, 0, 255);
    pattern_pnt_lst = inputGen.GetLineFloat(pattern_pnt_sz * 3, 0);

    kp_x = randGen->GenerateFloat(0.0f, width);
    kp_y = randGen->GenerateFloat(0.0f, height);
    integral_stride = width * 4;

    int i = 0;
    for (i=0;i<pattern_pnt_sz*3; i+=3)
    {
        pattern_pnt_lst[i+2] = randGen->GenerateFloat(0.0f, 1.0f);
        pattern_pnt_lst[i] = randGen->GenerateFloat( - kp_x , width - kp_x - pattern_pnt_lst[i+2]);
        pattern_pnt_lst[i+1] = randGen->GenerateFloat( - kp_y , height - kp_y - pattern_pnt_lst[i+2]);
    }
    inputGen.SelectGenerator("uniform");
    intensitiesC = inputGen.GetLineU16(pattern_pnt_sz, 5);
    intensitiesAsm = inputGen.GetLineU16(pattern_pnt_sz, 5);

    mvcvSmoothed_intensity(kp_x, kp_y, (u8*)(integral+ integral_stride * 2), integral_stride , (mvcvPatternPoint*)pattern_pnt_lst, pattern_pnt_sz, intensitiesC);
    mvcvSmoothed_intensity_asm_test(kp_x, kp_y, integral, integral_stride, pattern_pnt_lst, pattern_pnt_sz, intensitiesAsm, width, height);

    RecordProperty("CycleCount", mvcvSmoothed_intensityCycleCount);

    arrCheck.CompareArrays(intensitiesC,intensitiesAsm,pattern_pnt_sz);
}


TEST_F(mvcvSmoothed_intensityTest , TestRandom)
{
    pattern_pnt_sz = randGen->GenerateUInt(4, 100, 4);;
    pattern_pnt_sz = 100;

    inputGen.SelectGenerator("random");
    width  = randGen->GenerateUInt(8, 640, 4);
    height = randGen->GenerateUInt(8, 10, 4);

    integral = inputGen.GetLine(width * (height+4) * 4, 0, 255);
    pattern_pnt_lst = inputGen.GetLineFloat(pattern_pnt_sz * 3, 0);

    kp_x = randGen->GenerateFloat(0.0f, width);
    kp_y = randGen->GenerateFloat(0.0f, height);
    integral_stride = width * 4;

    int i = 0;
    for (i=0;i<pattern_pnt_sz*3; i+=3)
    {
        pattern_pnt_lst[i+2] = randGen->GenerateFloat(0.0f, 1.0f);
        pattern_pnt_lst[i] = randGen->GenerateFloat( - kp_x , width - kp_x - pattern_pnt_lst[i+2]);
        pattern_pnt_lst[i+1] = randGen->GenerateFloat( - kp_y , height - kp_y - pattern_pnt_lst[i+2]);
    }
    inputGen.SelectGenerator("uniform");
    intensitiesC = inputGen.GetLineU16(pattern_pnt_sz, 5);
    intensitiesAsm = inputGen.GetLineU16(pattern_pnt_sz, 5);

    mvcvSmoothed_intensity(kp_x, kp_y, (u8*)(integral+ integral_stride * 2), integral_stride , (mvcvPatternPoint*)pattern_pnt_lst, pattern_pnt_sz, intensitiesC);
    mvcvSmoothed_intensity_asm_test(kp_x, kp_y, integral, integral_stride, pattern_pnt_lst, pattern_pnt_sz, intensitiesAsm, width, height);

    RecordProperty("CycleCount", mvcvSmoothed_intensityCycleCount);

    arrCheck.CompareArrays(intensitiesC,intensitiesAsm,pattern_pnt_sz);
}
