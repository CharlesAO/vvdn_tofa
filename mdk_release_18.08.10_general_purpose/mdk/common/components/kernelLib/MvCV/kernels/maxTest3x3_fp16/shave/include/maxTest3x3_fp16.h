#ifndef __MAXTEST3X3_H__
#define __MAXTEST3X3_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// The function will check maxCountIn locations.
/// @param[in]  inBufferCandidates - Fp16 buffer, where the candidates can be found
/// @param[in]  inBuffer 		   - Address of the fp16 image buffer. This buffer should have a size of 3 * width * sizeof(fp16) bytes (3 lines)
/// @param[in]  width 			   - Line width in pixels as u32
/// @param[in]  maxLocationsIn     - Gives the x coordinates of the candidates. Only these candidates are checked.
/// @param[out] maxLocationsOut    - The values from maxLocationsIn which passed the filter.
/// @param[in]  maxCountIn	       - Number of values in maxLocationsIn buffer
/// @param[out] maxCountOut	       - Number of values in maxLocationsOut buffer

MVCV_FUNC(void, mvcvMaxTest3x3_fp16, half* inBufferCandidates, half** inBuffer,
          u32 width, u32 maxLocationsIn[], u32 maxLocationsOut[], 
          u32 maxCountIn, u32* maxCountOut);
//!@}
#endif //__MAXTEST3X3_H__
