///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Common defines for camera configuration
///
///
///
///
///

#ifndef CAM_CONFIG_H
#define CAM_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
#define MAX_USED_BUF             4
#define FIRST_INCOMING_BUF_ID    2
#define FIRST_OUTGOING_BUF_ID    0

#define CAM_WINDOW_START_COLUMN  0
#define CAM_WINDOW_START_ROW     12   //cut the embedded info frame

#define CAM_WINDOW_WIDTH    1920//the current width must be x16
#define CAM_WINDOW_HEIGHT   1080

// ----------------------------------------------------------------------------
#define CAM_BPP 2    //2 bytes per pixel (only 10bits used) for the only one plane of RAW10

#define CAM_FRAME_SIZE_BYTES    (CAM_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)

#define DDR_AREA      __attribute__((section(".ddr.bss")))

// actual payload header size is 12. The values 16 is needed to keep the 8 bytes
// alignment of the frame buffer required by SIPP
#define PAYLOAD_HEADER_OFFSET 16

#define MSG_QUEUE_SIZE   1
#define MSG_SIZE         1
#define USB_HIGH_SPEED   2
#define USB_SUPER_SPEED  3

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


#endif // CAM_CONFIG_H
