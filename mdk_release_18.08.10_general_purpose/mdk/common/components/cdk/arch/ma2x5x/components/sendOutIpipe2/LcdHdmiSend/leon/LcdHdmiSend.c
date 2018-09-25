
/**************************************************************************************************

 @File         : lcdHdmiSend.c
 @Author       : MT
 @Brief        : Allow display frames over HDmi
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "mv_types.h"
#include "IcTypes.h"
#include "DrvI2cMaster.h"
#include "DrvRegUtilsDefines.h"
#include "DrvTimer.h"
#include "DrvGpio.h"
#include <DrvLcd.h>             /// LCD Driver
#include <LcdApi.h>             /// LCD Component API
#include <DrvMss.h>            /// Mipi Tx/Rx Driver
#include "ipipe.h"
#include "unistd.h"
#include "LcdHdmiSend.h"
#include "BoardHdmiHw.h"

/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/
#define LCD_WINDOW_WIDTH        1920
#define LCD_WINDOW_HEIGHT       1080
#define MAX_USED_BUF            5
#define MAX_NR_OF_SOURCES       6

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/

/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/
LCDHandle               lcdHndl;
static LCDDisplayCfg    *lcdHdmiSend_lcdCfg;
frameSpec               lcdFrameSpec;
LCDLayerOffset          lcdLayerStartOffset = {0, 0};
frameBuffer             lcdFrame[MAX_USED_BUF];
uint8_t ddrBlackUV[SEND_OUT_MAX_WIDTH*SEND_OUT_MAX_HEIGHT>>2] __attribute__((section(".ddr.bss")))  __attribute__((aligned(8)));
uint32_t lcdFrameCnt = 0;
uint32_t crtResolution = 0;

/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static void setNewFrameToLcd(u8 *bufferP1,u8 * bufferP2, u8 * bufferP3);
static frameBuffer* allocateLcdFrame(int layer);

/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/

void lcdHdmiSendCreate(HdmiCfg_t *cfg)
{
    int status = 0;
    status = boardHdmiHwInit();
    if (status != 0){
        printf("Failed at board specific HDMI initialization\n");
    }
    // save LCD config to global variable
    if(cfg != NULL)
    {
        lcdHdmiSend_lcdCfg = cfg->lcdCfg;
    }
}

//
void lcdHdmiSentInit(void) {
    int ibuf;
    crtResolution = 0;
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, DEV_MSS_LCD);
    int32_t status;
    // !!! hardcoded for 1080p resolution, nv12 video format, used just for demo
    //printf("Configuring HDMI chip ADV7513...\n\n");

    status = boardHdmiHwOpen();
    if (status != 0){
        printf("Failed at board specific HDMI open\n");
    }

    //connect the LCD to output parallel bus (by GPIO) instead of MIPI Tx
    DrvMssConnectLcdToGpio();
    for (ibuf = 0; ibuf < MAX_USED_BUF; ibuf++)
    {
        lcdFrame[ibuf].spec = lcdFrameSpec;
        lcdFrame[ibuf].p1   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p2   = (uint8_t*)0x80000000;
        lcdFrame[ibuf].p3   = (uint8_t*)0x80000000;
    }
    memset(ddrBlackUV, 0x80, sizeof(ddrBlackUV));
}

void lcdHdmiSendFini(void) {
    int status;

    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, DISABLE_CLKS,  DEV_MSS_LCD);
    DrvCprSysDeviceAction(MSS_DOMAIN, ENABLE_CLKS,  DEV_MSS_LCD);
    //LCDStop(&lcdHndl);
    DrvCprSysDeviceAction(MSS_DOMAIN, ASSERT_RESET,  DEV_MSS_LCD);

    status = boardHdmiHwDeinit();
    if (status != 0){
        printf("Failed at board specific HDMI deinitialization\n");
    }
}

//
void lcdHdmiSentFrame(SendOutElement_t *task)
{
    FrameT *frame = task->buffer;
    uint32_t outputId = task->outId;
    uint32_t frmType = task->frmType;
    InternalCbSent sendOutCbSent = task->localCallback;

    dbgEnableOutput = swcLeonReadNoCacheU32((uint32_t)&dbgEnableOutput);

    uint8_t conversion_enable = 0;
    uint8_t* buffPtrP0 = frame->fbPtr[0];
    uint8_t* buffPtrP1 = NULL;
    uint8_t* buffPtrP2 = NULL;

    uint32_t videoOutputId  = outputId;
    uint32_t stillOutputId  = outputId + MAX_NR_OF_SOURCES;
    uint32_t lcdHdmiOutputId = 0u;
    uint8_t send_frame = (uint8_t) TRUE; // TRUE = send frame; FALSE = don't send frame

    if(frmType == IC_OUTPUT_FRAME_DATA_TYPE_PREVIEW) {
        lcdHdmiOutputId = videoOutputId;
    }
    else if(frmType == IC_OUTPUT_FRAME_DATA_TYPE_STILL) {
            lcdHdmiOutputId = stillOutputId;
    }
    else if(frmType == IC_OUTPUT_FRAME_DATA_TYPE_STILL_RAW) {
        // raw frame is not sent on HDMI
        send_frame = (uint8_t) FALSE;
    }
    else {
        assert(0);
    }

    if( (send_frame == TRUE) &&
		((dbgEnableOutput & ((uint32_t) 1u << lcdHdmiOutputId)) > 0u) ) {
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

            switch(frame->type)
            {
                case FRAME_T_FORMAT_422I:
                    conversion_enable = 0;
                    lcdFrameSpec.bytesPP = 2;
                    lcdFrameSpec.type    = YUV422i;
                    lcdFrameSpec.stride  = frame->stride[0];
                    break;
                default:
                case FRAME_T_FORMAT_YUV420:
                    conversion_enable = 1;
                    lcdFrameSpec.bytesPP = 1;
                    lcdFrameSpec.type    = YUV420p;
                    lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                    break;
                case FRAME_T_FORMAT_NV12_Y: /// Monochrome
                    conversion_enable = 1;
                    lcdFrameSpec.bytesPP = 1;
                    lcdFrameSpec.type    = YUV420p;
                    lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                    break;
                case FRAME_T_FORMAT_RAW_8: /// Monochrome
                    conversion_enable = 1;
                    lcdFrameSpec.bytesPP = 1;
                    lcdFrameSpec.type    = YUV420p;
                    lcdFrameSpec.stride  = frame->stride[0] * lcdFrameSpec.bytesPP;
                    break;
            }
            lcdFrameCnt = 0;
            LCDInit(&lcdHndl, lcdHdmiSend_lcdCfg, NULL, LCD1);
            LCDInitLayer(&lcdHndl, VL1, &lcdFrameSpec, lcdLayerStartOffset);
            LCDSetupCallbacks(&lcdHndl, &allocateLcdFrame, NULL, NULL, NULL);
            //printf("Starting LCD...\n");
            LCDStart(&lcdHndl);
            // Enable nv12 to Yuv422i format conversion on LCD interface
            if(conversion_enable)
            {
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

        switch(frame->type)
        {
        case FRAME_T_FORMAT_422I:
            buffPtrP1 = NULL;
            buffPtrP2 = NULL;
            break;
        default:
        case FRAME_T_FORMAT_YUV420:
            buffPtrP1 = frame->fbPtr[1];
            buffPtrP2 = frame->fbPtr[1]+frame->height[0]*frame->stride[0]/4;
            break;
        case FRAME_T_FORMAT_NV12_Y: /// Monochrome
            buffPtrP1 = ddrBlackUV;
            buffPtrP2 = ddrBlackUV;
            break;
        case FRAME_T_FORMAT_RAW_8: /// Monochrome
            buffPtrP1 = ddrBlackUV;
            buffPtrP2 = ddrBlackUV;
            break;
        }
        setNewFrameToLcd(buffPtrP0, buffPtrP1, buffPtrP2);
    }
    // release frame
    sendOutCbSent(task);
}

//
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
