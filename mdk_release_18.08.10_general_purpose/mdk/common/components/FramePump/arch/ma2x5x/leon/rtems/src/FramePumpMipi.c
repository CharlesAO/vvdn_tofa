///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include "FramePumpMipi.h"
#include "FramePumpDefines.h"

eDrvMipiInitFuncRet InitMIPI(eDrvMipiCtrlNo controllerNb,
                             const struct FramePumpSensorDriver *cameraSpec)
{
    eDrvMipiInitFuncRet retValue = DRV_MIPI_CFG_DONE;
    tDrvMipiControllerConfigStruct mipiCtrlCfg = {0};

    // Take MIPI interface out of reset.
    // DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_MIPI);

    mipiCtrlCfg.direction = MIPI_RX;
    mipiCtrlCfg.type = MIPI_CSI;
    mipiCtrlCfg.controllerNo = controllerNb;
    mipiCtrlCfg.rxPacket.dataMode = cameraSpec->spec.mipi.dataMode;
    mipiCtrlCfg.rxPacket.dataType = cameraSpec->spec.mipi.pixelFormat;
    mipiCtrlCfg.height = cameraSpec->height;
    mipiCtrlCfg.width = cameraSpec->width;
    mipiCtrlCfg.noLanes = cameraSpec->spec.mipi.nbOflanes;
    mipiCtrlCfg.laneRateMbps = cameraSpec->spec.mipi.dataRateMbps;

    retValue = DrvMipiInit(&mipiCtrlCfg);

    return retValue;
}
