///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Stereo visualization
///

/// compiler includes
#include <iomanip>
#include <string>
///  Local Modules
#include "StereoDrawer.h"
#include "moviFloat32.h"
using namespace cv;
using namespace std;
static unsigned  char dummyBuff[1280*720];
static void drawDebugWindow( const string& meta_str, Mat& l_img, Mat& r_img, Mat& disp_img, Mat& depth_img) {

    int w = 2;
    int h = 2;
    int size_w = l_img.cols;
    int size_h = l_img.rows;
    int border_size = 10;
    static int frame_count = 0;
    Mat DispImage = Mat::zeros(Size((border_size*(w+1)) + size_w*w, (border_size*(h+1)) + size_h*h + 20), CV_8UC3);

    // Create ROIs for each image and copy to display image
    Rect ROI_l_img( border_size, border_size, size_w, size_h);
    Rect ROI_r_img( (2*border_size)+size_w, border_size, size_w, size_h);
    Rect ROI_d_img( border_size, (2*border_size)+size_h, size_w, size_h);
    Rect ROI_depth_img( (2*border_size)+size_w, (2*border_size)+size_h, size_w, size_h);
    l_img.copyTo(DispImage(ROI_l_img));
    r_img.copyTo(DispImage(ROI_r_img));
    disp_img.copyTo(DispImage(ROI_d_img));
    depth_img.copyTo(DispImage(ROI_depth_img));

    // print debug status information
    putText(DispImage, meta_str, Point(border_size, DispImage.rows-10 ), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));

    namedWindow( "Stereo", CV_WINDOW_NORMAL | CV_GUI_NORMAL);
    imshow( "Stereo", DispImage);
    cvWaitKey(1);
}

static string collateMetaData(VisionFrame* frame, const float& avg_fps){
    string sep = " | ";
    string fps = "FPS: " + to_string(avg_fps);
    string fc = "Frame Count: " + to_string(frame->frameCount);
    string ft = "Frame Type: " + to_string(frame->frameType);
    string ts = "Timestamp [Ns]: " + to_string(frame->timestampNs);
    string ex = "Exposure [Ns]: " + to_string(frame->exposureNs);
    string la = "Latency [Ns]: " + to_string(frame->latencyNs);
    string pw = "Pixel width [Bits]: " + to_string(frame->bpp);
    return fps + sep + fc + sep + ft + sep + ts + sep + ex + sep + la + sep + pw;
}

StereoDrawer::StereoDrawer(bool show_image) :
    m_prev_ts_ns(0),
    m_frame_count(0),
    m_show_image(show_image){
    memset(dummyBuff, 0, 1280*720);
}

float StereoDrawer::getAvgFPS(uint64_t ts){
    float avg_fps;
    float delta_s = (ts - m_prev_ts_ns)/1000000000.0;
    float current_fps = 1/delta_s;
    if(delta_s != 0 ) {
        //avg_fps = ((avg_fps * m_frame_count) + current_fps) / (++m_frame_count);
        avg_fps = current_fps;
    }

    m_prev_ts_ns = ts;
    return avg_fps;
}
void StereoDrawer::stretchDisparityRange(cv::Mat& frame, int disp)
{
    const int scaleFactorLowerD = 2;
    const int scaleFactorUpperD = 1;

    int halfIntervalD = disp/2;

    for (int row = 0; row < frame.rows; row++)
        for (int col = 0; col < frame.step; col++)
        {
            // if disparity value = [0, D/2), multiply it with 4
            if (frame.data[col + row * frame.step] < halfIntervalD)
                frame.data[col + row * frame.step] *= scaleFactorLowerD;
            else
                // if disparity value = [D/2, D), multiply it with 2
                //frame.data[col + row * frame.step] *= scaleFactorUpperD;
                frame.data[col + row * frame.step] =
            frame.data[col + row * frame.step] * scaleFactorUpperD +
            scaleFactorLowerD * halfIntervalD;
        }
}
Mat StereoDrawer::convhalfToMat(VisionFrame* frame, bool col_map, bool median){
    unsigned int exp;

    unsigned int intval;

    Mat im_gray = Mat(Size(frame->width, frame->height),  CV_8UC1);
    uint16_t *p16 = (uint16_t*)frame->p1;
    for(int x=0;x< frame->width*frame->height;x++)
    {
        intval = f16_to_f32_conv(*p16,&exp);
        im_gray.data[x] = (uint8_t)(*(reinterpret_cast<float*> (&intval)));
        //im_gray.data[x] = x%255;
        p16++;
    }

    Mat im_col(Size(frame->width, frame->height),  CV_8UC3);
    if (median){
        medianBlur(im_gray, im_gray, 5);
//#define DISPARITY 96
//        stretchDisparityRange(im_gray, DISPARITY);
    }
    if(col_map){
        applyColorMap(im_gray, im_col, COLORMAP_JET);
    }
    else{
        cvtColor(im_gray, im_col, COLOR_GRAY2RGB);
    }
    return im_col;

}

