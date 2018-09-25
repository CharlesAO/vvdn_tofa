///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - definitions
///
/// @ingroup  Opipe
/// @{
///

#ifndef __OPIPE_DEFS_H__
#define __OPIPE_DEFS_H__

#include <DrvCmxDma.h>
#include "SippHwDefs.h"

//#########################################################################################
//Resource directory (test images)
#ifndef RDIR
#define RDIR "../resources/"
#endif

//#########################################################################################
//Chroma Median: using Luma reference
#ifndef LUMA_BASED_BLEND
#define LUMA_BASED_BLEND    1
#endif

//#########################################################################################
//IRQ rate control default value 2^3 = 8 lines
#ifndef IRQ_RATE_POW
#define IRQ_RATE_POW 3
#endif

#define IRQ_RATE (1<<IRQ_RATE_POW)

#ifndef SIPP_IRQ_LEVEL
#define SIPP_IRQ_LEVEL 10
#endif

//#########################################################################################
//Align/Section macros
#if !defined(ALIGNED)
  #if defined(__sparc)
    #define ALIGNED(x) __attribute__((aligned(x)))
  #else
    #define ALIGNED(x)  //nothing
  #endif
#endif

#if !defined(SECTION)
  #if defined(__sparc)
    #define SECTION(x) __attribute__((section(x)))
  #else
    #define SECTION(x)  //nothing
  #endif
#endif

//#########################################################################################
//Buffer Input/Output markers
#define D_IN  1
#define D_OUT 2
#define D_DMA 4 //flag to mark source (D_DMA|DMA_IN) or sink (D_DMA|D_OUT)

//#########################################################################################
//CMX buffers get defined at APP level, but these are the minimum recommended buffer heights:
#ifndef DBYR_Y_H     //Debayer Luma CMX buffer height
#define DBYR_Y_H  32 //[lines]
#endif

//Rick mentioned in sims this fills up to 4 lines or so
#ifndef SHARP_Y_H    //Sharpen CMX buffer height
#define SHARP_Y_H  8 //[lines]
#endif

//LUT output buffer
#ifndef LUT_H
#define LUT_H  16 //[lines]
#endif

//#########################################################################################
//Circular IO CMX buffers associated to main buffers (peripheral filters)
//These macros need to be multiple of IRQ_RATE !
//Here are provided minimal values !
#ifndef I_CBUFF_H
#define I_CBUFF_H (3*IRQ_RATE)
#endif

#ifndef O_CBUFF_H
#define O_CBUFF_H (2*IRQ_RATE)
#endif

//#########################################################################################
//decorators
#ifndef BPP
#define BPP(x) x // bytes per pixel
#endif

#ifndef NPL
#define NPL(x) x // number of planes
#endif

#ifndef MAX_SOURCES //max sources/pipe
#define MAX_SOURCES 4
#endif

#ifndef MAX_SINKS   //max sinks/pipe
#define MAX_SINKS 4
#endif

#ifndef N_DESCS     //number of circular descriptors per src/snk
#define N_DESCS 4
#endif

#ifndef CDMA_DEF_REQ
#define CDMA_DEF_REQ 3
#endif

//#########################################################################################
//Possible pixel formats
#define SIPP_FMT_8BIT    0x1
#define SIPP_FMT_16BIT   0x2
#define SIPP_FMT_32BIT   0x4
#define SIPP_FMT_PACK10  0x5 //packed type
#define SIPP_FMT_PACK12  0x3 //packed type

//#########################################################################################
//Flags
#define GEN_PREVIEW         (1<<0) //USER
#define DETERMINED_CFG      (1<<1) //internal flag
#define PIPE_RUNNING        (1<<2) //internal flag
#define PREVIEW_ABLE        (1<<3) //internal flag (isp pipe)
#define LOST_IRQ            (1<<4) //internal flag
#define CLEAN_EXIT          (1<<5) //disable filters, links, irq resources @ eof

//#########################################################################################
//Misc
#define DW_ALIGN(x) (((x) + 7) & (~7))

