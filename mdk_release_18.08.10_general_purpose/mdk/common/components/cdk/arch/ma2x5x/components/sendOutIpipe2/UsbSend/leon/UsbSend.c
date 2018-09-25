
/**************************************************************************************************

 @File         : UsbSend.c
 @Author       : MT
 @Brief        : Allow display frames over USB
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "IcTypes.h"
#include "UsbSend.h"
#include <OsDrvTimer.h>



extern int VideoDemo_SendFrame(sUsbTest *sUsbData);
extern int usb_in_ctrl_thread_init(UsbSend_InputCtrlCallback_Type cb_func);

sUsbTest usbStruct;


#ifdef DBG_USBSEND
static void UsbSendCalcDeltaTime(uint32_t frame_ctr);
#endif

// Initialization function for USB physical layer and USB Rtems DataPump
void UsbSendCreate(UsbCfg_t *cfg)
{
//    printf("\n\n UsbSendCreate \n\n\n");
	if(cfg != (UsbCfg_t *) NULL)
	{
		OsDrvUsbPhyInit(cfg->phyParamInit);

		if (UsbPump_Rtems_DataPump_Startup(cfg->dataPumpCfgInit) != NULL)
		{
			printf("\n\nUsbPump_Rtems_DataPump_Startup() init OK!\n\n\n");
		}
		else
		{
			printf("\n\nUsbPump_Rtems_DataPump_Startup() failed!\n\n\n");
			exit(1);
		}
		{
			if (usb_in_ctrl_thread_init(cfg->cb_function))
			{
				printf("\n\n usb_in_ctrl_thread_init() failed!\n\n\n");
				exit(1);
			}
		}
	}
}

#ifdef DBG_USBSEND
/* measure and print time (in [ms]) from previous call
 * calculation valid for sys clock = DEFAULT_APP_CLOCK_KHZ */
static void UsbSendCalcDeltaTime(uint32_t frame_ctr)
{
    static tyTimeStamp testUsbCamFrmTs;
    u32 testUsbCamFrmMs;
    u64 testUsbCamFrmTicks;

    if(frame_ctr > 0u)
    {
        if(OsDrvTimerGetElapsedTicks(&testUsbCamFrmTs, &testUsbCamFrmTicks) == MYR_DRV_SUCCESS)
        {
            testUsbCamFrmMs = (u32) ((u64) testUsbCamFrmTicks / (u32) DEFAULT_APP_CLOCK_KHZ);
            DBG_PRINTF(" -> %lu ms\n", testUsbCamFrmMs);
        }
        else
        {
            DBG_PRINTF("OsDrvTimerGetElapsedTicks error!\n\n");
        }
    }

    if(OsDrvTimerStartTicksCount(&testUsbCamFrmTs) != MYR_DRV_SUCCESS)
    {
        DBG_PRINTF("OsDrvTimerStartTicksCount error!\n\n");
    }
}
#endif // DBG_USBSEND

void UsbSendFrame(SendOutElement_t *task)
{
InternalCbSent sendOutCbSent = task->localCallback;

    assert(NULL != task);
    assert(NULL != task->buffer);
#ifdef DBG_USBSEND
    DBG_PRINTF("\nCAM %lu av frm %lu", task->outId, task->buffer->seqNo);

    UsbSendCalcDeltaTime(task->buffer->seqNo);
#endif

    /* in case busy or sendInProgress set, skip frame
     * busy is set during structure update
     * sendInProgress is set during USB transmission */
    if((usbStruct.busy == 0) &&
       (usbStruct.sendInProgress == 0))
    {
        usbStruct.busy = 1;
        usbStruct.filled = 1;
        usbStruct.task = task;
        usbStruct.busy = 0;

        if (VideoDemo_SendFrame(&usbStruct))
        {
            usbStruct.filled = 0;
            /* release frame buffers */
            DBG_ERROR_PRINTF("skip1!  %d %p\n", (int)task->buffer->seqNo, task->buffer);
            sendOutCbSent(task);
        }
    }
    else
    {
        /* skip the frame */
        DBG_ERROR_PRINTF("skip2!  %d %p\n", (int)task->buffer->seqNo, task->buffer);
        sendOutCbSent(task);
    }
}


