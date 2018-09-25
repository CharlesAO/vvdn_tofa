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
* @file dtp_server_defs.h
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

#ifndef __DTP_SERVER_DEFS_PUBLIC_H__
#define __DTP_SERVER_DEFS_PUBLIC_H__

#include <osal/osal_stdtypes.h>
#include <gzz_effects_types.h>
#include <gzz_scene_types.h>
#include <dtp_ids.h>

#include "dtp_scene_defs.h"
#include "dtp_effects_defs.h"
#include "dtp_pproc_defs.h"

// Data base IDs
typedef enum {
    DTP_DB_ID_VPIPE,
    DTP_DB_ID_ACA,
    DTP_DB_ID_SG,
    DTP_DB_ID_CL,
    DTP_DB_ID_CD,
    DTP_DB_ID_SYS,
    DTP_DB_ID_IPIPE,
    DTP_DB_ID_DTP_SERV = 1 << 10,
    DTP_DB_ID_CHECK_SUM,
}dtpdb_id_t;

// Abstract Image pipe algos IDs
typedef enum {
    DTP_ID_VPIPE_DCSUB,
    DTP_ID_VPIPE_DPC,
    DTP_ID_VPIPE_GRGB,
    DTP_ID_VPIPE_LSC,
    DTP_ID_VPIPE_H3A_AEWB_CONF,
    DTP_ID_VPIPE_H3A_AF_CONF,
    DTP_ID_VPIPE_CFAI,
    DTP_ID_VPIPE_GAMMA,
    DTP_ID_VPIPE_RGB2RGB,
    DTP_ID_VPIPE_RGB2YUV,
    DTP_ID_VPIPE_LNF,
    DTP_ID_VPIPE_SHARPNESS,
    DTP_ID_VPIPE_CNF,
    DTP_ID_VPIPE_LTM,
    DTP_ID_VPIPE_POST_PROC,
    DTP_ID_VPIPE_DOG,
} dtpdb_vpipe_id_t;

// Automatic Camera Algorithms algos IDs
typedef enum {
    DTP_ID_ACA_ALG_AE_CALC,
    DTP_ID_ACA_ALG_AE_STAB,
    DTP_ID_ACA_ALG_AWB_CALC,
    DTP_ID_ACA_ALG_AWB_STAB,
    DTP_ID_ACA_ALG_AF_AFFW,
    DTP_ID_ACA_ALG_AF_SAF,
    DTP_ID_ACA_ALG_AF_CAF,
    DTP_ID_ACA_ALG_AFD_CALC,
    DTP_ID_ACA_ALG_HLLC,
    DTP_ID_ACA_ALG_FMV,
    DTP_ID_ACA_ALG_AE_MERGE,
    DTP_ID_ACA_ALG_AWB_MERGE,
} dtpdb_aca_id_t;

// Settings Generator algos ID
typedef enum {
    DTP_ID_SG_ALG_AE_SMOOTH,
    DTP_ID_SG_ALG_AE_LIMITS,
    DTP_ID_SG_ALG_AE_DISTRIB,
    DTP_ID_SG_ALG_AFD_DISTRIB,
    DTP_ID_SG_ALG_AWB,
    DTP_ID_SG_ALG_AF_AFFW,
    DTP_ID_SG_ALG_AF_HLLC,
    DTP_ID_SG_ALG_SCENES,
    DTP_ID_SG_ALG_FLASH_GEN,
} dtpdb_sg_id_t;

// Control Logic algos IDs
typedef enum {
    DTP_ID_CL_FSM_PREVIEW,
    DTP_ID_CL_FSM_CAPTURE,
} dtpdb_cl_id_t;

// Camera Driver algos IDs
typedef enum {
    DTP_ID_CD_CAMERA_MODULE,
    DTP_ID_CD_SENSOR,
    DTP_ID_CD_LENS,
    DTP_ID_CD_LIGHTS,
    DTP_ID_CD_NVM,
} dtpdb_cd_id_t;

// System algos IDs
typedef enum {
    DTP_ID_SYS_CONFIG,
    DTP_ID_SYS_CAM_MODE,
    DTP_ID_SYS_PIPE,
    DTP_ID_SYS_HIF,
    DTP_ID_SYS_SCENE,
    DTP_ID_SYS_EFFECTS,
} dtpdb_sys_id_t;

