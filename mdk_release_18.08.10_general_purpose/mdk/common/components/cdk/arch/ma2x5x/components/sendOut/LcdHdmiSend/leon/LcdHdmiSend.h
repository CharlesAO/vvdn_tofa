///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup
/// @{
/// @brief
///
///
///


#ifndef LCD_HDMI_SEND_H
#define LCD_HDMI_SEND_H

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "sendOutApi.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
void lcdHdmiSendCreate(HdmiCfg_t *cfg);
void lcdHdmiSentInit(void);
void lcdHdmiSentFrame(SendOutElement_t *task);
void lcdHdmiSendFini(void);


#define TRANSPORT_create        lcdHdmiSendCreate
#define TRANSPORT_init          lcdHdmiSentInit
#define TRANSPORT_SendFrame     lcdHdmiSentFrame
#define TRANSPORT_Fini          lcdHdmiSendFini

#define TRANSPORT_PARAM         hdmiCfg

#ifdef __cplusplus
}
#endif


#endif //LCD_HDMI_SEND_H
