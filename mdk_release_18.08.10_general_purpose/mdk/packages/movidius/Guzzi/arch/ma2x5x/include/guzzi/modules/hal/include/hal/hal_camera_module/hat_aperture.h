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
* @file hat_aperture.h
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

#ifndef __HAL_TYPES_APERTURE_H__
#define __HAL_TYPES_APERTURE_H__

#include <hal/hat_types.h>

struct hat_lens_shutter {
    int isopen;
};

typedef struct {
    uint32 present;     // 0 - no shutter 1 - present
    uint32 open_time;   // close to open time
    uint32 close_time;  // open to close time
}hat_shutter_desc_t;

/* Iris */
typedef struct {
    uint32 open_time;
    uint32 close_time;
    uint32 f_no;    // f/no U32Q16
    uint32 ratio;   // light reduction ratio U32Q16
}hat_iris_desc_t;

typedef struct {
    uint32 num_iris;           // list size. Minimum 1 element
    hat_iris_desc_t *irisf; // pointer to list
}hat_lens_iris_t;

/* Lens features */
typedef struct {
    hat_shutter_desc_t shutter;
    hat_lens_iris_t iris;
}hat_lens_features;


/* Lens configuration structure */
typedef struct {
    struct hat_lens_shutter shutter;
    uint32 iris_index; // index in hat_iris_desc_t list - up to num_iris
} hat_lens_ctrl_t;

typedef struct {
    struct hat_lens_shutter shutter;
    uint32          iris_index; // index in hat_iris_desc_t list - up to num_iris
} hat_lens_state_t;

#endif // __HAL_TYPES_APERTURE_H__

