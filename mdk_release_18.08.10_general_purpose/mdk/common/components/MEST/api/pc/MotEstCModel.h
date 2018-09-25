///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Common header for MEST wrapper and C Model.
///
/// This is the common area for wrapper and C Model
///

#ifndef _CMODEL_H_
#define _CMODEL_H_

/*****************************************************************************/
/* Windsor - Advanced Video Encoder                                          */
/* C MODEL related tasks                                                     */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "MotEst_wrapper.h"

enum ImgMode_e
{
    IMG_SEQ, IMG_SINGLE, IMG_SYNTH, IMG_RAND
};

enum SrchMode_e
{
    SRCH_FULL, SRCH_SINGLE, SRCH_RAND, SRCH_LIST
};

////////////////////////////
// C Model control variables
////////////////////////////


class CCModel
{
public:

    CCModel();
    ~CCModel();

    // Log/statistics file generation
    int InitLogFile(char FileName[]);

    // Variable to enable/disable debug dumping to file - defaults to false upon initialization
    FILE *fp_log;
    FILE *fp_src;
};

#define   IMG_MAX_WIDTH   8192
#define   IMG_MAX_HEIGHT  4096

extern uint8_t line[IMG_MAX_WIDTH];
extern uint8_t img[2][IMG_MAX_WIDTH * IMG_MAX_HEIGHT];
extern uint8_t img_arr[IMG_MAX_HEIGHT][IMG_MAX_WIDTH];
extern uint8_t img_lpf[IMG_MAX_HEIGHT][IMG_MAX_WIDTH];
extern uint32_t results[IMG_MAX_HEIGHT * IMG_MAX_WIDTH];
extern uint32_t coords[IMG_MAX_HEIGHT * IMG_MAX_WIDTH];

extern int32_t sumOfVectorsX, sumOfVectorsY, numOfSearchedVectors;
extern void *(*callPreprocImg)(void *arg), *callPreprocImgArg;
extern void *(*callPostprocImg)(void *arg), *callPostprocImgArg;

int MotEstConfig(ARGS args);

#endif // _CMODEL_H_
