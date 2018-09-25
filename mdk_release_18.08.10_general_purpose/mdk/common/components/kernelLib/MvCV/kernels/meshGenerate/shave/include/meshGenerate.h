#ifndef __MESHGENERATE_H__
#define __MESHGENERATE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a mesh generator
/// @param[out] mesh    - array of output_mesh line
/// @param[in]  mat 	- warpAffine mat
/// @param[in]  width   - width of output line
/// @param[in]  posx    - x coordinate
/// @param[in]  posy    - y coordinate

MVCV_FUNC(void, mvcvMeshGenerate, half *mesh, float *mat, u32 width, u32 posx, u32 posy)
//!@}

#endif //__MESHGENERATE_H__

