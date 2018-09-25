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
* @file hat_csi.h
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

#ifndef __HAT_TYPES_CSI_H__
#define __HAT_TYPES_CSI_H__

#include <hal/hat_types.h>

/** max lanes per CSI - 4data + 1clock lanes */
#define HAT_MAX_CSI_LANES 	5
typedef void* hat_csi_hndl;

/* ======================================================================= */
/**
 * CSI input format - derived from MIPI standard
*/
/* ======================================================================= */
typedef enum {

    HAT_MIPI_OTHERS= 0x000,

    HAT_MIPI_EMBEDDED_8BIT_NON_IMAGE_DATA = 0x012,

    HAT_MIPI_YUV420_8bit = 0x018,
    HAT_MIPI_YUV420_10bit = 0x019,
    HAT_MIPI_YUV420_8BIT_legacy = 0x01A,
    HAT_MIPI_YUV420_8BIT_CSPS = 0x01C,
    HAT_MIPI_YUV420_10bit_CSPS = 0x01D,
    HAT_MIPI_YUV422_8bit = 0x01E,
    HAT_MIPI_YUV422_10bit = 0x01F,

    HAT_MIPI_RGB565 = 0x022,
    HAT_MIPI_RGB888 = 0x024,

    HAT_MIPI_RAW6 = 0x028,
    HAT_MIPI_RAW7 = 0x029,
    HAT_MIPI_RAW8 = 0x02A,
    HAT_MIPI_RAW10 = 0x02B,
    HAT_MIPI_RAW12 = 0x02C,
    HAT_MIPI_RAW14 = 0x02D,
    HAT_MIPI_RGB666_EXP32_24= 0x033,

    HAT_MIPI_USER_8_BIT_TYPE_1 = 0x040,
    HAT_MIPI_USER_8_BIT_TYPE_2 = 0x041,
    HAT_MIPI_USER_8_BIT_TYPE_3 = 0x042,
    HAT_MIPI_USER_8_BIT_TYPE_4 = 0x043,
    HAT_MIPI_USER_8_BIT_TYPE_5 = 0x044,
    HAT_MIPI_USER_8_BIT_TYPE_6 = 0x045,
    HAT_MIPI_USER_8_BIT_TYPE_7 = 0x046,
    HAT_MIPI_USER_8_BIT_TYPE_8 = 0x047,

    HAT_MIPI_RAW6_EXP8 = 0x068,
    HAT_MIPI_RAW7_EXP8 = 0x069,

    HAT_MIPI_USER_8_BIT_TYPE_1_EXP8 = 0x080,
    HAT_MIPI_USER_8_BIT_TYPE_2_EXP8 = 0x081,
    HAT_MIPI_USER_8_BIT_TYPE_3_EXP8 = 0x082,
    HAT_MIPI_USER_8_BIT_TYPE_4_EXP8 = 0x083,
    HAT_MIPI_USER_8_BIT_TYPE_5_EXP8 = 0x084,
    HAT_MIPI_USER_8_BIT_TYPE_6_EXP8 = 0x085,
    HAT_MIPI_USER_8_BIT_TYPE_7_EXP8 = 0x086,
    HAT_MIPI_USER_8_BIT_TYPE_8_EXP8 = 0x087,

    HAT_MIPI_YUV422_8BIT_VP = 0x09E,

    HAT_MIPI_RGB444_EXP16 = 0x0A0,
    HAT_MIPI_RGB555_EXP16 = 0x0A1,

    HAT_MIPI_RAW10_EXP16 = 0x0AB,
    HAT_MIPI_RAW12_EXP16 = 0x0AC,
    HAT_MIPI_RAW14_EXP16 = 0x0AD,
    HAT_MIPI_YUV422_8BIT_VP_16bit = 0x0DE,

    HAT_MIPI_RGB666_EXP32 = 0x0E3,
    HAT_MIPI_RGB888_EXP32 = 0x0E4,

    HAT_MIPI_RAW6_DPCM10_VP = 0x0E8,
    HAT_MIPI_RAW8_VP = 0x12A,
    HAT_MIPI_RAW12_VP = 0x12C,
    HAT_MIPI_RAW14_VP = 0x12D,
    HAT_MIPI_RAW10_VP = 0x12F,

    HAT_MIPI_USER_8_BIT_TYPE_1_DPCM12_VP = 0x140,
    HAT_MIPI_USER_8_BIT_TYPE_2_DPCM12_VP = 0x141,
    HAT_MIPI_USER_8_BIT_TYPE_3_DPCM12_VP = 0x142,
    HAT_MIPI_USER_8_BIT_TYPE_4_DPCM12_VP = 0x143,
    HAT_MIPI_USER_8_BIT_TYPE_5_DPCM12_VP = 0x144,
    HAT_MIPI_USER_8_BIT_TYPE_6_DPCM12_VP = 0x145,
    HAT_MIPI_USER_8_BIT_TYPE_7_DPCM12_VP = 0x146,
    HAT_MIPI_USER_8_BIT_TYPE_8_DPCM12_VP = 0x147,
    HAT_MIPI_USER_8_BIT_TYPE_1_DPCM12_EXP16 = 0x1C0,
    HAT_MIPI_USER_8_BIT_TYPE_2_DPCM12_EXP16 = 0x1C1,
    HAT_MIPI_USER_8_BIT_TYPE_3_DPCM12_EXP16 = 0x1C2,
    HAT_MIPI_USER_8_BIT_TYPE_4_DPCM12_EXP16 = 0x1C3,
    HAT_MIPI_USER_8_BIT_TYPE_5_DPCM12_EXP16 = 0x1C4,
    HAT_MIPI_USER_8_BIT_TYPE_6_DPCM12_EXP16 = 0x1C5,
    HAT_MIPI_USER_8_BIT_TYPE_7_DPCM12_EXP16 = 0x1C6,
    HAT_MIPI_USER_8_BIT_TYPE_8_DPCM12_EXP16 = 0x1C7,

    HAT_MIPI_RAW7_DPCM10_EXP16 = 0x229,
    HAT_MIPI_RAW6_DPCM10_EXP16 = 0x2A8,
    HAT_MIPI_RAW8_DPCM10_EXP16 = 0x2AA,

    HAT_MIPI_USER_8_BIT_TYPE_1_DPCM10_EXP16 = 0x2C0,
    HAT_MIPI_USER_8_BIT_TYPE_2_DPCM10_EXP16 = 0x2C1,
    HAT_MIPI_USER_8_BIT_TYPE_3_DPCM10_EXP16 = 0x2C2,
    HAT_MIPI_USER_8_BIT_TYPE_4_DPCM10_EXP16 = 0x2C3,
    HAT_MIPI_USER_8_BIT_TYPE_5_DPCM10_EXP16 = 0x2C4,
    HAT_MIPI_USER_8_BIT_TYPE_6_DPCM10_EXP16 = 0x2C5,
    HAT_MIPI_USER_8_BIT_TYPE_7_DPCM10_EXP16 = 0x2C6,
    HAT_MIPI_USER_8_BIT_TYPE_8_DPCM10_EXP16 = 0x2C7,

    HAT_MIPI_RAW7_DPCM10_VP = 0x329,
    HAT_MIPI_RAW8_DPCM10_VP = 0x32A,

    HAT_MIPI_USER_8_BIT_TYPE_1_DPCM10_VP = 0x340,
    HAT_MIPI_USER_8_BIT_TYPE_2_DPCM10_VP = 0x341,
    HAT_MIPI_USER_8_BIT_TYPE_3_DPCM10_VP = 0x342,
    HAT_MIPI_USER_8_BIT_TYPE_4_DPCM10_VP = 0x343,
    HAT_MIPI_USER_8_BIT_TYPE_5_DPCM10_VP = 0x344,
    HAT_MIPI_USER_8_BIT_TYPE_6_DPCM10_VP = 0x345,
    HAT_MIPI_USER_8_BIT_TYPE_7_DPCM10_VP = 0x346,
    HAT_MIPI_USER_8_BIT_TYPE_8_DPCM10_VP = 0x347,

    HAT_MIPI_RAW6_DPCM12_VP = 0x368,
    HAT_MIPI_RAW7_DPCM12_EXP16 = 0x369,
    HAT_MIPI_RAW8_DPCM12_EXP16 = 0x36A,
    HAT_MIPI_RAW6_DPCM12_EXP16 = 0x3A8,
    HAT_MIPI_RAW7_DPCM12_VP = 0x3A9,
    HAT_MIPI_RAW8_DPCM12_VP = 0x3AA

}hat_mipi_format_t;


