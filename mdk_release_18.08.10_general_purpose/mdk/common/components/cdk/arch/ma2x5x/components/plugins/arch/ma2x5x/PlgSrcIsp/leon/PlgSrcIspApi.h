#ifndef __PLG_SRC_ISP_API_H__
#define __PLG_SRC_ISP_API_H__

#include "PlgTypes.h"
#include "ipipe.h"
#include "Opipe.h"
#include "PlgSrcPipeDef.h"
#include "IspCommonUtils.h"
#include "DrvMipiDefines.h"
#include "MemMgrApi.h"

typedef enum {
    PLG_SRC_ISP_NOTMADE = 0,
    PLG_SRC_ISP_CREATED = 1,
    PLG_SRC_ISP_INUSE   = 2
}PlgSrcIspStatus;

typedef enum {
    SOF_IDX = 0,
    EOF_IDX = 1,
    HIT_IDX = 2
}LineRefsSrc;

typedef enum {
    ERR_PLGSRCISP_NO_OUT_BUF,
    ERR_PLGSRCISP_NO_ISP_CFG,
}PlgSrcIspErrors;

typedef struct
{
    PlgType plg;

  //Main opipe objects
    OpipeRxIsp pRxIsp;  //used to do ISP
    OpipeRx    pRxSkip; //used to skip frames (when cfg or oFrame are not available)

    FramePool       *outputPools;
    FrameT          *frame;
    PlgSrcIspStatus status;

  /*RX*/  void (*eofEvent)(void *plg, FrameT *frame); //End-Of-Frame
  /*RX*/  void (*sofEvent)(void *plg, FrameT *frame); //Start-Of-Frame
  /*RX*/  void (*hitEvent)(void *plg, FrameT *frame); //Line-Hit
  /*RX*/
  /*RX*/  uint32_t      srcId;
  /*RX*/  MipiRxCfg     rxCfg;
  /*RX*/  icMipiConfig  mipiRxData;
  /*RX*/  int           downshift; //Downshift data at receiver if necessary.

  /*ISP*/ icIspConfig *curIspCfg;
  /*ISP*/ icIspConfig *nxtIspCfg;
  /*ISP*/ icSize       frmSz;     //input frame size
  /*ISP*/ YuvScale     scale;     //yuv420 arbitrary scale output

  /*PLG*/ void (*procesIspError) (void* plg, PlgSrcIspErrors errorNo);
} PlgSrcIsp;

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void PlgSrcIspCmxAlloc  (PlgSrcIsp *obj, icSourceSetup *src, MemPoolId id);

void PlgSrcIspStart     (void *pluginObject, icSourceConfig *sourceConfig);
void PlgSrcIspCreate    (void *pluginObject, icSourceInstance instId);
void PlgSrcIspSetLineHit(void *pluginObject, uint32_t lineNo);

#endif //__PLG_MIPI_RX_API_H
