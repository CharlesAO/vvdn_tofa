///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#ifndef __PIXEL_PIPE_API_H__
#define __PIXEL_PIPE_API_H__
#include "mv_types.h"
#ifndef __PC__
#include "theDynContext.h"
#endif
#include <swcFrameTypes.h> //frameBuffer
#include "vTrackPrivateTypes.h" //tvFeatureCell
#include "pixelPipeHelper.h" //pixelPipeParams_t

#define NUM_SHAVES_SLAVE 10  //max number of shaves for gauss/corner filter

typedef u32 swcShaveUnit_t;

struct fifoCommMasterHandler_t;
typedef struct fifoCommMasterHandler_t fifoCommMasterHandler_t;

struct fifoCommSlaveHandler_t;
typedef struct fifoCommSlaveHandler_t fifoCommSlaveHandler_t;

struct fifoCommTask_t;
typedef struct fifoCommTask_t fifoCommTask_t;

struct t_pPipeShaveConfig {
    u32 gaussNoShaves;
    swcShaveUnit_t* gaussShaveList;
    u32 cornerNoShaves;
    swcShaveUnit_t* cornerShaveList;
    swcShaveUnit_t* ppShaveNum;
};

struct ppThresholds_t
{
    fp32 thresholdDecreaseVelocity;
    fp32 thresholdIncreaseVelocity;
    fp32 thresholdMin;
    fp32 thresholdMax;
};

struct t_ppFifoCfg
{
    u8 fifo1;
    u8 fifo2;
    u8 fifo3;
    u8 fifo4;
};

struct t_pPipeResourceCfg {
    t_pPipeShaveConfig* shaveConfig;
    u8*                 ppBuffs;
    u32                 ppBuffsSize;
    u8*                 ppCmxBuffs;
    u32                 ppCmxBuffsSize;
    u8                  ppCacheData;
    u8                  ppCacheInstr;
    t_ppFifoCfg         fifoCfg;
};

void UpdateCellThresholds(fp32* thresholds, tvFeatureCell* featureCells,
       ppThresholds_t* thresholdCfg, u32 nCells, u32 targetNumFeatures);


class PixelPipe {

public:
    PixelPipe();
    void ppInit(t_pPipeResourceCfg* vpResource, pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg);

    u32 ppRun(frameBuffer* in_img, tvFeatureCell** feature_cells,
              tvPyramidBuffer* frameBuffer,
              fp32* thresholds,
              u32 num_pyr_levels, u32 num_pyrs,
              u32 cellGridDimension, u32 maxNumFeatures,
              u32 targetNumFeatures, 
              ppThresholds_t* thresholdCfg);
private:
    void initPixelPipe(t_pPipeResourceCfg* vpResource, pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg);

    u32 pixelPipe(frameBuffer* in_img, tvFeatureCell** feature_cells,
              tvPyramidBuffer* frameBuffer,
              fp32* thresholds,
              u32 num_pyr_levels, u32 num_pyrs,
              u32 cellGridDimension, u32 maxNumFeatures,
              u32 targetNumFeatures, ppThresholds_t* thresholdCfg);


    // ptrs to objects in application layer
    swcShaveUnit_t *gaussShavesList;
    swcShaveUnit_t *cornerShavesList;

    //shave config
    swcShaveUnit_t ppMasterShaveNum;
    swcShaveUnit_t slaveShaves[NUM_SHAVES_SLAVE];

    u32 slaveNumShaves;
    u32 gaussNumShaves;
    u32 cornerNumShaves;
    // config
    pixelPipeParams_t*       pixelPipeParams;
    u8                      ppCacheData;
    u8                      ppCacheInstr;
    u8 shaveBuf[1088] __attribute__((aligned(64))); //buffer to save shave class instance and config params TODO: MOVe
    float                   harrisThresholds[MAX_NUM_CELLS] __attribute__((aligned(64)));

    // handlers etc
    fifoCommMasterHandler_t*    masterHandler;
    fifoCommSlaveHandler_t*     slaveHandler;
    fifoCommTask_t*             shaveTaskTypes;
    fifoCommTask_t*             taskTypes;

#ifndef __PC__

            DynamicContext_t ppContext ALIGNED(64);
            DynamicContextInstances_elm ppContextInstanceData ALIGNED(64);

            DynamicContextInstancesPtr ppContextInstanceDataPtr ALIGNED(64);
            DynamicContext_t corContext ALIGNED(64);
            DynamicContextInstances_elm corContextInstanceData ALIGNED(64);

            DynamicContextInstancesPtr corContextInstanceDataPtr ALIGNED(64);
            DynamicContext_t gaussContext ALIGNED(64);
            DynamicContextInstances_elm gaussContextInstanceData ALIGNED(64);

            DynamicContextInstancesPtr gaussContextInstanceDataPtr ALIGNED(64);
#endif


} ALIGNED(64);
#endif //__PIXELPIPE_API_H__



