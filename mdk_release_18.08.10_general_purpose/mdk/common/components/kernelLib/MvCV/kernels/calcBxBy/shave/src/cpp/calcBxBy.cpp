#include "calcBxBy.h"
#include <mvcv_types.h>
#include <stdio.h>
#include <stdlib.h>
/// CalcBxBy - calcOpticalFlow component 
///@param[in] patchI    - 
///@param[in] isz        -  
///@param[in] Ix        -  
///@param[in] Iy        -  
///@param[in] minI        -  
///@param[in] patchJ    -  
///@param[in] jsz        -  
///@param[in] minJ        -  
///@param[in] error        -  
///@param[out] dstY        - 


mvCvPoint2D32f myPoint2D32f( float x, float y )
{

    mvCvPoint2D32f p;
    p.x = (float)x;
    p.y = (float)y;
    return p;

}

mvCvPoint2D32f mvcvCalcBxBy(const u8* patchI, ClSizeW isz, const float* Ix, const float* Iy, mvCvPointW minI,
    const u8* patchJ, ClSizeW jsz, mvCvPointW minJ, float* error)
{
    float bx = 0;
    float by = 0;
    
    float errorTemp = 0;

    for (int y = 0; y < jsz.y; y++)
    {
        const u8* pi = patchI +
            (y + minJ.y - minI.y + 1) * isz.x + minJ.x - minI.x + 1;
        const u8* pj = patchJ + y * jsz.x;
        
        
        const float* ix = Ix +
            (y + minJ.y - minI.y) * (isz.x - 2) + minJ.x - minI.x;
        const float* iy = Iy + (ix - Ix);
        
        // Vectorize
        for (int x = 0; x < jsz.x; x++)
        {
            short t0 = pi[x] - pj[x];
            bx += t0 * ix[x];
            by += t0 * iy[x];

            errorTemp += t0 * t0;
        }
    }

    error[0] = errorTemp;
    return myPoint2D32f(bx, by);
}