#ifndef BRD_RTEMS_H_
#define BRD_RTEMS_H_

#include <rtems.h>

#include <FramePumpImx208.h>
#include <FramePumpOv7750.h>
#include <FramePumpOv9282.h>
#include <FramePumpImx214.h>

#define IN_FRAMES_QUEUE_SIZE    (4) //...but should be bigger, if memory space allows
#define OUT_FRAMES_QUEUE_SIZE   (4)

#define I2C_BUS_NAME "/dev/i2c"

rtems_status_code brdInitializeBoard(void);

#endif  //BRD_RTEMS_H_
