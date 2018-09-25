#ifndef VTRACK_PRIVATE_H
#define VTRACK_PRIVATE_H

#include "OsDrvCpr.h"
#ifndef VTRACK_DISABLE_SHAVE_PWR_MGT
#if defined(MA2150) || defined(MA2450)
#define localTurnOnShave(x)  OsDrvCprTurnOnShaveMask(1<<x)
#define localTurnOffShave(x) OsDrvCprTurnOffShaveMask(1<<x)
#else
#define localTurnOnShave(x) { int sc_turnOnShave = OsDrvCprTurnOnIsland((HglCprPwrIsland)(HGL_CPR_PWR_ISLAND_SHAVE_0 + x)); \
OsDrvCprDevice shave_dev = (OsDrvCprDevice) (OS_DRV_CPR_DEV_UPA_SHAVE_0 + x); \
OsDrvCprDeviceConfig devCfg = {shave_dev, OS_DRV_CPR_DEV_ENABLE}; \
sc_turnOnShave += (int)OsDrvCprSysDeviceAction(&devCfg); \
assert(RTEMS_SUCCESSFUL == (rtems_status_code) sc_turnOnShave); } 
#define localTurnOffShave(x) { rtems_status_code sc_turnOffShave = OsDrvCprTurnOffIsland((HglCprPwrIsland)(HGL_CPR_PWR_ISLAND_SHAVE_0 + x), 0); \
assert(RTEMS_SUCCESSFUL == sc_turnOffShave); }
#endif
#else
#define localTurnOnShave(x) ((void) (x))
#define localTurnOffShave(x) ((void) (x))
#endif

#endif
