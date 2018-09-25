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

#include "testHwLsc.h"

#include <include/sippHwDefs.h>
#include <arch/ma2x5x/include/sippHwDefs_ma2x5x.h>


//The full-frame buffers
UInt16     DDR_DATA TestHwLsc_dmaIn0_buffer  [SIPP_HW_LSC_BUFFER_WIDTH* SIPP_HW_LSC_BUFFER_HEIGHT* SIPP_HW_LSC_BUFFER_NUM_PLANES]ALIGNED(8);
UInt16     DDR_DATA TestHwLsc_dmaOut0_buffer [SIPP_HW_LSC_BUFFER_WIDTH* SIPP_HW_LSC_BUFFER_HEIGHT* SIPP_HW_LSC_BUFFER_NUM_PLANES]ALIGNED(8);
	
///
void readTestHwLscInput(TestHwLsc *pPl)
{
   sippRdFileU8((UInt8*)TestHwLsc_dmaIn0_buffer,SIPP_HW_LSC_BUFFER_WIDTH * SIPP_HW_LSC_BUFFER_HEIGHT * SIPP_HW_LSC_BUFFER_NUM_PLANES* sizeof(UInt16),"../../../../../../../resources/wall_3264x2448_hwlscOFF_5.7ms_1x.raw");
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);
	
}

void writeTestHwLscOutput(TestHwLsc *pPl)
{
   sippWrFileU8((UInt8*)TestHwLsc_dmaOut0_buffer,SIPP_HW_LSC_BUFFER_WIDTH * SIPP_HW_LSC_BUFFER_HEIGHT * SIPP_HW_LSC_BUFFER_NUM_PLANES* sizeof(UInt16),"output.raw");
   //Ensure parameter is used to avoid compiler warning
   (void)(pPl);

}
   
void buildTestHwLsc(TestHwLsc *pPl)
{
   // create new pipeline
   pPl->pl               = sippCreatePipeline(0, 0, SIPP_MBIN(mbinImgSipp));

   // create filters
   pPl->dmaIn0          =  sippCreateFilter(pPl->pl, 0x00,       SIPP_HW_LSC_BUFFER_WIDTH, SIPP_HW_LSC_BUFFER_HEIGHT, N_PL(SIPP_HW_LSC_BUFFER_NUM_PLANES), SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
   pPl->lscMa2x5x0      =  sippCreateFilter(pPl->pl, 0x0,        SIPP_HW_LSC_BUFFER_WIDTH, SIPP_HW_LSC_BUFFER_HEIGHT, N_PL(SIPP_HW_LSC_BUFFER_NUM_PLANES), SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_LSC_ID,        0);
   pPl->dmaOut0         =  sippCreateFilter(pPl->pl, 0x00,       SIPP_HW_LSC_BUFFER_WIDTH, SIPP_HW_LSC_BUFFER_HEIGHT, N_PL(SIPP_HW_LSC_BUFFER_NUM_PLANES), SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);
	
   // link filters
   sippLinkFilter(pPl->lscMa2x5x0,    pPl->dmaIn0,        1,0                 );
   sippLinkFilter(pPl->dmaOut0,       pPl->lscMa2x5x0,    1,1                 );
	
}

  
void configTestHwLsc(TestHwLsc *pPl)
{
   DmaParam            *dmaIn0Cfg          = (DmaParam         *)pPl->dmaIn0->params;
   LscParam            *lscMa2x5x0Cfg      = (LscParam         *)pPl->lscMa2x5x0->params;
   DmaParam            *dmaOut0Cfg         = (DmaParam         *)pPl->dmaOut0->params;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // here go through all the filter instances and assign the value for all parameter
  // fields as defined in the filter property window
   lscMa2x5x0Cfg->gmBase=   bayerGainMesh;
   lscMa2x5x0Cfg->gmWidth=  GM_WIDTH;
   lscMa2x5x0Cfg->gmHeight= GM_HEIGHT;
   lscMa2x5x0Cfg->dataFormat=SIPP_LSC_CFG_FORMAT(FORMAT);
   lscMa2x5x0Cfg->dataWidth=DATA_WIDTH;
   dmaIn0Cfg->ddrAddr   = (UInt32)&TestHwLsc_dmaIn0_buffer;
   dmaOut0Cfg->ddrAddr  = (UInt32)&TestHwLsc_dmaOut0_buffer;
  
  
}