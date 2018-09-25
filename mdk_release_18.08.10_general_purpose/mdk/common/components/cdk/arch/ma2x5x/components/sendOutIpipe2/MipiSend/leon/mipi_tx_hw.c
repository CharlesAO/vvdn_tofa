///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// -----------------------------------------------------------------------------
///
/// Revision History
/// ===================================
/// 05-Nov-2014 : Author ( MM Solutions AD )
/// Created
/// =============================================================================

#include <stdio.h>
#include <assert.h>

#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <DrvCpr.h>
#include <DrvMipi.h>
#include <DrvLcd.h>
#include <LcdApi.h>

#include "mipi.h"
#include "mipi_tx_hw.h"
#include "mipi_tx_board_params.h"
#include "MipiSendApi.h"

//int sss[300];
//int iii = 0;


/* Default values will not be use to send data.
 * Before every send these values will be overwritten */

#define MIPI_TX_DEFAULT_HEADER_DATA_TYPE 0x30
#define MIPI_TX_DEFAULT_DATA_TYPE 0x30

#define LCD_HOR_PULSE_WIDTH 40
#define LCD_HOR_BACK_PORCH 97
#define LCD_HOR_FRONT_PORCH 0

#define LCD_VER_PULSE_WIDTH 5
#define LCD_VER_BACK_PORCH 40
#define LCD_VER_FRONT_PORCH 40

#define ARRAY_SIZE(arr) \
    ((int)(sizeof(arr) / sizeof((arr)[0])))

enum {
    LAYER_HEADER,
    LAYER_PAYLOAD_1,
    LAYER_PAYLOAD_2,
    LAYER_PAYLOAD_3,
    LAYERS_NUM
};

typedef struct {
    /* Number of bytes on parallel port */
    /* TODO: Use mipi_data_mode+data_type to wc? */
    int mipi_wc;
    eDrvMipiDataMode mipi_data_mode;
    u32 lcd_out_format;
    frameType lcd_in_format;
    /* TODO: Use lcd_in_format to bytesPP? */
    unsigned int lcd_in_bytesPP;
} data_type_info_t;

/* To be used as index for data_type_info array */
enum {
    DATA_TYPE_USER, /* MIPI data types 0x1E,0x30-0x37 (422,USER) */
};

static data_type_info_t data_type_info[] = {
    { /* DATA_TYPE_USER */
        .mipi_wc        = 2,
        .mipi_data_mode = MIPI_D_MODE_1,
        .lcd_out_format = D_LCD_OUTF_FORMAT_YCBCR422_8B,
        .lcd_in_format  = YUV422i,
        .lcd_in_bytesPP = 2
    }
};

static data_type_info_t *data_type_info_get(int data_type)
{
    switch (data_type) {
        case MIPI_DT_CSI2_YUV_422_8BIT:
        case MIPI_DT_CSI2_USER_1:
        case MIPI_DT_CSI2_USER_2:
        case MIPI_DT_CSI2_USER_3:
        case MIPI_DT_CSI2_USER_4:
        case MIPI_DT_CSI2_USER_5:
        case MIPI_DT_CSI2_USER_6:
        case MIPI_DT_CSI2_USER_7:
        case MIPI_DT_CSI2_USER_8:
            return data_type_info + DATA_TYPE_USER;
        default:
            return data_type_info + DATA_TYPE_USER;
    }
}

static int data_type_to_wc(int data_type)
{
    return data_type_info_get(data_type)->mipi_wc;
}

static eDrvMipiDataMode data_type_to_mipi_data_mode(int data_type)
{
    return data_type_info_get(data_type)->mipi_data_mode;
}

static u32 data_type_to_lcd_out_format(int data_type)
{
    return data_type_info_get(data_type)->lcd_out_format;
}

static frameType data_type_to_lcd_in_format(int data_type)
{
    return data_type_info_get(data_type)->lcd_in_format;
}

static unsigned int data_type_to_lcd_in_bytesPP(int data_type)
{
    return data_type_info_get(data_type)->lcd_in_bytesPP;
}

/*
 * ****************************************************************************
 * ** TEMP DRIVER SUPPORT *****************************************************
 * ****************************************************************************
 */

