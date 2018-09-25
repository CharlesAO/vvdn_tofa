///
/// @file      PlgHdmiOut.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Plugin for HDMI output.
///            Platform(s) supported : ma2x5x
///

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "mv_types.h"
#include "FrameTypes.h"
#include "DrvI2cMaster.h"
#include "DrvRegUtilsDefines.h"
#include "DrvTimer.h"
#include "DrvGpio.h"
#include "PlgHdmiOut.h"
//
//#ifndef RUNNING_ON_CUSTOM_BOARD
// #ifdef MA2450
//  #include "MV0212Api.h"
// #else
//  #include "Board182Api.h"
// #endif
//#else
// #include "brdWM100.h"
//#endif


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
#include <OsADV7513.h>

//#include "LcdCEA1080p60.h"
//#include "DrvADV7513.h"
#include <DrvLcd.h>            /// LCD Driver
#include <LcdApi.h>            /// LCD Component API
#include <DrvMss.h>            /// Mipi Tx/Rx Driver

//hard-coded LCD resolution
#define LCD_WINDOW_WIDTH        1920
#define LCD_WINDOW_HEIGHT       1080
#define MAX_USED_BUF            5

uint32_t dbgEnableOutput = 255; // lcd demo related

/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/
LCDHandle               lcdHndl;
frameSpec               lcdFrameSpec;
LCDLayerOffset          lcdLayerStartOffset = {0, 0};
frameBuffer             lcdFrame[MAX_USED_BUF];
uint8_t  ddrBlackUV[3842*2200>>2] __attribute__((section(".ddr.bss")))  __attribute__((aligned(8)));
uint32_t lcdFrameCnt = 0;
uint32_t crtResolution = 0;

LCDDisplayCfg lcdSpec1080p60 = {
            .width          = 1920,
            .hPulseWidth    = 44,
            .hBackP         = 148,
            .hFrontP        = 88,
            .height         = 1080,
            .vPulseWidth    = 5,
            .vBackP         = 36,
            .vFrontP        = 4,
            .outputFormat   = D_LCD_OUTF_FORMAT_YCBCR422_8B | D_LCD_OUTF_USE_EXTERNAL_CLK,
            .control        = D_LCD_CTRL_OUTPUT_RGB | D_LCD_CTRL_PROGRESSIVE,
            .pixelClockkHz  = 148500
};

uint32_t    outputIdCrt = 0;
ImgFrame    *frameCrt = NULL;

//###################################################################################
void lcdHdmiSendInit(void) {
    int ibuf;

    int sc, fd;

    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();

    fd = open("/dev/i2c2." ADV7513_NAME, O_RDWR);
    if (fd < 0)
    {
        perror("hdmiThread: ADV7513 open failed ");
        exit(-1);
    }
    // !!! hardcoded for 1080p resolution, nv12 video format, used just for demo
    //printf("Configuring HDMI chip ADV7513...\n\n");
    sc = ioctl(fd, ADV7513_CFG_1080P60, NULL);
    if (sc < 0)
    {
        perror("hdmiThread: ADV7513 control failed !");
        exit(-1);
    }
    close(fd);

    crtResolution = 0;
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,   DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LCD);


    for (ibuf = 0; ibuf < MAX_USED_BUF; ibuf++)
    {
        lcdFrame[ibuf].spec = lcdFrameSpec;
        lcdFrame[ibuf].p1   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p2   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p3   = (uint8_t*)0x80000000;
    }
    memset(ddrBlackUV, 0x80, sizeof(ddrBlackUV));
}

//###################################################################################
void lcdHdmiSendFini(void) {
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS,   DEV_MSS_LCD);
    //LCDStop(&lcdHndl);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
}


//###################################################################################
static void setNewFrameToLcd(u8 *bufferP1,u8 * bufferP2, u8 * bufferP3) {
    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p1   = bufferP1;
    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p2   = bufferP2;
    lcdFrame[(lcdFrameCnt + 1) % MAX_USED_BUF].p3   = bufferP3;//+(1920*1080/4);
    lcdFrameCnt++;
}

//
static frameBuffer* allocateLcdFrame(int layer)
{
    UNUSED(layer);
    return (&lcdFrame[lcdFrameCnt % MAX_USED_BUF]);
}

