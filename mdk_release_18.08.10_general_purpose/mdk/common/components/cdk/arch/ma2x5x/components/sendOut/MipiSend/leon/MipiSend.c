/**************************************************************************************************

 @File         : mipiSend.c
 @Brief        : Contains all mipi tx sending frame interaction
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
#include "MipiSendApi.h"
#include "mipi_tx_hw.h"
#include "mipi_tx_board_params.h"
#include "DrvMss.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/
#define  REF_OSC_CLOCK           BRD_REF_OSC_CLOCK
#define  MIPI_TX_MIPI_CTRL_NO    BRD_MIPI_TX_MIPI_CTRL_NO
#define  MIPI_TX_NUM_LANES       BRD_MIPI_TX_NUM_LANES
#define  MIPI_TX_CLOCK           BRD_MIPI_TX_CLOCK
#define  MIPI_TX_USE_IRQ         BRD_MIPI_TX_USE_IRQ

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
static mipi_tx_board_params_t _mipi_tx_board_params = {
        .mipi_controller    = BRD_MIPI_TX_MIPI_CTRL_NO,
        .mipi_ref_clock_kHz = REF_OSC_CLOCK,
};
mipi_tx_board_params_t *mipi_tx_board_params = &_mipi_tx_board_params;

mipi_tx_hw_t    *mipi;

static MipiCfg_t mipi_config = {
	.ctrlNo = MIPI_TX_MIPI_CTRL_NO,
	.mss_device = DRV_MSS_LCD,
	.num_lanes = MIPI_TX_NUM_LANES,
	.tx_clock = MIPI_TX_CLOCK,
	.ref_clock_kHz = REF_OSC_CLOCK,
	.use_irq = MIPI_TX_USE_IRQ
};

/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
void sentCallback(mipi_tx_hw_t *mipi_tx_hw, void *client_prv);


/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/
//
void mipiSendCreate(MipiCfg_t *cfg)
{
    if(cfg != (MipiCfg_t *) NULL)
    {
        mipi_config.ctrlNo = cfg->ctrlNo;
        mipi_config.mss_device = cfg->mss_device;
        mipi_config.num_lanes = cfg->num_lanes;
        mipi_config.tx_clock = cfg->tx_clock;
        mipi_config.use_irq = cfg->use_irq;
        _mipi_tx_board_params.mipi_controller = cfg->ctrlNo;
        _mipi_tx_board_params.mipi_ref_clock_kHz = cfg->ref_clock_kHz;
    }
}

void mipiSendInit(void)
{
    DrvMssConnectMipiToDevice(mipi_config.ctrlNo, mipi_config.mss_device);
    mipi = mipi_tx_hw_create(mipi_config.num_lanes, mipi_config.tx_clock, mipi_config.use_irq, sentCallback);
}

void mipiSendFini(void)
{
    mipi_tx_hw_destroy(mipi);
}

//
void sentCallback(mipi_tx_hw_t *mipi_tx_hw, void *client_prv) {
    UNUSED(mipi_tx_hw);
    SendOutElement_t *task = client_prv;

    if (task->localCallback)
        task->localCallback(task);
}


//
void mipiSendSentFrame(SendOutElement_t *task)
{
    FrameT *frame;
    mipi_tx_hw_frame_t frameHw;

    assert(NULL != task);
    assert(NULL != task->buffer);

    frame    = task->buffer;

    frameHw.width = frame->stride[0]>>1;
    frameHw.line_size = frame->stride[0];
    frameHw.height[0] = SEND_OUT_HEADER_HEIGHT;
    frameHw.height[1] = frame->height[0];
    frameHw.height[2] = frame->height[1];
    frameHw.height[3] = 0;
    frameHw.data_type[0] = 49;
    frameHw.data_type[1] = 49;
    frameHw.data_type[2] = 49;
    frameHw.data_type[3] = 49;
    frameHw.p[0] = sendOutGetBufferHeader();
    frameHw.p[1] = frame->fbPtr[0];
    frameHw.p[2] = frame->fbPtr[1];
    frameHw.p[3] = 0;

    assert (0 == mipi_tx_hw_send(mipi, task, &frameHw));
}

