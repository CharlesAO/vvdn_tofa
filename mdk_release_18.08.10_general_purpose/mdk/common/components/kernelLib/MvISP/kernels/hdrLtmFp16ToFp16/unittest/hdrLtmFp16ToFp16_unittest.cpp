//mvispHdrLtmFp16ToFp16  
/// 
/// @param[out] output          - the output of the local tone mapping
/// @param[in]  inLumaFusion    - Short and Long exposures are adaptively blended
/// @param[in]  inGaussUp       - the LumaFusion after downscale, convolution and upscale
/// @param[in]  params          - thr, thr2, thr3 – tone map threholds
/// @param[in]  width           - width of the input lines
/// @return     Nothing

#include "gtest/gtest.h"
#include "hdrLtmFp16ToFp16_asm_test.h"
#include <hdrLtmFp16ToFp16.h>
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = 0.01;//accepted tolerance between C and ASM results

class hdrLtmFp16ToFp16 : public ::testing::TestWithParam< unsigned long > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half *inLumaFusion;
    half *inGaussUpC;
    half *inGaussUpAsm;
    half *outLineC;
    half *outLineAsm;
    float *params;
    unsigned int width;
    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};


TEST_F(hdrLtmFp16ToFp16, Width16RandInput)
{
    width = 16;
    inputGen.SelectGenerator("random");
    params = inputGen.GetLineFloat(4, 0);
    params[3] = 0;
    inLumaFusion = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpC = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpAsm = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    memcpy(inGaussUpAsm, inGaussUpC, width * sizeof(half));
    
    inputGen.SelectGenerator("uniform"); 
    outLineC = (half*)inputGen.GetLine(width * 2, 0);
    outLineAsm = (half*)inputGen.GetLine(width * 2, 0);
    
    mvispHdrLtmFp16ToFp16(inLumaFusion, inGaussUpC, outLineC, width, (HdrLtmParam*)params);
    hdrLtmFp16ToFp16_asm_test(inLumaFusion, inGaussUpAsm, outLineAsm, width, params);
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
    outputChecker.CompareArrays(inGaussUpC, inGaussUpAsm, width, DELTA);

    //outputChecker.DumpArrays("refInputRGB", "refInputRGB.c", inRGB, width, 3);
    //outputChecker.DumpArrays("refInputY", "refInputY.c", inY, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width, 3);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width, 3);
}

TEST_F(hdrLtmFp16ToFp16, Width16UniformInput)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    params = inputGen.GetLineFloat(4, 0);
    params[3] = 0;
    inLumaFusion = inputGen.GetLineFloat16(width, (half)5.0f);
    inGaussUpC = inputGen.GetLineFloat16(width, (half)5.0f);
    inGaussUpAsm = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    memcpy(inGaussUpAsm, inGaussUpC, width * sizeof(half));
       
    inputGen.SelectGenerator("uniform"); 
    outLineC = (half*)inputGen.GetLine(width * 2, 0);
    outLineAsm = (half*)inputGen.GetLine(width * 2, 0);
    
    mvispHdrLtmFp16ToFp16(inLumaFusion, inGaussUpC, outLineC, width, (HdrLtmParam*)params);
    hdrLtmFp16ToFp16_asm_test(inLumaFusion, inGaussUpAsm, outLineAsm, width, params);
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
    outputChecker.CompareArrays(inGaussUpC, inGaussUpAsm, width, DELTA);

    //outputChecker.DumpArrays("refInputRGB", "refInputRGB.c", inRGB, width, 3);
    //outputChecker.DumpArrays("refInputY", "refInputY.c", inY, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width, 3);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width, 3);
}

TEST_F(hdrLtmFp16ToFp16, RandWidthRandData1)
{
    width = randGen->GenerateUInt(8, 1280, 8);
    inputGen.SelectGenerator("random");
    params = inputGen.GetLineFloat(4, 0, 1);
    params[3] = 0;
    inLumaFusion = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpC = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpAsm = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    memcpy(inGaussUpAsm, inGaussUpC, width * sizeof(half));
       
    inputGen.SelectGenerator("uniform"); 
    outLineC = (half*)inputGen.GetLine(width * 2, 0);
    outLineAsm = (half*)inputGen.GetLine(width * 2, 0);
    
    mvispHdrLtmFp16ToFp16(inLumaFusion, inGaussUpC, outLineC, width, (HdrLtmParam*)params);
    hdrLtmFp16ToFp16_asm_test(inLumaFusion, inGaussUpAsm, outLineAsm, width, params);
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
    outputChecker.CompareArrays(inGaussUpC, inGaussUpAsm, width, DELTA);

    //outputChecker.DumpArrays("refInputRGB", "refInputRGB.c", inRGB, width, 3);
    //outputChecker.DumpArrays("refInputY", "refInputY.c", inY, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width, 3);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width, 3);
}

TEST_F(hdrLtmFp16ToFp16, RandWidthRandData2)
{
    width = randGen->GenerateUInt(1280, 1920, 8);
    inputGen.SelectGenerator("random");
    params = inputGen.GetLineFloat(4, 0, 1);
    params[3] = 0;
    inLumaFusion = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpC = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    inGaussUpAsm = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
    memcpy(inGaussUpAsm, inGaussUpC, width * sizeof(half));
       
    inputGen.SelectGenerator("uniform"); 
    outLineC = (half*)inputGen.GetLine(width * 2, 0);
    outLineAsm = (half*)inputGen.GetLine(width * 2, 0);
    
    mvispHdrLtmFp16ToFp16(inLumaFusion, inGaussUpC, outLineC, width, (HdrLtmParam*)params);
    hdrLtmFp16ToFp16_asm_test(inLumaFusion, inGaussUpAsm, outLineAsm, width, params);
    
    outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
    outputChecker.CompareArrays(inGaussUpC, inGaussUpAsm, width, DELTA);

    //outputChecker.DumpArrays("refInputRGB", "refInputRGB.c", inRGB, width, 3);
    //outputChecker.DumpArrays("refInputY", "refInputY.c", inY, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width, 3);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width, 3);
}


