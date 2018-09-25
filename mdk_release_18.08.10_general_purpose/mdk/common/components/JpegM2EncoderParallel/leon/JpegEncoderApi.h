///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup JpegEncoderApi JPEG Encoder API
/// @{
/// @brief     JPEG Encoder Functions API.
///
/// This is the API to a simple JPEG library implementing a jpeg encoding process.
///
/// @par Component Usage
///  In order to use the component the following steps are ought to be done:
///  1. User should declare variables for the 3 image planes:
///           - extern unsigned char img_Y;
///           - extern unsigned char img_U;
///           - extern unsigned char img_V;
///  2. User should configure the image, by using a variable of jpegFrameSpec type,
///           which is a structure whose fields are the 3 image planes, and information
///           about image width and height
///  4. User should start applying the encode algorithm on the image, by calling JPEG_encode function
///  5. User can verify the processing result, by making a memory dump from "jpeg_buff",
///           having the size equal to "outbytes"
///
///@}
/// @defgroup JpegEncoderParallel JPEG Encoder Parallel API
/// @ingroup JpegEncoderParallel
/// @{
/// @brief JPEG Encoder Parallel functions API
///

#ifndef _JPEG_ENCODER_API_H_
#define _JPEG_ENCODER_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <swcFrameTypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// Shave L2 Cache partition numbers
#define PARTITION_0     (0)
#define PARTITION_1     (1)
#define PARTITION_2     (2)
#define PARTITION_3     (3)
#define PARTITION_4     (4)
#define PARTITION_5     (5)
#define PARTITION_6     (6)
#define PARTITION_7     (7)
#define PARTITION_8     (8)
#define PARTITION_9     (9)
#define PARTITION_10    (10)
#define PARTITION_11    (11)


// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

enum
{
    JPEG_420_PLANAR,
    JPEG_422_PLANAR,
    JPEG_444_PLANAR
};

// 4: inline function implementations
// ----------------------------------------------------------------------------

/// @brief The JPEG encoding algorithm
/// @param[in] imgInfo - structure that contains info about input image
/// @param[out] output - pointer to the output buffer
/// @param[in] shaveNo - number of shaves to be used
/// @param[in] inBuffSizeShave - the size of the input buffer per shave
/// @param[in] jpegFormat - JPEG_420_PLANAR or JPEG_422_PLANAR or JPEG_444_PLANAR
/// @return u32 length of output buffer
///
u32 JPEG_encode(frameBuffer imgInfo, u8 *outputLocal, u32 shvNo, u32 inBuffSizeShave, int jpegFormat);

/// @}
#ifdef __cplusplus
}
#endif

#endif
