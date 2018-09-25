///
/// @file      LcdHdmiSend.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Allow frames display over HDMI, 1080p30 interface
///            Platform(s) supported : ma2x8x
///

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "mv_types.h"
#include "FrameTypes.h"
#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/status-checks.h>
#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>

#include "DrvCpr.h"
#include <OsADV7513.h>
#include <DrvLcd.h>            /// LCD Driver
#include <LcdApi.h>            /// LCD Component API
#include <DrvMss.h>            /// Mipi Tx/Rx Driver
#include "DrvRegUtilsDefines.h"
#include "LcdCEA1080p60.h"

#include "LcdHdmiSend.h"
#include "SendOutTransport.h"


/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/
#define LCD_WINDOW_WIDTH        1920
#define LCD_WINDOW_HEIGHT       1080
#define MAX_USED_BUF            5

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/

/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/
SendOutDev      devHdmi;
LCDHandle       lcdHndl;
frameSpec       lcdFrameSpec;
LCDLayerOffset  lcdLayerStartOffset = {0, 0};
frameBuffer     lcdFrame[MAX_USED_BUF];
uint32_t        lcdFrameCnt = 0;
uint32_t        crtResolution = 0;
int             fd = 0; // hdmi i2c control file description
sem_t           hdmiSendSem;
int             hdmiStart = 0;
/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static void         lcdHdmiSentFrame(SoElem *task);
static void         setNewFrameToLcd(u8 *bufferP1,u8 * bufferP2, u8 * bufferP3);
static frameBuffer* allocateLcdFrame(int layer);

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/
//
void lcdHdmiSendCreate(void) {
    int sc;
    fd = open("/dev/i2c2." ADV7513_NAME, O_RDWR);
    if (fd < 0) {
        perror("hdmiThread: ADV7513 open failed ");
        exit(-1);
    }
    // !!! hardcoded for 1080p resolution, nv12 video format, used just for demo
    //printf("Configuring HDMI chip ADV7513...\n\n");
    sc = ioctl(fd, ADV7513_CFG_1080P60, NULL);
    if (sc < 0) {
        perror("hdmiThread: ADV7513 control failed !");
        exit(-1);
    }
    sendOutTrCreate(&devHdmi);
    devHdmi.thrCtrl.sendFrame = lcdHdmiSentFrame;
    assert(RTEMS_SUCCESSFUL == close(fd));
}

//
void lcdHdmiSentInit(void) {
    int ibuf;
    crtResolution = 0;
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LCD);
    for (ibuf = 0; ibuf < MAX_USED_BUF; ibuf++) {
        lcdFrame[ibuf].spec = lcdFrameSpec;
        lcdFrame[ibuf].p1   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p2   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p3   = (uint8_t*)0x80000000;
    }
    assert(sem_init(&hdmiSendSem, 0, 0) == RTEMS_SUCCESSFUL);
    hdmiStart = 0;
    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();
    sendOutTrInit(&devHdmi);
}

void lcdHdmiSendControl(uint32_t camEnBitMask, uint32_t frmTypeEnBitMask, uint32_t frmFmtEnBitMask) {
    sendOutTrControl(&devHdmi, camEnBitMask, frmTypeEnBitMask, frmFmtEnBitMask);
}

void lcdHdmiSend(FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent) {
    sendOutTrSend(&devHdmi, frame, outputId, frmType, sendOutCbSent);
    if (sendOutIsDisable(&devHdmi, outputId, frmType, frame->type)) {
        // not change as will not be send
    }
    else {
        if(hdmiStart) {
            sem_post(&hdmiSendSem);
        }
        else {
            hdmiStart++;
        }
    }
}

void lcdHdmiSendFini(void) {
    sem_destroy(&hdmiSendSem);
    sendOutTrFini(&devHdmi);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS,  DEV_MSS_LCD);
    //LCDStop(&lcdHndl);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
}

/**************************************************************************************************
 ~~~ Local File Functions Implementation
 **************************************************************************************************/
