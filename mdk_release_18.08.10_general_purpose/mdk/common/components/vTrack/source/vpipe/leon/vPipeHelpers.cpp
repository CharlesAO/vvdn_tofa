///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
/// For License Warranty see: common/license.txt
///
/// @brief Vtrack main pipeline helper functions
///
#include <assert.h>
#include "string.h"
#include <mv_types.h>
#include "vTrack.h"
#include "vPipeHelpers.h"
#include "swcFrameTypes.h"
#include <stdio.h>
#include "vPipePublicTypes.h"
#include "vPipeImpl.h"

#define XCOMPILABLE_ALIGN_UP(x, a)   ((typeof(x))(((uint64_t)(x) + a - 1) & (~(a-1))))
// we allocate stuff on the ddr heap in one go here
    void
VpipeImpl::initialiseHeap(frameSpec * input_frame_spec,
        tvPyramidBuffer * pyramid,
        const u32 num_pyr,
        const u32 num_pyr_levels,
        const u32 num_pyr_levels_cmx,
        frameBuffer pyrImages[][MAX_PYRAMID_LEVELS],
        VpipeBuffer& heapDDR,VpipeBuffer& heapCMX)
{
    u32 pyr;            // pyramid buffer iterator
    u32 lvl;            // level buffer iterator
    u8 *ddr_heap;            // DDR heap
    u8 *cmx_heap;            // CMX heap

    assert(num_pyr_levels <= MAX_PYRAMID_LEVELS);
    // re-initialise heap here.  Eventually we'll need to call this multiple times.
    ddr_heap = XCOMPILABLE_ALIGN_UP(heapDDR.ptr, 64);
    cmx_heap = XCOMPILABLE_ALIGN_UP(heapCMX.ptr, 64);

    // Allocate pyramids on ddr & CMX
    assert((uint64_t)ddr_heap % 0x1000 == 0);
    // Allocated pyramid buffers on DDR heap
    for (pyr = 0; pyr < num_pyr; pyr++)
    {
        for (lvl = 0; lvl < num_pyr_levels; lvl++)
        {
            frameBuffer *my_frame = &(pyrImages[pyr][lvl]);
            my_frame->spec.type = YUV400p;
            my_frame->spec.height = (input_frame_spec->height >> lvl);
            my_frame->spec.width = (input_frame_spec->width >> lvl);
            my_frame->spec.bytesPP = 1;
            my_frame->spec.stride = my_frame->spec.width
                * my_frame->spec.bytesPP + 2 * PADDING_H;
            if(num_pyr_levels > 1)
            {
                if (lvl < num_pyr_levels - num_pyr_levels_cmx)
                {
                    my_frame->p1 = ddr_heap + (PADDING_V * my_frame->spec.stride);
                    // increment heap pointer by correct amount and maintain 4k alignment
                    ddr_heap += ((my_frame->spec.height + 2 * PADDING_V) * (my_frame->spec.width
                                * my_frame->spec.bytesPP
                                + 2 * PADDING_H));
                    ddr_heap += 0x0FFF;
                    uint64_t tmp = (uint64_t)ddr_heap%0x1000;
                    ddr_heap -= tmp;
                }
                else
                {
                    my_frame->p1 = cmx_heap + (PADDING_V * my_frame->spec.stride);
                    // increment heap pointer by correct amount and maintain 64B alignment
                    cmx_heap += XCOMPILABLE_ALIGN_UP((my_frame->spec.height + 2 * PADDING_V) * (my_frame->spec.width
                                * my_frame->spec.bytesPP
                                + 2 * PADDING_H), 64);
                }
            }
        }

        pyramid[pyr].pyrLevel = pyrImages[pyr];
        pyramid[pyr].numPyramidLevels = num_pyr_levels;
    }


    //TODO:add error codes?
    if( ((uint64_t)cmx_heap- (uint64_t)heapCMX.ptr)  > heapCMX.size )
    {
        printf("VPIPE INTERNAL ERROR: NOT ENOUGH CMX HEAP! need %d\n",(int) (cmx_heap - heapCMX.ptr));
        assert(0);
    }
    if(((uint64_t)ddr_heap - (uint64_t)heapDDR.ptr)  > heapDDR.size)
    {
        printf("VPIPE INTERNAL ERROR: NOT ENOUGH DDR HEAP! need %d\n",(int) (ddr_heap - heapDDR.ptr));
        assert(0);
    }
}

