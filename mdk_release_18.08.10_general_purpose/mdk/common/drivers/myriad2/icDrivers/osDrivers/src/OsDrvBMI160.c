#include <assert.h>
#include <rtems.h>
#include <rtems/libi2c.h>
#include <math.h>

#include <mv_types.h>
#include <stdio.h>

#include <OsDrvBMI160.h>
#include <rtems/libio.h>
#include <OsDrvGpio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <OsDrvTimer.h>

#include "bmi160.h"
#include <mvLog.h>

#define CHECK_OVERFLOW 0

struct bmi160_t s_bmi160;

extern volatile struct Bmi160Stats bmi160_stats;

static rtems_device_major_number bmi160_major;
static rtems_device_minor_number bmi160_minor;

static rtems_id bmi160_interrupt_mq;

static float gRotMat[3][3] =
{
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

static s8 bmi160_libi2c_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    int sc, i;

    (void)dev_addr;

    // this is a write transaction;
    reg_addr &= ~0x80;

    sc = rtems_libi2c_send_start(bmi160_minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "WRITE1 err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(bmi160_minor,TRUE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "WRITE2 err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes (bmi160_minor, &reg_addr, 1);
    if (sc != 1) {
        mvLog(MVLOG_DEBUG, "WRITE3 read err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_write_bytes (bmi160_minor, reg_data, cnt);
    if (sc != cnt) {
        mvLog(MVLOG_DEBUG, "WRITE4 read err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_send_stop(bmi160_minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "WRITE5 err = 0x%0x\n", -sc);
    }

    mvLog(MVLOG_DEBUG, "WROTE: 0x%02X :", reg_addr & 0x7F);
    for(i = 0 ; i < cnt ; i++)
        mvLog(MVLOG_DEBUG, " 0x%02X", reg_data[i]);
    mvLog(MVLOG_DEBUG, "\n");

    return RTEMS_SUCCESSFUL;
}

static s8 bmi160_libi2c_burst_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u32 cnt)
{
    int sc;
    u32 i;

    (void)dev_addr;

    // this is a read transaction;
    reg_addr |= 0x80;

    sc = rtems_libi2c_send_start(bmi160_minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "READ1 err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(bmi160_minor,TRUE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "READ2 err = 0x%0x\n", sc);
        return sc;
    }
    sc = rtems_libi2c_write_bytes (bmi160_minor, &reg_addr, 1);
    if (sc != 1) {
        mvLog(MVLOG_DEBUG, "READ3 read err = 0x%0x, 0x%x\n", sc, reg_addr);
        return sc;
    }

    sc = rtems_libi2c_send_addr(bmi160_minor,FALSE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "READ2 err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_read_bytes (bmi160_minor, reg_data, cnt);
    if (sc != (s32)cnt) {
        mvLog(MVLOG_DEBUG, "READ4 read err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_send_stop(bmi160_minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "READ5 err = 0x%0x\n", -sc);
    }

    mvLog(MVLOG_DEBUG, "READ: 0x%02X :", reg_addr & 0x7F);
    for(i = 0 ; i < cnt ; i++)
        mvLog(MVLOG_DEBUG, " 0x%02X", reg_data[i]);
    mvLog(MVLOG_DEBUG, "\n");

    return RTEMS_SUCCESSFUL;
}

static s8 bmi160_libi2c_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt)
{
    return bmi160_libi2c_burst_read(dev_addr, reg_addr, reg_data, cnt);
}

static void bmi160_delay_msec(BMI160_MDELAY_DATA_TYPE msek)
{
    rtems_task_wake_after(msek);
}

static rtems_status_code spi_BMI160_write(rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void                      *arg)
{
    (void)major;
    (void)minor;
    (void)arg;

    return RTEMS_NOT_DEFINED;
}

static rtems_status_code spi_BMI160_control( rtems_device_major_number major,
                                             rtems_device_minor_number minor,
                                             void                      *arg );


static rtems_status_code spi_BMI160_close( rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           void                      *arg )
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    (void)major;
    (void)minor;
    (void)arg;

    sc =  bmi160_set_command_register(ACCEL_SUSPEND);
    sc += bmi160_set_command_register(GYRO_MODE_SUSPEND);
    sc += bmi160_set_command_register(MAG_MODE_SUSPEND);
    s_bmi160.delay_msec(10);

    int fd = open("/dev/gpio.bmi160_int1", O_RDWR);
    sc = ioctl(fd, GPIO_IRQ_DISABLE);
    close(fd);

#if CHECK_OVERFLOW
    int fd2 = open("/dev/gpio.bmi160_int2", O_RDWR);
    sc = ioctl(fd2, GPIO_IRQ_DISABLE);
    close(fd2);
#endif

    return sc;
}

#define ACCEL_RANGE 4
#define ONE_G ((float) 9.80665 ) // see: https://en.wikipedia.org/wiki/G-force

static float convertAccelToSI(s16 rawVal)
{
    float r;
    r = (float)rawVal;
    r /= (float)0x7FFF;
    r *= (float)ACCEL_RANGE;
    r *= (float) ONE_G;
    return r;
}

#define GYRO_RANGE ((float)2000.0f)
#define RAD_PER_DEG ( M_PI / 180.0f )

static float convertGyroToSI(s16 rawVal)
{
    float r;
    r = (float)rawVal;
    r *= GYRO_RANGE;
    r /= (float)0x7FFF;
    r *= RAD_PER_DEG;
    return r;
}

static float convertMagToSI(s16 rawVal)
{
    float r;
    r = (float)rawVal;
    r /= 16;
    return r;
}

static mvImuData_t applyRotationMatrix(mvImuData_t in, float r[3][3])
{
    mvImuData_t out = in; // preserve other parameters

    out.axisData.x =
        in.axisData.x * r[0][0] +
        in.axisData.y * r[0][1] +
        in.axisData.z * r[0][2];

    out.axisData.y =
        in.axisData.x * r[1][0] +
        in.axisData.y * r[1][1] +
        in.axisData.z * r[1][2];

    out.axisData.z =
        in.axisData.x * r[2][0] +
        in.axisData.y * r[2][1] +
        in.axisData.z * r[2][2];

    return out;
}

static void spi_BMI160_fill_timestamps(u64 current_timestamp, u64 saved_timestamp,
                                       u8 nsamples, u64* timestamp_fifo)
{
    if (nsamples == 0)
        return;

    u64 diff = current_timestamp - saved_timestamp;
    if (saved_timestamp == 0)
        diff = 0;

    u64 interp = diff / nsamples;
    unsigned int i;
    for (i = 0; i < v_accel_index_u8; ++i) {
        timestamp_fifo[i] = current_timestamp - (interp * i);
    }
}

static int fifo_reads = 0;
static rtems_status_code spi_BMI160_read(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void                      *arg)
{
    rtems_libio_rw_args_t *rwargs = arg;
    int rc = RTEMS_SUCCESSFUL;
    u64 lasttimestampNs;
    size_t size;
    mvImuData_t *idt;
    static u32 ai = 0;
    static u32 gi = 0;
    static u32 mi = 0;
    static u64 saved_timestamp = 0;
    static u64 accel_timestamp_fifo[FIFO_FRAME_CNT];
    static u64 gyro_timestamp_fifo[FIFO_FRAME_CNT];
    static u64 mag_timestamp_fifo[FIFO_FRAME_CNT];

    (void)major;
    (void)minor;

    rwargs->bytes_moved = 0;

    if(rwargs == NULL || rwargs->buffer == NULL)
        return RTEMS_INVALID_ADDRESS;

    if (ai < v_accel_index_u8 || gi < v_gyro_index_u8 || mi < v_mag_index_u8) {
        // If the read index values and the sensor fifo index values do not match,
        // it means the last read buffer was too small to fit all the data that
        // was read from the IMU FIFO. This can happen if > 1 sample is added
        // to the IMU FIFO between reads and is a sign the read thread is reading
        // too slowly. It can also happen if the supplied buffer is too small.
        // In this case, immediately return with the data that is already
        // available.
        bmi160_stats.late_reads++;
    } else {
        // If the read index values and the sensor fifo index values DO match,
        // all the available data has alraedy been read.  Reset the indices,
        // wait for new data to arrive in the IMU FIFO and read it into the
        // sensor FIFOs.
        fifo_reads++;
        rc = rtems_message_queue_receive(bmi160_interrupt_mq,
                                       &lasttimestampNs,
                                         &size,
                                         RTEMS_WAIT,
                                         500);
        if(rc)
            return rc;

        u32 fifo_length = 0;
        rc = bmi160_fifo_length(&fifo_length);
        if (rc)
            return rc;

        /* read the FIFO data*/
        rc =  bmi160_read_fifo_header_data(fifo_length);
      if (rc)
            return rc;

        ai = 0;
        gi = 0;
        mi = 0;

      u64 current_timestamp = lasttimestampNs;
        spi_BMI160_fill_timestamps(current_timestamp, saved_timestamp,
                                   v_accel_index_u8, accel_timestamp_fifo);
        spi_BMI160_fill_timestamps(current_timestamp, saved_timestamp,
                                   v_gyro_index_u8, gyro_timestamp_fifo);
        spi_BMI160_fill_timestamps(current_timestamp, saved_timestamp,
                                   v_mag_index_u8, mag_timestamp_fifo);
        saved_timestamp = current_timestamp;
    }


    idt = (mvImuData_t *)rwargs->buffer;

    mvLog(MVLOG_DEBUG, "%p < %p", idt + sizeof(*idt), rwargs->buffer + rwargs->count);
    while((u32)(idt + 1) <= (u32)(rwargs->buffer + rwargs->count))
    {
        if(ai < v_accel_index_u8)
        {
            idt->type = IMU_TYPE_ACCELEROMETER;
            idt->axisData.x = convertAccelToSI(accel_fifo[ai].x);
            idt->axisData.y = convertAccelToSI(accel_fifo[ai].y);
            idt->axisData.z = convertAccelToSI(accel_fifo[ai].z);
            idt->timestampNs = accel_timestamp_fifo[ai];

            *idt = applyRotationMatrix(*idt, gRotMat);

            ai++;
            idt++;
            rwargs->bytes_moved += sizeof(*idt);
            continue;
        }

        if(gi < v_gyro_index_u8)
        {
            idt->type = IMU_TYPE_GYROSCOPE;
            idt->axisData.x = convertGyroToSI(gyro_fifo[gi].x);
            idt->axisData.y = convertGyroToSI(gyro_fifo[gi].y);
            idt->axisData.z = convertGyroToSI(gyro_fifo[gi].z);
            idt->timestampNs = gyro_timestamp_fifo[gi];

            *idt = applyRotationMatrix(*idt, gRotMat);

            gi++;
            idt++;
            rwargs->bytes_moved += sizeof(*idt);
            continue;
        }

        if(mi < v_mag_index_u8)
        {
            idt->type = IMU_TYPE_MAGNETOMETER;
            idt->axisData.x = convertMagToSI(mag_fifo[mi].x);
            idt->axisData.y = convertMagToSI(mag_fifo[mi].y);
            idt->axisData.z = convertMagToSI(mag_fifo[mi].z);
            idt->timestampNs = mag_timestamp_fifo[mi];

            *idt = applyRotationMatrix(*idt, gRotMat);

            mi++;
            idt++;
            rwargs->bytes_moved += sizeof(*idt);
            continue;
        }

        break;
    }

    bmi160_stats.reads++;
    return rc;
}

/*!
 * @brief struct used for assign the value for
 *      gyro sleep configuration
 */
struct gyro_sleep_setting {
    u8 sleep_trigger;/**< gyro sleep trigger configuration*/
    u8 wakeup_trigger;/**< gyro wakeup trigger configuration*/
    u8 sleep_state;/**< gyro sleep state configuration*/
    u8 wakeup_int;/**< gyro wakeup interrupt configuration*/
};
/********************************/
/**\name POWE MODES DEFINITION */
/*******************************/
#define ACCEL_MODE_NORMAL       0x11
#define GYRO_MODE_NORMAL        0x15
#define ACCEL_LOWPOWER          0X12
#define MAG_SUSPEND_MODE        1
#define C_BMI160_THIRTY_U8X             30


#define BMI160_MAG_INTERFACE_OFF_PRIMARY_ON             0x00
#define BMI160_MAG_INTERFACE_ON_PRIMARY_ON              0x02

#define BMI160_WRITE_ZERO 0
#define BMI160_WRITE_ONE 1
#define BMI160_WRITE_TWO 2
#define BMI160_WRITE_FOUR 4

static BMI160_RETURN_FUNCTION_TYPE bmi160_interrupt_configuration(void)
{
    /* This variable used for provide the communication
       results*/
    BMI160_RETURN_FUNCTION_TYPE com_rslt = ERROR;

    /* Configure the in/out control of interrupt1*/
    com_rslt = bmi160_set_output_enable(C_BMI160_ZERO_U8X,
                                        BMI160_WRITE_ONE);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);
    /* Configure the in/out control of interrupt2*/
    com_rslt += bmi160_set_output_enable(C_BMI160_ONE_U8X,
                                         BMI160_WRITE_ONE);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

    com_rslt += bmi160_set_intr_output_type(C_BMI160_ZERO_U8X,
                                            BMI160_WRITE_ZERO);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);
    com_rslt += bmi160_set_intr_output_type(C_BMI160_ONE_U8X,
                                            BMI160_WRITE_ZERO);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

    /* Configure the interrupt1 active high
       0x00 -   Active low
       0x01 -   Active high*/
    com_rslt += bmi160_set_intr_level(C_BMI160_ZERO_U8X,
                                      BMI160_WRITE_ONE);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);
    /* Configure the interrupt2 active high
       0x00 -   Active low
       0x01 -   Active high*/
    com_rslt += bmi160_set_intr_level(C_BMI160_ONE_U8X,
                                      BMI160_WRITE_ONE);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

//    com_rslt += bmi160_set_latch_intr(0x00);
//    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

//    bmi160_set_fifo_tag_intr2_enable(0x01);
//    bmi160_set_fifo_tag_intr1_enable(0x01);
//    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

    bmi160_set_intr_enable_1(BMI160_FIFO_WM_ENABLE, 1);
    s_bmi160.delay_msec(C_BMI160_ONE_U8X);

    return com_rslt;
}

