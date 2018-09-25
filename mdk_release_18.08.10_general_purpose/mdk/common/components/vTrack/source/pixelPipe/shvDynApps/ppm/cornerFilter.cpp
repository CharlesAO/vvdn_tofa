#include <string.h>
#include <stdio.h>
#include <new>
#include <stdlib.h>
#include <assert.h>

#include <mv_types.h>
#include "mvMacros.h"
#include "DrvRegUtilsDefines.h"
#include "registersMyriad.h"
#include "pixelPipeCornerShaveHelper.h"
#include "iExtremaLocator.h"
#include "cornerImpl.h"
#include "cornerFilter.h"

#define CORNER_FILTER_START_POINT_X (PADDING_H)    // MA2150 Harris filter handles automatic padding.
                                                   // The first image pixel is the start position for the filter.


#define SIPP_SC_OFFSET 10

#define SIPP_HARRIS_ID     14

CornerFilter::CornerFilter(
        frameSpec spec,
        uint32_t num_input_images,
        u8* harrisBaseAddr,
        u8** buf,
        u8* minMaxObjBuf,
        cornerConfig_t corCfg,
        fifoCommTask_t* _cornerTask,
        u32 cellGridDimension,
        u32 _maxNumFeatures) :
    num_inputImages(num_input_images),
    harrisInputLineBaseAddr(harrisBaseAddr),
    lastLineProcessedCorner(-1),
    maxNumFeatures(_maxNumFeatures)

{
    (void)_cornerTask;
    uint32_t width = spec.width;
    uint32_t height = spec.height;
    paddedWidth = width + (2*PADDING_H);
    u8* curAddr = *buf;
    curAddr = ALIGN_UP(curAddr, 16);
    for(int i = 0; i < N_CORNER_LINE_BUFFS; i++ )
    {
        cornerScoresBuffer[i] = (HARRIS_OUTPUT_TYPE*)curAddr;
        curAddr += ((paddedWidth) * PP_MAX_INPUT_IMAGES) * sizeof(HARRIS_OUTPUT_TYPE) ;
    }
    //TODO: This configuration below is a bit messy
    u8 scaleExponent=0;
    if(sizeof(HARRIS_OUTPUT_TYPE) == 2) {
        scaleExponent=1;
    }
    u8 shiTomasi=0;
    // subtrahend is the value subtracted from the exponent of the floating point to scale to fp16 (see Harris filter document for scaling formula)
    // it is determined empirically by trying to simulate a maximum corner score (eg. corner in T6) without overflow
    // If subtrahend is too large, maxima can be lost as they will be seen as zeros. If it is too small, large corner values will overflow and there
    // could be poor localisation of the maxima
    subtrahend = 0;
    switch(corCfg.alg) {
        case HARRIS:
            shiTomasi = 0;
            if(scaleExponent) {
                subtrahend = 24;
            }
            break;
        case SHI_TOMASI:
            shiTomasi = 1;
            if(scaleExponent){
                subtrahend = 16;
            }
            break;
        default:
            assert(0);
    };

    u8 useSobel = corCfg.useSobel;
    initSippFilter(height, shiTomasi, useSobel, scaleExponent);
    nCells = cellGridDimension * cellGridDimension;
    maxFeatPerCell = maxNumFeatures/nCells;

    // TODO: If we're doing it like this (not giving the application the option of which to use)
    // we can probably change the architecture (don't need inheritance - maybe pimpl?)
#if defined(MA2150) || defined(MA2450)
    extLoc = new (minMaxObjBuf) MvCvFilter(_cornerTask, spec,
                                           num_input_images, maxNumFeatures,
                                           cellGridDimension, corCfg.insertAndSort);
#else
    u16* outputBuff = (u16*)curAddr;
    curAddr += paddedWidth * sizeof(u16) * N_EXTLOC_LINE_BUFFS;
    extLoc = new (minMaxObjBuf) MinMaxHW(&cornerScoresBuffer[0][0], spec,
                                         num_input_images, maxNumFeatures,
                                         cellGridDimension, subtrahend, outputBuff,
                                         corCfg.insertAndSort);
#endif
    *buf = curAddr; //return address after constructor has completed
}

