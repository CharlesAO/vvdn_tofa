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
* @file hat_img_fmt.h
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

#ifndef __HAL_TYPES_IMAGE_FORMAT_H__
#define __HAL_TYPES_IMAGE_FORMAT_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>

#include <hal/hat_types.h>
#include <hal/hat_pix_fmt.h>

typedef struct  {
    hat_pix_fmt_t pix;     // Pixel format
    hat_size_t size;       // [pixels] Resolution of image (width & height)
    hat_size_t padding;    // [pixels] Padding by horizontal and verical
    uint32 planar_padding;      // [Pixels] Padding between two planar buffers
    uint32 mem_size;            // Memory size in bytes
}hat_img_fmt_t;

typedef struct  {
    hat_range_t range;     // Valid range of pixel value
    hat_size_t szcol0;     // Size for color 0 (R, G, B, Gr, Gb, Y)
    hat_size_t szcol1;     // Size for color 1 (U, V)
    hat_size_t xydec;      // X/Y Decimation
    hat_pix_dsc_t dsc;      // Pixel description separated by field
    hat_color_lrg_t pofs;   // Pixel offset [in bytes]
    hat_color_lrg_t ppln;   // Pixels per line
    hat_color_lrg_t ppcol;  // Pixels per color (next same color)
    hat_color_lrg_t ppwrap; // Pixels per wrap to the next line
    hat_color_lrg_t plsz;   // Count of pixels for plane (+padding)
    uint32 lines;               // Count of pixels for line (+padding)
    uint32 bytespp;             // Count of bytes per pixel
    uint32 bytespc;             // Count of bytes per color
}hat_img_geometry_t;

#endif // __HAL_TYPES_IMAGE_FORMAT_H__

