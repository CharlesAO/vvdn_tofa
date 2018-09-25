// PC MODEL SPECIFIC DEFINES

#ifndef PC_PARAMETERS_H_
#define PC_PARAMETERS_H_
#pragma once

#include "vTrackOutput.h"
// Setup input type
//#define WEB_CAM   // Use this to activate webcam, if off - use a single image siftied each iteration
#define DATASET // Use this to activate dataset, if off - use a single image siftied each iteration

// Algo Config
const int   cfgDoHarrisOrFast           = 0;    // 0 - Only use harris image and NMS to generate feature points
                                                // 1 - Use FAST points and sort by harris score @ that point
const int   cfgDoFastDetection          = 1;    // 0 - do for the first frame only; 1 - do each frame;
const int   cfgDoSubpixelDetection      = 0;    // 0 - keep initial position; 1 - refine and detect with subpixel accuracy;
const int   cfgDoOfMovi                 = 1;    // 0 - LK OpenCV OF; 
                                                // 1 - LK MvCV OF; 
const int   cfgMaxIter                  = 2000;  // max number of frames processed;

// Results display
#define WaitDebug cvWaitKey(10)
#define BinaryFileW //fwrite        // activate define if you wanna save buffer to a binary file
const int cfgShowCells      = 1;    // 0 - off; 1 - show cell grid for corners image
const int cfgShowCorners    = 1;    // 0 - off; 1 - show corners image
const int cfgShowOf         = 1;    // 0 - off; 1 - show OF image
const int cfgShowCornerIfno = 2;    // 0 - no info displayed; 1 - dispaly pos and age; 2 - only age
const int cfgScript         = 1;    // 0 - off; 1 - sace script (feature_tracks.txt) file

// Prefered frame size for webcam video stream
#define FRAME_WIDTH         640
#define FRAME_HEIGHT        480

// Fast Config
const int   cfgFastInit                 = 1;    // initial FAST threshold value
const float cfgFastIncreaseFactor       = 1.05f;    // Multiplication factor for FAST threshold used when number of corners > cfgFastTargetCornersPerCell
const float cfgFastDecreaseFactor       = 0.95f;    // Multiplication factor for FAST threshold used when number of corners < cfgFastTargetCornersPerCell
const float cfgFastMinTh                = 1;
const float cfgFastMaxTh                = 100;
const int   cfgDoFastThresholdUpdate     = 0;    // 0 - keep FAST threshold fix; 1 - update each interation;

// Harris Config
const int   cfgHarrisKernelSize       = 3;
const int   cfgHarrisApertureSize     = 3;
const float cfgHarrisK                = 0.04f;
const int   cfgNormaliseHarris        = 0;    // 0 - Don't normalise harris image; 1 - Normalise harris image

// Optical Flow Config
const int cfgOfPyrLevels = 4;                   // 4 for 480, and 5 for bigger resolutions
const int cfgOfWinSize   = 5;
const int cfgOfMaxIter   = 9;
const float cfgOfEpsilon = 0.01f;
const int errorWinSize   = 5;

#endif // PC_PARAMETERS_H_
