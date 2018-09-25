#ifndef __RX_ISP_OPIPE_DEF_H_
#define __RX_ISP_OPIPE_DEF_H_

#include "Opipe.h"
#include "OpipeDefs.h"
#include "OpipeApps.h" //Buf def

//#define MIPI_CMX_SIGMA

typedef struct
{
  Opipe     p;
  uint32_t  cdmaReq;

  DBuffer  *pOutY;
  DBuffer  *pOutUV;

  Buf   cBufMipiOut; //optional
  Buf   cBufDbyrY;
  Buf   cBufSharp;
  Buf   cBufLut;
  Buf   cBufPoly;

  //UPFIRDN_1 reference buffers for general resize case
  DBuffer  *pOutLut;//input ref
  DBuffer  *pInUV;  //input
}
OpipeRxIsp;

void OpipeCreateRxIsp(OpipeRxIsp *pRx,  uint32_t mipiRxId);

#endif