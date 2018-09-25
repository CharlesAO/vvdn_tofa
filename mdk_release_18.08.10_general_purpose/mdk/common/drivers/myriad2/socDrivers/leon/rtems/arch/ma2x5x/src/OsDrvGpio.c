///
/// @file OsDrvGpio.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvGpio
/// @{
/// @brief     GPIO RTEMS API Functions.
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvGpio.h"
#include "OsCommon.h"
#include <rtems/status-checks.h>
#include <rtems/libio.h>
#include <bsp/irq-generic.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DEFAULT_WAIT_TICKS_TIMEOUT 5
#define NUM_PWM_BLOCKS 6

#define OS_DRV_GPIO_L1

#define GPIO_BASE_NAME "/dev/gpio."

//#define OS_DRV_GPIO_L2
//#define OS_DRV_GPIO_L3

#ifdef OS_DRV_GPIO_L1
#define OS_GPIO_DPRINTF1(...)            printf(__VA_ARGS__)
#else
#define OS_GPIO_DPRINTF1(...)
#endif

#ifdef OS_DRV_GPIO_L2
#define OS_GPIO_DPRINTF2(...)            printf(__VA_ARGS__)
#else
#define OS_GPIO_DPRINTF2(...)
#endif

#ifdef OS_DRV_GPIO_L3
#define OS_GPIO_DPRINTF3(...)            printf(__VA_ARGS__)
#else
#define OS_GPIO_DPRINTF3(...)
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
typedef struct {
    // Use this flag to mark a Gpio as taken or available
    rtems_id takenByTaskId;
} osDrvGpioControl_t;

static rtems_id gpioControlMutex;
static rtems_id gpioPwmControlMutex;

static osDrvGpioControl_t osDrvGpioControl[GPIO_NUM_PINS];
static osDrvGpioControl_t osDrvGpioPwmControl[GPIO_PWM_NUM];

static OS_MYRIAD_DRIVER_STATUS osDrvGpioControlStatus;

typedef struct gpioToPwmBlock_t
{
    u8 gpio;
    u8 pwmBlock;
} gpioToPwmBlock_t;

static gpioToPwmBlock_t gpioToPwm[] =
{
///{GPIO, PWM BLOCK}
    { 27, 1 },
    { 33, 0 },
    { 45, 0 },
    { 44, 2 },
    { 46, 1 },
    { 49, 3 },
    { 50, 4 },
    { 51, 5 },
    { 62, 3 },
    { 63, 4 },
    { 74, 4 },
    { 75, 5 },
    { 77, 0 },
    { 82, 3 },
    { 84, 2 },
};

// Default value for wait ticks. Can be modified from IOCTL
static u32 waitTicks = DEFAULT_WAIT_TICKS_TIMEOUT;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioLock(rtems_id semId, u32 waitTicks)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;
    rtems_status_code rc;

    rc = rtems_semaphore_obtain(semId,
                                (waitTicks == 0)?RTEMS_NO_WAIT:RTEMS_WAIT,
                                waitTicks);

    if (rc != RTEMS_SUCCESSFUL)
    {
        // Unable to obtain semaphore
        res = (waitTicks == 0) ? OS_MYR_DRV_RESOURCE_BUSY : OS_MYR_DRV_ERROR;
    }
    OS_GPIO_DPRINTF2("osDrvGpioLock res %d wait_ticks %d\n", res,
                     waitTicks);
    return res;
}

