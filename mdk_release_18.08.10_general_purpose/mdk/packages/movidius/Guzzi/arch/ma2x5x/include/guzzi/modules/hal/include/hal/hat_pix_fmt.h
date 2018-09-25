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
* @file hat_pix_fmt.h
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

#ifndef __HAL_TYPES_PIXEL_FORMAT_H__
#define __HAL_TYPES_PIXEL_FORMAT_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>

#include <hal/hat_types.h>

/* Bayer - unsigned */
typedef struct  {
    uint16 r;
    uint16 gr;
    uint16 gb;
    uint16 b;
}hat_bayer_pix_t;

/* Bayer - Signed */
typedef struct  {
    int16 r;
    int16 gr;
    int16 gb;
    int16 b;
}hat_bayer_ofs_t;

/* Bayer - Signed large */
typedef struct  {
    int32 r;
    int32 gr;
    int32 gb;
    int32 b;
}hat_bayer_lrg_t;

/* Bayer - Float */
typedef struct  {
    float r;
    float gr;
    float gb;
    float b;
}hat_bayer_float_t;

/* RGB - unsigned */
typedef struct  {
    uint16 r;
    uint16 g;
    uint16 b;
}hat_rgb_pix_t;

/* RGB - Signed */
typedef struct  {
    int16 r;
    int16 g;
    int16 b;
}hat_rgb_ofs_t;

/* RGB - Signed large */
typedef struct {
    int32 r;
    int32 g;
    int32 b;
}hat_rgb_lrg_t;

/* RGB - Float */
typedef struct  {
    float r;
    float g;
    float b;
}hat_rgb_float_t;

/* YUV - unsigned */
typedef struct  {
    uint16 y;
    uint16 u;
    uint16 v;
}hat_yuv_pix_t;

/* YUV - Signed */
typedef struct  {
    int16 y;
    int16 u;
    int16 v;
}hat_yuv_ofs_t;

/* YUV - Signed large */
typedef struct  {
    int32 y;
    int32 u;
    int32 v;
}hat_yuv_lrg_t;

/* YUV - Float */
typedef struct  {
    float y;
    float u;
    float v;
}hat_yuv_float_t;

typedef union  {
    hat_bayer_pix_t bayer;
    hat_rgb_pix_t rgb;
    hat_yuv_pix_t yuv;
}hat_color_pix_t;

typedef union  {
    hat_bayer_ofs_t bayer;
    hat_rgb_ofs_t rgb;
    hat_yuv_ofs_t yuv;
}hat_color_ofs_t;

typedef union  {
    hat_bayer_lrg_t bayer;
    hat_rgb_lrg_t rgb;
    hat_yuv_lrg_t yuv;
}hat_color_lrg_t;

typedef union  {
	hat_bayer_float_t bayer;
    hat_rgb_float_t rgb;
    hat_yuv_float_t yuv;
}hat_color_flt_t;

/* BAYER pixel operations */
#define HAL_PIXOPS_SET_BAYER(p,val) \
    { (p)->r = val; (p)->gr = val; (p)->gb = val; (p)->b = val; }

#define HAL_PIXOPS_MUL_BAYER(p,val) \
    { (p)->r *= val; (p)->gr *= val; (p)->gb *= val; (p)->b *= val; }

#define HAL_PIXOPS_ADD_BAYER(p,val) \
    { (p)->r += val; (p)->gr += val; (p)->gb += val; (p)->b += val; }

/* RGB pixel operations */
#define HAL_PIXOPS_SET_RGB(p,val) \
    { (p)->r = val; (p)->g = val; (p)->b = val; }

#define HAL_PIXOPS_MUL_RGB(p,val) \
    { (p)->r *= val; (p)->g *= val; (p)->b *= val; }

#define HAL_PIXOPS_ADD_RGB(p,val) \
    { (p)->r += val; (p)->g += val; (p)->b += val; }

/* YUV pixel operations */
#define HAL_PIXOPS_SET_YUV(p,val) \
    { (p)->y = val; (p)->u = val; (p)->v = val; }

#define HAL_PIXOPS_MUL_YUV(p,val) \
    { (p)->y *= val; (p)->u *= val; (p)->v *= val; }

