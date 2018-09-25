//Asm function prototype:
//     void mvcvCalcBxBy_asm(const u8* patchI, ClSize isz, const float* Ix, const float* Iy, mvCvPoint minI,
//                const u8* patchJ, ClSize jsz, mvCvPoint minJ, float& error);

//Asm test function prototype:
//       mvCvPoint2D32f calcBxBy_asm_test(const unsigned char* patchI, ClSize isz, const float* Ix, const float* Iy, mvCvPoint minI,
//                     const unsigned char* patchJ, ClSize jsz, mvCvPoint minJ, float& error);

//C function prototype:
//       void mvCvPoint2D32f mvcvCalcBxBy(const u8* patchI, ClSize isz, const float* Ix, const float* Iy, mvCvPoint minI,
//                    const u8* patchJ, ClSize jsz, mvCvPoint minJ, float& error);

#include "gtest/gtest.h"
#include "calcBxBy.h"
#include "calcBxBy_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>



class calcBxByTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    u8* patchI_C;
    u8* patchJ_C;
    float* Ix_C;
    float* Iy_C;

    u8* patchI_Asm;
    u8* patchJ_Asm;
    float* Ix_Asm;
    float* Iy_Asm;

    ClSizeW isz;
    ClSizeW jsz;
    mvCvPointW minI;
    mvCvPointW minJ;
    float errorAsm;
    float errorC;
    mvCvPoint2D32f retValAsm;
    mvCvPoint2D32f retValC;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(calcBxByTest, TestIequalJ)
{
    inputGen.SelectGenerator("uniform");
    patchI_C = inputGen.GetLine(1920, 2);
    patchJ_C = inputGen.GetLine(1920, 2);
    Ix_C = inputGen.GetLineFloat(1920, 33);
    Iy_C = inputGen.GetLineFloat(1920, 33);

    patchI_Asm = inputGen.GetLine(1920, 2);
    patchJ_Asm = inputGen.GetLine(1920, 2);
    Ix_Asm = inputGen.GetLineFloat(1920, 33);
    Iy_Asm = inputGen.GetLineFloat(1920, 33);

    isz.x = 11;
    isz.y = 4;
    jsz.x = 11;
    jsz.y = 50;
    minI.x = 1;
    minI.y = 2;
    minJ.x = 3;
    minJ.y = 19;

    float *res = calcBxBy_asm_test(patchI_Asm, (u32 *)&isz.x, Ix_Asm, Iy_Asm,
                                   (u32 *)&minI.x, patchJ_Asm, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI_C, isz, Ix_C, Iy_C, minI, patchJ_C,
                           jsz, minJ, &errorC);

    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}

TEST_F(calcBxByTest, TestRandom1_jsz_x_smallerThen8)
{
    inputGen.SelectGenerator("random");
    patchI_C = inputGen.GetLine(1920, 0, 255);
    patchJ_C = inputGen.GetLine(1920, 0, 255);
    Ix_C = inputGen.GetLineFloat(1920, 0, 255);
    Iy_C = inputGen.GetLineFloat(1920, 0, 255);

    patchI_Asm = inputGen.GetLine(1920, 0, 255);
    patchJ_Asm = inputGen.GetLine(1920, 0, 255);
    Ix_Asm = inputGen.GetLineFloat(1920, 0, 255);
    Iy_Asm = inputGen.GetLineFloat(1920, 0, 255);

    for(int i = 0; i < 1920; i++)
    {
        patchI_C[i] = patchI_Asm[i];
        patchJ_C[i] = patchJ_Asm[i];
        Ix_C[i] = Ix_Asm[i];
        Iy_C[i] = Iy_Asm[i];
    }

    isz.x = 11;
    isz.y = 4;
    jsz.x = 6;
    jsz.y = 50;
    minI.x = 1;
    minI.y = 2;
    minJ.x = 3;
    minJ.y = 19;

    float *res = calcBxBy_asm_test(patchI_Asm, (u32 *)&isz.x, Ix_Asm, Iy_Asm,
                                   (u32 *)&minI.x, patchJ_Asm, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI_C, isz, Ix_C, Iy_C, minI, patchJ_C,
                           jsz, minJ, &errorC);

    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}

