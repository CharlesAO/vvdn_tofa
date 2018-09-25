/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Example of usage for the Power Manager component
///
///            If tested on mv182/mv212 board, the wake up from low power need the DIP SWITCH8 switched on (UP position)

#include <stdio.h>
#include <rtems.h>
#include <fcntl.h>

#ifndef MA2X8X
#include <OsDrvCpr.h>
#endif
#include <pwr_manager.h>
#include "app_config.h"

static rtems_id init_task_id;
static rtems_id protected_print;

static uint32_t cb_request_count[NUMTASKS] = {0};
static uint32_t cb_granted_count[NUMTASKS] = {0};
static uint32_t cb_cancel_count[NUMTASKS]  = {0};
static uint32_t cb_exit_count[NUMTASKS]    = {0};

#ifdef MA2X8X
LowPowerInfo lp_gpio_32khz          = {{LP_WAKEUP_SOURCE_GPIO, {{GPIO_ID}}}, {LP_SRC_32KHZ_CLK}};
LowPowerInfo lp_rtc_32khz           = {{LP_WAKEUP_SOURCE_RTC,  {{5000}}},    {LP_SRC_32KHZ_CLK}};
#else
LowPowerInfo lp_gpio_32khz          = {{LP_WAKEUP_SOURCE_GPIO, {{GPIO_ID}}}, {LP_SRC_32KHZ_CLK, {{1, 1 }}}};
#endif

static PwrManagerIoctlPowerState pstate;
static uint32_t pwr_mngr_major;

//-----------------------------------------------
//Callback functions (common code for multiple tasks callbacks)
static rtems_status_code cbRequest(void *param) {
    uint32_t minor = (uint32_t)param;

    DPRINTK("called cbRequest\n");

    //the request callback can perform specific peripheral stop/reset into a clean way;
    //if the current status of the task/HW, ... is not right for entering into low power, it can reject the enter LP by any OS error code
    if ((cb_request_count[minor]++) > 0) //fail the 1st request call of every device
        return RTEMS_SUCCESSFUL;
    else
        return RTEMS_IO_ERROR;
}

