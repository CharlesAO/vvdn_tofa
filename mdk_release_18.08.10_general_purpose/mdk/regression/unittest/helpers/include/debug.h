#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <cv.h>

void displayMatAttr(cv::Mat* image);
void saveFile(std::string fileName, char* buffer, unsigned int bytes);

#endif

