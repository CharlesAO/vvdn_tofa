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
* @file dtp_pproc_defs.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 17-Oct-2014 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef DTP_PPROC_DEFS_H_
#define DTP_PPROC_DEFS_H_

#include <hal/hat_matrix.h>
#include <hal/hat_gamma.h>

typedef enum {
    PPROC_VALID_RGB2RGB = 1 << 0,
    PPROC_VALID_RGB2YUV = 1 << 1,
    PPROC_VALID_R_GAMMA = 1 << 2,
    PPROC_VALID_G_GAMMA = 1 << 3,
    PPROC_VALID_B_GAMMA = 1 << 4,
    MAX_PPROC_VALID         = PPROC_VALID_B_GAMMA,
} pproc_validity_t;

typedef struct {
    pproc_validity_t    valid_params;
    hat_rgb2rgb_t       rgb2rgb;
    hat_rgb2yuv_t       rgb2yuv;
    hat_gamma_color_t   r_gamma;
    hat_gamma_color_t   g_gamma;
    hat_gamma_color_t   b_gamma;
} pproc_param_t;

#endif /* DTP_PPROC_DEFS_H_ */