TEST_F(calcBxByTest, TestRandom2_jsz_x_biggerThen8)
{
    inputGen.SelectGenerator("random");
    patchI_C = inputGen.GetLine(1920, 0, 255);
    patchJ_C = inputGen.GetLine(1920, 0, 255);
    Ix_C = inputGen.GetLineFloat(1920, 0, 255);
    Iy_C = inputGen.GetLineFloat(1920, 0, 255);

    patchI_Asm = inputGen.GetLine(1920, 0, 255);
    patchJ_Asm = inputGen.GetLine(1920, 0, 255);
    Ix_Asm = inputGen.GetLineFloat(1920, 0, 255);
    Iy_Asm = inputGen.GetLineFloat(1920, 0, 255);

    for(int i = 0; i < 1920; i++)
    {
        patchI_C[i] = patchI_Asm[i];
        patchJ_C[i] = patchJ_Asm[i];
        Ix_C[i] = Ix_Asm[i];
        Iy_C[i] = Iy_Asm[i];
    }

    isz.x = 11;
    isz.y = 4;
    jsz.x = 11;
    jsz.y = 50;
    minI.x = 1;
    minI.y = 2;
    minJ.x = 3;
    minJ.y = 19;

    float *res = calcBxBy_asm_test(patchI_Asm, (u32 *)&isz.x, Ix_Asm, Iy_Asm,
                                   (u32 *)&minI.x, patchJ_Asm, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI_C, isz, Ix_C, Iy_C, minI, patchJ_C,
                           jsz, minJ, &errorC);
    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}

TEST_F(calcBxByTest, TestIequalK)
{
    inputGen.SelectGenerator("uniform");
    patchI_C = inputGen.GetLine(1920, 2);
    patchJ_C = inputGen.GetLine(1920, 2);
    Ix_C = inputGen.GetLineFloat(1920, 33);
    Iy_C = inputGen.GetLineFloat(1920, 33);

    patchI_Asm = inputGen.GetLine(1920, 2);
    patchJ_Asm = inputGen.GetLine(1920, 2);
    Ix_Asm = inputGen.GetLineFloat(1920, 33);
    Iy_Asm = inputGen.GetLineFloat(1920, 33);

    isz.x = 13;
    isz.y = 13;
    jsz.x = 11;
    jsz.y = 11;
    minI.x = 0;
    minI.y = 0;
    minJ.x = 0;
    minJ.y = 0;

    float *res = calcBxBy_asm_test(patchI_Asm, (u32 *)&isz.x, Ix_Asm, Iy_Asm,
                                   (u32 *)&minI.x, patchJ_Asm, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI_C, isz, Ix_C, Iy_C, minI, patchJ_C,
                           jsz, minJ, &errorC);
   
    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}