#define HAL_PIXOPS_ADD_YUV(p,val) \
    { (p)->y += val; (p)->u += val; (p)->v += val; }

/*
 * True table of Pixel formats:
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |D15-D14|D13|D12|D11|D10|D9 |D8 | D7-D6 |D5-D4 |D3-D2 |D1-D0 |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |Bayer  | I | 0 | 0 | 0 | 0 | 0 |idx-B  |idx-Gr|idx-Gb|idx-R |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |Bayer  | P | 0 | 0 | 0 | 0 | 0 |idx-B  |idx-Gr|idx-Gb|idx-R |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |RGB    | I | 0 | 0 | 0 | 0 | 0 |  00   |idx-B |idx-G |idx-R |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |RGB    | P | 0 | 0 | 0 | 0 | 0 |  00   |idx-B |idx-G |idx-R |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |YUV    | I | 0 | 0 | 0 | 0 | 0 |  00   |idx-V |idx-U |idx-Y |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 * |YUV    | P | C/L/P |x/y Dec| 0 |  00   |idx-V |idx-U |  00  |
 * +-------+---+---+---+---+---+---+-------+------+------+------+
 *
 * D15-D14 - Type of colors
 *              0 - Bayer
 *              1 - RGB
 *              2 - YUV
 *              3 - Reserved
 *
 * D13     - Interleave  / Planar between all colors
 *              0 - Interleave (Packed)
 *              1 - Planar
 *
 * D12-11  - Interleave by Colour, Line or Plane (C/L/P)
 *              0 - Colour (Cr,Cb,Cr...)
 *              1 - Line (Cr,Cr,Cr,Cr.... Cb,Cb,Cb,Cb....., Cr,Cr,Cr,Cr...)
 *              2 - Plane (Cr,Cr,Cr,Cr.... Cb,Cb,Cb,Cb.....)
 *    NOTE: This is available only in YUV and Planar mode for Y component.
 *          idx-Y always should be "0"
 *
 * D10-D9  - X/Y Decimation
 *             00 - No decimation
 *             01 - X decimation
 *             10 - Y decimation
 *             11 - X & Y decimation
 *
 * D7-D6   - Index of color B in BAYER
 *
 * D5-D4   - Index of color Gr in BAYER
 *           Index of color B in RGB
 *           Index of color V (Cr) in YUV
 *
 * D3-D2   - Index of color Gb in BAYER
 *           Index of color G in RGB
 *           Index of color U (Cb) in YUV
 *
 * D1-D0   - Index of color R  in BAYER and RGB
 *           Index of color Y  in YUV
 *
 */

/* Fill elements of pixel format value */
#define HAT_PIXFMT_COLOUR(color)    ((((uint16)(color)) & 0x3) << 14)
#define HAT_PIXFMT_PLANAR(planar)   ((((uint16)(planar)) & 0x1) << 13)
#define HAT_PIXFMT_UVPKD(pkd)       ((((uint16)(pkd)) & 0x3) << 11)
#define HAT_PIXFMT_XYDEC(dec)       ((((uint16)(dec)) & 0x3) << 9)
#define HAT_PIXFMT_ORDER(order)     ((((uint16)(order)) & 0xFF) << 0)
#define HAT_PIXFMT_POS0(idx)        ((((uint16)(idx)) & 0x3) << 0)
#define HAT_PIXFMT_POS1(idx)        ((((uint16)(idx)) & 0x3) << 2)
#define HAT_PIXFMT_POS2(idx)        ((((uint16)(idx)) & 0x3) << 4)
#define HAT_PIXFMT_POS3(idx)        ((((uint16)(idx)) & 0x3) << 6)

/* Extract elements of pixel format v(alue */
#define HAT_PIXMSK(val, type, shift, bits) \
    (type)((val >> shift) & ((1 << bits) -1))

