/**************************************************************************************************

 @File         : TimeSyncMgr.c
 @Author       : xx
 @Brief        : Contain function necessary in order to obtain Us time, in synchronisation with Los
 Date          : 03 - June - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
***************************************************************************************************/
#include "TimeSyncMgr.h"
#if defined(__sparc)
#include "swcLeonMath.h"
#include "DrvTimer.h"
#else
#endif
#include "ipipe.h"

extern icCtrl* pSrvIcCtrl;

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/

icTimestamp TimeSyncMsgGetTimeUs(void) {
#if defined(__sparc)
    icTimestamp ticks;
    DrvTimerGetSystemTicks64(&ticks);
    ticks = (ticks-pSrvIcCtrl->ticksDelay)/pSrvIcCtrl->sysClkPerUs;
    return (pSrvIcCtrl->curTime + ticks);
#else
    return 0; // NOT implemented
#endif
}

