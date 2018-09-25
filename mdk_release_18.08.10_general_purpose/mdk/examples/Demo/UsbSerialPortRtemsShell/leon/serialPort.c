///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Home for VideoDemo_MjpegInit()
///

// 1: Includes
// ----------------------------------------------------------------------------

#include "udevice.h"
#include "uplatformapi.h"
#include "usbpumpapi.h"
#include "usbpump_usbseri.h"
#include "usbseri_oslinkage.h"
#include "usbpump_application_rtems_api.h"
#include "usbpump_rtems.h"
#include "usbpumpobjectapi.h"

#include "bsp.h"
#include "rtems/bspIo.h"
#include "rtems.h"
#include "serialPort.h"
#include <termios.h>
#include <rtems/termiostypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "usbserifc.h"
#include <rtems/shell.h>
#include "mv_types.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define START_SHELL_EVENT RTEMS_EVENT_31
#define START_PLANE_SERIAL_EVENT 1
#define DTR_ON_SERIAL_EVENT      2
#define DTR_OFF_SERIAL_EVENT     3

typedef struct {
    rtems_id readSem;
    rtems_id writeSem;
    size_t readSize;
    size_t writeSize;
} readWriteControl;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static RtemsEvent_t serialPortEvent;
static readWriteControl readWriteCtx;
static void* hUsbPort;
static int usbUartInitialized;
static int32_t dtrOn;
static int32_t startPlane;

