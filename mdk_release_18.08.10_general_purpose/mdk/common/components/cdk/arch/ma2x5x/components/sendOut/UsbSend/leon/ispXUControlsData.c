#include "ispXUControlsData.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "ispXUControlsM2.h"
#include "ispXUControlsMX.h"

static XU_CONFIGURATION_ID_t g_xuConfigId = XCI_CURRENT_ID;


static hold_event_control_t holdEventData =
	{3, 8,
	{0, 0},// cur
	{0, 0},// min
	{MAX_U32_VALUE, MAX_U32_VALUE},// max
	{1, 1},// res
	{0, 0},// def
	};

static image_raw_metadata_control_t rawImageMetadataData =
	{3,  16,
	{ 0, 0, 0, 0},//cur
	{ 0, 0, 0, 0},//min
	{MAX_U32_VALUE, MAX_U32_VALUE, MAX_U32_VALUE,MAX_U32_VALUE},//max
	{ 1, 1, 1, 1},//res
	{ 0, 0, 0, 0},//def
	};

static image_yuv_metadata_control_t yuvImageMetadataData =
	{3, 16,
	{ 0, 0, 0, 0},//cur
	{ 0, 0, 0, 0},//min
	{MAX_U32_VALUE,MAX_U32_VALUE,
	 MAX_U32_VALUE,MAX_U32_VALUE},//max
	{ 1, 1, 1, 1},//res
	{ 0, 0, 0, 0},//def
	};


static capture_image_t captureImageData =
	{ 3, 8,
	{0,0},//cur
	{0,0},//min
	{5,5},//max
	{1,1},//res
	{0,0},//def
	};

static uvcx_manual_focus_ctrl_t manualFocusData =
	{
       3, 4,
	   {0},//cur
	   {0},//min
	   {0},//max
	   {0},//res
	   {0} //def
	};

static uvcx_start_stop_myriad_ctrl_t startStopData =
	{
       3, 8,
	   {0,0},//cur
	   {0,0},//min
	   {0,0},//max
	   {0,0},//res
	   {0,0} //def
	};

static image_upload_control_t rawImageUploadData;
static image_upload_control_t yuvImageYUploadData;
static image_upload_control_t yuvImageUVUploadData;
static uint8_t * pu8RawImage_Buffer   = NULL;
static uint8_t * pu8YuvImage_YBuffer  = NULL;
static uint8_t * pu8YuvImage_UVBuffer = NULL;

static uint8_t * pu8RawImage_Pointer        = 0;
static uint8_t * pu8YUVImage_YPlanePointer  = 0;
static uint8_t * pu8YUVImage_UVPlanePointer = 0;

static uint8_t bCommandPending = 0;

static void copyYUVImageBuffer(uint8_t planeNb)
{
	// plane Y
	if (planeNb == 0)
	{
		memmove(yuvImageYUploadData.cur.payload, pu8YUVImage_YPlanePointer, 512);
		pu8YUVImage_YPlanePointer += 512;
	}
	// plane UV
	else if (planeNb == 1)
	{
		memmove(yuvImageUVUploadData.cur.payload, pu8YUVImage_UVPlanePointer, 512);
		pu8YUVImage_UVPlanePointer += 512;
	}
}

static void copyRawImageBuffer()
{
	memmove(rawImageUploadData.cur.payload, pu8RawImage_Pointer, 512);
	pu8RawImage_Pointer += 512;
}