#include <DrvIcbDefines.h>

#ifdef LCD_BLEN8
#define LCD_BLEN_BURST  D_LCD_DMA_LAYER_AXI_BURST_8
#else
#define LCD_BLEN_BURST  D_LCD_DMA_LAYER_AXI_BURST_16
#endif

typedef struct
{
    unsigned int MXI_DISPX_BASE_ADR;
    unsigned int IRQ_LCD_X;
    unsigned int LCDX_INT_CLEAR_ADR;
} LCDRegisters;

static const u32 csc_coef_lcd[N_COEFS] =
{
        1024, 0, 1436,
        1024, -352, -731,
        1024, 1814, 0,
        -179, 125, -226
};

//static LCDHandle *LCDHndl[1] = { NULL};

static const LCDRegisters lcds[1] =
{
    {
        .MXI_DISPX_BASE_ADR = LCD1_BASE_ADR,
	#ifdef Use_LcdGeneric_on_LRT
        .IRQ_LCD_X          = IRQ_LCD,
	#else
        .IRQ_LCD_X          = IRQ_DYNAMIC_0,
	#endif
        .LCDX_INT_CLEAR_ADR = LCD1_INT_CLEAR_ADR
    }
};

static int mipi_set_packet(
        int controler_n,
        int packet_n,
        int data_type,
        int data_mode,
        int width,
        int height
    )
{
    SET_REG_WORD(
            MIPI0_HS_CTRL_ADDR + controler_n * 0x100 + MIPI_TX_HS_SECT0_PH_OFFSET + packet_n*8,
              ((data_mode << D_MIPI_TX_HS_PH_DATA_MODE_POS      ) & D_MIPI_TX_HS_PH_DATA_MODE_MASK      )
            | ((data_type << D_MIPI_TX_HS_PH_DATA_IDENTIFIER_POS) & D_MIPI_TX_HS_PH_DATA_IDENTIFIER_MASK)
            | ((width * data_type_to_wc(data_type)              ) & D_MIPI_TX_HS_PH_WORD_COUNT_MASK     )
        );
    SET_REG_WORD(
            MIPI0_HS_CTRL_ADDR + controler_n * 0x100 + MIPI_TX_HS_SECT0_LINE_CNT_OFFSET + packet_n*8,
            height
        );
    return 0;
}

static int mipi_configure_packets(tDrvMipiControllerConfigStruct *mipi_cfg)
{
    int i;
    //printf("mipi_set_packet %d\n",  ARRAY_SIZE(mipi_cfg->txPacket));
    for (i = 0; i < ARRAY_SIZE(mipi_cfg->txPacket); i++) {
        mipi_set_packet(
                mipi_cfg->controllerNo,
                i,
                mipi_cfg->txPacket[i].dataType,
                mipi_cfg->txPacket[i].dataMode,
                mipi_cfg->width,
                mipi_cfg->txPacket[i].noPackets
            );
    }
    return 0;
}

static void LCDEnableDMA(LCDHandle *handle)
{
    if (handle->layerEnabled & VL1_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & VL2_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & GL1_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & GL2_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET) | D_LCD_DMA_LAYER_ENABLE);
    }
}

static void LCDDisableDMA(LCDHandle *handle)
{
    if (handle->layerEnabled & VL1_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_CFG_OFFSET) & ~D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & VL2_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_CFG_OFFSET) & ~D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & GL1_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_CFG_OFFSET) & ~D_LCD_DMA_LAYER_ENABLE);
    }
    if (handle->layerEnabled & GL2_ENABLED) {
        SET_REG_WORD(
            lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET,
            GET_REG_WORD_VAL(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_CFG_OFFSET) & ~D_LCD_DMA_LAYER_ENABLE);
    }
}