static void convertDisparityToDepth(Mat& disparity, Mat& depth, const float B, const float fov)
{
    const float PI = 3.14159265358979323846;
    const float fDegreesToRadians = PI/180.0;
    unsigned char* depthData = (unsigned char*)depth.data;
    for (int row = 0; row < disparity.rows; row++)
    {
        for (int col = 0; col < disparity.cols; col++)
        {
            if (disparity.data[col + row * disparity.cols] != 0)
            {
                float temp = (depth.cols * B) / (2 * tan((fov / 2) * fDegreesToRadians) * disparity.data[col + row * disparity.cols]);
                depth.data[col + row * depth.cols] = (uint8_t)(*(reinterpret_cast<float*> (&temp)));
            }
            else
            {
                depth.data[col + row * depth.cols] = 255;
            }
        }
    }
}

Mat StereoDrawer::convToMatDepth(VisionFrame* frame, float b, float fov)
{
    Mat im_gray = Mat(Size(frame->width, frame->height),  CV_8UC1);
    im_gray.data = (unsigned char*)frame->p1;

    Mat depth_img(Size(frame->width, frame->height),  CV_8UC1);
    convertDisparityToDepth(im_gray, depth_img, b, fov);

    Mat im_col(Size(frame->width, frame->height),  CV_8UC3);
    cvtColor(depth_img, im_col, COLOR_GRAY2RGB);

    return im_col;
}

Mat StereoDrawer::convToMat(VisionFrame* frame, bool col_map, bool median){

    Mat im_gray = Mat(Size(frame->width, frame->height),  CV_8UC1);
    im_gray.data = (unsigned char*)frame->p1;

    Mat im_col(Size(frame->width, frame->height),  CV_8UC3);
    if (median){
        medianBlur(im_gray, im_gray, 5);
#define DISPARITY 96
        stretchDisparityRange(im_gray, DISPARITY);
    }
    if(col_map){
        applyColorMap(im_gray, im_col, COLORMAP_JET);
    }
    else{
        cvtColor(im_gray, im_col, COLOR_GRAY2RGB);
    }
    return im_col;
}

int StereoDrawer::draw(VisionFrame* left,VisionFrame* right, VisionFrame* disp, VisionFrame* depth){

    Mat l_img = convToMat(left, false, false);
    Mat r_img = convToMat(right, false, false);

    Mat disp_img = convToMat(disp, true, true);
    Mat depth_img;

    if (depth->width == 0)
    {
        // M2: compute depth map on PC side
        depth_img = convToMatDepth(disp, baselineOv7251, fovOv7251);
    }
    else
    {
        // MX: depth map is streamed from myriad side flic plugin
        //depth_img = convhalfToMat(depth, true, false);
        depth_img = convToMatDepth(disp, baselineOv7251, fovOv7251);
    }

    string meta_str = collateMetaData(left, getAvgFPS(left->timestampNs));
    drawDebugWindow(meta_str, l_img, r_img, disp_img, depth_img);

    return 1;
}
