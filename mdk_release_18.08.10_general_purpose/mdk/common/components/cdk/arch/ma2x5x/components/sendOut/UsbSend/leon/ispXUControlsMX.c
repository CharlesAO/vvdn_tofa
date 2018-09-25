#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ispXUControlsMX.h"

static uvcx_mx_general_ctrl_t uvcx_mx_generalCtrl =
	{
		3, sizeof(uvcx_mx_general_data_t),
		{"", 0, 0, 0, 0, 0},//cur
		{"", 0, 0, 0, 0, 0},//min
		{"", 0, 0, 0, 0, 0},//max
		{"", 1, 1, 1, 1, 1},//res
		{"", 0, 0, 0, 0, 0},//def
	};

static uvcx_mx_blc_ctrl_t uvcx_mx_blcCtrl =
	{    3, 16,
		{0, 0, 0, 0},//cur
		{0, 0, 0, 0},//min
		{MAX_U16_VALUE, MAX_U16_VALUE,
		 MAX_U16_VALUE, MAX_U16_VALUE},//max
		{1, 1, 1, 1},//res
		{0, 0, 0, 0} //def
	};

static uvcx_mx_sgmdns_ctrl_t uvcx_mx_sgmdnsCtrl =
	{
		3, 36,
		{0, 0, 0, 0, 0, 0, 0, 0, 0},//cur
		{0, 0, 0, 0, 0, 0, 0, 0, 0},//min
		{MAX_U16_VALUE,
		 MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
		 MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE},//max
		{1, 1, 1, 1, 1, 1, 1, 1, 1},//res
		{0, 0, 0, 0, 0, 0, 0, 0, 0} //def
	};

static uvcx_mx_lsc_ctrl_t uvcx_mx_lscCtrl =
		{
			3, 12,
			{3, 3, 3}, //cur
			{3, 3, 3}, //min
			{MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE}, //max
			{1, 1, 1}, //res
			{3, 3, 3}, //def
		};

static uvcx_mx_lsctable_ctrl_t         uvcx_mx_lsctableCtrl;
static uvcx_mx_lsctabletransfer_ctrl_t uvcx_mx_lsctabletransferCtrl;

static uvcx_mx_raw_ctrl_t uvcx_mx_rawCtrl =
	{
		3, 76,
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //cur
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //min
		{MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE,                //gain
		 MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE,                // clamp(saturation)
		 MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE, MAX_U16_VALUE, // imbal
		 MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U16_VALUE, // dpc
		 MAX_U16_VALUE //output bits
		}, //max
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //res
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  //def
	};

static uvcx_mx_demosaic_ctrl_t uvcx_mx_demosaicCtrl =
	{
	    3, sizeof(uvcx_mx_demosaic_data_t),
		{0,0,0,0,0,0},//cur
		{0,0,0,0,0,0},//min
		{0,0,0,0,0,0},//max
		{1,1,1,1,1,1},//res
		{0,0,0,0,0,0}//def
	};

static uvcx_mx_chromagen_ctrl_t uvcx_mx_chromagenCtrl =
	{
		3, sizeof(uvcx_mx_chromagen_data_t),
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //cur
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //min
		{MAX_U8_VALUE,
	     MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
         MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
         MAX_U8_VALUE, MAX_U8_VALUE,
         MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
         MAX_U8_VALUE, MAX_U8_VALUE,
         MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE}, //max
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //res
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //def
	};

static uvcx_mx_median_ctrl_t uvcx_mx_medianCtrl =
	{
		3, 12,
		{0, 0, 0},//cur
		{0,                       0, MIN_I8_VALUE},//min
		{MAX_U8_VALUE, MAX_U8_VALUE, MAX_I8_VALUE},//max
		{1, 1, 1},//res
		{0, 0, 0},//def
	};

static uvcx_mx_sharpen_ctrl_t uvcx_mx_sharpenCtrl =
	{
			3, 88,
			{0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, { 0, 0, 0}, {0, 0, 0}, {0,0,0,0}, 0,0,0},//cur
			{0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, { 0, 0, 0}, {0, 0, 0}, {0,0,0,0}, 0,0,0},//min
			{0.0, 0.0, 0.0, 0.0,
					0, 0, 0, 0, 0,
					{ MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE},
					{0, 0, 0},
					{ MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE},
					0,0,0},//max
					{0.0, 0.0, 0.0, 0.0, 1, 1, 1, 1, 1, { 1, 1, 1}, {1, 1, 1}, {1,1,1,1}, 1,1,1},//res
					{0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, { 0, 0, 0}, {0, 0, 0}, {0,0,0,0}, 0,0,0} //def

	};

static uvcx_mx_lumadns_ctrl_t    uvcx_mx_lumadnsCtrl;
static uvcx_mx_lumadnsref_ctrl_t uvcx_mx_lumadnsrefCtrl;

static uvcx_mx_ltm_ctrl_t        uvcx_mx_ltmCtrl;
static uvcx_mx_dogdenoise_ctrl_t uvcx_mx_dogdenoiseCtrl;