typedef struct {
  uint16_t tlcX;   //topLeftCorner.X [bytes]
  uint16_t tlcY;   //topLeftCorner.Y [lines]
  uint16_t w;      //out-width       [bytes]
  uint16_t h;      //out-height      [lines]
  uint16_t lStride;//out-lineStride  [bytes]
  uint32_t pStride;//out-planeStride [bytes]
  uint16_t lineNo; //internal
}RectRgn;

//#########################################################################################
//used in conjunction with SW links, used to monitor Producer/Consumers fill levels
//Progress Buffer
typedef struct {
   uint32_t height;       ///< full buffer height
   uint32_t curLine;      ///< line progress (e.g. DBYR written lines / DOGL read lines)
   uint32_t irqRatePow;  ///< IRQ rate of SIPP filter
   uint32_t irqRate;     ///< derived as 1<<irqRatePow
}PBuffer;

//#########################################################################################
//CMX/DDR view of same logical buffer (CMX circular buff / DDR full buff)

//================================================================
//Simple buffer definition
typedef struct {
   uint32_t base;         ///< buffer base addr in CMX
   uint32_t height;       ///< buffer height for circular progress
   uint32_t lineStride;   ///< aux: line stride [bytes] = 8Byte aligned version of (lineW)
   uint32_t lineNo;
   uint32_t lineW;        ///<line width in Bytes (ddr.lineW could be smaller on crop)
}SBuffer;

//================================================================
typedef struct {
 //inlined SBuffer here
   uint32_t base;         ///< buffer base addr in CMX
   uint32_t height;       ///< buffer height for circular progress
   uint32_t lineStride;   ///< aux: line stride [bytes] = 8Byte aligned version of (lineW)
   uint32_t lineNo;
   uint32_t lineW;        ///<line width in Bytes

 //DMA related members
   uint32_t           cpLines;         ///<number of lines that actually got coppied in/out by DMA
   dmaTransactionList dmaDsc[N_DESCS]; ///<Dma descriptors
   uint32_t           curDsc;          ///<cur descriptor [0..N_DESCS-1]
}MBuffer; //Main(full) buffer

struct _SwLink; //FWD declaration
//================================================================
///[Full-DDR] + [circular-CMX] circular buffer pair
/// DDR is only used for sink/source constructs
typedef struct{
   SBuffer            cmx;             ///< cmx view [mandatory]
   MBuffer            ddr;             ///< ddr view [optional, just for sinks/sources]

 //Common
   void               *pipeRef;        ///<ref to pipe it belongs to
   uint32_t           unitID;          ///<unit ID of the filter that gets fed
   uint32_t           dir;             ///<direction: IN/OUT: only relevant for SRK/SNK
   uint32_t           sippBuffBase;    ///< associated Sipp IOBuff Reg addr
   uint32_t           nPlanes;         ///<num planes
   uint32_t           fmt;             ///<bytes / pix format
   uint32_t           irqRatePow;      ///<IRQ rate of SIPP filter
   uint32_t           irqRate;         ///<derived as 1<<irqRatePow

 #if defined(OPIPE_RUNTIME_CHECKS)
   uint32_t           liloCnt;         ///<LineIn/LineOut sw track
 #endif

 #if defined(TAPOUT_EN)
 //Relevant for SINK constructs only
   uint32_t         doneCnt;
   struct _SwLink   *link;
 #endif

 #if defined(OPIPE_CROP)
  RectRgn crop;
 #endif

}DBuffer; //Sink/Source is of type DBuffer

//================================================================
typedef void (*deferCb)(struct _SwLink *swLink, uint32_t nLines);

typedef struct _SwLink{
   void     *pipeRef;         ///<ref to pipe it belongs to
   uint32_t  prodId;          ///< producer ID
   uint32_t  allConsIdMask;   ///< consumers ID mask
   uint32_t  lastConsId;      ///< last consumer ID

  //Monitor copied lines
  //(to cope with HW bug, as sometimes OBFL_INC doesn't fire at EOF)
   PBuffer   prodMon;        ///<producer monitor
   PBuffer   lastConsMon;    ///<last consumer monitor(decrements producer OBFL)

 #if defined(SW_LINK_EXT)
   deferCb   cbIbflIncHook;  ///<fwd IbflInc to inner SW block
   deferCb   cbObflDecHook;  ///<fwd ObflDec to inner SW block
 #endif

 #if defined(TAPOUT_EN)
   uint32_t  doneCnt;
   DBuffer  *sink;
 #endif

}SwLink;

