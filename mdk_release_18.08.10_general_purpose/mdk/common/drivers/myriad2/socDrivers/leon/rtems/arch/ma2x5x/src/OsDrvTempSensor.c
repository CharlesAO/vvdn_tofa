///
/// @file OsDrvTempSensor.c
/// 
/// 
/// @ingroup OsDrvTempSensor
/// @{
///

#include "OsCommon.h"
#include "HglTempSensor.h"
#include "OsDrvTempSensor.h"
#include "DrvIcbDefines.h"

#include <rtems.h>
#include <rtems/libio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>

#define MAX_DEVNAME 48
#define TEMPSENS_DEFAULT_PRIORITY   14
#define TEMPSENS_INVALID_THRESHOLD (0xDEAD)
#define RES_FREE  0
#define RES_TAKEN 1

#define TEMPSENS_NAME "/dev/tempsensor"

#define OS_DRV_TSENS_N_LOCK(n)          rtems_semaphore_obtain(osDrvTempSensorHandler[n].temp_control_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT)

#define OS_DRV_TSENS_N_UNLOCK(n)        rtems_semaphore_release(osDrvTempSensorHandler[n].temp_control_mutex)

static void osTempSensIrqHandler(uint32_t source);

static struct
{
    rtems_id bg_task_id;
    rtems_id irq_sem;
    volatile uint32_t sensor_field;
}osDrvTempSensorWaitTaskCfg;

typedef struct
{
    uint16_t          irq_sample;
    uint16_t          raw_threshold;
}OsDrvTempSensorIrqData;

static struct
{
    rtems_id                           temp_control_mutex;
    uint32_t                           open_cnt;
    uint32_t                           res_status;
    struct OsDrvTempSensorInfo         info;
    OsDrvTempSensorIrqData             irq_data;
    struct OsDrvTempSensorIrqSettings  irq_settings;
}osDrvTempSensorHandler[TEMPSENS_NUM];

static uint8_t getSensor(volatile uint32_t *field, uint32_t *cb_queue)
{
    uint8_t iter = 0;
    rtems_interrupt_level irq_level;

    for (int i = 0; i < TEMPSENS_NUM; ++i)
    {
        if(((*field >> i) & 1) == 1)
        {
            cb_queue[iter] = i;
            rtems_interrupt_disable(irq_level);
            *field &= ~(1 << i); // clear the corresponding bit in the bit field
            rtems_interrupt_enable(irq_level);
            iter++;
        }
        else
            continue;
    }
    return iter;
}

