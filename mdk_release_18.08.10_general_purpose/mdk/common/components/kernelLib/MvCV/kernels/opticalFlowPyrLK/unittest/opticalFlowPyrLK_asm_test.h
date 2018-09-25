#ifndef _OPTICALFLOWPYRLK_ASM_TEST_H_
#define _OPTICALFLOWPYRLK_ASM_TEST_H_

#include <mv_types.h>
#include <cv.h>

extern unsigned int opticalFlowCycleCount;

void OpticalFlowPyrLK_asm_test(std::vector<cv::Mat>pyrA, std::vector<cv::Mat>pyrB,
                               std::vector<cv::Point2f> prevPts, std::vector<cv::Point2f>& nextPtsShave,
                               std::vector<u8>& statusShave, std::vector<fp32>& errorShave, cv::Size winSize,
                               s32 levels, cv::TermCriteria termCriteria, s32 flags);

#endif
