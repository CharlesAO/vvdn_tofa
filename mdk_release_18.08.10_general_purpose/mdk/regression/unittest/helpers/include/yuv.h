#ifndef __YUV_H__
#define __YUV_H__

#include <cv.h>

IplImage* loadImageYUV(const char* name_file, int w, int h);
void saveImageYUV(const char* name_file, IplImage* img);
void showImageYUV(const char* window, IplImage* img, int waitTime);

#endif