//================================================================
//Opipe Globals
typedef struct{
   uint32_t unitsInUse; //all units
   uint32_t cdmaReq;    //to control CDMA agent assignment
   uint32_t nSources;
   uint32_t nSinks;
   uint32_t nSwLinks;
   uint32_t nPipes;

 //Keep all pipes sources/sinks centralized as we need a centralized approach
 //from ISR
   DBuffer  *sources[32];
   DBuffer  *sinks  [32];
   SwLink   *swCtrl [32];

}OpipeGlobal;

//===================================================================
///Mipi-TX loopback debug params
typedef struct {
    uint32_t txID;               ///<SIPP_MIPI_TX0_ID or SIPP_MIPI_TX1_ID
    uint8_t *imgAddr;            ///<full image base address
    uint32_t imgW;               ///<full image width
    uint32_t imgH;               ///<full image height
    uint32_t bpp;                ///<bytes per pixel
    uint32_t hbp;                ///<timing: horizontal back porch [pclk]
    uint32_t hfp;                ///<timing: horizontal front porch [pclk]
    uint32_t hsync;              ///<timing: horizontal sync [pclk]
    uint32_t vsync;              ///<timing: vertical sync [lines]
}oMipiTxLoopbackParam;

//Callbacks for Eof, LineReached, PreStart
typedef struct OpipeS Opipe;
typedef void (*opipeCb)(Opipe* p);

//#########################################################################################
/// Main O-Pipe data struct
struct OpipeS
{
    uint32_t oldW;       ///<prev W for res internal adjustment
    uint32_t oldH;       ///<prev H for res internal adjustment

    uint32_t width;       ///<Used to compute XXX_FRM_DIM above DBYR (including)
    uint32_t height;      ///<Used to compute XXX_FRM_DIM above DBYR (including)

  //Still split members
    uint32_t fullW;       ///<Used to by lumaDns
    uint32_t fullH;       ///<Used to by lumaDns
    uint32_t offX;        ///<Used to by lumaDns
    uint32_t offY;        ///<Used to by lumaDns

  //Resolution for MAIN ISP part (filters below debayer...)
  //to allow preview-mode
    uint32_t width2;     ///<Used to compute XXX_FRM_DIM below DBYR
    uint32_t height2;    ///<Used to compute XXX_FRM_DIM below DBYR

    uint32_t unitIDs;     ///<Mask containing bit-enables for all filters used in this pipeline
    uint32_t running;     ///<flag = 1 if pipe is in progress
    uint32_t irqRate;     ///<default to 8
    uint32_t format;      ///<Bayer/Planar (shared by bayer blocks)
    uint32_t bayerPattern;///<Relevant if format == BAYER
    uint32_t rawBits;     ///<Bayer path bits (shared by bayer blocks)
    uint32_t enMask;      ///<Filters enable mask (or-ed into SIPP_CONTROL_ADR)

    uint32_t id;          ///<app general purpose indentification
    void    *params[8];   ///<app general purpose params

  //typical oPipe IF doesn't expose cfg reg, so add it here, user needs to set accordingly.
    uint32_t cfg    [SIPP_F_NUM]; ///<user CFG override for all filters
    uint8_t  iPlanes[SIPP_F_NUM]; ///<Input planes (for each unit)
    uint8_t  oPlanes[SIPP_F_NUM]; ///<Output planes (for each unit), mostly unused as matches iPlanes

  //==============================================================
  //Callbacks - line callbacks (just one sink per pipe can trigger line cb)
    uint32_t  triggerSinkId; ///<SIPP ID of filter that can generate the line callbacks
    uint32_t  currHitLine;   ///<notify user what is current hit line
    uint16_t  targetLine[4]; ///<up to 4 lines per frame that can trigger the line-CB
    opipeCb   cbLineHit;     ///<line-reached callback (OPTIONAL)

