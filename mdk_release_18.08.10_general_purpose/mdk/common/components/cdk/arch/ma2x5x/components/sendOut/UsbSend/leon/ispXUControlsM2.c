#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ispXUControlsM2.h"

#define DEBUG 1

static uvcx_m2_general_ctrl_t uvcx_m2_generalCtrl =
	{
		3, sizeof(uvcx_m2_general_data_t),
		{"", 0, 0, 0, 0, 0},//cur
		{"", 0, 0, 0, 0, 0},//min
		{"", 0, 0, 0, 0, 0},//max
		{"", 1, 1, 1, 1, 1},//res
		{"", 0, 0, 0, 0, 0},//def
	};

static uvcx_m2_blc_ctrl_t      uvcx_m2_blcCtrl =
	{    3, 16,
		{0, 0, 0, 0},//cur
		{0, 0, 0, 0},//min
		{MAX_U16_VALUE, MAX_U16_VALUE,
		 MAX_U16_VALUE, MAX_U16_VALUE},//max
		{1, 1, 1, 1},//res
		{0, 0, 0, 0} //def
	};

static uvcx_m2_sgmdns_ctrl_t   uvcx_m2_sgmdnsCtrl =
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

static uvcx_m2_lsc_ctrl_t uvcx_m2_lscCtrl =
		{
			3, 12,
			{3, 3, 3}, //cur
			{3, 3, 3}, //min
			{MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE}, //max
			{1, 1, 1}, //res
			{3, 3, 3}, //def
		};

static uvcx_m2_lsctable_ctrl_t         uvcx_m2_lsctableCtrl;
static uvcx_m2_lsctabletransfer_ctrl_t uvcx_m2_lsctabletransferCtrl;

static uvcx_m2_raw_ctrl_t uvcx_m2_rawCtrl =
	{
		3, 76,
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //cur
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, //min
		{MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE,               //gain
		 MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE,               // clamp(saturation)
		 MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE, MAX_U10_VALUE, MAX_U16_VALUE, // imbal
		 MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U4_VALUE,  MAX_U16_VALUE, // dpc
		 MAX_U16_VALUE //output bits
		}, //max
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, //res
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  //def
	};

static uvcx_m2_demosaic_ctrl_t uvcx_m2_demosaicCtrl =
	{
			3, 24,
			{0, 0, 0, 0, 0, 0},//cur
			{           0,             0, MIN_I16_VALUE,            0,            0,            0},//min
			{MAX_U8_VALUE, MAX_U16_VALUE, MAX_I16_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE},//max
			{1, 1, 1, 1, 1, 1},//res
			{0, 0, 0, 0, 0, 0},//def
	};

static uvcx_m2_chromageneration_ctrl_t uvcx_m2_chromagenerationCtrl =
	{
			3, sizeof(uvcx_m2_chromageneration_data_t),
			{0,0,0,0,0,0,0,0,0,0},//cur
			{0,0,0,0,0,0,0,0,0,0},//min
			{MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
             MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
             MAX_U8_VALUE, MAX_U8_VALUE},//max
			{1,2,0,0,0,0,0,0,0,0},//res
			{0,0,0,0,0,0,0,0,0,0},//def
	};

static uvcx_m2_colorcombine_ctrl_t uvcx_m2_colorcombineCtrl =
	{
			3, 60,
			{{0,0,0,0,0,0,0,0,0}, {0,0,0}, 0,0,0},//cur
			{{0,0,0,0,0,0,0,0,0}, {0,0,0}, 0,0,0},//min
			{{0,0,0,0,0,0,0,0,0}, {0,0,0}, MAX_U12_VALUE, MAX_U12_VALUE, MAX_U12_VALUE},//max
			{{0,0,0,0,0,0,0,0,0}, {0,0,0}, 0,0,0},//res
			{{0,0,0,0,0,0,0,0,0}, {0,0,0}, 0,0,0},//def
	};

static uvcx_m2_ltm_ctrl_t        uvcx_m2_ltmCtrl;

static uvcx_m2_dogdenoise_ctrl_t uvcx_m2_dogdenoiseCtrl =
	{
			3, 30,
			{0, 0, 0.0, 0.0, {0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}},//cur
			{0, 0, 0.0, 0.0, {0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}},//min
			{MAX_U8_VALUE, MAX_U8_VALUE, 0.0, 0.0,
					{MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE},
					{MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE,MAX_U8_VALUE}},//max
			{1, 1, 0.0, 0.0, {1,1,1,1,1,1}, {1,1,1,1,1,1,1,1}},//res
			{0, 0, 0.0, 0.0, {0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}},//def
	};

static uvcx_m2_lumadns_ctrl_t    uvcx_m2_lumadnsCtrl;
static uvcx_m2_lumadnsref_ctrl_t uvcx_m2_lumadnsrefCtrl;


static uvcx_m2_sharpen_ctrl_t    uvcx_m2_sharpenCtrl =
		{
				3, 32,
				{0.0, {0,0,0,0}, 0,0,0,0,0,0,0,0,0,0},//cur
				{0.0, {0,0,0,0}, 0,0,0,0,0,0,0,0,0,0},//min
				{0.0, {MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE},
					   MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE,
					   MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE, MAX_U16_VALUE},//max
				{0.0, {1,1,1,1}, 1,1,1,1,1,1,1,1,1,1},//res
				{0.0, {0,0,0,0}, 0,0,0,0,0,0,0,0,0,0} //def
		};

static uvcx_m2_median_ctrl_t uvcx_m2_medianCtrl =
	{		3, 12,
			{0, 0, 0},//cur
			{           0,            0, MIN_I8_VALUE},//min
			{MAX_U8_VALUE, MAX_U8_VALUE, MAX_I8_VALUE},//max
			{1, 1, 1},//res
			{0, 0, 0},//def
	};

static uvcx_m2_chromadenoise_ctrl_t uvcx_m2_chromadenoiseCtrl =
	{
			3, 52,
			{0,0,0,0,0,0,0,0,0,0,0,0,0},//cur
			{0,0,0, 0,0, 0,MIN_I8_VALUE, 0,0,0, 0,0,0},//min
			{MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
             MAX_U8_VALUE, MAX_U8_VALUE,
             MAX_U8_VALUE, MAX_I8_VALUE,
             MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE,
             MAX_U8_VALUE, MAX_U8_VALUE, MAX_U8_VALUE},//max
			{1,1,1,1,1,1,1,1,1,1,1,1,1},//res
			{0,0,0,0,0,0,0,0,0,0,0,0,0},//def
	};

