/**************************************************************************************************

 @File         : PlgSourceApi.h
 @Author       : AL
 @Brief        : Contain Source configuration plug-in interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

     Resources used:
         Leon RT.
         SippRx.
         Cif.
         MipiPhy.
         MipiCtrl.

    Interrupt base.

 **************************************************************************************************/
#ifndef __PLG_MIPI_RX_API_H
#define __PLG_MIPI_RX_API_H

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "PlgTypes.h"
#include "ipipe.h" // TODO: replace ipipe.h dependency (icSourceConfig)
#include "Opipe.h"
#include "OpipeApps.h"
#include "DrvMipiDefines.h"
#include "MemMgrApi.h"

/**************************************************************************************************
 ~~~  Basic typedefs
 **************************************************************************************************/
typedef enum {
    PLG_SOURCE_NOTMADE = 0,
    PLG_SOURCE_CREATED = 1,
    PLG_SOURCE_INUSE   = 2
}PlgSourceStatus;

typedef enum {
    SOF_IDX_SRC = 0,
    EOF_IDX_SRC = 1,
    HIT_IDX_SRC = 2
}LineRefs;

typedef struct
{
    PlgType plg;
  //Main opipe objects
    OpipeRx   pRx;
    MipiRxCfg rxCfg; //used when source is a SIPP-RX block

  //Specific component interface
    void   (*eofEvent)(void *plg, FrameT *frame); //End-Of-Frame
    void   (*sofEvent)(void *plg, FrameT *frame); //Start-Of-Frame
    void   (*hitEvent)(void *plg, FrameT *frame); //Line-Hit

  //Private members. Do not touch!
    FramePool       *outputPools;
    FrameT          *frame;
    uint32_t        srcId;
    uint32_t        frameCnt;
    icMipiConfig    mipiRxData;
    PlgSourceStatus status;
    int             downshift; //Downshift data at receiver if necessary.

    FrameT               *inProcessFrame;
    FrameT               *producedFrame;
    uint32_t             receiverEofIdx; // cif or sipp finished before mipi Controller eof;
    uint32_t             mipiCtrlEofIdx;
    uint32_t             conectedToController;
} PlgSource;

/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void PlgSourceCmxAlloc  (uint32_t instNo, icSourceSetup *src, MemPoolId id);

void PlgSourceStart     (void *pluginObject, icSourceConfig *sourceConfig, uint32_t outFmt);
void PlgSourceCreate    (void *pluginObject, icSourceInstance instId);
void PlgSourceSetLineHit(void *pluginObject, uint32_t lineNo);

#endif //__PLG_MIPI_RX_API_H
