#ifndef VPIPE_TYPES_H
#define VPIPE_TYPES_H

#include "vTrack.h"
#include "global_constants.h"



struct frameSpecs;
typedef struct frameSpecs frameSpec;

enum t_vPipeMode
{
    VPIPE_MODE_PP = 0, /* Only run pixelPipe                   */
    VPIPE_MODE_PP_FM = 1, /* Only run pixelPipe + featureManager  */
    VPIPE_MODE_PP_FM_OF = 2, /* Run full pipeline                   */
    VPIPE_MODE_DUMMY = 3, /* Generate dummy features and add them to the output */
    VPIPE_MODE_GYRO_ASSIST_TEST = 4, /* Test gyro assist feature*/
    VPIPE_MODE_LAST = 5
};

typedef struct
{
    float lostFeatureTreshold;
    float minimumHarrisResponseOldFeatures;
    float minimumFeatureDist;
} FMFeatureThresholds_t;

struct t_vPipeParams{
    // If the harris score of a pixel is bigger than this value, pixelPipe will add it to the candidate features list
    // Zero means automatic, any other value is manual fixed threshold
    float harrisThresholds[MAX_NUM_CELLS];
    // If tracking error is greater than this then don't track any more
    float lostFeatureError;
    // If Harris threshold of tracked feature is less than this then don't track any more
    float minHarrisResponseOldFeatures;
    // Minimum distance between tracked features
    float minDistanceSquared;
    // In case of automatic mode, these constants specify the speed of harris threshold changes
    float thresholdDecreaseVelocity;
    float thresholdIncreaseVelocity;
    // The harris threshold will be between these values
    float thresholdMin;
    float thresholdMax;
};


struct t_vPipeShaveConfig
{
    uint32_t gaussNoShaves;
    uint32_t* gaussShaveList;

    uint32_t cornerNoShaves;
    uint32_t* cornerShaveList;

    uint32_t ppShaveNum;

    uint32_t ofNoShaves;
    uint32_t* ofShaveList;

    uint32_t  fmShaveNum;
};

struct VpipeBuffer {

    VpipeBuffer() : ptr(NULL), size(0) {}
    uint8_t* ptr;
    uint32_t size;
    
    template <size_t N>
    void operator=(uint8_t (&buf)[N]) {
        ptr = buf;
        size = sizeof(buf);
    }
};


struct t_vpMemCfg
{
    VpipeBuffer vpCmxHeap;
    VpipeBuffer vpDdrHeap;
    VpipeBuffer vpCmxBuffs;
    VpipeBuffer vpPpBuffs;
    VpipeBuffer vpPpCmxBuffs;
    VpipeBuffer vpMestBuffs;
};

struct t_vpCacheCfg
{
    uint8_t pp_part_data;
    uint8_t pp_part_instr;
    uint8_t of_part_data;
    uint8_t of_part_instr;
    uint8_t fm_part_data;
    uint8_t fm_part_instr;
};

struct t_vpFifoCfg
{
    uint8_t fifo1;
    uint8_t fifo2;
    uint8_t fifo3;
    uint8_t fifo4;
};

struct t_vPipeResourceCfg
{
    t_vPipeShaveConfig* vp_shaveCfg;
    t_vpMemCfg vp_memCfg;
    t_vpCacheCfg vp_cacheCfg;
    t_vpFifoCfg vp_fifoCfg;
};

//properties of the camera
struct t_camProperties
{
    //degree of FOV sensor
    float fov;
    //settings for camera center on x and y axis
    float camCenterX;
    float camCenterY;
};

enum ePixelPipeMode {
    DEFAULT
};

enum vPipeOutputType {
    TRACKED,
    PP_PLUS_TRACKED,
    PP_ONLY
};
//Structure to store all the settings for the camera, window and algorithm.
//These will be have some default values set by vPipeInitDefaultAlgoCfg(
struct t_vPipeAlgoCfg
{
    //general vTrack cfg
    frameSpec* inputFrameSpec;
    vPipeOutputType outputType;
    
