///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - app helpers
///

#ifndef __OPIPE_APPS_H__
#define __OPIPE_APPS_H__

#include "Opipe.h"

#ifdef __cplusplus
extern "C" {
#endif

//#######################################################################################
uint32_t OpipeDefaultCfg(Opipe *p, uint32_t unitID);

//#######################################################################################
// MipiRx->DDR, useful for multiple camera capture scenario
typedef struct
{
  Opipe     p;
  DBuffer  *pOut;//to direct DDR output
  uint32_t  cdmaReq;
}
OpipeRx;

void OpipeCreateRx(OpipeRx *pp,
                   uint32_t width,        uint32_t height,
                   uint32_t mipiRxId,     uint32_t outFmt,
                   uint32_t oCircBufAddr, uint32_t oCircBufH);

//#######################################################################################
// Pre Processor block (PP), useful for ISP-SPLIT mode
typedef struct
{
  Opipe     p;
  DBuffer  *pOut;//to direct DDR output
  uint32_t  cdmaReq;
}
OpipePP;

void OpipeCreatePP(OpipePP *pp,
                   uint32_t width,        uint32_t height,
                   uint32_t mipiRxId,     uint32_t outFmt,
                   uint32_t oCircBufAddr, uint32_t oCircBufH);

//#######################################################################################
// Full & Main ISP, useful for frame processing from/to DDR
typedef struct{
   uint32_t base;
   uint32_t w;
   uint32_t h;
}Buf;

typedef struct
{
   Opipe p;

   DBuffer  *pIn;   //mainIn
   DBuffer  *pOutY; //mainOut.Y or mainOut.YUV if "disablePolyFir" is set
   DBuffer  *pOutUV;//mainOut.UV   (don't care if "disablePolyFir" is set)
   DBuffer  *pDbyrC;//Dbyr cons (needed to bypass DOGL and hook to LUMA)

 //UPFIRDN_1 reference buffers for general resize case
   DBuffer  *pOutLut;//input ref
   DBuffer  *pInUV;  //input

 //Preview required buffs
   DBuffer  *pOutRaw;
   DBuffer  *pInDbyr;

   uint32_t  cdmaReq;

   SwLink   *dbyrI;
   SwLink   *dbyrO;

 union{
   Buf   cBufSigma; // Full-ISP
   Buf   cBufDbyr;  // Main-ISP
 }in;

   Buf   cBufDbyrIn;  //required if PREVIEW is to be used
   Buf   cBufDbyrY;
   Buf   cBufSharp;
   Buf   cBufLut;     //3 plane buffer (2BPP)
   Buf   cBufPoly;    //3 plane buffer (1BPP) if .base==0, no resize will be done

}
OpipeMF; //main or full pipe data struct

typedef enum {
   DOGL_ON_F16_CMX    = 0,
   DOGL_ON_U8F_STREAM = 1,
   DOGL_ON_F16_STREAM = 2, //BROKEN, debug only, won't get fixed !!!
   DOGL_OFF           = 3,
}DoglCfg;

void OpipeCreateFull(OpipeMF *oFull, uint32_t inFmt);
void OpipeCreateMain(OpipeMF *oMain, uint32_t inFmt);
void OpipeSetSizeMF (OpipeMF *o    , uint32_t iWidth, uint32_t iHeight);
void OpipeSetDoglCfg(OpipeMF *o    , DoglCfg cfg);

//#######################################################################################
//Full-ISP with debayer tapout (Y or RGB) possibility

//Creation Flags:
typedef struct {
   uint8_t mode  : 1; //0:RGB; 1:luma
   uint8_t bpp   : 2; //tap bytes/pix : 1 or 2
   uint8_t inFmt : 4; //in(sigma) format
} TapParam;

#define TAP_RGB  0
#define TAP_LUMA 1

typedef struct
{
   Opipe p;

   DBuffer  *pIn;    //mainIn
   DBuffer  *pOutY;  //mainOut.Y
   DBuffer  *pOutUV; //mainOut.UV

  union{
   DBuffer  *rgb;    //tapout dbyr.RGB
   DBuffer  *y;      //tapout dbyr.y
   DBuffer  *any;    //common reference
  }pOutTap;

 //UPFIRDN_1 reference buffers for general resize case
   DBuffer  *pOutLut;//input ref
   DBuffer  *pInUV;  //input

   SwLink   *dbyrO;
   SwLink   *dbyrOrgb;

 union{
   Buf   cBufSigma; // Full-ISP
   Buf   cBufDbyr;  // Main-ISP
 }in;

   Buf   cBufDbyrY;
   Buf   cBufDbyrRgb;
   Buf   cBufSharp;
   Buf   cBufLut;     //3 plane buffer (2BPP)
   Buf   cBufPoly;    //3 plane buffer (1BPP) if .base==0, no resize will be done
}
OpipeTap; //main or full pipe data struct

void OpipeCreateDbyrTap(OpipeTap *o, TapParam param);
void OpipeSetSizeTap   (OpipeTap *o, uint32_t iWidth, uint32_t iHeight);

//#######################################################################################
// Full ISP with LCA SW block on shave, useful for frame processing from/to DDR

typedef struct
{
   Opipe p;

   DBuffer  *pIn;   //mainIn
   DBuffer  *pOutY; //mainOut.Y or mainOut.YUV if "disablePolyFir" is set
   DBuffer  *pOutUV;//mainOut.UV   (don't care if "disablePolyFir" is set)
   DBuffer  *pDbyrC;//Dbyr cons (needed to bypass DOGL and hook to LUMA)

 //UPFIRDN_1 reference buffers for general resize case
   DBuffer  *pOutLut;//input ref
   DBuffer  *pInUV;  //input

 //Preview required buffs
//   DBuffer  *pOutRaw;
//   DBuffer  *pInDbyr;

   //LCA
   DBuffer *pRawO; //raw out buff
   DBuffer *pDbyI; //dbyr in buff

   uint32_t  cdmaReq;

   SwLink   *dbyrI;
   SwLink   *dbyrO;

 union{
   Buf   cBufSigma; // Full-ISP
   Buf   cBufDbyr;  // Main-ISP
 }in;

//   Buf   cBufDbyrIn;  //required if PREVIEW is to be used
   Buf   cBufDbyrY;
   Buf   cBufSharp;
   Buf   cBufLut;     //3 plane buffer (2BPP)
   Buf   cBufPoly;    //3 plane buffer (1BPP) if .base==0, no resize will be done

   //LCA
   Buf   oCbRaw;
   Buf   iCbDbyr;
}
OpipeFLca; //main or full pipe data struct

void OpipeCreateFullLca(OpipeFLca *oMain, uint32_t inFmt);
void OpipeSetSizeFLca(OpipeFLca *o, uint32_t iWidth, uint32_t iHeight);

//#######################################################################################
//[Luma in]->[Luma out] mono pipeline
//#######################################################################################
typedef struct
{
   Opipe p;

   DBuffer  *pIn;
   DBuffer  *pOut;

 //Circular CMX buffer
   Buf cBufInSig;
   Buf cBufOutRaw;
   Buf cBufOutUsm;
   Buf cBufOutLut;

 //SW links
   SwLink   *rawO; //raw     -> luma
   SwLink   *usmO; //sharpen -> lut
}
OpipeLumaMono;

void OpipeCreateLumaMono(OpipeLumaMono *o, uint32_t inFmt);

//#######################################################################################
//[Bayer in] -> [Luma out] mono pipe
//#######################################################################################
typedef struct{
  Opipe     p;

  DBuffer  *pIn;  //Sigma in
  DBuffer  *pOut; //Lut   out

 //Circular CMX buffers
  Buf cBufInSig;
  Buf cBufOutDbyr;
  Buf cBufOutUsm;
  Buf cBufOutLut;

 //Sw links
  SwLink   *dbyrO;//Dbyr->Dogl
  SwLink   *usmO; //Sharpen ->Lut
}OpipeBayerMono;

void OpipeCreateBayerMono(OpipeBayerMono *o, uint32_t inFmt);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