static rtems_status_code spi_BMI160_control(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void  *arg)
{
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) arg;
    struct gyro_sleep_setting gyr_setting;

    /* Variable used for get the status of mag interface*/
    u8 v_mag_interface_u8 = C_BMI160_ZERO_U8X;
    BMI160_RETURN_FUNCTION_TYPE com_rslt = ERROR;

    (void)major;
    (void)minor;

    if (!ctl)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    u8 v_running_mode_u8 = ctl->command;

    /* Configure the gyro sleep setting based on your need*/
    if (v_running_mode_u8 == IOCTL_STANDARD_UI_ADVANCEPOWERSAVE) {
        gyr_setting. sleep_trigger = BMI160_WRITE_FOUR;
        gyr_setting. wakeup_trigger = BMI160_WRITE_TWO;
        gyr_setting. sleep_state = BMI160_WRITE_ZERO;
        gyr_setting. wakeup_int = BMI160_WRITE_ZERO;
        (void)gyr_setting; // TODO: check to use this variable
    }

    /* The below code used for enable and
       disable the secondary mag interface*/
    com_rslt = bmi160_get_if_mode(&v_mag_interface_u8);
    if (((v_running_mode_u8 == IOCTL_STANDARD_UI_IMU_FIFO) ||
         (v_running_mode_u8 == IOCTL_STANDARD_UI_IMU) ||
         (v_running_mode_u8 == IOCTL_STANDARD_UI_ADVANCEPOWERSAVE) ||
         (v_running_mode_u8 == IOCTL_APPLICATION_NAVIGATION) ||
         (v_running_mode_u8 == IOCTL_ACCEL_PEDOMETER) ||
         (v_running_mode_u8 == IOCTL_APPLICATION_REMOTE_CONTROL) ||
         (v_running_mode_u8 == IOCTL_APPLICATION_INDOOR_NAVIGATION))
        && (v_mag_interface_u8 == BMI160_MAG_INTERFACE_ON_PRIMARY_ON))
    {
        com_rslt += bmi160_set_bmm150_mag_and_secondary_if_power_mode(MAG_SUSPEND_MODE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        com_rslt += bmi160_set_if_mode(BMI160_MAG_INTERFACE_OFF_PRIMARY_ON);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
    }
    if (v_running_mode_u8 == IOCTL_STANDARD_UI_9DOF_FIFO ||
        v_running_mode_u8 == IOCTL_APPLICATION_HEAD_TRACKING ||
        v_running_mode_u8 == IOCTL_APPLICATION_NAVIGATION) {
        if (v_mag_interface_u8 != BMI160_MAG_INTERFACE_OFF_PRIMARY_ON) {
            com_rslt += bmi160_set_if_mode(BMI160_MAG_INTERFACE_OFF_PRIMARY_ON);
        }
        /* Init the magnetometer */
        com_rslt += bmi160_bmm150_mag_interface_init();
        /* bmi160_delay_ms in ms*/
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);
    }

    switch (ctl->command)
    {
    case IOCTL_STANDARD_UI_IMU_FIFO:
    case IOCTL_STANDARD_UI_IMU:
    case IOCTL_STANDARD_UI_ADVANCEPOWERSAVE:
    case IOCTL_ACCEL_PEDOMETER:
    case IOCTL_APPLICATION_HEAD_TRACKING:
    case IOCTL_APPLICATION_NAVIGATION:
    case IOCTL_APPLICATION_REMOTE_CONTROL:
    case IOCTL_APPLICATION_INDOOR_NAVIGATION:
    case IOCTL_STANDARD_UI_9DOF_FIFO:
    {
        /*Set the accel mode as Normal write in the register 0x7E*/
        com_rslt = bmi160_set_command_register(ACCEL_MODE_NORMAL);
        /* bmi160_delay_ms in ms*/
        s_bmi160.delay_msec(C_BMI160_THIRTY_U8X);
        /*Set the gyro mode as Normal write in the register 0x7E*/
        com_rslt += bmi160_set_command_register(GYRO_MODE_NORMAL);
        /* bmi160_delay_ms in ms*/
        s_bmi160.delay_msec(C_BMI160_THIRTY_U8X);
        /* Set the accel bandwidth as Normal */
        com_rslt += bmi160_set_accel_bw(BMI160_ACCEL_NORMAL_AVG4);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Set the gryo bandwidth as Normal */
        com_rslt += bmi160_set_gyro_bw(BMI160_GYRO_NORMAL_MODE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* set gyro data rate as 100Hz*/
        // NOTE: if this changes, please change the fifo_overflow estimate in
        // the bmi160.c read_fifo function.
        com_rslt += bmi160_set_gyro_output_data_rate(
            BMI160_GYRO_OUTPUT_DATA_RATE_200HZ);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* set accel data range to 4G*/
        com_rslt += bmi160_set_accel_range(BMI160_ACCEL_RANGE_4G);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* set gyro data range to 2000deg/sec*/
        com_rslt += bmi160_set_gyro_range(BMI160_GYRO_RANGE_2000_DEG_SEC);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* set accel data rate as 100Hz*/
        // NOTE: if this changes, please change the fifo_overflow estimate in
        // the bmi160.c read_fifo function.
        com_rslt += bmi160_set_accel_output_data_rate(
            BMI160_ACCEL_OUTPUT_DATA_RATE_200HZ);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /***** read FIFO data based on interrupt*****/
        com_rslt += bmi160_interrupt_configuration();
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO header*/
        com_rslt += bmi160_set_fifo_header_enable(FIFO_HEADER_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO mag*/
        com_rslt += bmi160_set_fifo_mag_enable(FIFO_MAG_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO accel*/
        com_rslt += bmi160_set_fifo_accel_enable(FIFO_ACCEL_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO gyro*/
        com_rslt += bmi160_set_fifo_gyro_enable(FIFO_GYRO_ENABLE);
        /* Enable the FIFO time*/
        com_rslt += bmi160_set_fifo_time_enable(FIFO_TIME_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO stop on full*/
        com_rslt += bmi160_set_fifo_stop_on_full(
            FIFO_STOPONFULL_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO water mark interrupt1*/
        com_rslt += bmi160_set_intr_fifo_wm(C_BMI160_ZERO_U8X,
                                            FIFO_WM_INTERRUPT_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* Enable the FIFO water mark interrupt2*/
        com_rslt += bmi160_set_intr_fifo_full(C_BMI160_ONE_U8X,
                                              FIFO_WM_INTERRUPT_ENABLE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/
        /* set the fifo water mark*/
        com_rslt += bmi160_set_fifo_wm(BMI160_WRITE_ONE);
        s_bmi160.delay_msec(C_BMI160_ONE_U8X);/* bmi160_delay_ms in ms*/

        ctl->ioctl_return = com_rslt;
        return com_rslt;
    }
    case IOCTL_SET_ROTATION_MATRIX: {
        if (ctl->buffer == NULL) {
            ctl->ioctl_return = -RTEMS_INVALID_ADDRESS;
            return -RTEMS_INVALID_ADDRESS;
        }
        memcpy(gRotMat, ctl->buffer, sizeof(gRotMat));
        ctl->ioctl_return = RTEMS_SUCCESSFUL;
        return RTEMS_SUCCESSFUL;
    }break;
    case IOCTL_BMI160_GET_TEMP: {
      s16 temp_raw;
      com_rslt = bmi160_get_temp(&temp_raw);
      if (!com_rslt) {
        // 0 = 23 degrees, each LSB increases by 1/2^9.
        float temp_celcius = 23.0 + (temp_raw / 512.0);
        memcpy(ctl->buffer, &temp_celcius, sizeof(float));
      }
      ctl->ioctl_return = com_rslt;
      return com_rslt;
    }break;
    default:
        return -RTEMS_INVALID_NUMBER;
    }

    return com_rslt;
}


static rtems_status_code readbyte(rtems_device_minor_number minor, uint8_t address, uint8_t * value) {
    uint8_t rxbuf[2];
    uint8_t txbuf[2];
    rtems_libi2c_read_write_t rwop;
    rtems_status_code sc;

    txbuf[0] = 0x80 | address;
    txbuf[1] = 0xAA;
    rwop.byte_cnt = 2;
    rwop.rd_buf = &rxbuf[0];
    rwop.wr_buf = &txbuf[0];

    sc = rtems_libi2c_send_start(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "IMU err = 0x%0x\n", sc);
        return sc;
    }

    sc = rtems_libi2c_send_addr(minor,TRUE);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "IMU2 err = 0x%0x\n", sc);
        return sc;
    }
    sc = rtems_libi2c_ioctl (minor,
                             RTEMS_LIBI2C_IOCTL_READ_WRITE,
                             &rwop);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "IMU3 read err = 0x%0x\n", sc);
        return sc;
    } else {
        *value=rxbuf[1];
    }

    sc = rtems_libi2c_send_stop(minor);
    if (sc != RTEMS_SUCCESSFUL) {
        mvLog(MVLOG_DEBUG, "IMU4 err = 0x%0x\n", sc);
    }
    return sc;
}

static void bmi160HandlerIntr1(void * context)
{
    u64 timestampNs;
    int sc;

    (void)context;
    bmi160_stats.interrupts++;
    sc = OsDrvTimerGetSystemTimeNs(&timestampNs);
    if(sc)
        timestampNs = -1;
    // Subtract the LPF time
    //TODO: Find exact value which needs to be subtracted ( 5ms was measured)
    timestampNs -= 5 * 1000 * 1000;

    sc = rtems_message_queue_send(bmi160_interrupt_mq,
                                  &timestampNs,
                                  sizeof(timestampNs));
    if(sc) {
        bmi160_stats.interrupt_queue_errors++;
        //mvLog(MVLOG_WARN, "%s %d", __func__, sc);
    }
}

#if CHECK_OVERFLOW
static void bmi160HandlerIntr2(void * context, struct timespec ts)
{
    (void)context;

    printk("%s\n", __func__);

    bmi160_fifoOverflow++;
}
#endif

static rtems_status_code bmi160SetGpioIrq(const char * gpioName, int priority,
                                          void (*handler)(void * context),
                                          void *context)
{
    int sc = RTEMS_SUCCESSFUL;
    gpioInterruptHndl_t gpioIrqHndl;

    gpioIrqHndl.priority = priority;
    gpioIrqHndl.type = POS_EDGE_INT;
    gpioIrqHndl.handler = handler;
    gpioIrqHndl.context = context;

    int fd = open(gpioName, O_RDWR);
    sc = ioctl(fd, GPIO_IRQ_ENABLE, &gpioIrqHndl);
    close(fd);
    return sc;
}

static rtems_status_code spi_BMI160_open( rtems_device_major_number major,
                                          rtems_device_minor_number minor,
                                          void                      *arg )
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_libio_ioctl_args_t ctl;
    u32 cnt;
    u64 temp;
    size_t size;

    (void)arg;

#if CHECK_OVERFLOW
    sc = bmi160SetGpioIrq("/dev/gpio.bmi160_int2", 10, bmi160HandlerIntr2, NULL);
    mvLog(MVLOG_DEBUG, " bosch isr driver init sc = %d", sc);
#endif

    sc = bmi160SetGpioIrq("/dev/gpio.bmi160_int1", 10, bmi160HandlerIntr1, NULL);
    mvLog(MVLOG_DEBUG, " bosch isr driver init sc = %d", sc);

    ctl.command = IOCTL_STANDARD_UI_9DOF_FIFO;
    sc = spi_BMI160_control(major, minor, &ctl);
    if(sc) return sc;

    //reset interrupt engine
    sc = bmi160_set_command_register(0xB1);
    // clear data in FIFO - this will cause interrupts to be generated again.
    sc +=  bmi160_set_command_register(0xB0);
    if(sc) return sc;

    // Wait until at least 1 interrupt has been generated - after this point
    // no further interrupts will be generated until the IMU FIFO is cleared,
    // either by reading or with a flush.
    sc = rtems_message_queue_receive(bmi160_interrupt_mq, &temp, &size,
                                     RTEMS_WAIT, 500);
    if (sc) {
        perror("IMU not generating interrupts\n");
        return sc;
    }
    sc = rtems_message_queue_flush(bmi160_interrupt_mq, &cnt);
    if(sc) return sc;

    v_accel_index_u8 = 0;
    v_gyro_index_u8 = 0;
    v_mag_index_u8 = 0;
    memset((void*)&bmi160_stats, 0, sizeof(bmi160_stats));

    return sc;
}


static rtems_status_code spi_BMI160_init(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void  *arg)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    uint8_t bytevalue;

    (void)arg;

    // Toggle the SS line from SPI on low in order to switch to SPI mode
    // When connected to I2C it basically does nothing.
    sc = readbyte(minor, 0x7F, & bytevalue);

    rtems_task_wake_after(10);

    s_bmi160.bus_write  = bmi160_libi2c_bus_write;
    s_bmi160.bus_read   = bmi160_libi2c_bus_read;
    s_bmi160.burst_read = bmi160_libi2c_burst_read;
    s_bmi160.delay_msec = bmi160_delay_msec;

    bmi160_major = major;
    bmi160_minor = minor;

    sc = bmi160_init(&s_bmi160);
    mvLog(MVLOG_DEBUG, " bosch driver init sc = %d \n", sc);

    mvLog(MVLOG_DEBUG, "Chip ID is %X \n", s_bmi160.chip_id);

    if(s_bmi160.chip_id != 0xD1)
        return RTEMS_INVALID_ID;

    sc = rtems_message_queue_create(rtems_build_name( 'B', 'O', 'S', 'H' ),
                                    1,
                                    sizeof(u64),
                                    RTEMS_DEFAULT_ATTRIBUTES,
                                    &bmi160_interrupt_mq);
    if(sc)
        mvLog(MVLOG_ERROR, "queue creation error %d \n", sc);

    // Send software reset
    sc = bmi160_set_command_register(0xB6);
    if(sc) return sc;
    rtems_task_wake_after(50);

    return sc;
}

rtems_driver_address_table OsBMI160DrvRwOps = {
    .initialization_entry = spi_BMI160_init,
    .open_entry = spi_BMI160_open,
    .close_entry = spi_BMI160_close,
    .read_entry =  spi_BMI160_read,
    .write_entry = spi_BMI160_write,
    .control_entry = spi_BMI160_control,
};

OsSpiBMI160_t OsSpiBMI160RwDrv = {
    {   /* public fields */
        .ops =         &OsBMI160DrvRwOps, /* operations of general drv */
        .size =        sizeof (OsSpiBMI160RwDrv),
    },
    {
        .baudrate =             10*1000*1000,
    },
};

rtems_libi2c_drv_t OsBMI160DriverDescriptor =
{   /* public fields */
    .ops =         &OsBMI160DrvRwOps, /* operations of general drv */
    .size =        sizeof (OsBMI160DriverDescriptor),
};
