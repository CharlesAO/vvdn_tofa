#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>
#include "OsCommon.h"
#include "OsCross.h"
#include <DrvTimer.h>
#include <DrvCpr.h>
#include <OsDrvCpr.h>

#include <OsDrvGpio.h>
#include <OsBmx055.h>
#include <OsImuHAL.h>
#include <bmm055.h>

#include <math.h>

#if 0
#define BMXPRINTK(...) printf(__VA_ARGS__)
#else
#define BMXPRINTK(...)
#endif

/// IMU Number of Mag and Baro reads
#define IMU_BMX055_ONE_MAG_IN 4
#define IMU_BMP180_ONE_BAR_IN 5

/// IMU BMX055 Accel registers address
#define IMU_BMX055_ACC_X_LSB 0x02
#define IMU_BMX055_ACC_Y_LSB 0x04
#define IMU_BMX055_ACC_Z_LSB 0x06

#define IMU_BMX055_ACC_PMU_BW 0x10
#define IMU_BMX055_ACC_HBW 0x13
#define IMU_BMX055_ACC_INT_EN_1 0x17
#define IMU_BMX055_ACC_INT_MAP_1 0x1A
#define IMU_BMX055_ACC_PMU_RANGE 0x0F
#define IMU_BMX055_ACC_INT0 0x22

/// IMU BMX055 Gyro registers address
#define IMU_BMX055_GYRO_RATE_X_LSB 0x02
#define IMU_BMX055_GYRO_RATE_Y_LSB 0x04
#define IMU_BMX055_GYRO_RATE_Z_LSB 0x06

#define IMU_BMX055_GYRO_RANGE 0x0F
#define IMU_BMX055_GYRO_BW 0x10
#define IMU_BMX055_GYRO_RATE_HBW 0x13
#define IMU_BMX055_GYRO_INT_EN_0 0x15
#define IMU_BMX055_GYRO_INT_EN_1 0x16
#define IMU_BMX055_GYRO_INT_MAP_1 0x18
#define IMU_BMX055_GYRO_BIST 0x3C /// Built in Self-Test register
#define IMU_BMX055_GYRO_HIGH_TH_Z 0x26

/// IMU BMX055 Magneto registers address
#define IMU_BMX055_MAG_X_LSB 0x42
#define IMU_BMX055_MAG_HALL_LSB 0x48
#define IMU_BMX055_MAG_OPMODE 0x4C
#define IMU_BMX055_MAG_INT_EN 0x4E

/// IMU BMX055 Accel register values
#define IMU_BMX055_ACC_BGW_CHIPID_VAL 0xFA
#define IMU_BMX055_ACC_DATA_RDY_INT (1<<4)
#define IMU_BMX055_ACC_MAP_TO_INT1 0x1
#define IMU_BMX055_ACC_INT0_DEF_VAL 0x9
#define IMU_BMX055_ACC_INT1_DEF_VAL 0x30

/// IMU BMX055 Accel range
#define IMU_BMX055_ACC_RANGE2G 0x3
#define IMU_BMX055_ACC_RANGE4G 0x5
#define IMU_BMX055_ACC_RANGE8G 0x8
#define IMU_BMX055_ACC_RANGE16G 0xC

/// IMU BMX055 Accel data filter bandwidth (sample rate = 2*bandwidth
#define IMU_BMX055_ACC_PMU_BW_8HZ 0x8
#define IMU_BMX055_ACC_PMU_BW_16HZ 0x9
#define IMU_BMX055_ACC_PMU_BW_32HZ 0xA
#define IMU_BMX055_ACC_PMU_BW_125HZ 0xC
#define IMU_BMX055_ACC_PMU_BW_250HZ 0xD
#define IMU_BMX055_ACC_PMU_BW_500HZ 0xE
#define IMU_BMX055_ACC_PMU_BW_1000HZ 0xF

/// IMU BMX055 Gyro register values
#define IMU_BMX055_GYRO_DATA_RDY_INT (1<<7)
#define IMU_BMX055_GYRO_HIGH_TH_Z_DEF 0x2
#define IMU_BMX055_GYRO_HIGH_DUR_Z_DEF 0x19
#define IMU_BMX055_GYRO_BIST_RATE_OK 0x10
#define IMU_BMX055_GYRO_BIST_TRIG_MASK 0x1
#define IMU_BMX055_GYRO_BIST_STAT_MASK 0x6
#define IMU_BMX055_GYRO_BIST_PASS 0x2
#define IMU_BMX055_GYRO_INT3_LVL_H 0x1
#define IMU_BMX055_GYRO_INT3_MAP_DATA_RDY 0x1