void initControls()
{
	printf("initControls\n");
	Init_M2_Controls();
	Init_MX_Controls();

    // raw image upload control
    rawImageUploadData.u8Info = 3;
    rawImageUploadData.u16Len = FILE_CHUNK_SIZE;

    memset(rawImageUploadData.cur.payload, 0, FILE_CHUNK_SIZE);
    memset(rawImageUploadData.min.payload, 0, FILE_CHUNK_SIZE);
    memset(rawImageUploadData.max.payload, 0, FILE_CHUNK_SIZE);
    memset(rawImageUploadData.res.payload, 0, FILE_CHUNK_SIZE);
    memset(rawImageUploadData.def.payload, 0, FILE_CHUNK_SIZE);

    // yuv image upload control
    yuvImageYUploadData.u8Info = 3;
    yuvImageYUploadData.u16Len = FILE_CHUNK_SIZE;

    memset(yuvImageYUploadData.cur.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageYUploadData.min.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageYUploadData.max.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageYUploadData.res.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageYUploadData.def.payload, 0, FILE_CHUNK_SIZE);

    yuvImageUVUploadData.u8Info = 3;
    yuvImageUVUploadData.u16Len = FILE_CHUNK_SIZE;

    memset(yuvImageUVUploadData.cur.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageUVUploadData.min.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageUVUploadData.max.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageUVUploadData.res.payload, 0, FILE_CHUNK_SIZE);
    memset(yuvImageUVUploadData.def.payload, 0, FILE_CHUNK_SIZE);

}

