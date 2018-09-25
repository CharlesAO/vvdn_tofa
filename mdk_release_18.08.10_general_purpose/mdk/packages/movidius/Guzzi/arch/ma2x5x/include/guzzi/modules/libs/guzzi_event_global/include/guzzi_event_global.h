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
* @file guzzi_event_global.h
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

#ifndef _GUZZI_EVENT_GLOBAL_H_
#define _GUZZI_EVENT_GLOBAL_H_

#include <guzzi_event/include/guzzi_event.h>

enum {
    CAM_EVT_SOF,                // 0x00
    CAM_EVT_AFD_READY,          // 0x01
    CAM_EVT_AF_READY,           // 0x02
    CAM_EVT_AE_READY,           // 0x03
    CAM_EVT_AE_STAB_READY,      // 0x04
    CAM_EVT_AWB_READY,          // 0x05
    CAM_EVT_SEN_MODE_CHANGE,    // 0x06
    CAM_EVT_DETECTED_CAMERA,    // 0x07
    CAM_EVT_BUFF_LOCKED,        // 0x08
    CAM_EVT_FMV_READY,          // 0x09
    CAM_EVT_FR_ISP_REACHED,     // 0x0A FR reached ISP
    CAM_EVT_FR_ISP_DONE,        // 0x0B ISP is done
    CAM_EVT_LRT_STARTED,        // 0x0C
    CAM_EVT_MOVE_LENS,          // 0x0D
    CAM_EVT_AE_MERGER_READY,    // 0x0E Golbal - for all cameras
    CAM_EVT_AWB_MERGER_READY,   // 0x0F
    MAX_CAM_EVT
};

#define CAM_EVT_EID_SHIFT 0
#define CAM_EVT_EID_MASK (0xFFFF << CAM_EVT_EID_SHIFT)
#define CAM_EVT_CID_SHIFT 16
#define CAM_EVT_CID_MASK (0xFFFF << CAM_EVT_CID_SHIFT)

#define geg_camera_event_mk(cid, eid) \
( \
      (((cid) << CAM_EVT_CID_SHIFT) & CAM_EVT_CID_MASK) \
    | (((eid) << CAM_EVT_EID_SHIFT) & CAM_EVT_EID_MASK) \
)

#define geg_camera_event_get_cid(camera_event) \
( \
    ((camera_event) & CAM_EVT_CID_MASK) >> CAM_EVT_CID_SHIFT \
)

#define geg_camera_event_get_eid(camera_event) \
( \
    ((camera_event) & CAM_EVT_EID_MASK) >> CAM_EVT_EID_SHIFT \
)

const char * guzzi_event_global_id2str(int id);

guzzi_event_t *guzzi_event_global(void);

void guzzi_event_global_destroy(void);
void guzzi_event_global_ctreate(void);

#endif /* _GUZZI_EVENT_GLOBAL_H_ */