static uvcx_m2_colorconvert_ctrl_t uvcx_m2_colorconvertCtrl =
	{
			3, 48,
			{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},//cur
			{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},//min
			{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},//max
			{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},//res
			{{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}},//def
	};
static uvcx_m2_lut3dtable_ctrl_t         uvcx_m2_lut3dtableCtrl;
static uvcx_m2_lut3dtabletransfer_ctrl_t uvcx_m2_lut3dtabletransferCtrl;

static uvcx_m2_gamma_ctrl_t uvcx_m2_gammaCtrl =
	{
			3, 4,
			{0},//cur
			{0},//min
			{MAX_U32_VALUE},//max
			{1},//res
			{0},//def
	};

static uvcx_m2_gammatable_ctrl_t         uvcx_m2_gammatableCtrl;
static uvcx_m2_gammatabletransfer_ctrl_t uvcx_m2_gammatabletransferCtrl;

static char updateM2Table[XCS_M2_MAX];

void Init_M2_Controls()
{
	printf("Init_M2_Controls\n");

	uvcx_m2_lsctableCtrl.u8Info = 3;
	uvcx_m2_lsctableCtrl.u16Len = MAX_LSC_SIZE ;//* sizeof(uint16_t);
	memset(&uvcx_m2_lsctableCtrl.cur, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_m2_lsctableCtrl.min, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_m2_lsctableCtrl.max, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_m2_lsctableCtrl.res, 0, sizeof(uint16_t) * MAX_LSC_SIZE);
	memset(&uvcx_m2_lsctableCtrl.def, 0, sizeof(uint16_t) * MAX_LSC_SIZE);

	uvcx_m2_lsctabletransferCtrl.u8Info = 3;
	uvcx_m2_lsctabletransferCtrl.u16Len = 8 + TABLE_CHUNK;//512
	memset(&uvcx_m2_lsctableCtrl.cur, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_m2_lsctableCtrl.min, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_m2_lsctableCtrl.max, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_m2_lsctableCtrl.res, 0, 8 + TABLE_CHUNK);
	memset(&uvcx_m2_lsctableCtrl.def, 0, 8 + TABLE_CHUNK);

	uvcx_m2_ltmCtrl.u8Info = 3;
	uvcx_m2_ltmCtrl.u16Len = sizeof(uvcx_m2_ltm_data_t);
	memset(&uvcx_m2_ltmCtrl.cur, 0, sizeof(uvcx_m2_ltm_data_t));
	memset(&uvcx_m2_ltmCtrl.min, 0, sizeof(uvcx_m2_ltm_data_t));
	memset(&uvcx_m2_ltmCtrl.max, 0, sizeof(uvcx_m2_ltm_data_t));
	memset(&uvcx_m2_ltmCtrl.res, 0, sizeof(uvcx_m2_ltm_data_t));
	memset(&uvcx_m2_ltmCtrl.def, 0, sizeof(uvcx_m2_ltm_data_t));

	/////////////////////////////////////////////////////////////
	//luma denoise
	/////////////////////////////////////////////////////////////
	uvcx_m2_lumadnsCtrl.u8Info = 3;
	uvcx_m2_lumadnsCtrl.u16Len = 48;
	//cur
	uvcx_m2_lumadnsCtrl.cur.strength = 0.0;
	uvcx_m2_lumadnsCtrl.cur.alpha    = 0;
	uvcx_m2_lumadnsCtrl.cur.bitpos   = 0;
	uvcx_m2_lumadnsCtrl.cur.f2       = 0;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsCtrl.cur.lut[i] = 0;
	//min
	uvcx_m2_lumadnsCtrl.min.strength = 0.0;
	uvcx_m2_lumadnsCtrl.min.alpha    = 0;
	uvcx_m2_lumadnsCtrl.min.bitpos   = 0;
	uvcx_m2_lumadnsCtrl.min.f2       = 0;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsCtrl.min.lut[i] = 0;
	//max
	uvcx_m2_lumadnsCtrl.max.strength = 0.0;
	uvcx_m2_lumadnsCtrl.max.alpha    = MAX_U8_VALUE;
	uvcx_m2_lumadnsCtrl.max.bitpos   = MAX_U32_VALUE;
	uvcx_m2_lumadnsCtrl.max.f2       = MAX_U32_VALUE;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsCtrl.max.lut[i] = MAX_U8_VALUE;
	//res
	uvcx_m2_lumadnsCtrl.res.strength = 0.0;
	uvcx_m2_lumadnsCtrl.res.alpha    = 1;
	uvcx_m2_lumadnsCtrl.res.bitpos   = 1;
	uvcx_m2_lumadnsCtrl.res.f2       = 1;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsCtrl.res.lut[i] = 1;
	//def
	uvcx_m2_lumadnsCtrl.def.strength = 0.0;
	uvcx_m2_lumadnsCtrl.def.alpha    = 0;
	uvcx_m2_lumadnsCtrl.def.bitpos   = 0;
	uvcx_m2_lumadnsCtrl.def.f2       = 0;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsCtrl.def.lut[i] = 0;

	/////////////////////////////////////////////////////////////
	//luma denoise ref
	/////////////////////////////////////////////////////////////
	uvcx_m2_lumadnsrefCtrl.u8Info = 3;
	uvcx_m2_lumadnsrefCtrl.u16Len = 286;
	//cur
	uvcx_m2_lumadnsrefCtrl.cur.angle_of_view = 1.0;
	uvcx_m2_lumadnsrefCtrl.cur.gamma         = 1.0;
	for (uint32_t i = 0; i < 32; i++)
		uvcx_m2_lumadnsrefCtrl.cur.lutDist[i] = 1;
	for (uint32_t i = 0; i < 9; i++)
		uvcx_m2_lumadnsrefCtrl.cur.lutGamma0_32[i]   = 1;
	for (uint32_t i = 0; i < 9; i++)
		uvcx_m2_lumadnsrefCtrl.cur.lutGamma32_255[i] = 1;
	uvcx_m2_lumadnsrefCtrl.cur.shift = 1;
	/////////////////////////////////////////////////////////////
	// lut3d table
	/////////////////////////////////////////////////////////////
	uvcx_m2_lut3dtableCtrl.u8Info = 3;
	uvcx_m2_lut3dtableCtrl.u16Len = sizeof(uvcx_m2_lut3dtable_data_t);
	memset(&uvcx_m2_lut3dtableCtrl.cur, 0, sizeof(uvcx_m2_lut3dtable_data_t));
	memset(&uvcx_m2_lut3dtableCtrl.min, 0, sizeof(uvcx_m2_lut3dtable_data_t));
	memset(&uvcx_m2_lut3dtableCtrl.max, 0, sizeof(uvcx_m2_lut3dtable_data_t));
	memset(&uvcx_m2_lut3dtableCtrl.res, 0, sizeof(uvcx_m2_lut3dtable_data_t));
	memset(&uvcx_m2_lut3dtableCtrl.def, 0, sizeof(uvcx_m2_lut3dtable_data_t));

	uvcx_m2_lut3dtabletransferCtrl.u8Info = 3;
	uvcx_m2_lut3dtabletransferCtrl.u16Len = sizeof(uvcx_m2_lut3dtabletransfer_data_t);
	memset(&uvcx_m2_lut3dtabletransferCtrl.cur, 0, sizeof(uvcx_m2_lut3dtabletransfer_data_t));
	memset(&uvcx_m2_lut3dtabletransferCtrl.min, 0, sizeof(uvcx_m2_lut3dtabletransfer_data_t));
	memset(&uvcx_m2_lut3dtabletransferCtrl.max, 0, sizeof(uvcx_m2_lut3dtabletransfer_data_t));
	memset(&uvcx_m2_lut3dtabletransferCtrl.res, 0, sizeof(uvcx_m2_lut3dtabletransfer_data_t));
	memset(&uvcx_m2_lut3dtabletransferCtrl.def, 0, sizeof(uvcx_m2_lut3dtabletransfer_data_t));
	/////////////////////////////////////////////////////////////
	// gamma table
	/////////////////////////////////////////////////////////////
	uvcx_m2_gammatableCtrl.u8Info = 3;
	uvcx_m2_gammatableCtrl.u16Len = sizeof(uvcx_m2_gammatable_data_t);
	memset(&uvcx_m2_gammatableCtrl.cur, 0, sizeof(uvcx_m2_gammatable_data_t));
	memset(&uvcx_m2_gammatableCtrl.min, 0, sizeof(uvcx_m2_gammatable_data_t));
	memset(&uvcx_m2_gammatableCtrl.max, 0, sizeof(uvcx_m2_gammatable_data_t));
	memset(&uvcx_m2_gammatableCtrl.res, 0, sizeof(uvcx_m2_gammatable_data_t));
	memset(&uvcx_m2_gammatableCtrl.def, 0, sizeof(uvcx_m2_gammatable_data_t));

	uvcx_m2_gammatabletransferCtrl.u8Info = 3;
	uvcx_m2_gammatabletransferCtrl.u16Len = sizeof(uvcx_m2_gammatabletransfer_data_t);
	memset(&uvcx_m2_gammatabletransferCtrl.cur, 0, sizeof(uvcx_m2_gammatabletransfer_data_t));
	memset(&uvcx_m2_gammatabletransferCtrl.min, 0, sizeof(uvcx_m2_gammatabletransfer_data_t));
	memset(&uvcx_m2_gammatabletransferCtrl.max, 0, sizeof(uvcx_m2_gammatabletransfer_data_t));
	memset(&uvcx_m2_gammatabletransferCtrl.res, 0, sizeof(uvcx_m2_gammatabletransfer_data_t));
	memset(&uvcx_m2_gammatabletransferCtrl.def, 0, sizeof(uvcx_m2_gammatabletransfer_data_t));

}

