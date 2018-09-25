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
* @file profile.h
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

#ifndef _PROFILE_H
#define _PROFILE_H

#include <osal/osal_stdtypes.h>

#define PROFILE_DESTROY() \
    profile_destroy(profile)

#define PROFILE_INIT(SIZE, COUNT, CB, PRV) \
    profile = profile_create(SIZE, COUNT, CB, PRV)

#define PROFILE_RELEASE_READY(BUF) \
    profile_release_ready(profile, BUF)

#define PROFILE_DUMP() \
    profile_dump(profile)

#define PROFILE_ADD_RAW(TIME, ID, V1, V2) \
    do { \
        if (mmsdbgis(DL_PROFILE)) { \
            profile_add_raw( \
                    profile, \
                    (profile_time_t)(TIME), \
                    (profile_id_t)(ID), \
                    (profile_val_t)(V1), \
                    (profile_val_t)(V2) \
                ); \
        } \
    } while (0)

#define PROFILE_ADD(ID, V1, V2) \
    PROFILE_ADD_RAW(profile_get_time(), ID, V1, V2)

typedef struct profile profile_t;

typedef void profile_ready_cb_t(
        profile_t *profile,
        void *prv,
        void *buffer,
        unsigned int buffer_size
    );

typedef uint32 profile_time_t;
typedef uint32 profile_id_t;
typedef uint32 profile_val_t;

enum {
    PROFILE_ID_SKIPPED,
    PROFILE_ID_APPLICATION_START = 0x0100,
    PROFILE_ID_SENSOR = 0x0200,
    PROFILE_ID_ALGOS = 0x0300,
    PROFILE_ID_LRT = 0x0400,
    PROFILE_ID_EXT_COMMAND = 0x0500,
    PROFILE_ID_LOS = 0x0600,
    PROFILE_ID_DTP = 0x0700,
    PROFILE_ID_SYS = 0x0800,
};

void profile_release_ready(profile_t *profile, void *buffer);
void profile_dump(profile_t *profile);
profile_time_t profile_get_time(void);
void profile_add_raw(
        profile_t *profile,
        profile_time_t time,
        profile_id_t id,
        profile_val_t v1,
        profile_val_t v2
    );
void profile_destroy(profile_t *profile);
profile_t * profile_create(
        unsigned int buffer_size,
        unsigned int buffers_count,
        profile_ready_cb_t *ready_cb,
        void *prv
    );

extern profile_t *profile;

#endif /* _PROFILE_H */