/// IMU BMX055 Magneto register values
#define IMU_BMX055_MAG_CHIP_ID_VAL 0x32
#define IMU_BMX055_MAG_RATE2 0x1
#define IMU_BMX055_MAG_RATE6 0x2
#define IMU_BMX055_MAG_RATE8 0x3
#define IMU_BMX055_MAG_RATE10 0x0
#define IMU_BMX055_MAG_RATE15 0x4
#define IMU_BMX055_MAG_RATE20 0x5
#define IMU_BMX055_MAG_RATE25 0x6
#define IMU_BMX055_MAG_RATE30 0x7

#define IMU_BMX055_MAG_INT_SET_DEF 0x3F
#define IMU_BMX055_MAG_INT_CTRL_DEF 0x7
#define IMU_BMX055_MAG_SLEEP_MODE 0x6
#define IMU_BMX055_MAG_NORMAL_SELF_TEST_BIT 0x1
#define IMU_BMX055_MAG_SELF_TEST_PASS 0x3



/// IMU BMP180 Baro registers
#define IMU_BMP180_BARO_SOFT_RESET 0xE0
#define IMU_BMP180_BARO_ADC_OUT_LSB 0xF7

/// BMP packet type
#define IMU_BMP180_BARO_PACKET 0
#define IMU_BMP180_TEMP_PACKET 1

/// IMU BMP180 Baro expected values
#define IMU_BMP180_BARO_SOFT_RESET_VALUE 0xB6
#define IMU_BMP180_BARO_OUT_MSB 0x80

#define IMU_BMP180_TEMP_MEASURE_VAL 0x2E /// temperature measurent
#define IMU_BMP180_PROM_START_ADDR 0xaa
#define IMU_BMP180_PROM_DATA_LEN 22

#define IMU_BMP180_BARO_PARAM_MG 3038 /// calibration parameter
#define IMU_BMP180_BARO_PARAM_MH -7357 /// calibration parameter
#define IMU_BMP180_BARO_PARAM_MI 3791 /// calibration parameter


/// IMU BMX055 and BMP180 Valid flags
#define IMU_BMX055_ACC_VALID 0x0001
#define IMU_BMX055_GYRO_VALID 0x0002
#define IMU_BMX055_MAG_VALID 0x0004
#define IMU_BMP180_BARO_VALID 0x0008
#define IMU_BMP180_TEMP_VALID 0x0010

#define IMU_BMP180_BARO_SLAVE_ADR 0x77

/// Gyro range
#define IMU_BMX055_GYRO_RANGE2000 0x0
#define IMU_BMX055_GYRO_RANGE1000 0x1
#define IMU_BMX055_GYRO_RANGE500 0x2
#define IMU_BMX055_GYRO_RANGE250 0x3
#define IMU_BMX055_GYRO_RANGE125 0x4

/// Gyro rate and filter bandwidth
#define IMU_BMX055_GYRO_100HZ_BW_32HZ 0x7
#define IMU_BMX055_GYRO_200HZ_BW_64HZ 0x6
#define IMU_BMX055_GYRO_400HZ_BW_47HZ 0x3
#define IMU_BMX055_GYRO_1000HZ_BW_116HZ 0x2
#define IMU_BMX055_GYRO_2000HZ_BW_230HZ 0x1

/// Accelerometer Config registers
#define IMU_BMX055_ACC_REG_PMU_LPW_ADDR 0x11
#define IMU_BMX055_ACC_REG_BGW_SOFT_RESET_ADDR 0x14

#define IMU_BMX055_ACC_DEEP_SUSPEND_BIT_OFFSET 5
#define IMU_BMX055_ACC_PMU_BW_62HZ 0x0B
#define IMU_BMX055_ACC_STARTUP_TIME_US 1300

/// Gyro Config registers
#define IMU_BMX055_GYRO_REG_LPM1_ADDR 0x11
#define IMU_BMX055_GYRO_REG_BGW_SOFTRESET_ADDR 0x14

#define IMU_BMX055_GYRO_DEEP_SUSPEND_BIT_OFFSET 5
#define IMU_BMX055_GYRO_STARTUP_TIME_US 30000
/// Magnetometer Config registers
#define IMU_BMX055_MAG_STARTUP_TIME_US 3000
#define IMU_BMX055_MAG_POWER_CONTROL_OFFSET 0

#define IMU_BMX055_ACC_REG_VALUE_SOFT_RESET 0xB6
#define IMU_BMX055_ACC_BGW_CHIP_ID_VAL 0xFA
#define IMU_BMX055_GYRO_CHIP_ID_VAL 0x0F

/// IMU BMP180 pressure val
#define IMU_BMP180_PRESS_MEASURE_VAL 0x34
/// IMU BMP180 baro ID val
#define IMU_BMP180_BARO_ID_VALUE 0x55
#define IMU_BMP180_BARO_ADC_OUT_MSB 0xF6
#define IMU_BMP180_BARO_CTRL_MEAS 0xF4
#define IMU_BMP180_BARO_ID 0xD0


