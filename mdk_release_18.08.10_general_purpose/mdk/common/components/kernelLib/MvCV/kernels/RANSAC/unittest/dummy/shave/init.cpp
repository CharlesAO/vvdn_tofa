#include <swcFrameTypes.h>
#include <svuCommonShave.h>
#include <stdio.h>
#include <string.h>
#include <mv_types.h>
//#include "shaveStart.h"
#include "../shared/RANSAC.h"
#include "../shared/FundamentalMatrix.h"
//#define CVMA2480
#ifdef CVMA2480
#include "ShDrvCmxDma.h"
#endif
#ifdef CVMA2150
#include "swcCdma.h"
#endif
#ifdef CVMA2450
#include "swcCdma.h"
#endif
#include "../shared/Sections.h"
#define printf(...)
#define EXTERN extern "C"

EXTERN void  start(RANSAC_input *Inputs,RANSAC_output *Outputs);

float  cmxp1[2*MAX_NUMPOINTS];
float  cmxp2[2*MAX_NUMPOINTS];
/*output pre pad marker*/
int32_t  __attribute__((section(".kept.data"))) cmxInliersprePad[4] __attribute__ ((aligned (16)));
//uint32_t cmxInliersprePad;
/*output data marker*/
int __attribute__((section(".kept.data"))) cmxInliers[MAX_NUMPOINTS] __attribute__ ((aligned (16)));
//int cmxInliers[MAX_NUMPOINTS];
/*output post pad marker*/
int32_t __attribute__((section(".kept.data"))) cmxInlierspostPad[4] __attribute__ ((aligned (16)));
//uint32_t cmxInlierspostPad;
/*output pre pad marker*/
int32_t  __attribute__((section(".kept.data"))) fmprePad[4] __attribute__ ((aligned (16)));
//uint32_t fmprePad;
/*output data marker*/
float __attribute__((section(".kept.data"))) fm[9] __attribute__ ((aligned (16)));
//float  fm[9];
/*output post pad marker*/
int32_t  __attribute__((section(".kept.data"))) fmpostPad[4] __attribute__ ((aligned (16)));
//uint32_t fmpostPad;
/*output pre pad marker*/
int32_t  __attribute__((section(".kept.data"))) DebugprePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
int __attribute__((section(".kept.data"))) Debug[MAX_NUMPOINTS] __attribute__ ((aligned (16)));
/*output post pad marker*/
int32_t  __attribute__((section(".kept.data"))) DebugpostPad[4] __attribute__ ((aligned (16)));
#ifdef CVMA2480
ShDrvCmxDmaTransaction __attribute__((section(".cmx_direct.data"))) list[2];
ShDrvCmxDmaTransactionHnd handle;
#endif
#ifdef CVMA2150
dmaTransactionList_t CMX_DMA_DESCRIPTORS task1, task2;
#endif
#ifdef CVMA2450
dmaTransactionList_t CMX_DMA_DESCRIPTORS task1, task2;
#endif
RANSAC_input RANSAC_Inputs;
RANSAC_output RANSAC_Outputs;

EXTERN int main(void)
{
  printf("SHAVE HERE\n");
  start(&RANSAC_Inputs,&RANSAC_Outputs);
  return 0;
}


EXTERN void start(RANSAC_input *Inputs,RANSAC_output *Outputs)
{
#ifdef CVMA2480
  ShDrvCmxDmaInitialize(NULL);
  memset(&handle, 0, sizeof(handle));
  memset(&list, 0, sizeof(list));

  ShDrvCmxDmaCreateTransaction(&handle, &list[0], (u8*)Inputs->p1, (u8*)&cmxp1[0], 2 * Inputs->Params->nPoints * sizeof(float));
  ShDrvCmxDmaAddTransaction(&handle, &list[1], (u8*)Inputs->p2, (u8*)&cmxp2[0], 2 * Inputs->Params->nPoints * sizeof(float));

  ShDrvCmxDmaStartTransfer(&handle);
  ShDrvCmxDmaWaitTransaction(&handle);
#endif
#ifdef CVMA2150
  dmaTransactionList_t *ref1;
  dmaTransactionList_t *ref2;

  u32 id1 = dmaInitRequester(1);

  // DMA in the points
  ref1 = dmaCreateTransaction(id1, &task1, (u8*)Inputs->p1, (u8*)&cmxp1[0], 2 * Inputs->Params->nPoints * sizeof(float));
  ref2 = dmaCreateTransaction(id1, &task2, (u8*)Inputs->p2, (u8*)&cmxp2[0], 2 * Inputs->Params->nPoints * sizeof(float));

  //printf("S2\n");
  dmaLinkTasks(ref1, 1, ref2);
  //printf("S3\n");
  dmaStartListTask(ref1);

  dmaWaitTask(ref1);
#endif
#ifdef CVMA2450
  dmaTransactionList_t *ref1;
  dmaTransactionList_t *ref2;

  u32 id1 = dmaInitRequester(1);

  // DMA in the points
  ref1 = dmaCreateTransaction(id1, &task1, (u8*)Inputs->p1, (u8*)&cmxp1[0], 2 * Inputs->Params->nPoints * sizeof(float));
  ref2 = dmaCreateTransaction(id1, &task2, (u8*)Inputs->p2, (u8*)&cmxp2[0], 2 * Inputs->Params->nPoints * sizeof(float));

  //printf("S2\n");
  dmaLinkTasks(ref1, 1, ref2);
  //printf("S3\n");
  dmaStartListTask(ref1);

  dmaWaitTask(ref1);
#endif
  // Calculate the fundamental matrix
  findFundamentalMat(&cmxp1[0], &cmxp2[0],Inputs->Params->nPoints,Outputs->fm,&cmxInliers[0],Outputs->Debug,Inputs->Params->inliers_ratio,Inputs->Params->confidence,Inputs->Params->dist_threshold,Inputs->Params->max_iterations);
  //printf("S6\n");
  SHAVE_HALT;

  // DMA the outputs

}


