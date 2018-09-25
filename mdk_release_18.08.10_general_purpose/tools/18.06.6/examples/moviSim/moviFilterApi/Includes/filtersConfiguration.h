//--------------------------------------------
//File Name   : filtersConfiguration.h
//Description : Structures to configure Warp and Stereo filters
//Author      : Adrian Lazar(adrian.lazar@intel.com)
//Date        : 17.11.2017
//--------------------------------------------
#ifndef __FILTERS_CONFIGURATION__
#define __FILTERS_CONFIGURATION__

#include <stdint.h>
#include <stdlib.h>

using namespace std;
/*
* WARP Configuration - FROM MDK EXAMPLES MyriadX
*/

//Interpolation mode: 2-bit field
#define BICUB  (1<<0) //bit  9
#define BILIN  (1<<1) //bit 10
#define NONE   (   0)

//Mesh config
#define S_F32      (3<<6)                //sparse.f32
#define D_I16     ((2<<6)|(1<<5))        //dense.i16
#define D_I16_REL ((2<<6)|(1<<5)|(1<<0)) //dense.i16 (relative to out pix)

//Replicating color
#define REPL              0  //replicate pixel
#define COLOR(c) (1<<16|(c)) //use clear-color

//Data alignment (to 16 Bytes target)
#define D_ALIGN(x) (((x) + 15) & (~15))

//Warp IO indices (as in HW)
#define W_MESH 0
#define W_IN   1
#define W_OUT  2

#define BPP(x) (x)

//WARP circular buff progress
typedef struct
{
    uint32_t  base;  // base addr
    uint32_t  h;     // cb height
    uint32_t  pos;   // cb position
    uint32_t  top;   // cb Top
} WarpCb;

typedef struct {
    uint16_t x;
    uint16_t y;
} TilePos;

typedef struct
{
    uint32_t  base;        // filter base address
    uint32_t  rows;        // number of rows per run
    uint32_t  mode;        // mode  config register
    uint32_t  runs;        // runs  config register
    uint32_t  start;       // start config register
    uint32_t  inW;         // in  W [pixels]
    uint32_t  inH;         // in  H [pixels]
    uint32_t  outW;        // out W [pixels]
    uint32_t  outH;        // out H [pixels]
    uint32_t  meshW;       // mesh W
    uint32_t  meshH;       // mesh H
    uint32_t  tileW_pwr2;  // tile W as power of 2 [pixels]
    uint32_t  tileH_pwr2;  // tile H as power of 2 [pixels]
    uint32_t  tileW;       // tile W [pixels]
    uint32_t  tileH;       // tile H [pixels]
    uint32_t  runNo;       // run number
    uint32_t  interp;      // interpolation mode
    uint32_t  meshMode;    // sparse/dense
    uint32_t  pfbc;
    uint32_t  memBurst[3];
    uint32_t  edgeCol;
    uint32_t  linesDone;   // [0..outH-1]
    uint32_t  frameDone;
    uint32_t  leftRuns;    // remaining runs to be kicked

    WarpCb    cBuf[3];     // {mesh, in, out}
    void     *full[3];     // full buff addr {mesh, in, out}
    uint32_t  strd[3];     // stride [bytes] {mesh, in, out}
    float    *mat3x3;      // coord transform matrix (can be NULL)

                           //Precompute params to avoid FP operations in IRQ handler
                           //this saves FP regs save/restore, also saves cc.
    uint16_t *meshStartYi; //mesh start Y [Integer ]
    uint32_t *meshStartYf; //mesh start Y [Fraction]
    uint16_t *meshStartXi; //mesh start X [Integer ] (unused typically)
    uint32_t *meshStartXf; //mesh start X [Fraction] (unused typically)
    TilePos  *tileStartXY; //tile start              (unused typically)
    uint32_t  meshStartSz; //size of array
} WarpConfiguration;

/*
* Stereo Configuration - FROM MDK EXAMPLES MyriadX
*/

//Input image is organized as 2-planes of input buffer
#define ST_RIGHT 0 //plane 0
#define ST_LEFT  1 //plane 1