#define HAT_PIXFMT_IS_COLOUR(fmt)    HAT_PIXMSK(fmt, enum hat_color_fmt, 14, 2)
#define HAT_PIXFMT_IS_PLANAR(fmt)    HAT_PIXMSK(fmt, enum hat_packed_fmt, 13, 1)
#define HAT_PIXFMT_IS_UVPKD(fmt)     HAT_PIXMSK(fmt, enum hat_uv_packed, 12, 1)
#define HAT_PIXFMT_IS_XYDEC(fmt)     HAT_PIXMSK(fmt, enum hat_xy_decim,  9, 2)
#define HAT_PIXFMT_IS_ORDER(fmt)     HAT_PIXMSK(fmt, uint16,  0, 8)
#define HAT_PIXFMT_IS_POS0(fmt)      HAT_PIXMSK(fmt, uint16,  0, 2)
#define HAT_PIXFMT_IS_POS1(fmt)      HAT_PIXMSK(fmt, uint16,  2, 2)
#define HAT_PIXFMT_IS_POS2(fmt)      HAT_PIXMSK(fmt, uint16,  4, 2)
#define HAT_PIXFMT_IS_POS3(fmt)      HAT_PIXMSK(fmt, uint16,  6, 2)

#define HAT_PIXORD_BAYER(R,GR,B,GB) \
    (HAT_PIXFMT_POS0(R) | HAT_PIXFMT_POS2(GB) | \
     HAT_PIXFMT_POS1(GR) | HAT_PIXFMT_POS3(B))

#define HAT_PIXORD_RGB(R, G, B) \
    (HAT_PIXFMT_POS0(R) | HAT_PIXFMT_POS1(G) | HAT_PIXFMT_POS2(B))

#define HAT_PIXORD_YUV(Y,U,V) \
    (HAT_PIXFMT_POS0(Y) | HAT_PIXFMT_POS1(U) | HAT_PIXFMT_POS2(V))

#define HAT_PIXORD_YUYV(Y0,V,Y1,U) \
    (HAT_PIXFMT_POS0(Y0) | HAT_PIXFMT_POS1(U) | \
     HAT_PIXFMT_POS2( V) | HAT_PIXFMT_POS3(Y1))

#define HAT_PIXFMT_IBAYER(order)    HAT_PIXFMT_COLOUR(HAT_PIXCOL_BAYER) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PACKED) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_PBAYER(order)    HAT_PIXFMT_COLOUR(HAT_PIXCOL_BAYER) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PLANAR) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_IRGB(order)      HAT_PIXFMT_COLOUR(HAT_PIXCOL_RGB) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PACKED) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_PRGB(order)      HAT_PIXFMT_COLOUR(HAT_PIXCOL_RGB) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PLANAR) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_IYUV(order)      HAT_PIXFMT_COLOUR(HAT_PIXCOL_YUV) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PACKED) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_PYUV(order)      HAT_PIXFMT_COLOUR(HAT_PIXCOL_YUV) | \
    HAT_PIXFMT_PLANAR(HAT_PIXPKD_PLANAR) | HAT_PIXFMT_ORDER(order)

#define HAT_PIXFMT_SYUV(uv,xy,ord)  HAT_PIXFMT_PYUV(ord) | \
    HAT_PIXFMT_UVPKD(uv)| HAT_PIXFMT_XYDEC(xy)

/* Colour types */
typedef enum  {
    HAT_PIXCOL_BAYER = 0,               // Pixel color: BAYER
    HAT_PIXCOL_RGB,                     // Pixel color: RGB
    HAT_PIXCOL_YUV,                     // Pixel color: YUV
}hat_color_fmt_t;

/* Packed formats */
typedef enum  {
    HAT_PIXPKD_PACKED = 0,              // Interleave
    HAT_PIXPKD_PLANAR,                  // Planar
}hat_packed_fmt_t;

/* Interleave/Planar type of U (Cb) & V (Cr) */
typedef enum  {
    HAT_UVPKD_PACKED = 0,               // Interleave of Cr & Cb
    HAT_UVPKD_LINE,                     // Interleave for each line
    HAT_UVPKD_PLANAR,                   // Planar for each component
}hat_uv_packed_t;

/* Colour X/Y decimation - used only for YUV */
typedef enum  {
    HAT_XYDEC_NO = 0,                   // No Decimation
    HAT_XYDEC_X,                        // Decimation by X
    HAT_XYDEC_Y,                        // Decimation by Y
    HAT_XYDEC_XY,                       // Decimation by X & Y
}hat_xy_decim_t;

typedef uint16 hat_pixel_fmt_t;

