//WarpMeshSampleFp16bit
//Asm function prototype:
//    void mvcvWarpMeshSampleFp16bit_asm(half **tile, half * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Asm test function prototype:
//  void  warpMeshSampleFp16bit_asm_test(half **input_tile, half * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);
//
//C function prototype:
//    void mvcvWarpMeshSampleFp16bit(half **tile, half * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height);
//
//Parameter description:
//
// mvcvWarpMeshSampleFp16bit - in order to compute everything ok we need one extra pixel to the right and one extra line at the bottom for interpolation
// param[in] tile                 - Input tile of data, (fp16 format) it has variable width/height size
// param[out] out_tile            - Output tile (fp16 format) it has a 16*16 fixed dimension
// param[in] mx                   - array of x coordinates relative to input tile
// param[in] my                   - array of y coordinates relative to input tile
// param[in] tile_width           - tile_width of the input tile + the eventualy padding when making the interpolation
// param[in] tile_height          - tile_height of the input tile + the eventualy padding when making the interpolation
// Nothing

#include "gtest/gtest.h"
#include "warpMeshSampleFp16bit.h"
#include "warpMeshExpand.h"
#include "warpMeshSampleFp16bit_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include "half.h"

#define TILE_SIZE 16
#define PADDING 32

half DELTA=0.7;

class warpMeshSampleFp16bittest : public ::testing::Test {
 protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    half **tile;
    half *out_tileC;
    half *out_tileAsm;
    half xcoord[4];
    half ycoord[4];
    float *xcoord_p;
    float *ycoord_p;
    half *mx;
    half *my;
    unsigned int tile_width;
    unsigned int tile_height;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    DataGenerator  genfloat;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(warpMeshSampleFp16bittest , first_test_fixed_dimensions)
{
    inputGen.SelectGenerator("random");
    tile_width = 60;
    tile_height = 38;
    
    xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
    xcoord[0] = *xcoord_p;                                                ycoord[0] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[0], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[0], tile_height-1);
    xcoord[1] = *xcoord_p;                                                ycoord[1] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
    xcoord[2] = *xcoord_p;                                                ycoord[2] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[2], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[2], tile_height-1);
    xcoord[3] = *xcoord_p;                                                ycoord[3] = *ycoord_p;
    
    tile = inputGen.GetLinesFloat16(tile_width, tile_height, 0, 255);
 
    inputGen.SelectGenerator("uniform");
    out_tileC = inputGen.GetLineFloat16((TILE_SIZE * TILE_SIZE), 0);
    out_tileAsm = inputGen.GetLineFloat16((TILE_SIZE * TILE_SIZE), 0);
    
    mx   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
    my   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
    
    mvcvWarpMeshExpand(xcoord, ycoord,   mx,   my);
    mvcvWarpMeshSampleFp16bit(tile,   out_tileC, mx, my, tile_width, tile_height);
    warpMeshSampleFp16bit_asm_test(tile, out_tileAsm, mx, my, tile_width, tile_height);
    
    RecordProperty("CyclePerPixel", warpMeshSampleFp16bitCycleCount / (TILE_SIZE * TILE_SIZE));

    arrCheck.CompareArrays(&out_tileC, &out_tileAsm, TILE_SIZE * TILE_SIZE, 1, DELTA);
    
}

TEST_F(warpMeshSampleFp16bittest , second_test_random_dimensions)
{
    inputGen.SelectGenerator("random");
    
    tile_width = randGen->GenerateUInt(20, 90, 1);
    tile_height = randGen->GenerateUInt(20, 90, 1);
    
    xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
    xcoord[0] = *xcoord_p;                                                ycoord[0] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[0], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[0], tile_height-1);
    xcoord[1] = *xcoord_p;                                                ycoord[1] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_width/2);  ycoord_p = (float *)inputGen.GetLineFloat(1,         0, tile_height/2);
    xcoord[2] = *xcoord_p;                                                ycoord[2] = *ycoord_p;                                                 
    xcoord_p = (float *)inputGen.GetLineFloat(1, xcoord[2], tile_width-1);  ycoord_p = (float *)inputGen.GetLineFloat(1, ycoord[2], tile_height-1);
    xcoord[3] = *xcoord_p;                                                ycoord[3] = *ycoord_p;
    
    tile = inputGen.GetLinesFloat16(tile_width, tile_height, 0, 255);
 
    inputGen.SelectGenerator("uniform");
    out_tileC = inputGen.GetLineFloat16((TILE_SIZE * TILE_SIZE), 0);
    out_tileAsm = inputGen.GetLineFloat16((TILE_SIZE * TILE_SIZE), 0);
    
    mx   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
    my   = inputGen.GetLineFloat16(TILE_SIZE * (TILE_SIZE + PADDING), 0.0);
    
    mvcvWarpMeshExpand(xcoord, ycoord,   mx,   my);
    mvcvWarpMeshSampleFp16bit(tile,   out_tileC, mx, my, tile_width, tile_height);
    warpMeshSampleFp16bit_asm_test(tile, out_tileAsm, mx, my, tile_width, tile_height);
    
    RecordProperty("CyclePerPixel", warpMeshSampleFp16bitCycleCount / (TILE_SIZE * TILE_SIZE));

    arrCheck.CompareArrays(&out_tileC, &out_tileAsm, TILE_SIZE * TILE_SIZE, 1, DELTA);
    
}
