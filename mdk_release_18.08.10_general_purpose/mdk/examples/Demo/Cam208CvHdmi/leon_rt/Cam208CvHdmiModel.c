///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the source code of the SIPP pipeline
///            generated by the SIPP Graph Designer.
///
///
///  ************************ DO NOT EDIT, THIS IS A GENERATED FILE!!!! ************************
/// 

#include "Cam208CvHdmiModel.h"

#include <include/sippHwDefs.h>
#include <filters/sobel/sobel.h>
#include <filters/erode3x3/erode3x3.h>
#include <filters/dilate3x3/dilate3x3.h>
#include <arch/ma2x5x/include/sippHwDefs_ma2x5x.h>


//The full-frame buffers
	
///
void readCam208CvHdmiModelInput(Cam208CvHdmiModel *pPl)
{
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);
	
}

void writeCam208CvHdmiModelOutput(Cam208CvHdmiModel *pPl)
{
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);

}
   
void buildCam208CvHdmiModel(Cam208CvHdmiModel *pPl)
{
   // create new pipeline
   pPl->pl               = sippCreatePipeline(0, 6, SIPP_MBIN(mbinImgSipp));

   // create filters
   pPl->dmaIn0          =  sippCreateFilter(pPl->pl, 0x00,       1920,   1080,   N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
   pPl->sobel0          =  sippCreateFilter(pPl->pl, 0x0,        1920,   1080,   N_PL(1),  SZ(UInt8), 0,                  SVU_SYM(svuSobel),            0);
   pPl->erode3x30       =  sippCreateFilter(pPl->pl, 0x0,        1920,   1080,   N_PL(1),  SZ(UInt8), SZ(Erode3x3Param),  SVU_SYM(svuErode3x3),         0);
   pPl->dilate3x30      =  sippCreateFilter(pPl->pl, 0x0,        1920,   1080,   N_PL(1),  SZ(UInt8), SZ(Dilate3x3Param), SVU_SYM(svuDilate3x3),        0);
   pPl->lutMa2x5x0      =  sippCreateFilter(pPl->pl, 0x0,        1920,   1080,   N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_LUT_ID,        0);
   pPl->dmaOut0         =  sippCreateFilter(pPl->pl, 0x00,       1920,   1080,   N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
	
   // link filters
   sippLinkFilter(pPl->sobel0,        pPl->dmaIn0,        3,3                 );
   sippLinkFilter(pPl->erode3x30,     pPl->sobel0,        3,3                 );
   sippLinkFilter(pPl->dilate3x30,    pPl->erode3x30,     3,3                 );
   sippLinkFilter(pPl->lutMa2x5x0,    pPl->dilate3x30,    1,1                 );
   sippLinkFilter(pPl->dmaOut0,       pPl->lutMa2x5x0,    1,1                 );
	
}

  
void configCam208CvHdmiModel(Cam208CvHdmiModel *pPl)
{
   DmaParam            *dmaIn0Cfg          = (DmaParam         *)pPl->dmaIn0->params;
   Erode3x3Param       *erode3x30Cfg       = (Erode3x3Param    *)pPl->erode3x30->params;
   Dilate3x3Param      *dilate3x30Cfg      = (Dilate3x3Param   *)pPl->dilate3x30->params;
   LutParam            *lutMa2x5x0Cfg      = (LutParam         *)pPl->lutMa2x5x0->params;
   DmaParam            *dmaOut0Cfg         = (DmaParam         *)pPl->dmaOut0->params;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // here go through all the filter instances and assign the value for all parameter
  // fields as defined in the filter property window
   erode3x30Cfg->eMat[0]=   &erodeMat[0];
   erode3x30Cfg->eMat[1]=    &erodeMat[3];
   erode3x30Cfg->eMat[2]=    &erodeMat[6];
   dilate3x30Cfg->dMat[0]=  &dilateMat[0];
   dilate3x30Cfg->dMat[1]=   &dilateMat[3];
   dilate3x30Cfg->dMat[2]=   &dilateMat[6];
   lutMa2x5x0Cfg->cfg=      (0      )  | (0  << 1)  | (8  << 3) | (0  << 8);
   lutMa2x5x0Cfg->sizeA=    0x44444444;
   lutMa2x5x0Cfg->sizeB=    0x44444444;
   lutMa2x5x0Cfg->lut=      lutTab;
   lutMa2x5x0Cfg->lutFormat=LUT_LD_FORMAT;
   lutMa2x5x0Cfg->mat[0]=   0;
   lutMa2x5x0Cfg->mat[1]=    0;
   lutMa2x5x0Cfg->mat[2]=    0;
   lutMa2x5x0Cfg->mat[3]=    0;
   lutMa2x5x0Cfg->mat[4]=    0;
   lutMa2x5x0Cfg->mat[5]=    0;
   lutMa2x5x0Cfg->mat[6]=    0;
   lutMa2x5x0Cfg->mat[7]=    0;
   lutMa2x5x0Cfg->mat[8]=    0;
   lutMa2x5x0Cfg->offset[0]=0;
   lutMa2x5x0Cfg->offset[1]= 0;
   lutMa2x5x0Cfg->offset[2]= 0;
   dmaIn0Cfg->ddrAddr   = (UInt32)iBuf[0];
   dmaOut0Cfg->ddrAddr  = (UInt32)oBuf[0];
  
  
}
