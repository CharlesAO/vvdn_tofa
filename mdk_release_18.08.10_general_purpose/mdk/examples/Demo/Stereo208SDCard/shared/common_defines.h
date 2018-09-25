/// =====================================================================================
///
///        @file:      common_defines.h
///        @brief:     Common defines shared between LOS and LRT
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

#ifndef COMMON_DEFINES_H
#define COMMON_DEFINES_H

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

#define SYS_CLK_KHZ             (12000) // 12MHz
#define PLL_DESIRED_FREQ_KHZ    (480000)

#define MAX_USED_BUF             3
#define FIRST_INCOMING_BUF_ID    2
#define FIRST_OUTGOING_BUF_ID    0

#define CAM_WINDOW_START_COLUMN  0
#define CAM_WINDOW_START_ROW     8   //cut the embedded info frame

#define CAM_LEFT_WINDOW_WIDTH    960 //the current width must be x16
#define CAM_RIGHT_WINDOW_WIDTH   960 //the current width must be x16
#define CAM_WINDOW_HEIGHT        1080

// ----------------------------------------------------------------------------
#define LCD_LEFT_WINDOW_WIDTH    CAM_LEFT_WINDOW_WIDTH
#define LCD_RIGHT_WINDOW_WIDTH   CAM_RIGHT_WINDOW_WIDTH
#define LCD_WINDOW_HEIGHT        CAM_WINDOW_HEIGHT

#define LCD_WINDOW_LEFT_STARTX   0
#define LCD_WINDOW_LEFT_STARTY   0
#define LCD_WINDOW_RIGHT_STARTX  LCD_LEFT_WINDOW_WIDTH
#define LCD_WINDOW_RIGHT_STARTY  0

#define CAM_BPP 2    //2 bytes per pixel (only 10bits used) for the only one plane of RAW10
#define LCD_BPP 2    //2bpp for the only one plane of YUV422i

#define CAM_LEFT_FRAME_SIZE_BYTES    (CAM_LEFT_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)
#define CAM_RIGHT_FRAME_SIZE_BYTES   (CAM_RIGHT_WINDOW_WIDTH * CAM_WINDOW_HEIGHT * CAM_BPP)

#define LCD_LEFT_FRAME_SIZE_BYTES    (LCD_LEFT_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * LCD_BPP)
#define LCD_RIGHT_FRAME_SIZE_BYTES   (LCD_RIGHT_WINDOW_WIDTH * LCD_WINDOW_HEIGHT * LCD_BPP)

#endif