void onEventM2(uint8_t controlSelector, uint8_t request, uint16_t *pLen, uint8_t *pPayload)
{
    uint8_t *pCur = NULL;
    uint8_t *pMin = NULL;
    uint8_t *pMax = NULL;
    uint8_t *pRes = NULL;
    uint8_t *pDef = NULL;
    uint8_t u8Info  = 0;
    uint16_t u16Len =  0;

    if (controlSelector >= 25) return;

    switch (controlSelector)
    {
		case XCS_M2_GENERAL :
		{
			u8Info = uvcx_m2_generalCtrl.u8Info;
			u16Len = uvcx_m2_generalCtrl.u16Len;
			pCur   = (uint8_t *)&uvcx_m2_generalCtrl.cur;
			pMin   = (uint8_t *)&uvcx_m2_generalCtrl.min;
			pMax   = (uint8_t *)&uvcx_m2_generalCtrl.max;
			pRes   = (uint8_t *)&uvcx_m2_generalCtrl.res;
			pDef   = (uint8_t *)&uvcx_m2_generalCtrl.def;
			if (request == USB_bRequest_Video_SET_CUR)
				updateM2Table[XCS_M2_GENERAL] = 1;
			break;
		}
        case XCS_M2_BLC :
        {
            u8Info = uvcx_m2_blcCtrl.u8Info;
            u16Len = uvcx_m2_blcCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_blcCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_blcCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_blcCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_blcCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_blcCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_BLC] = 1;
            break;
        }
        case XCS_M2_SIGMA_DENOISE :
        {
            u8Info = uvcx_m2_sgmdnsCtrl.u8Info;
            u16Len = uvcx_m2_sgmdnsCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_sgmdnsCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_sgmdnsCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_sgmdnsCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_sgmdnsCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_sgmdnsCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_SIGMA_DENOISE] = 1;
            break;
        }
        case XCS_M2_LSC :
        {
            u8Info = uvcx_m2_lscCtrl.u8Info;
            u16Len = uvcx_m2_lscCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_lscCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_lscCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_lscCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_lscCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_lscCtrl.def;
            break;
        }
        case XCS_M2_LSC_TABLE_TRANSFER :
        {
            u8Info = uvcx_m2_lsctabletransferCtrl.u8Info;
            u16Len = uvcx_m2_lsctabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_lsctabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_lsctabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_lsctabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_lsctabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_lsctabletransferCtrl.def;
            break;
        }
        case XCS_M2_RAW :
        {
            u8Info = uvcx_m2_rawCtrl.u8Info;
            u16Len = uvcx_m2_rawCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_rawCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_rawCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_rawCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_rawCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_rawCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_RAW] = 1;
            break;
        }
        case XCS_M2_DEMOSAIC :
        {
            u8Info = uvcx_m2_demosaicCtrl.u8Info;
            u16Len = uvcx_m2_demosaicCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_demosaicCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_demosaicCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_demosaicCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_demosaicCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_demosaicCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_DEMOSAIC] = 1;
            break;
        }
        case XCS_M2_CHROMA_GEN :
        {
            u8Info = uvcx_m2_chromagenerationCtrl.u8Info;
            u16Len = uvcx_m2_chromagenerationCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_chromagenerationCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_chromagenerationCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_chromagenerationCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_chromagenerationCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_chromagenerationCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_CHROMA_GEN] = 1;
            break;
        }
        case XCS_M2_SHARPEN :
        {
            u8Info = uvcx_m2_sharpenCtrl.u8Info;
            u16Len = uvcx_m2_sharpenCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_sharpenCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_sharpenCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_sharpenCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_sharpenCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_sharpenCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_SHARPEN] = 1;
            break;
        }
        case XCS_M2_MEDIAN:
        {
            u8Info = uvcx_m2_medianCtrl.u8Info;
            u16Len = uvcx_m2_medianCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_medianCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_medianCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_medianCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_medianCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_medianCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_MEDIAN] = 1;
            break;
        }
        case XCS_M2_LD :
        {
            u8Info = uvcx_m2_lumadnsCtrl.u8Info;
            u16Len = uvcx_m2_lumadnsCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_lumadnsCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_lumadnsCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_lumadnsCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_lumadnsCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_lumadnsCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_LD] = 1;
            break;
        }
        case XCS_M2_LD_REF :
        {
            u8Info = uvcx_m2_lumadnsrefCtrl.u8Info;
            u16Len = uvcx_m2_lumadnsrefCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_lumadnsrefCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_lumadnsrefCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_lumadnsrefCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_lumadnsrefCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_lumadnsrefCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_LD_REF] = 1;
            break;
        }
        case XCS_M2_LTM :
        {
            u8Info = uvcx_m2_ltmCtrl.u8Info;
            u16Len = uvcx_m2_ltmCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_ltmCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_ltmCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_ltmCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_ltmCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_ltmCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_LTM] = 1;
            break;
        }
        case XCS_M2_DOG :
        {
            u8Info = uvcx_m2_dogdenoiseCtrl.u8Info;
            u16Len = uvcx_m2_dogdenoiseCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_dogdenoiseCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_dogdenoiseCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_dogdenoiseCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_dogdenoiseCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_dogdenoiseCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_DOG] = 1;
            break;
        }
        case XCS_M2_CHROMA_DENOISE :
        {
            u8Info = uvcx_m2_chromadenoiseCtrl.u8Info;
            u16Len = uvcx_m2_chromadenoiseCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_chromadenoiseCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_chromadenoiseCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_chromadenoiseCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_chromadenoiseCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_chromadenoiseCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_CHROMA_DENOISE] = 1;
            break;
        }
        case XCS_M2_COL_COM :
        {
            u8Info = uvcx_m2_colorcombineCtrl.u8Info;
            u16Len = uvcx_m2_colorcombineCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_colorcombineCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_colorcombineCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_colorcombineCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_colorcombineCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_colorcombineCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_COL_COM] = 1;
            break;
        }
        case XCS_M2_L3D_TABLE_TRANSFER :
        {
            u8Info = uvcx_m2_lut3dtabletransferCtrl.u8Info;
            u16Len = uvcx_m2_lut3dtabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_lut3dtabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_lut3dtabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_lut3dtabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_lut3dtabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_lut3dtabletransferCtrl.def;
            break;
        }
        case XCS_M2_COL_CONV :
        {
            u8Info = uvcx_m2_colorconvertCtrl.u8Info;
            u16Len = uvcx_m2_colorconvertCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_colorconvertCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_colorconvertCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_colorconvertCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_colorconvertCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_colorconvertCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_COL_CONV] = 1;
            break;
        }
        case XCS_M2_GAMMA :
        {
            u8Info = uvcx_m2_gammaCtrl.u8Info;
            u16Len = uvcx_m2_gammaCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_gammaCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_gammaCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_gammaCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_gammaCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_gammaCtrl.def;
        	if (request == USB_bRequest_Video_SET_CUR)
        		updateM2Table[XCS_M2_GAMMA] = 1;
            break;
        }
        case XCS_M2_GAMMA_TABEL_TRANSFER :
        {
            u8Info = uvcx_m2_gammatabletransferCtrl.u8Info;
            u16Len = uvcx_m2_gammatabletransferCtrl.u16Len;
            pCur   = (uint8_t *)&uvcx_m2_gammatabletransferCtrl.cur;
            pMin   = (uint8_t *)&uvcx_m2_gammatabletransferCtrl.min;
            pMax   = (uint8_t *)&uvcx_m2_gammatabletransferCtrl.max;
            pRes   = (uint8_t *)&uvcx_m2_gammatabletransferCtrl.res;
            pDef   = (uint8_t *)&uvcx_m2_gammatabletransferCtrl.def;
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
    	printf("[ispXUControlsM2] Invalid Request controlSelector %d request %d len %d\n", controlSelector, request, u16Len);
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
    	if (XCS_M2_LSC_TABLE_TRANSFER == controlSelector)
    	{
    		memmove(uvcx_m2_lsctableCtrl.cur.lscTable + uvcx_m2_lsctabletransferCtrl.cur.header[0] * TABLE_CHUNK / 2, uvcx_m2_lsctabletransferCtrl.cur.payload, TABLE_CHUNK);

    		if ((uvcx_m2_lsctabletransferCtrl.cur.header[0] + 1) == uvcx_m2_lsctabletransferCtrl.cur.header[1]) {
    			updateM2Table[XCS_M2_LSC] = 1;
    	    }
   	    }
    	if (XCS_M2_L3D_TABLE_TRANSFER == controlSelector)
    	{
    		uint8_t *pTable = (uint8_t *)uvcx_m2_lut3dtableCtrl.cur.lut3dTable;
    		memmove(pTable + uvcx_m2_lut3dtabletransferCtrl.cur.header[0] * TABLE_CHUNK_SIZE,
    				uvcx_m2_lut3dtabletransferCtrl.cur.payload,
					uvcx_m2_lut3dtabletransferCtrl.cur.header[2]);
//
//    		printf("XCS_M2_L3D_TABLE_TRANSFER %d %d %d\n",
//    				uvcx_m2_lut3dtabletransferCtrl.cur.header[0],
//					uvcx_m2_lut3dtabletransferCtrl.cur.header[1],
//					uvcx_m2_lut3dtabletransferCtrl.cur.header[2]);

    		if ((uvcx_m2_lut3dtabletransferCtrl.cur.header[0] + 1) == uvcx_m2_lut3dtabletransferCtrl.cur.header[1])
    		{
    			updateM2Table[XCS_M2_L3D_TABLE_TRANSFER] = 1;
    	    }
   	    }
    	if (XCS_M2_GAMMA_TABEL_TRANSFER == controlSelector)
    	{
    		uint8_t *pTable = (uint8_t *)uvcx_m2_gammatableCtrl.cur.gammaTable;
    		memmove(pTable + uvcx_m2_gammatabletransferCtrl.cur.header[0] * TABLE_CHUNK_SIZE,
    				uvcx_m2_gammatabletransferCtrl.cur.payload,
					uvcx_m2_gammatabletransferCtrl.cur.header[2]);

    		if ((uvcx_m2_gammatabletransferCtrl.cur.header[0] + 1) == uvcx_m2_gammatabletransferCtrl.cur.header[1])
    		{
    			updateM2Table[XCS_M2_GAMMA_TABEL_TRANSFER] = 1;
    	    }
    	}

   	}


}