// Image pipe components
typedef enum {
    DTP_ID_IPIPE_DPC,
    DTP_ID_IPIPE_GRGB,
    DTP_ID_IPIPE_DEMOSAIC,
    DTP_ID_IPIPE_GEN_CHROMA,
    DTP_ID_IPIPE_LUMMA_NF,
    DTP_ID_IPIPE_LTM,
    DTP_ID_IPIPE_CHROMA_MEDIAN,
    DTP_ID_IPIPE_CHROMA_NF,
    DTP_ID_IPIPE_SHARPENING,
    DTP_ID_IPIPE_FOCUS_FILTERS,
} dtpdb_ipipe_id_t;

// Static part
// -------------

typedef enum {                     // --- enumerate ---
    DTP_COMMON_PAR_SENSOR_ID = 0,   // ID for Sensor ID, [0 .. 4294967295]
    DTP_COMMON_PAR_MODE,            // ID for Current camera mode, [0 .. 64]
    DTP_COMMON_PAR_EFFECT,          // ID for Current effect, [0 .. 64]
    DTP_COMMON_PAR_SCENE,           // ID for Current scene mode, [0 .. 64]
} dtpdb_static_par_id_t;


typedef union {
    uint32 params[8];
    struct {                       //  --- enumerate ---
        uint32 camera_id;           // Camera ID, [0 .. 4294967295]
        uint32 mode;                // Current camera mode, [0 .. 64]
        uint32 effect;              // Current effect, [0 .. 64]
        uint32 scene;               // Current scene mode, [0 .. 64]
    };
} dtpdb_static_common_t;


typedef union {
    uint32 params[8];
    struct {
        uint32 par_0;           // Reserved
        uint32 par_1;           // Reserved
        uint32 par_2;           // Reserved
        uint32 par_3;           // Reserved
        uint32 par_4;           // Reserved
        uint32 par_5;           // Reserved
        uint32 par_6;           // Reserved
        uint32 par_7;           // Reserved
    };
} dtpdb_static_private_t;


// Dynamic part
// -------------

typedef enum {                     // --- Ranges ---
    DTP_COMMON_PAR_EXPOSURE = 200,  // ID for AE Exposure Time
    DTP_COMMON_PAR_AGIAN,           // ID for AE Analog Gain
    DTP_COMMON_PAR_DGAIN,           // ID for Total digital gain applied in image ipipe
    DTP_COMMON_PAR_TOT_EXPOSURE,    // ID for AE Required Total Exposure
    DTP_COMMON_PAR_COLOR_TEMP,      // ID for AWB Color Temperature
    DTP_COMMON_PAR_AV_Y_LEVEV,      // ID for Average Y level [0 .. 255]
    DTP_COMMON_PAR_FALSH_RATIO,     // ID for AE Flash Ratio
    DTP_COMMON_PAR_OBJ_DISTANCE,    // ID for AF Focus Distance
    MAX_COMMON_PAR
} dtpdb_dynamic_par_id_t;

typedef union {
    uint32 params[8];
    struct {                   //  --- Ranges ---
        uint32 exposure;        // AE Exposure Time, [usec], [1 .. 1000000] ( 1sec)
        uint32 agian;           // AE Analog Gain  (1.0 = 256) [256 .. 16384]
        uint32 dgain;           // Total digital gain applied in image ipipe (1.0 = 256) [256 .. 16384]
        uint32 tot_exposure;    // AE Required Total Exposure [usec], [1 - 4294967295]
        uint32 color_temp;      // AWB Color Temperature K, [1000 .. 20000]
        uint32 ave_y_level;     // Average Y level, [0 .. 255] for 8 bit image
        uint32 falsh_ratio;     // AE Flash Ratio, %, [0 .. 100]
        uint32 obj_distance;    // AF Focus Distance, [mm], [1 .. 5000]
    };
} dtpdb_dynamic_common_t;

typedef union {
    uint32 params[8];
    struct {
        uint32 par_0;           // Reserved
        uint32 par_1;           // Reserved
        uint32 par_2;           // Reserved
        uint32 par_3;           // Reserved
        uint32 par_4;           // Reserved
        uint32 par_5;           // Reserved
        uint32 par_6;           // Reserved
        uint32 par_7;           // Reserved
    };
} dtpdb_dynamic_private_t;

typedef struct {
    dtpdb_static_common_t st_common;
    dtpdb_static_private_t st_priv;
    dtpdb_dynamic_common_t dy_common;
    dtpdb_dynamic_private_t dy_priv;
} db_ids_order_t;

typedef struct {
    uint32 leaf_ver;
    uint32 leaf_size;
    void* leaf_private_data;
} dtp_leaf_data_t;