static inline OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioUnlock(rtems_id semId)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;
    rtems_status_code rc;

    rc = rtems_semaphore_release(semId);
    if (rc != RTEMS_SUCCESSFUL)
    {
        // Failed to release semaphore
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }
    return res;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioRequest(
    osDrvGpioControl_t controlArray[],
    u8 index,
    u32 waitTicks,
    rtems_id semId)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (osDrvGpioLock(semId, waitTicks) != OS_MYR_DRV_SUCCESS)
    {
        // Unable to obtain semaphore
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    // Check if Gpio is FREE, if so, mark it as taken
    if (controlArray[index].takenByTaskId == 0)
    {
        controlArray[index].takenByTaskId = rtems_task_self();
    }
    else
    {
        // Failed to obtain GPIO
        res = OS_MYR_DRV_RESOURCE_BUSY;
    }

    if (osDrvGpioUnlock(semId) != OS_MYR_DRV_SUCCESS)
    {
        return OS_MYR_DRV_ERROR;
    }

    return res;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioRelease(
    osDrvGpioControl_t controlArray[],
    u8 index,
    u32 waitTicks,
    rtems_id semId)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (osDrvGpioLock(semId, waitTicks) != OS_MYR_DRV_SUCCESS)
    {
        // Unable to obtain semaphore
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    // Check if Gpio is FREE, if so, mark it as taken
    if (controlArray[index].takenByTaskId == rtems_task_self())
    {
        controlArray[index].takenByTaskId = 0;
    }
    else
    {
        // Failed to obtain GPIO
        res = OS_MYR_RESOURCE_NOT_OWNED;
    }

    if (osDrvGpioUnlock(semId) != OS_MYR_DRV_SUCCESS)
    {
        return OS_MYR_DRV_ERROR;
    }

    return res;
}

static u8 osDrvGpioIsOwner(osDrvGpioControl_t controlArray[],
                           u8 index,
                           rtems_id taskId)
{
    if (controlArray[index].takenByTaskId == taskId)
    {
        // taskId is owner of the Gpio
        return 1;
    }
    else
    {
        if (controlArray[index].takenByTaskId == 0)
        {
            // Gpio should be requested before trying to be used
            OS_GPIO_DPRINTF1("Error: Gpio %d should be marked as taken before "
                             "trying to use it. Call OsDrvGpioRequest first\n",
                             index);
        }
        else
        {
            // Gpio taken by another task.
            OS_GPIO_DPRINTF1("Error: Gpio %d already taken by another task with task_id %lx \n",
                             index,
                             controlArray[index].takenByTaskId);
        }
        //error
        return 0;
    }
}

static OS_MYRIAD_DRIVER_STATUS osDrvGpioInitialize(void)
{
    u8 i;
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;

    if (osDrvGpioControlStatus == OS_DRV_STAT_UNINITIALIZED)
    {
        for (i = 0; i < GPIO_NUM_PINS; i++)
        {
            // value 0 will be used to mark GPIO NOT TAKEN
            osDrvGpioControl[i].takenByTaskId = 0;
        }

        // Create one global semaphore to be used by any tasks
        if (rtems_semaphore_create(rtems_build_name('G', 'P', 'I', 'O'), 1,
                                   RTEMS_BINARY_SEMAPHORE, 0,
                                   (rtems_id *) &gpioControlMutex)
            != RTEMS_SUCCESSFUL)
        {
            res = OS_MYR_DRV_ERROR;
        }

        for (i = 0; i < GPIO_PWM_NUM; i++)
        {
            // value 0 will be used to mark GPIO NOT TAKEN
            osDrvGpioPwmControl[i].takenByTaskId = 0;
        }

        // Create one global semaphore to be used by any tasks
        if (rtems_semaphore_create(rtems_build_name('P', 'W', 'M', 'B'),
                                   1, RTEMS_BINARY_SEMAPHORE, 0,
                                   (rtems_id *) &gpioPwmControlMutex)
            != RTEMS_SUCCESSFUL)
        {
            res = OS_MYR_DRV_ERROR;
        }

        osDrvGpioControlStatus = OS_DRV_STAT_INITIALIZED;
    }
    else if (osDrvGpioControlStatus == OS_DRV_STAT_INITIALIZED)
    {
        res = OS_MYR_DRV_ALREADY_INITIALIZED;
    }
    else
    {
        res = OS_MYR_DRV_ERROR;
    }

    OS_GPIO_DPRINTF2("OsDrvGpioInit() res %d\n", res);
    return res;
}

#define MAX_NAME_LENGTH 256

static OS_MYRIAD_DRIVER_STATUS osDrvGpioArrayConfig(
    rtems_device_major_number major,
    const drvGpioConfigRangeType * initArray)
{
    int index = 0;

    if (initArray == NULL)
    {
        return OS_MYR_DRV_ERROR;
    }

    if (osDrvGpioLock(gpioControlMutex, waitTicks) != OS_MYR_DRV_SUCCESS)
    {
        // Unable to obtain semaphore
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    // Initialize GPIOs with default values (board dependent). Every board should have such an default struct
    DrvGpioInitialiseRange(initArray);
    do
    {
        if(initArray[index].name)
        {
            rtems_device_driver status;
            char name[MAX_NAME_LENGTH];

            strcpy(name, GPIO_BASE_NAME);
            strncat(name, initArray[index].name, MAX_NAME_LENGTH - strlen(name) - 1);

            //register with the minor number
            status = rtems_io_register_name(name, major, initArray[index].startGpio);

            if (status != RTEMS_SUCCESSFUL)
            {
                OS_GPIO_DPRINTF2("failed to register %s\n", name);
            }
            else
            {
                OS_GPIO_DPRINTF3("registered %s\n", name);
            }
        }
        index++;
    } while ((initArray[index].action & ACTION_TERMINATE_ARRAY) == 0);

    if (osDrvGpioUnlock(gpioControlMutex) != OS_MYR_DRV_SUCCESS)
    {
        return OS_MYR_DRV_ERROR;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioReadValue(u8 gpioNum,
                                                       gpioVal_t *retVal)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        *retVal = (gpioVal_t) DrvGpioGetPin((u32) gpioNum);
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioWriteValue(u8 gpioNum, gpioVal_t val)
{
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        DrvGpioSetPin(gpioNum, (u32) val);
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioSetMode(u8 gpioNum, u32 mode)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        DrvGpioSetMode((u32) gpioNum, mode);
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioGetMode(u8 gpioNum, u32 *mode)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        *mode = DrvGpioGetMode((u32) gpioNum);
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioGetRaw(u8 gpioNum, u32 *value)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        *value = DrvGpioGetRaw((u32) gpioNum);
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static int gpioToPwmBlockFind(u8 gpio)
{
    s32 pwmBlock = -1;
    s32 i;

    for(i = 0 ; i < (s32)(sizeof(gpioToPwm)/sizeof(gpioToPwmBlock_t)) ; i++)
        if(gpioToPwm[i].gpio == gpio)
        {
            pwmBlock = gpioToPwm[i].pwmBlock;
            break;
        }

    return pwmBlock;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioPwmSet(u8 gpio, pwmSettings_t *pwms)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpio, rtems_task_self()))
    {
        int pwmBlock = gpioToPwmBlockFind(gpio);

        if ((pwmBlock >= 0) && (pwmBlock < NUM_PWM_BLOCKS))
        {
            OS_MYRIAD_DRIVER_STATUS_CODE sc;
            // Request is done in pwmSet function. Release is done in pwmDisable
            sc = osDrvGpioRequest(osDrvGpioPwmControl, pwmBlock,
                                  waitTicks, gpioPwmControlMutex);

            if (sc != OS_MYR_DRV_SUCCESS)
                return sc;

            // Check if pwm values are correct
            if (((pwms->repeat         | D_GPIO_PWM_LEADIN_REPEAT_MASK) != D_GPIO_PWM_LEADIN_REPEAT_MASK) ||
                (((pwms->leadIn<<16)   | D_GPIO_PWM_LEADIN_LEADIN_MASK) != D_GPIO_PWM_LEADIN_LEADIN_MASK) ||
                ((pwms->lowCount       | D_GPIO_PWM_HIGHLOW_LOW_MASK  ) != D_GPIO_PWM_HIGHLOW_LOW_MASK  ) ||
                (((pwms->highCount<<16)| D_GPIO_PWM_HIGHLOW_HIGH_MASK ) != D_GPIO_PWM_HIGHLOW_HIGH_MASK ))
                return OS_MYR_DRV_ERROR;

            DrvGpioSetPwm((u32)pwmBlock, pwms->repeat, pwms->leadIn,
                          pwms->highCount, pwms->lowCount);
        }
        else
        {
            return OS_MYR_DRV_ERROR;
        }
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioPwmDisable(u8 gpio)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpio, rtems_task_self()))
    {
        int pwmBlock = gpioToPwmBlockFind(gpio);

        if ((pwmBlock >= 0) && (pwmBlock < NUM_PWM_BLOCKS))
        {
            OS_MYRIAD_DRIVER_STATUS_CODE sc;

            // Task also owner of PWM?
            if (!osDrvGpioIsOwner(osDrvGpioPwmControl, pwmBlock,
                                  rtems_task_self()))
            {
                return OS_MYR_DRV_RESOURCE_BUSY;
            }

            // Disable
            DrvGpioEnPwm(pwmBlock, 0);

            // Request is done in pwmSet function. Release is done in pwmDisable
            sc = osDrvGpioRelease(osDrvGpioPwmControl, pwmBlock,
                                  rtems_task_self(), gpioPwmControlMutex);
            if (sc != OS_MYR_DRV_SUCCESS)
            {
                return sc;
            }
        }
        else
        {
            return OS_MYR_DRV_ERROR;
        }
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioPwmEnable(u8 gpio)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpio, rtems_task_self()))
    {
        int pwmBlock = gpioToPwmBlockFind(gpio);

        if ((pwmBlock >= 0) && (pwmBlock < NUM_PWM_BLOCKS))
        {
            // task also owner of the pwm block?
            if (!osDrvGpioIsOwner(osDrvGpioPwmControl, pwmBlock,
                                  rtems_task_self()))
            {
                return OS_MYR_DRV_RESOURCE_BUSY;
            }

            //Enable
            DrvGpioEnPwm(pwmBlock, 1);
        }
        else
        {
            return OS_MYR_DRV_ERROR;
        }
    }
    else
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    return OS_MYR_DRV_SUCCESS;
}

//TODO: find a better way
struct gpioIrqMap_t {
    int irqSource;
    void *handler;
    void *context;
};

struct gpioIrqMap_t gpioIrqMap[100];

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioEnableInterrupt(
    u8 gpioNum, gpioInterruptHndl_t *interrupt)
{
    char irqName[] = "IRQ_GPIO_0";
    s32 irqSrc=-1, i;
    int sc;

    // Gpio marked as taken by me(rtems_task_self())?
    if (!osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        return OS_MYR_DRV_RESOURCE_BUSY;
    }

    if (gpioNum > GPIO_NUM_PINS)
    {
        OS_GPIO_DPRINTF1("Error: Invalid GPIO Number. Max Gpio Num is %d\n",
                         GPIO_NUM_PINS);
        return OS_MYR_DRV_ERROR;
    }

    // Check if GPIO pin is already attached to an IRQ source.
    if (gpioIrqMap[gpioNum].irqSource != 0) {
        OS_GPIO_DPRINTF1(
            "Error: GPIO pin is already triggering an interrupt \n");
        return OS_MYR_DRV_ERROR;
    }

    //Check if handler was set already for irq source
    for(i = 0 ; i < 100 ; i++)
    {
        if(gpioIrqMap[i].handler == interrupt->handler)
        {
            int irqSrc = gpioIrqMap[i].irqSource - IRQ_GPIO_0;
            printf("adding GPIO %d to handler %p\n", gpioNum, interrupt->handler);
            if (DrvGpioIrqJustConfigAnyGpio(gpioNum, irqSrc) != 0)
            {
                OS_GPIO_DPRINTF1("Error: Unable to configure gpio to trigger interrupt \n");
                return OS_MYR_DRV_ERROR;
            }
            else
            {
                return RTEMS_SUCCESSFUL;
            }
        }
    }

    // Check if irqSrc has at least one triggering GPIO available
    // Avoid GPIO bug (only one GPIO can trigger an interrupt on a irqSrc
    for(i = 0; (i < GPIO_MAX_IRQ_SRCS) && (irqSrc<0); i++)
        if(DrvGpioIrqSrcNotUsed(i))
            irqSrc = i;

    if (irqSrc < 0)
    {
        OS_GPIO_DPRINTF1("Error: All GPIO IRQ sources taken\n");
        return OS_MYR_DRV_ERROR;
    }

    if (DrvGpioIrqJustConfigAnyGpio(gpioNum, irqSrc) != 0)
    {
        OS_GPIO_DPRINTF1("Error: Unable to configure gpio to trigger interrupt \n");
        return OS_MYR_DRV_ERROR;
    }

    // Calculate the IRQ name
    irqName[strlen(irqName) - 1] = '0' + irqSrc;

    BSP_Clear_interrupt(IRQ_GPIO_0 + irqSrc);
    BSP_Set_interrupt_type_priority(IRQ_GPIO_0 + irqSrc, interrupt->type,
                                    interrupt->priority);

    sc = rtems_interrupt_handler_install(IRQ_GPIO_0 + irqSrc,
                                         irqName,
                                         RTEMS_INTERRUPT_UNIQUE,
                                         interrupt->handler,
                                         (void*)interrupt->context);
    if(sc)
        return sc;

    gpioIrqMap[gpioNum].irqSource = IRQ_GPIO_0 + irqSrc;
    gpioIrqMap[gpioNum].handler = interrupt->handler;
    gpioIrqMap[gpioNum].context = interrupt->context;

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioDisableInterrupt(u8 gpioNum)
{
    int sc;

    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        sc = rtems_interrupt_handler_remove(gpioIrqMap[gpioNum].irqSource,
                                            gpioIrqMap[gpioNum].handler,
                                            gpioIrqMap[gpioNum].context);
        if(sc)
        {
            OS_GPIO_DPRINTF1("unable to unregister \n");
        }

        if (DrvGpioIrqDisableAny(gpioNum) != 0)
        {
            OS_GPIO_DPRINTF1("Error: Gpio %d was not configured "
                             "to trigger an interrupt\n",
                             gpioNum);
            return OS_MYR_DRV_ERROR;
        }

        memset(&gpioIrqMap[gpioNum], 0, sizeof(gpioIrqMap[gpioNum]));

        return OS_MYR_DRV_SUCCESS;
    }
    return OS_MYR_DRV_RESOURCE_BUSY;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioDisableSpecificInterrupt(u8 gpioNum, u32 irqSrc)
{
    // Gpio marked as taken by me(rtems_task_self())?
    if (osDrvGpioIsOwner(osDrvGpioControl, gpioNum, rtems_task_self()))
    {
        if (DrvGpioIrqDisable(gpioNum, irqSrc) != 0)
        {
            OS_GPIO_DPRINTF1("Error: Gpio %d was not configured "
                             "to trigger an interrupt\n",
                             gpioNum);
            return OS_MYR_DRV_ERROR;
        }
        return OS_MYR_DRV_SUCCESS;
    }
    return OS_MYR_DRV_RESOURCE_BUSY;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioTimerSetConfig(
    u8 gpioNum, DrvGpioTriggerType *trigger)
{
    // Check args.
    if (gpioNum >= GPIO_NUM_PINS) {
        OS_GPIO_DPRINTF1("Error: Invalid GPIO Number. Max Gpio Num is %d\n",
                         GPIO_NUM_PINS);
        return OS_MYR_DRV_ERROR;
    }
    if (NULL == trigger) {
        OS_GPIO_DPRINTF1("Error: trigger type not provided\r\n");
        return OS_MYR_DRV_ERROR;
    }

    u32 irq_source;

    // Check that the pin is assigned to an interrupt.
    if (0 == gpioIrqMap[gpioNum].irqSource) {
        // Pin is not attached to interrupt.
        OS_GPIO_DPRINTF1(
            "Error: Gpio %d was not configured to trigger an interrupt\n",
            gpioNum);
        return OS_MYR_DRV_ERROR;
    }

    // Get the IRQ source which the GPIO pin is driving.
    irq_source = gpioIrqMap[gpioNum].irqSource - IRQ_GPIO_0;

    // irq_source directly maps to timestamp_num.
    if (DrvGpioTimerSetConfig(irq_source, *trigger) != 0) {
        OS_GPIO_DPRINTF1("Error: Unable to set GPIO Timer config\n");
        return OS_MYR_DRV_ERROR;
    }

    return OS_MYR_DRV_SUCCESS;
}

static OS_MYRIAD_DRIVER_STATUS_CODE osDrvGpioTimerGet(u8 gpioNum,
                                                      u64 *timestamp)
{
    // Check args.
    if (gpioNum >= GPIO_NUM_PINS) {
        OS_GPIO_DPRINTF1("Error: Invalid GPIO Number. Max Gpio Num is %d\n",
                         GPIO_NUM_PINS);
        return OS_MYR_DRV_ERROR;
    }
    if (NULL == timestamp) {
        OS_GPIO_DPRINTF1("Error: NULL pointer\r\n");
        return OS_MYR_DRV_ERROR;
    }

    u32 irq_source;

    // Check that the pin is assigned to an interrupt.
    if (0 == gpioIrqMap[gpioNum].irqSource) {
        // Pin is not attached to interrupt.
        OS_GPIO_DPRINTF1(
            "Error: Gpio %d was not configured to trigger an interrupt\n",
            gpioNum);
        return OS_MYR_DRV_ERROR;
    }

    // Get the IRQ source which the GPIO pin is driving.
    irq_source = gpioIrqMap[gpioNum].irqSource - IRQ_GPIO_0;

    // irq_source directly maps to timestamp_num.
    if (DrvGpioTimerGet(irq_source, timestamp) != 0) {
        OS_GPIO_DPRINTF1("Error: Unable to set GPIO Timer config\n");
        return OS_MYR_DRV_ERROR;
    }

    return OS_MYR_DRV_SUCCESS;
}

rtems_status_code gpio_block_initialize(rtems_device_major_number major,
                                        rtems_device_minor_number minor,
                                        void *e)
{
    UNUSED(minor);// hush the compiler warning.

    // Call from register driver function
    if (e == NULL)
    {
        return osDrvGpioInitialize();
    }
    else
    {
        rtems_device_driver status;

        //register with the minor number from virtual channel
        status = rtems_io_register_name(OSDRVGPIO_DEVNAME,
                                        major, 0);

        if (status != RTEMS_SUCCESSFUL)
        {
            return status;
        }

        // Disable irqSrc. They are enabled by default
        DrvGpioIrqSrcDisable(0);
        DrvGpioIrqSrcDisable(1);
        DrvGpioIrqSrcDisable(2);
        DrvGpioIrqSrcDisable(3);

        return osDrvGpioArrayConfig(major, (const drvGpioConfigRangeType *) e);
    }
}

rtems_device_driver gpio_block_open(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    void *e)
{
    UNUSED(major);// hush the compiler warning.

    u32 wt = waitTicks;
    u32 gpioNum = (u32) minor;
    rtems_libio_rw_args_t *rwargs = (rtems_libio_rw_args_t*) e;

    if (rwargs)
    {
        wt = rwargs->flags & LIBIO_FLAGS_NO_DELAY ? 0 : waitTicks;
    }

    return osDrvGpioRequest(osDrvGpioControl, gpioNum, wt, gpioControlMutex);
}

rtems_device_driver gpio_block_close(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *e)
{
    UNUSED(major);// hush the compiler warning.

    u32 wt = waitTicks;
    u32 gpioNum = (u32) minor;
    rtems_libio_rw_args_t *rwargs = (rtems_libio_rw_args_t*) e;

    if (rwargs)
    {
        wt = (rwargs->flags & LIBIO_FLAGS_NO_DELAY) ? 0 : waitTicks;
    }

    return osDrvGpioRelease(osDrvGpioControl, gpioNum, wt, gpioControlMutex);
}

rtems_device_driver gpio_block_read(rtems_device_major_number major,
                                    rtems_device_minor_number minor,
                                    void *e)
{
    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    UNUSED(major);// hush the compiler warning.

    u32 gpioNum = (u32) minor;

    if (!e || !rw_args->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    return osDrvGpioReadValue(gpioNum, (gpioVal_t*)rw_args->buffer);
}

rtems_device_driver gpio_block_write(rtems_device_major_number major,
                                     rtems_device_minor_number minor,
                                     void *e)
{
    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    UNUSED(major);// hush the compiler warning.

    u32 gpioNum = (u32) minor;

    if (!e || !rw_args->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    return osDrvGpioWriteValue(gpioNum, *(gpioVal_t*) rw_args->buffer);
}

rtems_device_driver gpio_block_control(rtems_device_major_number major,
                                       rtems_device_minor_number minor,
                                       void *e)
{
    OS_MYRIAD_DRIVER_STATUS_CODE res = OS_MYR_DRV_SUCCESS;
    UNUSED(major);// hush the compiler warning.

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    u32 gpioNum = (u32) minor;

    if (!ctl || (!ctl->buffer && ctl->command != GPIO_ENABLE_PWM &&
        ctl->command != GPIO_DISABLE_PWM && ctl->command != GPIO_IRQ_DISABLE))
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case GPIO_ARRAY_CONFIG:
        res=osDrvGpioArrayConfig(major, (const drvGpioConfigRangeType *) ctl->buffer);
        break;
    case GPIO_SET_MODE:
        res=osDrvGpioSetMode(gpioNum, *(u32*) ctl->buffer);
        break;
    case GPIO_GET_MODE:
        res=osDrvGpioGetMode(gpioNum, ctl->buffer);
        break;
    case GPIO_CONFIGURE_AND_ENABLE_PWM:
        res=osDrvGpioPwmSet(gpioNum, (pwmSettings_t*) ctl->buffer);
        if (res == OS_MYR_DRV_SUCCESS)
            osDrvGpioPwmEnable(gpioNum);
        break;
    case GPIO_CONFIGURE_PWM:
        res=osDrvGpioPwmSet(gpioNum, (pwmSettings_t*) ctl->buffer);
        break;
    case GPIO_ENABLE_PWM:
        res=osDrvGpioPwmEnable(gpioNum);
        break;
    case GPIO_DISABLE_PWM:
        res=osDrvGpioPwmDisable(gpioNum);
        break;
    case GPIO_IRQ_ENABLE:
        res=osDrvGpioEnableInterrupt(gpioNum, (gpioInterruptHndl_t *) ctl->buffer);
        break;
    case GPIO_IRQ_DISABLE:
        res=osDrvGpioDisableInterrupt(gpioNum);
        break;
    case GPIO_IRQ_DISABLE_SPECIFIC:
        res=osDrvGpioDisableSpecificInterrupt(gpioNum, *(u32*) ctl->buffer);
        break;
    case GPIO_DEFAULT_TIMEOUT:
        waitTicks = *(u32*) ctl->buffer;
        res = RTEMS_SUCCESSFUL;
        break;
    case GPIO_GET_RAW:
        res=osDrvGpioGetRaw(gpioNum, ctl->buffer);
        break;
    case GPIO_TIMER_SET_CONFIG:
        res = osDrvGpioTimerSetConfig(gpioNum, ctl->buffer);
        break;
    case GPIO_TIMER_GET:
        res = osDrvGpioTimerGet(gpioNum, ctl->buffer);
        break;
    default:
        OS_GPIO_DPRINTF1("Error: Invalid command %ld\n", ctl->command);
        return RTEMS_UNSATISFIED;
    }
    if (res == OS_MYR_DRV_SUCCESS)
    {
        ctl->ioctl_return = 0;
    }
    else
    {
        ctl->ioctl_return = -1;
    }
    return res;
}

rtems_driver_address_table osDrvGpioTblName =                   \
{                                                               \
    gpio_block_initialize,                                      \
    gpio_block_open,                                    \
    gpio_block_close,                                   \
    gpio_block_read,                                    \
    gpio_block_write,                                   \
    gpio_block_control                                  \
};

///@}
