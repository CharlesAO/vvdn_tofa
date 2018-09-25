#ifndef __KERNEL_UNIT_TEST_SAMPLE_H__
#define __KERNEL_UNIT_TEST_SAMPLE_H__

#include <mv_types.h>

/// Dummy kernel that only copies data around
/// @param[in] - In_tile address of the memory where the input tile is stored
/// @param[in] - Out_tile_space address of the memory where the output tile is stored
/// @param[in] - Width width of the tile
/// @param[in] - Height height of the tile
/// @return u8* pointer to the memory area where the tile was copied
u8* dummy_kernel_result(u8* in_tile, u8* out_tile_space, int width, int height);

/// Kernel that bit or blends two planes
/// @param[in]  - In1 address to plane 1 to blend
/// @param[in]  - In2 address to plane 2 to blend
/// @param[out] - Out pointer to output memory with blended result
/// @param[in]  - Width width of the planes
/// @param[in]  - Height height of the planes
/// @return none
void blendkernel(u8* in1, u8* in2, u8* out, u32 width, u32 height);

/// Kernel that blurs with 3x5 radius (5 lines, 3 columns)
/// @param[in]  - In_lines pointer to the array of input line pointers
/// @param[out] - Out_lines pointer to the array of output line pointers
/// @param[in]  - Width width of the planes
/// @return none
void avg3x5kernel_newint(u8** in_lines, u8** out_lines, u32 width);

/// Kernel that blurs with 1x5 radius (5 lines, 1 column)
/// @param[in]   - In address to plane 1 to blend
/// @param[out]  - Out pointer to output memory with blended result
/// @param[in]   - Width width of the planes
/// @return none
void avg1x5kernel(u8** in, u8** out, u32 width);

#endif //__KERNEL_UNIT_TEST_SAMPLE_H__
