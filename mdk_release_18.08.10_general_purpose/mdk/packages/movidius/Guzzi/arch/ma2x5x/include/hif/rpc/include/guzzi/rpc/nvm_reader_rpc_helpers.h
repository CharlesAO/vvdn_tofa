
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
* @file
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

#ifndef _RPC_GUZZI_NVM_READER_HELPERS_H
#define _RPC_GUZZI_NVM_READER_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <osal/osal_stdtypes.h>
#include <osal/osal_stdlib.h>
#include <osal/osal_string.h>

#include <guzzi/rpc/nvm_reader_rpc.h>
#include <hal/hal_camera_module/hat_cm_nvm_data_struct.h>
#include <hal/hal_camera_module/hat_cm_nvm_data_struct_helpers.h>

/*
 * ****************************************************************************
 * ** Helpers - get size of ... By rpc_nvm_info_t or nvm_info_t *************** 
 * ****************************************************************************
 */
#define rpc_nvm_info_size_of_nvm_info(nvm_info) \
( \
      sizeof (rpc_nvm_info_t) \
    + rpc_nvm_info_size_of_center_spot(nvm_info) \
    + rpc_nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
    + rpc_nvm_info_size_of_lsc_spot_sens(nvm_info) \
    + rpc_nvm_info_size_of_dpix_info(nvm_info) \
)

#define rpc_nvm_info_size_of_center_spot(nvm_info) \
( \
      sizeof (rpc_nvm_center_spot_t) \
    * (nvm_info)->center.spots_uarr_size \
)

#define rpc_nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
( \
      sizeof (rpc_nvm_lsc_spot_sens_grid_t) \
    * (nvm_info)->lsc.grids_uarr_size \
)

#define rpc_nvm_info_size_of_single_lsc_spot_sens(nvm_info) \
( \
      sizeof (rpc_nvm_spot_sens_t) \
    * (nvm_info)->lsc.xsz \
    * (nvm_info)->lsc.ysz \
)

#define rpc_nvm_info_size_of_lsc_spot_sens(nvm_info) \
( \
      rpc_nvm_info_size_of_single_lsc_spot_sens(nvm_info) \
    * (nvm_info)->lsc.grids_uarr_size \
)

#define rpc_nvm_info_size_of_dpix_info(nvm_info) \
( \
      sizeof (rpc_nvm_dpix_info_t) \
    * (nvm_info)->dpc.dpix_info_uarr_size \
)

/*
 * ****************************************************************************
 * ** Helpers - get data array pointer ****************************************
 * ****************************************************************************
 */
#define rpc_nvm_info_ptr_center_spot(nvm_info, n) \
( \
    (rpc_nvm_center_spot_t *)( \
            (char *)(nvm_info)->var_size_data \
        ) + (n) \
)

#define rpc_nvm_info_ptr_lsc_spot_sens_grid(nvm_info, n) \
( \
    (rpc_nvm_lsc_spot_sens_grid_t *)( \
              (char *)(nvm_info)->var_size_data \
            + rpc_nvm_info_size_of_center_spot(nvm_info) \
        ) + (n) \
)

#define rpc_nvm_info_ptr_lsc_spot_sens(nvm_info, n) \
( \
    (rpc_nvm_spot_sens_t *)( \
              (char *)(nvm_info)->var_size_data \
            + rpc_nvm_info_size_of_center_spot(nvm_info) \
            + rpc_nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
        ) + (n) \
          * (nvm_info)->lsc.xsz \
          * (nvm_info)->lsc.ysz \
)

#define rpc_nvm_info_ptr_dpix_info(nvm_info, n) \
( \
    (rpc_nvm_dpix_info_t *)( \
              (char *)(nvm_info)->var_size_data \
            + rpc_nvm_info_size_of_center_spot(nvm_info) \
            + rpc_nvm_info_size_of_lsc_spot_sens_grid(nvm_info) \
            + rpc_nvm_info_size_of_lsc_spot_sens(nvm_info) \
        ) + (n) \
)

/*
 * ****************************************************************************
 * ** Helpers - convert nvm_info_t <-> rpc_nvm_info_t *************************
 * ****************************************************************************
 */
#define rpc_nvm_info_copy_field(field) \
    to->field = from->field