    //pixelPipe cfg
    uint32_t numPyrLevels;
    float harrisThrInitValue;
    pyramidAlgoType_t pyrCfg;
    cornerConfig_t corCfg;
    uint32_t cellGridDimension;
    uint32_t targetNumFeatures;
    uint32_t maxNumFeatures;
    
    //gyroAssist cfg
    t_camProperties cam;

    //opticalFlow cfg
    uint32_t windowWidth; //patch sizing
    uint32_t windowHeight; //patch sizing
    uint32_t maxOfIterations ;
    float epsilon;
    ofAlgoType_t ofCfg;
};

// Debug interface -------------------------
enum t_vPipeCapt
{
    CaptNone = 0,
    CaptPPdata = 1,
    CaptFMdata = 2,
    CaptOFdata = 3
};

//VGA
#define VGA_DDR_HEAP_SIZE (((800 * 1024) + 4096) * PP_MAX_INPUT_IMAGES) //space required for lower level pyramids in VGA mode (+4096 for alignment)
#define VGA_CMX_HEAP_SIZE (60352* PP_MAX_INPUT_IMAGES) // space required for higher level pyramids in VGA mode NOTE: must be multiple of 64
#define VGA_PP_BUFS_SIZE (26176)   // pixelPipe buffer mempool in VGA mode

//P720
#define P720_DDR_HEAP_SIZE (2719744* PP_MAX_INPUT_IMAGES)  //space required for lower level pyramids in 720p mode
#define P720_CMX_HEAP_SIZE (47104* PP_MAX_INPUT_IMAGES) // space required for higher level pyramids in 720p mode
#define P720_PP_BUFS_SIZE (41600)  // pixelPipe buffer mempool in 720p mode

#define PP_CMX_BUFS_SIZE (64 + (1 + PP_MAX_INPUT_IMAGES) * 128) //64:cache line alignment, 128: rounded up size of ShDrvCmxDmaTransaction

#define MEST_BUFS_SIZE (20032)

#define CMX_INTERNAL_POOL_FEATURE_INDEPENDENT_SIZE ((sizeof(uint32_t)*3*MAX_NUM_CELLS) + \
(8*2*MAX_NUM_CELLS) /*sizeof(tvFeatureCell)*/ + \
(12*2*MAX_NUM_CELLS) /*sizeof(t_vTrackHarrisFeat*/ + \
(sizeof(float)*MAX_NUM_CELLS) + \
(sizeof(FMFeatureThresholds_t)) + \
(64*(18+(MAX_NUM_CELLS*2)))) 
#define CMX_INTERNAL_POOL_FEATURE_DEPENDENT_SIZE ((sizeof(tvXYLoc) * 4) + \
(sizeof(float)*4) + \
(sizeof(FeatureMetadata)*2) + \
(12*2))

                                                    
template <size_t N>
struct t_vpCmxInternals {
    uint8_t buf[CMX_INTERNAL_POOL_FEATURE_INDEPENDENT_SIZE +
                (CMX_INTERNAL_POOL_FEATURE_DEPENDENT_SIZE*N)
                ];
};

typedef uint8_t t_vpVgaCmxHeap[VGA_CMX_HEAP_SIZE] ALIGNED(64);  // the user should declare these
typedef uint8_t t_vp720CmxHeap[P720_CMX_HEAP_SIZE] ALIGNED(64); // based on the resolution they are running
typedef uint8_t t_vpVgaDdrHeap[VGA_DDR_HEAP_SIZE] ALIGNED(4096);  // the 2 resolution modes require different
typedef uint8_t t_vp720DdrHeap[P720_DDR_HEAP_SIZE] ALIGNED(4096); // memory pools that are declared in the application
typedef uint8_t t_vpVgaPpBuffs[VGA_PP_BUFS_SIZE] ALIGNED(64);   // and passed as parameters at initialisation
typedef uint8_t t_vpPpCmxBuffs[PP_CMX_BUFS_SIZE] ALIGNED(64);   // and passed as parameters at initialisation
typedef uint8_t t_vpMestBuffs[MEST_BUFS_SIZE] ALIGNED(64);   // and passed as parameters at initialisation
typedef uint8_t t_vp720PpBuffs[P720_PP_BUFS_SIZE] ALIGNED(64);


#endif
