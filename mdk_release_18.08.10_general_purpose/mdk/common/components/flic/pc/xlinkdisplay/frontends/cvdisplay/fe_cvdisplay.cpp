/// =====================================================================================
///
///        @file:      fe_cvdisplay.cpp
///        @brief:     flicpc OpenCv Simple display front-end
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
/// Opencv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#include <stdio.h>
#include <stdlib.h>

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "fe_cvdisplay.h"

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------
void cv_display_frame(const char* windowname, uint32_t width, uint32_t height, void* data,
                        frameFmt_t fmt, float scaling, int framecount , const char* resultStr)
{
    float bpp;
    int type;
    int conversiontype;

    switch(fmt)
    {
        case FRAME_FORMAT_RAW8:
            bpp = 1;
            type = CV_8UC1;
            conversiontype = CV_GRAY2BGR;
            break;
        case FRAME_FORMAT_YUV420P:
            bpp = 1.5;
            type = CV_8UC1;
            conversiontype = CV_YUV2RGB_YV12;
            break;
        case FRAME_FORMAT_NV12:
            bpp = 1.5;
            type = CV_8UC1;
            conversiontype = CV_YUV2BGR_NV12;
            break;
        case FRAME_FORMAT_RGB_PLANAR:
            bpp = 3;
            type = CV_8UC3;
            conversiontype = 0;
            break;
        default:
            bpp = 1;
            type = CV_8UC1;
            conversiontype = CV_YUV2RGB_YV12;
            printf ( "Unsupported format %d\n", fmt );
            break;
    }

    cv::Mat incoming_img = cv::Mat(cv::Size(width, height*bpp), type, data, 0);
    cv::Mat outgoing_img = cv::Mat(height, width, CV_8UC3);


    if(conversiontype)
    {
        cv::cvtColor(incoming_img, outgoing_img, conversiontype, 3);
    }
    else
    {
        /// Manual rgb planar to interleaved
        int i;
        int j;
        int roffset = 0;
        int goffset = width * height;
        int boffset = 2 * width * height;
        for(i = 0, j = 0; i<(width*(height)); i++, j+=3)
        {
            outgoing_img.data[j + 2] = incoming_img.data[roffset + i];
            outgoing_img.data[j + 1] = incoming_img.data[goffset + i];
            outgoing_img.data[j + 0] = incoming_img.data[boffset + i];
        }
    }

    if((scaling != 1.0) || (scaling != 0))
    {
        cv::resize(outgoing_img, outgoing_img, cv::Size(), scaling, scaling);
    }

    /// Fcnt
    if(framecount >= 0)
    {
        std::string framecnt = std::to_string(framecount);
        if(resultStr != nullptr){
             framecnt.append(" : ");
             framecnt.append(resultStr);
             cv::putText(outgoing_img, framecnt, cv::Point(100,50), 0, 1, cv::Scalar(255,0,0), 2, 8, false );
        }
        else
             cv::putText(outgoing_img, framecnt, cv::Point(100,50), 0, 1, cv::Scalar(0,0,0), 1, 8, false );
    }

    cv::namedWindow(windowname, CV_WINDOW_AUTOSIZE);
    cv::imshow(windowname, outgoing_img);

    cv::waitKey(1);
}

void cv_display_mat(const char *windowname, void * ptr, int  framecount)
{
    cv::Mat *img = (cv::Mat *) ptr;

    /// Fcnt
    if(framecount >= 0)
    {
        std::string framecnt = std::to_string(framecount);
        cv::putText(*img, framecnt, cv::Point(100,50), 0, 1, cv::Scalar(0,0,0), 1, 8, false );
    }

    cv::namedWindow(windowname, CV_WINDOW_AUTOSIZE);
    cv::imshow(windowname, *img);

    cv::waitKey(1);
}
