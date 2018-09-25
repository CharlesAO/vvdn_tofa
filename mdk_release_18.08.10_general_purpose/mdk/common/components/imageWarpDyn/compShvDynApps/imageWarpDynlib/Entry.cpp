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
#include "warpMeshSample8bit.h"
#include "imageWarpDefines.h"
#include "imageWarp.h"
#ifdef USE_CMX_DMA_NEW_DRIVER
#include <scCmxDma.h>
#else
#include "swcCdma.h"
#endif
#include "swcFrameTypes.h"
#include "moviVectorUtils.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define NO_DDR_OUTPUT   ((u32)0xFFFFFFFF)
//#define USE_DMA
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


__attribute__((aligned(16))) unsigned char inputBufferMem[CMX_BUFFER_SIZE];
__attribute__((aligned(16))) unsigned char outBufferMem[2][OUT_TILE_SIZE];

#ifdef USE_CMX_DMA_NEW_DRIVER
ScCmxDmaTransaction list[2];
ScCmxDmaTransactionHnd ref[2];
#else
dmaTransactionList_t list[2];
dmaTransactionList_t *ref[2];
#endif

meshPortion meshLUT[MESH_LUT_LENGTH + 1];
u32 tileLutElemCount;
reTileEntry tiles[2];
reTileEntry referenceTile;
__attribute__((aligned(16))) half mx[OUT_TILE_WIDTH * OUT_TILE_WIDTH + 8];
__attribute__((aligned(16))) half my[OUT_TILE_WIDTH * OUT_TILE_WIDTH + 8];
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
static inline  void prepareMesh(meshStruct* mesh, frameSpec* frSpec, tileList* tileNodes)
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

}

static inline void prepareTile(reTileEntry* tile,
                        meshStruct* mesh,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        tileList* coord,
                        unsigned int cmxY)
{
    float4 x,y;
    unsigned int out_width = outputFb->spec.width;
    unsigned int in_width = inputFb->spec.width;
    unsigned int meshW = mesh->meshWidth;
    float *mx = mesh->meshX;
    float *my = mesh->meshY;
    unsigned int tileIndex = coord->y * meshW + coord->x;

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
    float minFloor = (int)(min - __builtin_shave_sau_frac_f32_r(min));
    x -= minFloor;
    tile->dstOffsetDDR = coord->y * OUT_TILE_HEIGHT * out_width + coord->x  * OUT_TILE_WIDTH;
    if (min > in_width || max < 0)
    {
        tile->isInsideImg = 0;
        // insert in the list which will be filled with padding
    }
    else
    {
        tile->isInsideImg = 1;

        adjustBufferPointers(tile, &referenceTile, (unsigned int) minFloor);

        tile->xCoords = mvuConvert_half4(x);
        tile->yCoords = mvuConvert_half4(y);
    }
}

static inline void processTile(reTileEntry* tile,
                        frameBuffer *inputFb,
                        frameBuffer *outputFb,
                        unsigned short paddingvalue,
                        dmaRequesterId dmaId)
{
    UNUSED(outputFb);
    UNUSED(paddingvalue);
    UNUSED(dmaId);

    mvcvWarpMeshExpand_asm((half*)&tile->xCoords, (half*)&tile->yCoords, mx, my);
    mvcvWarpMeshSample8bit_asm(&tile->cmxInBuffP[VERTICAL_PAD], tile->cmxOutBuff, mx, my,
                               inputFb->spec.width + 2 * HORIZONTAL_PAD,
                               CMX_BUFFER_LINES);

}

// scCmxDmaResolveRelAddr is static inline in CMXDMA driver; TODO find a better solution than copying the function here
//
static uint32_t scCmxDmaResolveRelAddr(uint32_t in_addr,
                                              uint32_t shave_number) {
  uint32_t window = 0;
  uint32_t window_base;
  uint32_t *win_reg_ptr =
      (uint32_t *)(SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shave_number) +
                   SLC_TOP_OFFSET_WIN_A);

  uint32_t resolved;
  switch (in_addr >> 24) {
    case 0x1C:
      window = 0;
      break;
    case 0x1D:
      window = 1;
      break;
    case 0x1E:
      window = 2;
      break;
    case 0x1F:
      window = 3;
      break;
    default:
      return (in_addr);
      break;  // absolute address, no translation is to be done
  }
  window_base = win_reg_ptr[window];
  resolved = ((in_addr & 0x00FFFFFF) + window_base);

  return resolved;
}

