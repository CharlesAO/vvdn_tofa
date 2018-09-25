#ifndef IMUFRAME_H__
#define IMUFRAME_H__

#include <stdint.h>

#include <OsImuHAL.h>

#define IMU_TOTAL_SIZE (96*1024)

#define IMU_PACKET_COUNT  ((IMU_TOTAL_SIZE - sizeof(uint32_t)) / sizeof(mvImuData_t))

#define IMU_PACKET_COUNT  ((IMU_TOTAL_SIZE - sizeof(uint32_t)) / sizeof(mvImuData_t))

typedef __attribute__((aligned(4))) struct imuFrame {
    uint32_t    count;
    mvImuData_t data[IMU_PACKET_COUNT];
    uint8_t padImu[IMU_TOTAL_SIZE -
                   sizeof(mvImuData_t) * IMU_PACKET_COUNT -
                   sizeof(uint32_t)];
} imuFrame;

#endif
