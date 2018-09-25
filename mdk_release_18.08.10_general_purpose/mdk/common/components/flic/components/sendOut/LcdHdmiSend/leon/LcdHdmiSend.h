///
/// @file      LcdHdmiSend.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     LcdHdmiSend header.
///

#ifndef _LCD_HDMI_API_H_
#define _LCD_HDMI_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "SendOutTransport.h"
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
void lcdHdmiSendCreate(void);
void lcdHdmiSentInit(void);
void lcdHdmiSendControl(uint32_t camEnBitMask, uint32_t frmTypeEnBitMask, uint32_t frmFmtEnBitMask);
void lcdHdmiSend(FrameT *frame, uint32_t outputId, uint32_t frmType, SendOutCbSent sendOutCbSent);
void lcdHdmiSendFini(void);

#ifdef __cplusplus
}
#endif


#endif //_LCD_HDMI_API_H_
