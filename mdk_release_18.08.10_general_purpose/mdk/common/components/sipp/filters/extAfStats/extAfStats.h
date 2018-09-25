///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref afStats SIPP filter API.
///

#ifndef __SIPP_EXTAFSTATS_H__
#define __SIPP_EXTAFSTATS_H__ 


#include <sipp.h>

/// @sf_definition afStats AF Stats
/// @sf_description This filter gathers auto-focus statistics from a RGB planar image.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{

typedef struct
{
    UInt32 base;     //+0x0000 : base address
    UInt32 cfg;      //+0x0004 : configuration
    UInt32 ls;       //+0x0008 : line stride
    UInt32 ps;       //+0x000C : plane stride
    UInt32 irqRate;  //+0x0010 : fileds for Chunk-size !
    UInt32 fillCtrl; //+0x0014 : unused in sync mode
    UInt32 ctx;      //+0x0018 : context/status
}SippHwIOBuf;

/// @brief Parameter structure of the @ref afStats filter.
typedef struct {
    struct {
        UInt32    firstPatchX;        /* X coord of top-left pixel of first patch */
        UInt32    firstPatchY;        /* Y coord of top-left pixel of first patch */
        UInt32    patchWidth;
        UInt32    patchHeight;
        UInt32    nPatchesX;
        UInt32    nPatchesY;
        UInt32    nSkipRows;          /* Number of skipped lines in calc; 0 = no skipping */
        Int32     initialSubtractionValue;
        Int32     f1Threshold;
        Int32     f2Threshold;
        Int32     f2Coeffs[11];
        Int32     f1Coeffs[11];
    } afConfig;
    UInt32*     outStatsBuffer;
    UInt32      runNr[2];
    UInt32      posInPaxel[2];
    UInt32      paxelNr[2];
    Int32       a_f1f2[4]; // for second half line processing
    Int32       b_f1f2[4];
    UInt32      mask;
    SippHwIOBuf in[2];
    UInt32*     outStatsBufferOutside;
    void        (*statsAf0)(void *);
    UInt32      shaveNr;
    UInt32      stackPointer;
}PpAf;


/// @brief Shave function of the @ref afStats filter.
void SVU_SYM(svuExtAfStats)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuExtAfStats);

/// @}

#endif // __SIPP_EXTAFSTATS_H__ 
