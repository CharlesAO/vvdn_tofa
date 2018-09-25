///
/// @file      combDecimDemosaicAwbGains.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     combine decimation Demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image and applay WB gains
///
#include <mv_types.h>
#include <combDecimDemosaicAwbGains.h>
#include <string.h>
#include "stdio.h"
#include <stdlib.h>

/// combine decimation Demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image, and apply WB gains
/// @param[out] output   - 3 pointers to RGB lines channel
/// @param[in]  iline    - array of pointers to input line of the image 4 lines
/// @param[in] gains     - list 0f 3 white balance gains, in fixed point 8.8
/// @param[in] width     - width of the output lines
/// @return    Nothing
#define CLAMPZ255(X) ((X)>255 ? 255 : ((X)<0 ? 0 : (X)))

void mvispCombDecimDemosaicAwbGains(unsigned char **output, unsigned short **iline, unsigned short gains[3], unsigned int width)
{
    int i;
    unsigned short pixR;
    unsigned short pixG;
    unsigned short pixB;
    
    // special case for first position, kernel make padding 
    pixR =   ((iline[1][1] * 3 +  // R
                 iline[1][0*2+1] * 9 + 
                 iline[3][1] * 1 + 
                 iline[3][0*2+1] * 3) >> 4 );
    pixG =   ((iline[1][0*2  ] +  // G
                 iline[2][0*2+1] + 1) >> 1);
    pixB =   ((iline[0][0*2  ] * 3 +  // B
                 iline[0][0*2+2] * 1 + 
                 iline[2][0*2  ] * 9 + 
                 iline[2][0*2+2] * 3) >> 4); 
    // applay gains
    output[0][0] = CLAMPZ255(((( int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
    output[1][0] = CLAMPZ255(((( int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
    output[2][0] = CLAMPZ255(((( int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    
    for(i=1; i<(int)width; i++) {
        pixR =   ((iline[1][i*2-1] * 3 +  // R
                 iline[1][i*2+1] * 9 + 
                 iline[3][i*2-1] * 1 + 
                 iline[3][i*2+1] * 3) >> 4 );
                             
        pixG =   ((iline[1][i*2  ] +  // G
                 iline[2][i*2+1] + 1) >> 1);
        pixB =   ((iline[0][i*2  ] * 3 +  // B
                 iline[0][i*2+2] * 1 + 
                 iline[2][i*2  ] * 9 + 
                 iline[2][i*2+2] * 3) >> 4); 
        // apply gains
        output[0][i] = CLAMPZ255(((( int)pixR * gains[0]) >> 8)>>2); // 2 instructions / 4
        output[1][i] = CLAMPZ255(((( int)pixG * gains[1]) >> 8)>>2); // 2 instructions / 4
        output[2][i] = CLAMPZ255(((( int)pixB * gains[2]) >> 8)>>2); // 2 instructions / 4
    }
    output[2][width-1] = output[2][width-2];
}

