
#ifndef STEREO_CFG_H
#include <OsDrvStereoDefines.h>
#include <Pool.h>

enum data_type { EIGHT_BIT = 0, TEN_BIT, FLOAT_16};
enum mask_type {MASK_ZERO = 0, MASK_COL, MASK_ROW, MASK_RASTER};

typedef struct
{
    uint8_t enable;
    float Homography[9];
}warpCfg;



typedef struct stereoCalib
{
    float baseline;
    float fov;
    uint32_t width;
    uint32_t height;
    warpCfg RightWarp;
    warpCfg LeftWarp;
} stereoCalib;

typedef struct
{

    uint8_t wsgbm_cmx_start;
    uint8_t wsgbm_cmx_end;
    int sipp_prot_sem;
    char *threadname;
}stereo_resources;

typedef struct algoMode
{
    OsDrvStereoInputFormat inMode;      // input bits/pixel (8 or 10)
    OsDrvStereoMode opMode;             // operation mode
    OsDrvStereoOutputMode outMode;  // output mode, i.e. single disparity, confidence disparity, etc
    OsDrvStereoDisparityWidth dispRange;// number of disparities (64 or 96)
    uint8_t compandEn;                  // companding enabled; used for more covered disparities
    uint8_t costDump;                   // dump cost volume; can be used for subpixel SW interpolation
    uint8_t subpixelEn;                 // enable subpixel SW interpolation
}algoMode;

typedef struct descFormat
{
    OsDrvStereoCTOutputFormat ctFormat; // CT descriptor format
    OsDrvStereoCTKernelSize ctKernel;   // CT kernel size
    uint8_t maskEn;                     // enable CT mask
    mask_type maskT;                     // mask type
}descFormat;

typedef struct stereoCore
{
    bool customStereoConfig = false;
    algoMode algoCfg;           // output mode, i.e. single disparity, confidence disparity, etc
    descFormat ctCfg;           // conditioned disparity threshold
}stereoCore;

typedef struct
{
    bool wsgbmEn = true;
    bool disp2depthEn = true;
    bool flipByWarp = true;
    bool flipByDisp2depth = false;
    stereoCalib calib = {0.035,69.0,1280,720,
        {true,{1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0}},
        {false,{1.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0}}};
    stereoCore stereocore; // stereo core algo parameters
    uint8_t confidenceThr = 200; // conditioned disparity threshold
}stereo_cfg;


#define STEREO_CFG_H

#endif