void
CornerFilter::initSippFilter(u32 height, u8 useShiTomasi, u8 useSobel, u8 scaleExponent)
{
#if defined(MA2150) || defined(MA2450)
#define SIPP_IBUF_BASE_OFF        (  (0x000) )
#define SIPP_IBUF_CFG_OFF         (  (0x004) )
#define SIPP_IBUF_LS_OFF          (  (0x008) )
#define SIPP_IBUF_PS_OFF          (  (0x00c) )
#define SIPP_IBUF_IR_OFF          (  (0x010) )
#define SIPP_IBUF_FC_OFF          (  (0x014) )
#define SIPP_IBUF_ICTX_OFF        (  (0x018) )
#define SIPP_OBUF_CFG_OFF         (  (0x004) )
#define SIPP_OBUF_LS_OFF          (  (0x008) )
#define SIPP_OBUF_PS_OFF          (  (0x00c) )
#define SIPP_OBUF_IR_OFF          (  (0x010) )
#define SIPP_OBUF_FC_OFF          (  (0x014) )
#define SIPP_OBUF_OCTX_OFF        (  (0x018) )
   assert(!useShiTomasi); //MA2150 and MA2450 do not support shi tomasi
   harrisInputBaseAdr = SIPP_IBUF14_BASE_ADR;
   harrisOutputBaseAdr = SIPP_OBUF14_BASE_ADR;
   if(!(GET_REG_WORD_VAL(SIPP_CONTROL_ADR) & (1 << SIPP_HARRIS_ID)))
    {
        // Enable SIPP Harris filter. For MA2100 it is a must to do this before the configuration.
        SET_REG_WORD(SIPP_CONTROL_ADR, (GET_REG_WORD_VAL(SIPP_CONTROL_ADR) | (1 << SIPP_HARRIS_ID)));
    }
    harrisInterruptStatusAdr = SIPP_INT1_STATUS_ADR;
    harrisInterruptClearAdr = SIPP_INT1_CLEAR_ADR;
    harrisInterruptMask = 1 << SIPP_HARRIS_ID;


#else
   harrisInputBaseAdr = SIPP_HARRIS_IBUF0_BASE_ADR;
   harrisOutputBaseAdr = SIPP_HARRIS_OBUF0_BASE_ADR;
    if (!GET_REG_WORD_VAL(SIPP_HARRIS_ENABLE_ADR))
    {
        SET_REG_WORD(SIPP_HARRIS_ENABLE_ADR, 1);
    }
    harrisInterruptStatusAdr = SIPP_HARRIS_INT_STATUS_ADR;
    harrisInterruptClearAdr = SIPP_HARRIS_INT_CLEAR_ADR;
    harrisInterruptMask = 1 << 1; //OBFL_INC mask

#endif
    //Harris specific params
    SET_REG_WORD(SIPP_HARRIS_FRM_DIM_ADR, ((height + 7 * HARRIS_PADDING) << 16) | ((paddedWidth) * PP_MAX_INPUT_IMAGES));
    //TODO: subtracting 24(HARRIS) or 16(SHI_TOMASI) (subtrahend) from exponent before conversion of harris output to fp16.
    //      need to determine a more sensible value
    SET_REG_WORD(SIPP_HARRIS_CFG_ADR, HARRIS_KERNEL_SIZE | (useShiTomasi << 7) | ((useSobel==0) << 6) | ((subtrahend*scaleExponent) << 8));
    fp32 harris_k = 0.04f;
    SET_REG_WORD(SIPP_HARRIS_K_ADR, *(u32*) &harris_k);

    //Program I buffers
    SET_REG_WORD(harrisInputBaseAdr,  harrisInputLineBaseAddr);
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_CFG_OFF,  0x10000000 | N_HARRIS_LINE_BUFFS);
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_LS_OFF,   (paddedWidth) * PP_MAX_INPUT_IMAGES);
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_PS_OFF,   (paddedWidth) * PP_MAX_INPUT_IMAGES * (height + 2 * HARRIS_PADDING));
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_IR_OFF,   0x00000000);
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_FC_OFF,   0x00000000 | (1 << 31));
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_ICTX_OFF,       0x00000000 | (1 << 31));

    //Program O buffers
    SET_REG_WORD(harrisOutputBaseAdr,  &cornerScoresBuffer[0][0]);
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_CFG_OFF,   N_CORNER_LINE_BUFFS | sizeof(HARRIS_OUTPUT_TYPE) << 28);
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_LS_OFF,    (paddedWidth) * PP_MAX_INPUT_IMAGES * sizeof(HARRIS_OUTPUT_TYPE));          // Line stride
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_PS_OFF,    (paddedWidth) * PP_MAX_INPUT_IMAGES * sizeof(HARRIS_OUTPUT_TYPE) * height); // Plane stride
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_IR_OFF,    0x00000000);                // Interrupt rate
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 31));    // Fill control
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_OCTX_OFF,       0x00000000 | (1 << 31));

}

void
CornerFilter::startFrame(tvFeatureCell** _detectedFeatures) {
    for (int i = 0; i < N_CORNER_LINE_BUFFS; i++)
        cornerScoresPointers[i] = cornerScoresBuffer[i];
    waitHarrisAndCornerFilterCalled=false;
    postprocessedFirstResults=false;
    lastLineProcessedCorner = -1;
    clearFeatures(_detectedFeatures);
}