static uvcx_mx_chromadenoise_ctrl_t uvcx_mx_chromadenoiseCtrl =
	{
			3, sizeof(uvcx_mx_chromadenoise_data_t),
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//cur
			//min
			{0,0,0,0, //dark
			 0,0,0,0, //bright
			 0,0,     //denoise
			 0,0,0,   //grey
			 0,0,     //grey desat
			 0,0,     //limit henable
			 0,0,0},  //convcoeff
			//max
			{MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,//dark
			 MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,//bright
			 MAX_U8_VALUE,MAX_U8_VALUE,             //denoise
			 MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,//grey
			 MAX_U8_VALUE,MAX_U8_VALUE,             //grey desat

			 MAX_U8_VALUE,MAX_U8_VALUE,             //limit henable
			 MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE},//convcoeff

			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//res
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},//def
	};

static uvcx_mx_colorcombine_ctrl_t uvcx_mx_colorcombineCtrl =
	{
		3, sizeof(uvcx_mx_colorcombine_data_t),
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, 0, 0, 0},//cur
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, 0, 0, 0},//min
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, 0, 0, 0},//max
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, 1, 1, 1},//res
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, 0, 0, 0},//def
	};

static uvcx_mx_lut3dtable_ctrl_t         uvcx_mx_lut3dtableCtrl;
static uvcx_mx_lut3dtabletransfer_ctrl_t uvcx_mx_lut3dtabletransferCtrl;

static uvcx_mx_colorconvert_ctrl_t  uvcx_mx_colorconvertCtrl =
	{
		3, sizeof(uvcx_mx_colorconvert_data_t),
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}},//cur
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}},//min
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}},//max
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}},//res
		{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},{0.0, 0.0, 0.0}},//def
	};

static uvcx_mx_gamma_ctrl_t uvcx_mx_gammaCtrl =
	{
			3, sizeof(uvcx_mx_gamma_data_t),
			{512},//cur
			{512},//min
			{512},//max
			{1},  //res
			{512},//def
	};

static uvcx_mx_gammatable_ctrl_t         uvcx_mx_gammatableCtrl;
static uvcx_mx_gammatabletransfer_ctrl_t uvcx_mx_gammatabletransferCtrl;

static char updateMXTable[XUC_MX_MAX];