void
VpipeImpl::allocateInternalBuffers(VpipeBuffer& bufSpace, u16 max_features)
{
    u8* curAddr;
    curAddr = bufSpace.ptr;

    memset((void*)curAddr, 0, bufSpace.size);

    curAddr = XCOMPILABLE_ALIGN_UP(curAddr, 64);
    features[0] = (tvXYLoc*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(tvXYLoc), 64);
    features[1] = (tvXYLoc*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(tvXYLoc), 64);

    featuresTmp = (tvXYLoc*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(tvXYLoc), 64);
    featuresInputOF = (tvXYLoc*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(tvXYLoc), 64);


    featuresError[0] = (fp32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(fp32), 64);
    featuresError[1] = (fp32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(fp32), 64);
    featuresErrorTmp[0] = (fp32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(fp32), 64);
    featuresErrorTmp[1] = (fp32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(fp32), 64);

    featureMetadata[0] = (FeatureMetadata*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(FeatureMetadata), 64);
    featureMetadata[1] = (FeatureMetadata*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(max_features * sizeof(FeatureMetadata), 64);

    cellFeatureCount[0] = (u32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(u32), 64);
    cellFeatureCount[1] = (u32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(u32), 64);
    featureIdCounter = (u32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(u32), 64);

    featureCells[0] = (tvFeatureCell*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(tvFeatureCell), 64);
    featureCells[1] = (tvFeatureCell*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(tvFeatureCell), 64);
    u32 maxFeatPerCell = max_features/nCells;
    for(u32 i=0; i<nCells; i++) {
        featureCells[0][i].features=(t_vTrackHarrisFeat*)curAddr;
        curAddr += XCOMPILABLE_ALIGN_UP((maxFeatPerCell + 1)* sizeof(t_vTrackHarrisFeat), 64);  // +1 for smartInsert function
        featureCells[1][i].features=(t_vTrackHarrisFeat*)curAddr;
        curAddr += XCOMPILABLE_ALIGN_UP((maxFeatPerCell + 1)* sizeof(t_vTrackHarrisFeat), 64);  // +1 for smartInsert function
    }
    featThresholds = (FMFeatureThresholds_t*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(sizeof(FMFeatureThresholds_t), 64);
    
    // Threshold buffer on cmx
    harrisThresholds = (fp32*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(nCells * sizeof(fp32), 64);

//    TODO:add error codes?
    if(((uint64_t)curAddr - (uint64_t)bufSpace.ptr) > bufSpace.size)
    {
        printf("VPIPE INTERNAL ERROR: NOT ENOUGH SIZE FOR INTERNAL BUFFERS need: %d currently have: %d\n",(int) (curAddr - bufSpace.ptr), (int)bufSpace.size);
        assert(0);
    }
}
#if defined(MA2150) || defined(MA2450)
void
VpipeImpl::allocateMestBuffers(VpipeBuffer& bufSpace, u16 max_features)
{
    (void)bufSpace;
    (void)max_features;
}
#else
#include "MestApi.h"
void
VpipeImpl::allocateMestBuffers(VpipeBuffer& bufSpace, u16 max_features)
{
    u8* curAddr;
    curAddr = bufSpace.ptr;

    curAddr = XCOMPILABLE_ALIGN_UP(curAddr, 128);

    of.MEST_COORDS_HEAP = (uint8_t*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(MEST_GET_COORDS_HEAP_BYTES(max_features), 128);

    of.MEST_RES_TMP_HEAP = (uint8_t*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(MEST_GET_RES_HEAP_BYTES(max_features), 128);

    of.MEST_RES_HEAP = (uint8_t*)curAddr;
    curAddr += XCOMPILABLE_ALIGN_UP(MEST_GET_RES_BYTES(max_features), 128);

    if(((uint64_t)curAddr - (uint64_t)bufSpace.ptr) > bufSpace.size)
    {
        printf("VPIPE INTERNAL ERROR: NOT ENOUGH SIZE FOR INTERNAL BUFFERS need: %d currently have: %d\n",(int) (curAddr - bufSpace.ptr), (int)bufSpace.size);
        assert(0);
    }
}
#endif
