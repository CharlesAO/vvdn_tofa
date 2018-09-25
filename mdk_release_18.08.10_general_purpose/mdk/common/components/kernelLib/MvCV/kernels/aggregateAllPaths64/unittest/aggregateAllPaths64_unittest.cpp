
//Parameter description:
// mvcvAggregateAllPaths64     - computes average sum from 4 paths (width x 64 arrays)
// @param[in] path0            - pointer to a width x 64 array from path0
// @param[in] path1            - pointer to a width x 64 array from path1
// @param[in] path2            - pointer to a width x 64 array from path2
// @param[in] path3            - pointer to a width x 64 array from path3
// @param[out] output             - pointer to output result
// @param[in] width              - line width
// @return    Nothing


#include "gtest/gtest.h"
#include <aggregateAllPaths64.h>
#include "aggregateAllPaths64_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define OFFSET PADDING/2

class mvcvAggregateAllPaths64KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *path0;
    unsigned char *path1;
    unsigned char *path2;
    unsigned char *path3;
    unsigned char *outputAsm;
    unsigned char *outputC;
    unsigned int width;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvAggregateAllPaths64KernelTest, TestAll0)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
    inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);


    path0 = inputGen.GetLine((width * DISPARITIES), 0);
    path1 = inputGen.GetLine((width * DISPARITIES), 0);
    path2 = inputGen.GetLine((width * DISPARITIES), 0);
    path3 = inputGen.GetLine((width * DISPARITIES), 0);

    mvcvAggregateAllPaths64(path0, path1, path2, path3, outputC + OFFSET, width);
    mvcvAggregateAllPaths64_asm_test(path0, path1, path2, path3, outputAsm, width);

    RecordProperty("CyclePerPixel", mvcvAggregateAllPaths64CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}

TEST_F(mvcvAggregateAllPaths64KernelTest, TestAllMaxValues)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
    inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);


    path0 = inputGen.GetLine((width * DISPARITIES), 255);
    path1 = inputGen.GetLine((width * DISPARITIES), 255);
    path2 = inputGen.GetLine((width * DISPARITIES), 255);
    path3 = inputGen.GetLine((width * DISPARITIES), 255);

    mvcvAggregateAllPaths64(path0, path1, path2, path3, outputC + OFFSET, width);
    mvcvAggregateAllPaths64_asm_test(path0, path1, path2, path3, outputAsm, width);

    RecordProperty("CyclePerPixel", mvcvAggregateAllPaths64CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}

TEST_F(mvcvAggregateAllPaths64KernelTest, TestAllRandomValues)
{
    width = 320;
    inputGen.SelectGenerator("uniform");
    outputC   = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    outputAsm = inputGen.GetEmptyLine(width * DISPARITIES + PADDING);
    inputGen.FillLine(outputC,  width * DISPARITIES + PADDING , 5);
    inputGen.FillLine(outputAsm,width * DISPARITIES + PADDING , 5);

    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(1, 320, 1);
    path0 = inputGen.GetLine((width * DISPARITIES), 0, 255);
    path1 = inputGen.GetLine((width * DISPARITIES), 0, 255);
    path2 = inputGen.GetLine((width * DISPARITIES), 0, 255);
    path3 = inputGen.GetLine((width * DISPARITIES), 0, 255);

    mvcvAggregateAllPaths64(path0, path1, path2, path3, outputC + OFFSET, width);
    mvcvAggregateAllPaths64_asm_test(path0, path1, path2, path3, outputAsm, width);

    RecordProperty("CyclePerPixel", mvcvAggregateAllPaths64CycleCount / (width * DISPARITIES ));

    outputChecker.CompareArrays(outputC, outputAsm, width * DISPARITIES + PADDING);
}
