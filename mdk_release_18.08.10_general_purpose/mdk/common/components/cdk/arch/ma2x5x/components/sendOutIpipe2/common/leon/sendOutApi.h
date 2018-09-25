/**************************************************************************************************

 @File         : sendOutApi.h
 @Author       : MT
 @Brief        : Containing Ipipe2 Los output interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :
 **************************************************************************************************/
#ifndef SEND_OUT_API_H
#define SEND_OUT_API_H


/**************************************************************************************************
 ~~~  Include standard types And General Plug-in used module
 **************************************************************************************************/
#include "mv_types.h"
#include "IcTypes.h"

#include "sendout_config.h"


/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/

typedef struct
{
    HdmiCfg_t *hdmiCfg;
    MipiCfg_t *mipiCfg;
    UsbCfg_t  *usbCfg;
} SendOutInitCfg_t;

typedef void (*SendOutCbSent)(FrameT *frame, uint32_t outputId, uint32_t frmType);

typedef struct {
    uint32_t frame_type;                     /* content type */
    uint32_t frame_format;                   /* content format */
    uint32_t frame_width;                    /* number of pixels */
    uint32_t frame_height;                   /* number of lines */
    uint32_t frame_time_stamp_hi;            /* time stamp captured by MIPI RX */
    uint32_t frame_time_stamp_lo;
    uint32_t frame_proc_time_stamp_hi;       /* time stamp at processing start */
    uint32_t frame_proc_time_stamp_lo;
    uint32_t frame_idx_req_hal;              /* HAL frame request index */
    uint32_t frame_idx_req_app;              /* Application frame request index */
    uint32_t frame_idx_mipi_rx;              /* MIPI RX frame incremental counter */
    uint32_t frame_idx_process;              /* Still image processing counter */
    uint32_t header_height;                  /* number of line for the header */
    uint32_t slice_data_type;
    uint32_t slice_y_offset;                 /* number of bytes within payload */
    uint32_t slice_y_size;                   /* number of bytes */
    uint32_t slice_uv_offset;                /* number of bytes within payload */
    uint32_t slice_uv_size;                  /* number of bytes */
    uint32_t slice_total_number;             /* incremental counter in a frame */
    uint32_t slice_last_flag;                /* 0 - one more slice, 1 - last */
    uint32_t debug_data_enable;
    uint32_t camera_id;                      /* 0 - left chunk, 1 - right chunk */
    uint32_t buff_width;
    uint32_t buff_height;
    uint32_t buff_stride;
    uint32_t buff_pxl_size_nom;
    uint32_t buff_pxl_size_denom;
    uint32_t check_sum;                      /* magic number to check for valid header */
} client_tx_frame_header_t;

typedef struct {
    uint32_t chunk;
    char client_data[sizeof (client_tx_frame_header_t)];
    uint8_t metadata[0] __attribute((aligned (8)));
} send_out_tx_buffer_header_t;

typedef void (*InternalCbSent)(void *task);

typedef struct {
    FrameT              *buffer;
    uint32_t            outId;
    uint32_t            frmType;
    SendOutCbSent       sendOutCbSent;
    InternalCbSent      localCallback;
}SendOutElement_t;

/**************************************************************************************************
 ~~~  Global variables
**************************************************************************************************/

extern SendOutInitCfg_t sendOut_initCfg;

// for debug purpose in order to display not all outputs on lcd
extern uint32_t         dbgEnableOutput;

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
extern void sendOutCreate(SendOutInitCfg_t *cfg);
extern void sendOutInit(void);
extern void sendOutControl(uint32_t camera_en_bit_mask, uint32_t frame_type_en_bit_mask, uint32_t  frame_format_en_bit_mask);
extern void sendOutSend(FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent);
extern void sendOutFini();

#ifdef USE_STATIC_HEADER
extern send_out_tx_buffer_header_t* sendOutGetBufferHeader(void);
#endif


#endif //SEND_OUT_API_H
