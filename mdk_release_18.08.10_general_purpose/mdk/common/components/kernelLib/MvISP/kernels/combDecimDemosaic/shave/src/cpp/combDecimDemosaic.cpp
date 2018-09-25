///
/// @file      combDecimDemosaic.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     combine decimation Demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image
///
#include <mv_types.h>
#include <combDecimDemosaic.h>
#include <string.h>
#include "stdio.h"
#include <stdlib.h>

/// combine decimation Demosaic - Obtain a RGB image, downscaled with 0.5 from an bayer GRBG input image
/// @param[out] output   - 3 pointers to RGB lines channel
/// @param[in]  iline    - array of pointers to input line of the image 4 lines
/// @param[in] width     - width of the output lines
/// @return    Nothing
void mvispCombDecimDemosaic(unsigned short **output, unsigned short **iline, unsigned int width)
{
    int i;
    for(i=0; i<(int)width; i++)
    {
        output[0][i] =   ((iline[1][i*2-1] * 3 +  // R
                           iline[1][i*2+1] * 9 + 
                           iline[3][i*2-1] * 1 + 
                           iline[3][i*2+1] * 3) >> 4 );
                             
        output[1][i] =   ((iline[1][i*2  ] +  // G
                           iline[2][i*2+1] + 1) >> 1);

        output[2][i] =   ((iline[0][i*2  ] * 3 +  // B
                           iline[0][i*2+2] * 1 + 
                           iline[2][i*2  ] * 9 + 
                           iline[2][i*2+2] * 3) >> 4); 
    }
}
