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
* @file camera.h
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

#ifndef _CAMERA_H
#define _CAMERA_H

#include <camera_common.h>
#include <camera_config_index.h>

typedef enum {
    CAMERA_EVENT_ERROR,
    CAMERA_EVENT_ERROR_START,
    CAMERA_EVENT_START_DONE,
    CAMERA_EVENT_STOP_DONE,
    CAMERA_EVENT_BUFFER_FLUSH,
    CAMERA_EVENT_FLUSH_DONE,
    CAMERA_EVENT_PROCESS_DONE,
    CAMERA_EVENT_GENERIC
} camera_event_id_t;

typedef union {
    unsigned int v;
    struct {
        unsigned int sub_type:8;
        camera_event_id_t type:8;
    };
} camera_event_t;

typedef void (*camera_callback_t)(
        camera_t *camera,
        void *app_prv,
        camera_event_t event,
        int num,
        void *ptr
    );

typedef struct {
    int camera_id;
    camera_modes_t mode;
    void *dtp_server;
    camera_callback_t callback;
    void *app_prv;
} camera_create_params_t;

int camera_get_id(camera_t *camera);

int camera_config_get(
        camera_t *camera,
        camera_config_index_t index,
        void *config
    );

int camera_config_set_begin(camera_t *camera);
int camera_config_set(
        camera_t *camera,
        camera_config_index_t index,
        void *config
    );
int camera_config_set_end(camera_t *camera);
int camera_config_set_cancel(camera_t *camera);

int camera_start(camera_t *camera);
int camera_stop(camera_t *camera);
int camera_flush(camera_t *camera);
int camera_process(camera_t *camera, int port, void *buf);
void camera_destroy(camera_t *camera);
camera_t *camera_create(camera_create_params_t *params);

#endif /* _CAMERA_H */