static void LCDSetLayerBuffersNormalRegisters(
        LCDHandle *hndl,
        int layer
    )
{
    //Layer 0
    if (layer == VL1 && hndl->currentFrameVL1 != NULL)
    {
        if (hndl->currentFrameVL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p1);
        if (hndl->currentFrameVL1->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CB_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p2);
        if (hndl->currentFrameVL1->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER0_DMA_START_CR_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL1->p3);
    }
    //Layer 1
    if (layer == VL2 && hndl->currentFrameVL2 != NULL)
    {
        if (hndl->currentFrameVL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p1);
        if (hndl->currentFrameVL2->p2 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CB_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p2);
        if (hndl->currentFrameVL2->p3 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER1_DMA_START_CR_ADR_OFFSET,
                (unsigned int )hndl->currentFrameVL2->p3);
    }
    //Layer 2 GL1
    if (layer == GL1 && hndl->currentFrameGL1 != NULL)
        if (hndl->currentFrameGL1->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER2_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameGL1->p1);

    //Layer 3 GL2
    if (layer == GL2 && hndl->currentFrameGL2 != NULL)
        if (hndl->currentFrameGL2->p1 != NULL)
            SET_REG_WORD(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_LAYER3_DMA_START_ADR_OFFSET,
                (unsigned int )hndl->currentFrameGL2->p1);
}

static void LCDConfigureLayersX(
        LCDHandle *hndl,
        const LCDDisplayCfg *lcdsp
    )
{
    u32 layer_cfg;
    u32 layer_ctrl;
    u32 layers_enabled;

    layer_ctrl =
          LCD_BLEN_BURST
        | D_LCD_DMA_LAYER_V_STRIDE_EN;

    layer_cfg =
          D_LCD_LAYER_FIFO_00
        | D_LCD_LAYER_32BPP
        | D_LCD_LAYER_FORMAT_YCBCR422LIN;

    layers_enabled = 0;

    SET_REG_WORD(lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET, 0);

    DrvLcdTiming(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
            hndl->lcdSpec->width,
            hndl->lcdSpec->hPulseWidth,
            hndl->lcdSpec->hBackP,
            hndl->lcdSpec->hFrontP,
            hndl->lcdSpec->height,
            hndl->lcdSpec->vPulseWidth,
            hndl->lcdSpec->vBackP,
            hndl->lcdSpec->vFrontP
        );

    if (hndl->layerEnabled & VL1_ENABLED) {
        DrvLcdLayer0(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                hndl->VL1frameSpec->width,
                hndl->VL1frameSpec->height,
                hndl->VL1offset.x,
                hndl->VL1offset.y,
                layer_cfg
            );

        DrvLcdDmaLayer0(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                (u32)NULL,
                hndl->VL1frameSpec->width,
                hndl->VL1frameSpec->height,
                hndl->VL1frameSpec->bytesPP,
                hndl->VL1frameSpec->stride,
                layer_ctrl
            );

        layers_enabled |= D_LCD_CTRL_VL1_ENABLE;
    }

    if (hndl->layerEnabled & VL2_ENABLED) {
        DrvLcdLayer1(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                hndl->VL2frameSpec->width,
                hndl->VL2frameSpec->height,
                hndl->VL2offset.x,
                hndl->VL2offset.y,
                layer_cfg
            );

        DrvLcdDmaLayer1(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                (u32)NULL,
                hndl->VL2frameSpec->width,
                hndl->VL2frameSpec->height,
                hndl->VL2frameSpec->bytesPP,
                hndl->VL2frameSpec->stride,
                layer_ctrl
            );

        layers_enabled |= D_LCD_CTRL_VL2_ENABLE;
    }

    if (hndl->layerEnabled & GL1_ENABLED) {
        DrvLcdLayer2(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                hndl->GL1FrameSpec->width,
                hndl->GL1FrameSpec->height,
                hndl->GL1offset.x,
                hndl->GL1offset.y,
                layer_cfg
            );

        DrvLcdDmaLayer2(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                (u32)NULL,
                hndl->GL1FrameSpec->width,
                hndl->GL1FrameSpec->height,
                hndl->GL1FrameSpec->bytesPP,
                hndl->GL1FrameSpec->stride,
                layer_ctrl
            );

        layers_enabled |= D_LCD_CTRL_GL1_ENABLE;
    }

    if (hndl->layerEnabled & GL2_ENABLED) {
        DrvLcdLayer3(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                hndl->GL2FrameSpec->width,
                hndl->GL2FrameSpec->height,
                hndl->GL2offset.x,
                hndl->GL2offset.y,
                layer_cfg
            );

        DrvLcdDmaLayer3(
                lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR,
                (u32)NULL,
                hndl->GL2FrameSpec->width,
                hndl->GL2FrameSpec->height,
                hndl->GL2FrameSpec->bytesPP,
                hndl->GL2FrameSpec->stride,
                layer_ctrl
            );

        layers_enabled |= D_LCD_CTRL_GL2_ENABLE;
    }

    SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_OUT_FORMAT_CFG_OFFSET,
            lcdsp->outputFormat
        );
    SET_REG_WORD(
            lcds[hndl->lcdNumber].MXI_DISPX_BASE_ADR + LCD_CONTROL_OFFSET,
              hndl->lcdSpec->control
            | layers_enabled
            | D_LCD_CTRL_TIM_GEN_ENABLE
            | D_LCD_CTRL_ENABLE
        );

    return;
}