#define IMU_BMX055_ACC_BGW_CHIP_ID 0x00
#define IMU_BMX055_GYRO_CHIP_ID 0x00
#define IMU_BMX055_MAG_POWER_RESET 0x4B
#define IMU_BMX055_MAG_CHIP_ID 0x40
#define IMU_BMX055_MAG_INT_SET 0x4D
#define IMU_BMX055_MAG_STARTUP_TIME_US 3000
#define IMU_BMX055_MAG_RESET_TIME_US 1000



/// This structure holds all device specific calibration parameters
typedef struct imuBmp180CalibrationParam_t
{
    s16 ac1;
    s16 ac2;
    s16 ac3;
    u16 ac4;
    u16 ac5;
    u16 ac6;
    s16 b1;
    s16 b2;
    s16 mb;
    s16 mc;
    s16 md;
} imuBmp180CalibrationParam_t;

/// BMP180 image registers data structure
typedef struct imuBmp180_t
{
    struct imuBmp180CalibrationParam_t calibrationParam;
    u8 mode;
    u8 chipId, mlVersion, alVersion;
    u8 deviceAddr;
    u8 sensorType;

    s32 paramB5;
    s32 numberOfSamples;
    s16 oversamplingSetting;
    s16 swOss;
} imuBmp180_t;

typedef enum bmxbmpDirection {
    BMX_BMP_WRITE = 0, // this has to match the values of i2c read/write addr
    BMX_BMP_READ = 1,
} bmxbmpDirection;


#define ONE_G ((float) 9.80665 ) // see: https://en.wikipedia.org/wiki/G-force
#define DEFAULT_ACCEL_RANGE 4.0
static float accelRange = DEFAULT_ACCEL_RANGE;
static float convertAccelToSI(int32_t rawVal)
{
    float r;
    r = (float)rawVal;
    r /= (float)0x7FFF;
    r *= (float)accelRange;
    r *= (float) ONE_G;
    return r;
}

#define RAD_PER_DEG ( M_PI / 180.0f )
#define DEFAULT_GYRO_RANGE 2000.0
static float gyroRange = DEFAULT_GYRO_RANGE;
static float convertGyroToSI(int32_t rawVal)
{
    float r;
    r = (float)rawVal;
    r *= gyroRange;
    r /= (float)0x7FFF;
    r *= RAD_PER_DEG;
    return r;
}
static float convertTempToSI(int32_t rawVal)
{
    float r = 296.15; // 23 degree Celsius
    return r + 0.5 * rawVal;
}

static u64 convertTimestampClksToNs(u64 timestampClks)
{
    u64 timestampNs;
    u32 clkPerUs;
    int rval;

    rval = OsDrvCprGetSysClockPerUs(&clkPerUs);
    if(rval) return (u64)-1;

    timestampNs = timestampClks * 1000 / clkPerUs;

    return timestampNs;
}

static rtems_status_code bmxbmpFullReadWrite(rtems_device_minor_number minor,
                                             u8 regAddr, void *data, u32 size,
                                             bmxbmpDirection rw)
{
    rtems_status_code sc;

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_send_start failed %d\n", sc);
        return -sc;
    }
    sc = rtems_libi2c_send_addr(minor, (int)0);
    if (sc != RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_send_start 1 failed %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_write_bytes(minor, (u8 *)&regAddr, 1);
    if (sc < RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_addr(minor, (int)rw);
    if (sc != RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_send_start failed %d\n", sc);
        return -sc;
    }

    switch(rw)
    {
    case BMX_BMP_READ:
        sc = rtems_libi2c_read_bytes(minor, (u8 *) data, size);
        break;
    case BMX_BMP_WRITE:
        sc = rtems_libi2c_write_bytes(minor, (u8 *) data, size);
        break;
    default:
        return RTEMS_INVALID_ID;
    }
    if (sc < RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_read/write failed, sc = %d\n", sc);
        return -sc;
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        BMXPRINTK("rtems_libi2c_send_stop failed, sc = %d\n", sc);
        return -sc;
    }

    return -sc;
}
static rtems_status_code bmxbmpFullRead(rtems_device_minor_number minor,
                                             u8 regAddr, void *data, u32 size){
    return bmxbmpFullReadWrite(minor, regAddr, data, size, BMX_BMP_READ);
}

static rtems_status_code bmx055AccelInit(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 data[2];

    // Probe bmx055 id
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_BGW_CHIP_ID, &data[0], 1,
                             BMX_BMP_READ);
    if(sc)
        return sc;

    if (IMU_BMX055_ACC_BGW_CHIP_ID_VAL != data[0])
    {
        BMXPRINTK("BMX055 expected 0x%02X | got 0x%02X \n",
               IMU_BMX055_ACC_BGW_CHIP_ID_VAL, data[0]);
        return RTEMS_INVALID_ID;
    }

    return RTEMS_SUCCESSFUL;
}

