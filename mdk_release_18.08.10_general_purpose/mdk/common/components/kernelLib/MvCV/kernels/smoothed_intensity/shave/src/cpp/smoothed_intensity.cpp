#include "smoothed_intensity.h"

void mvcvSmoothed_intensity(float kp_x, float kp_y, u8* integral, int integral_stride,
                            mvcvPatternPoint* pattern_pnt_lst, int pattern_pnt_sz,
                            unsigned short* intensities)
{
    for(int idx = 0; idx <  pattern_pnt_sz; idx++)
    {
        // get point position in image
        const mvcvPatternPoint& FreakPoint = pattern_pnt_lst[idx];
        const float radius = FreakPoint.sigma;
        const float xf = FreakPoint.x + kp_x;
        const float yf = FreakPoint.y + kp_y;

        // calculate borders
        // TODO: -1 here is because we reduced the integral image size from width + 1 to width. Revert this when the integral image is fixed.
        const int x_left = int(xf - radius + 0.5) - 1;
        const int y_top = int(yf - radius + 0.5) - 1;
        const int x_right = int(xf + radius + 1.5) - 1;//integral image is 1px wider
        const int y_bottom = int(yf + radius + 1.5) - 1;//integral image is 1px higher
        int ret_val;


        ret_val = ((int*)(integral + integral_stride * y_bottom))[x_right]; //bottom right corner
        ret_val -= ((int*)(integral + integral_stride * y_bottom))[x_left];
        ret_val += ((int*)(integral + integral_stride * y_top))[x_left];
        ret_val -= ((int*)(integral + integral_stride * y_top))[x_right];
        ret_val = ret_val/( (x_right - x_left)* (y_bottom - y_top) );
        intensities[idx] = (unsigned short)(ret_val);
    }
    return;
}

