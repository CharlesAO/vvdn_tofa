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
* @file guzzi_event.h
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

#ifndef _GUZZI_EVENT_H_
#define _GUZZI_EVENT_H_

#include <osal/osal_stdtypes.h>

typedef struct guzzi_event guzzi_event_t;
typedef struct guzzi_event_create_params guzzi_event_create_params_t;

struct guzzi_event_create_params {
    int max_events;
};

typedef void (*guzzi_event_notify_t)(
        void *prv,
        int event_id,
        uint32 num,
        void *data
    );

guzzi_event_t * guzzi_event_create(
        guzzi_event_create_params_t *create_params
    );
void guzzi_event_destroy(guzzi_event_t *evt);

int guzzi_event_reg_recipient(
        guzzi_event_t *evt,
        int event_id,
        guzzi_event_notify_t notify,
        void *prv
    );

int guzzi_event_unreg_recipient(
        guzzi_event_t *evt,
        int event_id,
        guzzi_event_notify_t notify,
        void *prv
    );

int guzzi_event_send(
        guzzi_event_t *evt,
        int event_id,
        uint32 num,
        void *data
    );

#endif /* _GUZZI_EVENT_H_ */