volatile u64 bmx055AccelTimestamp;

static void bmx055AccelHandler(void * context)
{
    rtems_id accelTaskId = (rtems_id)context;
    DrvTimerGetSystemTicks64((u64*)&bmx055AccelTimestamp);
    rtems_event_send(accelTaskId, RTEMS_EVENT_0);
}

rtems_status_code bmxSetGpioIrq(int gpio, int priority,
                                void (*handler)(void * context),
                                void *context)
{
    rtems_status_code sc;
    gpioInterruptHndl_t gpioIrqHndl;
    rtems_libio_ioctl_args_t gpioIrqCtl;
    struct stat st;

    gpioIrqHndl.priority = priority;
    gpioIrqHndl.type = POS_EDGE_INT;
    gpioIrqHndl.handler = handler;
    gpioIrqHndl.context = context;

    gpioIrqCtl.command = GPIO_IRQ_ENABLE;
    gpioIrqCtl.buffer = (gpioInterruptHndl_t *)&gpioIrqHndl;

    sc = stat( "/dev/gpioblock" , &st );
    if(sc) return sc;

    sc = rtems_io_open(rtems_filesystem_dev_major_t( st.st_rdev ),
                       gpio, NULL);
    if(sc) return sc;

    sc = rtems_io_control(rtems_filesystem_dev_major_t( st.st_rdev ),
                          gpio, &gpioIrqCtl);

    return sc;
}

static rtems_status_code bmx055AccelOpen(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major); // hush the compiler warning.
    UNUSED(arg); // hush the compiler warning.

//    bmxAccelOpenConfiguration_t *c = (bmxAccelOpenConfiguration_t *)arg;
    rtems_status_code sc;
    u8 data[2];

    // Reset accelerometer
    data[0] = IMU_BMX055_ACC_REG_VALUE_SOFT_RESET;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_REG_BGW_SOFT_RESET_ADDR,
                             &data[0], 1, BMX_BMP_WRITE);
    if(sc) return sc;

    // TODO: Proper sleep time
    rtems_task_wake_after(IMU_BMX055_ACC_STARTUP_TIME_US/1000 + 1);

    data[0] = IMU_BMX055_ACC_SAMPLERATE_500HZ;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_PMU_BW, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    accelRange = DEFAULT_ACCEL_RANGE;
    data[0] = IMU_BMX055_ACC_RANGE2G;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_PMU_RANGE, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = 0;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_HBW, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = IMU_BMX055_ACC_DATA_RDY_INT;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_INT_EN_1, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = IMU_BMX055_ACC_MAP_TO_INT1;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_INT_MAP_1, &data[0], 1,
                             BMX_BMP_WRITE);
    return sc;
}

