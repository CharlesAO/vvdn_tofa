//AddV2Fp16 test    
//Asm function prototype:
//    void mvcvAddV2Fp16_asm(half** dst, half** input, u32 width);
//
//Asm test function prototype:
//    void AddV2Fp16_asm_test(half** dst, half** input, u32 width);
//
//C function prototype:
//    void mvcvAddV2Fp16(half** dst, half** input, u32 width);
//
/// mvcvAddV2Fp16 kernel does an addition between elements in *dst and *input
/// @param[in][out] dst        - Input line that contains elements to be added and also destination for new elements
/// @param[in] input           - Input line that contains elements to be added
/// @param[in] width           - Width of the input lines
/// @return    Nothing

#include "gtest/gtest.h"
#include "addV2Fp16.h"
#include "addV2Fp16_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

#define PADDING 32
class AddV2Fp16Test : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }


    half** inLines;
    half** outLinesC;
    half** outLinesASM;
    int width;
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

};

TEST_F(AddV2Fp16Test , AllUniform)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 100);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AddV2Fp16Test , AllUniform0)
{
    width = 1024;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 0);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 0);

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AddV2Fp16Test , AllUniform255)
{
    width = 1024;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 255);

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AddV2Fp16Test , AllRandomSmallWidth)
{
    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(8, 1280, 8);
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AddV2Fp16Test , AllRandomBigWidth)
{
    width = randGen->GenerateUInt(1280, 1920, 8);
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AddV2Fp16Test , AllRandomMaxWidth)
{
    width = 1920;
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 2, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AddV2Fp16_asm_test(outLinesASM, inLines, width);
    mvcvAddV2Fp16(outLinesC, inLines, width);
    RecordProperty("CycleCount", AddV2Fp16CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}