static int LCDQueueFrameX(
        LCDHandle *handle,
        frameBuffer *VL1Buffer,
        frameBuffer *VL2Buffer,
        frameBuffer *GL1Buffer,
        frameBuffer *GL2Buffer
    )
{
    if (!LCDCanQueueFrame(handle)) {
        return -1;
    }

    handle->currentFrameVL1 = VL1Buffer;
    handle->currentFrameVL2 = VL2Buffer;
    handle->currentFrameGL1 = GL1Buffer;
    handle->currentFrameGL2 = GL2Buffer;

    LCDDisableDMA(handle);
    if (handle->layerEnabled & VL1_ENABLED) {
        LCDSetLayerBuffersNormalRegisters(handle, VL1);
    }
    if (handle->layerEnabled & VL2_ENABLED) {
        LCDSetLayerBuffersNormalRegisters(handle, VL2);
    }
    if (handle->layerEnabled & GL1_ENABLED) {
        LCDSetLayerBuffersNormalRegisters(handle, GL1);
    }
    if (handle->layerEnabled & GL2_ENABLED) {
        LCDSetLayerBuffersNormalRegisters(handle, GL2);
    }
    LCDEnableDMA(handle);

    // Trigger sending a frame
    SET_REG_WORD(lcds[handle->lcdNumber].MXI_DISPX_BASE_ADR + LCD_TIMING_GEN_TRIG_OFFSET, 1);

    return 0;
}

static void workaroundDereferenceNullCurrentFrame(LCDHandle *handle)
{
    static frameBuffer fb;
    //printf("workaroundDereferenceNullCurrentFrame  \n");
/*
 * A proper fix is available here:
 *
 * https://github.com/movidius/T2-Singijeon.git
 *
 * commit 28e23ba88b5c349459aab8190fc4233e7bb35946
 * Author: Alexandru-Sever Horin <alex.sever.h@gmail.com>
 * Date:   Sat Sep 27 10:47:55 2014 -0700
 *
 *     Add integration tests for singijeon platform. They are functional with Initkial commit of singijeon linux driver
 *
 */

    handle->currentFrameVL1 = &fb;
    handle->currentFrameVL2 = &fb;
    handle->currentFrameGL1 = &fb;
    handle->currentFrameGL2 = &fb;
}

static int lcd_frame_end_is_set(mipi_tx_hw_t *mipi_tx_hw)
{
    UNUSED(mipi_tx_hw);
    return GET_REG_WORD_VAL(LCD1_INT_STATUS_ADR) & D_LCD_INT_EOF;
}

static void lcd_frame_end_clear(mipi_tx_hw_t *mipi_tx_hw)
{
    UNUSED(mipi_tx_hw);
    SET_REG_WORD(LCD1_INT_CLEAR_ADR, D_LCD_INT_EOF);
}

/*
 * ****************************************************************************
 * ** END OF TEMP DRIVER SUPPORT **********************************************
 * ****************************************************************************
 */

struct mipi_tx_hw {
    int num_lanes;
    int mipi_clock;
    mipi_tx_hw_sent_callback_t *sent_callback;
    void *client_prv;
    int sending;
    int use_irq;

    tDrvMipiControllerConfigStruct mipi_cfg;

