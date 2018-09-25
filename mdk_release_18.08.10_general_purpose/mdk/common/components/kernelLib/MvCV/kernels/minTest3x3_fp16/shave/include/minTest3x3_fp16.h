#ifndef __MINTEST3X3_H__
#define __MINTEST3X3_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This function will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// The function will check minCountIn locations.
/// @param[in]  inBufferCandidates - fp16 buffer, where the candidates can be found
/// @param[in]  inBuffer - address of the fp16 image buffer. This buffer should have a size of 3 * width * sizeof(fp16) bytes (3 lines)
/// @param[in]  width - Line width in pixels as u32
/// @param[in]  minLocationsIn - gives the x coordinates of the candidates. Only these candidates are checked.
/// @param[out] minLocationsOut - the values from minLocationsIn which passed the filter.
/// @param[in]  minCountIn - number of values in minLocationsIn buffer
/// @param[out] minCountOut - number of values in minLocationsOut buffer
MVCV_FUNC(void, mvcvMinTest3x3_fp16, half* inBufferCandidates, half** inBuffer,
          u32 width, u32 minLocationsIn[], u32 minLocationsOut[],
          u32 minCountIn, u32* minCountOut);
//!@}
#endif //__MINTEST3X3_H__