/* ======================================================================= */
/**
 * enumeration for CSI power state selection
 *
 * @param HAT_CSI2_OFF  power-off state
 * @param HAT_CSI2_ON   power-on state
 * @param HAT_CSI2_ULP  ultra-low-power state
 */
/* ======================================================================= */
typedef enum {
    HAT_CSI2_OFF,
    HAT_CSI2_ON,
    HAT_CSI2_ULP
}hat_csi_pwr_t;


/* ======================================================================= */
/**
 * enumeration for selection of polarity order of the channel
 *
 * @param HAT_CSI_PLUS_MINUS  +/- polarity
 * @param HAT_CSI_MINUS_PLUS  -/+ polarity
 */
/* ======================================================================= */
typedef enum {
    HAT_CSI_PLUS_MINUS,
    HAT_CSI_MINUS_PLUS
}hat_csi_lane_polarity_t;


/* ======================================================================= */
/**
 * per lane configuration
 *
 * @param position  lane position
 * @param polarity  lane polarity
 */
/* ======================================================================= */
typedef  struct  {
	uint8 					position;
	hat_csi_lane_polarity_t	polarity;
}hat_csi_lane_cfg_t;

/* ======================================================================= */
/**
 * CSI physical configuration
 *
 * @param num_lanes   number of lanes used for particular sensor mode
 * @param mipi_clock  MIPI clock for one lane [Hz] - note bit rate is 2 times higher than MIPI clock
 * @param continuous_clock does clock is present during vertical/horizontal blanking
 * @param lanes_cfg[HAT_MAX_CSI_LANES]  per lane configuration
 * @param clock_lane_idx - index in lanes_cfg that represents clock lane
 * @param csi_pwr	  CSI power configuration
 *
 * @see hat_csi_lane_cfg_t
 * @see hat_csi_pwr_t
 * @see HAT_MAX_CSI_LANES
 */
