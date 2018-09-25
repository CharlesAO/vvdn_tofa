#ifndef __MINTEST3X3_S16_H__
#define __MINTEST3X3_S16_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// The function will check minCountIn locations.
/// @param[in]  inBufferCandidates - s16 buffer, where the candidates can be found
/// @param[in]  inBuffer           - pointers to the u16 input line. It will contain three pointers for three lines
/// @param[in]  minLocationsIn     - Gives the x coordinates of the candidates. Only these candidates are checked.
/// @param[out] minLocationsOut    - The values from minLocationsIn which passed the filter.
/// @param[in]  minCountIn	       - Number of values in minLocationsIn buffer
/// @param[out] minCountOut	       - Number of values in minLocationsOut buffer

MVCV_FUNC(void, mvcvMinTest3x3_s16, s16* inBufferCandidates, s16** inBuffer,
          u32 minLocationsIn[], u32 minLocationsOut[],
          u32 minCountIn, u32* minCountOut);
//!@}
#endif //__MINTEST3X3_S16_H__