static rtems_task osDrvTempSensorWaitIrq(rtems_task_argument arg)
{
    UNUSED(arg);
    float act_val[TEMPSENS_NUM];
    uint32_t sens_queue[TEMPSENS_NUM];
    rtems_interrupt_level irq_level;

    while(1)
    {
        rtems_semaphore_obtain(osDrvTempSensorWaitTaskCfg.irq_sem, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

        // The task may receive notifications from multiple sensors from the interrupt handler,
        // therefore each sensor that sent a bit in the sensor bit field is identified and
        // for each sensor its callback is executed
        uint8_t nr_exec = getSensor(&osDrvTempSensorWaitTaskCfg.sensor_field, &sens_queue[0]);

        for (int i = 0; i < nr_exec; ++i)
        {
            uint32_t sensor = sens_queue[i];
            int32_t status = OS_DRV_TSENS_N_LOCK(sensor);
            if (status == RTEMS_SUCCESSFUL)
            {
                if (osDrvTempSensorHandler[sensor].info.mode == OSDRV_TSENS_CONT_TEMPERATURE)
                {
                    act_val[sensor] = HglTempSensorConvertRawtoTemp((float)osDrvTempSensorHandler[sensor].irq_data.irq_sample);
                }
                else
                {
                    act_val[sensor] = HglTempSensorConvertRawToVolt((float)osDrvTempSensorHandler[sensor].irq_data.irq_sample);
                }
                // save locally the callback and the context
                OsDrvTempSensorCb s_callback = osDrvTempSensorHandler[sensor].irq_settings.callback;
                void *s_context = osDrvTempSensorHandler[sensor].irq_settings.context;

                // Release the threshold here in order to be able to configure it inside the callback
                rtems_interrupt_disable(irq_level);
                osDrvTempSensorHandler[sensor].irq_data.raw_threshold = TEMPSENS_INVALID_THRESHOLD;
                rtems_interrupt_enable(irq_level);
                // Release the rest of the resources
                osDrvTempSensorHandler[sensor].irq_settings.callback  = NULL;
                osDrvTempSensorHandler[sensor].irq_settings.context   = NULL;

                if(s_callback != NULL)
                {
                    s_callback(sensor, act_val[sensor], s_context);
                }
            }
            else
            {
                rtems_task_delete(RTEMS_SELF);
            }
            OS_DRV_TSENS_N_UNLOCK(sensor);
        }
    }
}

static int32_t osDrvTempSensorCreateNotifierTask(void)
{
    int32_t res = RTEMS_IO_ERROR;

    res = rtems_task_create(rtems_build_name('T', 'S', 'E', 'V' ),
                            1,
                            RTEMS_MINIMUM_STACK_SIZE,
                            RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_INTERRUPT_LEVEL(0),
                            RTEMS_DEFAULT_ATTRIBUTES | RTEMS_FLOATING_POINT,
                            &osDrvTempSensorWaitTaskCfg.bg_task_id);

    // TODO: Improve the way we handle callbacks:
    // - We may delete the thread when all instances have been closed
    // - We may have a callback server for all drivers .
    if (res == RTEMS_SUCCESSFUL)
    {
        // Start the background task
        res = rtems_task_start(osDrvTempSensorWaitTaskCfg.bg_task_id, osDrvTempSensorWaitIrq, (rtems_task_argument)NULL);
        if (res != RTEMS_SUCCESSFUL)
        {
            rtems_task_delete(osDrvTempSensorWaitTaskCfg.bg_task_id);
        }
    }
    return res;
}

static int32_t osDrvTempSensorInit(rtems_device_major_number major, uint32_t priority)
{
    int32_t sc = RTEMS_IO_ERROR;

    for (int sensor = 0; sensor < TEMPSENS_NUM; ++sensor)
    {
        osDrvTempSensorHandler[sensor].irq_data.raw_threshold = TEMPSENS_INVALID_THRESHOLD;

        if (osDrvTempSensorHandler[sensor].info.mode == OSDRV_TSENS_DISABLED)
            continue;

        char devname[MAX_DEVNAME] = TEMPSENS_NAME;
        // duplicate null termination as we are adding a digit before it
        devname[strlen(devname) + 1] = '\0';
        devname[strlen(devname) + 0] = '0' + (char)sensor;

        //register with the minor number from virtual channel
        sc = rtems_io_register_name(devname, major, sensor);
        if (sc)
        {
            return sc;
        }

        // Create a global semaphore for each of the four sensors
        sc = rtems_semaphore_create(rtems_build_name('T', 'S', 'N', (char)sensor), 1,
                                    RTEMS_BINARY_SEMAPHORE, 0,
                                   (rtems_id *)&osDrvTempSensorHandler[sensor].temp_control_mutex);
        if (sc != RTEMS_SUCCESSFUL)
        {
            return sc;
        }
    }
    // Create a global semaphore for the background task
    sc = rtems_semaphore_create(rtems_build_name('T', 'S', 'W', 'T'), 0,
                                RTEMS_SIMPLE_BINARY_SEMAPHORE, 0,
                               (rtems_id *)&osDrvTempSensorWaitTaskCfg.irq_sem);
    if (sc != RTEMS_SUCCESSFUL)
    {
        return sc;
    }

    HglTempSensorInit();
    // Disable interrupt if enabled
    BSP_Mask_interrupt(IRQ_AON);
    // Clear interrupt if already triggered
    BSP_Clear_interrupt(IRQ_AON);
    // Set Type and priority
    BSP_Set_interrupt_type_priority(IRQ_AON, POS_EDGE_INT, priority);
    // Install new Handler
    BSP_interrupt_register(IRQ_AON, NULL, (rtems_interrupt_handler)osTempSensIrqHandler, (void *)(IRQ_AON));
    // Enable Interrupt
    BSP_Unmask_interrupt(IRQ_AON);

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver temp_sens_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(minor);

    rtems_device_driver sc = RTEMS_IO_ERROR;
    uint32_t i;

    if (e == NULL)
    {
        return RTEMS_SUCCESSFUL;
    }
    else
    {
        uint8_t task_flag = 0;
        struct OsDrvTempSensorConfig *init_cfg = (struct OsDrvTempSensorConfig *) e;

        for (i = 0; i < TEMPSENS_NUM; ++i)
        {
            if (init_cfg->tsens[i].mode < OSDRV_TSENS_DISABLED && init_cfg->tsens[i].mode > OSDRV_TSENS_MANUAL)
            {
                return RTEMS_IO_ERROR;
            }

            osDrvTempSensorHandler[i].info.mode = init_cfg->tsens[i].mode;

            if ( osDrvTempSensorHandler[i].info.mode == OSDRV_TSENS_CONT_TEMPERATURE ||
                 osDrvTempSensorHandler[i].info.mode == OSDRV_TSENS_CONT_VOLTAGE )
            {
                task_flag = 1;
            }
        }

        sc = osDrvTempSensorInit(major, init_cfg->irq_priority);
        if (sc)
        {
            return sc;
        }

        // Create the background task only if there is a sensor configured in continuous mode
        if (task_flag)
        {
            sc = osDrvTempSensorCreateNotifierTask();
        }
    }

    return sc;
}

static int32_t osDrvTempSensorSetMode(OsDrvTempSensor sensor, OsDrvTempSensorMode mode, OsDrvTempSensorSampleType sample_type)
{
    rtems_interrupt_level irq_level;

    rtems_interrupt_disable(irq_level);
    HglTempSensorSetMode(sensor, mode);
    HglTempSensorSetSampleType(sensor, sample_type);
    rtems_interrupt_enable(irq_level);

    osDrvTempSensorHandler[sensor].info.sample_type = sample_type;

    return RTEMS_SUCCESSFUL;
}


static rtems_device_driver temp_sens_open(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning
    UNUSED(major);
    UNUSED(e);
    rtems_device_driver sc = RTEMS_IO_ERROR;
    rtems_libio_open_close_args_t *oargs = (rtems_libio_open_close_args_t *) e;

    if (oargs->iop == NULL)
    {
        return RTEMS_INCORRECT_STATE;
    }

    int32_t status = OS_DRV_TSENS_N_LOCK(minor);
    if (status == RTEMS_SUCCESSFUL)
    {
        if (osDrvTempSensorHandler[minor].open_cnt == 0)
        {
           switch(osDrvTempSensorHandler[minor].info.mode)
           {
            case OSDRV_TSENS_CONT_TEMPERATURE:
                status = osDrvTempSensorSetMode(minor, TSENS_CONTINUOUS_MODE, TSENS_SAMPLE_TEMP);
                sc = status;
                break;
            case OSDRV_TSENS_CONT_VOLTAGE:
                status = osDrvTempSensorSetMode(minor, TSENS_CONTINUOUS_MODE, TSENS_SAMPLE_VOLTAGE);
                sc = status;
                break;
            case OSDRV_TSENS_MANUAL:
                // Is handled inside the IOCTL
                sc = RTEMS_SUCCESSFUL;
                break;
            default:
                sc = RTEMS_IO_ERROR;
                break;
            }
        }
        else
        {
            sc = RTEMS_SUCCESSFUL;
        }
        oargs->iop->data0 = 0;
        osDrvTempSensorHandler[minor].open_cnt++;
    }
    else
    {
        sc = RTEMS_IO_ERROR;
    }
    OS_DRV_TSENS_N_UNLOCK(minor);
    return sc;
}

static rtems_device_driver temp_sens_close(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(e);
    rtems_interrupt_level irq_level;
    rtems_device_driver sc = RTEMS_IO_ERROR;
    rtems_libio_open_close_args_t *cargs = (rtems_libio_open_close_args_t *) e;

    if (cargs->iop == NULL)
    {
        return RTEMS_INCORRECT_STATE;
    }

    int32_t status = OS_DRV_TSENS_N_LOCK(minor);
    if (status == RTEMS_SUCCESSFUL)
    {
        osDrvTempSensorHandler[minor].open_cnt--;
        // Only the thread that configured the irq can release resources
        if (cargs->iop->data0 == 1)
        {
            rtems_interrupt_disable(irq_level);
            osDrvTempSensorHandler[minor].irq_data.raw_threshold = TEMPSENS_INVALID_THRESHOLD;
            HglTempSensorDisableDeviceIrq(minor);
            rtems_interrupt_enable(irq_level);
            osDrvTempSensorHandler[minor].irq_settings.callback  = NULL;
            osDrvTempSensorHandler[minor].irq_settings.context   = NULL;

            osDrvTempSensorHandler[minor].res_status = RES_FREE;
            cargs->iop->data0 = 0;
        }

        if (osDrvTempSensorHandler[minor].open_cnt == 0)
        {
            rtems_interrupt_disable(irq_level);
            status = HglTempSensorSetMode(minor, TSENS_DISABLE);
            rtems_interrupt_enable(irq_level);
            if(status)
            {
                sc = RTEMS_IO_ERROR;
            }
            else
                sc = RTEMS_SUCCESSFUL;
        }
        else
            sc = RTEMS_SUCCESSFUL;
    }
    else
        sc = status;
    OS_DRV_TSENS_N_UNLOCK(minor);
    return sc;
}


static rtems_device_driver temp_sens_read(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver temp_sens_write(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    return RTEMS_SUCCESSFUL;

}

static int32_t osDrvTempSensorEnableIrq(OsDrvTempSensor sensor, struct OsDrvTempSensorIrqSettings* cfg)
{
    uint16_t thr_level = 0;
    rtems_interrupt_level irq_level;

    if (cfg->callback == NULL)
        return RTEMS_IO_ERROR;

    if (osDrvTempSensorHandler[sensor].info.mode == OSDRV_TSENS_CONT_TEMPERATURE)
    {
        thr_level = (uint16_t)HglTempSensorConvertTempToRaw(cfg->threshold);
    }
    else
    {
        thr_level = (uint16_t)HglTempSensorConvertVoltToRaw(cfg->threshold);
    }

    rtems_interrupt_disable(irq_level);
    HglTempSensorSetIrqThreshold(sensor, thr_level);

    osDrvTempSensorHandler[sensor].irq_data.raw_threshold = thr_level;
    osDrvTempSensorHandler[sensor].irq_settings.callback  = cfg->callback;
    osDrvTempSensorHandler[sensor].irq_settings.context   = cfg->context;

    HglTempSensorEnableDeviceIrq(sensor);
    rtems_interrupt_enable(irq_level);

    return RTEMS_SUCCESSFUL;
}

static int32_t osDrvTempSensorReadData(OsDrvTempSensor sensor, float *data, OsDrvTempSensorSampleType sample_type)
{
    int32_t status = RTEMS_IO_ERROR, rc = RTEMS_IO_ERROR;

    switch(osDrvTempSensorHandler[sensor].info.mode)
    {
        case OSDRV_TSENS_CONT_TEMPERATURE:
        {
            // Check if we requested the sample type the sensor was initialised with
            if (sample_type != TSENS_SAMPLE_TEMP)
            {
                return RTEMS_IO_ERROR;
            }

            *data = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawtoTemp);
            break;
        }
        case OSDRV_TSENS_CONT_VOLTAGE:
        {
            // Check if we requested the sample type the sensor was initialised with
            if (sample_type != TSENS_SAMPLE_VOLTAGE)
            {
                return RTEMS_IO_ERROR;
            }

            *data = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawToVolt);
            break;
        }
        case OSDRV_TSENS_MANUAL:
        {
            rc = osDrvTempSensorSetMode(sensor, TSENS_ONE_SHOT_MODE, sample_type);
            if(rc)
                return RTEMS_IO_ERROR;

            if (sample_type == TSENS_SAMPLE_TEMP)
            {
                *data = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawtoTemp);
            }
            else
            {
                *data = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawToVolt);
            }
            break;
        }
        default:
            status = RTEMS_INVALID_NAME;
            break;
    }

    if (*data != TEMPSENS_INVALID)
    {
        status = RTEMS_SUCCESSFUL;
    }
    else
    {
        status = RTEMS_IO_ERROR;
    }

    return status;
}