/* Bayer pixel order
 *  +----+----+
 *  |pos0|pos1|
 *  +----+----+    {0}_{1}_{2}_{3}
 *  |pos2|pos3|
 *  +----+----+
 */
typedef enum  {
    HAT_PORDBYR_R_B_Gr_Gb = HAT_PIXORD_BAYER(0, 2, 1, 3),   //  R,  B, Gr, Gb
    HAT_PORDBYR_R_B_Gb_Gr = HAT_PIXORD_BAYER(0, 3, 1, 2),   //  R,  B, Gb, Gr
    HAT_PORDBYR_R_Gr_Gb_B = HAT_PIXORD_BAYER(0, 1, 3, 2),   //  R, Gr, Gb,  B
    HAT_PORDBYR_R_Gr_B_Gb = HAT_PIXORD_BAYER(0, 1, 2, 3),   //  R, Gr,  B, Gb
    HAT_PORDBYR_R_Gb_Gr_B = HAT_PIXORD_BAYER(0, 2, 3, 1),   //  R, Gb, Gr,  B
    HAT_PORDBYR_R_Gb_B_Gr = HAT_PIXORD_BAYER(0, 3, 2, 1),   //  R, Gb,  B, Gr
    HAT_PORDBYR_B_R_Gr_Gb = HAT_PIXORD_BAYER(1, 2, 0, 3),   //  B,  R, Gr, Gb
    HAT_PORDBYR_B_R_Gb_Gr = HAT_PIXORD_BAYER(1, 3, 0, 2),   //  B,  R, Gb, Gr
    HAT_PORDBYR_B_Gr_Gb_R = HAT_PIXORD_BAYER(3, 1, 0, 2),   //  B, Gr, Gb,  R
    HAT_PORDBYR_B_Gr_R_Gb = HAT_PIXORD_BAYER(2, 1, 0, 3),   //  B, Gr,  R, Gb
    HAT_PORDBYR_B_Gb_Gr_R = HAT_PIXORD_BAYER(3, 2, 0, 1),   //  B, Gb, Gr,  R
    HAT_PORDBYR_B_Gb_R_Gr = HAT_PIXORD_BAYER(2, 3, 0, 1),   //  B, Gb,  R, Gr
    HAT_PORDBYR_Gr_R_Gb_B = HAT_PIXORD_BAYER(1, 0, 3, 2),   // Gr,  R, Gb,  B
    HAT_PORDBYR_Gr_R_B_Gb = HAT_PIXORD_BAYER(1, 0, 2, 3),   // Gr,  R,  B, Gb
    HAT_PORDBYR_Gr_B_Gb_R = HAT_PIXORD_BAYER(3, 0, 1, 2),   // Gr,  B, Gb,  R
    HAT_PORDBYR_Gr_B_R_Gb = HAT_PIXORD_BAYER(2, 0, 1, 3),   // Gr,  B,  R, Gb
    HAT_PORDBYR_Gr_Gb_R_B = HAT_PIXORD_BAYER(2, 0, 3, 1),   // Gr, Gb,  R,  B
    HAT_PORDBYR_Gr_Gb_B_R = HAT_PIXORD_BAYER(3, 0, 2, 1),   // Gr, Gb,  B,  R
    HAT_PORDBYR_Gb_R_Gr_B = HAT_PIXORD_BAYER(1, 2, 3, 0),   // Gb,  R, Gr,  B
    HAT_PORDBYR_Gb_R_B_Gr = HAT_PIXORD_BAYER(1, 3, 2, 0),   // Gb,  R,  B, Gr
    HAT_PORDBYR_Gb_B_Gr_R = HAT_PIXORD_BAYER(3, 2, 1, 0),   // Gb,  B, Gr,  R
    HAT_PORDBYR_Gb_B_R_Gr = HAT_PIXORD_BAYER(2, 3, 1, 0),   // Gb,  B,  R, Gr
    HAT_PORDBYR_Gb_Gr_R_B = HAT_PIXORD_BAYER(2, 1, 3, 0),   // Gb, Gr,  R,  B
    HAT_PORDBYR_Gb_Gr_B_R = HAT_PIXORD_BAYER(3, 1, 2, 0),   // Gb, Gr,  B,  R
}hat_pix_order_bayer_t;