#define NO_MASK 0xFFFFFFFFFFFFFFFFLL

//Input formats
typedef enum {
    BITS_08 = 0,
    BITS_10_FLAT = 1,
    BITS_10_PACK = 2
}StereoInFmt;

typedef enum {
    //u32 block descriptor:
    PV8_CT24 = 0,
    PV0_CT32 = 1,
    //u64 block descriptor:
    PV0_CT64 = 2,
    PV8_CT56 = 3,
    PV10_CT48 = 4
}CtFormat;

//CT kernel size
typedef enum {
    CT_KS_5x5 = 0,
    CT_KS_7x7 = 1,
    CT_KS_7x9 = 2,
}CtKerSz;

//Using Vasile's enum
typedef enum {
    OMODE_DSP_MAP = 0, // ( d0 , min1 , min2 )
    OMODE_DSP_MC = 1, // ( d0 , r(p))
    OMODE_DSP_CD = 2, // disp(p) : just conditioned disparity
    OMODE_DSP_SINGLE = 3, // (d0)    : just disparity
    OMODE_DSP_COMB = 4, // ( disp(p) , r(p), min1 )
}StereoOut;

//SGBM = Semi Global Block Matching
typedef enum {
    CT_ONLY = 0, // CensusTransform only
    SGBM_ONLY = 1, // SGBM only (read pre-computed costs)
    CT_CM = 2, // CT + CM
    FULL_PIPE = 3, // full pipe
}StereoMode;

typedef enum //maps on STEREO_CFG[22:21]
{
    DBG_NONE = 0x0, //2'b00
    CT_des = 0x1, //2'b01
    DBG_map = 0x2  //2'b10
}DbgDump;

typedef enum //maps on STEREO_CFG[14:13]
{
    DISP_64 = 0,
    DISP_96 = 1
}NumDisp;

typedef struct
{
    uint64_t base;
    uint32_t lnStride;
    uint32_t plStride;
    uint32_t curLine;
    uint32_t height;
} Buff;

// This structure is used also for the circular buffer
// Circular Buffer not likely to be used in full frame tests on pcSimulation
typedef struct
{
    uint32_t runNo;
    uint32_t lnPerRun;
    uint32_t width;   //[line width]
    uint32_t cpLines; //should match circular buffer height eventually
    Buff buff;
} CBuff;


//#########################################################
typedef struct
{
    uint32_t   inFmt; //0:8bit; 1:10bits-flat; 2:10bit-packed
    StereoOut  outMode;
    StereoMode operation;
    CtFormat   ctFormat;
    CtKerSz    kerSz;
    uint64_t   mask;
    NumDisp    nd;
    DbgDump    dd;
    uint32_t   params;
    uint32_t   invalidDisp;
    uint32_t   companding;
    uint32_t   enCtThr;
    uint32_t   enCtMean;
    CBuff      iBuf[2];  // for full-frame
    CBuff      oBuf[2];  // for full-frame
    uint32_t   nInPl[2];  //  num input planes
                          //SGBM penalties
    uint32_t   aggDiv;
    uint32_t   hP1, hP2; // horizontal
    uint32_t   vP1, vP2; // vertical
    uint16_t  *aggLutH;  // user
    uint16_t  *aggLutV;  // user

    uint32_t   iIrqRatePow[2];
    uint32_t   oIrqRatePow[2];

    uint32_t iFill; // Input buffer[N] fill control. STARTS STEREO. In full frame is the Image Height!!!
}
StereoConfiguration;

// API callback to be called by implementer for read requests (DPU case)
typedef void(*dataReadCallback)(uint64_t address, uintptr_t *data, size_t size);
// API callback to be called by implementer for write requests (DPU case)
typedef void(*dataWriteCallback)(uint64_t address, uintptr_t data, size_t size);

//#########################################################
typedef struct
{
    dataReadCallback dataRead = 0;
    dataWriteCallback dataWrite = 0;
}
dpuMemoryAccess;

//#########################################################
typedef struct
{
}
MestConfiguration;

//#########################################################
typedef struct
{
}
MatmulConfiguration;
#endif