static rtems_device_driver temp_sens_control(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);
    int32_t status = RTEMS_IO_ERROR;
    rtems_libio_ioctl_args_t *ctl = (rtems_libio_ioctl_args_t *) e;
    rtems_device_driver rc;
    OsDrvTempSensor sensor = (OsDrvTempSensor) minor;

    if (!ctl || !ctl->buffer)
    {
        ctl->ioctl_return = RTEMS_INVALID_ADDRESS;
        return ctl->ioctl_return;
    }

    if (ctl->iop == NULL)
    {
        ctl->ioctl_return = RTEMS_INCORRECT_STATE;
        return ctl->ioctl_return;
    }

    status = OS_DRV_TSENS_N_LOCK(sensor);
    if (status == RTEMS_SUCCESSFUL)
    {
        switch (ctl->command)
        {
            case IOCTL_READ_TEMP:
            {
                rc = osDrvTempSensorReadData(sensor, (float *)ctl->buffer, TSENS_SAMPLE_TEMP);
                ctl->ioctl_return = rc;
                break;
            }
            case IOCTL_READ_VOLT:
            {
                rc = osDrvTempSensorReadData(sensor, (float *)ctl->buffer, TSENS_SAMPLE_VOLTAGE);
                ctl->ioctl_return = rc;
                break;
            }
            case IOCTL_TEMP_THR_CB:
            {
                if (osDrvTempSensorHandler[sensor].info.mode != OSDRV_TSENS_CONT_TEMPERATURE)
                {
                    ctl->ioctl_return = RTEMS_IO_ERROR;
                }
                else
                {
                    if (osDrvTempSensorHandler[sensor].res_status == RES_TAKEN && ctl->iop->data0 == 0)
                    {
                        ctl->ioctl_return = RTEMS_RESOURCE_IN_USE;
                    }
                    else
                    {
                        struct OsDrvTempSensorIrqSettings *irq_cfg = (struct OsDrvTempSensorIrqSettings*)ctl->buffer;
                        rc = osDrvTempSensorEnableIrq(sensor, irq_cfg);
                        if(rc)
                        {
                            ctl->ioctl_return = RTEMS_IO_ERROR;
                        }
                        else
                        {
                            ctl->ioctl_return = RTEMS_SUCCESSFUL;
                        }

                        osDrvTempSensorHandler[sensor].res_status = RES_TAKEN;
                        // the file descriptor will be the owner of the resource
                        ctl->iop->data0 = 1;
                    }
                }
                break;
            }
            case IOCTL_VOLT_THR_CB:
            {
                if (osDrvTempSensorHandler[sensor].info.mode != OSDRV_TSENS_CONT_VOLTAGE)
                {
                    ctl->ioctl_return = RTEMS_IO_ERROR;
                }
                else
                {
                    if (osDrvTempSensorHandler[sensor].res_status == RES_TAKEN && ctl->iop->data0 == 0)
                    {
                        ctl->ioctl_return = RTEMS_RESOURCE_IN_USE;
                    }
                    else
                    {
                        struct OsDrvTempSensorIrqSettings *irq_cfg = (struct OsDrvTempSensorIrqSettings*)ctl->buffer;
                        rc = osDrvTempSensorEnableIrq(sensor, irq_cfg);
                        if(rc)
                        {
                            ctl->ioctl_return = RTEMS_IO_ERROR;
                        }
                        else
                        {
                            ctl->ioctl_return = RTEMS_SUCCESSFUL;
                        }
                        osDrvTempSensorHandler[sensor].res_status = RES_TAKEN;
                        // the file descriptor will be the owner of the resource
                        ctl->iop->data0 = 1;
                    }
                }
                break;
            }
            case IOCTL_GET_TSENS_STATUS:
            {
                struct OsDrvTempSensorInfo *tsens_info = (struct OsDrvTempSensorInfo *)ctl->buffer;
                tsens_info->mode        = osDrvTempSensorHandler[minor].info.mode;
                tsens_info->sample_type = osDrvTempSensorHandler[minor].info.sample_type;

                ctl->ioctl_return = RTEMS_SUCCESSFUL;
                break;
            }
            default:
            {
                ctl->ioctl_return = RTEMS_NOT_IMPLEMENTED;
                break;
            }
        }
    }
    else
        ctl->ioctl_return = status;

    OS_DRV_TSENS_N_UNLOCK(sensor);
    return ctl->ioctl_return;
}

