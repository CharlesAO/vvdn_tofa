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
* @file hat_sensor_imgif.h
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

#ifndef __HAT_TYPES_IMG_IF_H__
#define __HAT_TYPES_IMG_IF_H__

#include <hal/hat_types.h>
#include <hal/hat_pix_fmt.h>

/* ======================================================================= */
/**
 * enumeration for IMG_IF power state selection
 *
 * @param HAT_IMG_IF2_OFF  power-off state
 * @param HAT_IMG_IF2_ON   power-on state
 * @param HAT_IMG_IF2_LP   low-power state
 */
/* ======================================================================= */
typedef enum {
    HAT_IMG_IF2_OFF,
    HAT_IMG_IF2_ON,
    HAT_IMG_IF2_LP
}hat_img_if_pwr_t;


/* ======================================================================= */
/**
 * IMG_IF physical configuration
 *
 * @param pixel_clock       input pixel clock (the sensor output speed)
 * @param continuous_clock  does clock is present during vertical/horizontal blanking
 * @param img_if_pwr	    IMG_IF power configuration
 *
 * @see hat_img_if_pwr_t
 */
/* ======================================================================= */

typedef struct {
    uint32              pixel_clock;
    uint32              continuous_clock;
    hat_img_if_pwr_t    img_if_pwr;
}hat_img_if_physical_cfg_t;

/**
 * Per-context configuration parameters - information needed to configure the
 * IMG_IF context.
 *
 * @param camera_size; // IMG_IF input size
 * @param crop_window; // IMG_IF output size
 * @param in_format;   // input (sensor) format
 * @param out_format; // if direct memory write is supported by IMG_IF

 * @see hat_size_t
 * @see hat_rect_t
 * @see hat_mipi_format_t
 * @see hat_pix_fmt
 */
typedef struct {
        hat_size_t    camera_size;        // IMG_IF input size
        hat_rect_t    crop_window;        // IMG_IF output size
        hat_pix_fmt_t   in_format;          // input (sensor) format
        hat_pix_fmt_t   out_format;         // if direct memory write is supported by IMG_IF
} hat_img_if_context_cfg_t;

/* ======================================================================= */
/**
 * enumeration for selection of per-context event
 *
 * @param HAT_IMG_IF_EVENT_FS      start of frame event
 * @param HAT_IMG_IF_EVENT_FE      end of frame event
 * @param HAT_IMG_IF_EVENT_LN      reception reached to line number
 * @param HAT_IMG_IF_EVENT_ERROR   event indicating an error from specific context or IMG_IF controller
 *
 */
/* ======================================================================= */
typedef enum {
    HAT_IMG_IF_EVENT_FS,  //!< HAT_IMG_IF_EVENT_FS
    HAT_IMG_IF_EVENT_FE,  //!< HAT_IMG_IF_EVENT_FE
    HAT_IMG_IF_EVENT_LN,  //!< HAT_IMG_IF_EVENT_LN
    HAT_IMG_IF_EVENT_ERROR//!< HAT_IMG_IF_EVENT_ERROR
}hat_img_if_evt_t;

/**
 * Callback invoked at some
 *
 * @param h     handle to IMG_IF controller instance
 * @param ctx   refers to context inside IMG_IF controller
 * @param client_prv client private data
 * @param evt   event type @see hat_img_if_evt_t
 * @param ev_data data specific to event
 * @param data_ptr if the event requires more data it will point to some more complex structure
 *                 (it be available only during this call).
 */
typedef void (*hat_img_if_callback_t) (hat_img_if_client_t h, uint32 ctx, void* client_prv, hat_img_if_evt_t evt, uint32 ev_data, void* data_ptr);


/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_cfg_t)(hat_img_if_client_t h, hat_img_if_physical_cfg_t* cfg);

/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_cfg_ctx_t)(hat_img_if_client_t h, uint32 ctx, hat_img_if_context_cfg_t* cfg);

/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_start_ctx_t)(hat_img_if_client_t h, uint32 ctx, void* buff1, void* buff2);

/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_immediate_stop_ctx_t)(hat_img_if_client_t h, uint32 ctx, void** buff1, void** buff2);

/**
 * Type for platform specific image controller implementation
 */
typedef void* (*hat_img_if_set_buff_t)(hat_img_if_client_t h, uint32 ctx, void* new_buff);

/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_enable_evtent_t)(hat_img_if_client_t h, uint32 ctx, int32 notificationLine);

/**
 * Type for platform specific image controller implementation
 */
typedef int (*hat_img_if_disable_event_t)(hat_img_if_client_t h, uint32 ctx, int32 notificationLine);

/**
 * Generic opaque handle - client provides for each image interface instance
 *
 * @see hat_img_if_create
 */
typedef void* hat_img_if_client_t;

/**
 * Image interface object
 *
 * @param client_handle     client handle
 * @param interface_cfg     pointer to interface configuration API
 * @param ctx_cfg           pointer to context configuration API
 * @param ctx_start         pointer to context start API
 * @param ctx_imm_stop      pointer to context immediate stop API
 * @param ctx_set_buffer    pointer to context set buffer API
 * @param ctx_enable_event  pointer to context enable event API
 * @param ctx_disable_event pointer to context disable event API
 */