extern "C" void Entry(meshStruct* mesh,
                      frameBuffer *inputFb,
                      frameBuffer *outputFb,
                      tileList* tileNodes,
                      unsigned short paddingvalue)
{
    unsigned int lineId;
    unsigned char *img = inputFb->p1;
    unsigned char *out_img = outputFb->p1;
    unsigned int out_width = outputFb->spec.width;
    unsigned int out_height = outputFb->spec.height;

    unsigned int in_width = inputFb->spec.width;
    unsigned int in_width_pad = inputFb->spec.width + 2* HORIZONTAL_PAD;

    reTileEntry* currTile, *nextTile, *swapTile;
    static u32 id1;
#ifdef USE_CMX_DMA_NEW_DRIVER
    ScCmxDmaInitialize(NULL);
#else
    id1 = dmaInitRequester(3);
#endif
    assert((CMX_BUFFER_LINES - 1) % 4 == 0);
//    memset(inputBufferMem, paddingvalue, CMX_BUFFER_SIZE);

    currTile = &tiles[0];
    nextTile = &tiles[1];
    initTileBuffers(currTile, inputBufferMem, in_width_pad);
    initTileBuffers(nextTile, inputBufferMem, in_width_pad);
    initTileBuffers(&referenceTile, inputBufferMem, in_width_pad);

    prepareMesh(mesh, &inputFb->spec, tileNodes);
    lineId = 0;

    while (meshLUT[lineId].head == NULL)
        lineId++;
#ifdef USE_CMX_DMA_NEW_DRIVER
    ScCmxDmaCreateStrideTransaction(
                    &ref[0], &list[0], img + lineId * in_width,
                    currTile->cmxInBuffP[VERTICAL_PAD],
                    in_width,
                    in_width,
                    in_width,
                    in_width_pad,
                    in_width * CMX_BUFFER_LINES);
    ScCmxDmaStartTransfer(&ref[0]);
    // create output transaction with dummy addresses. We will fill it later
    ScCmxDmaCreateStrideTransaction(
                    &ref[1], &list[1], img + lineId * in_width, // new CMXDMA driver requires addr!=NULL here, even if addr is updated later
                    out_img + NO_DDR_OUTPUT,
                    OUT_TILE_WIDTH, OUT_TILE_WIDTH,
                    OUT_TILE_WIDTH, inputFb->spec.width,
                    OUT_TILE_WIDTH * OUT_TILE_HEIGHT);
    ScCmxDmaWaitTransaction(&ref[0]);
#else
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
#endif // USE_CMX_DMA_NEW_DRIVER
    u8* dmaSrcAddress = img + in_width * (lineId + CMX_BUFFER_LINES);
#ifdef USE_CMX_DMA_NEW_DRIVER
    ScCmxDmaCreateTransaction(
                    &ref[0], &list[0], dmaSrcAddress,
                    referenceTile.swapP, in_width);
#else
    ref[0] = dmaCreateTransaction(
                    id1, &list[0], dmaSrcAddress,
                    referenceTile.swapP, in_width);
#endif // USE_CMX_DMA_NEW_DRIVER
    currTile->cmxOutBuff = outBufferMem[0];
    nextTile->cmxOutBuff = outBufferMem[1];

    //main loop
    for (; lineId + 1 + CMX_BUFFER_LINES < out_height;lineId++)
    {
        u32 i;
        list[0].src = img + in_width * (lineId + CMX_BUFFER_LINES);
        list[0].dst = (u8 *)scCmxDmaResolveRelAddr((u32)referenceTile.swapP, scGetShaveNumber());
#ifdef USE_CMX_DMA_NEW_DRIVER
        ScCmxDmaStartTransfer(&ref[0]);
#else
        dmaStartListTask(ref[0]);
#endif // USE_CMX_DMA_NEW_DRIVER
        tileList *listP2, * listP = meshLUT[lineId].head;
        list[1].dst = (void*)NO_DDR_OUTPUT;
        if (listP != NULL)
        {
#ifdef USE_CMX_DMA_NEW_DRIVER
            ScCmxDmaWaitTransaction(&ref[0]);
#else
            dmaWaitTask(ref[0]);
#endif
            while (listP != NULL)
            {
                if (list[1].dst != (void*)NO_DDR_OUTPUT)
                {
#ifdef USE_CMX_DMA_NEW_DRIVER
                    ScCmxDmaWaitTransaction(&ref[1]);
#else
                    dmaWaitTask(ref[1]);
#endif // USE_CMX_DMA_NEW_DRIVER
                }
                //prepare tile may reorganize the list
                listP2 = listP;
                listP = listP->next;
                prepareTile(currTile, mesh, inputFb, outputFb, listP2, lineId);
                if (currTile->isInsideImg)
                    processTile(currTile, inputFb, outputFb, paddingvalue, id1);

                u8* dstAddress = outputFb->p1 + currTile->dstOffsetDDR;
                u8* srcAddress = currTile->cmxOutBuff;
                if (currTile->isInsideImg)
                {
#ifdef USE_DMA
                    list[1].src = srcAddress;
                    list[1].dst = dstAddress;
                    ScCmxDmaStartTransfer(&ref[1]);
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
                    list[1].dst = (void*)NO_DDR_OUTPUT;
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
#ifdef USE_CMX_DMA_NEW_DRIVER
            ScCmxDmaWaitTransaction(&ref[0]);
#else
            dmaWaitTask(ref[0]);
#endif // USE_CMX_DMA_NEW_DRIVER
        }
        rotateBufferPointers(&referenceTile);


    }
    int i;
    // padding
    for (; lineId < out_height;lineId++)
    {
        referenceTile.swapP = referenceTile.cmxInBuffP[0];
        tileList* listP = meshLUT[lineId].head;
        while (listP != NULL)
        {
            prepareTile(currTile, mesh, inputFb, outputFb, listP, lineId);
            processTile(currTile, inputFb, outputFb, paddingvalue, id1);
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

