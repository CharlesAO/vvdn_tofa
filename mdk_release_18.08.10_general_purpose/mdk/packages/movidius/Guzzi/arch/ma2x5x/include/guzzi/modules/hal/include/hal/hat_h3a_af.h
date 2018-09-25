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
* @file hat_h3a_af.h
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

#ifndef __HAL_TYPES_H3A_AF_H__
#define __HAL_TYPES_H3A_AF_H__

#include "hal/hat_types.h"
#define HAT_H3A_AF_PAX_MAX_V    (64)    // Maximum number of paxels in horizontal direction
#define HAT_H3A_AF_PAX_MAX_H    (64)    // Maximum number of paxels in vertical direction
#define HAT_H3A_AF_PAX_MAX      (HAT_H3A_AF_PAX_MAX_H * HAT_H3A_AF_PAX_MAX_V)

/* Filter configuration
 *   Horizontal filter: size = {x = ceficient count, y = 1}
 *   Vertical filter:   size = {x = 1, y = ceficient count}
 *   Box filter:        size = {x = coefficient count, y = coefficient count}
 */
typedef struct {
    hat_strength_t filter_strength;      // TODO Value of filter strength
} hat_h3a_af_filter_cfg_t;


/* AF configuration
 * Rules:
 *    "Pixel"      - one bayer pixel (R or Gr or Gb or B)
 *
 *    "Macropixel" - matrix 2 x 2 of bayer pixels (R + Gr + Gb + B)
 *      PAXEL_BOX = 2 x 2 = 4 [pixels]
 *
 *    "Paxel"      - even number by horizontal and vertical of "pixel boxes"
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
typedef struct
{
    hat_rect_float_t win;          // Position and size of window
    hat_dim_2d_t     pax;          // Count of "paxels" in window
    hat_h3a_af_filter_cfg_t filter_strong;
    hat_h3a_af_filter_cfg_t filter_weak;
}hat_h3a_af_cfg_t;

/* AF filter statistics */
typedef struct {
    float sum;
    float max;
} hat_h3a_af_paxel_stat_t;

/* AF filter statistics */
typedef struct {
//    hat_h3a_af_paxel_stat_t r;
//    hat_h3a_af_paxel_stat_t gr;
//    hat_h3a_af_paxel_stat_t gb;
//    hat_h3a_af_paxel_stat_t b;

    hat_h3a_af_paxel_stat_t val_filter_strong;  // Only green pixels are used
    hat_h3a_af_paxel_stat_t val_filter_weak;    // Only green pixels are used
    float avr_pixel;
    uint32 time_st;                             // Relative to start of frame readout [ns]
    float lens_position;                        // Lens position relative to focus range (far<->near) [0.0 .. 1.0]
}  hat_h3a_af_stat_entry_t;

/* AF buffer statistics */
typedef struct {
    uint32                  seq_cnt;       // Focus iteration sequence counter
    hat_timestamp_t         ts;            // Time stamp of statistics
    hat_h3a_af_cfg_t        cfg;
    uint32                  exposure;      // U32Q0 Exposure time in us
    float                   curr_lens_pos;
    hat_h3a_af_stat_entry_t *paxels;
    float 					actual_lens_position;
}hat_h3a_af_stat_t;

#endif // __HAL_TYPES_H3A_AF_H__

