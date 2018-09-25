///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup svuCommonShaveLUT Shave LUT
/// @{
/// @brief Shave Look up table module functions API
///
/// Used for inserting values into vectors from LUT.
///

#ifndef __SVUCOMMONSHAVELUT__H_
#define __SVUCOMMONSHAVELUT__H_

#include <mv_types.h>
#include <moviVectorUtils.h>


#ifdef __cplusplus
extern "C" {
#endif

/// @brief Function that reads 8 ushort values into a ushort8 vector from LUT
/// @param[in] in_values  - vector type to read input for performing LUT
/// @param[in] lut_memory - pointer to the LUT memory
/// @return vectorized LUT seek results
///
ushort8 svuGet16BitVals16BitLUT(ushort8 in_values, u16* lut_memory);

/// @brief Function that reads 8 uchar values into a ushort8 vector from LUT
/// @param[in] in_values  -  vector type to read input for performing LUT
/// @param[in] lut_memory - pointer to the LUT memory
/// @return vectorized LUT seek results
///
uchar8 svuGet8BitVals16BitLUT(ushort8 in_values, u16* lut_memory);

/// @brief Function that reads 8 uchar values into a uchar8 vector from LUT
/// @param[in] in_values  - vector type to read input for performing LUT
/// @param[in] lut_memory - pointer to the LUT memory
/// @return vectorized LUT seek results
///
uchar8 svuGet8BitVals8BitLUT(uchar8 in_values, u8* lut_memory); 

/// @brief Function that reads 16 uchar values into a uchar16 vector from LUT
/// @param[in] in_values  - vector type to read input for performing LUT
/// @param[in] lut_memory - pointer to the LUT memory
/// @return vectorized LUT seek results
///
uchar16 svuGet16_8BitVals8BitLUT(uchar16 in_values, u8* lut_memory);

/// @brief Function that reads 8 ushort values into a ushort8 vector from LUT
/// @param[in] in_values  - vector type to read input for performing LUT
/// @param[in] lut_memory - pointer to the LUT memory
/// @return vectorized LUT seek results
///
ushort8 svuGetu16BitVals16BitLUT(ushort8 in_values, u16* lut_memory);
/// @}
#ifdef __cplusplus
}
#endif

#endif //__SVUCOMMONSHAVELUT__H_
