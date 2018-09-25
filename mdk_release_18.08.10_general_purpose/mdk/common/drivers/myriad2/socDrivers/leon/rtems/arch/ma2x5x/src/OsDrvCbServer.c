///
/// @file OsDrvCbServer.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvCbServer
/// @{
/// @brief      Callback server implementation
///
///             The purpose of this driver is to execute centrally callbacks.
///             Drivers and other components can offload callbacks out of their critical path, to this driver.
///             The API of this driver supports the classical and POSIX APIs of RTEMS for maximum compatibility.
///             The following commands are implemented as io control commands:
///             CB_SERVER_CALLBACK_REQUEST  - Non blocking command that enqueues a request to the CALLBACK service
///             CB_SERVER_BARRIER           - Blocking command that blocks the caller thread until previous CALLBACK request is executed
///             CB_SERVER_SUSPEND           - Blocking command that blocks the caller thread until all CALLBACK requests are executed and then suspends the service
///             CB_SERVER_RESUME            - Non blocking command that resumes a suspended service
///             CB_SERVER_GET_STATE         - Non blocking command that returns the current state of the service
///


// 1: Includes
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <assert.h>
#include <rtems/libio.h>
#include <rtems/status-checks.h>

#include "OsCommon.h"
#include "OsDrvCbServer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define OS_DRV_CB_SERVER_L1   //Errors
//#define OS_DRV_CB_SERVER_L2   //Warnings
//#define OS_DRV_CB_SERVER_L3   //Debug prints

#ifdef OS_DRV_CB_SERVER_L1
#define OS_CB_SERVER_DPRINTF1(...)  printf(__VA_ARGS__)
#else
#define OS_CB_SERVER_DPRINTF1(...)
#endif

#ifdef OS_DRV_CB_SERVER_L2
#define OS_CB_SERVER_DPRINTF2(...)  printf(__VA_ARGS__)
#else
#define OS_CB_SERVER_DPRINTF2(...)
#endif

#ifdef OS_DRV_CB_SERVER_L3
#define OS_CB_SERVER_DPRINTF3(...)  printf(__VA_ARGS__)
#else
#define OS_CB_SERVER_DPRINTF3(...)
#endif

#if defined(OS_DRV_CB_SERVER_L1) || \
    defined(OS_DRV_CB_SERVER_L2) || \
    defined(OS_DRV_CB_SERVER_L3)
#define OS_DRV_CB_SERVER_DEBUG 1
#else
#define OS_DRV_CB_SERVER_DEBUG 0
#endif

ISR_LOCK_DEFINE(static, cbServer_lock, "CallBackServer Lock")

typedef struct {
    volatile CbServerStates state;          // Server state
    rtems_id                callback_queue; // Message Queue for the callback requests
    rtems_id                suspend_mutex;  // Mutex for the suspend/barrier requests
    rtems_id                task;           // task id for the callback thread
} CbServerData;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static CbServerData *cbServer = NULL;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

static void cbServerExecuteSuspend(void * unused) {
    UNUSED(unused);
    cbServer->state = CB_SERVER_SUSPENDED;
    OS_CB_SERVER_DPRINTF3("CB_SERVER SUSPEND Signal\n");
    rtems_semaphore_release(cbServer->suspend_mutex);
}

static void cbServerExecuteBarrier(void * unused) {
    UNUSED(unused);
    OS_CB_SERVER_DPRINTF3("CB_SERVER_BARRIER Signal\n");
    rtems_semaphore_release(cbServer->suspend_mutex);
}

static rtems_task cbServerFunction(rtems_task_argument argument) {
    CbServerCbRequest cb_req;
    size_t cb_size;
    rtems_status_code status;

    UNUSED(argument);
    assert(cbServer);
    assert(cbServer->state == CB_SERVER_INITIALIZED ||
           cbServer->state == CB_SERVER_SUSPEND_REQUESTED);

    if (cbServer->state == CB_SERVER_INITIALIZED)
        cbServer->state = CB_SERVER_STARTED;

    while (1) {
        status = rtems_message_queue_receive(cbServer->callback_queue,
                                             &cb_req,
                                             &cb_size,
                                             RTEMS_WAIT,
                                             RTEMS_NO_TIMEOUT);
        if (status != RTEMS_SUCCESSFUL) {
            OS_CB_SERVER_DPRINTF1("rtems_message_queue_receive failed with %d\n",status);
            goto exit;
        }

        if (cb_size!=sizeof(CbServerCbRequest)) {
            OS_CB_SERVER_DPRINTF1("%s dropping data with invalid size %u\n", __func__, cb_size);
            continue;
        }

        OS_CB_SERVER_DPRINTF3("%s calling %p %p\n", __func__, cb_req.callback, cb_req.argument);
        cb_req.callback(cb_req.argument);
    }

    exit:
    cbServer->state = CB_SERVER_EXITED_ON_ERROR;
    rtems_task_delete(RTEMS_SELF);
}

