///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt

#ifndef __SIPPWARP_H__
#define __SIPPWARP_H__

#include <sipp.h>

//#define BUILD_WARP_OUT

// Test specific macros
//#define SIPP_CV_OPIPE_BUFFER_WIDTH    1280//1280
//#define SIPP_CV_OPIPE_BUFFER_HEIGHT   720
//720
//#define SIPP_CV_OPIPE_BUFFER_SIZE     (SIPP_CV_OPIPE_BUFFER_WIDTH * SIPP_CV_OPIPE_BUFFER_HEIGHT)
#define SIPP_CV_OPIPE_NUM_INPUTS      2

///////////////////////////////////////////////////
// Warp setup
#define SIPP_CV_WARP_NUM_LINES        24
//#define SIPP_CV_WARP_LNB_SIZE    (SIPP_CV_OPIPE_BUFFER_WIDTH * SIPP_CV_WARP_NUM_LINES * 2)
///////////////////////////////////////////////////
// Stereo setup

// Test specific macros
//#define SIPP_CV_STEREO_BUFFER_WIDTH          SIPP_CV_OPIPE_BUFFER_WIDTH
//#define SIPP_CV_STEREO_BUFFER_HEIGHT         SIPP_CV_OPIPE_BUFFER_HEIGHT
#define SIPP_CV_STEREO_DISP                  64

//#define SIPP_CV_OPIPE_DEBUG_BUFFER_SIZE      (SIPP_CV_OPIPE_BUFFER_WIDTH * SIPP_CV_OPIPE_BUFFER_HEIGHT * (SIPP_CV_STEREO_DISP >> 0x3))

#define SIPP_CV_STEREO_0_NUM_LINES    4
#define SIPP_CV_STEREO_1_NUM_LINES    24

//#define SIPP_CV_STEREO_0_LNB_SIZE     (SIPP_CV_OPIPE_BUFFER_WIDTH * SIPP_CV_STEREO_DISP * SIPP_CV_STEREO_0_NUM_LINES)
//#define SIPP_CV_STEREO_1_LNB_SIZE     (SIPP_CV_OPIPE_BUFFER_WIDTH * SIPP_CV_STEREO_1_NUM_LINES)



typedef struct CvOPipe
{
    SippPipeline *pl_wsgbm;
    SippFilter * warp;
    SippFilter * stereo;
    SippFilter * dmaOutDisp;
#ifdef BUILD_WARP_OUT
	SippFilter * dmaOutWarp;
#endif
	SippFilter * subpixel;
    SippFilter * disp2depth;
    SippFilter * dmaOutDepth;

} StereoPipe;

void readStereoPipeInput (StereoPipe *pPl);
void writeStereoPipeOutput (StereoPipe *pPl);

void buildStereoPipe (StereoPipe *pPl);
void configStereoPipe (StereoPipe *pPl);
void deleteStereoPipe (StereoPipe * pPl);
#endif
