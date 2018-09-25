#ifndef IPIPE_OPIPE_UTILS_H
#define IPIPE_OPIPE_UTILS_H

#include "ipipe.h"
#include "Opipe.h"
#include "MemMgrApi.h"
#include "IspCommonUtils.h" //PlgIspBase defs

MemPoolId CheckAllocOmemPool();
void      AllocOpipeReset      ();
void      AllocOpipeRxCmxBuffs (icCtrl *ctrl);
void      AllocOpipeIspCmxBuffs(icCtrl *ctrl, uint32_t maxCams, PlgIspBase *ispPlg[]);
void      AllocIspBuffs        (PlgIspBase *base, uint32_t inW, uint32_t mainW, uint32_t outW);

void      dbgSetSourceSetup(icCtrl *ctrl, uint32_t no, uint32_t maxW, uint32_t maxB);
uint32_t  GetFrameBppPackFormat(uint32_t bpp);


#endif