#define rpc_nvm_info_copy(TO, FROM)                                 \
do {                                                                \
    typeof(TO) to = TO;                                             \
    typeof(FROM) from = FROM;                                       \
    int i;                                                          \
                                                                    \
    for (i = 0; i < ARRAY_SIZE(from->sensor_specific); i++) {       \
        rpc_nvm_info_copy_field(sensor_specific[i]);                \
    }                                                               \
                                                                    \
    rpc_nvm_info_copy_field(version.manufacturer_id     );          \
    rpc_nvm_info_copy_field(version.sensor_id           );          \
    rpc_nvm_info_copy_field(version.camera_module_ver   );          \
    rpc_nvm_info_copy_field(version.voice_coil_motor_id );          \
    rpc_nvm_info_copy_field(version.module_assembling_id);          \
    rpc_nvm_info_copy_field(version.sensor_ver          );          \
    rpc_nvm_info_copy_field(version.isp_ver             );          \
    rpc_nvm_info_copy_field(version.firmware_ver        );          \
    rpc_nvm_info_copy_field(version.nvm_data_ver        );          \
    rpc_nvm_info_copy_field(version.pcb_fpc_ver         );          \
    rpc_nvm_info_copy_field(version.opt_mech_ver        );          \
    rpc_nvm_info_copy_field(version.shading_data_ver    );          \
    rpc_nvm_info_copy_field(version.register_map        );          \
    for (i = 0; i < ARRAY_SIZE(from->version.sw_version); i++) {	\
        rpc_nvm_info_copy_field(version.sw_version[i]);             \
    }                                                               \
    for (i = 0; i < ARRAY_SIZE(from->version.serial_num); i++) {    \
        rpc_nvm_info_copy_field(version.serial_num[i]);             \
    }                                                               \
                                                                    \
    rpc_nvm_info_copy_field(center.spots_uarr_size      );          \
                                                                    \
    rpc_nvm_info_copy_field(lsc.xsz                     );          \
    rpc_nvm_info_copy_field(lsc.ysz                     );          \
    rpc_nvm_info_copy_field(lsc.grids_uarr_size         );          \
                                                                    \
    rpc_nvm_info_copy_field(positions.start_current     );          \
    rpc_nvm_info_copy_field(positions.inf_pos_typ       );          \
    rpc_nvm_info_copy_field(positions.mac_pos_typ       );          \
    rpc_nvm_info_copy_field(positions.inf_pos_min       );          \
    rpc_nvm_info_copy_field(positions.inf_pos_max       );          \
    rpc_nvm_info_copy_field(positions.mac_pos_min       );          \
    rpc_nvm_info_copy_field(positions.mac_pos_max       );          \
    rpc_nvm_info_copy_field(positions.tal               );          \
    rpc_nvm_info_copy_field(positions.tah               );          \
    rpc_nvm_info_copy_field(positions.tbl               );          \
    rpc_nvm_info_copy_field(positions.tbh               );          \
    rpc_nvm_info_copy_field(positions.period            );          \
    rpc_nvm_info_copy_field(positions.pulse_time        );          \
    rpc_nvm_info_copy_field(positions.pwm_per_um_fwd    );          \
    rpc_nvm_info_copy_field(positions.pwm_per_um_bkwd   );          \
                                                                    \
    for (i = 0; i < ARRAY_SIZE(from->bl_vs_gain); i++) {            \
        rpc_nvm_info_copy_field(bl_vs_gain[i].r );                  \
        rpc_nvm_info_copy_field(bl_vs_gain[i].gr);                  \
        rpc_nvm_info_copy_field(bl_vs_gain[i].gb);                  \
        rpc_nvm_info_copy_field(bl_vs_gain[i].b );                  \
    }                                                               \
                                                                    \
    rpc_nvm_info_copy_field(pedestal                    );          \
                                                                    \
    rpc_nvm_info_copy_field(dpc.dpix_info_uarr_size     );          \
} while (0)

#define rpc_nvm_info_copy_center_pax(center_pax)    \
do {                                                \
    rpc_nvm_info_copy_field(center_pax.valid);      \
    rpc_nvm_info_copy_field(center_pax.x);          \
    rpc_nvm_info_copy_field(center_pax.y);          \
} while (0)