static rtems_status_code bmx055AccelClose(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_status_code bmx055AccelRead(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    rtems_libio_rw_args_t *rwargs = arg;
    mvImuData_t *data;
    rtems_event_set ev;

    if(rwargs == NULL || rwargs->buffer == NULL)
        return RTEMS_INVALID_ADDRESS;

    if(rwargs->count < sizeof(mvImuData_t) * 2)
        return RTEMS_IO_ERROR;

    data = (mvImuData_t *)rwargs->buffer;

    // Wait for data available IRQ
    sc = rtems_event_receive(RTEMS_EVENT_0,
                             RTEMS_EVENT_ANY,
                             100,
                             &ev);
    if (sc) return sc;

    if((ev & RTEMS_EVENT_0) == 0)
        return RTEMS_INVALID_ID;

    data[0].type = IMU_TYPE_ACCELEROMETER;
    // Set the last timestamp
    data[0].timestampNs = convertTimestampClksToNs(bmx055AccelTimestamp);
    data[1].type = IMU_TYPE_TEMPERATURE;
    data[1].timestampNs =  data[0].timestampNs;

    // Read all the accelerometer data (contiguous registers)
    int16_t tmp[4];
    tmp[3] = 0;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_X_LSB, &tmp[0],
                        sizeof(int16_t) * 3 + 1, BMX_BMP_READ);
    data[0].rawAxisData.x = (int32_t)tmp[0];
    data[0].rawAxisData.y = (int32_t)tmp[1];
    data[0].rawAxisData.z = (int32_t)tmp[2];

    data[0].axisData.x = convertAccelToSI(data->rawAxisData.x);
    data[0].axisData.y = convertAccelToSI(data->rawAxisData.y);
    data[0].axisData.z = convertAccelToSI(data->rawAxisData.z);
    data[1].temperatureData.t = convertTempToSI(tmp[3]);
    if(sc) return sc;
    rwargs->bytes_moved = sizeof(mvImuData_t) * 2;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code bmx055AccelWrite(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

rtems_status_code bmx055AccelControl(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }
    bmxAccelControl_t *c = (bmxAccelControl_t *)ctl->buffer;
    u8 data[2];
    switch (ctl->command)
    {
    case BMX_INT_GPIO_SET:
        sc = bmxSetGpioIrq(c->bmxIrqGpio,c->bmxIrqPriority,
                           bmx055AccelHandler, (void *)rtems_task_self());
        ctl->ioctl_return = sc;
        break;
    case BMX_SAMPLERATE_SET:
        data[0] = c->sampleRate;
        sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_PMU_BW, &data[0], 1,
                                 BMX_BMP_WRITE);
        ctl->ioctl_return = sc;
        break;
    case BMX_RANGE_SET:
        switch (c->range)
        {
        case IMU_BMX055_ACC_RANGE2G:
            accelRange = 2.0f;
            break;
        case IMU_BMX055_ACC_RANGE4G:
            accelRange = 4.0f;
            break;
        case IMU_BMX055_ACC_RANGE8G:
            accelRange = 8.0f;
            break;
        case IMU_BMX055_ACC_RANGE16G:
            accelRange = 16.0f;
            break;
        default:
            return RTEMS_NOT_DEFINED;
        }
        data[0] = c->range;
        sc = bmxbmpFullReadWrite(minor, IMU_BMX055_ACC_PMU_RANGE, &data[0], 1,
                                 BMX_BMP_WRITE);
        ctl->ioctl_return = sc;
        break;
    default:
        return RTEMS_NOT_DEFINED;
    }

    return sc;
}

static const rtems_driver_address_table bmxAccelProtocolAdrTbl = {
    .initialization_entry = bmx055AccelInit,
    .open_entry = bmx055AccelOpen,
    .close_entry = bmx055AccelClose,
    .read_entry = bmx055AccelRead,
    .write_entry = bmx055AccelWrite,
    .control_entry = bmx055AccelControl,
};

const rtems_libi2c_drv_t bmxAccelProtocolDrvTbl = {
    .ops = &bmxAccelProtocolAdrTbl,
    .size = sizeof(bmxAccelProtocolDrvTbl),
};

static rtems_status_code bmx055GyroInit(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 data[2];

    // Probe bmx055 gyro id
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_CHIP_ID, &data[0], 1, BMX_BMP_READ);
    if(sc) return sc;

    if(IMU_BMX055_GYRO_CHIP_ID_VAL != data[0])
    {
        BMXPRINTK("BMX055 gyro expected 0x%02X | got 0x%02X \n",
               IMU_BMX055_GYRO_CHIP_ID_VAL, data[0]);
        return RTEMS_INVALID_ID;
    }

    return RTEMS_SUCCESSFUL;
}

volatile u64 bmx055GyroTimestamp;

static void bmx055GyroHandler(void * context)
{
    rtems_id gyroTaskId = (rtems_id)context;
    DrvTimerGetSystemTicks64((u64*)&bmx055GyroTimestamp);
    rtems_event_send(gyroTaskId, RTEMS_EVENT_1);
}


static rtems_status_code bmx055GyroOpen(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 data[2];

    // Leave deep suspend mode
    data[0] = 0;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_REG_LPM1_ADDR, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    // TODO: Proper sleep time
    rtems_task_wake_after(IMU_BMX055_GYRO_STARTUP_TIME_US/1000 + 1);

    gyroRange = DEFAULT_GYRO_RANGE;
    data[0] = IMU_BMX055_GYRO_RANGE2000;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_RANGE, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] =IMU_BMX055_GYRO_400HZ_BW_47HZ;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_BW,
                             &data[0], 1, BMX_BMP_WRITE);
    if(sc) return sc;

    // Setting filtered mode and shadowing
    data[0] = 0;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_RATE_HBW,
                             &data[0], 1, BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = IMU_BMX055_GYRO_DATA_RDY_INT;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_INT_EN_0,
                             &data[0], 1, BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = IMU_BMX055_GYRO_INT3_LVL_H;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_INT_EN_1, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    data[0] = IMU_BMX055_GYRO_INT3_MAP_DATA_RDY;
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_INT_MAP_1, &data[0], 1,
                             BMX_BMP_WRITE);
    return sc;
}


