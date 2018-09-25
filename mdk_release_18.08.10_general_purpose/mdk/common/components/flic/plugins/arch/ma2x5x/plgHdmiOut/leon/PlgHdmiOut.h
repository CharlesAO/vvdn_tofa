///
/// @file      PlgHdmiOut.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgHdmiOut header.
///

#ifndef _LCD_HDMI_API_H_
#define _LCD_HDMI_API_H_

#include <ImgFrame.h> //from Flic

#ifdef __cplusplus
extern "C" {
#endif

void lcdHdmiSendInit();
void lcdHdmiSendFini();
void lcdHdmiSendFrame(ImgFramePtr *frame, uint32_t outputId);

// in order to allow to not display all output to the lcd
extern uint32_t dbgEnableOutput;

#ifdef __cplusplus
}
#endif

#endif
