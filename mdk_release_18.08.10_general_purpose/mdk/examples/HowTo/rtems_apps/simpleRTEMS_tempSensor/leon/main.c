#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <pthread.h>
#include <UnitTestApi.h>
#include "semaphore.h"
#include "mv_types.h"
#include "OsDrvTempSensor.h"
#include "app_config.h"

#define THREAD_NR    5

#define TEMP_THRESHOLD (30)   // degrees Celsius

#define TSENS_CSS_NAME  "/dev/tempsensor0"
#define TSENS_MSS_NAME  "/dev/tempsensor1"
#define TSENS_UPA0_NAME "/dev/tempsensor2"
#define TSENS_UPA1_NAME "/dev/tempsensor3"

char devnames[][17] = {{TSENS_CSS_NAME}, {TSENS_MSS_NAME}, {TSENS_UPA0_NAME}, {TSENS_UPA1_NAME}};
pthread_t thread_id[THREAD_NR];
sem_t sem;

struct TaskArgs
{
    char *devname;
    void *task;
    struct OsDrvTempSensorIrqSettings* irq_cfg;
}tsk_args[THREAD_NR];

static void* tempCb(OsDrvTempSensor sensor, float data, void* context)
{
    printf("IRQ Triggered Sensor: %x, context: %x, sample: %f degrees Celsius\n", sensor, (unsigned int)*(uint32_t*)context, data);

    sem_post(&sem);

    return NULL;
}

void *task0(void *args)
{
    int status = 0, fd;
    struct TaskArgs *opt = (struct TaskArgs*)args;

    printf("Start thread %x\n", (unsigned int)rtems_task_self());

    fd = open(opt->devname, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        perror("open failed");
        goto EXIT_THREAD;
    }

    // Request the interrupt
    status = ioctl(fd, IOCTL_TEMP_THR_CB, opt->irq_cfg);
    if (status)
    {
        printf("ioctl irq status: %d %x\n", status,  (unsigned int)rtems_task_self());
        perror("");
        goto EXIT_THREAD;
    }

    // Wait till the interrupt triggers
    sem_wait(&sem);

    status = close(fd);
    if (status)
    {
        printf("close failed with sc %d\n", status);
        goto EXIT_THREAD;
    }

    printf("Finished thread id %x\n", (unsigned int)rtems_task_self());

EXIT_THREAD:
    pthread_exit(NULL);
    return NULL;
}

void *task1(void *args)
{
    int status, fd;
    struct TaskArgs *opt = (struct TaskArgs*)args;
    float value;

    printf("Start thread %x\n", (unsigned int)rtems_task_self());

    fd = open(opt->devname, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        perror("open failed");
        goto EXIT_THREAD1;
    }

    // Read temperature several times to see that the values are stable
    for (int i = 0; i < 10; ++i)
    {
        status = ioctl(fd, IOCTL_READ_TEMP, &value);
        if (status)
        {
            perror("read ioctl failed");
            goto EXIT_THREAD1;
        }

        printf("%s: %f degrees Celsius\n",opt->devname, value );

        if (!strcmp(opt->devname, TSENS_CSS_NAME) ||  !strcmp(opt->devname, TSENS_MSS_NAME))
        {
            sleep(5);
        }
        else
        {
            sleep(10);
        }
    }

    status = close(fd);
    if (status)
    {
        printf("close failed with sc %d\n", status);
        goto EXIT_THREAD1;
    }

    printf("Finished thread id %x\n", (unsigned int)rtems_task_self());

EXIT_THREAD1:
    pthread_exit(NULL);
    return NULL;
}