static void osDrvTempSensorDisableIrq(OsDrvTempSensor sensor)
{
    HglTempSensorDisableDeviceIrq(sensor);
    HglTempSensorIrqClr(sensor);
}

static void osTempSensIrqHandler(uint32_t source)
{
    UNUSED(source);
    uint16_t sample = 0, raw_sample = 0, nonThresholdReached = 0;
    uint32_t irq_status = 0;

    // There is only one interrupt line for all sensors,
    // therefore we don't know which sensor triggered the interrupt
    // and we have to iterate through all of them
    for (int sensor = 0; sensor < TEMPSENS_NUM; ++sensor)
    {
        if (osDrvTempSensorHandler[sensor].irq_data.raw_threshold != TEMPSENS_INVALID_THRESHOLD)
        {
            sample = HglTempSensorSingleDataRead(sensor); //if invalid data was read, the content is zero, so no threshold will be reached
            raw_sample = sample & DRV_TSENS_SAMPLE_MASK;
            if ((raw_sample != 0) && (raw_sample >= osDrvTempSensorHandler[sensor].irq_data.raw_threshold))
            {
                osDrvTempSensorHandler[sensor].irq_data.irq_sample = raw_sample;

                osDrvTempSensorWaitTaskCfg.sensor_field |= 1<<sensor;
                irq_status++;

                osDrvTempSensorDisableIrq(sensor);
            }
            else
            {
                nonThresholdReached++;
            }
        }
        }
    if (nonThresholdReached == TEMPSENS_NUM)
    {
        // None of the sensors reach the temperature Threshold,
        // so clear the IRQs manually
        for (int sensor = 0; sensor < TEMPSENS_NUM; ++sensor)
        {
            HglTempSensorIrqClr(sensor);
        }
    }
    if(irq_status)
        rtems_semaphore_release(osDrvTempSensorWaitTaskCfg.irq_sem);
}


rtems_driver_address_table temp_sensor_drv_tbl = {        \
    temp_sens_initialize,                                 \
    temp_sens_open,                                       \
    temp_sens_close,                                      \
    temp_sens_read,                                       \
    temp_sens_write,                                      \
    temp_sens_control                                     \
};

///@}
