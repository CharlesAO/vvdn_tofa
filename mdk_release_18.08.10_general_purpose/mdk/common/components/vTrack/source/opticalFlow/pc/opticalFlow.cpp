///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     opticalFlow for vtrack project
///

#include "mv_types.h"
// MvCV
#include <mvcv.h>  //movi Mat Type
#include <opticalFlowPyrLK.h> //movi implementation of LK Optical flow
// OpenCV
#include <opencv2/opencv.hpp>
// Local Headers
#include "vTrack.h"
#include "global_constants.h"
#include "opticalFlowPrivate.h" //API
#include "swcFrameTypes.h" //API
#include <mvMacros.h>

// Special function for clamping points inside a specified bound
#define CLAMP(x,bound) {x = x<0 ? 0 : x>=bound ? bound-1 : x;}


//sub-pixel refinement-----------------
#define FIND_SUBPIX_CORNER_EPSILON 0.03
#define FIND_SUBPIX_CORNER_ITERATIONS 20
#define FIND_SUBPIX_CORNER_WINDOW_SIZE 10


//config params
static u32 g_num_pyr_levels;
static tvSize g_winSize;
static tvTermCriteria g_termCriteria;
//config params extended
static u8 cfgDoOfMovi;
static u8 cfgDoSubpixelDetection;

//pyramid buffers
static Mat movi_PyrA[MAX_PYRAMID_LEVELS];
static Mat movi_PyrB[MAX_PYRAMID_LEVELS];

void opticalFlowSetup(  tvPyramidBuffer* pyramid,
        tvOpticalFlowCfg* algConfig,
        Mat g_pyrA[],
        Mat g_pyrB[]) {
    (void)g_pyrA;
    (void)g_pyrB;

    g_num_pyr_levels = (algConfig->pyrLevels < MAX_PYRAMID_LEVELS) ? algConfig->pyrLevels :  MAX_PYRAMID_LEVELS;
    g_winSize = algConfig->winSize;
    g_termCriteria = algConfig->termCriteria;

    if(algConfig->algoType == OF_OPENCV)
        cfgDoOfMovi = 0;
    else if(algConfig->algoType == OF_MOVI)
        cfgDoOfMovi = 1;
    cfgDoSubpixelDetection = algConfig->subpixelDetection;
    for (u32 i = 0; i < g_num_pyr_levels + 1; i++)
    {
        mvcvSetData1(&movi_PyrA[i], pyramid->pyrLevel[i].spec.height,
                     pyramid->pyrLevel[i].spec.width,
                     0, CV_8U);
        movi_PyrA[i].step0[0] = pyramid->pyrLevel[i].spec.stride;
        movi_PyrA[i].step0[1] = pyramid->pyrLevel[i].spec.bytesPP;
        movi_PyrA[i].step = movi_PyrA[i].step0[0];

        mvcvSetData1(&movi_PyrB[i], pyramid->pyrLevel[i].spec.height,
                     pyramid->pyrLevel[i].spec.width,
                     0, CV_8U);
        movi_PyrB[i].step0[0] = pyramid->pyrLevel[i].spec.stride;
        movi_PyrB[i].step0[1] = pyramid->pyrLevel[i].spec.bytesPP;
        movi_PyrB[i].step = movi_PyrB[i].step0[0];
    }
}