void *task2(void *args)
{
    int status, fd;
    struct TaskArgs *opt = (struct TaskArgs*)args;
    float value;

    printf("Start thread %x\n", (unsigned int)rtems_task_self());

    fd = open(opt->devname, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        perror("open failed");
        goto EXIT_THREAD1;
    }

    // Read voltage several times to see that the values are stable
    for (int i = 0; i < 10; ++i)
    {
        status = ioctl(fd, IOCTL_READ_VOLT, &value);
        if (status)
        {
            perror("read ioctl failed");
            goto EXIT_THREAD1;
        }

        printf("%s: %f Volts\n",opt->devname, value );

        if (!strcmp(opt->devname, TSENS_CSS_NAME) ||  !strcmp(opt->devname, TSENS_MSS_NAME))
        {
            sleep(5);
        }
        else
        {
            sleep(10);
        }
    }

    status = close(fd);
    if (status)
    {
        printf("close failed with sc %d\n", status);
        goto EXIT_THREAD1;
    }

    printf("Finished thread id %x\n", (unsigned int)rtems_task_self());

EXIT_THREAD1:
    pthread_exit(NULL);
    return NULL;
}

void runThreads(pthread_attr_t *attr, struct TaskArgs *t_cfg)
{
    int sc;
    int j;

    // Start threads
    for (j = 0; j < THREAD_NR; ++j)
    {
        sc = pthread_create(&thread_id[j], attr, t_cfg[j].task, (void *)&t_cfg[j]);
        if (sc)
        {
            printf("Thread %d creation failed", j);
            perror("");
        }
    }

    for (j = 0; j < THREAD_NR; ++j)
    {
        sc = pthread_join(thread_id[j], NULL);
        if (sc)
        {
            printf("Thread %d join failed\n", j);
            perror("");
        }
    }
}

void *POSIX_Init( void *argument )
{
    UNUSED(argument);
    pthread_attr_t attr;

    rtems_device_major_number temp_sens_major;

    printf("\n---- Temperature Sensor App Init ----\n");

    unitTestInit();
    // User has to provide a configuration that will remain unchanged
    // and will be used for the entire application
    struct OsDrvTempSensorConfig initcfg =
    {
        .irq_priority = 1,
        {
            {OSDRV_TSENS_MANUAL},           // CSS sensor
            {OSDRV_TSENS_MANUAL},           // MSS sensor
            {OSDRV_TSENS_CONT_TEMPERATURE}, // UPA0 sensor
            {OSDRV_TSENS_CONT_VOLTAGE}      // UPA1 sensor
        }
    };

    // Init clocks, including Temperature sensors clock as it is not handled by the driver
    initClocksAndMemory();

    rtems_io_register_driver(0, &temp_sensor_drv_tbl, &temp_sens_major);
    // Initialise the driver
    rtems_io_initialize(temp_sens_major, 0, (void *)&initcfg);

    if(pthread_attr_init(&attr) != 0)
    {
        printf("pthread_attr_init error");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
        printf("pthread_attr_setinheritsched error");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
    {
        printf("pthread_attr_setschedpolicy error");
    }

    sem_init(&sem, 0, 0);

    // Initialise configuration structures
    struct OsDrvTempSensorIrqSettings cfg =
    {
        .threshold = TEMP_THRESHOLD,
        .context = (void *)&thread_id[2],
        .callback = tempCb
    };

    // Set the device that will be opened in each thread
    for (int i = 0; i < (THREAD_NR - 1); ++i)
    {
        tsk_args[i].devname = (char*)&devnames[i];
    }

    tsk_args[0].task  = (void *)&task2; // Task for reading voltage
    tsk_args[1].task  = (void *)&task1; // Task for reading temperature
    tsk_args[2].task  = (void *)&task1; // Task for reading temperature
    tsk_args[3].task  = (void *)&task2; // Task for reading voltage

    // Initialise the task for threshold interrupt request
    tsk_args[4].devname = (char*)&devnames[2];
    tsk_args[4].irq_cfg = &cfg;
    tsk_args[4].task    = (void *)&task0;

    runThreads(&attr, tsk_args);

    unitTestFinalReport();
    exit(0);
}
