//meshExpand kernel test

//Asm function prototype:
//  void MeshGenerate(half *mesh, float *mat, u32 width, u32 posx, u32 posy)

//Asm test function prototype:
//  void meshGenerate_asm_test(half *mesh, float *mat, unsigned int width, unsigned int posx, unsigned int posy)

//C function prototype:
//  void MeshGenerate(half *mesh, float *mat, u32 width, u32 posx, u32 posy)

// mesh         - array of output_mesh line
// mat          - warpAffine mat
// width        - width of output line
// posx         - x coordinate
// posy         - y coordinate

#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "meshGenerate.h"
#include "meshGenerate_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>



using ::testing::TestWithParam;
using ::testing::Values;

const half DELTA = 4.0;

class meshGenerateTest : public ::testing::TestWithParam< unsigned long > {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }


    half *outLineC;
    half *outLineAsm;


    unsigned int width;
    unsigned int posx;
    unsigned int posy;
    float mat[6];


    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;

    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(meshGenerateTest, TestMinWidth)
{
    width = 176;
    inputGen.SelectGenerator("random");
    outLineC = inputGen.GetLineFloat16(width*2, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(width*2, 0.0f);

    posx = 0;
    posy = 0;

/*  mat[0] =  0.99619472;
    mat[1] = -0.087155744;
    mat[2] =  0.0f;
    mat[3] =  0.087155744;
    mat[4] =  0.99619472;
    mat[5] =  0.0f;  */

    int mat[6] = {(int)0x3F7F069E, (int)0xBDB27EB6, (int)0x00000000, (int)0x3DB27EB6, (int)0x3F7F069E, (int)0x00000000};


    mvcvMeshGenerate(outLineC, (float*)mat, width, posx*8, posy*8);
    meshGenerate_asm_test(outLineAsm, (float*)mat, width, posx, posy);

    outputCheck.CompareArrays(outLineC, outLineAsm, width, DELTA);

}


INSTANTIATE_TEST_CASE_P(RandomInputs, meshGenerateTest,
        Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960));

TEST_P(meshGenerateTest , TestRandomData)
{
    inputGen.SelectGenerator("random");
    width = GetParam();
    inputGen.SelectGenerator("uniform");
    outLineC = inputGen.GetLineFloat16(width*2, 0.0f);
    outLineAsm = inputGen.GetLineFloat16(width*2, 0.0f);

    for (int i=0; i<6; i++) {
        mat[i] = randGen->GenerateFloat(-0.1, 1.0);
    }

    posx = randGen->GenerateUInt(0, width);
    posy = randGen->GenerateUInt(0, width);

    mvcvMeshGenerate(outLineC, mat, width, posx*8, posy*8);
    meshGenerate_asm_test(outLineAsm, mat, width, posx, posy);

    outputCheck.CompareArrays(outLineC, outLineAsm, width, DELTA);
}
