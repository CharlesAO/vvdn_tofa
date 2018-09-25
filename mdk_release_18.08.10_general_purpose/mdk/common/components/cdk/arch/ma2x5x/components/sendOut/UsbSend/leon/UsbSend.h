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


// 1: Includes
// ----------------------------------------------------------------------------
#ifndef USBSEND_H
#define USBSEND_H

#include "mv_types.h"
#include "sendOutApi.h"

#ifdef SEND_OUT_USB_SEND_USE_XU
#include "ispXUControlsData.h"
#include "ispUserControlsData.h"
#endif

// if defined it prints info about frames transmission
#define DBG_ERROR_PRINTF(...)            printf(__VA_ARGS__)

#ifdef DBG_USBSEND
    #define DBG_PRINTF(...)            printf(__VA_ARGS__)
#else
    #define DBG_PRINTF(...)
#endif

typedef struct
{
    u8            busy;
    u8            filled;
    u8            sendInProgress;
    u8            notSent;
    SendOutElement_t *task;
} sUsbTest;

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

void UsbSendCreate(UsbCfg_t *cfg);
static inline void UsbSendInit(void) {/*printf("\n\n UsbSendInit \n\n\n");*/};
void UsbSendFrame(SendOutElement_t *task);
static inline void UsbSendFini(void) {};

#define TRANSPORT_create        UsbSendCreate
#define TRANSPORT_init          UsbSendInit
#define TRANSPORT_SendFrame     UsbSendFrame
#define TRANSPORT_Fini          UsbSendFini

#define TRANSPORT_PARAM         usbCfg

#ifdef SEND_OUT_USB_SEND_USE_XU
///////////////////////////////////////////////////////////////////////////////
// XU Controls functions
///////////////////////////////////////////////////////////////////////////////
void getXUControlsConfig(icIspConfig *pCfg);
void getUserControls(uint16_t * pu16Exposure, uint16_t * pu16Gain);

void getXUCommand(uint32_t *pCommandId, uint32_t *pCameraId, uint32_t *pCameraCfgId);
void getXUManualFocus(float *pfManualFocus);
void getXUActionControl(uint32_t * pu32Action);
void setXUStateControl(uint32_t u32State);
void setXURawImageBuffer(uint8_t * pu8RawImageBuffer,
		                 uint32_t u32RawImageWidth,
		                 uint32_t u32RawImageHeight,
		                 uint32_t u32Bpp);
void setXUYuvImageBuffer(uint8_t * pu8YuvImageYBuffer,
		                 uint32_t u32YuvImageYWidth,
		                 uint32_t u32YuvImageYHeight,
				         uint8_t * pu8YuvImageUVBuffer,
						 uint32_t u32YuvImageUVWidth,
						 uint32_t u32YuvImageUVHeight);
#endif

#endif // USBSEND_H
