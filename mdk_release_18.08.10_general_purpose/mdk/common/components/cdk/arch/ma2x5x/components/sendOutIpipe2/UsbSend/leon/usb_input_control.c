/*
 * usb_input_control.c
 *
 *  Created on: Oct 26, 2016
 *      Author: aiovtchev
 */

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <rtems.h>
#include <semaphore.h>
#include <pthread.h>
#include "UsbSend.h"


void app_guzzi_command_execute(uint8_t *in_command);

#define IN_MESSAGE_MAX  64

static uint8_t in_message[IN_MESSAGE_MAX];
static uint8_t in_processing_message[IN_MESSAGE_MAX];
static int in_msg_cnt = 0;
static int in_message_err = 0;


#ifndef USB_IN_CTRL_THREAD_PRIORITY
#define USB_IN_CTRL_THREAD_PRIORITY     190
#endif

#ifndef USB_IN_CTRL_THREAD_STACK_SIZE
#define USB_IN_CTRL_THREAD_STACK_SIZE   (4*1024)
#endif


typedef enum {
    IN_CTRL_THREAD__ACTIVE,
    IN_CTRL_THREAD__STOPPING,
} usb_in_ctrl_thread_state_t;

typedef struct {
    pthread_t *thread;
    sem_t *control_sem;
    usb_in_ctrl_thread_state_t state;
} usb_in_ctrl_thread_t;

usb_in_ctrl_thread_t usb_in_ctrl_module_storage;

pthread_t usb_in_thread;
static sem_t usb_in_control_sem;
UsbSend_InputCtrlCallback_Type usb_in_ctrl_callback = NULL;

static void * usb_in_ctrl_thread_func(void *arg)
{
    usb_in_ctrl_thread_t *usb_in_ctrl_thread;
    int err;

    usb_in_ctrl_thread = arg;

    for (;;) {
        err = sem_wait(usb_in_ctrl_thread->control_sem);
        if (err) {
            printf("USB in Control: Failed to obtain control_sem: err=%d", err);
        }
        if (usb_in_ctrl_thread->state == IN_CTRL_THREAD__STOPPING)
            break;

        if(usb_in_ctrl_callback != NULL)
        {
            (*usb_in_ctrl_callback)(&in_processing_message[4]);
        }
    }
    return NULL;
}



int usb_in_ctrl_thread_init(UsbSend_InputCtrlCallback_Type cb_func)
{
    usb_in_ctrl_thread_t *usb_in_ctrl_thread;

    usb_in_ctrl_thread = &usb_in_ctrl_module_storage;

    usb_in_ctrl_callback = cb_func;

    if(sem_init(&usb_in_control_sem, 0, 0) != RTEMS_SUCCESSFUL) {
        printf("Failed to create message control semaphore\n");
        goto exit1;
    }
    usb_in_ctrl_thread->control_sem = &usb_in_control_sem;

    // create tx thread
    pthread_attr_t attr;
    struct sched_param param;
    assert(RTEMS_SUCCESSFUL == pthread_attr_init           (&attr));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedpolicy (&attr, SCHED_RR));
    assert(RTEMS_SUCCESSFUL == pthread_attr_getschedparam (&attr, &param));
    param.sched_priority = USB_IN_CTRL_THREAD_PRIORITY;
    assert(RTEMS_SUCCESSFUL == pthread_attr_setschedparam (&attr, &param));
    assert(RTEMS_SUCCESSFUL == pthread_create(&usb_in_thread, &attr, usb_in_ctrl_thread_func, usb_in_ctrl_thread));
    assert(RTEMS_SUCCESSFUL == pthread_attr_destroy   (&attr));
    usb_in_ctrl_thread->thread = &usb_in_thread;
    usb_in_ctrl_thread->state = IN_CTRL_THREAD__ACTIVE;

    return 0;

exit1:
    sem_destroy(&usb_in_control_sem);
    usb_in_ctrl_thread->control_sem = NULL;
    return 1;
}


void usb_in_ctrl_thread_fini(void)
{
	usb_in_ctrl_thread_t *usb_in_ctrl_thread = &usb_in_ctrl_module_storage;

    usb_in_ctrl_thread->state = IN_CTRL_THREAD__STOPPING;
    sem_post(usb_in_ctrl_thread->control_sem);    // Make sure thread will see this request
    pthread_join(usb_in_thread, NULL);
    usb_in_ctrl_thread->thread = NULL;
    sem_destroy(usb_in_ctrl_thread->control_sem);
    usb_in_ctrl_thread->control_sem = NULL;
    usb_in_ctrl_callback = NULL;
}


static inline void in_message_init(void)
{
    in_msg_cnt = 0;
    in_message_err = 0;
}

static inline void in_message_process(void)
{
    usb_in_ctrl_thread_t *usb_in_ctrl_thread = &usb_in_ctrl_module_storage;
    if (in_msg_cnt < 5)
    {
        in_message_err++;
        printf("Error: incoming message is too short (%d) or protocol error\n", in_msg_cnt);
    }
    if ((in_message[3]+4) != in_msg_cnt)
    {
        in_message_err++;
        printf("Error: protocol error msg_len = %d, but in_msg_cnt = %d\n", (in_message[3]+1), in_msg_cnt);
    }
    if (in_message_err == 0)
    {
        memcpy (in_processing_message, in_message, IN_MESSAGE_MAX);
        sem_post(usb_in_ctrl_thread->control_sem);
    }
    in_message_init();
}

void in_symbol_process(uint16_t in_symbol)
{
    if ((in_symbol&0xFF) != 0xFF)
    {
        if (in_msg_cnt < (IN_MESSAGE_MAX-1))
        {
            in_message[in_msg_cnt] = (uint8_t)in_symbol;
            in_msg_cnt++;
        } else {
            in_message_err++;
            printf("Error: incoming message is too big or protocol error\n");
        }
    }
    if (in_symbol&0xFF00)
    {
        if (in_msg_cnt < IN_MESSAGE_MAX)
        {
            in_message[in_msg_cnt] = 0;
        } else
        {
            in_message[IN_MESSAGE_MAX-1] = 0;
            in_message_err++;
        }
        in_message_process();
    }
}


