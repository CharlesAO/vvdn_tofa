#ifndef __FECVDISPLAY_H__
#define __FECVDISPLAY_H__
/// =====================================================================================
///
///        @file:      fe_cvdisplay.h
///        @brief:     
///        @created:   11/13/2017 04:22:49 PM
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
typedef enum
{
    FRAME_FORMAT_YUV420P,
    FRAME_FORMAT_RAW8,
    FRAME_FORMAT_BGR,
    FRAME_FORMAT_NV12,
    FRAME_FORMAT_RGB_PLANAR
}frameFmt_t;

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

void cv_display_frame(const char* windowname, uint32_t width, uint32_t height, void* data,
                        frameFmt_t fmt, float scaling, int framecount, const char* resultStr);

void cv_display_mat(const char *windowname, void * ptr, int framecount);

#endif // __FECVDISPLAY_H__
