/**************************************************************************************************

 @File         : TimeSyncMgr.h
 @Author       : xx
 @Brief        : Contain function necesarry in order to obtain Us time, in synchronisation with Los
 Date          : 03 - June - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/

#ifndef __TIMESYNCMGR_H__
#define __TIMESYNCMGR_H__
#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************************************
 ~~~  Include standard types
 **************************************************************************************************/
#include "IcTypes.h"


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
//
icTimestamp TimeSyncMsgGetTimeUs(void);

#ifdef __cplusplus
}
#endif
#endif // __TIMESYNCMGR_H__
