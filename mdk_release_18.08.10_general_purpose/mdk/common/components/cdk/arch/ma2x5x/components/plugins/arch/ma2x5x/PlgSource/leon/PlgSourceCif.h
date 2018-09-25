/**************************************************************************************************

 @File         : PlgSourceCif.h
 @Author       : MT
 @Brief        : Contain Source Cif interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

 **************************************************************************************************/

#ifndef __PLG_SOURCE_CIF_API_H
#define __PLG_SOURCE_CIF_API_H
/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "TimeSyncMgr.h"
#include "PlgSourceApi.h"
#include "FrameMgrApi.h"
#include <string.h>
#include <DrvTimer.h>
#include "swcLeonUtils.h"
#include "Opipe.h"
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "swcWhoAmI.h"
#include "DrvCpr.h"
#include "DrvGpio.h"
#include "DrvMipi.h"
//#include "DrvDdr.h"
#include "DrvCif.h"
//#include "swcCrc.h"
#include "DrvMss.h"
#include "assert.h"
#include "ipipeDbg.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#define PLGCIF_GET_ID(X) ((X) == IC_CIF0_DEVICE4 ? CAMERA_1 : CAMERA_2)

/**************************************************************************************************
 ~~~ Imported functions declarations
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Basic typedefs
 **************************************************************************************************/
typedef struct {
    IcMipiRxCtrlRecNoT     id;
    uint32_t               BASE_ADR;
    uint32_t               INT_CLEAR_ADR;
    uint32_t               INT_ENABLE_ADR;
    uint32_t               irqSourceDefault;
    irq_handler            handler;
} RecCtrlT;


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
void PlgSourceCifCfg(icSourceConfig  *srcCfg, PlgSource *plug);
void IpipeMipiRxCifSof(uint32_t sourceD);
void IpipeMipiRxCifEof(uint32_t sourceD);
void IpipeMipiRxCifLineHit(uint32_t sourceD);
void PlgSourceCifStop( PlgSource *plug);
void producedFrameCifCall(PlgSource *plug);
#endif //__PLG_SOURCE_CIF_API_H