void opticalFlowRun(tvOpticalFlowCfg* algConfig,
        tvPyramidBuffer* pyrImgPrev,
        tvPyramidBuffer* pyrImgCur,
        tvXYLoc featuresPrev[],
        tvXYLoc featuresCur[],
        fp32 featuresError[],
        u32 featuresCount
        ) {
    opticalFlowSetup(pyrImgPrev, algConfig, 0, 0);
    IplImage* tempGrayImg[MAX_PYRAMID_LEVELS];
    IplImage* tempPreviousGrayImg[MAX_PYRAMID_LEVELS];
    if(0 == cfgDoOfMovi || cfgDoSubpixelDetection) {

        // C++ function calcOpticalFlowPyrLK(Mat) has bad detection rate and does not allow to compute yourself the image pyramid
        // ==> use C function cvCalcOpticalFlowPyrLK(IplImage*)
        char *src, *dst;
        for(int i = 0; i < g_num_pyr_levels + 1; i++) {

            tempGrayImg[i] = cvCreateImage(cvSize(pyrImgCur->pyrLevel[i].spec.width, pyrImgCur->pyrLevel[i].spec.height), IPL_DEPTH_8U, 1);
            src = (char *) pyrImgCur->pyrLevel[i].p1 + PADDING_H;
            dst = tempGrayImg[i]->imageData;
            for (int j = 0; j < pyrImgCur->pyrLevel[i].spec.height; i++)
                for (int k = 0; k < pyrImgCur->pyrLevel[i].spec.width; k++)
                    dst[j * tempGrayImg[i]->widthStep + k] = src[j * pyrImgCur->pyrLevel[i].spec.stride + k];


            tempPreviousGrayImg[i] = cvCreateImage(cvSize(pyrImgPrev->pyrLevel[i].spec.width, pyrImgPrev->pyrLevel[i].spec.height), IPL_DEPTH_8U, 1);
            src = (char *) pyrImgPrev->pyrLevel[i].p1 + PADDING_H;
            dst = tempPreviousGrayImg[i]->imageData;
            for (int j = 0; j < pyrImgPrev->pyrLevel[i].spec.height; j++)
                for (int k = 0; k < pyrImgPrev->pyrLevel[i].spec.width; k++)
                    dst[j * tempPreviousGrayImg[i]->widthStep + k] = src[j * pyrImgPrev->pyrLevel[0].spec.stride + k];
        }
    }

    // cvCalcOpticalFlowPyrLK() requires CvPoint2D32f type for computing corners
    CvPoint2D32f*   cornersPreviousFrame    = new CvPoint2D32f[featuresCount];
    CvPoint2D32f*   cornersCurrentFrame     = new CvPoint2D32f[featuresCount];
    char            featuresFound[featuresCount];
    float           featureErrors[featuresCount];

    // save corners in CvPoint2D32f* type for cvCalcOpticalFlowPyrLK
    for(int i = 0; i < featuresCount; i++) {
        cornersPreviousFrame[i].x = featuresPrev[i].x;
        cornersPreviousFrame[i].y = featuresPrev[i].y;
        cornersCurrentFrame[i].x = featuresCur[i].x;
        cornersCurrentFrame[i].y = featuresCur[i].y;
    }

    // Refine corner locations
    if (cfgDoSubpixelDetection) {
        cvFindCornerSubPix(tempPreviousGrayImg,
                cornersPreviousFrame,
                featuresCount,
                cvSize(FIND_SUBPIX_CORNER_WINDOW_SIZE,FIND_SUBPIX_CORNER_WINDOW_SIZE),
                cvSize(-1,-1),
                cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, FIND_SUBPIX_CORNER_ITERATIONS, FIND_SUBPIX_CORNER_EPSILON));
    }


    // Optical Flow
    if (0 == cfgDoOfMovi) {

        // CvSeq containing image pyramid -> calling cvCalcOpticalFlowPyrLK() requires CvSeq
        CvSeq *pyrSeq, *previousPyrSeq;
        CvMemStorage* storage = cvCreateMemStorage(0);
        previousPyrSeq = cvCreateSeq(0, sizeof(CvSeq),sizeof(IplImage), storage);
        pyrSeq = cvCreateSeq(0, sizeof(CvSeq),sizeof(IplImage), storage);
        for (int i = 1; i < g_num_pyr_levels+1; i++)
        {
            cvSeqPush(previousPyrSeq, tempPreviousGrayImg[i]);

        }

        for (int i = 1; i < g_num_pyr_levels+1; i++)
        {
            cvSeqPush(pyrSeq, tempGrayImg[i]);
        }
        //printf("Corners: %d \n", count);
        cvCalcOpticalFlowPyrLK (  tempPreviousGrayImg,                                  // previous image
                tempGrayImg,                                          // current image
                previousPyrSeq,                                       // previous image pyramids sequence
                pyrSeq,                                               // current image pyramids sequence
                cornersPreviousFrame,                                 // corners from previous image
                cornersCurrentFrame,                                  // corners in second image
                featuresCount,
                cvSize(g_winSize.width, g_winSize.height),
                g_num_pyr_levels,                                       // Number of levels
                featuresFound,                                        // array determining if feature has been found
                featureErrors,                                        // error associated with feature found
                cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, g_termCriteria.max_iter, g_termCriteria.epsilon),
                0);
        // release allocated memory
        cvClearMemStorage(storage);
        cvReleaseMemStorage(&storage);
    }
    else if (1 == cfgDoOfMovi) {
        // Call Our Optical Flow
        //printf("Corners: %d \n", count);
        mvCvTermCriteria movi_termCrt = {CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, g_termCriteria.max_iter, g_termCriteria.epsilon};
        // cv::Mat is not full copatible with mvcv::Mat ==> copy data
        for (u32 i = 0; i < g_num_pyr_levels+1; i++)
        {
            //printf("Width: %d, Height: %d, Stride: %d, startptr: %x\n", pyrImgPrev->pyrLevel[i].spec.width, pyrImgPrev->pyrLevel[i].spec.height, pyrImgPrev->pyrLevel[i].spec.stride, pyrImgPrev->pyrLevel[i].p1);
            movi_PyrA[i].data =  pyrImgPrev->pyrLevel[i].p1 + PADDING_H;
            movi_PyrB[i].data =  pyrImgCur->pyrLevel[i].p1 + PADDING_H;


        }
        // new MvCV Optical Flow
        mvcvCalcOpticalFlowPyrLK_tuned (movi_PyrA,
                movi_PyrB,
                (mvCvPoint2D32f*)cornersPreviousFrame,
                (mvCvPoint2D32f*)cornersCurrentFrame,
                (u8*)featuresFound,
                featureErrors,
                mvCvSize(g_winSize.width, g_winSize.height),
                g_num_pyr_levels,
                movi_termCrt,
                MVCV_LKFLOW_INITIAL_GUESSES,
                featuresCount);


        // Scale errors within the range that makes sense to FM
        for (u32 i = 0; i < featuresCount; i++)
        {
            featureErrors[i] *= 200.0f;
        }

        // save tracked corners
        for (u32 i = 0; i < featuresCount; i++) {
            featuresCur[i].x = cornersCurrentFrame[i].x;
            featuresCur[i].y = cornersCurrentFrame[i].y;
            featuresError[i] = featureErrors[i];
        }
    }

    delete[] cornersPreviousFrame;
    delete[] cornersCurrentFrame;

    if(0 == cfgDoOfMovi || cfgDoSubpixelDetection) {
        for(int i = 0; i < g_num_pyr_levels + 1; i++) {
            cvReleaseImage(&tempGrayImg[i]);
            cvReleaseImage(&tempPreviousGrayImg[i]);
        }
    }
}