static rtems_status_code cbServerCallbackRequest(CbServerCbRequest *cb_req) {
    rtems_status_code ret;
    if (cbServer->state != CB_SERVER_STARTED &&
        cbServer->state != CB_SERVER_INITIALIZED)
        return RTEMS_ALREADY_SUSPENDED;
    if (cb_req == NULL || cb_req->callback == NULL)
        return RTEMS_INVALID_ADDRESS;

    OS_CB_SERVER_DPRINTF3("CB_SERVER_CALLBACK_REQUEST\n");
    ret = rtems_message_queue_send(cbServer->callback_queue,
                                   (void *)cb_req,
                                   sizeof(CbServerCbRequest));
    OS_CB_SERVER_DPRINTF3("CB_SERVER_CALLBACK_REQUEST finished %u\n", ret);

    return ret;
}

static rtems_status_code cbServerBarrier(void) {
    rtems_status_code ret;
    CbServerCbRequest cb_req = { &cbServerExecuteBarrier, NULL};
    OS_CB_SERVER_DPRINTF3("CB_SERVER_BARRIER Start\n");
    ret = rtems_message_queue_send(cbServer->callback_queue,
                                   (void *)&cb_req,
                                   sizeof(CbServerCbRequest));
    if (ret != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_message_queue_send failed with %u\n", ret);
        return ret;
    }

    ret = rtems_semaphore_obtain(cbServer->suspend_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    OS_CB_SERVER_DPRINTF3("CB_SERVER_BARRIER finished %u\n", ret);
    return ret;

}

static rtems_status_code cbServerSuspendRequest(void) {

    rtems_status_code ret;
    rtems_interrupt_lock_context lcontext;

    OS_CB_SERVER_DPRINTF3("CB_SERVER SUSPEND Start\n");

    rtems_interrupt_lock_acquire(&cbServer_lock, &lcontext);
    if (cbServer->state != CB_SERVER_INITIALIZED &&
        cbServer->state != CB_SERVER_STARTED) {
        ret = RTEMS_UNSATISFIED;
        rtems_interrupt_lock_release(&cbServer_lock, &lcontext);
        goto out;
    }

    cbServer->state = CB_SERVER_SUSPEND_REQUESTED;
    rtems_interrupt_lock_release(&cbServer_lock, &lcontext);

    CbServerCbRequest cb_req = { &cbServerExecuteSuspend, NULL};
    ret = rtems_message_queue_send(cbServer->callback_queue,
                                   (void *)&cb_req,
                                   sizeof(CbServerCbRequest));
    if (ret != RTEMS_SUCCESSFUL)  {
        OS_CB_SERVER_DPRINTF1("rtems_message_queue_send failed with %u\n", ret);
        goto out;
    }

    ret = rtems_semaphore_obtain(cbServer->suspend_mutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

out:
    OS_CB_SERVER_DPRINTF3("CB_SERVER_SUSPEND finished %u\n", ret);
    return ret;
}

static rtems_status_code cbServerResume(void) {
    rtems_interrupt_lock_context lcontext;
    OS_CB_SERVER_DPRINTF3("CB_SERVER_RESUME\n");

    rtems_interrupt_lock_acquire(&cbServer_lock, &lcontext);
    if (cbServer->state != CB_SERVER_SUSPENDED) {
        rtems_interrupt_lock_release(&cbServer_lock, &lcontext);
        return RTEMS_UNSATISFIED;
    }

    cbServer->state = CB_SERVER_STARTED;
    rtems_interrupt_lock_release(&cbServer_lock, &lcontext);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code cbServerGetState(CbServerState* st) {
    OS_CB_SERVER_DPRINTF3("CB_SERVER_GET_STATE\n");
    if (st) {
        st->state = cbServer->state;
        return RTEMS_SUCCESSFUL;
    }
    else
        return RTEMS_INVALID_ADDRESS;
}

///////////////////////////////////////////////////////////////////////////////

//power manager registration
static rtems_status_code registerDevice(rtems_device_major_number major,
                                        rtems_device_minor_number minor) {
    rtems_status_code status;
    OS_CB_SERVER_DPRINTF3("%s() called with arguments: %lu %lu\n", __func__, major, minor);

    assert(cbServer == NULL);

    cbServer = malloc(sizeof(CbServerData));
    if (cbServer == NULL) {
        OS_CB_SERVER_DPRINTF1("%s() unable to allocate data\n", __func__);
        return RTEMS_NO_MEMORY;
    }

    memset(cbServer, 0, sizeof(CbServerData));

    //Set state to it's initial value
    cbServer->state = CB_SERVER_UNINITIALIZED;

    status = rtems_semaphore_create(rtems_build_name('C', 'B', 'M', 'T'),
                                    0,
                                    RTEMS_SIMPLE_BINARY_SEMAPHORE,
                                    RTEMS_DEFAULT_ATTRIBUTES,
                                    &cbServer->suspend_mutex);
    if (status != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_semaphore_create failed with %d \n",status);
        goto error_mutex;
    }

    status = rtems_message_queue_create(rtems_build_name('C', 'B', 'S', 'Q'),
                                        CB_SERVER_MAX_QUEUE_SIZE,
                                        sizeof(CbServerCbRequest),
                                        RTEMS_FIFO,
                                        &(cbServer->callback_queue));
    if (status != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_message_queue_create failed with %d \n",status);
        goto error_queue;
    }

    status = rtems_task_create(rtems_build_name('C', 'B', 'T', 'H'),
                                PRIORITY_PSEUDO_ISR,
                                RTEMS_MINIMUM_STACK_SIZE,
                                RTEMS_NO_PREEMPT,
                                RTEMS_SYSTEM_TASK,
                                &(cbServer->task) );
    if (status != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_task_create failed with %d \n",status);
        goto error_task;
    }

    //Initialize state
    cbServer->state = CB_SERVER_INITIALIZED;

    status = rtems_task_start(cbServer->task, &cbServerFunction, (rtems_task_argument)NULL);
    if (status != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_task_start failed with %d \n",status);
        goto error_task_start;
    }

    //register with the minor number from virtual channel
    status = rtems_io_register_name(OS_DRV_CB_SERVER_DEV_NAME, major, minor);
    if (status != RTEMS_SUCCESSFUL) {
        OS_CB_SERVER_DPRINTF1("rtems_io_register_name failed with %d \n",status);
        goto error_io_name;
    }

    return RTEMS_SUCCESSFUL;

    error_io_name:
        cbServer->state = CB_SERVER_UNINITIALIZED;
    error_task_start:
        rtems_task_delete(cbServer->task);
    error_task:
        rtems_message_queue_delete(cbServer->callback_queue);
    error_queue:
        rtems_semaphore_delete(cbServer->suspend_mutex);
    error_mutex:
        free(cbServer);
        cbServer=NULL;
        return status;
}

//rtems_io_control implementation
static rtems_status_code cbServerControl(rtems_device_major_number major,
                                           rtems_device_minor_number minor,
                                           void *e) {
    rtems_status_code ret = RTEMS_SUCCESSFUL;
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;

    assert (cbServer != NULL);

    if (!ctl)
        return RTEMS_INVALID_ADDRESS;

    if (ret != RTEMS_SUCCESSFUL)
        return ret;

    switch (ctl->command) {
        case CB_SERVER_CALLBACK_REQUEST:
            ret = cbServerCallbackRequest((CbServerCbRequest *)ctl->buffer);
            break;
        case CB_SERVER_BARRIER:
            ret = cbServerBarrier();
            break;
        case CB_SERVER_SUSPEND:
            ret = cbServerSuspendRequest();
            break;
        case CB_SERVER_RESUME:
            ret = cbServerResume();
            break;
        case CB_SERVER_GET_STATE:
            ret = cbServerGetState((CbServerState *)ctl->buffer);
            break;
        default:
            ret = RTEMS_UNSATISFIED;
    }

    ctl->ioctl_return = ret;
    return ret;
}

//handles driver registration and initialization
static rtems_status_code cbServerInitialize(rtems_device_major_number major,
                                            rtems_device_minor_number minor,
                                            void *e) {
    if (e) { // rtems_io_initialize
        return RTEMS_UNSATISFIED;
    } else { //rtems_io_register_driver
        if (cbServer == NULL)
            return registerDevice(major, minor);
        else
            return RTEMS_INTERNAL_ERROR;
    }
}

//rtems_io_open implementation
//open one client storage area within power manager
static rtems_status_code cbServerOpen(rtems_device_major_number major,
                                      rtems_device_minor_number minor,
                                      void *e) {
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    if (cbServer == NULL)
        return RTEMS_NOT_CONFIGURED;

    OS_CB_SERVER_DPRINTF3("%s() called \n", __func__);

    return RTEMS_SUCCESSFUL;
}

//rtems_io_close implementation
//close the current client storage area within power manager
static rtems_status_code cbServerClose(rtems_device_major_number major,
                                         rtems_device_minor_number minor,
                                         void *e) {
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    if (cbServer == NULL)
        return RTEMS_NOT_CONFIGURED;

    OS_CB_SERVER_DPRINTF3("%s() called \n", __func__);

    return RTEMS_SUCCESSFUL;
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------

rtems_driver_address_table osDrvCbServerTbl = \
{                           \
    cbServerInitialize,     \
    cbServerOpen,           \
    cbServerClose,          \
    NULL,                   \
    NULL,                   \
    cbServerControl         \
};


///@}