static rtems_status_code bmx055GyroClose(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_status_code bmx055GyroRead(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_status_code sc;
    rtems_libio_rw_args_t *rwargs = arg;
    mvImuData_t *data;
    rtems_event_set ev;

    if(rwargs == NULL || rwargs->buffer == NULL)
        return RTEMS_INVALID_ADDRESS;

    if(rwargs->count < sizeof(mvImuData_t))
        return RTEMS_IO_ERROR;

    data = (mvImuData_t *)rwargs->buffer;

    // Wait for data available IRQ
    sc = rtems_event_receive(RTEMS_EVENT_1,
                             RTEMS_EVENT_ANY,
                             100,
                             &ev);
    if (sc) return sc;

    if((ev & RTEMS_EVENT_1) == 0)
        return RTEMS_INVALID_ID;


    data->type = IMU_TYPE_GYROSCOPE;
    // Set the last timestamp
    data->timestampNs = convertTimestampClksToNs(bmx055GyroTimestamp);

    // Read all the accelerometer data (contiguous registers)
    int16_t tmp[3];
    bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_RATE_X_LSB, &tmp[0],
                        sizeof(int16_t) * 3, BMX_BMP_READ);
    data->rawAxisData.x = (int32_t)tmp[0];
    data->rawAxisData.y = (int32_t)tmp[1];
    data->rawAxisData.z = (int32_t)tmp[2];
    data->axisData.x = convertGyroToSI(data->rawAxisData.x);
    data->axisData.y = convertGyroToSI(data->rawAxisData.y);
    data->axisData.z = convertGyroToSI(data->rawAxisData.z);

    rwargs->bytes_moved = sizeof(mvImuData_t);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code bmx055GyroWrite(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);

    // TODO: Implement
    return RTEMS_NOT_IMPLEMENTED;
}

rtems_status_code bmx055GyroControl(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    void *arg)
{
    UNUSED(major);// hush the compiler warning.

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }
    bmxGyroControl_t *c = (bmxGyroControl_t *)ctl->buffer;

    u8 data[2];
    switch (ctl->command)
    {
    case BMX_INT_GPIO_SET:
        sc = bmxSetGpioIrq(c->bmxIrqGpio,c->bmxIrqPriority,
                           bmx055GyroHandler, (void *)rtems_task_self());
        ctl->ioctl_return = sc;
        break;
    case BMX_SAMPLERATE_SET:
        data[0] = c->sampleRate;
        sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_BW,
                                 &data[0], 1, BMX_BMP_WRITE);
        ctl->ioctl_return = sc;
        break;
    case BMX_RANGE_SET:
        switch (c->range)
        {
        case IMU_BMX055_GYRO_RANGE2000:
            gyroRange = 2000.0f;
            break;
        case IMU_BMX055_GYRO_RANGE1000:
            gyroRange = 1000.0f;
            break;
        case IMU_BMX055_GYRO_RANGE500:
            gyroRange = 500.0f;
            break;
        case IMU_BMX055_GYRO_RANGE250:
            gyroRange = 250.0f;
            break;
        case IMU_BMX055_GYRO_RANGE125:
            gyroRange = 125.0f;
            break;
        default:
            return RTEMS_NOT_DEFINED;
        }
        data[0] = c->range;
        sc = bmxbmpFullReadWrite(minor, IMU_BMX055_GYRO_RANGE, &data[0], 1,
                                 BMX_BMP_WRITE);
        ctl->ioctl_return = sc;
        break;
    default:
        return RTEMS_NOT_DEFINED;
    }
    return sc;
}

static const rtems_driver_address_table bmxGyroProtocolAdrTbl = {
    .initialization_entry = bmx055GyroInit,
    .open_entry = bmx055GyroOpen,
    .close_entry = bmx055GyroClose,
    .read_entry = bmx055GyroRead,
    .write_entry = bmx055GyroWrite,
    .control_entry = bmx055GyroControl,
};

const rtems_libi2c_drv_t bmxGyroProtocolDrvTbl = {
    .ops = &bmxGyroProtocolAdrTbl,
    .size = sizeof(bmxGyroProtocolDrvTbl),
};


