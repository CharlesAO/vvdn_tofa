#ifndef __SMOOTHED_INTENSITY_H__
#define __SMOOTHED_INTENSITY_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvSmoothed_intensity kernel computes the average image intensity for N patches based on the integral image
/// @param[in] kp_x             - Patch center X coordinate
/// @param[in] kp_y             - Patch center Y coordinate
/// @param[in] integral         - pointer to integral image (u32)
/// @param[in] integral_stride  - the stride of integral image lines in bytes
/// @param[in] pattern_pnt_lst  - the list of the (x,y, sigma) values. X and Y means the offset from kp_x and kp_y, and sigma is the size of the patch
/// @param[in] pattern_pnt_sz   - size of the pattern_pnt_lst. This is the number of patches
/// @param[out] intensities     - Output values. The average intensities of the specified patches
/// @return    Nothing

struct mvcvPatternPoint
{
    float x; // x coordinate relative to center
    float y; // y coordinate relative to center
    float sigma; // Gaussian smoothing sigma
};

MVCV_FUNC(void, mvcvSmoothed_intensity, float kp_x, float kp_y, u8* integral,
          int integral_stride, mvcvPatternPoint* pattern_pnt_lst,
          int pattern_pnt_sz, unsigned short* intensities)
//!@}


#endif //__SMOOTHED_INTENSITY_H__