void onEvent(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload)
{
	onEventM2(controlSelector, request, pLen, pPayload);
	onEventMX(controlSelector, request, pLen, pPayload);

	uint8_t *pCur = NULL;
    uint8_t *pMin = NULL;
    uint8_t *pMax = NULL;
    uint8_t *pRes = NULL;
    uint8_t *pDef = NULL;
    uint8_t u8Info  = 0;
    uint16_t u16Len =  0;

    if (controlSelector < 50) return;

    printf("[ispXUControlsData] start/stop evt %d\n",controlSelector);

    switch (controlSelector)
    {
        case XUC_CAPTURE_IMAGE:
        {
            u8Info = captureImageData.u8Info;
            u16Len = captureImageData.u16Len;
            pCur   = (uint8_t *)&captureImageData.cur;
            pMin   = (uint8_t *)&captureImageData.min;
            pMax   = (uint8_t *)&captureImageData.max;
            pRes   = (uint8_t *)&captureImageData.res;
            pDef   = (uint8_t *)&captureImageData.def;
            break;
        }
        case XUC_RAW_IMAGE_METADATA:
        {
            u8Info = rawImageMetadataData.u8Info;
            u16Len = rawImageMetadataData.u16Len;
            pCur   = (uint8_t *)&rawImageMetadataData.cur;
            pMin   = (uint8_t *)&rawImageMetadataData.min;
            pMax   = (uint8_t *)&rawImageMetadataData.max;
            pRes   = (uint8_t *)&rawImageMetadataData.res;
            pDef   = (uint8_t *)&rawImageMetadataData.def;
            break;
        }
        case XUC_YUV_IMAGE_METADATA:
        {
            u8Info = yuvImageMetadataData.u8Info;
            u16Len = yuvImageMetadataData.u16Len;
            pCur   = (uint8_t *)&yuvImageMetadataData.cur;
            pMin   = (uint8_t *)&yuvImageMetadataData.min;
            pMax   = (uint8_t *)&yuvImageMetadataData.max;
            pRes   = (uint8_t *)&yuvImageMetadataData.res;
            pDef   = (uint8_t *)&yuvImageMetadataData.def;
            break;
        }
        case XUC_RAW_IMAGE_UPLOAD:
        {
            u8Info = rawImageUploadData.u8Info;
            u16Len = rawImageUploadData.u16Len;
            pCur   = (uint8_t *)&rawImageUploadData.cur;
            pMin   = (uint8_t *)&rawImageUploadData.min;
            pMax   = (uint8_t *)&rawImageUploadData.max;
            pRes   = (uint8_t *)&rawImageUploadData.res;
            pDef   = (uint8_t *)&rawImageUploadData.def;
            break;
        }
        case XUC_YUV_IMAGE_Y_UPLOAD:
        {
            u8Info = yuvImageYUploadData.u8Info;
            u16Len = yuvImageYUploadData.u16Len;
            pCur   = (uint8_t *)&yuvImageYUploadData.cur;
            pMin   = (uint8_t *)&yuvImageYUploadData.min;
            pMax   = (uint8_t *)&yuvImageYUploadData.max;
            pRes   = (uint8_t *)&yuvImageYUploadData.res;
            pDef   = (uint8_t *)&yuvImageYUploadData.def;
            break;
        }
        case XUC_YUV_IMAGE_UV_UPLOAD:
        {
            u8Info = yuvImageUVUploadData.u8Info;
            u16Len = yuvImageUVUploadData.u16Len;
            pCur   = (uint8_t *)&yuvImageUVUploadData.cur;
            pMin   = (uint8_t *)&yuvImageUVUploadData.min;
            pMax   = (uint8_t *)&yuvImageUVUploadData.max;
            pRes   = (uint8_t *)&yuvImageUVUploadData.res;
            pDef   = (uint8_t *)&yuvImageUVUploadData.def;
            break;
        }
        case XUC_HOLD:
        {
            u8Info = holdEventData.u8Info;
            u16Len = holdEventData.u16Len;
            pCur   = (uint8_t *)&holdEventData.cur;
            pMin   = (uint8_t *)&holdEventData.min;
            pMax   = (uint8_t *)&holdEventData.max;
            pRes   = (uint8_t *)&holdEventData.res;
            pDef   = (uint8_t *)&holdEventData.def;
            break;
        }

        case XUC_MANUAL_FOCUS:
        {
            u8Info = manualFocusData.u8Info;
            u16Len = manualFocusData.u16Len;
            pCur   = (uint8_t *)&manualFocusData.cur;
            pMin   = (uint8_t *)&manualFocusData.min;
            pMax   = (uint8_t *)&manualFocusData.max;
            pRes   = (uint8_t *)&manualFocusData.res;
            pDef   = (uint8_t *)&manualFocusData.def;
            break;
        }

        case XUC_START_STOP_MYRIAD:
        {
        	printf("[ispXUControlsData] start/stop evt\n");
            u8Info = startStopData.u8Info;
            u16Len = startStopData.u16Len;
            pCur   = (uint8_t *)&startStopData.cur;
            pMin   = (uint8_t *)&startStopData.min;
            pMax   = (uint8_t *)&startStopData.max;
            pRes   = (uint8_t *)&startStopData.res;
            pDef   = (uint8_t *)&startStopData.def;
            break;
        }


    };

    if ((u8Info == 0)||(u16Len == 0)||
        (pCur == NULL)||(pMin == NULL)||(pMax == NULL)||(pRes == NULL)||(pDef == NULL))
    {
    	printf("[ispXUControlsData] Invalid Request controlSelector %d request %d len %d\n", controlSelector, request, *pLen);
    	return;
    }

    if (USB_bRequest_Video_GET_CUR == request)
    {
    	if (XUC_YUV_IMAGE_Y_UPLOAD == controlSelector)
    	{
    		copyYUVImageBuffer(0);
    	}
    	else if (XUC_YUV_IMAGE_UV_UPLOAD == controlSelector)
    	{
    		copyYUVImageBuffer(1);
    	}
    	else if (XUC_RAW_IMAGE_UPLOAD == controlSelector) {
    		copyRawImageBuffer();
    	}
    }

    if (USB_bRequest_Video_SET_CUR == request)
    	if (XUC_START_STOP_MYRIAD == controlSelector)
    	{
    		bCommandPending = 1;
    	}


    switch (request)
    {
    	case USB_bRequest_Video_GET_LEN: { *pLen = 2;      memmove(pPayload, &u16Len, 2);   break;}
	    case USB_bRequest_Video_GET_INFO:{ *pLen = 2;      memmove(pPayload, &u8Info, 1);   break;}
	    case USB_bRequest_Video_GET_MIN: { *pLen = u16Len; memmove(pPayload, pMin, u16Len); break;}
	    case USB_bRequest_Video_GET_MAX: { *pLen = u16Len; memmove(pPayload, pMax, u16Len); break;}
	    case USB_bRequest_Video_GET_RES: { *pLen = u16Len; memmove(pPayload, pRes, u16Len); break;}
	    case USB_bRequest_Video_GET_DEF: { *pLen = u16Len; memmove(pPayload, pDef, u16Len); break;}
	    case USB_bRequest_Video_GET_CUR: { *pLen = u16Len; memmove(pPayload, pCur, u16Len); break;}
	    case USB_bRequest_Video_SET_CUR: { *pLen = u16Len; memmove(pCur, pPayload, u16Len); break;}
	    default: break;
	 }


    if ((XUC_HOLD == controlSelector)&&
        (request == USB_bRequest_Video_SET_CUR))
    {
    	switch (holdEventData.cur.holdEventId) {
			case HE_HOLD_PRESSED:
				//copyConfiguration();
				g_xuConfigId = XCI_HOLD_ID;
				break;
			case HE_HOLD_RELEASED:
				g_xuConfigId = XCI_CURRENT_ID;
				break;
			case HE_HOLD_UNDO:
				g_xuConfigId = XCI_CURRENT_ID;
				break;
			case HE_HOLD_REDO:
				g_xuConfigId = XCI_HOLD_ID;
				break;

			default:
				break;
		}
    }

}

