///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Image warp component
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


#include <svuCommonShave.h>
#include "warpMeshExpand.h"
#include "warpMeshSample8bitStride.h"
#include "warpMeshSample8bit.h"
#include "imageWarpDefines.h"
#include "imageWarp.h"

#include "swcCdma.h"
#include "swcFrameTypes.h"
#include "moviVectorUtils.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define NO_DDR_OUTPUT   ((u32)0xFFFFFFFF)

#ifdef IMAGEWARP_USE_DMA_ENGINE
#define USE_DMA
#endif

#ifndef IMAGEWARP_DMA_AGENT
#define IMAGEWARP_DMA_AGENT (2)
#endif

typedef struct meshPortion
{
    tileList* head;
}meshPortion;
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Sections decoration is required here for downstream tool


// 4: Static Local Data
// ----------------------------------------------------------------------------
// Buffers
#define MAX_TILES_PER_BLOCK 4

 __attribute__((aligned(16))) unsigned char inputBufferMem[CMX_BUFFER_SIZE];
 __attribute__((aligned(16))) unsigned char outBufferMem[2][OUT_TILE_SIZE * MAX_TILES_PER_BLOCK];

CMX_DMA dmaTransactionList_t list[2];
CMX_DMA dmaTransactionList_t *ref[2];

meshPortion meshLUT[MESH_LUT_LENGTH + 1];
u32 tileLutElemCount;
__attribute__((aligned(16))) reTileEntry tiles[2];
__attribute__((aligned(16))) reTileEntry referenceTile;
__attribute__((aligned(16))) half mxExpand[OUT_TILE_WIDTH * OUT_TILE_WIDTH + 8];
// +8 is a workaround for the meshSample kernel.
__attribute__((aligned(16))) half myExpand[OUT_TILE_WIDTH * OUT_TILE_WIDTH + 8];


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static inline float min4Elem(float4 vec)
{
    float xmin;
    xmin = __builtin_shave_cmu_min_f32_rr_float(vec[0], vec[1]);
    xmin = __builtin_shave_cmu_min_f32_rr_float(xmin, vec[2]);
    return __builtin_shave_cmu_min_f32_rr_float(xmin, vec[3]);
}

static inline float max4Elem(float4 vec)
{
    float xmax;
    xmax = __builtin_shave_cmu_max_f32_rr_float(vec[0], vec[1]);
    xmax = __builtin_shave_cmu_max_f32_rr_float(xmax, vec[2]);
    return __builtin_shave_cmu_max_f32_rr_float(xmax, vec[3]);
}

static void initTileBuffers(reTileEntry* currTile, u8* buffer, u32 lineLength)
{
    u32 ix;
    for(ix = 0; ix < VERTICAL_PAD; ix++)
    {
        currTile->cmxInBuffP[ix] = (u8*)buffer + CMX_BUFFER_LINES * lineLength + HORIZONTAL_PAD;
        currTile->cmxInBuffP[ix + CMX_BUFFER_LINES + VERTICAL_PAD] = (u8*)buffer + CMX_BUFFER_LINES * lineLength + HORIZONTAL_PAD;
    }
    for(ix = 0; ix < CMX_BUFFER_LINES; ix++)
    {
        currTile->cmxInBuffP[ix + VERTICAL_PAD] = (u8*)buffer + ix * lineLength + HORIZONTAL_PAD;
    }
    currTile->swapP = (u8*)buffer + (CMX_BUFFER_LINES + 1) * lineLength + HORIZONTAL_PAD;

}
static inline void rotateBufferPointers(reTileEntry* currTile)
{
    // TODO: optimize
    u32 i;
    u8* swapP = currTile->cmxInBuffP[VERTICAL_PAD];

    for(i = 0; i < CMX_BUFFER_LINES - 1 ; i++)
    {
        currTile->cmxInBuffP[i + VERTICAL_PAD] = currTile->cmxInBuffP[i + VERTICAL_PAD + 1];
    }
    currTile->cmxInBuffP[CMX_BUFFER_LINES + VERTICAL_PAD - 1] = currTile->swapP;
    currTile->swapP = swapP;
}

static inline void adjustBufferPointers(reTileEntry* currTile, reTileEntry* refTile, int amount)
{
    // TODO: optimize
    u32 i;
    uint4* lineP = (uint4*)&currTile->cmxInBuffP[VERTICAL_PAD];
    uint4* lineP2 = (uint4*)&refTile->cmxInBuffP[VERTICAL_PAD];

    for(i = 0; i < CMX_BUFFER_LINES >> 2 ; i++)
    {
        lineP[i] = lineP2[i] + (uint4)amount;
    }
    currTile->cmxInBuffP[CMX_BUFFER_LINES + VERTICAL_PAD - 1] =
                    refTile->cmxInBuffP[CMX_BUFFER_LINES + VERTICAL_PAD - 1] + amount;
}

