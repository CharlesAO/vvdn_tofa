/* =============================================================================
* Copyright (c) 2015 MM Solutions AD
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
* @file dtp_ids.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 27-May-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */
#ifndef __DTP_IDS_H__
#define __DTP_IDS_H__

#define DTP_DUMMY (0)

// Camera IDs
typedef enum {
    DTP_CAM_ID_DUMMY = DTP_DUMMY,        // 0
    DTP_CAM_ID_AR1330,       // 1
    DTP_CAM_ID_IMX214,       // 2
    DTP_CAM_ID_HIGHLANDS,    // 3
    DTP_CAM_ID_IMX208,       // 4
    DTP_CAM_ID_OV13860,      // 5
    DTP_CAM_ID_AR0330,       // 6
    DTP_CAM_ID_OV7251,       // 7
} dtp_cam_module_id_t;

// Sensors IDs
typedef enum {
    DTP_SEN_DUMMY = DTP_DUMMY,  // 0
    DTP_SEN_AR1330,     // 1
    DTP_SEN_IMX214,     // 2
    DTP_SEN_IMX208,     // 3
    DTP_SEN_OV13860,    // 4
    DTP_SEN_AR0330,     // 5
    DTP_SEN_OV7251_1,   // 6
    DTP_SEN_OV7251_2,   // 7
} dtp_sensors_id_t;

// Lenses IDs
typedef enum {
    DTP_LENS_DUMMY = DTP_DUMMY,
    DTP_LENS_APTINA,
    DTP_LENS_DW9714A,
    DTP_LENS_LC898212XA,
} dtp_lenses_id_t;

// Lights IDs
typedef enum {
    DTP_LIGHTS_DUMMY = DTP_DUMMY,
    DTP_LIGHTS_AS3645,
} dtp_lights_id_t;

// NVM IDs
typedef enum {
    DTP_NVM_DUMMY = DTP_DUMMY,
    DTP_NVM_M24C16,               // 1
    DTP_NVM_M24128                // 2
} dtp_nvm_id_t;

#endif //__DTP_IDS_H__
