#ifndef DRV_WM8325
#define DRV_WM8325

#include "DrvWm8325Defines.h"

typedef void (* tDrvPmicI2cWrite)(u32, u32);
typedef u32  (* tDrvPmicI2cRead)(u32);

#ifdef __cplusplus
extern "C" {
#endif

void DrvWm8325SetDcDcVoltage(u8 Dc , u32 voltageInMicroVolt, tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);
void DrvWm8325SetLdoVoltage (u8 Ldo, u32 voltageInMiliVolt , tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);
s32  DrvWm8325GetLdoVoltage (u8 Ldo, tDrvPmicI2cRead readFunc);
void DrvWm8325SetGpioMode(u8 gpio, u16 value,  tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);
void DrvWm8325SetGpioLevel(u8 gpio, u8 level,  tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);
u8 DrvWm8325GetGpioLevel(u8 gpio, tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);
float DrvWm8325GetPmicTemp(tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc);

#ifdef __cplusplus
}
#endif

#endif
