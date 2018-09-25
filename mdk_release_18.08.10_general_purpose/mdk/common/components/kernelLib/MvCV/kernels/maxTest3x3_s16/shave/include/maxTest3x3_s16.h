#ifndef __MAXTEST3X3_S16_H__
#define __MAXTEST3X3_S16_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// The function will check maxCountIn locations.
/// @param[in]  inBufferCandidates - s16 buffer, where the candidates can be found
/// @param[in]  inBuffer           - pointers to the u16 input line. It will contain three pointers for three lines
/// @param[in]  maxLocationsIn     - Gives the x coordinates of the candidates. Only these candidates are checked.
/// @param[out] maxLocationsOut    - The values from maxLocationsIn which passed the filter.
/// @param[in]  maxCountIn	       - Number of values in maxLocationsIn buffer
/// @param[out] maxCountOut	       - Number of values in maxLocationsOut buffer

MVCV_FUNC(void, mvcvMaxTest3x3_s16, s16* inBufferCandidates, s16** inBuffer,
          u32 maxLocationsIn[], u32 maxLocationsOut[],
          u32 maxCountIn, u32* maxCountOut);
//!@}
#endif //__MAXTEST3X3_H__