//
static void lcdHdmiSentFrame(SoElem *task) {
    FrameT *frame = task->buffer;
    InternalCbSent sendOutCbSent = task->localCallback;

    uint8_t conversion_enable = 0;
    uint8_t* buffPtrP0 = frame->fbPtr[0];
    uint8_t* buffPtrP1 = NULL;
    uint8_t* buffPtrP2 = NULL;

    //printf("frm: %p, %p, %p, %lu, %lu, %lu \n", frame->fbPtr[0], frame->fbPtr[1], frame->fbPtr[2],
    //        frame->height[0], frame->stride[0], frame->type);

    if(crtResolution != ((frame->height[0]<<16)|(frame->stride[0]))) {
        if(0 != crtResolution) {
            //LCDStop(&lcdHndl);
            DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
            DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LCD);
        }
        crtResolution = ((frame->height[0]<<16)|(frame->stride[0]));
        if(frame->stride[0] < LCD_WINDOW_WIDTH) {
            lcdFrameSpec.width = frame->stride[0];
        }
        else {
            lcdFrameSpec.width   = LCD_WINDOW_WIDTH;
        }
        if(frame->height[0] < LCD_WINDOW_HEIGHT) {
            lcdFrameSpec.height  = frame->height[0];
        }
        else {
            lcdFrameSpec.height  = LCD_WINDOW_HEIGHT;
        }

        switch(frame->fmt) {
            case FRAME_FMT_422I:
                conversion_enable = 0;
                lcdFrameSpec.bytesPP = 2;
                lcdFrameSpec.type    = YUV422i;
                lcdFrameSpec.stride  = frame->stride[0];
                break;
            default:
            case FRAME_FMT_YUV420:
                conversion_enable = 1;
                lcdFrameSpec.bytesPP = 1;
                lcdFrameSpec.type    = YUV420p;
                lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                break;
            case FRAME_FMT_NV12_Y: /// Monochrome
                conversion_enable = 1;
                lcdFrameSpec.bytesPP = 1;
                lcdFrameSpec.type    = YUV420p;
                lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                break;
            case FRAME_FMT_RAW_8: /// Monochrome
                conversion_enable = 1;
                lcdFrameSpec.bytesPP = 1;
                lcdFrameSpec.type    = YUV420p;
                lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                break;
        }
        lcdFrameCnt = 0;
        LCDInit(&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
        LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
        LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);
        //printf("Starting LCD...\n");
        LCDStart(&lcdHndl);
        // Enable nv12 to Yuv422i format conversion on LCD interface
        if(conversion_enable) {
            u32 data  = 0;
            data |= (0x2   << 9);   // Layer format
            data |= (0x1     << 14);  // Planar storage
            data |= ((0x1-1) << 15);  // Bytes per pixel
            SET_REG_WORD(LCD1_LAYER0_CFG_ADR,        data);
        }
        //TODO: This is a temp fix, remove it when problem is fixed in LCD (bug_23117)
        // Clear bit28 (D_LCD_CTRL_PIPELINE_DMA) of LCD register LCD_CONTROL (0x30fc0000)
        u32 lcdCtrl = GET_REG_WORD_VAL(LCD1_CONTROL_ADR);
        lcdCtrl &= 0xefffffff;
        SET_REG_WORD(LCD1_CONTROL_ADR, lcdCtrl);
    }

    switch(frame->fmt)
    {
    case FRAME_FMT_422I:
        buffPtrP1 = NULL;
        buffPtrP2 = NULL;
        break;
    default:
    case FRAME_FMT_YUV420:
        buffPtrP1 = frame->fbPtr[1];
        buffPtrP2 = frame->fbPtr[1]+frame->height[0]*frame->stride[0]/4;
        break;
    case FRAME_FMT_NV12_Y: /// Monochrome
        buffPtrP1 = devHdmi.blackUVBuf;
        buffPtrP2 = devHdmi.blackUVBuf;
        break;
    case FRAME_FMT_RAW_8: /// Monochrome
        buffPtrP1 = devHdmi.blackUVBuf;
        buffPtrP2 = devHdmi.blackUVBuf;
        break;
    }
    setNewFrameToLcd(buffPtrP0, buffPtrP1, buffPtrP2);
    sem_wait(&hdmiSendSem);
    sendOutCbSent((void*)&devHdmi);
}

//
static void setNewFrameToLcd(u8 *bufferP1,u8 * bufferP2, u8 * bufferP3) {

    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p1   = bufferP1;
    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p2   = bufferP2;
    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p3   = bufferP3;//+(1920*1080/4);
    lcdFrameCnt++;
}

//
static frameBuffer* allocateLcdFrame(int layer) {
    UNUSED(layer);
    return (&lcdFrame[lcdFrameCnt % MAX_USED_BUF]);

}
