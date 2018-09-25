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

// 2:  Source Specific defines and types
// ----------------------------------------------------------------------------
#define CAM_BPP					2	// imx208 sensors capture RAW10 grayscale
#define ALGO_BPP				1	// algorithm processes YUV400
#define USB_BPP					2	// USB streams YUV422

#define MAX_USED_BUF			3	// number of circular buffers
#define FIRST_INCOMING_BUF_ID	2	// first buffer index from the camera
#define FIRST_OUTGOING_BUF_ID	0	// first buffer index to the USB

#define CAM_WINDOW_WIDTH		640
#define CAM_WINDOW_HEIGHT		480

#define CAM_WINDOW_START_COLUMN		164		// crop the center of the camera frame
#define CAM_WINDOW_START_ROW		34		// crop the center of the camera frame

#define CAM_FRAME_SIZE_BYTES	(CAM_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)
#define ALGO_FRAME_SIZE_BYTES	(CAM_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * ALGO_BPP)
#define USB_FRAME_SIZE_BYTES    (CAM_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * USB_BPP)

#define PAYLOAD_HEADER_OFFSET	16	// actual payload header size is 12; use 16 to keep 8 bytes alignment for SIPP


#endif // CAM_CONFIG_H
