#ifndef _ISP_XUCONTROLS_DATA_H_
#define _ISP_XUCONTROLS_DATA_H_

#include "sendOutApi.h"
#include "IspCommon.h"
#include "ispXUControlsCommon.h"

typedef enum _XUC_CONTROL_SELECTOR_t
{
	XUC_CAPTURE_IMAGE        = 50,
	XUC_RAW_IMAGE_METADATA,
	XUC_YUV_IMAGE_METADATA,
	XUC_RAW_IMAGE_UPLOAD,
    XUC_YUV_IMAGE_Y_UPLOAD,
    XUC_YUV_IMAGE_UV_UPLOAD,
    XUC_HOLD,
	XUC_MANUAL_FOCUS,
	XUC_START_STOP_MYRIAD,
	XCS_MAX
} XUC_CONTROL_SELECTOR_t;


void initControls();
void getConfig(icIspConfig *pCfg);

void getCommand(uint32_t *pCommandId, uint32_t *pCameraId, uint32_t *pCameraCfgId);
void getManualFocus(float *pfManualFocus);
void getXUActionControlValues(uint32_t * pu32Action);
void setXUStateControlValues(uint32_t u32State);
void setXURawImageBufferValues(uint8_t * pu8RawImageBuffer,  uint32_t u32RawImageWidth,  uint32_t u32RawImageHeight, uint32_t u32Bpp);
void setXUYuvImageBufferValues(uint8_t * pu8YuvImageYBuffer,  uint32_t u32YuvImageYWidth,  uint32_t u32YuvImageYHeight,
			 				   uint8_t * pu8YuvImageUVBuffer, uint32_t u32YuvImageUVWidth, uint32_t u32YuvImageUVHeight);

void onEvent(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload);

#endif //_ISP_XUCONTROLS_DATA_H_