fp32 dbgAvgNumMergedTiles = 0;
u32 dbgExitCond = 0;

static inline void prepareMesh_withoutp(meshStruct* mesh, frameSpec* frSpec, tileList* tileNodes)
{

    unsigned int i, j, k;
    unsigned int meshH = mesh->meshHeight;
    unsigned int meshW = mesh->meshWidth;
    float *my = mesh->meshY;
    float *mx = mesh->meshX;

    float minThrX = (float)mesh->coord_min_x;
    float maxThrX = (float)mesh->coord_max_x;
  

    for (j = 0; j < frSpec->height; j++ )
    {
        meshLUT[j].head = NULL;
    }

    meshLUT[MESH_LUT_LENGTH].head = NULL;
    tileLutElemCount = 0;
    u32 mergedTilesSum = 0;
    u32 superTileCount = 0;

    for (i = 0; i < (meshH - 1); i++ )
    {
        u32 mergedTilesPerBlock = 0;
       // tileList* blockLeaderTile = NULL;
        int leaderYPos = 0;
        int leaderYPosMax = 0;
        int valid = 0;

        for (j = 0; j < meshW - 3; j += 3 )
        {
            u32 ind = i * meshW + j;
            float4* first = (float4*)&my[ind];
            float4* second = (float4*)&my[ind + meshW];
            float4 min = __builtin_shave_cmu_min_f32_rr_float4(first[0], second[0]);
            float4 max = __builtin_shave_cmu_max_f32_rr_float4(first[0], second[0]);

            float4 min2 = __builtin_shave_cmu_alignvec_rri_float4(min, min, 4);
            float4 max2 = __builtin_shave_cmu_alignvec_rri_float4(max, max, 4);

            min = __builtin_shave_cmu_min_f32_rr_float4(min, min2);
            max = __builtin_shave_cmu_max_f32_rr_float4(max, max2);
            int4 minInt = mvuConvert_int4(min);
            int4 minIntPositive = __builtin_shave_cmu_clamp0_i32_rr_int4(minInt, minInt);
            
            int4 maxInt = mvuConvert_int4(max);
            int4 diff = maxInt - minInt;
            for(k = 0; k < 3; k++)
            {
                float4 xVect;
                xVect[0] = mx[ind + k];
                xVect[1] = mx[ind + k + 1];
                xVect[2] = mx[ind + meshW + k];
                xVect[3] = mx[ind + meshW + k + 1];
                float minValX = min4Elem(xVect);
                float maxValX = max4Elem(xVect);
                if ((diff[k] <= VERTICAL_PAD) && (minInt[k] < (int)frSpec->height)
                                && (maxInt[k] >= 0) && (minInt[k] > - VERTICAL_PAD) &&
                                (minValX >= minThrX-HORIZONTAL_PAD) && (maxValX <= maxThrX+HORIZONTAL_PAD))
                {
                    valid = 1;
                    int tmpMin = __builtin_shave_cmu_min_i32_rr_int(leaderYPos, minIntPositive[k]);
                    int tmpMax = __builtin_shave_cmu_max_i32_rr_int(leaderYPosMax, maxInt[k]);
                                             
                    if (mergedTilesPerBlock == 0 || tmpMax - tmpMin >= CMX_BUFFER_LINES - 1 || mergedTilesPerBlock >= MAX_TILES_PER_BLOCK)
                    {
                        if(mergedTilesPerBlock)
                        {
                            tileNodes[tileLutElemCount - mergedTilesPerBlock].next = meshLUT[leaderYPos].head;
                            meshLUT[leaderYPos].head = &tileNodes[tileLutElemCount - 1];
                        }
                        mergedTilesSum += mergedTilesPerBlock;
                        superTileCount++;
                        mergedTilesPerBlock = 1;
                        leaderYPos = minIntPositive[k];
                        leaderYPosMax = maxInt[k];
                    }
                    else
                    {
                        mergedTilesPerBlock++;
                        leaderYPos = tmpMin;
                        leaderYPosMax = tmpMax;
                    }
                    tileNodes[tileLutElemCount].next = &tileNodes[tileLutElemCount - 1];
                    tileNodes[tileLutElemCount].x = j + k;
                    tileNodes[tileLutElemCount].y = i;
                    tileNodes[tileLutElemCount].tilesPerBlock = mergedTilesPerBlock;
                    tileLutElemCount++;
                }
                else
                {
                    valid = 0;
                }
            }
        }
        /// Working the last elements
        if( j < meshW - 1)
        {
            u32 ind = i * meshW + j;
            float4* first = (float4*)&my[ind];
            float4* second = (float4*)&my[ind + meshW];
            float4 min = __builtin_shave_cmu_min_f32_rr_float4(first[0], second[0]);
            float4 max = __builtin_shave_cmu_max_f32_rr_float4(first[0], second[0]);

            float4 min2 = __builtin_shave_cmu_alignvec_rri_float4(min, min, 4);
            float4 max2 = __builtin_shave_cmu_alignvec_rri_float4(max, max, 4);

            min = __builtin_shave_cmu_min_f32_rr_float4(min, min2);
            max = __builtin_shave_cmu_max_f32_rr_float4(max, max2);
            int4 minInt = mvuConvert_int4(min);
            int4 minIntPositive = __builtin_shave_cmu_clamp0_i32_rr_int4(minInt, minInt);
            int4 maxInt = mvuConvert_int4(max);
            int4 diff = maxInt - minInt;
            for(k = 0; ((j+k) < (meshW -1)); k++)
            {
                float4 xVect;
                xVect[0] = mx[ind + k];
                xVect[1] = mx[ind + k + 1];
                xVect[2] = mx[ind + meshW + k];
                xVect[3] = mx[ind + meshW + k + 1];
                float minValX = min4Elem(xVect);
                float maxValX = max4Elem(xVect);
                if ((diff[k] <= CMX_BUFFER_LINES) && (minInt[k] < (int)frSpec->height)
                        && (maxInt[k] >= 0) && (minInt[k] > - VERTICAL_PAD) &&
                        (minValX >= minThrX) && (maxValX <= maxThrX))
                {
                    valid = 1;
                    int tmpMin = __builtin_shave_cmu_min_i32_rr_int(leaderYPos, minIntPositive[k]);
                    int tmpMax = __builtin_shave_cmu_max_i32_rr_int(leaderYPosMax, maxInt[k]);

                    if (mergedTilesPerBlock == 0 || tmpMax - tmpMin >= CMX_BUFFER_LINES || mergedTilesPerBlock >= MAX_TILES_PER_BLOCK)
                    {
                        if(mergedTilesPerBlock)
                        {
                            tileNodes[tileLutElemCount - mergedTilesPerBlock].next = meshLUT[leaderYPos].head;
                            meshLUT[leaderYPos].head = &tileNodes[tileLutElemCount - 1];
                        }
                        mergedTilesSum += mergedTilesPerBlock;
                        superTileCount++;
                        mergedTilesPerBlock = 1;
                        leaderYPos = minIntPositive[k];
                        leaderYPosMax = maxInt[k];
                    }
                    else
                    {
                        mergedTilesPerBlock++;
                        leaderYPos = tmpMin;
                        leaderYPosMax = tmpMax;
                    }
                    tileNodes[tileLutElemCount].next = &tileNodes[tileLutElemCount - 1];
                    tileNodes[tileLutElemCount].x = j + k;
                    tileNodes[tileLutElemCount].y = i;
                    tileNodes[tileLutElemCount].tilesPerBlock = mergedTilesPerBlock;
                    tileLutElemCount++;
                }
                else
                {
                    valid = 0;
                }
            }
        }
        if(valid || (mergedTilesPerBlock >= 1))
        {
            tileNodes[tileLutElemCount - mergedTilesPerBlock].next = meshLUT[leaderYPos].head;
            meshLUT[leaderYPos].head = &tileNodes[tileLutElemCount - 1];
        }
    }
    dbgAvgNumMergedTiles = (1.0*mergedTilesSum) / superTileCount;
  }