void getManualFocus(float *pfManualFocus)
{
	*pfManualFocus = manualFocusData.cur.fManualFocus;
}

void getCommand(uint32_t *pCommandId, uint32_t *pCameraId, uint32_t *pCameraCfgId)
{
	UNUSED(pCommandId);
	UNUSED(pCameraId);
	UNUSED(pCameraCfgId);
}

void getXUActionControlValues(uint32_t * pu32Action)
{
	*pu32Action  = 0;
	*pu32Action = captureImageData.cur.imageCaptureAction;
	captureImageData.cur.imageCaptureAction = 0;
}

void setXUStateControlValues(uint32_t u32State)
{
	printf("%ld \n", u32State);
	captureImageData.cur.imageCaptureState = u32State;
}

void setXURawImageBufferValues(uint8_t * pu8RawImageBuffer,  uint32_t u32RawImageWidth,  uint32_t u32RawImageHeight, uint32_t u32Bpp)
{
	pu8RawImage_Buffer   = pu8RawImageBuffer;
	pu8RawImage_Pointer  = pu8RawImageBuffer;

	printf("%ld %ld %ld \n", u32RawImageWidth, u32RawImageHeight, u32Bpp);
	rawImageMetadataData.cur.image_Width  = u32RawImageWidth;
	rawImageMetadataData.cur.image_Height = u32RawImageHeight;
	rawImageMetadataData.cur.image_Bpp    = u32Bpp;

}

void setXUYuvImageBufferValues(uint8_t * pu8YuvImageYBuffer,  uint32_t u32YuvImageYWidth,  uint32_t u32YuvImageYHeight,
			 				   uint8_t * pu8YuvImageUVBuffer, uint32_t u32YuvImageUVWidth, uint32_t u32YuvImageUVHeight)
{
	pu8YuvImage_YBuffer  = pu8YuvImageYBuffer;
	pu8YuvImage_UVBuffer = pu8YuvImageUVBuffer;

	pu8YUVImage_YPlanePointer  = pu8YuvImageYBuffer;
    pu8YUVImage_UVPlanePointer = pu8YuvImageUVBuffer;

	printf("%ld %ld %ld %ld\n", u32YuvImageYWidth, u32YuvImageYHeight, u32YuvImageUVWidth, u32YuvImageUVHeight);
    yuvImageMetadataData.cur.imageY_Width   = u32YuvImageYWidth;
	yuvImageMetadataData.cur.imageY_Height  = u32YuvImageYHeight;
	yuvImageMetadataData.cur.imageUV_Width  = u32YuvImageUVWidth;
	yuvImageMetadataData.cur.imageUV_Height = u32YuvImageUVHeight;
}

void getConfig(icIspConfig *pCfg)
{
	getConfigM2(pCfg);
}