/* ======================================================================= */

typedef struct {
    uint32              num_lanes;
    uint32              mipi_clock;
    uint32              continuous_clock;
    hat_csi_lane_cfg_t  lanes_cfg[HAT_MAX_CSI_LANES];
    uint32              clock_lane_idx;
    hat_csi_pwr_t       csi_pwr;

} hat_csi_physical_cfg_t;

/**
 * Per-context configuration parameters - information needed to configure the
 * CSI context.
 *
 * @param camera_output_size; // CSI input size
 * @param crop_window; // CSI output size
 * @param in_format;   // input MIPI format
 * @param out_format; // if direct memory write is supported by CSI
 * @param csi_vchannel_id // CSI virtual channel ID  along with input data tyep identifys the MIPI strram

 * @see hat_size_t
 * @see hat_rect_t
 * @see hat_mipi_format_t
 * @see hat_pix_fmt_t
 */
typedef struct {
        hat_size_t          camera_output_size; // CSI input size
        hat_rect_t          crop_window;        // CSI output size
        hat_mipi_format_t   in_format;          // input MIPI format
        hat_pix_fmt_t       out_format;         // if direct memory write is supported by CSI
        uint16              csi_vchannel_id;    // CSI virtual channel ID
} hat_csi_context_cfg;

/* ======================================================================= */
/**
 * enumeration for selection of per-context event
 *
 * @param HAT_CSI_EVENT_FS      start of frame event
 * @param HAT_CSI_EVENT_FE      end of frame event
 * @param HAT_CSI_EVENT_LN      reception reached to line number
 * @param HAT_CSI_EVENT_ERROR   event indicating an error from specific context or CSI controller
 *
 */
/* ======================================================================= */
typedef enum {
    HAT_CSI_EVENT_FS,  //!< HAT_CSI_EVENT_FS
    HAT_CSI_EVENT_FE,  //!< HAT_CSI_EVENT_FE
    HAT_CSI_EVENT_LN,  //!< HAT_CSI_EVENT_LN
    HAT_CSI_EVENT_ERROR//!< HAT_CSI_EVENT_ERROR
}hat_csi_evt_t;