static trimRegs_t trimRegs;
static bmxMagAccuracy_t currentMagAccuracy;
static rtems_id magTimer;
static bmxMagSampleRate_t currentMagRate;
// number of measurement repetitions for each preset. Numbers are defined in the datasheet.
// Note that these numbers are not REG_VALUES which are equal to repetition count -1.
static const int repXYPerAccuracy[IMU_BMX055_MAG_ACC_LAST] = {
    3,
    9,
    15,
    47
};
static const int repZPerAccuracy[IMU_BMX055_MAG_ACC_LAST] = {
    3,
    15,
    27,
    83
};
static void bmx055MagHandler(rtems_id id, void * context)
{
    UNUSED(id);
    rtems_timer_reset(magTimer);
    rtems_id taskId = (rtems_id)context;
    rtems_event_send(taskId, RTEMS_EVENT_1);
}
static rtems_status_code bmx055MagInit(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.
    rtems_status_code sc;
    u8 data[2];
    data[0] = (1 << 7) | (1 << 1); // software reset
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_POWER_RESET, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc)
        return sc;
    usleep(IMU_BMX055_MAG_RESET_TIME_US); //TODO: check exact timing
    data[0] = 1; // power control bit -> entering sleep mode
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_POWER_RESET, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc)
        return sc;
    usleep(IMU_BMX055_MAG_STARTUP_TIME_US);
    // Probe bmx055 id
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_CHIP_ID, &data[0], 1,
                             BMX_BMP_READ);
    if(sc)
        return sc;
    if (IMU_BMX055_MAG_CHIP_ID_VAL != data[0])
    {
        BMXPRINTK("BMX055 expected 0x%02X | got 0x%02X \n",
                  IMU_BMX055_MAG_CHIP_ID_VAL, data[0]);
        return RTEMS_INVALID_ID;
    }

    return RTEMS_SUCCESSFUL;
}
static int getRepRegValue(uint32_t rep)
{
    return rep - 1;
}
static rtems_status_code bmx055SetAccuracy(bmxMagAccuracy_t accuracy,  rtems_device_minor_number minor)
{
    rtems_status_code sc;

    char data[2];
    currentMagAccuracy = accuracy;
    data[0] = getRepRegValue(repXYPerAccuracy[currentMagAccuracy]);
    sc = bmxbmpFullReadWrite(minor, 0x51, &data[0], 1,
                             BMX_BMP_WRITE);
    data[0] = getRepRegValue(repZPerAccuracy[currentMagAccuracy]);
    sc = bmxbmpFullReadWrite(minor, 0x52, &data[0], 1,
                             BMX_BMP_WRITE);
    return sc;
}
static bmxMagSampleRate_t getMaxSampleRateForMode(bmxMagAccuracy_t mode)
{
    bmxMagSampleRate_t ret = IMU_BMX055_MAG_RATE_HZ_MIN;
    switch (mode){
    case IMU_BMX055_MAG_ACC_LOW_POWER:
        ret =  IMU_BMX055_MAG_RATE_HZ_MAX_LOW_POWER;
        break;
    case IMU_BMX055_MAG_ACC_REGULAR:
        ret = IMU_BMX055_MAG_RATE_HZ_MAX_REGULAR;
            break;
    case IMU_BMX055_MAG_ACC_ENHANCED:
        ret = IMU_BMX055_MAG_RATE_HZ_MAX_ENHANCED_REGULAR;
            break;
    case IMU_BMX055_MAG_ACC_HIGH:
        ret = IMU_BMX055_MAG_RATE_HZ_MAX_HIGH_ACCURACY;
            break;
    default:
        break;
    }
    return ret;
}
static int getMagIntegrationTimeUs(void)
{
    // from BMX055 datasheet
    return 145 * repXYPerAccuracy[currentMagAccuracy] + 500 * repZPerAccuracy[currentMagAccuracy] + 980;
}

static rtems_status_code bmx055MagOpen(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code sc;
    u8 data[2];
    // sleep mode and set rate
    // we will use magneto in forced mode, as we don't have the Data ready pin connected to myriad by default.
    // This means that we will leave sleep mode each time we want to take a measurement.
    data[0] = (11 << 1);
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_OPMODE, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;
    sc = bmm050_init_trim_registers(minor, &trimRegs, bmxbmpFullRead);
    if(sc) return sc;

    //default accuracy
    sc = bmx055SetAccuracy(IMU_BMX055_MAG_ACC_REGULAR, minor);
    if(sc) return sc;

    sc = rtems_timer_create(rtems_build_name('T','M','A','G'), &magTimer);
    if(sc) return sc;
    currentMagRate = IMU_BMX055_MAG_RATE_HZ_MIN; //default rate;
    // As we are using forced mode, dataRate will be controlled by a timer
    rtems_timer_fire_after(magTimer, rtems_clock_get_ticks_per_second() / currentMagRate,
                           bmx055MagHandler, (void*)rtems_task_self());
    return sc;
}
static rtems_status_code bmx055MagClose(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);
    return RTEMS_NOT_IMPLEMENTED;
}
rtems_status_code bmx055MagControl(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    void *arg)
{
    UNUSED(major);

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }
    bmxMagControl_t *c = (bmxMagControl_t *)ctl->buffer;

    switch (ctl->command)
    {
    case BMX_INT_GPIO_SET:
        // because of lacking DRDY connection, we use the mag without interrupts
        ctl->ioctl_return = RTEMS_NOT_IMPLEMENTED;
        return RTEMS_NOT_IMPLEMENTED;
        break;
    case BMX_SAMPLERATE_SET:
        if (c->sampleRate > getMaxSampleRateForMode(currentMagAccuracy))
        {
            return RTEMS_INVALID_NUMBER;
        }
        currentMagRate = c->sampleRate;
        sc = rtems_timer_cancel(magTimer);
        sc += rtems_timer_fire_after(magTimer,
                                     rtems_clock_get_ticks_per_second() / c->sampleRate,
                                     bmx055MagHandler, (void*)rtems_task_self());
        ctl->ioctl_return = sc;
        break;
    case BMX_RANGE_SET:
        if (c->accuracy >= IMU_BMX055_MAG_ACC_LAST)
        {
            return RTEMS_INVALID_NUMBER;
        }
        if (c->accuracy > currentMagAccuracy)
        {
            // trying to configure bigger accuracy than previous. Checking if the current rate allows this accuracy
            if (currentMagRate > getMaxSampleRateForMode(c->accuracy)){
                return RTEMS_INVALID_NUMBER;
            }
        }
        sc = bmx055SetAccuracy(c->accuracy, minor);
        ctl->ioctl_return = sc;
        break;
    default:
        return RTEMS_NOT_DEFINED;
    }
    return sc;
}
static rtems_status_code bmx055MagWrite(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *arg)
{
    UNUSED(major);
    UNUSED(minor);
    UNUSED(arg);
    return RTEMS_NOT_IMPLEMENTED;
}

