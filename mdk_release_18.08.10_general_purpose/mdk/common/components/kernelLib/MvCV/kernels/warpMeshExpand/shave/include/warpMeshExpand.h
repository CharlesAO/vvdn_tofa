#ifndef __WARPMESHEXPAND_H__
#define __WARPMESHEXPAND_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvWarpMeshExpand - function that expands coordinates between 4 initial coordinates on x and y, it outputs 16x16 coordinates for x mesh and y mesh
/// @param[in] xcoord            - array of xcoords of the 4 points
/// @param[in] ycoord            - array of ycoords of the 4 points
/// @param[out] mx               - array of 16x16 xcoords of between the initial 4 coordinates
/// @param[out] my               - array of 16x16 ycoords of between the initial 4 coordinates
/// @return    Nothing

MVCV_FUNC(void, mvcvWarpMeshExpand, half *xcoord, half *ycoord, half *mx, half *my)

//!@}

#endif //__WARPMESHEXPAND_H__