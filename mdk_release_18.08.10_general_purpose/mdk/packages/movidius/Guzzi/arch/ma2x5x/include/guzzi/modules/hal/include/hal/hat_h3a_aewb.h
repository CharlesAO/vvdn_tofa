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
* @file hat_h3a_aewb.h
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

#ifndef __HAL_TYPES_H3A_AEWB_H__
#define __HAL_TYPES_H3A_AEWB_H__

#include "hal/hat_types.h"

#define HAT_H3A_AEWB_PAX_MAX_V      (64)    // Maximum number of paxels in horizontal direction
#define HAT_H3A_AEWB_PAX_MAX_H      (64)    // Maximum number of paxels in vertical direction
#define HAT_H3A_AEWB_PAX_MAX        (HAT_H3A_AEWB_PAX_MAX_H * HAT_H3A_AEWB_PAX_MAX_V)

/* AEWB configuration
 * Rules:
 *    "Pixel"      - one bayer pixel (R or Gr or Gb or B)
 *
 *    "Macropixel" - matrix 2 x 2 of bayer pixels (R + Gr + Gb + B)
 *      PAXEL_BOX = 2 x 2 = 4 [pixels]
 *
 *    "Paxel"      - even number by horizontal and vertical of "Macropixel"
 *      PAXEL.w = (1 Macropixel) * pix.w = 2 * pix.w [pixels]
 *      PAXEL.h = (1 Macropixel) * pix.h = 2 * pix.h [pixels]
 *
 *    "Window"     - horizontal nad vetrical "paxels"
 *      WINDOW.w = PAXEL.w * pax.w [pixels]
 *      WINDOW.h = PAXEL.h * pax.h [pixels]
 *
 *    "Margin"     - Margin outside of statistics window
 *      MARGIN.x = (IMAGE.w - WINDOW.w) / 2 [pixels]
 *      MARGIN.y = (IMAGE.h - WINDOW.h) / 2 [pixels]
 */
typedef struct  {
//    uint32 enable;
    hat_rect_float_t win;        // Position and size of window
    hat_dim_2d_t pax;            // Count of "paxels" in window
    float unsat_threshold;      // Pixel value of threshold relative to max pixel size
}hat_h3a_aewb_cfg_t;

/* AEWB paxel statistics
 *   Format of data:
 *      r, gr, gb, b - Summing of all pixels, normalized
 *                     to number of pixels in paxel : ( 0 - 1.0 )
 */
typedef struct  {
    float r;            // Normalized (divided by pixels in paxel)
    float r_usat;      // Relative to pixels in paxel
    float gr;           // Normalized (divided by pixels in paxel)
    float gr_usat;      // Relative to pixels in paxel
    float gb;           // Normalized (divided by pixels in paxel)
    float gb_usat;      // Relative to pixels in paxel
    float b;            // Normalized (divided by pixels in paxel)
    float b_usat;      // Relative to pixels in paxel
    uint32 time_st;     // Relative to start of frame readout [ns]
}hat_h3a_aewb_entry_t;

/* AEWB buffer statistics */
typedef struct  {
    hat_timestamp_t       ts;            // Time stamp of statistics (Start of readout)
    hat_h3a_aewb_cfg_t    cfg;
    hat_h3a_aewb_entry_t *paxels;
}hat_h3a_aewb_stat_t;

#endif // __HAL_TYPES_H3A_AEWB_H__