// All CSI functions are blocking
/**
 * Callback invoked at some
 *
 * @param h     handle to CSI controller instance
 * @param ctx   refers to context inside CSI controller
 * @param client_prv client private data
 * @param evt   event type @see hat_csi_evt_t
 * @param ev_data data specific to event
 * @param data_ptr if the event requires more data it will point to some more complex structure
 *                 (it be available only during this call).
 */
typedef void (*hat_csi_callback) (hat_csi_hndl h, uint32 ctx, void* client_prv, hat_csi_evt_t evt, uint32 ev_data, void* data_ptr);


/**
 * Create parameters for CSI instance
 *
 * @param csi_idx  CSI controller index - typically we have 2 CSI(sockets) for primary and secondary sensors
 * @param client_prv client private data returned when callback is invoked
 */
typedef struct {
    uint32           csi_idx;
    void*           client_prv;
    hat_csi_callback cb;
} hat_csi_create_params;

/**
 * Instantiates driver for CSI controller
 *
 * @param[out] h returns handle to
 * @param cfg    instance create parameters
 * @return 0 on success
 */

int hat_csi_create(hat_csi_hndl *h, hat_csi_create_params cfg);
/**
 * Destroy driver instance
 *
 * @param h  handle to driver instance
 * @return 0 on success
 */
int hat_csi_destroy(hat_csi_hndl h);

/**
 * Configures CSI instance
 *
 * @param h handle to driver instance
 * @param cfg instance configuration parameters
 * @return  0 on success
 */
int hat_csi_cfg(hat_csi_hndl h, hat_csi_physical_cfg_t cfg);

/**
 * Configures context
 * @param h handle to driver instance
 * @param ctx context index
 * @param cfg context configuration parameters
 * @return  0 on success
 */
int hat_csi_cfg_csi_ctx(hat_csi_hndl h, uint32 ctx, hat_csi_context_cfg cfg);

/**
 * Start context
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @param buff1 context ping buffer
 * @param buff2 context pong buffer
 * @return  0 on success
 */
int hat_csi_start_csi_ctx(hat_csi_hndl h, uint32 ctx, void* buff1, void* buff2);

/**
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @param[out] buff1 returns ping buffer from context
 * @param[out] buff2 returns pong buffer from context
 * @return  0 on success
 */
int hat_csi_immediate_stop_csi_ctx(hat_csi_hndl h, uint32 ctx, void** buff1, void** buff2);

/**
 * set a new buffer for CSI context and returns a ready buffer
 * @param h handle to driver instance
 * @param ctx context index
 * @param new_buff new buffer address
 * @return ready buffer address
 */
void* hat_set_csi_buff(hat_csi_hndl h, uint32 ctx, void* new_buff);

/**
 * Set (enable) notification (event) to be generated when reception reach particular line
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @param notificationLine  specifies line number
 * @return 0 on success
 */
int hat_csi_enb_line_evt(hat_csi_hndl h, uint32 ctx, int32 notificationLine);

/**
 * Disable notification (event) at line
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @param notificationLine  line number
 * @return 0 on success
 */
int hat_csi_dis_line_evt(hat_csi_hndl h, uint32 ctx, int32 notificationLine);

/**
 * Set (enable) notification (event) to be generated at start of frame
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @return 0 on success
 */
int hat_csi_enb_FS(hat_csi_hndl h, uint32 ctx);

/**
 * Disable notification (event) to be generated at start of frame
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @return 0 on success
 */
int hat_csi_dis_FS(hat_csi_hndl h, uint32 ctx);

/**
 * Set (enable) notification (event) to be generated at end of frame
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @return 0 on success
 */
int hat_csi_enb_FE(hat_csi_hndl h, uint32 ctx);

/**
 * Disable notification (event) to be generated at end of frame
 *
 * @param h handle to driver instance
 * @param ctx context index
 * @return 0 on success
 */
int hat_csi_dis_FE(hat_csi_hndl h, uint32 ctx);


#endif // __HAT_TYPES_CSI_H__