TEST_F(calcBxByTest, TestRandomData)
{
    inputGen.SelectGenerator("random");
    Ix_C = inputGen.GetLineFloat(512, 33, 0);
    Iy_C = inputGen.GetLineFloat(512, 33, 0);
    Ix_Asm = inputGen.GetLineFloat(512, 33);
    Iy_Asm = inputGen.GetLineFloat(512, 33);
    unsigned char patchI[1920] = {
        0x6c, 0x6c, 0x6c, 0x6b, 0x70, 0x78, 0x76, 0x74, 0x73, 0x70,
        0x6c, 0x6c, 0x6c, 0x6b, 0x70, 0x78, 0x76, 0x73, 0x73, 0x6f,
        0x6c, 0x6c, 0x6c, 0x6c, 0x70, 0x78, 0x75, 0x73, 0x73, 0x6f,
        0x6d, 0x6d, 0x6c, 0x6c, 0x70, 0x78, 0x76, 0x73, 0x73, 0x6f,
        0x6d, 0x6d, 0x6c, 0x6c, 0x70, 0x77, 0x76, 0x74, 0x72, 0x70,
        0x6d, 0x6d, 0x6d, 0x6c, 0x70, 0x77, 0x76, 0x74, 0x72, 0x71,
        0x6e, 0x6e, 0x6e, 0x6e, 0x71, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x73, 0x73, 0x73, 0x72, 0x73, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x78, 0x78, 0x77, 0x76, 0x76, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x77, 0x77, 0x78, 0x77, 0x77, 0x77, 0x75, 0x73, 0x72, 0x73,
        0x76, 0x76, 0x77, 0x77, 0x77, 0x78, 0x76, 0x73, 0x73, 0x73,
        0x76, 0x76, 0x76, 0x76, 0x77, 0x78, 0x76, 0x74, 0x73, 0x74,
        0x77, 0x77, 0x76, 0x76, 0x77, 0x78, 0x76, 0x74, 0x73, 0x74
    };
    unsigned char patchJ[1920] = {
    0x69, 0x70, 0x72, 0x74, 0x73, 0x73, 0x73, 0x75,
    0x67, 0x69, 0x6d, 0x70, 0x71, 0x71, 0x71, 0x73,
    0x67, 0x68, 0x6c, 0x70, 0x70, 0x6f, 0x6f, 0x70,
    0x69, 0x6c, 0x6e, 0x70, 0x70, 0x6f, 0x6f, 0x6f,
    0x6c, 0x70, 0x72, 0x72, 0x73, 0x72, 0x72, 0x72,
    0x6d, 0x70, 0x72, 0x72, 0x73, 0x73, 0x73, 0x73,
    0x71, 0x73, 0x74, 0x74, 0x74, 0x74, 0x75, 0x75,
    0x7b, 0x7b, 0x7b, 0x7b, 0x7c, 0x7c, 0x7d, 0x7d,
    0x7e, 0x7d, 0x7d, 0x7d, 0x7f, 0x80, 0x82, 0x80,
    0x78, 0x77, 0x77, 0x78, 0x79, 0x7b, 0x7e, 0x7d,
    0x72, 0x72, 0x72, 0x72, 0x73, 0x75, 0x7c, 0x7c};
    float Ix[1920] =
    {
        0.000000, -0.406250, 2.000000, 6.406250, 2.906250, -2.406250, -1.406250, -2.000000,
        -0.093750, -0.187500, 0xFF, 6.093750, 2.687500, -2.500000, -1.187500, -2.000000,
        -0.406250, -0.406250, 2.000000, 5.906250, 2.906250, 0xFF, 0xFF, -2.000000,
        -0.406250, -0.500000, 1.906250, 5.593750, 3.000000, -1.687500, -1.906250, -1.906250,
		0xFF, -0.406250, 0xFF, 5.312500, 2.812500, -1.500000, -1.906250, -1.500000,
        0.000000, -0.187500, 1.218750, 0xFF, 2.000000, -1.500000, -1.593750, -1.093750,
        -0.093750, -0.500000, 0.187500, 2.500000, 0.906250, -1.500000, 0xFF, -1.000000,
        -0.218750, -0.718750, -0.406250, 0.781250, -0.312500, -1.593750, -1.500000, -0.812500,
        0.312500, 0xFF, -0.406250, 0.187500, -0.812500, -2.000000, -1.500000, -0.187500,
        0.406250, 0.312500, 0.000000, 0.500000, 0xFF, -2.312500, -1.500000, 0.000000,
        0.000000, 0.000000, 0.406250, 0.906250, -0.500000, -2.093750, -1.500000, 0xFF
    };
    float Iy[1920] =
    {
        0.000000, 0.093750, 0.312500, 0.093750, -0.093750, -0.406250, -0.406250, -0.187500,
        0.406250, 0.187500, 0.312500, 0.093750, 0.000000, 0.000000, 0.000000, 0.000000,
        0.406250, 0.093750, 0.000000, -0.093750, -0.218750, 0.312500, 0.312500, -0.125000,
        0.093750, 0.312500, 0.093750, -0.093750, -0.312500, 0.000000, 0.218750, -0.031250,
        0.593750, 0.906250, 0.906250, 0.500000, 0.000000, -0.312500, -0.093750, 0.187500,
        3.000000, 3.000000, 2.718750, 1.500000, 0.187500, -0.312500, -0.093750, 0.093750,
        4.906250, 4.500000, 3.812500, 2.312500, 0.468750, 0.000000, 0.000000, 0.000000,
        2.093750, 2.406250, 2.406250, 1.718750, 0.375000, -0.093750, -0.312500, 0.000000,
        -0.812500, -0.093750, 0.406250, 0.500000, 0.500000, 0.312500, -0.125000, 0.312500,
        -0.593750, -0.812500, -0.500000, 0.000000, 0.406250, 0.500000, 0.500000, 0.500000,
        0.312500, -0.312500, -0.406250, -0.093750, 0.000000, 0.093750, 0.312500, 0.187500
    };
    isz.x = 19;
    isz.y = 13;
    jsz.x = 11;
    jsz.y = 11;
    minI.x = 3;
    minI.y = 0;
    minJ.x = 3;
    minJ.y = 0;

    float *res = calcBxBy_asm_test(patchI, (u32 *)&isz.x, Ix, Iy,
                                   (u32 *)&minI.x, patchJ, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI, isz, Ix, Iy, minI, patchJ,
                           jsz, minJ, &errorC);

    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}


