///
/// @file      PolyFirUtils.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PolyFirUtils header.
///

#ifndef __ISP_COMMON_POLIFIR_UTILS_H__
#define __ISP_COMMON_POLIFIR_UTILS_H__

#include <mvMacros.h>
#include "IspCommon.h"
#include "ImgTypes.h"
#include "Opipe.h"

#define MAX_PHASES 16 //polyfir max N

typedef enum
{
    FMT_420 = 0,
    FMT_422 = 1,
    FMT_444 = 2
}YuvFmt;

class PolyFirUtils {
public:
    //Sinc LUT optimization
#define MAX_SZ  1024
#define MAX_VAL  9.5f //max observed arg to sinc is about 9.42
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

    YuvScale    scale;     //yuv420 arbitrary scale output
    YuvScale    *s;     //yuv420 arbitrary scale output
    void computePolyFirInit  ();
    void computePolyfirParams(icIspConfig *ic);
private:
    static uint32_t sincLut[MAX_SZ];
    void setChromaRatio(YuvScale *s);
    uint32_t computeCoefs(uint8_t *hCoefs, uint8_t *vCoefs, UpfirdnCfg *cfg);
    void calcCoeffs(uint8_t *coef, uint32_t n, uint32_t d, uint32_t *ksize);
    inline float sincLookup(float x);
    inline float sincWarp(float x);
};

#endif //__ISP_COMMON_POLIFIR_UTILS_H__