static const USBSERI_EVENT_CODE_MAP EventCodeMap =
{
  -1, // RxDataAvailable;
  DTR_ON_SERIAL_EVENT, // DtrChangeHigh;
  DTR_OFF_SERIAL_EVENT, // DtrChangeLow;
  -1, // EscapeDataConnect;
  -1, // TxNotFull;
  -1, // BreakReceived;
  START_PLANE_SERIAL_EVENT, // StartPlane;
  -1, // StopPlane;
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static rtems_id initTaskId;

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void readDone(void *pDoneCtx, USBSERI_STATUS status,
              unsigned char *pBuffer, size_t nBuffer,
              size_t nActual)
{
    UNUSED(status);
    UNUSED(pBuffer);
    UNUSED(nBuffer);
    readWriteControl *readCtx = pDoneCtx;
    if (status == USBSERI_STATUS_OK)
    {
        readCtx->readSize = nActual;
        rtems_semaphore_release(readCtx->readSem);
    }
    else
        printk("Read error: %d\n", status);
}

static size_t USB_SyncRead(void *hPort, unsigned char *pBuffer, size_t nBuffer)
{
    int32_t status;

    status = Usbseri_ReadSubmit_V2(hPort, pBuffer, nBuffer,
                                   0, readDone, &readWriteCtx);
    if (status != USBSERI_STATUS_OK)
        return  0;
    status = rtems_semaphore_obtain(readWriteCtx.readSem,
                                    RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (status != RTEMS_SUCCESSFUL)
    {
        printk("Read sem obtain error: %ld\n", status);
        return 0;
    }
    return readWriteCtx.readSize;
}

static void writeDone(void *pDoneCtx, USBSERI_STATUS status,
               unsigned char *pBuffer, size_t nBuffer,
               size_t nActual)
{
    UNUSED(status);
    UNUSED(pBuffer);
    UNUSED(nBuffer);
    readWriteControl *writeCtx = pDoneCtx;
    if (status == USBSERI_STATUS_OK)
    {
        writeCtx->writeSize = nActual;
        rtems_semaphore_release(writeCtx->writeSem);
    }
    else
        printk("Write error: %d\n", status);
}

static size_t USB_SyncWrite(void *hPort, unsigned char *pBuffer, size_t nBuffer)
{
    int32_t status;

    status = Usbseri_WriteSubmit_V2(hPort, pBuffer, nBuffer,
                                    0, writeDone, &readWriteCtx);
    if (status != USBSERI_STATUS_OK)
        return  0;
    status = rtems_semaphore_obtain(readWriteCtx.writeSem,
                                    RTEMS_WAIT, RTEMS_NO_TIMEOUT);
    if (status != RTEMS_SUCCESSFUL)
    {
        printk("Write sem obtain error: %ld\n", status);
        return 0;
    }
    return readWriteCtx.writeSize;
}

static void SerialPortEventHandler(void *hPort, void *pClientContext,
    unsigned char event, void *pEventData)
{
    UNUSED(pClientContext);
    UNUSED(pEventData);
    UNUSED(hPort);

    switch (event) {
        case START_PLANE_SERIAL_EVENT:
            startPlane = 1;
        break;
        case DTR_ON_SERIAL_EVENT:
            printk("Terminal connected\n");
            dtrOn = 1;
        break;
        case DTR_OFF_SERIAL_EVENT:
            printk("Terminal disconnected\n");
            dtrOn = 0;
        break;
        default:
        break;
    }
    // RTEMS shell can be stared when serial port transfers are enabled
    // and a treminal is connected (DTR line is on)
    if (startPlane && dtrOn)
        rtems_event_send(initTaskId, START_SHELL_EVENT);
}


static rtems_task SerialPortInitTask(rtems_task_argument pContext)
{
    int status = 1;

    SerialPortContext *pSerialPortContext = (SerialPortContext*)pContext;

    initTaskId = rtems_task_self();
    // USB_OpenDevice must not be called in DataPump context because
    // it will block the DataPump task
    pSerialPortContext->portHandle = USB_OpenDevice(SERIALPORT_INSTANCE_NO);
    if (pSerialPortContext->portHandle)
    {
        hUsbPort = pSerialPortContext->portHandle;
        USB_RegisterEventNotify(hUsbPort,
          SerialPortEventHandler, pSerialPortContext, &EventCodeMap);

        status = rtems_semaphore_create(
            rtems_build_name('S', 'R', 'D', '0'), 0,
            RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, &readWriteCtx.readSem);
        if (status != RTEMS_SUCCESSFUL)
            printk("Error creating read semaphore: %d\n", status);

        status = rtems_semaphore_create(
            rtems_build_name('S', 'W', 'R', '0'), 0,
            RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, &readWriteCtx.writeSem);
        if (status != RTEMS_SUCCESSFUL)
            printk("Error creating write semaphore: %d\n", status);
    }
    else
    {
        printf("serial port init error\n");
    }

    if (status == RTEMS_SUCCESSFUL)
    {
        rtems_event_set eventOut;
        // wait for all condition to be met before starting the shell
        rtems_event_receive(
            START_SHELL_EVENT,
            RTEMS_WAIT | RTEMS_EVENT_ANY,
            RTEMS_NO_TIMEOUT,
            &eventOut
        );
        status = rtems_shell_init(
                        "SHLL", /* task name */
                        RTEMS_MINIMUM_STACK_SIZE * 2, /* task stack size */
                        200, /* task priority */
                        "/dev/usb0", /* device name */
                        false, /* run forever */
                        false, /* wait for shell to terminate */
                        NULL /* login check function,
                        use NULL to disable a login check */
        );
    }
    else
    {
        printk("Shell initialization error\n");
        exit(status);
    }

    rtems_task_delete(RTEMS_SELF);
}

SerialPortContext *SerialPortInit(UPLATFORM *pPlatform)
{
    int status;
    rtems_id taskId;

    SerialPortContext * pSerialPortContext;

    // Allocate context
    pSerialPortContext = UsbPumpPlatform_MallocZero(
            pPlatform,
            sizeof(*pSerialPortContext)
    );

    if (pSerialPortContext == NULL)
    {
        printk("pSerialPortContex allocation fail\n");
        return NULL;
    }

    pSerialPortContext->pPlatform = pPlatform;
    pSerialPortContext->portInstanceNo = SERIALPORT_INSTANCE_NO;

    status = rtems_task_create(
                rtems_build_name('S', 'e', 'r', 'i'),
                200,
                RTEMS_MINIMUM_STACK_SIZE,
                RTEMS_DEFAULT_MODES,
                RTEMS_DEFAULT_ATTRIBUTES,
                &taskId
                );
    if (status != RTEMS_SUCCESSFUL)
    {
        printk(" task create failed, Status(%d)\n", status);
    }
    serialPortEvent.taskId = taskId;
    status = rtems_task_start(
            taskId,
            SerialPortInitTask,
            (rtems_task_argument) pSerialPortContext
    );
    if (status != RTEMS_SUCCESSFUL)
    {
        printk("task start failed, Status(%d)\n", status);
    }

    //TODO this may not be the full context, since it is also modified
    //by the init task
    return pSerialPortContext;
}

// RTEMS specific event functions needed for
// synchronization with DataPump
void Usbseri_OS_InitializeEvent(
    USBSERI_OS_ABSTRACT_EVENT  *pEvent,
    unsigned sizeEvent  )
{
    UNUSED(sizeEvent);

    serialPortEvent.taskId = rtems_task_self();
    UHIL_cpybuf(pEvent, &serialPortEvent, sizeof(serialPortEvent));
}

void Usbseri_OS_DeinitializeEvent(USBSERI_OS_ABSTRACT_EVENT *pEvent)
{
    UNUSED(pEvent);

    // empty
}

int Usbseri_OS_WaitForEvent(USBSERI_OS_ABSTRACT_EVENT *pEvent)
{
    UNUSED(pEvent);

    rtems_event_set eventOut;

    return rtems_event_receive(
        SERIALPORT_EVENT,
        RTEMS_WAIT | RTEMS_EVENT_ANY,
        RTEMS_NO_TIMEOUT,
        &eventOut
    );
}

void Usbseri_OS_SetEvent(USBSERI_OS_ABSTRACT_EVENT *pEvent)
{
    RtemsEvent_t *pSerialEvent = (RtemsEvent_t *)pEvent;
    rtems_event_send(pSerialEvent->taskId, SERIALPORT_EVENT);
}

// libio functions

rtems_device_driver usb_uart_initialize(
  rtems_device_major_number  major,
  rtems_device_minor_number  minor,
  void  *pargp  __attribute__((unused))  )
{
    char device_name[16];
    rtems_device_driver status;

    strcpy(device_name,"/dev/usb0");

    device_name[8] += minor;

    status = rtems_io_register_name(
            device_name,
            major,
            minor
    );

    if (status != RTEMS_SUCCESSFUL)
        rtems_fatal_error_occurred(status);

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver usb_uart_open(
  rtems_device_major_number  major  __attribute__((unused)),
  rtems_device_minor_number  minor  __attribute__((unused)),
  void *pargp  )
{
    UNUSED(pargp);

    if (!usbUartInitialized)
    {
        SerialPortInit((UPLATFORM*)UsbPump_Rtems_DataPump_GetPlatform());
        usbUartInitialized = 1;
    }
    return RTEMS_SUCCESSFUL;
}

rtems_device_driver usb_uart_close(
  rtems_device_major_number  major  __attribute__((unused)),
  rtems_device_minor_number  minor  __attribute__((unused)),
  void *pargp __attribute__((unused))  )
{
    return RTEMS_SUCCESSFUL;
}

rtems_device_driver usb_uart_read(
    rtems_device_major_number  major  __attribute__((unused)),
    rtems_device_minor_number  minor  __attribute__((unused)),
    void  *pargp   )
{
  rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) pargp;

  if (rw_args)
  {
      if (rw_args->count > 0)
      {
          rw_args->bytes_moved = USB_SyncRead(hUsbPort, (unsigned char*)(rw_args->buffer), rw_args->count);
      }
  }
  if (rw_args->bytes_moved == 0)
  {
      // make sure that a read from stdin will not return EOF
      // because this will prevent subsequent reads
      rw_args->bytes_moved = RTEMS_UNSATISFIED;
  }

  return RTEMS_SUCCESSFUL;
}

rtems_device_driver usb_uart_write(
  rtems_device_major_number  major  __attribute__((unused)),
  rtems_device_minor_number  minor  __attribute__((unused)),
  void  *pargp  )
{
    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t *) pargp;

    if (rw_args)
        rw_args->bytes_moved = USB_SyncWrite(hUsbPort,
            (unsigned char*)(rw_args->buffer), rw_args->count);

    return RTEMS_SUCCESSFUL;
}

rtems_device_driver usb_uart_control(
    rtems_device_major_number  major  __attribute__((unused)),
    rtems_device_minor_number  minor  __attribute__((unused)),
    void  *pargp  __attribute__((unused))  )
{
    return RTEMS_NOT_IMPLEMENTED;
}