void prepareMesh_withp(meshStruct* mesh, frameSpec* frSpec, tileList* tileNodes)
{
    unsigned int i, j, k;
    unsigned int meshH = mesh->meshHeight;
    unsigned int meshW = mesh->meshWidth;
    float *my = mesh->meshY;
 for (j = 0; j < frSpec->height; j++ )
    {
        meshLUT[j].head = NULL;
    }
    meshLUT[MESH_LUT_LENGTH].head = NULL;
    tileLutElemCount = 0;
   for (i = 0; i < (meshH - 1); i++ )
    {
        // Process all cells horizontally. We are doing vectorized processing here, we process 3 elements in an iteration
        // We use elements j, j+1, j+2, j+3
        for (j = 0; j < meshW - 3; j += 3 )
        {
            u32 ind = i * meshW + j;
            float4* first = (float4*)&my[ind];
            float4* second = (float4*)&my[ind + meshW];
            float4 min = __builtin_shave_cmu_min_f32_rr_float4(first[0], second[0]);
            float4 max = __builtin_shave_cmu_max_f32_rr_float4(first[0], second[0]);

            float4 min2 = __builtin_shave_cmu_alignvec_rri_float4(min, min, 4);
            float4 max2 = __builtin_shave_cmu_alignvec_rri_float4(max, max, 4);

            min = __builtin_shave_cmu_min_f32_rr_float4(min, min2);
            max = __builtin_shave_cmu_max_f32_rr_float4(max, max2);
            int4 minInt = mvuConvert_int4(min);
            int4 maxInt = mvuConvert_int4(max);
            int4 diff = maxInt - minInt;
            for(k = 0; k < 3; k++)
            {
                if ((diff[k] <= CMX_BUFFER_LINES) && (minInt[k] < (int)frSpec->height)
                                && (maxInt[k] >= 0) && (minInt[k] > - VERTICAL_PAD))
                {
                    if (minInt[k] > 0)
                    {
                        tileNodes[tileLutElemCount].next = meshLUT[minInt[k]].head;
                        tileNodes[tileLutElemCount].x = j + k;
                        tileNodes[tileLutElemCount].y = i;
                        meshLUT[minInt[k]].head = &tileNodes[tileLutElemCount++];
                    }
                    else
                    {
                        tileNodes[tileLutElemCount].next = meshLUT[0].head;
                        tileNodes[tileLutElemCount].x = j + k;
                        tileNodes[tileLutElemCount].y = i;
                        meshLUT[0].head = &tileNodes[tileLutElemCount++];
                    }
                }
                else
                {
                    tileNodes[tileLutElemCount].next = meshLUT[MESH_LUT_LENGTH].head;
                    tileNodes[tileLutElemCount].x = j + k;
                    tileNodes[tileLutElemCount].y = i;
                    meshLUT[MESH_LUT_LENGTH].head = &tileNodes[tileLutElemCount++];
                }
            }

        }
     }
        // In case the mesh width was not multiple of 3, we need to process the last elements
        u32 ind = i * meshW + j;
        float4* first = (float4*)&my[ind];
        float4* second = (float4*)&my[ind + meshW];
        float4 min = __builtin_shave_cmu_min_f32_rr_float4(first[0], second[0]);
        float4 max = __builtin_shave_cmu_max_f32_rr_float4(first[0], second[0]);

        float4 min2 = __builtin_shave_cmu_alignvec_rri_float4(min, min, 4);
        float4 max2 = __builtin_shave_cmu_alignvec_rri_float4(max, max, 4);

        min = __builtin_shave_cmu_min_f32_rr_float4(min, min2);
        max = __builtin_shave_cmu_max_f32_rr_float4(max, max2);
        int4 minInt = mvuConvert_int4(min);
        int4 maxInt = mvuConvert_int4(max);
        int4 diff = maxInt - minInt;
        // Processing last cells.
        for(k = 0; j + k < meshW - 1; k++)
        {
            if ((diff[k] <= CMX_BUFFER_LINES) && (minInt[k] < (int)frSpec->height)
                            && (maxInt[k] >= 0) && (minInt[k] > - VERTICAL_PAD))
            {
                if (minInt[k] > 0)
                {
                    tileNodes[tileLutElemCount].next = meshLUT[minInt[k]].head;
                    tileNodes[tileLutElemCount].x = j + k;
                    tileNodes[tileLutElemCount].y = i;
                    meshLUT[minInt[k]].head = &tileNodes[tileLutElemCount++];
                }
                else
                {
                    tileNodes[tileLutElemCount].next = meshLUT[0].head;
                    tileNodes[tileLutElemCount].x = j + k;
                    tileNodes[tileLutElemCount].y = i;
                    meshLUT[0].head = &tileNodes[tileLutElemCount++];
                }
            }
            else
            {
                tileNodes[tileLutElemCount].next = meshLUT[MESH_LUT_LENGTH].head;
                tileNodes[tileLutElemCount].x = j + k;
                tileNodes[tileLutElemCount].y = i;
                meshLUT[MESH_LUT_LENGTH].head = &tileNodes[tileLutElemCount++];
            }
        }
    }


