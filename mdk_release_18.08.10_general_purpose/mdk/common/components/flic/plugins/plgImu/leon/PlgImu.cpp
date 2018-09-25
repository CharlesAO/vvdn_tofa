#include <rtems.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#include <rtems/status-checks.h>
#include <OsBmx055.h>

#include <math.h>
#include "PlgImu.h"
#define MVLOG_UNIT_NAME imu
#include <mvLog.h>
#define BUFFCNT 9

#define ABS(x)  ( (x) >= 0 ) ? (x) : (-(x))

#define IMU_MEM_AREA  __attribute__((section(".ddr.bss")))
#define ALIGNED(x) __attribute__((aligned(x)))




void* imuThreadFunc(void* context)
{
    imuContext *c = (imuContext *)context;
    mvImuData_t buff[3];
    int rval, i, k=0;
    int fd;

    fd = open(c->name, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {

        mvLog(MVLOG_INFO, "IMU %s open  failed\n No worries, just means that the type of IMU"
               " is different on your board. Will try to use another one\n", c->name);
        return NULL;
    }


    bmxAccelControl_t controlMessage;
    controlMessage.range = (bmxAccelRange_t)c->range;
    if (c->range != -1)
    {
        rval = ioctl(fd, BMX_RANGE_SET, &controlMessage);
        if (rval < 0)
        {
            perror("IMU 1 control failed !");
            return NULL;
        }
    }
    controlMessage.sampleRate = (bmxAccelSampleRate_t)c->sampleRate;
    if (c->sampleRate != -1)
    {
        rval = ioctl(fd, BMX_SAMPLERATE_SET, &controlMessage);
        if (rval < 0)
        {
            perror("IMU 2 control failed !");
            return NULL;
        }
    }
    controlMessage.bmxIrqGpio = c->bmxIrqGpio;
    controlMessage.bmxIrqPriority = c->bmxIrqPriority;
    if (c->bmxIrqGpio != -1 && c->bmxIrqPriority != -1)
    {
        rval = ioctl(fd, BMX_INT_GPIO_SET, &controlMessage);
        if (rval < 0)
        {
            perror("IMU control failed !");
            return NULL;
        }
    }
    while(1)
    {

        rval = read(fd, buff, 3 * sizeof(buff[0]));

        for(i = 0 ; i < rval / (s32)sizeof(buff[0]) ; i++)
        {
            switch(buff[i].type)
            {
            case IMU_TYPE_ACCELEROMETER:
                if((k % 30 == 0))
                    mvLog(MVLOG_DEBUG, "a: %15llu %lf m/s^2 %lf m/s^2 %lf m/s^2\n",
                          buff[i].timestampNs,
                          (double)buff[i].axisData.x,
                          (double)buff[i].axisData.y,
                          (double)buff[i].axisData.z);
                break;
            case IMU_TYPE_GYROSCOPE:
                if((k % 30 == 0))
                    mvLog(MVLOG_DEBUG, "g: %15llu %lf rad/s %lf rad/s %lf rad/s\n",
                           buff[i].timestampNs,
                           (double)buff[i].axisData.x,
                           (double)buff[i].axisData.y,
                           (double)buff[i].axisData.z);

                break;
            case IMU_TYPE_MAGNETOMETER:
                if((k % 30 == 0))
                    mvLog(MVLOG_DEBUG, "m: %15llu %lf uT %lf uT %lf uT\n",
                           buff[i].timestampNs,
                           (double)buff[i].axisData.x,
                           (double)buff[i].axisData.y,
                           (double)buff[i].axisData.z);
                break;
            case IMU_TYPE_TEMPERATURE:
                if((k % 30 == 0))
                    mvLog(MVLOG_DEBUG, "t: %15llu %lf K\n",
                           buff[i].timestampNs,
                           (double)buff[i].temperatureData.t);
                break;
            default:
                mvLog(MVLOG_WARN, "invalid data\n");
                break;
            }
            ImgFramePtr msg;
            if (0 == c->in->TryReceive(&msg)){
                msg.ptr->fb.p1 = (u8*)msg.ptr->base;
                memcpy(msg.ptr->fb.p1, &buff[i], sizeof(buff[i]));
                c->out->Send(&msg);
//                msg.Release();
            }else{
                mvLog(MVLOG_DEBUG,"dropping IMU sample\n");
            }
        }
        k++;
    }
}

int imuInit(void)
{

    return 0;
}
void* PlgImu::threadFunc(void *context)
{
    UNUSED(context);
    mvLogLevelSet(MVLOG_INFO);
    pthread_t taskIdAcc;
    pthread_t taskIdMag;
    pthread_t taskIdGyro;
    pthread_t taskIdBmi;
    int sc;
    imuCtxtAcc.name = "/dev/i2c.bmx055_accel";
    imuCtxtAcc.sampleRate = IMU_BMX055_ACC_SAMPLERATE_500HZ;
    imuCtxtAcc.range = IMU_BMX055_ACC_RANGE2G;
    imuCtxtAcc.bmxIrqGpio = 34;
    imuCtxtAcc.bmxIrqPriority = 10;
    imuCtxtGyro.name = "/dev/i2c.bmx055_gyro";
    imuCtxtGyro.sampleRate = IMU_BMX055_GYRO_400HZ_BW_47HZ;
    imuCtxtGyro.range = IMU_BMX055_GYRO_RANGE2000;
    imuCtxtGyro.bmxIrqGpio = 57;
    imuCtxtGyro.bmxIrqPriority = 10;
    imuCtxtMag.name = "/dev/i2c.bmx055_mag";
    imuCtxtMag.sampleRate = IMU_BMX055_MAG_RATE_HZ_RECOMMENDED;
    imuCtxtMag.range = IMU_BMX055_MAG_ACC_REGULAR;
    imuCtxtMag.bmxIrqGpio = -1; //not used
    imuCtxtMag.bmxIrqPriority = -1; //not used

    imuCtxtBmi.name = "/dev/i2c.BMI160";
    imuCtxtBmi.sampleRate = -1;
    imuCtxtBmi.range = -1;
    imuCtxtBmi.bmxIrqGpio = -1;
    imuCtxtBmi.bmxIrqPriority = -1;

    imuCtxtBmi.in = &emptyIn;
    imuCtxtBmi.out = &out;
    imuCtxtMag.in = &emptyIn;
    imuCtxtMag.out = &out;
    imuCtxtGyro.in = &emptyIn;
    imuCtxtGyro.out = &out;
    imuCtxtAcc.in = &emptyIn;
    imuCtxtAcc.out = &out;
    pthread_attr_t attr;
    struct sched_param params;
    params.sched_priority = sched_get_priority_max(SCHED_FIFO);

    if (pthread_attr_init(&attr) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_init error\n");
    }
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_setinheritsched error\n");
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0)
    {
        mvLog(MVLOG_ERROR, "pthread_attr_setschedpolicy error\n");
    }
    if(pthread_attr_setschedparam(&attr, &params) != 0) {
      mvLog(MVLOG_ERROR, "pthread_attr_setschedparam error");
    }
    sc = pthread_create(&taskIdAcc, &attr, imuThreadFunc, &imuCtxtAcc);
    if (sc)
    {
        mvLog(MVLOG_ERROR, "Control Thread creation failed\n");
    }
    sc = pthread_create(&taskIdGyro, &attr, imuThreadFunc, &imuCtxtGyro);
    if (sc)
    {
        mvLog(MVLOG_ERROR, "Control Thread creation failed\n");
    }
    sc = pthread_create(&taskIdMag, &attr, imuThreadFunc, &imuCtxtMag);
    if (sc)
    {
        mvLog(MVLOG_ERROR, "Control Thread creation failed\n");
    }
    sc = pthread_create(&taskIdBmi, &attr, imuThreadFunc, &imuCtxtBmi);
    if (sc)
    {
        mvLog(MVLOG_ERROR, "Control Thread creation failed\n");
    }

    imuThreadAcc = taskIdAcc;
    imuThreadGyro = taskIdGyro;
    imuThreadBmi = taskIdBmi;
    imuThreadMag = taskIdMag;

    sc = pthread_join(taskIdAcc, NULL);
    sc += pthread_join(taskIdGyro, NULL);
    sc += pthread_join(taskIdBmi, NULL);
    sc += pthread_join(taskIdMag, NULL);

    if (sc)
    {
        mvLog(MVLOG_ERROR, "pthread join failed\n");
    }
    return 0;
}
