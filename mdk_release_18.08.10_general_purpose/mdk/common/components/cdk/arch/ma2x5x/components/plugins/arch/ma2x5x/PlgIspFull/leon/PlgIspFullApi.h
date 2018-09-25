/**************************************************************************************************

 @File         : PlgIspFullApi.h
 @Author       : AL
 @Brief        : Contain Opipe full Isp plug-in interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

     Resources used:
         Leon RT.
         All sipp isp filters

    Interrupt base.

 **************************************************************************************************/
#ifndef __PLG_ISP_FULL_API__
#define __PLG_ISP_FULL_API__

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "PlgTypes.h"
#include "IspCommon.h"
#include "MemMgrApi.h"
#include "Opipe.h"
#include "OpipeApps.h"
#include "IspCommonUtils.h"
#include "ipipe.h"


/**************************************************************************************************
 ~~~  Basic typedefs
 **************************************************************************************************/
 // After a stop, at the new start not allocate again the buffers, just change the size
typedef enum {
    PLG_ISPFULL_NOTMADE = 0,
    PLG_ISPFULL_CREATED = 1,
    PLG_ISPFULL_INUSE   = 2
}PlgIspFullStatus;

typedef struct PlgIspFullStruct {
    PlgType    plg;
    PlgIspBase base;

  //Main opipe objects
    OpipeMF    op;
  //Required circular buffers
    uint8_t   *cSigma;
    uint8_t   *cDbyrY;
    uint8_t   *cSharpY;
    uint8_t   *cLut;
    uint8_t   *cUpfirDn;
    uint8_t   *cDbyrIn;
    //ISP params
    icIspConfig  *ispCfg;
    icSize        frmSz; //input frame size
    //Specific component interface
    void    (*procesStart)    (void* plg, uint32_t seqNr, void *userData);
    void    (*procesEnd  )    (void* plg, uint32_t seqNr, void *userData);
    void    (*procesIspError) (void* plg, icSeverity severity, icError errorNo, void *userData);

    //Private members. All data structures have to be internal
    FramePool               *outputPools;
    volatile int32_t        crtStatus; //internal usage
    FrameProducedCB         cbList[1];
    PlgIspFullStatus        status;    //used for avid double plug-in initialization start/stop.
    YuvScale                scale;     //yuv420 arbitrary scale output
} PlgIspFull;


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void PlgIspFullCreate(void *pluginObject);
void PlgIspFullConfig(void *pluginObject, icSize frameSz, uint32_t inFmt, uint32_t prevAble);

#endif
