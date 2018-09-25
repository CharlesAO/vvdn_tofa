#ifndef FRAMEPUMPMIPI_H__
#define FRAMEPUMPMIPI_H__

#ifndef MA2X8X
#include "DrvMipi.h"
#else
#include "OsDrvMipiDefines.h"
#endif

#include "FramePumpPrivate.h"

eDrvMipiInitFuncRet InitMIPI(eDrvMipiCtrlNo controllerNb,
                             const struct FramePumpSensorDriver *cameraSpec);

#endif
