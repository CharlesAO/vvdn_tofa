#ifndef __CombDecimDemosaicAwbGains_H__
#define __CombDecimDemosaicAwbGains_H__

#include <sipp.h>

/* Bayer pattern order */
typedef enum {
    BAYER_FORMAT_GRBG = 0,
    BAYER_FORMAT_RGGB = 1,
    BAYER_FORMAT_GBRG = 2,
    BAYER_FORMAT_BGGR = 3,
} BayerFormat;

typedef struct
{
    unsigned short   gains[3];
    unsigned int bayerOrder;
}CombDecimAwbGainsParam; 

void SVU_SYM(svuCombDecimDemosaicAwbGains)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuCombDecimDemosaicAwbGains);

#endif //AwbGains