/* RGB pixel order
 *  +----+----+----+
 *  |pos0|pos1|pos2|
 *  +----+----+----+
 */
typedef enum  {
    HAT_PORDRGB_RGB = HAT_PIXORD_RGB(0, 1, 2),              // R, G, B format
    HAT_PORDRGB_RBG = HAT_PIXORD_RGB(0, 2, 1),              // R, B, G format
    HAT_PORDRGB_BRG = HAT_PIXORD_RGB(1, 2, 0),              // B, R, G format
    HAT_PORDRGB_BGR = HAT_PIXORD_RGB(2, 1, 0),              // B, G, R format
    HAT_PORDRGB_GRB = HAT_PIXORD_RGB(1, 0, 2),              // G, R, B format
    HAT_PORDRGB_GBR = HAT_PIXORD_RGB(2, 0, 1),              // G, B, R format
}hat_pix_order_rgb_t;

/* YUV pixel order
 *  +----+----+----+
 *  |pos0|pos1|pos2|
 *  +----+----+----+
 */
typedef enum {
    HAT_PORDYUV_Y_U_V = HAT_PIXORD_YUV(0, 1, 2),            // Y, U, V
    HAT_PORDYUV_Y_V_U = HAT_PIXORD_YUV(0, 2, 1),            // Y, V, U
    HAT_PORDYUV_V_Y_U = HAT_PIXORD_YUV(1, 2, 0),            // V, Y, U
    HAT_PORDYUV_V_U_Y = HAT_PIXORD_YUV(2, 1, 0),            // V, U  Y
    HAT_PORDYUV_U_Y_V = HAT_PIXORD_YUV(1, 0, 2),            // U, Y, V
    HAT_PORDYUV_U_V_Y = HAT_PIXORD_YUV(2, 0, 1),            // U, V, Y
}hat_pix_order_yuv_t;

/* YUYV pixel order
 *  +----+----+----+----+
 *  |pos0|pos1|pos2|pos3|
 *  +----+----+----+----+
 */
typedef enum {
    HAT_PORDYCBCR_Y0_Y1_V_U = HAT_PIXORD_YUYV(0, 2, 1, 3),  // Y0, Y1,  V,  U
    HAT_PORDYCBCR_Y0_Y1_U_V = HAT_PIXORD_YUYV(0, 3, 1, 2),  // Y0, Y1,  U,  V
    HAT_PORDYCBCR_Y0_V_U_Y1 = HAT_PIXORD_YUYV(0, 1, 3, 2),  // Y0,  V,  U, Y1
    HAT_PORDYCBCR_Y0_V_Y1_U = HAT_PIXORD_YUYV(0, 1, 2, 3),  // Y0,  V, Y1,  U
    HAT_PORDYCBCR_Y0_U_V_Y1 = HAT_PIXORD_YUYV(0, 2, 3, 1),  // Y0,  U,  V, Y1
    HAT_PORDYCBCR_Y0_U_Y1_V = HAT_PIXORD_YUYV(0, 3, 2, 1),  // Y0,  U, Y1,  V
    HAT_PORDYCBCR_Y1_Y0_V_U = HAT_PIXORD_YUYV(1, 2, 0, 3),  // Y1, Y0,  V,  U
    HAT_PORDYCBCR_Y1_Y0_U_V = HAT_PIXORD_YUYV(1, 3, 0, 2),  // Y1, Y0,  U,  V
    HAT_PORDYCBCR_Y1_V_U_Y0 = HAT_PIXORD_YUYV(3, 1, 0, 2),  // Y1,  V,  U, Y0
    HAT_PORDYCBCR_Y1_V_Y0_U = HAT_PIXORD_YUYV(2, 1, 0, 3),  // Y1,  V, Y0,  U
    HAT_PORDYCBCR_Y1_U_V_Y0 = HAT_PIXORD_YUYV(3, 2, 0, 1),  // Y1,  U,  V, Y0
    HAT_PORDYCBCR_Y1_U_Y0_V = HAT_PIXORD_YUYV(2, 3, 0, 1),  // Y1,  U, Y0,  V
    HAT_PORDYCBCR_V_Y0_U_Y1 = HAT_PIXORD_YUYV(1, 0, 3, 2),  //  V, Y0,  U, Y1
    HAT_PORDYCBCR_V_Y0_Y1_U = HAT_PIXORD_YUYV(1, 0, 2, 3),  //  V, Y0, Y1,  U
    HAT_PORDYCBCR_V_Y1_U_Y0 = HAT_PIXORD_YUYV(3, 0, 1, 2),  //  V, Y1,  U, Y0
    HAT_PORDYCBCR_V_Y1_Y0_U = HAT_PIXORD_YUYV(2, 0, 1, 3),  //  V, Y1, Y0,  U
    HAT_PORDYCBCR_V_U_Y0_Y1 = HAT_PIXORD_YUYV(2, 0, 3, 1),  //  V,  U, Y0, Y1
    HAT_PORDYCBCR_V_U_Y1_Y0 = HAT_PIXORD_YUYV(3, 0, 2, 1),  //  V,  U, Y1, Y0
    HAT_PORDYCBCR_U_Y0_V_Y1 = HAT_PIXORD_YUYV(1, 2, 3, 0),  //  U, Y0,  V, Y1
    HAT_PORDYCBCR_U_Y0_Y1_V = HAT_PIXORD_YUYV(1, 3, 2, 0),  //  U, Y0, Y1,  V
    HAT_PORDYCBCR_U_Y1_V_Y0 = HAT_PIXORD_YUYV(3, 2, 1, 0),  //  U, Y1,  V, Y0
    HAT_PORDYCBCR_U_Y1_Y0_V = HAT_PIXORD_YUYV(2, 3, 1, 0),  //  U, Y1, Y0,  V
    HAT_PORDYCBCR_U_V_Y0_Y1 = HAT_PIXORD_YUYV(2, 1, 3, 0),  //  U,  V, Y0, Y1
    HAT_PORDYCBCR_U_V_Y1_Y0 = HAT_PIXORD_YUYV(3, 1, 2, 0),  //  U,  V, Y1, Y0
}hat_pix_order_yuyv_t;

