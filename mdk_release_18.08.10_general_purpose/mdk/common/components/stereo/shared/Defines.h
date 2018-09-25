#ifndef __DEFINES_SPARSE_H__
#define __DEFINES_SPARSE_H__

#define TILE_BASED_APPROACH

#ifndef DISP
#define DISP    128
#endif

#include <mv_types.h>
typedef half disparityType; // intermediate operations are done in fp16

//#define PC_VIEWER_FLAG
#ifdef PC_VIEWER_FLAG
typedef unsigned char outputType; // for T3
#else
typedef unsigned short outputType; // for T4
#endif

#define V4

#ifdef V4 // V4 pyr
 typedef unsigned char qvgaType; // qvga disparity map is u8
 #define OUT_SCALE_FACTOR    1
#else // V5 nonpyr
 #ifdef PC_VIEWER_FLAG
  typedef unsigned char qvgaType; // qvga disparity map sent to the PC is u8
 #else
  typedef unsigned short qvgaType; // qvga disparity map is u16
 #endif
 #define OUT_SCALE_FACTOR    2
#endif

// Maximum allowed configuration
#define NB_SHAVES_THREAD_0  1
#define SHAVE_THREAD_0      5

#define MAX_SHAVES          5
#define MAX_PYR_LEVELS      2
#define MAX_DISP            128
#define MAX_WIDTH           640
#define MAX_HEIGHT          480
#define STEREO_FRAMES       2

// User configuration
#define SHAVES_USED     5
#ifdef __PC__
#define SGBM_SHAVES_USED     1
#else
#define SGBM_SHAVES_USED     5
#endif
#define PYR_LEVELS      2 // number of pyramid levels
#define LEFT_MATCHING   0 // matching is done with DISP pixels to the left

// Fixed parameters
#define SGBM_MUTEX              13
#define SGBM_CROSSING           1 // 1 = left-right check; 2 = left-right and right-left check
#define CFG_IDX_FOR_RIGHT_LEFT  4 // first index for right-left crossing
#define CENSUS_KERNEL_SIZE      7 // census size kernel
#define LINE_PADDING            (CENSUS_KERNEL_SIZE/2) // line padding for census kernel
#define WTA_WINDOW              3
#define COST_WINDOW             (WTA_WINDOW + 2)
#define EXTRA_PIXELS_ADPYR      (COST_WINDOW - 1)

#define DEFAULT_CONFIDENCE_VAL  190 // Confidence threshold default value
#define SGBM_NUMBER_OF_PATHS    5 // number of aggregation paths for sgbm

// level 1: VGA
#define MAX_DISPARITIES_LEVEL1      DISP // disparities for VGA pyramid level
#define MAX_CENSUS_PYR_OFFSET       25 // according to CensusMatchingPyrOnePos kernel

// level 0: QVGA
#define SCALE_FACTOR                2 // scale factor for next pyramid level
#define MAX_DISPARITIES_LEVEL0      (DISP/SCALE_FACTOR) // disparities for QVGA pyramid level

// Padding
#define MAX_PADDING_8       7 // maximum padding needed to align each line to 8 bytes
#define MAX_PADDING_16      15 // maximum padding needed to align each line to 16 bytes
#define ALIGNMENT_PADDING   16 // maximum padding needed to align each pointer to 16 bytes due to ALIGN_UP(x,16)

// SGBM aggregation paths buffer sizes - SGBM aggregation is done at QVGA level, where the patch size is MAX_PATCH_WIDTH/SCALE_FACTOR
#define AGGREGATION_PATH_BUF_SIZE    ((MAX_WIDTH / SCALE_FACTOR + 2) * MAX_DISPARITIES_LEVEL0 + \
                                	   + ALIGNMENT_PADDING)

#ifdef __PC__
#define scGetShaveNumber(x) 0
#define absoluteDataAddr(x) x
#define ALIGNED(x)
#define ALIGN_UP(x, a) (x)
#define DDR_AREA
#define BSS
#define CMX_NOCACHE
#define CMX_DMA_AREA
#define SHAVE_HALT
#else
#include <mvMacros.h>
// Attributes and sections
#define ALIGNED(x) __attribute__((aligned(x)))
#define DDR_AREA __attribute__((section(".ddr.bss")))
#define BSS __attribute__((section(".bss")))
#define CMX_NOCACHE __attribute__((section(".cmx_direct.data")))
#define CMX_DMA_AREA __attribute__((section(".cmx.cdmaDescriptors")))
#endif

// Debug printfs
#ifdef PROFILING
#define PROF_PRINT          printf
#define SHAVE_PROF_PRINT    swcShaveProfPrint
#else
#define PROF_PRINT(...)
#define SHAVE_PROF_PRINT(...)
#endif


#define CHECK(call) \
        { \
            int ret; \
            if ((ret = call) != OS_MYR_DRV_SUCCESS) \
                printf("Error: %s line %d failed with code %d\n", __FUNCTION__, __LINE__, ret); \
        }

#endif

