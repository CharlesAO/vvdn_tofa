#ifndef __ISP_COMMON_UTILS_H__
#define __ISP_COMMON_UTILS_H__

#include "IspCommon.h"
#include "IcTypes.h"

#define MAX_PHASES 16 //polyfir max N

typedef struct PlgIspBaseS
{
   uint32_t  flags;
   void     *params;

  //All ISP plugins must be able to enumerate memory requirements
  //by implementing this prototype
   void (*fnQueryMemReq)(struct PlgIspBaseS *me, uint32_t wSig, uint32_t wMain, uint32_t wPoly, uint32_t* req);
}PlgIspBase;

typedef enum
{
   FMT_420 = 0,
   FMT_422 = 1,
   FMT_444 = 2
}YuvFmt;

typedef struct
{
    //Polyfir coef (arrays need to be 4Bytes alined as they get copied
    //              to HW filter area via u32_memcopy)
    uint8_t    _hCoefsY[MAX_PHASES*8] __attribute__((aligned(4))); //horizontal  Luma    coefs
    uint8_t    _vCoefsY[MAX_PHASES*8] __attribute__((aligned(4))); //vertical    Luma    coefs
    uint8_t    _hCoefsC[MAX_PHASES*8] __attribute__((aligned(4))); //horizontal  Chroma  coefs
    uint8_t    _vCoefsC[MAX_PHASES*8] __attribute__((aligned(4))); //vertical    Chroma  coefs
    UpfirdnCfg  polyY;                            //last Luma   polyFir params
    UpfirdnCfg  polyC;                            //last Chroma polyFir params
    YuvFmt      outFmt; //0:yuv420, 1:yuv422, 2:yuv444
}YuvScale;

void computePolyFirInit  (YuvScale *s);
void computePolyfirParams(YuvScale *s, icIspConfig *ic);
// useful for calculate separately the parameters structure
void computePolyfirParamsStruct(YuvScale *s, UpfirdnCfg *updnCfg0);

void initLutCopyTask();
void initLscCopyTask();
void fetchIcIspConfig(Opipe *p, icIspConfig *ic);
void fetchIcIspConfigLscLocal(Opipe *p, icIspConfig *ic);

void SetCropRegion(DBuffer  *pOutY,
                   DBuffer  *pOutUV,
                   YuvFmt    fmt,
                   icRect    cropRgn);
void initStatsCopyTask(void);
void plgIspStartStatsCopy(Opipe *p, AeAwbPatchStats* pCfgAeStats, AfPatchStats* pCfgAfStats);

extern uint8_t  *ogcBuff[24];
#endif
