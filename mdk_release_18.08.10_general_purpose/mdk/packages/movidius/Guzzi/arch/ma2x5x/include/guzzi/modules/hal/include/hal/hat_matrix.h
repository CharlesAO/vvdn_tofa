/* =============================================================================
* Copyright (c) 2013-2014 MM Solutions AD
* All rights reserved. Property of MM Solutions AD.
*
* This source code may not be used against the terms and conditions stipulated
* in the licensing agreement under which it has been supplied, or without the
* written permission of MM Solutions. Rights to use, copy, modify, and
* distribute or disclose this source code and its documentation are granted only
* through signed licensing agreement, provided that this copyright notice
* appears in all copies, modifications, and distributions and subject to the
* following conditions:
* THIS SOURCE CODE AND ACCOMPANYING DOCUMENTATION, IS PROVIDED AS IS, WITHOUT
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED. MM SOLUTIONS SPECIFICALLY DISCLAIMS
* ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN
* NO EVENT SHALL MM SOLUTIONS BE LIABLE TO ANY PARTY FOR ANY CLAIM, DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
* PROFITS, OR OTHER LIABILITY, ARISING OUT OF THE USE OF OR IN CONNECTION WITH
* THIS SOURCE CODE AND ITS DOCUMENTATION.
* =========================================================================== */
/**
* @file hat_matrix.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 05-Nov-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef __HAL_TYPES_MATRIX_H__
#define __HAL_TYPES_MATRIX_H__

#include <hal/hat_types.h>
#include <hal/hat_pix_fmt.h>

/* RGB to RGB Matrix:
 *  | RR GR BR |   | R |  ;  R = R x RR  +  G x GR  +  B x BR
 *  | RG GG BG | x | G |  ;  G = R x RG  +  G x GG  +  B x BG
 *  | RB GB BB |   | B |  ;  B = R x RB  +  G x GB  +  B x BB
 */
typedef union  {
    float m[3][3];
    struct {
        float rr, gr, br;       // [RR] [GR] [BR]
        float rg, gg, bg;       // [RG] [GG] [BG]
        float rb, gb, bb;       // [RB] [GB] [BB]
    };
}hat_rgb2rgb_mtx_t;

/* RGB to YUV Matrix: U/Cb ; V/Cr
 *  | RY GY BY |   | R |  ;  Y = R x RY  +  G x GY  +  B x BY
 *  | RU GU BV | x | G |  ;  U = R x RU  +  G x GU  +  B x BU
 *  | RV GV BU |   | B |  ;  V = R x RV  +  G x GV  +  B x BV
 */
typedef union  {
    float m[3][3];
    struct {
        float ry, gy, by;       // [RY] [GY] [BY]
        float ru, gu, bu;       // [RU] [GU] [BU]
        float rv, gv, bv;       // [RV] [GV] [BV]
    };
}hat_rgb2yuv_mtx_t;

/* YUV to RGB Matrix: U/Cb ; V/Cr
 *  | YR YG YB |   | Y |  ;  R = Y x YR  +  U x YG  +  V x YB
 *  | UR UG UB | x | U |  ;  G = Y x UR  +  U x UG  +  V x UB
 *  | VR VG VB |   | V |  ;  B = Y x VR  +  U x VG  +  V x VB
 */
typedef union  {
    float m[3][3];
    struct {
        float yr, yg, yb;       // [YR] [YG] [YB]
        float ur, ug, ub;       // [UR] [UG] [UB]
        float vr, vg, vb;       // [VR] [VG] [VB]
    };
}hat_yuv2rgb_mtx_t;

/*
 * R = R * c.r + o.r
 * G = G * c.g + o.g
 * B = B * c.b + o.b
 */
typedef struct  {
    hat_rgb2rgb_mtx_t m;    // float - Matrix
    hat_rgb_float_t o;    	// float [-1.0 .. +1.0]
}hat_rgb2rgb_t;

/*
 * R = R * c.r + o.r
 * G = G * c.g + o.g
 * B = B * c.b + o.b
 */
typedef struct  {
    hat_rgb2yuv_mtx_t m;    // float - Matrix
    hat_yuv_float_t o;     	// float [-1.0 .. +1.0]
}hat_rgb2yuv_t;

#endif // __HAL_TYPES_MATRIX_H__