void getConfigM2(icIspConfig *pCfg)
{
	if (updateM2Table[XCS_M2_GENERAL])
	{
		updateM2Table[XCS_M2_GENERAL] = 0;
#ifdef DEBUG
		printf("[general] %s %ld %ld  %ld  %ld  %ld \n",
				uvcx_m2_generalCtrl.cur.fileName,
				uvcx_m2_generalCtrl.cur.image_width, uvcx_m2_generalCtrl.cur.image_height,
				uvcx_m2_generalCtrl.cur.skip_lines,  uvcx_m2_generalCtrl.cur.bits,
				uvcx_m2_generalCtrl.cur.bayer_order);
#endif
	}

	if (updateM2Table[XCS_M2_BLC])
	{
	  updateM2Table[XCS_M2_BLC] = 0;
	  pCfg->blc.b  = uvcx_m2_blcCtrl.cur.b;
	  pCfg->blc.gb = uvcx_m2_blcCtrl.cur.gb;
	  pCfg->blc.gr = uvcx_m2_blcCtrl.cur.gr;
	  pCfg->blc.r  = uvcx_m2_blcCtrl.cur.r;
#ifdef DEBUG
	  printf("[blc] b gb br r  %ld %ld %ld %ld \n",pCfg->blc.b, pCfg->blc.gb, pCfg->blc.gr, pCfg->blc.r);
#endif
	}
	if (updateM2Table[XCS_M2_SIGMA_DENOISE])
	{
	  updateM2Table[XCS_M2_SIGMA_DENOISE] = 0;

	  pCfg->sigma.noiseFloor = uvcx_m2_sgmdnsCtrl.cur.noiseFloor;
	  pCfg->sigma.thresh1P0  = uvcx_m2_sgmdnsCtrl.cur.thresh1P0;
	  pCfg->sigma.thresh2P0  = uvcx_m2_sgmdnsCtrl.cur.thresh2P0;
	  pCfg->sigma.thresh1P1  = uvcx_m2_sgmdnsCtrl.cur.thresh1P1;
	  pCfg->sigma.thresh2P1  = uvcx_m2_sgmdnsCtrl.cur.thresh2P1;
	  pCfg->sigma.thresh1P2  = uvcx_m2_sgmdnsCtrl.cur.thresh1P2;
	  pCfg->sigma.thresh2P2  = uvcx_m2_sgmdnsCtrl.cur.thresh2P2;
	  pCfg->sigma.thresh1P3  = uvcx_m2_sgmdnsCtrl.cur.thresh1P3;
	  pCfg->sigma.thresh2P3  = uvcx_m2_sgmdnsCtrl.cur.thresh2P3;
#ifdef DEBUG
	  printf("[sgm dns] %ld %ld %ld %ld %ld %ld %ld %ld %ld\n" ,
			  pCfg->sigma.noiseFloor,
			  pCfg->sigma.thresh1P0, pCfg->sigma.thresh2P0,
			  pCfg->sigma.thresh1P1, pCfg->sigma.thresh2P1,
			  pCfg->sigma.thresh1P2, pCfg->sigma.thresh2P2,
			  pCfg->sigma.thresh1P3, pCfg->sigma.thresh2P3);
#endif
	}
	if (updateM2Table[XCS_M2_LSC])
	{
	  updateM2Table[XCS_M2_LSC] = 0;

#ifdef DEBUG
	  printf("LSC TABLE w %ld h %ld\n", uvcx_m2_lscCtrl.cur.lscWidth, uvcx_m2_lscCtrl.cur.lscHeight);
      for (uint32_t i = 0 ; i < uvcx_m2_lscCtrl.cur.lscHeight; i++)
      {
    	  for (uint32_t j = 0 ; j < uvcx_m2_lscCtrl.cur.lscWidth; j++)
    	  {
    		  printf("0x%04x ", uvcx_m2_lsctableCtrl.cur.lscTable[i * uvcx_m2_lscCtrl.cur.lscWidth + j]);
    	  }
    	  printf("\n");
      }
#endif

	  pCfg->lsc.lscWidth  = uvcx_m2_lscCtrl.cur.lscWidth;
	  pCfg->lsc.lscStride = uvcx_m2_lscCtrl.cur.lscWidth;
	  pCfg->lsc.lscHeight = uvcx_m2_lscCtrl.cur.lscHeight;
	  pCfg->lsc.pLscTable = uvcx_m2_lsctableCtrl.cur.lscTable;
	}

	if (updateM2Table[XCS_M2_RAW])
	{
	  updateM2Table[XCS_M2_RAW] = 0;
	  //gain
	  pCfg->raw.gainGr = uvcx_m2_rawCtrl.cur.gainGr;
	  pCfg->raw.gainR  = uvcx_m2_rawCtrl.cur.gainR;
	  pCfg->raw.gainB  = uvcx_m2_rawCtrl.cur.gainB;
	  pCfg->raw.gainGb = uvcx_m2_rawCtrl.cur.gainGb;
	  //sat
	  pCfg->raw.clampGr = uvcx_m2_rawCtrl.cur.clampGr;
	  pCfg->raw.clampR  = uvcx_m2_rawCtrl.cur.clampR;
	  pCfg->raw.clampB  = uvcx_m2_rawCtrl.cur.clampB;
	  pCfg->raw.clampGb = uvcx_m2_rawCtrl.cur.clampGb;
	  //imbal
	  pCfg->raw.grgbImbalPlatDark    = uvcx_m2_rawCtrl.cur.grgbImbalPlatDark;
	  pCfg->raw.grgbImbalDecayDark   = uvcx_m2_rawCtrl.cur.grgbImbalDecayDark;
	  pCfg->raw.grgbImbalPlatBright  = uvcx_m2_rawCtrl.cur.grgbImbalPlatBright;
	  pCfg->raw.grgbImbalDecayBright = uvcx_m2_rawCtrl.cur.grgbImbalDecayBright;
	  pCfg->raw.grgbImbalThr         = uvcx_m2_rawCtrl.cur.grgbImbalThr;
	  //dpc
	  pCfg->raw.dpcAlphaHotG   = uvcx_m2_rawCtrl.cur.dpcAlphaHotG;
	  pCfg->raw.dpcAlphaHotRb  = uvcx_m2_rawCtrl.cur.dpcAlphaHotRb;
	  pCfg->raw.dpcAlphaColdG  = uvcx_m2_rawCtrl.cur.dpcAlphaColdG;
	  pCfg->raw.dpcAlphaColdRb = uvcx_m2_rawCtrl.cur.dpcAlphaColdRb;
	  pCfg->raw.dpcNoiseLevel  = uvcx_m2_rawCtrl.cur.dpcNoiseLevel;
	  //bits
	  pCfg->raw.outputBits  = uvcx_m2_rawCtrl.cur.outputBits;

#ifdef DEBUG
	  printf("[raw][gain] gr %ld r  %ld b  %ld gb %ld\n", pCfg->raw.gainGr,  pCfg->raw.gainR,  pCfg->raw.gainB,  pCfg->raw.gainGb);
	  printf("[raw][sat]  gr %ld r  %ld b  %ld gb %ld\n", pCfg->raw.clampGr, pCfg->raw.clampR, pCfg->raw.clampB, pCfg->raw.clampGb);
	  printf("[raw][imbal]pd %ld dd %ld pb %ld db %ld thr %ld\n",
			  pCfg->raw.grgbImbalPlatDark,   pCfg->raw.grgbImbalDecayDark,
			  pCfg->raw.grgbImbalPlatBright, pCfg->raw.grgbImbalDecayBright,
			  pCfg->raw.grgbImbalThr);
	  printf("[raw][dpc]hg %ld hrb %ld cg %ld crb %ld nlv %ld\n",
			  pCfg->raw.dpcAlphaHotG,  pCfg->raw.dpcAlphaHotRb,
			  pCfg->raw.dpcAlphaColdG, pCfg->raw.dpcAlphaColdRb,
			  pCfg->raw.dpcNoiseLevel);
	  printf("[raw][ob] %ld\n",pCfg->raw.outputBits);
#endif

	}
	if (updateM2Table[XCS_M2_DEMOSAIC])
	{
	  updateM2Table[XCS_M2_DEMOSAIC] = 0;
	  pCfg->demosaic.dewormGradientMul = uvcx_m2_demosaicCtrl.cur.dewormGradientMul;
	  pCfg->demosaic.dewormSlope       = uvcx_m2_demosaicCtrl.cur.dewormSlope;
	  pCfg->demosaic.dewormOffset      = uvcx_m2_demosaicCtrl.cur.dewormOffset;
	  pCfg->demosaic.lumaWeightR       = uvcx_m2_demosaicCtrl.cur.lumaWeightR;
	  pCfg->demosaic.lumaWeightG       = uvcx_m2_demosaicCtrl.cur.lumaWeightG;
	  pCfg->demosaic.lumaWeightB       = uvcx_m2_demosaicCtrl.cur.lumaWeightB;

#ifdef DEBUG
	  printf("[demosaic] mix %ld %ld %ld lumaweight %ld %ld %ld\n",
			  pCfg->demosaic.dewormGradientMul, pCfg->demosaic.dewormSlope, pCfg->demosaic.dewormOffset,
			  pCfg->demosaic.lumaWeightR,       pCfg->demosaic.lumaWeightG, pCfg->demosaic.lumaWeightB);
#endif
	}
	if (updateM2Table[XCS_M2_CHROMA_GEN])
	{

	  updateM2Table[XCS_M2_CHROMA_GEN] = 0;
	  pCfg->chromaGen.epsilon     = uvcx_m2_chromagenerationCtrl.cur.epsilon;
	  pCfg->chromaGen.kr          = uvcx_m2_chromagenerationCtrl.cur.kr;
	  pCfg->chromaGen.kg          = uvcx_m2_chromagenerationCtrl.cur.kg;
	  pCfg->chromaGen.kb          = uvcx_m2_chromagenerationCtrl.cur.kb;
	  pCfg->chromaGen.lumaCoeffR  = uvcx_m2_chromagenerationCtrl.cur.lumaCoeffR;
	  pCfg->chromaGen.lumaCoeffG  = uvcx_m2_chromagenerationCtrl.cur.lumaCoeffG;
	  pCfg->chromaGen.lumaCoeffB  = uvcx_m2_chromagenerationCtrl.cur.lumaCoeffB;
	  pCfg->chromaGen.pfrStrength = uvcx_m2_chromagenerationCtrl.cur.pfrStrength;
	  pCfg->chromaGen.desatOffset = uvcx_m2_chromagenerationCtrl.cur.desatOffset;
	  pCfg->chromaGen.desatSlope  = uvcx_m2_chromagenerationCtrl.cur.desatSlope;

#ifdef DEBUG
	  printf("[chromagen] %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n",
			  pCfg->chromaGen.epsilon,
			  pCfg->chromaGen.kr,
			  pCfg->chromaGen.kg,
			  pCfg->chromaGen.kb,
			  pCfg->chromaGen.lumaCoeffR,
			  pCfg->chromaGen.lumaCoeffG,
			  pCfg->chromaGen.lumaCoeffB,
			  pCfg->chromaGen.pfrStrength,
			  pCfg->chromaGen.desatOffset,
			  pCfg->chromaGen.desatSlope);
#endif
	}


	if (updateM2Table[XCS_M2_SHARPEN])
	{
		updateM2Table[XCS_M2_SHARPEN] = 0;
		memmove(&pCfg->sharpen, &uvcx_m2_sharpenCtrl.cur, sizeof(uvcx_m2_sharpen_data_t));
#ifdef DEBUG
		printf("[sharpen] sigma %f 0x%04x 0x%04xd 0x%04x 0x%04x | 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x | 0x%04x 0x%04x 0x%04x 0x%04x | 0x%04x\n",
				pCfg->sharpen.sigma,
				pCfg->sharpen.sharpenCoeffs[0],
				pCfg->sharpen.sharpenCoeffs[1],
				pCfg->sharpen.sharpenCoeffs[2],
				pCfg->sharpen.sharpenCoeffs[3],
	    		pCfg->sharpen.strengthDarken,
				pCfg->sharpen.strengthLighten,
				pCfg->sharpen.alpha,
				pCfg->sharpen.overshoot,
				pCfg->sharpen.undershoot,
	    		pCfg->sharpen.rangeStop0,
				pCfg->sharpen.rangeStop1,
				pCfg->sharpen.rangeStop2,
				pCfg->sharpen.rangeStop3,
				pCfg->sharpen.minThr);
#endif
	}
	if (updateM2Table[XCS_M2_MEDIAN])
	{
		updateM2Table[XCS_M2_MEDIAN] = 0;
		pCfg->median.kernelSize = uvcx_m2_medianCtrl.cur.kernelSize;
		pCfg->median.slope      = uvcx_m2_medianCtrl.cur.slope;
		pCfg->median.offset     = uvcx_m2_medianCtrl.cur.offset;
#ifdef DEBUG
		printf("[median] k %ld s %ld o %ld\n", pCfg->median.kernelSize, pCfg->median.slope, pCfg->median.offset);
#endif
	}
	if (updateM2Table[XCS_M2_LD])
	{
		updateM2Table[XCS_M2_LD] = 0;
		pCfg->lumaDenoise.strength = uvcx_m2_lumadnsCtrl.cur.strength;
		pCfg->lumaDenoise.alpha    = uvcx_m2_lumadnsCtrl.cur.alpha;
		pCfg->lumaDenoise.f2       = uvcx_m2_lumadnsCtrl.cur.f2;
		pCfg->lumaDenoise.bitpos   = uvcx_m2_lumadnsCtrl.cur.bitpos;
		memmove(pCfg->lumaDenoise.lut, uvcx_m2_lumadnsCtrl.cur.lut, 32);

#ifdef DEBUG
		printf("[lumadns] str %f alpha %ld bitpos %ld f2 %ld\n",
				pCfg->lumaDenoise.strength, pCfg->lumaDenoise.alpha,
				pCfg->lumaDenoise.bitpos, pCfg->lumaDenoise.f2);
		printf("[lumadns] lut");
		for (uint32_t i = 0; i < 32; i++)
			printf("%d ", pCfg->lumaDenoise.lut[i]);
		printf("\n");
#endif
	}
	if (updateM2Table[XCS_M2_LD_REF])
	{
		updateM2Table[XCS_M2_LD_REF] = 0;
		pCfg->lumaDenoiseRef.gamma         = uvcx_m2_lumadnsrefCtrl.cur.gamma;
		pCfg->lumaDenoiseRef.angle_of_view = uvcx_m2_lumadnsrefCtrl.cur.angle_of_view;
		pCfg->lumaDenoiseRef.shift         = uvcx_m2_lumadnsrefCtrl.cur.shift;
		memmove(pCfg->lumaDenoiseRef.lutDist,        uvcx_m2_lumadnsrefCtrl.cur.lutDist,        32 * sizeof(uint64_t));
		memmove(pCfg->lumaDenoiseRef.lutGamma0_32,   uvcx_m2_lumadnsrefCtrl.cur.lutGamma0_32,   9 * sizeof(uint8_t));
		memmove(pCfg->lumaDenoiseRef.lutGamma32_255, uvcx_m2_lumadnsrefCtrl.cur.lutGamma32_255, 9 * sizeof(uint8_t));

#ifdef DEBUG
		printf("[lumadnsref] aov %f gamma %f shift %ld\n",
				pCfg->lumaDenoiseRef.angle_of_view, pCfg->lumaDenoiseRef.gamma, pCfg->lumaDenoiseRef.shift);
		printf("[lumadns] lutDist");
		for (uint32_t i = 0; i < 32; i++)
			printf("%d ", pCfg->lumaDenoise.lut[i]);
		printf("\n");
		printf("[lumadns] lutGamma0_32");
		for (uint32_t i = 0; i < 9; i++)
			printf("%d ", pCfg->lumaDenoiseRef.lutGamma0_32[i]);
		printf("\n");
		printf("[lumadns] lutGamma0_32");
		for (uint32_t i = 0; i < 9; i++)
			printf("%d ", pCfg->lumaDenoiseRef.lutGamma32_255[i]);
		printf("\n");
#endif
	}
	if (updateM2Table[XCS_M2_LTM])
	{
		updateM2Table[XCS_M2_LTM] = 0;
		pCfg->ltm.thr = uvcx_m2_ltmCtrl.cur.thr;
		memmove(pCfg->ltm.curves, uvcx_m2_ltmCtrl.cur.table, MAX_LTM_CURVES * sizeof(uint16_t));

#ifdef DEBUG
	    printf("[ltm] thr %ld\n", pCfg->ltm.thr);
	    for (uint32_t i = 0; i < 16; i++)
	    {
	    	for (uint32_t j = 0; j < 8; j++)
	    	{
	    		printf("%d ", pCfg->ltm.curves[8 * i + j]);
	    	}
	    	printf("\n");
	    }
#endif
	}
	if (updateM2Table[XCS_M2_DOG])
	{
		updateM2Table[XCS_M2_DOG] = 0;
		pCfg->dog.thr      = uvcx_m2_dogdenoiseCtrl.cur.thr;
	    pCfg->dog.strength = uvcx_m2_dogdenoiseCtrl.cur.strength;
	    pCfg->dog.sigma11  = uvcx_m2_dogdenoiseCtrl.cur.sigma11;
	    pCfg->dog.sigma15  = uvcx_m2_dogdenoiseCtrl.cur.sigma15;
	    memmove(pCfg->dog.coeffs11, uvcx_m2_dogdenoiseCtrl.cur.coeffs11, 6);
	    memmove(pCfg->dog.coeffs15, uvcx_m2_dogdenoiseCtrl.cur.coeffs15, 8);

#ifdef DEBUG
	    printf("[dog] thr %ld str %ld s11 %f s15 %f\n",
	    		pCfg->dog.thr, pCfg->dog.strength, pCfg->dog.sigma11, pCfg->dog.sigma15);
	    printf("[dog][s11] %d %d %d %d %d %d \n",
	    		pCfg->dog.coeffs11[0], pCfg->dog.coeffs11[1], pCfg->dog.coeffs11[2],
				pCfg->dog.coeffs11[3], pCfg->dog.coeffs11[4], pCfg->dog.coeffs11[5]);
	    printf("[dog][s15] %d %d %d %d %d %d %d %d\n",
	    		pCfg->dog.coeffs15[0], pCfg->dog.coeffs15[1],
				pCfg->dog.coeffs15[2], pCfg->dog.coeffs15[3],
				pCfg->dog.coeffs15[4], pCfg->dog.coeffs15[5],
				pCfg->dog.coeffs15[6], pCfg->dog.coeffs15[7]);
#endif
	}
	if (updateM2Table[XCS_M2_CHROMA_DENOISE])
	{
		updateM2Table[XCS_M2_CHROMA_DENOISE] = 0;
		memmove(&pCfg->chromaDenoise, &uvcx_m2_chromadenoiseCtrl.cur, sizeof(uvcx_m2_chromadenoise_data_t));

#ifdef DEBUG
		printf("[chromadns] %ld %ld %ld | %ld %ld | %ld %ld | %ld %ld %ld | %ld %ld %ld\n",
				pCfg->chromaDenoise.th_r,
				pCfg->chromaDenoise.th_g,
				pCfg->chromaDenoise.th_b,
				pCfg->chromaDenoise.limit,
				pCfg->chromaDenoise.hEnab,
				pCfg->chromaDenoise.greyDesatSlope,
				pCfg->chromaDenoise.greyDesatOffset,
				pCfg->chromaDenoise.greyCr,
				pCfg->chromaDenoise.greyCg,
				pCfg->chromaDenoise.greyCb,
				pCfg->chromaDenoise.convCoeffCenter,
				pCfg->chromaDenoise.convCoeffEdge,
				pCfg->chromaDenoise.convCoeffCorner);
#endif
	}
	if (updateM2Table[XCS_M2_COL_COM])
	{
	  updateM2Table[XCS_M2_COL_COM] = 0;

	  for (int i = 0; i < 9; i++)
	  {
		  pCfg->colorCombine.ccm[i] = uvcx_m2_colorcombineCtrl.cur.ccm[i];
	  }
	  for (int i = 0; i < 3; i++)
	  {
		  pCfg->colorCombine.ccmOff[i] = uvcx_m2_colorcombineCtrl.cur.ccmoff[i];
	  }
	  pCfg->colorCombine.kr = uvcx_m2_colorcombineCtrl.cur.kr;
	  pCfg->colorCombine.kg = uvcx_m2_colorcombineCtrl.cur.kg;
	  pCfg->colorCombine.kb = uvcx_m2_colorcombineCtrl.cur.kb;

#ifdef DEBUG
	  printf("[color combine] ccm    [%f %f %f %f %f %f %f %f %f \n",
			  pCfg->colorCombine.ccm[0], pCfg->colorCombine.ccm[1], pCfg->colorCombine.ccm[2],
			  pCfg->colorCombine.ccm[3], pCfg->colorCombine.ccm[4], pCfg->colorCombine.ccm[5],
			  pCfg->colorCombine.ccm[6], pCfg->colorCombine.ccm[7], pCfg->colorCombine.ccm[8]);
	  printf("[color combine] ccmoff [%f %f %f] \n",
			  pCfg->colorCombine.ccmOff[0], pCfg->colorCombine.ccmOff[1], pCfg->colorCombine.ccmOff[2]);
	  printf("[color combine] k rgb  [%ld %ld %ld] \n",
			  pCfg->colorCombine.kr, pCfg->colorCombine.kg, pCfg->colorCombine.kb);
#endif
	}
	if (updateM2Table[XCS_M2_L3D_TABLE_TRANSFER])
	{
		updateM2Table[XCS_M2_L3D_TABLE_TRANSFER] = 0;
#ifdef DEBUG
//		for (uint32_t i = 0 ; i < 256; i++)
//		{
//			for (uint32_t j = 0 ; j < 64; j++)
//		    {
//				printf("%d ", uvcx_m2_lut3dtableCtrl.cur.lut3dTable[i * 64 + j]);
//		    }
//		    printf("\n");
//		}
#endif
	}

	if (updateM2Table[XCS_M2_COL_CONV])
	{
		updateM2Table[XCS_M2_COL_CONV] = 0;
		memmove(pCfg->colorConvert.mat,    uvcx_m2_colorconvertCtrl.cur.mat, 9 * sizeof(float));
		memmove(pCfg->colorConvert.offset, uvcx_m2_colorconvertCtrl.cur.off, 3 * sizeof(float));
#ifdef DEBUG
		printf("[color convert] mat %f %f %f %f %f %f %f %f %f\n",
				pCfg->colorConvert.mat[0], pCfg->colorConvert.mat[1], pCfg->colorConvert.mat[2],
				pCfg->colorConvert.mat[3], pCfg->colorConvert.mat[4], pCfg->colorConvert.mat[5],
				pCfg->colorConvert.mat[6], pCfg->colorConvert.mat[7], pCfg->colorConvert.mat[8]);
		printf("[color convert] off %f %f %f\n",
				pCfg->colorConvert.offset[0], pCfg->colorConvert.offset[1], pCfg->colorConvert.offset[2]);
#endif
	}
	if (updateM2Table[XCS_M2_GAMMA])
	{
		updateM2Table[XCS_M2_GAMMA] = 0;
		pCfg->gamma.size = uvcx_m2_gammaCtrl.cur.size;
#ifdef DEBUG
		printf("[gamma] size %ld\n", pCfg->gamma.size);
#endif
	}
	if (updateM2Table[XCS_M2_GAMMA_TABEL_TRANSFER])
	{
		updateM2Table[XCS_M2_GAMMA_TABEL_TRANSFER] = 0;
		pCfg->gamma.table = uvcx_m2_gammatableCtrl.cur.gammaTable;
#ifdef DEBUG
    			for (uint32_t i = 0 ; i < 256; i++)
    			{
    				for (uint32_t j = 0 ; j < 8; j++)
    				{
    					printf("0x%04x ", pCfg->gamma.table[i * 8 + j]);
    				}
    				printf("\n");
    			}
#endif
	}

}