static inline void prepareTile_withoutp(reTileEntry* tile,
                        meshStruct* mesh,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        tileList* coord,
                        unsigned int cmxY)
{
    float4 x,y;
    unsigned int out_width = outputFb->spec.width;
    

    unsigned int meshW = mesh->meshWidth;
    float *mx = mesh->meshX;
    float *my = mesh->meshY;
    unsigned int tileIndex = coord->y * meshW + coord->x;

    float x_offset = (float)mesh->coord_min_x;
    
    y[0] = my[tileIndex] - cmxY;
    y[1] = my[tileIndex + 1] - cmxY;
    y[2] = my[tileIndex + meshW] - cmxY;
    y[3] = my[tileIndex + meshW + 1] - cmxY;
    x[0] = mx[tileIndex];
    x[1] = mx[tileIndex + 1];
    x[2] = mx[tileIndex + meshW];
    x[3] = mx[tileIndex + meshW + 1];

    float min = min4Elem(x);
    float max = max4Elem(x);

	if (min < 0)
		min = 0;
    float minFloor = (int)(min - __builtin_shave_sau_frac_f32_r(min));
    x -= minFloor;

    tile->dstOffsetDDR = coord->y * OUT_TILE_HEIGHT * out_width + coord->x  * OUT_TILE_WIDTH;

    adjustBufferPointers(tile, &referenceTile, (unsigned int) minFloor - x_offset);

    tile->xCoords = mvuConvert_half4(x);
    tile->yCoords = mvuConvert_half4(y);
	UNUSED(max);
        UNUSED(inputFb);
}


