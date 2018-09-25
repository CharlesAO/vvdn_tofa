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
* @file hat_cm_nvm_data_struct_helpers.h
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

#ifndef _HAL_CM_NVM_DATA_STRUCT_HELPERS_H
#define _HAL_CM_NVM_DATA_STRUCT_HELPERS_H

#include <osal/osal_stdlib.h>
#include <hal/hal_camera_module/hat_cm_nvm_data_struct.h>

#define NVM_INFO_SIZE_MAX (8*1024)

/*
 * ****************************************************************************
 * ** Size helpers ************************************************************
 * ****************************************************************************
 */
#define nvm_info_size_of_nvm_info(nvm_info) \
( \
      sizeof (nvm_info_t) \
    + nvm_info_size_of_center_spot(nvm_info) \
    + nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
    + nvm_info_size_of_lsc_spot_sens(nvm_info) \
    + nvm_info_size_of_dpix_info(nvm_info) \
)

#define nvm_info_size_of_center_spot(nvm_info) \
( \
      sizeof (nvm_center_spot_t) \
    * (nvm_info)->center.spots_uarr_size \
)

#define nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
( \
      sizeof (nvm_lsc_spot_sens_grid_t) \
    * (nvm_info)->lsc.grids_uarr_size \
)

#define nvm_info_size_of_single_lsc_spot_sens(nvm_info) \
( \
      sizeof (nvm_spot_sens_t) \
    * (nvm_info)->lsc.xsz \
    * (nvm_info)->lsc.ysz \
)

#define nvm_info_size_of_lsc_spot_sens(nvm_info) \
( \
      nvm_info_size_of_single_lsc_spot_sens(nvm_info) \
    * (nvm_info)->lsc.grids_uarr_size \
)

#define nvm_info_size_of_dpix_info(nvm_info) \
( \
      sizeof (dpix_info_t) \
    * (nvm_info)->dpc.dpix_info_uarr_size \
)

/*
 * ****************************************************************************
 * ** Pointer helpers *********************************************************
 * ****************************************************************************
 */
#define nvm_info_ptr_center_spot(nvm_info, n) \
( \
    (nvm_center_spot_t *)( \
            (char *)(nvm_info + 1) \
        ) + (n) \
)

#define nvm_info_ptr_lsc_spot_sens_grid(nvm_info, n) \
( \
    (nvm_lsc_spot_sens_grid_t *)( \
              (char *)(nvm_info + 1) \
            + nvm_info_size_of_center_spot(nvm_info) \
        ) + (n) \
)

#define nvm_info_ptr_lsc_spot_sens(nvm_info, n) \
( \
    (nvm_spot_sens_t *)( \
              (char *)(nvm_info + 1) \
            + nvm_info_size_of_center_spot(nvm_info) \
            + nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
        ) + (n) \
          * (nvm_info)->lsc.xsz \
          * (nvm_info)->lsc.ysz \
)

#define nvm_info_ptr_dpix_info(nvm_info, n) \
( \
    (dpix_info_t *)( \
              (char *)(nvm_info + 1) \
            + nvm_info_size_of_center_spot(nvm_info) \
            + nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
            + nvm_info_size_of_lsc_spot_sens(nvm_info) \
        ) + (n) \
)

static inline void nvm_info_convert_to_single_buffer_with_dst(nvm_info_t *dst, nvm_info_t *src)
{
    int i;

    osal_memcpy(dst, src, sizeof (*dst));

    dst->center.spots = nvm_info_ptr_center_spot(dst, 0);
    osal_memcpy(
            dst->center.spots,
            src->center.spots,
            nvm_info_size_of_center_spot(dst)
        );

    dst->lsc.grids = nvm_info_ptr_lsc_spot_sens_grid(dst, 0);
    osal_memcpy(
            dst->lsc.grids,
            src->lsc.grids,
            nvm_info_size_of_lsc_spot_sens_grid(dst)
        );

    for (i = 0; i < src->lsc.grids_uarr_size; i++) {
        dst->lsc.grids[i].pdata = nvm_info_ptr_lsc_spot_sens(dst, i);
        osal_memcpy(
                dst->lsc.grids[i].pdata,
                src->lsc.grids[i].pdata,
                nvm_info_size_of_single_lsc_spot_sens(dst)
            );
    }

    dst->dpc.dpix_info = nvm_info_ptr_dpix_info(dst, 0);
    osal_memcpy(
            dst->dpc.dpix_info,
            src->dpc.dpix_info,
            nvm_info_size_of_dpix_info(dst)
        );
}

static inline nvm_info_t * nvm_info_convert_to_single_buffer(nvm_info_t *src)
{
    nvm_info_t *dst;
    int nvm_info_size;

    nvm_info_size = nvm_info_size_of_nvm_info(src);
    if (NVM_INFO_SIZE_MAX < nvm_info_size) {
        return NULL;
    }

    dst = osal_calloc(1, nvm_info_size);
    if (!dst) {
        return NULL;
    }

    nvm_info_convert_to_single_buffer_with_dst(dst, src);

    return dst;
}

#endif /* _HAL_CM_NVM_DATA_STRUCT_HELPERS_H */