#define rpc_nvm_info_copy_grid(TO, FROM)            \
do {                                                \
    typeof(TO) to = TO;                             \
    typeof(FROM) from = FROM;                       \
                                                    \
    rpc_nvm_info_copy_field(color_temp);            \
    rpc_nvm_info_copy_center_pax(center_pax_r);     \
    rpc_nvm_info_copy_center_pax(center_pax_gr);    \
    rpc_nvm_info_copy_center_pax(center_pax_b);     \
    rpc_nvm_info_copy_center_pax(center_pax_gb);    \
} while (0)

static inline void rpc_nvm_info_convert_to_with_buffer(rpc_nvm_info_t *dst, nvm_info_t *src)
{
    int i;

    rpc_nvm_info_copy(dst, src);

    osal_memcpy(
            rpc_nvm_info_ptr_center_spot(dst, 0),
            src->center.spots,
            rpc_nvm_info_size_of_center_spot(dst)
        );
    for (i = 0; i < src->lsc.grids_uarr_size; i++) {
        rpc_nvm_info_copy_grid(
                rpc_nvm_info_ptr_lsc_spot_sens_grid(dst, i),
                src->lsc.grids + i
            );

        osal_memcpy(
                rpc_nvm_info_ptr_lsc_spot_sens(dst, i),
                src->lsc.grids[i].pdata,
                rpc_nvm_info_size_of_single_lsc_spot_sens(dst)
            );
    }
    osal_memcpy(
            rpc_nvm_info_ptr_dpix_info(dst, 0),
            src->dpc.dpix_info,
            rpc_nvm_info_size_of_dpix_info(dst)
        );
}

static inline rpc_nvm_info_t * rpc_nvm_info_convert_to(nvm_info_t *src)
{
    rpc_nvm_info_t *dst;
    int nvm_info_size;

    nvm_info_size = rpc_nvm_info_size_of_nvm_info(src);
    if (NVM_INFO_SIZE_MAX < nvm_info_size) {
        return NULL;
    }

    dst = osal_malloc(nvm_info_size);
    if (!dst) {
        return NULL;
    }

    rpc_nvm_info_convert_to_with_buffer(dst, src);

    return dst;
}

static inline void rpc_nvm_info_convert_from_with_buffer(nvm_info_t *dst, rpc_nvm_info_t *src)
{
    int i;

    rpc_nvm_info_copy(dst, src);

    dst->center.spots = nvm_info_ptr_center_spot(dst, 0);
    osal_memcpy(
            dst->center.spots,
            rpc_nvm_info_ptr_center_spot(src, 0),
            nvm_info_size_of_center_spot(dst)
        );

    dst->lsc.grids = nvm_info_ptr_lsc_spot_sens_grid(dst, 0);
    for (i = 0; i < src->lsc.grids_uarr_size; i++) {
        rpc_nvm_info_copy_grid(
                dst->lsc.grids + i,
                rpc_nvm_info_ptr_lsc_spot_sens_grid(src, i)
            );
        dst->lsc.grids[i].pdata_uarr_size = src->lsc.xsz * src->lsc.ysz;

        dst->lsc.grids[i].pdata = nvm_info_ptr_lsc_spot_sens(dst, i);
        osal_memcpy(
                dst->lsc.grids[i].pdata,
                rpc_nvm_info_ptr_lsc_spot_sens(src, i),
                nvm_info_size_of_single_lsc_spot_sens(dst)
            );
    }

    dst->dpc.dpix_info = nvm_info_ptr_dpix_info(dst, 0);
    osal_memcpy(
            dst->dpc.dpix_info,
            rpc_nvm_info_ptr_dpix_info(src, 0),
            nvm_info_size_of_dpix_info(dst)
        );
}

static inline nvm_info_t * rpc_nvm_info_convert_from(rpc_nvm_info_t *src)
{
    nvm_info_t *dst;
    int nvm_info_size;

    nvm_info_size = nvm_info_size_of_nvm_info(src);
    if (NVM_INFO_SIZE_MAX < nvm_info_size) {
        return NULL;
    }

    dst = osal_malloc(nvm_info_size);
    if (!dst) {
        return NULL;
    }

    rpc_nvm_info_convert_from_with_buffer(dst, src);

    return dst;
}

#ifdef __cplusplus
}
#endif

#endif /* _RPC_GUZZI_NVM_READER_HELPERS_H */