static void sendFrame(ImgFrame *frame, uint32_t outputId)
{
    dbgEnableOutput = swcLeonReadNoCacheU32((uint32_t)&dbgEnableOutput);

    uint8_t conversion_enable = 0;
    uint8_t* buffPtrP0 = (uint8_t*)frame->fb.p1;
    uint8_t* buffPtrP1 = NULL;
    uint8_t* buffPtrP2 = NULL;

    if (dbgEnableOutput & (1<<outputId)) {
        if(crtResolution != ((frame->fb.spec.height<<16)|(frame->fb.spec.stride))) {
            if(0 != crtResolution) {
                //LCDStop(&lcdHndl);
                DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
                DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LCD);
            }
            crtResolution = ((frame->fb.spec.height<<16)|(frame->fb.spec.stride));
            if(frame->fb.spec.stride < LCD_WINDOW_WIDTH) {
                lcdFrameSpec.width = frame->fb.spec.stride;
            }
            else {
                lcdFrameSpec.width   = LCD_WINDOW_WIDTH;
            }
            if(frame->fb.spec.height < LCD_WINDOW_HEIGHT) {
                lcdFrameSpec.height  = frame->fb.spec.height;
            }
            else {
                lcdFrameSpec.height  = LCD_WINDOW_HEIGHT;
            }

            switch(frame->fb.spec.type)
            {
                case YUV422i:
                    conversion_enable    = 0;
                    lcdFrameSpec.bytesPP = 2;
                    lcdFrameSpec.type    = YUV422i;
                    lcdFrameSpec.stride  = frame->fb.spec.stride;
                    break;
                default:
                case YUV420p:
                    conversion_enable    = 1;
                    lcdFrameSpec.bytesPP = 1;
                    lcdFrameSpec.type    = YUV420p;
                    lcdFrameSpec.stride  = frame->fb.spec.stride * lcdFrameSpec.bytesPP;
                    break;
             //case FRAME_FMT_NV12_Y: /// Monochrome
             //    conversion_enable    = 1;
             //    lcdFrameSpec.bytesPP = 1;
             //    lcdFrameSpec.type    = YUV420p;
             //    lcdFrameSpec.stride  = frame->fb.spec.stride * lcdFrameSpec.bytesPP;
             //    break;
                case RAW8: /// Monochrome
                    conversion_enable    = 1;
                    lcdFrameSpec.bytesPP = 1;
                    lcdFrameSpec.type    = YUV420p;
                    lcdFrameSpec.stride  = frame->fb.spec.stride * lcdFrameSpec.bytesPP;
                    break;
            }
            lcdFrameCnt = 0;
            LCDInit          (&lcdHndl, &lcdSpec1080p60, NULL, LCD1);
            LCDInitLayer     (&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
            LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);
            //printf("Starting LCD...\n");
            LCDStart(&lcdHndl);
            // Enable nv12 to Yuv422i format conversion on LCD interface
            if(conversion_enable)
            {
                u32 data  = 0;
                data |= (0x2     <<  9);  // Layer format
                data |= (0x1     << 14);  // Planar storage
                data |= ((0x1-1) << 15);  // Bytes per pixel
                SET_REG_WORD(LCD1_LAYER0_CFG_ADR,        data);
            }
            //TODO: This is a temp fix, remove it when problem is fixed in LCD (bug_23117)
            // Clear bit28 (D_LCD_CTRL_PIPELINE_DMA) of LCD register LCD_CONTROL (0x30fc0000)
            //u32 lcdCtrl = GET_REG_WORD_VAL(LCD1_CONTROL_ADR);
            //lcdCtrl &= 0xefffffff;
            //SET_REG_WORD(LCD1_CONTROL_ADR, lcdCtrl);

        }

        switch(frame->fb.spec.type)
        {
        case YUV422i:
            buffPtrP1 = NULL;
            buffPtrP2 = NULL;
            break;
        default:
        case YUV420p:
            buffPtrP1 = (uint8_t*)frame->fb.p2;
            buffPtrP2 = (uint8_t*)frame->fb.p2+frame->fb.spec.height*frame->fb.spec.stride/4;
            break;
      //case FRAME_FMT_NV12_Y: /// Monochrome ???
      //    buffPtrP1 = ddrBlackUV;
      //    buffPtrP2 = ddrBlackUV;
      //    break;
        case RAW8: /// Monochrome
            buffPtrP1 = ddrBlackUV;
            buffPtrP2 = ddrBlackUV;
            break;
        }
        setNewFrameToLcd(buffPtrP0, buffPtrP1, buffPtrP2);
    }
}

void lcdHdmiSendFrame(ImgFramePtr *frame, uint32_t outputId)
{
    dbgEnableOutput = swcLeonReadNoCacheU32((uint32_t)&dbgEnableOutput);
    if (dbgEnableOutput & (1<<outputId))
    {
      frameCrt    = (ImgFrame*)frame->ptr;
      outputIdCrt = outputId;
      sendFrame(frameCrt, outputId);
    }
}
