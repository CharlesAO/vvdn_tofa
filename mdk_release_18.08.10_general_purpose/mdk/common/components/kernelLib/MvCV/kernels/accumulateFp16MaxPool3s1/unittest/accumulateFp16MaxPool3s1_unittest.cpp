//AccumulateFp16MaxPool3s1 test    
//Asm function prototype:
//    void mvcvAccumulateFp16MaxPool3s1_asm(half** dst, half** input, u32 width);
//
//Asm test function prototype:
//    void AccumulateFp16MaxPool3s1_asm_test(half** dst, half** input, u32 width);
//
//C function prototype:
//    void mvcvAccumulateFp16MaxPool3s1(half** dst, half** input, u32 width);
//
//Parameter description:
// param[in][out] dst        - Input line that contains elements to be added and also destination for new elements
// param[in] input           - Input line that contains elements to be added
// param[in] width           - Width of the input lines

#include "gtest/gtest.h"
#include "accumulateFp16MaxPool3s1.h"
#include "accumulateFp16MaxPool3s1_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

#define PADDING 32
class AccumulateFp16MaxPool3s1Test : public ::testing::Test {
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

TEST_F(AccumulateFp16MaxPool3s1Test , AllUniform)
{
    width = 16;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AccumulateFp16MaxPool3s1Test , AllUniform0)
{
    width = 1024;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 0);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 0);

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AccumulateFp16MaxPool3s1Test , AllUniform255)
{
    width = 1024;
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 255);

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AccumulateFp16MaxPool3s1Test , AllRandomSmallWidth)
{
    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(8, 1280, 8);
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AccumulateFp16MaxPool3s1Test , AllRandomBigWidth)
{
    width = randGen->GenerateUInt(1280, 1920, 8);
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}

TEST_F(AccumulateFp16MaxPool3s1Test , AllRandomMaxWidth)
{
    width = 1920;
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesC = inputGen.GetLinesFloat16(width + PADDING, 1, 0, 255);
    outLinesASM = inputGen.GetLinesFloat16(width + PADDING, 1, 100);
    memcpy ( outLinesASM[0], outLinesC[0], (width + PADDING) * sizeof(half) );

    
    AccumulateFp16MaxPool3s1_asm_test(outLinesASM, inLines, width);
    mvcvAccumulateFp16MaxPool3s1(outLinesC, inLines, width);
    RecordProperty("CycleCount", AccumulateFp16MaxPool3s1CycleCount);

    arrCheck.CompareArrays(outLinesASM[0],outLinesC[0],width + PADDING);
}
