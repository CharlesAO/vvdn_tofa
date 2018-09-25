///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup bicubicWarpApiDefines Bicubic Warp API Defines
/// @ingroup  bicubicWarp
/// @{
/// @brief Definitions and types needed by the bicubic warp component
///
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for the bicubic warp component
///

#ifndef __BICUBIC_WARP_API_DEFINES_H__
#define __BICUBIC_WARP_API_DEFINES_H__

// Lowest interrupt priority is assigned by default. 
// Define it before including this header in order to 
// override it in an application.
#ifndef IRQ_PRIORITY_BICUBIC
#define IRQ_PRIORITY_BICUBIC 14
#endif

// CTRL register bitfield defines
#ifndef BICUBIC_RUN_SHIFT
#define BICUBIC_RUN_SHIFT            0
#endif

#ifndef BICUBIC_RUN_MASK
#define BICUBIC_RUN_MASK             0x01
#endif

#ifndef BICUBIC_BYPASS_SHIFT
#define BICUBIC_BYPASS_SHIFT         1
#endif

#ifndef BICUBIC_BYPASS_MASK
#define BICUBIC_BYPASS_MASK          0x02
#endif

#ifndef BICUBIC_IRQ_SHIFT
#define BICUBIC_IRQ_SHIFT            2
#endif

#ifndef BICUBIC_IRQ_MASK
#define BICUBIC_IRQ_MASK             0x04
#endif

#ifndef BICUBIC_SMOKETEST_SHIFT
#define BICUBIC_SMOKETEST_SHIFT      3
#endif

#ifndef BICUBIC_SMOKETEST_MASK
#define BICUBIC_SMOKETEST_MASK       0x08
#endif

#ifndef BICUBIC_RUNSTATUS_SHIFT
#define BICUBIC_RUNSTATUS_SHIFT      4
#endif

#ifndef BICUBIC_RUNSTATUS_MASK
#define BICUBIC_RUNSTATUS_MASK       0x10
#endif

#ifndef BICUBIC_CACHE_REQ_SHIFT
#define BICUBIC_CACHE_REQ_SHIFT      5
#endif

#ifndef BICUBIC_CACHE_REQ_MASK
#define BICUBIC_CACHE_REQ_MASK       0x20
#endif

#ifndef BICUBIC_CACHE_WEN_SHIFT
#define BICUBIC_CACHE_WEN_SHIFT      6
#endif

#ifndef BICUBIC_CACHE_WEN_MASK
#define BICUBIC_CACHE_WEN_MASK       0x40
#endif

#ifndef BICUBIC_BILINEAR_SHIFT
#define BICUBIC_BILINEAR_SHIFT       7
#endif

#ifndef BICUBIC_BILINEAR_MASK
#define BICUBIC_BILINEAR_MASK        0x80
#endif

#ifndef BICUBIC_PIX_FORMAT_SHIFT
#define BICUBIC_PIX_FORMAT_SHIFT     8
#endif

#ifndef BICUBIC_PIX_FORMAT_MASK
#define BICUBIC_PIX_FORMAT_MASK      0x300
#endif

#ifndef BICUBIC_CLAMP_SHIFT
#define BICUBIC_CLAMP_SHIFT          10
#endif

#ifndef BICUBIC_CLAMP_MASK
#define BICUBIC_CLAMP_MASK           0x400
#endif

#ifndef BICUBIC_BORDER_MODE_SHIFT
#define BICUBIC_BORDER_MODE_SHIFT    11
#endif

#ifndef BICUBIC_BORDER_MODE_MASK
#define BICUBIC_BORDER_MODE_MASK     0x800
#endif

#ifndef BICUBIC_BAYER_ENABLE_SHIFT
#define BICUBIC_BAYER_ENABLE_SHIFT   12
#endif

#ifndef BICUBIC_BAYER_ENABLE_MASK
#define BICUBIC_BAYER_ENABLE_MASK    0x1000
#endif

#ifndef BICUBIC_BAYER_Y_ODD_SHIFT
#define BICUBIC_BAYER_Y_ODD_SHIFT    13
#endif

#ifndef BICUBIC_BAYER_Y_ODD_MASK
#define BICUBIC_BAYER_Y_ODD_MASK     0x2000
#endif

#ifndef BICUBIC_BAYER_X_MODE_SHIFT
#define BICUBIC_BAYER_X_MODE_SHIFT   14
#endif

#ifndef BICUBIC_BAYER_X_MODE_MASK
#define BICUBIC_BAYER_X_MODE_MASK    0xC000
#endif

#ifndef BICUBIC_PIX_FORMAT_RGBX8888
#define BICUBIC_PIX_FORMAT_RGBX8888  0
#endif

#ifndef BICUBIC_PIX_FORMAT_U8F
#define BICUBIC_PIX_FORMAT_U8F       1
#endif

#ifndef BICUBIC_PIX_FORMAT_FP16
#define BICUBIC_PIX_FORMAT_FP16      2
#endif

#ifndef BICUBIC_PIX_FORMAT_U16F
#define BICUBIC_PIX_FORMAT_U16F      3
#endif

#ifndef BICUBIC_BAYER_MODE_NORMAL
#define BICUBIC_BAYER_MODE_NORMAL    0
#endif

#ifndef BICUBIC_BAYER_MODE_INVERT
#define BICUBIC_BAYER_MODE_INVERT    1
#endif

#ifndef BICUBIC_BAYER_MODE_ODD
#define BICUBIC_BAYER_MODE_ODD       2
#endif

#define CLAMP_MIN 0x0000 // 0.0 f16
#define CLAMP_MAX 0x3c00 // 1.0 f16

typedef struct bicubicWarpContext {
    fp32* xyRectifiedBuffer; // rectified x and y pixels
    fp32* mapx; // LUT for rectifying x coordinates
    fp32* mapy; // LUT for rectifying y coordinates
    fp32* homography; // homography transformation for right image
    u8* inputImageBuffer; // input image to be rectified
    u8* outputImageBuffer; // output rectified image
    u32 width; // input image width
    u32 height; // input image height
    u32 bpp; // bytes per pixel
    u32 pixelFormat; // pixel format
    irq_handler hndl; // interrupt handler
    u32 irqNumber; // interrupt number
}bicubicWarpContext;

#endif // __BICUBIC_WARP_API_DEFINES_H__