    struct {
        LCDHandle hdl;
        LCDDisplayCfg cfg;
        struct {
            int id;
            frameSpec frame_spec;
            LCDLayerOffset pos;
            frameBuffer frame_buffer;
        } layer[LAYERS_NUM];
        int layers_num;
    } lcd;
};

static mipi_tx_hw_t mipi_tx_hw_storage;

static void lcd_frame_ready_callback(
        frameBuffer *VL1Buffer,
        frameBuffer *VL2Buffer,
        frameBuffer *GL1Buffer,
        frameBuffer *GL2Buffer
    )
{
    UNUSED(VL1Buffer);
    UNUSED(VL2Buffer);
    UNUSED(GL1Buffer);
    UNUSED(GL2Buffer);

    //sss[iii++] = 0x08;
    mipi_tx_hw_t *mipi_tx_hw;
    mipi_tx_hw = &mipi_tx_hw_storage;
    mipi_tx_hw->sending = 0;
    //sss[iii++] = (uint32_t)&mipi_tx_hw->sent_callback;
    mipi_tx_hw->sent_callback(mipi_tx_hw, mipi_tx_hw->client_prv);
}

void mipi_tx_hw_poll(mipi_tx_hw_t *mipi_tx_hw)
{
    //printf("mipi_tx_hw_poll \n");
    if (!mipi_tx_hw->use_irq && mipi_tx_hw->sending) {
        if (lcd_frame_end_is_set(mipi_tx_hw)) {
            lcd_frame_end_clear(mipi_tx_hw);
            lcd_frame_ready_callback(NULL, NULL, NULL, NULL);
        }
    }
}