static inline void prepareTile_withp(reTileEntry* tile,
                        meshStruct* mesh,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        tileList* coord,
                        unsigned int cmxY)
{

   unsigned int in_width = inputFb->spec.width;
   unsigned int out_width = outputFb->spec.width;

    UNUSED(outputFb);
    unsigned int meshW = mesh->meshWidth;
    float *mx = mesh->meshX;
    float *my = mesh->meshY;
    unsigned int tileIndex = coord->y * meshW + coord->x;
 float4 x,y;
    float x_offset = (float)mesh->coord_min_x;
    x[0] = mx[tileIndex];
    x[1] = mx[tileIndex + 1];
    x[2] = mx[tileIndex + meshW];
    x[3] = mx[tileIndex + meshW + 1];
    y[0] = my[tileIndex] - cmxY;
    y[1] = my[tileIndex + 1] - cmxY;
    y[2] = my[tileIndex + meshW] - cmxY;
    y[3] = my[tileIndex + meshW + 1] - cmxY;
  

    float min = min4Elem(x);
    float max = max4Elem(x);

	if (min < 0)
		min = 0;
    float minFloor = (int)(min - __builtin_shave_sau_frac_f32_r(min));
    x -= minFloor;

    tile->dstOffsetDDR = coord->y * OUT_TILE_HEIGHT * out_width + coord->x  * OUT_TILE_WIDTH;
    adjustBufferPointers(tile, &referenceTile, (unsigned int) minFloor - x_offset);
if (min > in_width || max < 0)
    {
        tile->isInsideImg = 0;
        // insert in the list which will be filled with padding
    }
    else
    {
        tile->isInsideImg = 1;

        adjustBufferPointers(tile, &referenceTile, (unsigned int) minFloor);
    }
    
    tile->xCoords = mvuConvert_half4(x);
    tile->yCoords = mvuConvert_half4(y);
}





static inline void processTile_withoutp(reTileEntry* tile,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        unsigned short paddingvalue,
                        uint32_t stride,
                        dmaRequesterId dmaId)
{
    UNUSED(inputFb);
    UNUSED(outputFb);
    UNUSED(paddingvalue);
    UNUSED(dmaId);
    mvcvWarpMeshExpand_asm((half*)&tile->xCoords, (half*)&tile->yCoords, mxExpand, myExpand);
    mvcvWarpMeshSample8bitStride_asm(&tile->cmxInBuffP[VERTICAL_PAD], tile->cmxOutBuff, mxExpand, myExpand,
                               stride,
                               CMX_BUFFER_LINES);
}

static inline void processTile_withp(reTileEntry* tile,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        unsigned short paddingvalue,
                        dmaRequesterId dmaId)
{
    UNUSED(outputFb);
    UNUSED(paddingvalue);
    UNUSED(dmaId);

    mvcvWarpMeshExpand_asm((half*)&tile->xCoords, (half*)&tile->yCoords, mxExpand, myExpand);
    mvcvWarpMeshSample8bit_asm(&tile->cmxInBuffP[VERTICAL_PAD], tile->cmxOutBuff, mxExpand, myExpand,
                               inputFb->spec.width + 2 * HORIZONTAL_PAD,
                               CMX_BUFFER_LINES);

}


