#ifndef ISPXUCONTROLSCOMMON_H_
#define ISPXUCONTROLSCOMMON_H_

#include "sendOutApi.h"
#include "IspCommon.h"

#define USB_bRequest_Video_UNDEFINED 0x00    /* undefined */
#define USB_bRequest_Video_SET_CUR   0x01    /* set current */
#define USB_bRequest_Video_GET_CUR   0x81    /* get current */
#define USB_bRequest_Video_GET_MIN   0x82    /* get minimum */
#define USB_bRequest_Video_GET_MAX   0x83    /* get maximum */
#define USB_bRequest_Video_GET_RES   0x84    /* get resolution */
#define USB_bRequest_Video_GET_LEN   0x85    /* get length space */
#define USB_bRequest_Video_GET_INFO  0x86    /* get information space */
#define USB_bRequest_Video_GET_DEF   0x87    /* get default space */

#define MAX_U16_VALUE   (1 << (16 - 0)) - 1
#define MAX_U8_VALUE    (1 <<  (8 - 0)) - 1
#define MAX_U4_VALUE    (1 <<  (4 - 0)) - 1
#define MAX_U10_VALUE   (1 << (10 - 0)) - 1
#define MIN_I16_VALUE  -(1 << (16 - 1)) - 0
#define MAX_I16_VALUE   (1 << (16 - 1)) - 1
#define MIN_I8_VALUE   -(1 <<  (8 - 1)) - 0
#define MAX_I8_VALUE    (1 <<  (8 - 1)) - 1
#define MAX_U32_VALUE   ((uint64_t)1 << (32 - 0)) - 1
#define MAX_U12_VALUE   (1 << (12 - 0)) - 1

#define IPIPE_LSC_PADDING      4
#define  MAX_LSC_SIZE          (64 + IPIPE_LSC_PADDING) * 64 * 4
#define  MAX_GAMMA_SIZE        512 * 4
#define  MAX_LTM_SIZE          4 * 16 * 16 * 16
#define  MAX_AE_AWB_STATS_SIZE 196608
#define  MAX_AF_STATS_SIZE     131072
#define  MAX_LTM_CURVES        16 * 8
#define  LTM_CURVES_WIDTH      8
#define  LTM_CURVES_HEIGHT     16
#define  MAX_FILENAME_LEN      256
#define  MAX_3DLUT_SIZE        4 * 16 * 16 * 16

#define TABLE_CHUNK_SIZE 496
#define TABLE_CHUNK   504
#define FILE_CHUNK_SIZE 512

typedef enum _XU_HOLD_EVENT_t
{
	HE_HOLD   = 0,
	HE_UNHOLD = 1,
	HE_UNDO   = 2,
	HE_REDO   = 3,
	HE_MAX    = 4
} XU_HOLD_EVENT_t;

typedef enum _XU_CONTROL_SELECTOR_t
{
	XCS_M2_GENERAL = 1,
	XCS_M2_BLC,
	XCS_M2_SIGMA_DENOISE,
	XCS_M2_LSC,
	XCS_M2_LSC_TABLE,
	XCS_M2_LSC_TABLE_TRANSFER,
	XCS_M2_RAW,
	XCS_M2_DEMOSAIC,
	XCS_M2_CHROMA_GEN,
	XCS_M2_MEDIAN,
	XCS_M2_SHARPEN,
	XCS_M2_LD,
	XCS_M2_LD_REF,
	XCS_M2_LTM,
	XCS_M2_DOG,
	XCS_M2_CHROMA_DENOISE,
	XCS_M2_COL_COM,
	XCS_M2_L3D_TABLE,
	XCS_M2_L3D_TABLE_TRANSFER,
	XCS_M2_COL_CONV,
	XCS_M2_GAMMA,
	XCS_M2_GAMMA_TABEL,
	XCS_M2_GAMMA_TABEL_TRANSFER,
	XCS_M2_MAX

} XU_CONTROL_SELECTOR_t;

typedef enum _XUX_CONTROL_SELECTOR_t
{
	XCS_MX_GENERAL        = 25,
    XCS_MX_BLC,
    XCS_MX_SIGMA_DENOISE,
    XCS_MX_LSC,
	XCS_MX_LSC_TABLE,
	XCS_MX_LSC_TABLE_TRANSFER,
    XCS_MX_RAW,
    XCS_MX_DEMOSAIC,
    XCS_MX_CHROMA_GEN,
    XCS_MX_MEDIAN,
    XCS_MX_SHARPEN,
    XCS_MX_LD,
    XCS_MX_LD_REF,
    XCS_MX_LTM,
    XCS_MX_DOG,
    XCS_MX_CHROMA_DENOISE,
    XCS_MX_COL_COM,
    XCS_MX_L3D_TABEL,
    XCS_MX_L3D_TABEL_TRANSFER,
    XCS_MX_COL_CONV,
    XCS_MX_GAMMA,
    XCS_MX_GAMMA_TABEL,
	XCS_MX_GAMMA_TABEL_TRANSFER,
	XUC_MX_MAX

} XUX_CONTROL_SELECTOR_t;