typedef enum {
    HAT_PIXYUV_NV12 = HAT_PIXFMT_SYUV(HAT_UVPKD_PACKED, HAT_XYDEC_XY, HAT_PORDYUV_Y_V_U),
    HAT_PIXYUV_NV21 = HAT_PIXFMT_SYUV(HAT_UVPKD_PACKED, HAT_XYDEC_XY, HAT_PORDYUV_Y_U_V),
}hat_pixfmt_yuv_t;

typedef union {
    uint8 all;
    struct {
        uint8 R_R_Y:2;
        uint8 Gr_G_U:2;
        uint8 Gb_B_V:2;
        uint8 B:2;
    };
}hat_pix_order_t;

typedef union {
    hat_pixel_fmt_t dsc_full;                // Full color pixel format
    struct {
        hat_pix_order_t order;      // Order of colors in pixel
        uint8 reserved0:1;              // Reserved
        hat_xy_decim_t xydec:2;      // X/Y Decimation
        hat_uv_packed_t uvpkd:2;     // U/V Interleave/Line/Plane
        hat_packed_fmt_t pkd:1;      // Packed formats (Interleave/Planar)
        hat_color_fmt_t fmt:2;       // Color types (BAYER, RGB or YUV)
    };
}hat_pix_dsc_t;

typedef enum  {
    HAT_COMPRESSION_NONE,
    HAT_COMPRESSION_DPCM,
    HAT_COMPRESSION_DPCM_QCOM,
    HAT_COMPRESSION_DPCM_MIPI,
    HAT_COMPRESSION_PACKED,
    HAT_COMPRESSION_NONE_16BIT,
    HAT_COMPRESSION_A_LOW,
    HAT_COMPRESSION_u_LOW
}hat_pix_compression_t;

typedef union {
    uint32 fmt_full;
    struct {
        hat_pix_dsc_t fmt;                       // Full color pixel format
        uint8 bpc;                               // [U8Q0] bits per color
        hat_pix_compression_t compression:8; // format compression
    };
} hat_pix_fmt_t;

#endif // __HAL_TYPES_PIXEL_FORMAT_H__