void Init_MX_Controls()
{
	printf("Init_MX_Controls\n");
	uvcx_mx_lsctableCtrl.u8Info = 3;
	uvcx_mx_lsctableCtrl.u16Len = MAX_LSC_SIZE ;//* sizeof(uint16_t);
	memset(&uvcx_mx_lsctableCtrl.cur, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_mx_lsctableCtrl.min, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_mx_lsctableCtrl.max, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_mx_lsctableCtrl.res, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_mx_lsctableCtrl.def, 0, sizeof(uint16_t) * MAX_LSC_SIZE);

	uvcx_mx_lsctabletransferCtrl.u8Info = 3;
	uvcx_mx_lsctabletransferCtrl.u16Len = 8 + TABLE_CHUNK;//512
	memset(&uvcx_mx_lsctableCtrl.cur, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_mx_lsctableCtrl.min, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_mx_lsctableCtrl.max, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_mx_lsctableCtrl.res, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_mx_lsctableCtrl.def, 0, 8 + TABLE_CHUNK);

	uvcx_mx_lumadnsCtrl.u8Info = 3;
	uvcx_mx_lumadnsCtrl.u16Len = sizeof(uvcx_mx_lumadns_data_t);

	memset(&uvcx_mx_lumadnsCtrl.cur, 0, sizeof(uvcx_mx_lumadns_data_t));
	memset(&uvcx_mx_lumadnsCtrl.min, 0, sizeof(uvcx_mx_lumadns_data_t));
	memset(&uvcx_mx_lumadnsCtrl.max, 0, sizeof(uvcx_mx_lumadns_data_t));
	memset(&uvcx_mx_lumadnsCtrl.res, 0, sizeof(uvcx_mx_lumadns_data_t));
	memset(&uvcx_mx_lumadnsCtrl.def, 0, sizeof(uvcx_mx_lumadns_data_t));

	uvcx_mx_lumadnsrefCtrl.u8Info = 3;
	uvcx_mx_lumadnsrefCtrl.u16Len = sizeof(uvcx_mx_lumadnsref_data_t);

	memset(&uvcx_mx_lumadnsrefCtrl.cur, 0, sizeof(uvcx_mx_lumadnsref_data_t));
	memset(&uvcx_mx_lumadnsrefCtrl.min, 0, sizeof(uvcx_mx_lumadnsref_data_t));
	memset(&uvcx_mx_lumadnsrefCtrl.max, 0, sizeof(uvcx_mx_lumadnsref_data_t));
	memset(&uvcx_mx_lumadnsrefCtrl.res, 0, sizeof(uvcx_mx_lumadnsref_data_t));
	memset(&uvcx_mx_lumadnsrefCtrl.def, 0, sizeof(uvcx_mx_lumadnsref_data_t));

	uvcx_mx_ltmCtrl.u8Info = 3;
	uvcx_mx_ltmCtrl.u16Len = sizeof(uvcx_mx_ltm_data_t);

	memset(&uvcx_mx_ltmCtrl.cur, 0, sizeof(uvcx_mx_ltm_data_t));
	memset(&uvcx_mx_ltmCtrl.min, 0, sizeof(uvcx_mx_ltm_data_t));
	memset(&uvcx_mx_ltmCtrl.max, 0, sizeof(uvcx_mx_ltm_data_t));
	memset(&uvcx_mx_ltmCtrl.res, 0, sizeof(uvcx_mx_ltm_data_t));
	memset(&uvcx_mx_ltmCtrl.def, 0, sizeof(uvcx_mx_ltm_data_t));

	uvcx_mx_dogdenoiseCtrl.u8Info = 3;
	uvcx_mx_dogdenoiseCtrl.u16Len = sizeof(uvcx_mx_dogdenoise_data_t);

	memset(&uvcx_mx_dogdenoiseCtrl.cur, 0, sizeof(uvcx_mx_dogdenoise_data_t));
	memset(&uvcx_mx_dogdenoiseCtrl.min, 0, sizeof(uvcx_mx_dogdenoise_data_t));
	memset(&uvcx_mx_dogdenoiseCtrl.max, 0, sizeof(uvcx_mx_dogdenoise_data_t));
	memset(&uvcx_mx_dogdenoiseCtrl.res, 0, sizeof(uvcx_mx_dogdenoise_data_t));
	memset(&uvcx_mx_dogdenoiseCtrl.def, 0, sizeof(uvcx_mx_dogdenoise_data_t));

	uvcx_mx_lut3dtableCtrl.u8Info = 3;
	uvcx_mx_lut3dtableCtrl.u16Len = sizeof(uvcx_mx_lut3dtable_data_t);
	memset(&uvcx_mx_lut3dtableCtrl.cur, 0, sizeof(uvcx_mx_lut3dtable_data_t));
	memset(&uvcx_mx_lut3dtableCtrl.min, 0, sizeof(uvcx_mx_lut3dtable_data_t));
	memset(&uvcx_mx_lut3dtableCtrl.max, 0, sizeof(uvcx_mx_lut3dtable_data_t));
	memset(&uvcx_mx_lut3dtableCtrl.res, 0, sizeof(uvcx_mx_lut3dtable_data_t));
	memset(&uvcx_mx_lut3dtableCtrl.def, 0, sizeof(uvcx_mx_lut3dtable_data_t));

	uvcx_mx_lut3dtabletransferCtrl.u8Info = 3;
	uvcx_mx_lut3dtabletransferCtrl.u16Len = sizeof(uvcx_mx_lut3dtabletransfer_data_t);
	memset(&uvcx_mx_lut3dtabletransferCtrl.cur, 0, sizeof(uvcx_mx_lut3dtabletransfer_data_t));
	memset(&uvcx_mx_lut3dtabletransferCtrl.min, 0, sizeof(uvcx_mx_lut3dtabletransfer_data_t));
	memset(&uvcx_mx_lut3dtabletransferCtrl.max, 0, sizeof(uvcx_mx_lut3dtabletransfer_data_t));
	memset(&uvcx_mx_lut3dtabletransferCtrl.res, 0, sizeof(uvcx_mx_lut3dtabletransfer_data_t));
	memset(&uvcx_mx_lut3dtabletransferCtrl.def, 0, sizeof(uvcx_mx_lut3dtabletransfer_data_t));

	uvcx_mx_gammatabletransferCtrl.u8Info = 3;
	uvcx_mx_gammatabletransferCtrl.u16Len = sizeof(uvcx_mx_gammatabletransfer_data_t);
	memset(&uvcx_mx_gammatabletransferCtrl.cur, 0, sizeof(uvcx_mx_gammatabletransfer_data_t));
	memset(&uvcx_mx_gammatabletransferCtrl.min, 0, sizeof(uvcx_mx_gammatabletransfer_data_t));
	memset(&uvcx_mx_gammatabletransferCtrl.max, 0, sizeof(uvcx_mx_gammatabletransfer_data_t));
	memset(&uvcx_mx_gammatabletransferCtrl.res, 0, sizeof(uvcx_mx_gammatabletransfer_data_t));
	memset(&uvcx_mx_gammatabletransferCtrl.def, 0, sizeof(uvcx_mx_gammatabletransfer_data_t));


}