int mipi_tx_hw_send(
        mipi_tx_hw_t *mipi_tx_hw,
        void *client_prv,
        mipi_tx_hw_frame_t *frame
    )
{
    int i;
    //printf("mipi_tx_hw_send \n");
    //sss[iii++] = 0x03;
    mipi_tx_hw->client_prv = client_prv;

    /* MIPI */

    //sss[iii++] = 0xFF01;
    //sss[iii++] = 0xF801;
    //sss[iii++] = frame->height[i];

    mipi_tx_hw->mipi_cfg.height = 0;
    for (i = 0; (i < LAYERS_MAX) && frame->height[i]; i++) {
        //sss[iii++] = 100+i;
        mipi_tx_hw->mipi_cfg.txPacket[i].dataType = frame->data_type[i];
        mipi_tx_hw->mipi_cfg.txPacket[i].noPackets = frame->height[i];
        mipi_tx_hw->mipi_cfg.txPacket[i].dataMode =
            data_type_to_mipi_data_mode(mipi_tx_hw->mipi_cfg.txPacket[i].dataType);
        mipi_tx_hw->mipi_cfg.txPacket[i].bitsPerPixel =
            8 * data_type_to_wc(mipi_tx_hw->mipi_cfg.txPacket[i].dataType);
        mipi_tx_hw->mipi_cfg.height +=
            mipi_tx_hw->mipi_cfg.txPacket[i].noPackets;
        //sss[iii++] = 100+i;
    }
    //sss[iii++] = 0xFF02;
    for (; i < LAYERS_MAX; i++) {
        mipi_tx_hw->mipi_cfg.txPacket[i].noPackets = 0;
        mipi_tx_hw->mipi_cfg.txPacket[i].dataType = 0;
        mipi_tx_hw->mipi_cfg.txPacket[i].dataMode = MIPI_D_MODE_0;
        mipi_tx_hw->mipi_cfg.txPacket[i].bitsPerPixel = 0;
    }
    mipi_tx_hw->mipi_cfg.width = frame->width;
    //sss[iii++] = 0xFF03;
    /* LCD */
    mipi_tx_hw->lcd.cfg.width = mipi_tx_hw->mipi_cfg.width;
    mipi_tx_hw->lcd.cfg.height = mipi_tx_hw->mipi_cfg.height;
    mipi_tx_hw->lcd.cfg.outputFormat = /* The result should be the same with data_type_2... */
        data_type_to_lcd_out_format(mipi_tx_hw->mipi_cfg.txPacket[0].dataType);

    //sss[iii++] = 0xFF04;
    for (i = 0; (i < LAYERS_MAX) && frame->height[i]; i++) {
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.type =
            data_type_to_lcd_in_format(frame->data_type[i]);
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.height = frame->height[i];
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.width = frame->width;
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.stride = frame->line_size;
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.bytesPP =
            data_type_to_wc(frame->data_type[i]);
        mipi_tx_hw->lcd.layer[i].frame_buffer.p1 = frame->p[i];
        mipi_tx_hw->lcd.layer[i].frame_buffer.p2 = NULL;
        mipi_tx_hw->lcd.layer[i].frame_buffer.p3 = NULL;
        mipi_tx_hw->lcd.layer[i].pos.x = 0;
        mipi_tx_hw->lcd.layer[i].pos.y = i < 1 ? 0 :
              mipi_tx_hw->lcd.layer[i-1].pos.y
            + mipi_tx_hw->lcd.layer[i-1].frame_buffer.spec.height;
    }

    //sss[iii++] = 0xFF05;
    mipi_configure_packets(&mipi_tx_hw->mipi_cfg);

    mipi_tx_hw->lcd.hdl.layerEnabled = 0; /* TODO: better way to invalidate */
    for (i = 0; (i < mipi_tx_hw->lcd.layers_num) && frame->height[i]; i++) {
        LCDInitLayer(
                &mipi_tx_hw->lcd.hdl,
                mipi_tx_hw->lcd.layer[i].id,
                &mipi_tx_hw->lcd.layer[i].frame_buffer.spec,
                mipi_tx_hw->lcd.layer[i].pos
            );
    }
    //sss[iii++] = 0xFF06;
    LCDConfigureLayersX(
            &mipi_tx_hw->lcd.hdl,
            &mipi_tx_hw->lcd.cfg
        );
    mipi_tx_hw->sending = 1;
    //sss[iii++] = 0xFF07;
    lcd_frame_end_clear(mipi_tx_hw);
    assert(!LCDQueueFrameX(
            &mipi_tx_hw->lcd.hdl,
            &mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer,
            &mipi_tx_hw->lcd.layer[LAYER_PAYLOAD_1].frame_buffer,
            &mipi_tx_hw->lcd.layer[LAYER_PAYLOAD_2].frame_buffer,
            &mipi_tx_hw->lcd.layer[LAYER_PAYLOAD_3].frame_buffer
        ));
    //sss[iii++] = 0xFF;
    //sss[iii++] = 0x04;
    return 0;
}

void mipi_tx_hw_stop(mipi_tx_hw_t *mipi_tx_hw)
{
    UNUSED(mipi_tx_hw);
}

int mipi_tx_hw_start(mipi_tx_hw_t *mipi_tx_hw)
{
    UNUSED(mipi_tx_hw);
    return 0;
}

void mipi_tx_hw_destroy(mipi_tx_hw_t *mipi_tx_hw)
{
    //printf("mipi_tx_hw_destroy \n");
    LCDStop(&mipi_tx_hw->lcd.hdl);

    DrvCprSysDeviceAction(
            MSS_DOMAIN,
            ASSERT_RESET,
            0x0 | DEV_MSS_LCD
        );
}

