///
/// @file DrvMipi.h
/// 
/// 
/// @defgroup DrvMipi Mipi Driver
/// @{
/// @brief Mipi Driver functionality.
///

#ifndef DRV_MIPI_H
#define DRV_MIPI_H

#include "mv_types.h"
#include "DrvMipiDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

eDrvMipiInitFuncRet DrvMipiInit(tPtrDrvMipiControllerConfigStruct mipiCtrlCfg);

void DrvMipiTestClear(u8 MipiCtrlNo);
void DrvMipiTestClearAll(void);
u8   DrvMipiTestModeSendCmd(u8 MipiCtrlNo, u8 Command, u8 *DataIn, u8 *DataOut, u8 Length);
u32  DrvMipiSetRxRange(u32 mipiPhyNo, u32 desiredClkKHz);
u32 DrvMipiRxClkTermCtrl(u32 mipiPhyNo, eDrvMipiTermResMode TermMode);
u32  DrvMipiPllProg(u32 mipiCtrlNo, u32 refClkKHz, u32 desiredClkKHz);
int  DrvMipiWaitPllLock(u32 mipiCtrlNo, u32 timeout);
u32  DrvMipiInitDphy(u32 mipiDphyNo);
void DrvMipiTxLPData(u32 mipiLaneNo, u32 data);
void DrvMipiTxHSData(u32 ctrlNo, u32 data);
void DrvMipiTxWaitForStopState(u32 phy);
u32  DrvMipiTxHSLPData(u32 ctrlNo,u32 header, u32 *data);
void DrvMipiStatusCheck(tPtrDrvMipiCallBackStruct fOverrideCalls);
#if (defined MYRIAD2)
void DrvResetMssGpioLcdMipiLoopbackCfgRegisterValues(void);
#endif
#ifdef __cplusplus
}
#endif


#endif



///@}