static rtems_status_code cbCancel(void *param) {
    uint32_t minor = (uint32_t)param;

    DPRINTK("called cbCancel\n");

    //the cancel callback can restore specific peripheral to a working state
    cb_cancel_count[minor]++;

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cbGranted(void *param) {
    uint32_t minor = (uint32_t)param;
    static uint32_t total_count = 0;

    DPRINTK("called cbGranted\n");

    //the granted callback can perform last minute shutdown actions for specific HW
    //(treatment sharing can be foreseen between this cb and the request cb, in order to speed up the enter lp)
    cb_granted_count[minor]++;

    if ((total_count++) >= 2)
    {
        DPRINTK("\n The system will now enter into the configured low power mode;\n");
#ifdef MA2X8X
        DPRINTK(" The system will automatically wake up in 5 seconds\n\n");
#else
        DPRINTK(" to wakeup the system, set the DIP SWITCH8 to ON (UP position)\n\n");
#endif
    }

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cbExit(void *param) {
    uint32_t minor = (uint32_t)param;

    DPRINTK("called cbExit\n");

    //the exit callback can restore specific peripheral to a working state
    cb_exit_count[minor]++;

    return RTEMS_SUCCESSFUL;
}

// Tasks (common code for multiple tasks)
static rtems_task lpMultiTask(rtems_task_argument arg) {

    PwrManagerCallbacksTable cb_array = { &cbRequest, &cbCancel, &cbGranted, &cbExit };
    PwrManagerIoctlCallback  current_task_cb;
    int32_t                  current_task_fd;
    uint32_t                 curent_task_id = (uint32_t)(arg);

    DPRINTF(" Task%ld started\n", curent_task_id);

    //open an instance of the component for the current task
    current_task_fd = open(OSDRVPWRMANAGER_DEVNAME, O_RDONLY, 0);
    if (current_task_fd < 0)
        DPRINTF("Error while opening the power manager component for task%ld: %ld \n", curent_task_id, current_task_fd);

    current_task_cb.callbacks = cb_array;
    current_task_cb.callback_argument = (void *)curent_task_id;

    int32_t ret = ioctl(current_task_fd, PWR_MANAGER_ADD_NOTIFICATION_CALLBACKS, &current_task_cb.callbacks);
    if (ret !=0 )
        DPRINTF("Error when adding notification callbacks for the task%ld \n", curent_task_id);

    //free the processor; when all the tasks will release the processor, the low power mode will be triggered
    sleep(1);

    DPRINTF(" Task%ld finished\n", curent_task_id);

    ret = close(current_task_fd);
    if (ret !=0 )
        DPRINTF("Error when closing the task%ld \n", curent_task_id);

    rtems_event_send(init_task_id, TASK(curent_task_id));
    rtems_task_delete(RTEMS_SELF);
}

//test
void startLowPowerManagementTask(void)
{
    rtems_id                  task_id[NUMTASKS];
    rtems_name                task_name[NUMTASKS];
    uint32_t                  events_out;
    uint32_t                  i;
    int32_t                   main_fd;

    DPRINTF(" *** Test entering low power with a multitasking application\n");

    //open an instance of the component for the management task; this task won't have any callbacks registered
    main_fd = open(OSDRVPWRMANAGER_DEVNAME, O_RDONLY, 0);
    if (main_fd < 0)
        DPRINTF("Error while opening the power manager component: %ld \n", main_fd);

    //request entering LP into the configured state idx
    //the system will not immediately enter into low power, but only after all the running tasks will release the processor
    //(the execution of the low power switch will be handled by the RTEMS idle task)
    pstate.mode_idx = LP_GPIO_32KHZ_STATE_IDX;
    int32_t ret = ioctl(main_fd, PWR_MANAGER_SWITCH_POWER_STATE, &pstate);
    if (ret !=0 )
        DPRINTF("Error when requesting the switch to low power mode !\n");

    ret = close(main_fd);
    if (ret !=0 )
        DPRINTF("Error when closing the main task \n");

    //create some concurrent tasks which will all need to agree in order that the system goes into low power
    for (i=0;i<NUMTASKS;i++) {
        task_name[i] = rtems_build_name('T','S','K','0'+i);

        if (RTEMS_SUCCESSFUL != rtems_task_create(task_name[i], 1, RTEMS_MINIMUM_STACK_SIZE,
                                    RTEMS_PREEMPT | RTEMS_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(0),
                                    RTEMS_DEFAULT_ATTRIBUTES, &task_id[i]))
            DPRINTF("Error while creating the tasks\n");
    }

    //start the tasks
    for (i=0;i<NUMTASKS;i++)
        if (RTEMS_SUCCESSFUL != rtems_task_start(task_id[i], lpMultiTask, i))
            DPRINTF("Error while starting the tasks\n");

    //wait for the end of the test
    for (i=0;i<NUMTASKS;i++) {
        if (RTEMS_SUCCESSFUL != rtems_event_receive(TASK(i), RTEMS_WAIT|RTEMS_EVENT_ALL, RTEMS_NO_TIMEOUT, &events_out))
            DPRINTF("Error while synchronizing between tasks\n");
    }

    //check the results
    //some of the request callbacks indicate that the associated task is not able to enter into low power, so the component
    //triggered cancel functions calling for the functions which already confirmed the availability;
    //the request functions are repeatidly called (by the RTEMS idle task) until they all confirm availability;
    //once confirmed, all granted callbacks are called, followed by the exit callbacks when exiting low power
    uint32_t expected_request_count[NUMTASKS] = {4,3,2};
    uint32_t expected_cancel_count[NUMTASKS]  = {2,1,0};
    uint32_t expected_granted_count[NUMTASKS] = {1,1,1};
    uint32_t expected_exit_count[NUMTASKS]    = {1,1,1};

    for(i=0;i<NUMTASKS;i++) {
        if ( (cb_request_count[i] != expected_request_count[i]) ||
             (cb_cancel_count[i]  != expected_cancel_count[i])  ||
             (cb_granted_count[i] != expected_granted_count[i]) ||
             (cb_exit_count[i]    != expected_exit_count[i]) )
            DPRINTF("Error playing the test: one of the callbacks was not properly called !\n");
    }
}

// main function
//---------------------------------------------
rtems_task Init(rtems_task_argument argumentx) {
    UNUSED(argumentx);
    rtems_status_code sc;

    PwrManagerPowerStates valid_power_states[2] = {
            {.mode = POWER_MANAGER_MODE_LOW_POWER,
#ifdef MA2X8X
             .low_power_data = lp_rtc_32khz
#else
             .low_power_data = lp_gpio_32khz
#endif
            },
            {0}  //NULL ended list of valid low power states; max 16 entries (null  included)
    };

#ifndef MA2X8X
    OS_MYRIAD_DRIVER_STATUS_CODE res = OsDrvCprInit();
    if (res == OS_MYR_DRV_SUCCESS ||
        res == OS_MYR_DRV_ALREADY_INITIALIZED ||
        res == OS_MYR_DRV_ALREADY_OPENED) {
        if (res != OS_MYR_DRV_ALREADY_OPENED)
            OsDrvCprOpen();
    }
    else
        DPRINTF(" Cpr driver initialization failed \n");
#endif

    //create a semaphore to protect the concurrent printing
    if (rtems_semaphore_create(rtems_build_name( 'S', 'E', 'M', 'P' ), 1, RTEMS_DEFAULT_ATTRIBUTES, 0, &protected_print))
        DPRINTF("Prints will not be protected \n\n");

    init_task_id = rtems_task_self();

    //PwrManager component registration
    sc = rtems_io_register_driver( 0, &osDrvPwrManagerTbl, &pwr_mngr_major );
    if (sc != RTEMS_SUCCESSFUL )
        DPRINTF("Error while registering the power manager component: %u \n", sc);

    //initialize the component with the allowed states
    sc = rtems_io_initialize(pwr_mngr_major, 0, valid_power_states);
    if (sc != RTEMS_SUCCESSFUL )
        DPRINTF("Error while initializing the power manager component: %u \n", sc);

    //perform the power switch switch test
    startLowPowerManagementTask();

    DPRINTF("\n Test finished\n");

    rtems_shutdown_executive(0);
}