mipi_tx_hw_t *mipi_tx_hw_create(
        int num_lanes,
        int mipi_clock,
        int use_irq,
        mipi_tx_hw_sent_callback_t *sent_callback
    )
{
    //sss[iii++] = 0x01;
    //printf("mipi_tx_hw_create \n");
    mipi_tx_hw_t *mipi_tx_hw;
    frameReadyLcdFn *frame_ready_fn;
    int i;

    mipi_tx_hw = &mipi_tx_hw_storage;

    mipi_tx_hw->num_lanes = num_lanes;
    mipi_tx_hw->mipi_clock = mipi_clock;
    mipi_tx_hw->use_irq = use_irq;
    mipi_tx_hw->sent_callback = sent_callback;
    mipi_tx_hw->sending = 0;

    mipi_tx_hw->lcd.layers_num = ARRAY_SIZE(mipi_tx_hw->lcd.layer);

    /* MIPI Config */
    mipi_tx_hw->mipi_cfg.controllerNo = mipi_tx_board_params->mipi_controller;
    mipi_tx_hw->mipi_cfg.type = MIPI_CSI;
    mipi_tx_hw->mipi_cfg.direction = MIPI_TX;
    mipi_tx_hw->mipi_cfg.noLanes = mipi_tx_hw->num_lanes;
    mipi_tx_hw->mipi_cfg.laneRateMbps = mipi_tx_hw->mipi_clock;
    mipi_tx_hw->mipi_cfg.width = SEND_OUT_MAX_WIDTH;
    mipi_tx_hw->mipi_cfg.refClkKHz = mipi_tx_board_params->mipi_ref_clock_kHz;

    mipi_tx_hw->mipi_cfg.txPacket[0].noPackets = SEND_OUT_HEADER_HEIGHT;
    mipi_tx_hw->mipi_cfg.txPacket[0].dataType = MIPI_TX_DEFAULT_HEADER_DATA_TYPE;
    mipi_tx_hw->mipi_cfg.txPacket[0].dataMode =
        data_type_to_mipi_data_mode(mipi_tx_hw->mipi_cfg.txPacket[0].dataType);
    mipi_tx_hw->mipi_cfg.txPacket[0].bitsPerPixel =
        8 * data_type_to_wc(mipi_tx_hw->mipi_cfg.txPacket[0].dataType);

    mipi_tx_hw->mipi_cfg.height = mipi_tx_hw->mipi_cfg.txPacket[0].noPackets;

    for (i = 1; i < mipi_tx_hw->lcd.layers_num; i++) {
        mipi_tx_hw->mipi_cfg.txPacket[i].noPackets = SEND_OUT_MAX_HEIGHT;
        mipi_tx_hw->mipi_cfg.txPacket[i].dataType = MIPI_TX_DEFAULT_DATA_TYPE;
        mipi_tx_hw->mipi_cfg.txPacket[i].dataMode =
            data_type_to_mipi_data_mode(mipi_tx_hw->mipi_cfg.txPacket[i].dataType);
        mipi_tx_hw->mipi_cfg.txPacket[i].bitsPerPixel =
            8 * data_type_to_wc(mipi_tx_hw->mipi_cfg.txPacket[i].dataType);
        mipi_tx_hw->mipi_cfg.height += mipi_tx_hw->mipi_cfg.txPacket[i].noPackets;
    }

    /* LCD Config */
    mipi_tx_hw->lcd.cfg.width = SEND_OUT_MAX_WIDTH;
    mipi_tx_hw->lcd.cfg.hPulseWidth = LCD_HOR_PULSE_WIDTH;
    mipi_tx_hw->lcd.cfg.hBackP = LCD_HOR_BACK_PORCH;
    mipi_tx_hw->lcd.cfg.hFrontP = LCD_HOR_FRONT_PORCH;
    mipi_tx_hw->lcd.cfg.vPulseWidth = LCD_VER_PULSE_WIDTH;
    mipi_tx_hw->lcd.cfg.vBackP = LCD_VER_BACK_PORCH;
    mipi_tx_hw->lcd.cfg.vFrontP = LCD_VER_FRONT_PORCH;
    mipi_tx_hw->lcd.cfg.outputFormat =
        data_type_to_lcd_out_format(MIPI_TX_DEFAULT_HEADER_DATA_TYPE);
    mipi_tx_hw->lcd.cfg.control =
              D_LCD_CTRL_OUTPUT_RGB
            | D_LCD_CTRL_TIM_GEN_ENABLE
            | D_LCD_CTRL_ALPHA_TOP_VL1
            | D_LCD_CTRL_ALPHA_MIDDLE_VL2
            | D_LCD_CTRL_ALPHA_BOTTOM_GL1
            | D_LCD_CTRL_ALPHA_BLEND_GL2
            | D_LCD_CTRL_PROGRESSIVE
            | D_LCD_CTRL_DISPLAY_MODE_ONE_SHOT;
    mipi_tx_hw->lcd.cfg.pixelClockkHz = 84000; /* TODO: calculate based on MIPI data rate */

    mipi_tx_hw->lcd.layer[LAYER_HEADER].id = VL1;
    mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.height =
            SEND_OUT_HEADER_HEIGHT;
    mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.width =
        SEND_OUT_MAX_WIDTH;
    mipi_tx_hw->lcd.layer[LAYER_HEADER].pos.x = 0;
    mipi_tx_hw->lcd.layer[LAYER_HEADER].pos.y = 0;
    mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.type =
        data_type_to_lcd_in_format(MIPI_TX_DEFAULT_HEADER_DATA_TYPE);
    mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.bytesPP =
        data_type_to_lcd_in_bytesPP(MIPI_TX_DEFAULT_DATA_TYPE);
    mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.stride =
          mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.width
        * data_type_to_lcd_in_bytesPP(
                mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.type
            );

    mipi_tx_hw->lcd.cfg.width =
        mipi_tx_hw->lcd.layer[LAYER_HEADER].frame_buffer.spec.height;

    for (i = LAYER_PAYLOAD_1; i < mipi_tx_hw->lcd.layers_num; i++) {
        switch (i) {
            case LAYER_PAYLOAD_1:
                mipi_tx_hw->lcd.layer[i].id = VL2;
                break;
            case LAYER_PAYLOAD_2:
                mipi_tx_hw->lcd.layer[i].id = GL1;
                break;
            case LAYER_PAYLOAD_3:
                mipi_tx_hw->lcd.layer[i].id = GL2;
                break;
        }

        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.height = SEND_OUT_MAX_HEIGHT;
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.width  = SEND_OUT_MAX_WIDTH;
        mipi_tx_hw->lcd.layer[i].pos.x = 0;
        mipi_tx_hw->lcd.layer[i].pos.y =
                              mipi_tx_hw->lcd.layer[i-1].pos.y
                            + mipi_tx_hw->lcd.layer[i-1].frame_buffer.spec.height;

        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.type =
                            data_type_to_lcd_in_format(MIPI_TX_DEFAULT_DATA_TYPE);

        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.bytesPP =
                            data_type_to_lcd_in_bytesPP(MIPI_TX_DEFAULT_DATA_TYPE);
        mipi_tx_hw->lcd.layer[i].frame_buffer.spec.stride =
                              mipi_tx_hw->lcd.layer[i].frame_buffer.spec.width
                            * data_type_to_lcd_in_bytesPP(mipi_tx_hw->lcd.layer[i].frame_buffer.spec.type);

        mipi_tx_hw->lcd.cfg.height += mipi_tx_hw->lcd.layer[i].frame_buffer.spec.height;
    }

    DrvCprSysDeviceAction(MSS_DOMAIN, DEASSERT_RESET, 0x0 | DEV_MSS_LCD);

    {
        eDrvMipiInitFuncRet ret;
        ret = DrvMipiInit(&mipi_tx_hw->mipi_cfg);
        if (ret)
           printf ("\n!!! ERROR !!! returned from DrvMipiInit: ret = %ld (0x%lx) \n", (uint32_t)ret, (uint32_t)ret);
    }

    LCDInit(
            &mipi_tx_hw->lcd.hdl,
            &mipi_tx_hw->lcd.cfg,
            NULL,
            LCD1
        );

    for (i = 0; i < mipi_tx_hw->lcd.layers_num; i++) {
        LCDInitLayer(
                &mipi_tx_hw->lcd.hdl,
                mipi_tx_hw->lcd.layer[i].id,
                &mipi_tx_hw->lcd.layer[i].frame_buffer.spec,
                mipi_tx_hw->lcd.layer[i].pos
            );
    }

    if (mipi_tx_hw->use_irq) {
        frame_ready_fn = lcd_frame_ready_callback;
    } else {
        frame_ready_fn = NULL;
    }

    LCDSetupCallbacks(
            &mipi_tx_hw->lcd.hdl,
            NULL,
            frame_ready_fn,
            NULL,
            NULL
        );

    workaroundDereferenceNullCurrentFrame(&mipi_tx_hw->lcd.hdl);

    LCDStartOneShot(&mipi_tx_hw->lcd.hdl);

    return mipi_tx_hw;
}

