///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Wrapper over HW or C Model used for MEST API.
///
/// This is the API to the wrapper
///

#ifndef MOTEST_WRAPPER_H
#define MOTEST_WRAPPER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    int xpel;
    int ypel;
} POS;

typedef struct
{
    char ImgFname[2][256];
    char ListFname[256];
    int SkipChroma;
    int NumFrames;
    int NumImg;
    int ImgMode;
    int SrchMode;
    int OutMode;
    int Width;
    int Height;
    bool DisHpel;
    bool DisQpel;
    int ShiftX;
    int ShiftY;
    int Noise;
    int IncX;
    int IncY;
    int log_level;
    int tv_enable;
    char tv_dir[128];
    int NumRand;
    int NumSrch;
    int RasterStep;
    int BlkWidth;
    int BlkHeight;
    int SadThresh;
    bool DoHlpf;
    bool DoVlpf;
    POS srch[8];
    int tv_start;
    int tv_end;
} ARGS;

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Run MotEst C model
 * @param [in]  args  - arguments structure for MotEst C model
 * @param [out] res   - results buffer, format [cost: u16, dx: s8, dy: s8] or [dx: s8, dy: s8]
 *                      depending on args.OutMode
 * @param [in]  resSz - results buffer size, it is known before processing
 * @return
 *                      0 for success
 *                      MotEst C model error status
 */
int runMotestWrapper(ARGS args, uint32_t *res, uint32_t resSz);

/**
 * Get source (previous) frame buffer pointer
 * @return pointer to source buffer
 */
void* getSrcFramePtr();

/**
 * Get reference (current) frame buffer pointer
 * @return pointer to reference buffer
 */
void* getRefFramePtr();

/**
 * Get results buffer pointer
 * @return pointer to results buffer
 */
uint32_t* getResPtr();

#ifdef __cplusplus
}
#endif

#endif //MOTEST_WRAPPER_H