extern "C" void imageWarp(meshStruct* mesh,
                           frameBuffer *inputFb,
                    	   frameBuffer *outputFb,
                           tileList* tileNodes,
                           unsigned short paddingvalue,
                           uint32_t nr_shaves_vsplit,
                           uint32_t proc_shave_idx)
{
    UNUSED(nr_shaves_vsplit);
    UNUSED(proc_shave_idx);
    unsigned int lineId = 0;
    unsigned int out_width = outputFb->spec.width;
    unsigned int out_height = outputFb->spec.height;
    unsigned short padding;
    int i,j,choice=0;
    for (i=0;i<(int)mesh->meshHeight;i++)
    {
    	for(j=0;j<(int)mesh->meshWidth;j++)
    	{
    		if((mesh->meshX[i*mesh->meshWidth+j]>inputFb->spec.width) ||( mesh->meshX[i*mesh->meshWidth+j]<0))
    		{
    			choice=1;
    			i=mesh->meshHeight;
    			j=mesh->meshWidth;
    		}
    		if((mesh->meshY[i*mesh->meshWidth+j]>inputFb->spec.height) ||( mesh->meshY[i*mesh->meshWidth+j]<0))
    		{
    		    choice=1;
    		    i=mesh->meshHeight;
    		    j=mesh->meshWidth;
    	    }
    	}
    }
    padding=choice;

   // printf("padding:  %d\n",(int)padding);
    // workaround for meshExpand kernel. Ensure that the values after myExpand are 0
if(!padding){
	    while(lineId < 8)
	    {
		*(int*)&(myExpand[OUT_TILE_WIDTH * OUT_TILE_WIDTH + lineId])= 0;
		lineId+=2;
	    }
	 
	    unsigned int in_height = mesh->coord_max_y;
	    UNUSED(out_height);
	    unsigned int in_dma_size = mesh->coord_max_x - mesh->coord_min_x;
	    unsigned int in_stride = inputFb->spec.width;

	    unsigned int in_width_pad = in_dma_size + 2* HORIZONTAL_PAD;
	    memset(inputBufferMem, paddingvalue, CMX_BUFFER_SIZE);
	    unsigned char *img = inputFb->p1;
	    unsigned char *out_img = outputFb->p1;
	    reTileEntry* currTile, *swapTile;
		  UNUSED(swapTile);
	    
	    static dmaRequesterId id1 = dmaInitRequesterWithAgent(3, IMAGEWARP_DMA_AGENT);

		  assert((CMX_BUFFER_LINES - 1) % 4 == 0);
		  memset(inputBufferMem, paddingvalue, CMX_BUFFER_SIZE);

          currTile = &tiles[0];

	    initTileBuffers(currTile, inputBufferMem, in_width_pad);
	    initTileBuffers(&referenceTile, inputBufferMem, in_width_pad);

	    prepareMesh_withoutp(mesh, &inputFb->spec, tileNodes);
	    lineId = mesh->coord_min_y;

      while (meshLUT[lineId].head == NULL)
          lineId++;

 ref[0] = dmaCreateTransactionFullOptions(
                    id1, &list[0], img + mesh->coord_min_x +  lineId * in_stride,
                    currTile->cmxInBuffP[VERTICAL_PAD],
                    in_dma_size * CMX_BUFFER_LINES,
                    in_dma_size,
                    in_dma_size,
                    in_stride,
                    in_width_pad);
    dmaStartListTask(ref[0]);

    // create output transaction with dummy addresses. We will fill it later
    ref[1] = dmaCreateTransactionFullOptions(
                    id1, &list[1], NULL,
                    out_img + NO_DDR_OUTPUT, OUT_TILE_WIDTH * OUT_TILE_HEIGHT,
                    OUT_TILE_WIDTH, OUT_TILE_WIDTH, OUT_TILE_WIDTH,
                    out_width);
    dmaWaitTask(ref[0]);
    u8* dmaSrcAddress = img + mesh->coord_min_x + in_stride * (lineId + CMX_BUFFER_LINES);
    ref[0] = dmaCreateTransaction(
                    id1, &list[0], dmaSrcAddress,
                    referenceTile.swapP, in_dma_size);

    currTile->cmxOutBuff = outBufferMem[0];
    u32 processTogether;
    //main loop
    for (; lineId + 1 < in_height; lineId++)
    {
        u32 i;
		UNUSED(i);
        ref[0]->src = img + mesh->coord_min_x + in_stride * (lineId + CMX_BUFFER_LINES);
        ref[0]->dst = referenceTile.swapP;
        ref[0]->linkAddress = NULL;
		if (!(lineId >= in_height - CMX_BUFFER_LINES )) dmaStartListTask(ref[0]);
		u32 cmxY;
		cmxY = (lineId >= in_height - CMX_BUFFER_LINES ) ? (in_height - CMX_BUFFER_LINES ) : lineId;
        tileList *listP2, * listP = meshLUT[lineId].head;
        ref[1]->dst = (void*)NO_DDR_OUTPUT;
        if (listP != NULL)
        {
            if (!(lineId >= in_height - CMX_BUFFER_LINES )) dmaWaitTask(ref[0]);
            while (listP != NULL)
            {
               // u32 processTogether;
                processTogether = listP->tilesPerBlock;
                u32 tileID = 0;
                currTile->cmxOutBuff += OUT_TILE_WIDTH * processTogether ;
                for (tileID = 0; tileID < processTogether; tileID++)
                {
                    //prepare tile may reorganize the list
                    listP2 = listP;
                    listP = listP->next;
                    prepareTile_withoutp(currTile, mesh, inputFb, outputFb, listP2,cmxY);

                    currTile->cmxOutBuff -= OUT_TILE_WIDTH;
                    processTile_withoutp(currTile, inputFb, outputFb,paddingvalue,OUT_TILE_WIDTH * processTogether, id1);
                }
                u8* dstAddress = outputFb->p1 + currTile->dstOffsetDDR;
                u8* srcAddress = currTile->cmxOutBuff;
#ifdef USE_DMA
                ref[1]->src = srcAddress;
                ref[1]->dst = dstAddress;
                ref[1]->length = OUT_TILE_SIZE * processTogether;
                ref[1]->src_width = OUT_TILE_WIDTH * processTogether;
                ref[1]->src_stride = OUT_TILE_WIDTH * processTogether;
                ref[1]->dst_width = OUT_TILE_WIDTH * processTogether;
                ref[1]->linkAddress = NULL;
                dmaStartListTask(ref[1]);
                dmaWaitTask(ref[1]);

#else
                for(i = 0; i < OUT_TILE_HEIGHT; ++i)
                {
                    memcpy(dstAddress, srcAddress, OUT_TILE_WIDTH * processTogether);
                    dstAddress += out_width;
                    srcAddress += OUT_TILE_WIDTH * processTogether;
                }
#endif
            }
        }
        else
        {
            if (!(lineId >= in_height - CMX_BUFFER_LINES )) dmaWaitTask(ref[0]);
        }
        if (!(lineId >= in_height - CMX_BUFFER_LINES )) rotateBufferPointers(&referenceTile);
		
    }

}
else {
#undef  USE_DMA
	 unsigned char *img = inputFb->p1;
	 unsigned char *out_img = outputFb->p1;
	 unsigned int in_width = inputFb->spec.width;
	 unsigned int in_width_pad = inputFb->spec.width + 2* HORIZONTAL_PAD;

	 reTileEntry* currTile, *nextTile, *swapTile;
	 static dmaRequesterId id1 = dmaInitRequester(3);
	   assert((CMX_BUFFER_LINES - 1) % 4 == 0);
	memset(inputBufferMem, paddingvalue, CMX_BUFFER_SIZE);
        currTile = &tiles[0];

     nextTile = &tiles[1];
     initTileBuffers(currTile, inputBufferMem, in_width_pad);
     initTileBuffers(nextTile, inputBufferMem, in_width_pad);
     initTileBuffers(&referenceTile, inputBufferMem, in_width_pad);
     prepareMesh_withp(mesh, &inputFb->spec, tileNodes);
     lineId = 0;
   while (meshLUT[lineId].head == NULL)
      lineId++;

ref[0] = dmaCreateTransactionFullOptions(
                    id1, &list[0], img + lineId * in_width,
                    currTile->cmxInBuffP[VERTICAL_PAD],
                    in_width * CMX_BUFFER_LINES,
                    in_width,
                    in_width,
                    in_width,
                    in_width_pad);
    dmaStartListTask(ref[0]);
    // create output transaction with dummy addresses. We will fill it later
    ref[1] = dmaCreateTransactionFullOptions(
                    id1, &list[1], NULL,
                    out_img + NO_DDR_OUTPUT, OUT_TILE_WIDTH * OUT_TILE_HEIGHT,
                    OUT_TILE_WIDTH, OUT_TILE_WIDTH, OUT_TILE_WIDTH,
                    inputFb->spec.width);
    dmaWaitTask(ref[0]);
    u8* dmaSrcAddress = img + in_width * (lineId + CMX_BUFFER_LINES);
    ref[0] = dmaCreateTransaction(
                    id1, &list[0], dmaSrcAddress,
                    referenceTile.swapP, in_width);

    currTile->cmxOutBuff = outBufferMem[0];
    nextTile->cmxOutBuff = outBufferMem[1];

    //main loop
    for (; lineId + 1 + CMX_BUFFER_LINES < out_height;lineId++)
    {
        u32 i;
        ref[0]->src = img + in_width * (lineId + CMX_BUFFER_LINES);
        ref[0]->dst = (u8 *)dmaSolveRelAddr((u32)referenceTile.swapP, scGetShaveNumber());
        dmaStartListTask(ref[0]);
        tileList *listP2, * listP = meshLUT[lineId].head;
        ref[1]->dst = (void*)NO_DDR_OUTPUT;
        if (listP != NULL)
        {
            dmaWaitTask(ref[0]);
            while (listP != NULL)
            {
                if (ref[1]->dst != (void*)NO_DDR_OUTPUT)
                {
                    dmaWaitTask(ref[1]);
                }
                //prepare tile may reorganize the list
                listP2 = listP;
                listP = listP->next;
                prepareTile_withp(currTile, mesh, inputFb, outputFb, listP2,lineId);
                if (currTile->isInsideImg)
                    processTile_withp(currTile, inputFb, outputFb,paddingvalue,id1);

                u8* dstAddress = outputFb->p1 + currTile->dstOffsetDDR;
                u8* srcAddress = currTile->cmxOutBuff;
                if (currTile->isInsideImg)
                {
#ifdef USE_DMA
                    ref[1]->src = srcAddress;
                    ref[1]->dst = dstAddress;
                    dmaStartListTask(ref[1]);
#else
                    for(i = 0; i < OUT_TILE_HEIGHT; ++i)
                    {
                        memcpy(dstAddress, srcAddress, OUT_TILE_WIDTH);
                        dstAddress += inputFb->spec.width;
                        srcAddress += OUT_TILE_WIDTH;
                    }
#endif
                }
                else
                {
                    for(i = 0; i < OUT_TILE_HEIGHT; ++i)
                    {
                        memset(dstAddress, paddingvalue, OUT_TILE_WIDTH);
                        dstAddress += inputFb->spec.width;
                    }
                    ref[1]->dst = (void*)NO_DDR_OUTPUT;
                }
                swapTile = currTile;
                currTile = nextTile;
                nextTile = swapTile;
            }
#ifdef USE_DMA
            if (nextTile->dstOffsetDDR != NO_DDR_OUTPUT)
                dmaWaitTask(ref[1]);
#endif
        }
        else
        {
            dmaWaitTask(ref[0]);
        }
        rotateBufferPointers(&referenceTile);
    }
   int i; 
    for (; lineId < out_height;lineId++)
    {
        referenceTile.swapP = referenceTile.cmxInBuffP[0];
        tileList* listP = meshLUT[lineId].head;
        while (listP != NULL)
        {
            prepareTile_withp(currTile, mesh, inputFb, outputFb, listP, lineId);
            processTile_withp(currTile, inputFb, outputFb, paddingvalue,id1);
            u8* dstAddress = outputFb->p1 + currTile->dstOffsetDDR;
            u8* srcAddress = currTile->cmxOutBuff;
            if (currTile->dstOffsetDDR != NO_DDR_OUTPUT)
                for(i = 0; i < OUT_TILE_HEIGHT; ++i)
                {
                    memcpy(dstAddress, srcAddress, OUT_TILE_WIDTH);
                    dstAddress += inputFb->spec.width;
                    srcAddress += OUT_TILE_WIDTH;
                }
            listP = listP->next;
        }
        rotateBufferPointers(&referenceTile);
    }

    // fill out the tiles which are out of image
    tileList* listP = meshLUT[MESH_LUT_LENGTH].head;
    while (listP != NULL)
    {
        u8* dstAddress = out_img + listP->y * OUT_TILE_HEIGHT * out_width + listP->x  * OUT_TILE_WIDTH;
        for(i = 0; i < OUT_TILE_HEIGHT; ++i)
        {
            memset(dstAddress, paddingvalue, OUT_TILE_WIDTH);
            dstAddress += inputFb->spec.width;
        }
        listP = listP->next;
    }

 }

 SHAVE_HALT;
}

