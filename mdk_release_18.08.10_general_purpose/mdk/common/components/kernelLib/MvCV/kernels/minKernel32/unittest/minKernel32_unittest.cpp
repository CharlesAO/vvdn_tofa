
//Parameter description:
// Compute the minimum value from an input line of dimension 32
// @param[in] in         - input line
// @param[out] minVal    - minimum value from the input line
// @return               - nothing



#include "gtest/gtest.h"
#include <minKernel32.h>
#include "minKernel32_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define WIDTH 32

class mvcvMinKernel32KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned char *input;
    unsigned char *minC;
    unsigned char *minAsm;

    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};

TEST_F(mvcvMinKernel32KernelTest, TestAllRandomValues)
{

    inputGen.SelectGenerator("uniform");
    minC   = inputGen.GetEmptyLine(1);
    minAsm = inputGen.GetEmptyLine(1);
    
    inputGen.SelectGenerator("random");
    input = inputGen.GetLine((WIDTH), 0, 255);
    
    mvcvMinKernel32(input, minC);
    mvcvMinKernel32_asm_test(input, minAsm);

    RecordProperty("CyclePerPixel", mvcvMinKernel32CycleCount);

    outputChecker.CompareArrays(minC, minAsm, 1);
    
}