void onEventMX(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload)
{
    uint8_t *pCur = NULL;
    uint8_t *pMin = NULL;
    uint8_t *pMax = NULL;
    uint8_t *pRes = NULL;
    uint8_t *pDef = NULL;
    uint8_t u8Info  = 0;
    uint16_t u16Len =  0;

    if ((controlSelector < 25)||(controlSelector) >= 50) return;


    switch (controlSelector)
    {
		case XCS_MX_GENERAL :
		{
			u8Info = uvcx_mx_generalCtrl.u8Info;
			u16Len = uvcx_mx_generalCtrl.u16Len;
			pCur   = (uint8_t *)&uvcx_mx_generalCtrl.cur;
			pMin   = (uint8_t *)&uvcx_mx_generalCtrl.min;
			pMax   = (uint8_t *)&uvcx_mx_generalCtrl.max;
			pRes   = (uint8_t *)&uvcx_mx_generalCtrl.res;
			pDef   = (uint8_t *)&uvcx_mx_generalCtrl.def;
			if (request == USB_bRequest_Video_SET_CUR)
				updateMXTable[XCS_MX_GENERAL] = 1;
			break;
		}

    	case XCS_MX_BLC :
        {
            u8Info = uvcx_mx_blcCtrl.u8Info;
            u16Len = uvcx_mx_blcCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_blcCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_blcCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_blcCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_blcCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_blcCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_BLC] = 1;
            break;
        }
        case XCS_MX_SIGMA_DENOISE :
        {
            u8Info = uvcx_mx_sgmdnsCtrl.u8Info;
            u16Len = uvcx_mx_sgmdnsCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_sgmdnsCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_sgmdnsCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_sgmdnsCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_sgmdnsCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_sgmdnsCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_SIGMA_DENOISE] = 1;
            break;
        }
        case XCS_MX_LSC :
        {
            u8Info = uvcx_mx_lscCtrl.u8Info;
            u16Len = uvcx_mx_lscCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_lscCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_lscCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_lscCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_lscCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_lscCtrl.def;
            break;
        }
        case XCS_MX_LSC_TABLE_TRANSFER :
        {
            u8Info = uvcx_mx_lsctabletransferCtrl.u8Info;
            u16Len = uvcx_mx_lsctabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_lsctabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_lsctabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_lsctabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_lsctabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_lsctabletransferCtrl.def;
            break;
        }
        case XCS_MX_RAW :
        {
            u8Info = uvcx_mx_rawCtrl.u8Info;
            u16Len = uvcx_mx_rawCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_rawCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_rawCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_rawCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_rawCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_rawCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_RAW] = 1;
            break;
        }
        case XCS_MX_DEMOSAIC :
        {
            u8Info = uvcx_mx_demosaicCtrl.u8Info;
            u16Len = uvcx_mx_demosaicCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_demosaicCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_demosaicCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_demosaicCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_demosaicCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_demosaicCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_DEMOSAIC] = 1;
            break;
        }
        case XCS_MX_CHROMA_GEN :
        {
            u8Info = uvcx_mx_chromagenCtrl.u8Info;
            u16Len = uvcx_mx_chromagenCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_chromagenCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_chromagenCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_chromagenCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_chromagenCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_chromagenCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_CHROMA_GEN] = 1;
            break;
        }
        case XCS_MX_MEDIAN:
        {
            u8Info = uvcx_mx_medianCtrl.u8Info;
            u16Len = uvcx_mx_medianCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_medianCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_medianCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_medianCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_medianCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_medianCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_MEDIAN] = 1;
            break;
        }
        case XCS_MX_SHARPEN:
        {
            u8Info = uvcx_mx_sharpenCtrl.u8Info;
            u16Len = uvcx_mx_sharpenCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_sharpenCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_sharpenCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_sharpenCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_sharpenCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_sharpenCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_SHARPEN] = 1;
            break;
        }
        case XCS_MX_LD:
        {
            u8Info = uvcx_mx_lumadnsCtrl.u8Info;
            u16Len = uvcx_mx_lumadnsCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_lumadnsCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_lumadnsCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_lumadnsCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_lumadnsCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_lumadnsCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_LD] = 1;
            break;
        }
        case XCS_MX_LD_REF:
        {
            u8Info = uvcx_mx_lumadnsrefCtrl.u8Info;
            u16Len = uvcx_mx_lumadnsrefCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_lumadnsrefCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_lumadnsrefCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_lumadnsrefCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_lumadnsrefCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_lumadnsrefCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_LD_REF] = 1;
            break;
        }
        case XCS_MX_LTM:
        {
            u8Info = uvcx_mx_ltmCtrl.u8Info;
            u16Len = uvcx_mx_ltmCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_ltmCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_ltmCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_ltmCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_ltmCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_ltmCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_LTM] = 1;
            break;
        }
        case XCS_MX_DOG:
        {
            u8Info = uvcx_mx_dogdenoiseCtrl.u8Info;
            u16Len = uvcx_mx_dogdenoiseCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_dogdenoiseCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_dogdenoiseCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_dogdenoiseCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_dogdenoiseCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_dogdenoiseCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_DOG] = 1;
            break;
        }
        case XCS_MX_CHROMA_DENOISE:
        {
            u8Info = uvcx_mx_chromadenoiseCtrl.u8Info;
            u16Len = uvcx_mx_chromadenoiseCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_chromadenoiseCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_chromadenoiseCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_chromadenoiseCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_chromadenoiseCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_chromadenoiseCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_CHROMA_DENOISE] = 1;
            break;
        }
        case XCS_MX_COL_COM:
        {
            u8Info = uvcx_mx_colorcombineCtrl.u8Info;
            u16Len = uvcx_mx_colorcombineCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_colorcombineCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_colorcombineCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_colorcombineCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_colorcombineCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_colorcombineCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_COL_COM] = 1;
            break;
        }
        case XCS_MX_L3D_TABEL_TRANSFER:
        {
            u8Info = uvcx_mx_lut3dtabletransferCtrl.u8Info;
            u16Len = uvcx_mx_lut3dtabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_lut3dtabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_lut3dtabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_lut3dtabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_lut3dtabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_lut3dtabletransferCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_L3D_TABEL_TRANSFER] = 1;
            break;
        }
        case XCS_MX_COL_CONV:
        {
            u8Info = uvcx_mx_colorconvertCtrl.u8Info;
            u16Len = uvcx_mx_colorconvertCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_colorconvertCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_colorconvertCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_colorconvertCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_colorconvertCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_colorconvertCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_COL_CONV] = 1;
            break;
        }
        case XCS_MX_GAMMA:
        {
            u8Info = uvcx_mx_gammaCtrl.u8Info;
            u16Len = uvcx_mx_gammaCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_gammaCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_gammaCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_gammaCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_gammaCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_gammaCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_GAMMA] = 1;
            break;
        }
        case XCS_MX_GAMMA_TABEL_TRANSFER:
        {
            u8Info = uvcx_mx_gammatabletransferCtrl.u8Info;
            u16Len = uvcx_mx_gammatabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_mx_gammatabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_mx_gammatabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_mx_gammatabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_mx_gammatabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_mx_gammatabletransferCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateMXTable[XCS_MX_GAMMA_TABEL_TRANSFER] = 1;
            break;
        }


        default:break;
    }

    if ((u8Info == 0) ||
    	(u16Len == 0) ||
        (pCur == NULL)||
		(pMin == NULL)||
		(pMax == NULL)||
		(pRes == NULL)||
		(pDef == NULL))
    {
    	printf("[ispXUControlsMX] Invalid Request controlSelector %d request %d len %d\n", controlSelector, request, u16Len);
    	return;
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

    if (request == USB_bRequest_Video_SET_CUR)
    {
    	if (XCS_MX_LSC_TABLE_TRANSFER == controlSelector)
    	{
//    		memmove(uvcx_mx_lsctableCtrl.cur.lscTable + uvcx_mx_lsctabletransferCtrl.cur.header[0] * TABLE_CHUNK / 2, uvcx_mx_lsctabletransferCtrl.cur.payload, TABLE_CHUNK);
//
//    		if ((uvcx_mx_lsctabletransferCtrl.cur.header[0] + 1) == uvcx_mx_lsctabletransferCtrl.cur.header[1]) {
//    			updateMXTable[XCS_MX_LSC] = 1;
//
//    			printf("LSC TABLE w %d h %d\n", uvcx_mx_lscCtrl.cur.lscWidth, uvcx_mx_lscCtrl.cur.lscHeight);
//    			for (uint32_t i = 0 ; i < uvcx_mx_lscCtrl.cur.lscHeight; i++)
//    			{
//    				for (uint32_t j = 0 ; j < uvcx_mx_lscCtrl.cur.lscWidth; j++)
//    				{
//    					printf("%x ", uvcx_mx_lsctableCtrl.cur.lscTable[i * uvcx_mx_lscCtrl.cur.lscWidth + j]);
//    				}
//    				printf("\n");
//    			}
//    	    }
   	    }

    	if (XCS_MX_L3D_TABEL_TRANSFER == controlSelector)
    	{

//    		memmove(uvcx_mx_lut3dtableCtrl.cur.lut3dTable + uvcx_mx_lut3dtabletransferCtrl.cur.header[0] * TABLE_CHUNK / 2, uvcx_mx_lut3dtabletransferCtrl.cur.payload, TABLE_CHUNK * 2);
//
//    		printf("AAAA %d %d \n", uvcx_mx_lut3dtabletransferCtrl.cur.header[0], uvcx_mx_lut3dtabletransferCtrl.cur.header[1]);
//
//    		if ((uvcx_mx_lut3dtabletransferCtrl.cur.header[0] + 1) == uvcx_mx_lut3dtabletransferCtrl.cur.header[1]) {
//    			updateMXTable[XCS_MX_L3D_TABEL_TRANSFER] = 1;
//
//    			for (uint32_t i = 0 ; i < 256; i++)
//    			{
//    				for (uint32_t j = 0 ; j < 64; j++)
//    				{
//    					printf("%d ", uvcx_mx_lut3dtableCtrl.cur.lut3dTable[i * 64 + j]);
//    				}
//    				printf("\n");
//    			}
//    	    }
   	    }
    	if (XCS_MX_GAMMA_TABEL_TRANSFER == controlSelector)
    	{
//    		memmove(uvcx_mx_gammatableCtrl.cur.gammaTable + uvcx_mx_gammatabletransferCtrl.cur.header[0] * TABLE_CHUNK / 2, uvcx_mx_gammatabletransferCtrl.cur.payload, TABLE_CHUNK * 2);
//
//    		if ((uvcx_mx_gammatabletransferCtrl.cur.header[0] + 1) == uvcx_mx_gammatabletransferCtrl.cur.header[1]) {
//    			updateMXTable[XCS_MX_GAMMA_TABEL_TRANSFER] = 1;
//
//    			for (uint32_t i = 0 ; i < 256; i++)
//    			{
//    				for (uint32_t j = 0 ; j < 64; j++)
//    				{
//    					printf("%d ", uvcx_mx_lut3dtableCtrl.cur.lut3dTable[i * 64 + j]);
//    				}
//    				printf("\n");
//    			}
//    	    }
//
    	}

   	}

    //dummy call
    getConfigMX();
}