typedef struct
{
    hat_img_if_client_t             client_handle;

    hat_img_if_cfg_t                interface_cfg;
    hat_img_if_cfg_ctx_t            ctx_cfg;
    hat_img_if_start_ctx_t          ctx_start;
    hat_img_if_immediate_stop_ctx_t ctx_imm_stop;
    hat_img_if_set_buff_t           ctx_set_buffer;
    hat_img_if_enable_evtent_t      ctx_enable_event;
    hat_img_if_disable_event_t      ctx_disable_event;

}hat_img_if_object_t;

/**
 * Image interface handle
 *
 * @see hat_img_if_create
 */
typedef hat_img_if_object_t* hat_img_if_hndl_t;

/**
 * Create parameters for image interface instance
 *
 * @param img_if_identity  Camera instance + sensor (primary, secondary etc)
 * @param client_prv client private data returned when callback is invoked
 */
typedef struct {
    uint32   img_if_identity;
    void*    client_prv;
    hat_img_if_callback_t cb;
} hat_img_if_create_params_t;

/**
 * Instantiates image interface
 *
 * @param[out] h returns handle to
 * @param cfg    instance create parameters
 * @return 0 on success
 */
int hat_img_if_create(hat_img_if_hndl_t *h, hat_img_if_create_params_t* cfg);

/**
 * Destroy interface instance
 *
 * @param h  handle to driver instance
 * @return 0 on success
 */
int hat_img_if_destroy(hat_img_if_hndl_t h);


/**
 * Configures IMG_IF instance
 *
 * @param h   handle to interface
 * @param cfg instance configuration parameters
 * @return  0 on success
 */
static inline int hat_img_if_cfg(hat_img_if_hndl_t h, hat_img_if_physical_cfg_t* cfg)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->interface_cfg))
    {
        return 1;
    }
    ret_val = h->interface_cfg(h->client_handle, cfg);

    return ret_val;
}

/**
 * Configures context
 * @param h   handle to interface
 * @param ctx context index
 * @param cfg context configuration parameters
 * @return  0 on success
 */
static inline int hat_img_if_cfg_ctx(hat_img_if_hndl_t h, uint32 ctx, hat_img_if_context_cfg_t* cfg)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_cfg))
    {
        return 1;
    }
    ret_val = h->ctx_cfg(h->client_handle, ctx, cfg);

    return ret_val;
}

/**
 * Start context
 *
 * @param h     handle to interface
 * @param ctx   context index
 * @param buff1 context ping buffer
 * @param buff2 context pong buffer
 * @return  0 on success
 */
static inline int hat_img_if_start_ctx(hat_img_if_hndl_t h, uint32 ctx, void* buff1, void* buff2)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_start))
    {
        return 1;
    }
    ret_val = h->ctx_start(h->client_handle, ctx, buff1, buff2);

    return ret_val;
}

/**
 *
 * @param h    handle to interface
 * @param ctx  context index
 * @param[out] buff1 returns ping buffer from context
 * @param[out] buff2 returns pong buffer from context
 * @return  0 on success
 */
static inline int hat_img_if_immediate_stop_ctx(hat_img_if_hndl_t h, uint32 ctx, void** buff1, void** buff2)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_imm_stop))
    {
        return 1;
    }
    ret_val = h->ctx_imm_stop(h->client_handle, ctx, buff1, buff2);

    return ret_val;
}

/**
 * set a new buffer for IMG_IF context and returns a ready buffer
 * @param h   handle to interface
 * @param ctx context index
 * @param new_buff new buffer address
 * @return ready buffer address
 */
static inline void* hat_img_if_set_buff(hat_img_if_hndl_t h, uint32 ctx, void* new_buff)
{
    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_set_buffer))
    {
        return 1;
    }
    return (h->ctx_set_buffer(h->client_handle, ctx, new_buff ));
}

/**
 * Set (enable) notification (event) to be generated when reception reach particular line
 *
 * @param h   handle to interface
 * @param ctx context index
 * @param ev  event type
 * @param notificationLine specifies line number event should be generated - valid only for HAT_IMG_IF_EVENT_LN
 * @return 0 on success
 */
static inline int hat_img_if_enable_evtent(hat_img_if_hndl_t h, uint32 ctx, hat_img_if_evt_t ev, int32 notificationLine)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_enable_event))
    {
        return 1;
    }
    ret_val = h->ctx_enable_event(h->client_handle, ctx, ev, notificationLine);

    return ret_val;
}

/**
 * Disable notification event
 *
 * @param h   handle to interface
 * @param ctx context index
 * @param ev  event type
 * @param notificationLine line number - valid only for HAT_IMG_IF_EVENT_LN
 * @return 0 on success
 */
static inline int hat_img_if_disable_event(hat_img_if_hndl_t h, uint32 ctx, hat_img_if_evt_t ev, int32 notificationLine)
{
int ret_val;

    if ((NULL == h) ||
        (NULL == h->client_handle)||
        (NULL == h->ctx_disable_event))
    {
        return 1;
    }
    ret_val = h->ctx_disable_event(h->client_handle, ctx, ev, notificationLine);

    return ret_val;
}

#endif // __HAT_TYPES_IMG_IF_H__