typedef enum {
    DTP_STATIC_COMMON_PARAM,
    DTP_STATIC_PRIVATE_PARAM,
    DTP_DYNAMIC_COMMON_PARAM,
    DTP_DYNAMIC_PRIVATE_PARAM,
    DTP_TYPE_PARAM_LAST
} dtpdb_param_id_type_t;

/*
 *  Fixed and interpolated data
 */

typedef struct {
	uint32 param0_id;
    void* p_data0;
}dtp_fixed_data_t;

typedef struct {
	uint32 param0_id;
    uint32 param_min;
    uint32 param_max;
    void* p_data0;
    void* p_data1;
}dtp_1D_interp_data_t;


/*
                 param_min0                      param_max0
    param_min1      +-------------------------------+
                    | p_data_min0_min1              | p_data_max0_min1
                    |                               |
                    |                               |
                    |                               |
                    |                               |
                    |                               |
    param_max1      +-------------------------------+
                     p_data_min0_max1                p_data_max0_max1


*/

typedef struct {
	uint32 param0_id;
	uint32 param1_id;
    uint32 param0_min;
    uint32 param0_max;
    uint32 param1_min;
    uint32 param1_max;
    void* p_data_0_min__1_min;
    void* p_data_0_max__1_min;
    void* p_data_0_min__1_max;
    void* p_data_0_max__1_max;
}dtp_2D_interp_data_t;

typedef enum {
    DTP_DAT_TYPE_FIXED,
    DTP_DAT_TYPE_1D_FIXED,
    DTP_DAT_TYPE_1D_INTERP,
    DTP_DAT_TYPE_2D_FIXED,
    DTP_DAT_TYPE_2D_INTERP,
}dtp_out_data_type_t;


typedef struct {
    uint32 data_type_id;
    union {
        dtp_fixed_data_t d_fixed;
        dtp_1D_interp_data_t d_1D_interp;
        dtp_2D_interp_data_t d_2D_interp;
    };
}dtp_out_data_t;

int interp_1D_calc_k(uint32 param_curr, dtp_out_data_t *p_data, float *pk0, float *pk1);

int interp_2D_calc_k(uint32 curr_param0, uint32 curr_param1, dtp_out_data_t *p_data,
                     float *pk_0_min__1_min,
                     float *pk_0_max__1_min,
                     float *pk_0_min__1_max,
                     float *pk_0_max__1_max);

typedef void *dtp_server_hndl_t;

typedef void *dtp_hndl_t;      // Returned by DTP sever after

int dtpsrv_create(dtp_server_hndl_t *ctx);

int dtpsrv_destroy(dtp_server_hndl_t ctx);

int dtpsrv_import_db(dtp_server_hndl_t ctx, void *db_ptr, int size);

extern const db_ids_order_t dtp_param_order;
extern dtp_server_hndl_t  dtp_srv_hndl;

int dtpsrv_get_hndl(dtp_server_hndl_t ctx,
        dtpdb_id_t db_id,
        uint32 alg_id,
        uint32 alg_subtype_id,
        db_ids_order_t order,
        dtp_hndl_t *dtp_hndl,
        dtp_leaf_data_t *leaf);

int dtpsrv_pre_process(dtp_hndl_t dtp_hndl,
        dtpdb_static_common_t *p_static_common,
        dtpdb_static_private_t *p_static_private);

int dtpsrv_process(dtp_hndl_t dtp_hndl,
        dtpdb_dynamic_common_t *p_dynamic_common,
        dtpdb_dynamic_private_t *p_dynamic_private,
        void ** p_data);

int dtpsrv_free_hndl(dtp_hndl_t dtp_hndl);

/*
 *  Get current value of parameter
 *  This is needed for interpolation
 */
int dtpsrv_get_dynamic_param_value(dtp_hndl_t dtp_hndl,
                                       uint32 id,
                                       dtpdb_dynamic_common_t *p_d_comm,
                                       dtpdb_dynamic_private_t *p_d_priv,
                                       uint32 *p_value);

int dtpsrv_interp_1D_calc_k(uint32 param_curr, dtp_out_data_t *p_data, float *pk0, float *pk1);

int dtpsrv_interp_2D_calc_k(uint32 curr_param0, uint32 curr_param1, dtp_out_data_t *p_data,
                     float *pk_0_min__1_min,
                     float *pk_0_max__1_min,
                     float *pk_0_min__1_max,
                     float *pk_0_max__1_max);




#endif // __DTP_SERVER_DEFS_PUBLIC_H__