void getConfigMX()
{
	if (updateMXTable[XCS_MX_GENERAL])
	{
		updateMXTable[XCS_MX_GENERAL] = 0;
		printf("%s %ld %ld  %ld  %ld  %ld \n",
				uvcx_mx_generalCtrl.cur.fileName,
				uvcx_mx_generalCtrl.cur.image_width, uvcx_mx_generalCtrl.cur.image_height,
				uvcx_mx_generalCtrl.cur.skip_lines,  uvcx_mx_generalCtrl.cur.bits,
				uvcx_mx_generalCtrl.cur.bayer_order
		);
	}
	if (updateMXTable[XCS_MX_BLC])
	{
	  updateMXTable[XCS_MX_BLC] = 0;
	  printf("[blc] %ld %ld %ld %ld \n",
			  uvcx_mx_blcCtrl.cur.b,
			  uvcx_mx_blcCtrl.cur.gb,
			  uvcx_mx_blcCtrl.cur.gr,
			  uvcx_mx_blcCtrl.cur.r);
	}
	if (updateMXTable[XCS_MX_SIGMA_DENOISE])
	{
	  updateMXTable[XCS_MX_SIGMA_DENOISE] = 0;
	  printf("[sgmdns] %ld %ld %ld %ld %ld %ld %ld %ld %ld\n" ,
			  uvcx_mx_sgmdnsCtrl.cur.noiseFloor,
			  uvcx_mx_sgmdnsCtrl.cur.thresh1P0, uvcx_mx_sgmdnsCtrl.cur.thresh2P0,
			  uvcx_mx_sgmdnsCtrl.cur.thresh1P1, uvcx_mx_sgmdnsCtrl.cur.thresh2P1,
			  uvcx_mx_sgmdnsCtrl.cur.thresh1P2, uvcx_mx_sgmdnsCtrl.cur.thresh2P2,
			  uvcx_mx_sgmdnsCtrl.cur.thresh1P3, uvcx_mx_sgmdnsCtrl.cur.thresh2P3);
	}
	if (updateMXTable[XCS_MX_RAW])
	{
		updateMXTable[XCS_MX_RAW] = 0;
		printf("[raw] %ld | %ld %ld %ld %ld | %ld %ld %ld %ld | %ld %ld %ld %ld %ld |  %ld %ld %ld %ld %ld\n" ,
				uvcx_mx_rawCtrl.cur.outputBits,
				uvcx_mx_rawCtrl.cur.gainGr, uvcx_mx_rawCtrl.cur.gainGb,
				uvcx_mx_rawCtrl.cur.gainR,  uvcx_mx_rawCtrl.cur.gainB,

				uvcx_mx_rawCtrl.cur.clampGr, uvcx_mx_rawCtrl.cur.clampGb,
				uvcx_mx_rawCtrl.cur.clampR,  uvcx_mx_rawCtrl.cur.clampB,

				uvcx_mx_rawCtrl.cur.grgbImbalPlatDark,   uvcx_mx_rawCtrl.cur.grgbImbalDecayDark,
				uvcx_mx_rawCtrl.cur.grgbImbalPlatBright, uvcx_mx_rawCtrl.cur.grgbImbalDecayBright,
				uvcx_mx_rawCtrl.cur.grgbImbalThr,

				uvcx_mx_rawCtrl.cur.dpcAlphaHotG,  uvcx_mx_rawCtrl.cur.dpcAlphaHotRb,
				uvcx_mx_rawCtrl.cur.dpcAlphaColdG, uvcx_mx_rawCtrl.cur.dpcAlphaColdRb,
				uvcx_mx_rawCtrl.cur.dpcNoiseLevel);
	}
	if (updateMXTable[XCS_MX_DEMOSAIC])
	{
		updateMXTable[XCS_MX_DEMOSAIC] = 0;
		unsigned int dark   = uvcx_mx_demosaicCtrl.cur.gradientMulDark;
		unsigned int bright = uvcx_mx_demosaicCtrl.cur.gradientMulBright;
		printf("[demosaic]%ld 0x%04x 0x%04x %ld %ld %ld\n",
				uvcx_mx_demosaicCtrl.cur.greenInterpCoeff,
				dark,
				bright,
				uvcx_mx_demosaicCtrl.cur.lumaWeightR,
				uvcx_mx_demosaicCtrl.cur.lumaWeightG,
				uvcx_mx_demosaicCtrl.cur.lumaWeightB);
	}
	if (updateMXTable[XCS_MX_CHROMA_GEN])
	{
		updateMXTable[XCS_MX_CHROMA_GEN] = 0;
	    printf("[chromagen] %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
	    		uvcx_mx_chromagenCtrl.cur.epsilon,
				uvcx_mx_chromagenCtrl.cur.kr, uvcx_mx_chromagenCtrl.cur.kg, uvcx_mx_chromagenCtrl.cur.kb,
				uvcx_mx_chromagenCtrl.cur.lumaCoeffR, uvcx_mx_chromagenCtrl.cur.lumaCoeffG, uvcx_mx_chromagenCtrl.cur.lumaCoeffB,
				uvcx_mx_chromagenCtrl.cur.pfrStrengthR,
				uvcx_mx_chromagenCtrl.cur.pfrStrengthB,
				uvcx_mx_chromagenCtrl.cur.pfrCoeff0,
				uvcx_mx_chromagenCtrl.cur.pfrCoeff1,
				uvcx_mx_chromagenCtrl.cur.pfrCoeff2,
				uvcx_mx_chromagenCtrl.cur.desatOffset,
				uvcx_mx_chromagenCtrl.cur.desatSlope,
				uvcx_mx_chromagenCtrl.cur.emSlope,
				uvcx_mx_chromagenCtrl.cur.emOffset,
				uvcx_mx_chromagenCtrl.cur.emLimit
				);
	}

	if (updateMXTable[XCS_MX_MEDIAN])
	{
		updateMXTable[XCS_MX_MEDIAN] = 0;
		printf("[median] %ld %ld %ld\n" ,
				uvcx_mx_medianCtrl.cur.kernelSize, uvcx_mx_medianCtrl.cur.offset, uvcx_mx_medianCtrl.cur.slope);
	}
	if (updateMXTable[XCS_MX_SHARPEN])
	{
		updateMXTable[XCS_MX_SHARPEN] = 0;
		printf("[sharpen] %f %f %f | %f | %ld %ld %ld %ld | %ld | %ld %ld %ld | "
				"%ld %ld %ld | %ld %ld %ld %ld | %ld %ld %ld\n" ,
				uvcx_mx_sharpenCtrl.cur.lowpassSigma,   uvcx_mx_sharpenCtrl.cur.bandpassSigma0,
				uvcx_mx_sharpenCtrl.cur.bandpassSigma1, uvcx_mx_sharpenCtrl.cur.angleOfView,
				uvcx_mx_sharpenCtrl.cur.hpGainCenter,   uvcx_mx_sharpenCtrl.cur.hpGainCorner,
				uvcx_mx_sharpenCtrl.cur.bpGainCenter,   uvcx_mx_sharpenCtrl.cur.bpGainCorner,
				uvcx_mx_sharpenCtrl.cur.darkenBiasGain,
				uvcx_mx_sharpenCtrl.cur.nonlinearStops[0], uvcx_mx_sharpenCtrl.cur.nonlinearStops[1],
				uvcx_mx_sharpenCtrl.cur.nonlinearStops[2],
				uvcx_mx_sharpenCtrl.cur.nonlinearGains[0], uvcx_mx_sharpenCtrl.cur.nonlinearGains[1],
				uvcx_mx_sharpenCtrl.cur.nonlinearGains[2],
				uvcx_mx_sharpenCtrl.cur.rangeStops[0], uvcx_mx_sharpenCtrl.cur.rangeStops[1],
				uvcx_mx_sharpenCtrl.cur.rangeStops[2], uvcx_mx_sharpenCtrl.cur.rangeStops[3],
				uvcx_mx_sharpenCtrl.cur.overshoot,
				uvcx_mx_sharpenCtrl.cur.undershoot,
				uvcx_mx_sharpenCtrl.cur.alpha
				);
	}
	if (updateMXTable[XCS_MX_LD])
	{
		updateMXTable[XCS_MX_LD] = 0;

		printf("[luma dns] %f %ld %ld %ld\n",
				uvcx_mx_lumadnsCtrl.cur.strength, uvcx_mx_lumadnsCtrl.cur.alpha,
				uvcx_mx_lumadnsCtrl.cur.bitpos, uvcx_mx_lumadnsCtrl.cur.f2);
		for (uint32_t i = 0; i < 32; i++)
			printf("%d ", uvcx_mx_lumadnsCtrl.cur.lut[i]);
		printf("\n");
	}
	if (updateMXTable[XCS_MX_LTM])
	{
		updateMXTable[XCS_MX_LTM] = 0;
		printf("[ltm] %ld\n", uvcx_mx_ltmCtrl.cur.thr);
		for (uint32_t i = 0; i < 16; i++)
		{
			for (uint32_t j = 0; j < 16; j++)
				printf("%d ", uvcx_mx_ltmCtrl.cur.table[8 * i + j]);
		    printf("\n");
		}

	}
	if (updateMXTable[XCS_MX_DOG])
	{
		updateMXTable[XCS_MX_DOG] = 0;
		printf("[dog] %ld %ld %f %f\n",
				uvcx_mx_dogdenoiseCtrl.cur.thr,     uvcx_mx_dogdenoiseCtrl.cur.strength,
				uvcx_mx_dogdenoiseCtrl.cur.sigma11, uvcx_mx_dogdenoiseCtrl.cur.sigma15);

		for (uint32_t i = 0; i < 6; i++)
			printf("%d ", uvcx_mx_dogdenoiseCtrl.cur.coeffs11[i]);
		printf("\n");

		for (uint32_t i = 0; i < 8; i++)
			printf("%d ", uvcx_mx_dogdenoiseCtrl.cur.coeffs15[i]);
		printf("\n");
	}
	if (updateMXTable[XCS_MX_CHROMA_DENOISE])
	{
		updateMXTable[XCS_MX_CHROMA_DENOISE] = 0;
		printf("[chromadenoise] dark %ld %ld %ld %ld  | bright %ld %ld %ld %ld | denoise %ld %ld %ld %ld | greyc %ld %ld %ld | desat %ld %ld | coeffs %ld %ld %ld\n",
		uvcx_mx_chromadenoiseCtrl.cur.thrRDark,
		uvcx_mx_chromadenoiseCtrl.cur.thrGDark,
		uvcx_mx_chromadenoiseCtrl.cur.thrBDark,
		uvcx_mx_chromadenoiseCtrl.cur.thrLumaDark,

		uvcx_mx_chromadenoiseCtrl.cur.thrRBright,
		uvcx_mx_chromadenoiseCtrl.cur.thrGBright,
		uvcx_mx_chromadenoiseCtrl.cur.thrBBright,
		uvcx_mx_chromadenoiseCtrl.cur.thrLumaBright,

		uvcx_mx_chromadenoiseCtrl.cur.denoiseSlope,
		uvcx_mx_chromadenoiseCtrl.cur.denoiseOffset,
		uvcx_mx_chromadenoiseCtrl.cur.limit,
		uvcx_mx_chromadenoiseCtrl.cur.hEnab,

		uvcx_mx_chromadenoiseCtrl.cur.greyCr,
		uvcx_mx_chromadenoiseCtrl.cur.greyCg,
		uvcx_mx_chromadenoiseCtrl.cur.greyCb,

		uvcx_mx_chromadenoiseCtrl.cur.greyDesatSlope,
		uvcx_mx_chromadenoiseCtrl.cur.greyDesatOffset,

		uvcx_mx_chromadenoiseCtrl.cur.convCoeffCenter,
		uvcx_mx_chromadenoiseCtrl.cur.convCoeffEdge,
		uvcx_mx_chromadenoiseCtrl.cur.convCoeffCorner
		);

	}
	if (updateMXTable[XCS_MX_COL_COM])
	{
		updateMXTable[XCS_MX_COL_COM] = 0;
		printf("[colcomb] %f %f %f %f %f %f %f %f %f / %f %f %f\n",
				uvcx_mx_colorcombineCtrl.cur.ccm[0],    uvcx_mx_colorcombineCtrl.cur.ccm[1],    uvcx_mx_colorcombineCtrl.cur.ccm[2],
				uvcx_mx_colorcombineCtrl.cur.ccm[3],    uvcx_mx_colorcombineCtrl.cur.ccm[4],    uvcx_mx_colorcombineCtrl.cur.ccm[5],
				uvcx_mx_colorcombineCtrl.cur.ccm[6],    uvcx_mx_colorcombineCtrl.cur.ccm[7],    uvcx_mx_colorcombineCtrl.cur.ccm[8],
				uvcx_mx_colorcombineCtrl.cur.ccmoff[0], uvcx_mx_colorcombineCtrl.cur.ccmoff[1], uvcx_mx_colorcombineCtrl.cur.ccmoff[2]
				);
	}

	if (updateMXTable[XCS_MX_COL_CONV])
	{
		updateMXTable[XCS_MX_COL_CONV] = 0;
		printf("[colconv] %f %f %f %f %f %f %f %f %f / %f %f %f\n",
				uvcx_mx_colorconvertCtrl.cur.mat[0], uvcx_mx_colorconvertCtrl.cur.mat[1], uvcx_mx_colorconvertCtrl.cur.mat[2],
				uvcx_mx_colorconvertCtrl.cur.mat[3], uvcx_mx_colorconvertCtrl.cur.mat[4], uvcx_mx_colorconvertCtrl.cur.mat[5],
				uvcx_mx_colorconvertCtrl.cur.mat[6], uvcx_mx_colorconvertCtrl.cur.mat[7], uvcx_mx_colorconvertCtrl.cur.mat[8],
				uvcx_mx_colorconvertCtrl.cur.off[0], uvcx_mx_colorconvertCtrl.cur.off[1], uvcx_mx_colorconvertCtrl.cur.off[2]
				);
	}
	if (updateMXTable[XCS_MX_GAMMA])
	{
		updateMXTable[XCS_MX_GAMMA] = 0;
		printf("[gamma] %ld\n", uvcx_mx_gammaCtrl.cur.size);
	}
	if (updateMXTable[XCS_M2_GAMMA_TABEL_TRANSFER])
	{
		updateMXTable[XCS_M2_GAMMA_TABEL_TRANSFER] = 0;
		for (uint32_t i = 0 ; i < 256; i++)
		{
			for (uint32_t j = 0 ; j < 8; j++)
			{
				printf("0x%04x ", uvcx_mx_gammatableCtrl.cur.gammaTable[i * 8 + j]);
			}
			printf("\n");
		}

	}

}
