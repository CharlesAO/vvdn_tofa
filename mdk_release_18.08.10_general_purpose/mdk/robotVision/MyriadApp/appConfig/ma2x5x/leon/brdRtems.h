#ifndef BRD_RTEMS_H_
#define BRD_RTEMS_H_

#include <rtems.h>

#define I2C_BUS_NAME "/dev/i2c"

#ifdef __cplusplus
extern "C"
#endif
rtems_status_code brdInitializeBoard(void);
#ifdef __cplusplus
extern "C"
#endif
rtems_status_code brdInitializeSensors(int bpp, int binning);

#endif
