///
/// @file DrvMss.h
/// 
/// 
/// @defgroup DrvMss Mss Driver
/// @{
/// @brief Mss Driver API.
///

#ifndef DRV_MSS_H
#define DRV_MSS_H

#include "DrvMssDefines.h"
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void DrvMssResetAllMipiConnections(void);
int  DrvMssConnectMipiToDevice(u32 MipiCtrlNo, drvMssDeviceType device);
void DrvMssConnectCif1ToGpio(void);
void DrvMssConnectLcdToGpio(void);


#ifdef __cplusplus
}
#endif

#endif



///@}