void
CornerFilter::initExtLoc(fp32* _thresholds, tvFeatureCell** _detectedFeatures) {
    extLoc->initFrame(_detectedFeatures, _thresholds);
}

void
CornerFilter::startTask(u32 line) {
    callHarrisAndCornerFilter(line);
    lastLineProcessedCorner = line;
}

void
CornerFilter::postprocessLastResults() {
    extLoc->extractResultsFromPreviousCall();
    // The first time we call postprocessLastResults, we do not want to release the first harris line
    // this is because we will still need the first harris line for the second run
    if(postprocessedFirstResults) {
        SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 30) );
    } else {
        postprocessedFirstResults = true;
    }
}

void
CornerFilter::waitTask() {
    waitHarrisAndCornerFilter();
}
void
CornerFilter::endFrame() {
    int i;
    for(i=0; i<3; i++) {
        HARRIS_OUTPUT_TYPE* line_ptrs[3] = {&cornerScoresPointers[0][CORNER_FILTER_START_POINT_X], &cornerScoresPointers[1][CORNER_FILTER_START_POINT_X], &cornerScoresPointers[2][CORNER_FILTER_START_POINT_X]};
        extLoc->callFilter(substractCornerLineOffset(lastLineProcessedCorner + i + 1), line_ptrs);
        ROTATE_BUFFER(cornerScoresPointers, N_CORNER_LINE_BUFFS);
        extLoc->extractResultsFromPreviousCall();
        SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 30) );
        extLoc->waitFilter();

    }
    extLoc->endFrame();
    ROTATE_BUFFER(cornerScoresPointers, N_CORNER_LINE_BUFFS);

    // we just consume the last 2 lines of the filter to leave it clean
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 30) );
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 30) );
    SET_REG_WORD(harrisOutputBaseAdr + SIPP_OBUF_FC_OFF,    0x00000000 | (1 << 30) );


}

void
CornerFilter::callHarrisAndCornerFilter(u32 lineNo) {
    HARRIS_OUTPUT_TYPE* line_ptrs[3];
    if(lastLineProcessedCorner == -1) {
        line_ptrs[0] = &cornerScoresPointers[0][CORNER_FILTER_START_POINT_X];
        line_ptrs[1] = &cornerScoresPointers[0][CORNER_FILTER_START_POINT_X];
        line_ptrs[2] = &cornerScoresPointers[1][CORNER_FILTER_START_POINT_X];
    } else {
        line_ptrs[0] = &cornerScoresPointers[0][CORNER_FILTER_START_POINT_X];
        line_ptrs[1] = &cornerScoresPointers[1][CORNER_FILTER_START_POINT_X];
        line_ptrs[2] = &cornerScoresPointers[2][CORNER_FILTER_START_POINT_X];
    }
    extLoc->callFilter(substractCornerLineOffset(lineNo), line_ptrs);
    callHarrisFilter();
}

void
CornerFilter::waitHarrisAndCornerFilter() {
    waitHarrisFilter();
    if(waitHarrisAndCornerFilterCalled) {
        ROTATE_BUFFER(cornerScoresPointers, N_CORNER_LINE_BUFFS);
    } else {
        waitHarrisAndCornerFilterCalled = true;
    }
    extLoc->waitFilter();
}
// Starts the hardware Harris filter.
// The filter input is: pointed by harrisBufferPointers
// and the output is saved to: cornerScoresBuffer (configured in initSippFilter())
void
CornerFilter::callHarrisFilter()
{
    SET_REG_WORD(harrisInputBaseAdr + SIPP_IBUF_FC_OFF,    0x00000000 | (1 << 30) );
}

// Waits for the hardware Harris filter to complete.
void
CornerFilter::waitHarrisFilter()
{
    u32 status;

    do
    {
       status = GET_REG_WORD_VAL(harrisInterruptStatusAdr);
    }
    while((status & harrisInterruptMask) != (u32)harrisInterruptMask);
    SET_REG_WORD(harrisInterruptClearAdr, harrisInterruptMask);
}


void
CornerFilter::clearFeatures (tvFeatureCell** features)
{
    for (u8 i = 0; i < num_inputImages; i++) {
        for (u8 j = 0; j < nCells; j++) {
            features[i][j].featureCount = 0;
            memset(features[i][j].features, 0x00, sizeof(t_vTrackHarrisFeat) * maxFeatPerCell);
        }
    }
}

u32
CornerFilter::substractCornerLineOffset(u32 processedImageLineNo)
{
   // When the 5th line is processed, we have 3 Harris lines
   // so we can start the Corner filtering but
   // the results are for line 1
# define CORNER_START_LINE_OFFSET ((HARRIS_KERNEL_SIZE-1)/2+2)

    assert(processedImageLineNo >= CORNER_START_LINE_OFFSET);
    return (processedImageLineNo - CORNER_START_LINE_OFFSET);
}