typedef enum _XU_CONFIGURATION_ID_t
{
	XCI_CURRENT_ID = 0,
	XCI_HOLD_ID    = 1,
	XCI_MAX        = 2
} XU_CONFIGURATION_ID_t;

typedef enum _HOLD_EVENT_ID_t
{
    HE_HOLD_PRESSED  = 0,
    HE_HOLD_RELEASED = 1,
    HE_HOLD_UNDO     = 2,
    HE_HOLD_REDO     = 3,
    HE_HOLD_MAX      = 4
} HOLD_EVENT_ID_t;

///////////////////////////////////////////////////////////////////////////////
// hold event custom controls
///////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_hold_event_t
{
    uint32_t holdEventId;
    uint32_t unused;
} uvcx_hold_event_t;

typedef struct _hold_event_control_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_hold_event_t cur;
    uvcx_hold_event_t min;
    uvcx_hold_event_t max;
    uvcx_hold_event_t res;
    uvcx_hold_event_t def;
} hold_event_control_t;

///////////////////////////////////////////////////////////////////////////////
// capture image
///////////////////////////////////////////////////////////////////////////////
typedef struct _uvcx_image_raw_metadata_t
{
    uint32_t image_Width;
    uint32_t image_Height;
    uint32_t image_Bpp;
    uint32_t image_Unused;
} uvcx_image_raw_metadata_t;

typedef struct _image_raw_metadata_control_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_image_raw_metadata_t cur;
    uvcx_image_raw_metadata_t min;
    uvcx_image_raw_metadata_t max;
    uvcx_image_raw_metadata_t res;
    uvcx_image_raw_metadata_t def;
} image_raw_metadata_control_t;

typedef struct _uvcx_image_yuv_metadata_t
{
	uint32_t imageY_Width;
    uint32_t imageY_Height;
    uint32_t imageUV_Width;
    uint32_t imageUV_Height;
} uvcx_image_yuv_metadata_t;

typedef struct _image_yuv_metadata_control_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_image_yuv_metadata_t cur;
    uvcx_image_yuv_metadata_t min;
    uvcx_image_yuv_metadata_t max;
    uvcx_image_yuv_metadata_t res;
    uvcx_image_yuv_metadata_t def;
} image_yuv_metadata_control_t;

//[Capture image upload control]
typedef struct _uvcx_image_upload_control_t
{
	uint8_t payload[FILE_CHUNK_SIZE];
} uvcx_image_upload_control_t;

typedef struct _image_upload_control_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_image_upload_control_t cur;
    uvcx_image_upload_control_t min;
    uvcx_image_upload_control_t max;
    uvcx_image_upload_control_t res;
    uvcx_image_upload_control_t def;
} image_upload_control_t;

typedef struct _uvcx_capture_image_t
{
    uint32_t   imageCaptureAction;
    uint32_t   imageCaptureState;
} uvcx_capture_image_t;

typedef struct _capture_image_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_capture_image_t cur;
    uvcx_capture_image_t min;
    uvcx_capture_image_t max;
    uvcx_capture_image_t res;
    uvcx_capture_image_t def;
} capture_image_t;


typedef struct _uvcx_manual_focus_data_t
{
    float fManualFocus;
} uvcx_manual_focus_data_t;

typedef struct _uvcx_manual_focus_ctrl_t
{
    uint8_t  u8Info;
    uint16_t u16Len;
    uvcx_manual_focus_data_t cur;
    uvcx_manual_focus_data_t min;
    uvcx_manual_focus_data_t max;
    uvcx_manual_focus_data_t res;
    uvcx_manual_focus_data_t def;

} uvcx_manual_focus_ctrl_t;

typedef struct _uvcx_start_stop_myriad_data_t
{
	uint32_t u32Event;
    uint32_t u32SensorType;
} uvcx_start_stop_myriad_data_t;

typedef struct _uvcx_start_myriad_ctrl_t
{
	uint8_t u8Info;
	uint16_t u16Len;
    uvcx_start_stop_myriad_data_t cur;
    uvcx_start_stop_myriad_data_t min;
    uvcx_start_stop_myriad_data_t max;
    uvcx_start_stop_myriad_data_t res;
    uvcx_start_stop_myriad_data_t def;
} uvcx_start_stop_myriad_ctrl_t;


#endif //ISPXUCONTROLSCOMMON_H_