  //Callbacks - frame level
    opipeCb  cbEndOfFrame;        ///<end of frame callback (OPTIONAL)
    opipeCb  cbPreStart;          ///< (OPTIONAL)

  //==============================================================
  //HW blocks configuration (user should only set valid pointers to filters of interest)
    BlcCfg        *pBlcCfg;       ///<Black Level Correction params
    SigmaDnsCfg   *pSigmaCfg;     ///<Sigma Denoise params
    LscCfg        *pLscCfg;       ///<Lens Shading params
    RawCfg        *pRawCfg;       ///<Raw params
    DbyrCfg       *pDbyrCfg;      ///<Debayer params
    LtmCfg        *pLtmCfg;       ///<Local Tone Mapping params
    DogCfg        *pDogCfg;       ///<Difference Of Gaussians params
    LumaDnsCfg    *pLumaDnsCfg;   ///<Luma Denoise params
    LumaDnsRefCfg *pLumaDnsRefCfg;///<Luma Denoise Reference params
    SharpenCfg    *pSharpCfg;     ///<Luma Sharpen params
    ChromaGenCfg  *pChrGenCfg;    ///<Chroma Generation params
    MedianCfg     *pMedCfg;       ///<Median params
    ChromaDnsCfg  *pChromaDnsCfg; ///<Chroma Denoise params
    ColCombCfg    *pColCombCfg;   ///<Color Combination params
    LutCfg        *pLutCfg;       ///<Lookup table params
    ColConvCfg    *pColConvCfg;   ///<Color conversion params
    ConvCfg       *pConvCfg;      ///<Convolution params
    UpfirdnCfg    *pUpfirdn0Cfg;  ///<UPFIRDN_0 params
    UpfirdnCfg    *pUpfirdn12Cfg; ///<UPFIRDN_1 and UPFIRDN_2 shared params
    MipiRxCfg     *pMipiRxCfg[4]; ///<MipiRx params
    HarrisCfg     *pHarrisCfg;    ///<Harris params

  //RAW AE/AWB/AF configs
    AeAwbCfg      *aeCfg;         ///<Ae/Awb config struct
    AfCfg         *afCfg;         ///<Af config struct

  //RAW AE/AWB/AF outputs (ptr to output arrays)
    AeAwbPatchStats *aeStats;     ///<Ae/Awb output stats buff
    AfPatchStats    *afStats;     ///<Af output stats buff

  //RAW Histograms (set non-null, 64bit aligned ptrs to enable !)
    uint32_t *histLuma;          ///<Luma histogram (256 entries)
    uint32_t *histRgb;           ///<RGB histogram (3x128 entries)

  //==============================================================
  //Private stuff:
    uint32_t cfgMask;      ///<internally derived mask (to be OR=ed into SIPP_OPIPE_CFG_ADR)
    DBuffer  cmxBuffs[16]; ///<internal
    uint32_t nCmxBuffs;    ///<internal

  //Local references to own pipe sources and sinks
    uint32_t  nSrcs;            ///<internal
    uint32_t  nSnks;            ///<internal
    uint32_t  nSwLinks;
    DBuffer  *srcs[MAX_SOURCES];///<internal
    DBuffer  *snks[MAX_SINKS  ];///<internal
    SwLink    swCtrl[4];

  //IRQ enable masks (to be ored onto SIPP regs)
    uint32_t sippSrcIntEnMask; ///< goes OR-ed into SIPP_INT0_ENABLE_ADR
    uint32_t sippSnkIntEnMask; ///< goes OR-ed into SIPP_INT1_ENABLE_ADR

    uint32_t flags;            ///< pipe flags
    uint32_t cdmaReq;          ///< CMXDMA requester id (to control CMXDMA agent assignment)

   #if defined(OPIPE_RUNTIME_CHECKS)
    uint32_t nDmaPending;
    uint32_t nMissedIrq;
   #endif
};

//########################################################
uint32_t OpipeFinished(Opipe *p);
uint32_t OpipeDisable (Opipe *p);
void     oMemCompare(void* _refA, void* _refB, int len); //util

/// @}
#endif
