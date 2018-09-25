///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Setup configuration
///
///
///
///
///

#ifndef _SETUP_H_
#define _SETUP_H_

#include <Defines.h>
#include <DisparityMapApiDefines.h>
#include <sipp.h>
#include <sippTestCommon.h>

/******************************************************************************

 @brief     : Setup function, used to set all needed addresses

 @param     : pyrCfg                : pointer to DisparityConfig structure
 @param     : algoCfg   			: pointer to AlgorithmConfig structure
 @param     : imgL              	: pointer to the left image
 @param     : imgR              	: pointer to the right image
 @param     : disparities          	: pointer to the disparities image
 @param     : bpp              		: bytes per pixel

 @return    : void

******************************************************************************/
void setupConfiguration(DisparityConfig* pyrCfg, AlgorithmConfig* algoCfg, u8* imgL, u8* imgR, u8* disparities, u32 width, u32 height, u32 bpp);

/******************************************************************************

 @brief     : Function to split an image into patches

 @param     : originalFrame         : pointer to input image
 @param     : frameRectified   		: pointer to rectified image
 @param     : disparity             : pointer to disparity buffer
 @param     : pyrCfg                : pointer to DisparityConfig structure
 @param     : algoCfg   			: pointer to AlgorithmConfig structure
 @param     : width                	: image width
 @param     : height   				: image height

 @return    : void

******************************************************************************/
void computePatches(u8* originalFrame, u8* frameRectified, u8** disparity, AlgorithmConfig* algoCfg, DisparityConfig* pyrCfg, u32 width, u32 height);

/******************************************************************************

 @brief     : Function to reconstruct image from patches

 @param     : frameBufferDisparities   		: pointer to disparity buffer
 @param     : pyrCfg                : pointer to DisparityConfig structure

 @return    : void

******************************************************************************/
void recomputeImageFromPatches(u8* frameBufferDisparities, DisparityConfig* pyrCfg);

/******************************************************************************

 @brief     : Function for initializing the buffers with a random value

 @param     : frameBufferDisparitiesMedian	: pointer to disparity buffer after applying the median
 @param     : frameBufferDisparities   		: pointer to disparity buffer
 @param     : disparityMap              	: pointer to disparity map buffer that contains the patches
 @param		: usbBufPtr						: pointer to USB buffers

 @return    : void

******************************************************************************/
void setBuffers(u8* frameBufferDisparitiesMedian, u8* frameBufferDisparities, u8** disparityMap, u8** usbBufPtr);

#endif // _SETUP_H_