static rtems_status_code bmx055MagRead(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *arg)
{
    UNUSED(major);
    u8 data[2];
    rtems_status_code sc;
    rtems_libio_rw_args_t *rwargs = arg;
    mvImuData_t *dataImu;
    rtems_event_set ev;

    if(rwargs == NULL || rwargs->buffer == NULL)
        return RTEMS_INVALID_ADDRESS;

    if(rwargs->count < sizeof(mvImuData_t))
        return RTEMS_IO_ERROR;

    dataImu = (mvImuData_t *)rwargs->buffer;
    // Wait for data available IRQ
    sc = rtems_event_receive(RTEMS_EVENT_1,
                             RTEMS_EVENT_ANY,
                             rtems_clock_get_ticks_per_second() / IMU_BMX055_MAG_RATE_HZ_MIN,
                             &ev);

    if (sc){
        return sc;
    }

    if((ev & RTEMS_EVENT_1) == 0)
        return RTEMS_INVALID_ID;

    dataImu->type = IMU_TYPE_MAGNETOMETER;

    //trigger force mode
    data[0] = (1 << 1);
    sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_OPMODE, &data[0], 1,
                             BMX_BMP_WRITE);
    if(sc) return sc;

    u64 timestamp;
    DrvTimerGetSystemTicks64(&timestamp);

    dataImu->timestampNs = convertTimestampClksToNs(timestamp);
    timestamp += getMagIntegrationTimeUs() * 1000;
    usleep(getMagIntegrationTimeUs());
    while(1)
    {
        sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_HALL_LSB, &data[0], 8,
                                 BMX_BMP_READ);
        if(sc) return sc;
        if (data[0] & 1)
        {
            int16_t tmp[4];
            //read data
            sc = bmxbmpFullReadWrite(minor, IMU_BMX055_MAG_X_LSB, &tmp[0], sizeof(int16_t) * 4,
                                     BMX_BMP_READ);
            dataImu->rawAxisData.x = (int32_t)(tmp[0] / 8 );
            dataImu->rawAxisData.y = (int32_t)(tmp[1] / 8 );
            dataImu->rawAxisData.z = (int32_t)(tmp[2] / 2 );
            dataImu->axisData.x = bmm050_compensate_X_float(dataImu->rawAxisData.x, tmp[3], &trimRegs);
            dataImu->axisData.y = bmm050_compensate_Y_float(dataImu->rawAxisData.y, tmp[3], &trimRegs);
            dataImu->axisData.z = bmm050_compensate_Z_float(dataImu->rawAxisData.z, tmp[3], &trimRegs);

            rwargs->bytes_moved = sizeof(mvImuData_t);
            return sc;
        }
        if(sc) return sc;
    }
    return sc;
}
static const rtems_driver_address_table bmxMagProtocolAdrTbl = {
    .initialization_entry = bmx055MagInit,
    .open_entry = bmx055MagOpen,
    .close_entry = bmx055MagClose,
    .read_entry = bmx055MagRead,
    .write_entry = bmx055MagWrite,
    .control_entry = bmx055MagControl,
};

const rtems_libi2c_drv_t bmxMagProtocolDrvTbl = {
    .ops = &bmxMagProtocolAdrTbl,
    .size = sizeof(bmxMagProtocolAdrTbl),
};