TEST_F(calcBxByTest, TestRandomData2)
{
    inputGen.SelectGenerator("random");
    Ix_C = inputGen.GetLineFloat(512, 33, 0);
    Iy_C = inputGen.GetLineFloat(512, 33, 0);
    Ix_Asm = inputGen.GetLineFloat(512, 33);
    Iy_Asm = inputGen.GetLineFloat(512, 33);
    unsigned char patchI[1920] = {
        0x6c, 0x6c, 0x6c, 0x6b, 0x70, 0x78, 0x76, 0x74, 0x73, 0x70,
        0x6c, 0x6c, 0x6c, 0x6b, 0x70, 0x78, 0x76, 0x73, 0x73, 0x6f,
        0x6c, 0x6c, 0x6c, 0x6c, 0x70, 0x78, 0x75, 0x73, 0x73, 0x6f,
        0x6d, 0x6d, 0x6c, 0x6c, 0x70, 0x78, 0x76, 0x73, 0x73, 0x6f,
        0x6d, 0x6d, 0x6c, 0x6c, 0x70, 0x77, 0x76, 0x74, 0x72, 0x70,
        0x6d, 0x6d, 0x6d, 0x6c, 0x70, 0x77, 0x76, 0x74, 0x72, 0x71,
        0x6e, 0x6e, 0x6e, 0x6e, 0x71, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x73, 0x73, 0x73, 0x72, 0x73, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x78, 0x78, 0x77, 0x76, 0x76, 0x77, 0x75, 0x74, 0x72, 0x72,
        0x77, 0x77, 0x78, 0x77, 0x77, 0x77, 0x75, 0x73, 0x72, 0x73,
        0x76, 0x76, 0x77, 0x77, 0x77, 0x78, 0x76, 0x73, 0x73, 0x73,
        0x76, 0x76, 0x76, 0x76, 0x77, 0x78, 0x76, 0x74, 0x73, 0x74,
        0x77, 0x77, 0x76, 0x76, 0x77, 0x78, 0x76, 0x74, 0x73, 0x74
    };
    unsigned char patchJ[1920] = {
    0x69, 0x70, 0x72, 0x74, 0x73, 0x73, 0x73, 0x75,
    0x67, 0x69, 0x6d, 0x70, 0x71, 0x71, 0x71, 0x73,
    0x67, 0x68, 0x6c, 0x70, 0x70, 0x6f, 0x6f, 0x70,
    0x69, 0x6c, 0x6e, 0x70, 0x70, 0x6f, 0x6f, 0x6f,
    0x6c, 0x70, 0x72, 0x72, 0x73, 0x72, 0x72, 0x72,
    0x6d, 0x70, 0x72, 0x72, 0x73, 0x73, 0x73, 0x73,
    0x71, 0x73, 0x74, 0x74, 0x74, 0x74, 0x75, 0x75,
    0x7b, 0x7b, 0x7b, 0x7b, 0x7c, 0x7c, 0x7d, 0x7d,
    0x7e, 0x7d, 0x7d, 0x7d, 0x7f, 0x80, 0x82, 0x80,
    0x78, 0x77, 0x77, 0x78, 0x79, 0x7b, 0x7e, 0x7d,
    0x72, 0x72, 0x72, 0x72, 0x73, 0x75, 0x7c, 0x7c};
    float Ix[1920] =
    {
        0.000000, -0.406250, 2.000000, 6.406250, 2.906250, -2.406250, -1.406250, -2.000000,
        -0.093750, -0.187500, 2.000000, 6.093750, 2.687500, -2.500000, -1.187500, -2.000000,
        -0.406250, -0.406250, 2.000000, 5.906250, 2.906250, -2.312500, -1.500000, -2.000000,
        -0.406250, -0.500000, 1.906250, 5.593750, 3.000000, -1.687500, -1.906250, -1.906250,
        -0.093750, -0.406250, 1.593750, 5.312500, 2.812500, -1.500000, -1.906250, -1.500000,
        0.000000, -0.187500, 1.218750, 4.312500, 2.000000, -1.500000, -1.593750, -1.093750,
        -0.093750, -0.500000, 0.187500, 2.500000, 0.906250, -1.500000, -1.500000, -1.000000,
        -0.218750, -0.718750, -0.406250, 0.781250, -0.312500, -1.593750, -1.500000, -0.812500,
        0.312500, -0.093750, -0.406250, 0.187500, -0.812500, -2.000000, -1.500000, -0.187500,
        0.406250, 0.312500, 0.000000, 0.500000, -0.593750, -2.312500, -1.500000, 0.000000,
        0.000000, 0.000000, 0.406250, 0.906250, -0.500000, -2.093750, -1.500000, 0.000000
    };
    float Iy[1920] =
    {
        0.000000, 0.093750, 0.312500, 0.093750, -0.093750, -0.406250, -0.406250, -0.187500,
        0.406250, 0.187500, 0.312500, 0.093750, 0.000000, 0.000000, 0.000000, 0.000000,
        0.406250, 0.093750, 0.000000, -0.093750, -0.218750, 0.312500, 0.312500, -0.125000,
        0.093750, 0.312500, 0.093750, -0.093750, -0.312500, 0.000000, 0.218750, -0.031250,
        0.593750, 0.906250, 0.906250, 0.500000, 0.000000, -0.312500, -0.093750, 0.187500,
        3.000000, 3.000000, 2.718750, 1.500000, 0.187500, -0.312500, -0.093750, 0.093750,
        4.906250, 4.500000, 3.812500, 2.312500, 0.468750, 0.000000, 0.000000, 0.000000,
        2.093750, 2.406250, 2.406250, 1.718750, 0.375000, -0.093750, -0.312500, 0.000000,
        -0.812500, -0.093750, 0.406250, 0.500000, 0.500000, 0.312500, -0.125000, 0.312500,
        -0.593750, -0.812500, -0.500000, 0.000000, 0.406250, 0.500000, 0.500000, 0.500000,
        0.312500, -0.312500, -0.406250, -0.093750, 0.000000, 0.093750, 0.312500, 0.187500
    };

    isz.x = 21;
    isz.y = 4;
    jsz.x = 21;
    jsz.y = 50;
    minI.x = 1;
    minI.y = 2;
    minJ.x = 3;
    minJ.y = 19;

    float *res = calcBxBy_asm_test(patchI, (u32 *)&isz.x, Ix, Iy,
                                   (u32 *)&minI.x, patchJ, (u32 *)&jsz.x, (u32 *)&minJ.x, &errorAsm);
    retValAsm = mvCvPoint2D32f( res[0], res[1] );
    RecordProperty("CyclePerPixel", calcBxByCycleCount);
    retValC = mvcvCalcBxBy(patchI, isz, Ix, Iy, minI, patchJ,
                           jsz, minJ, &errorC);

    ASSERT_NEAR(errorC, errorAsm, 2.0f);
    ASSERT_NEAR(retValC.x, retValAsm.x, 2.0f);
    ASSERT_NEAR(retValC.y, retValAsm.y, 2.0f);
}
