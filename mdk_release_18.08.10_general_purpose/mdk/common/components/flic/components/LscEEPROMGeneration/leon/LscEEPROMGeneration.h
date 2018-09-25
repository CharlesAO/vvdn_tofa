
/* =============================================================================
*
* This product includes software from MM Solutions AD.
* Portions of this software are based in part on the work of MM Solutions AD. 
* Because Movidius has included the MM Solutions AD software in this product, Movidius is 
* required to include the following text that accompanied such software:
* Copyright (c) 2013-2016 MM Solutions AD
*
* =========================================================================== */

#ifndef _EEPROM_GENERATION_H_
#define _EEPROM_GENERATION_H_

// 1: Includes
// ----------------------------------------------------------------------------

#include "LscEEPROMGenerationDefines.h"

#ifdef __cplusplus
extern "C"
{
#endif

// 2: Defines
// ----------------------------------------------------------------------------

typedef enum
{
    CALIB_UNDEREXPOSURE          =-1,
    CALIB_SUCCESS                = 0,
    CALIB_OVEREXPOSURE           = 1,
    CALIB_INVALID_WIDTH          = 2,
    CALIB_INVALID_HEIGHT         = 3,
    CALIB_INVALID_PATTERN        = 4,
    CALIB_INVALID_ALG_ID         = 5,
    CAlIB_INVALID_LSC_FORMAT     = 6,
    CALIB_INVALID_IN_IMG_FORMAT  = 7,
	// error codes associated with sanity checks conditions:
	CALIB_ER_0_VALUE_EXIST       = 8,  //  1.  No 0 values error
    CALIB_ER_ONE_VAL_CONSISTENCY = 9, //  2.  Some values =1 for each color channel
    CALIB_ER_VAL_EXCEEDS_LIMIT   = 10, //  3.  No values > 0x800;   0x800 value is arbitrary large  could be configurable @build time
	CALIB_ER_LARGER_VALUES_CENTER= 11, //  4.  Values in corners > values in center

} calibration_error_codes;


typedef enum
{
    CALIB_BAYER_FORMAT_GRBG = 0,
    CALIB_BAYER_FORMAT_RGGB = 1,
    CALIB_BAYER_FORMAT_GBRG = 2,
    CALIB_BAYER_FORMAT_BGGR = 3,
} calibration_bayer_format;

typedef enum
{
    CALIB_ALGO_INVALID = 0,
    CALIB_ALGO_AWB = 1,
    CALIB_ALGO_LSC = 2,
    CALIB_ALGO_AF_VCM = 3,
    CALIB_ALGO_AF_PIEZO = 4,
    MAX_CALIB_ALGO = 5,
} calibration_algorithm_id_t;

typedef enum
{
    CALIB_LSC_FORMAT_INTERLEAVED_UNCOMPRESSED = 0,
    CALIB_LSC_FORMAT_INTERLEAVED_PACKED = 1,
    CALIB_LSC_FORMAT_INTERLEAVED_NORMALIZED = 2,
    MAX_CALIB_LSC_FORMAT = 3,
} calibration_lsc_format_t;

typedef struct
{
    ////////////////////// Description validation ////////////////////////
    u16 descr_size;			// Description size (Module+Sensor)
    u32 descr_checksum;		// CRC32 of description contents

    //////////////////////// Module description //////////////////////////
    u8 layout_version_major;				//
    u8 layout_version_minor;				//
    u8 sensor_module_serial_number[12];		// ACSI string
    u8 supplier_module_tracking_info[12];	// reserved

    //////////////////////// Common sensor description /////////////////////////
    u32 module_dtp_id;			//Sensor unique ID – should be equal to DTP
    calibration_bayer_format pathern;					//see calibration_bayer_format
    u8 ch_reserved_1;			//
    u16 rawwidth;				//Max read width – unit pixels
    u16 rawheight;				//Max read height – unit pixels
    u16 sensor_base_iso;		//U16Q8 format
    u8 sensor_bits_per_pixel;	//
    u32 ch_reserved_2;			//

    //////////////////////// Calibration data validation //////////////////////////
    u32 data_size;				//calibration data size
    u32 checksum;				// CRC32 of calibration data

    /////////////////////////////// LSC/AWB //////////////////////////////////////////
    // Common Algorithm HEADER - each algorithm should be proceeded by such header
    calibration_algorithm_id_t alg_id;					//see calibration_algorithm_id_t
    u8 ch_reserved_3;			//could be algorithm version
    u16 data_len;				// total size in bytes of data for algorithm

    // LSC specific data
    // LSC HEADER
    u8 XGridNumber;				        //
    u8 YGridNumber;				        //
    u8 lsc_num_plains;			        //
    calibration_lsc_format_t lsc_format;// see calibration_lsc_format_t
    u8 lsc_num_color_temperatures;	    //calibration_lsc_format_t
    u8 reserved;					    //Padding

    // LSC calibration data per color temperature
    u16 lsc_datasize;			// size of LSC calibration data for temperature
    u16 lsc_color_temperature;	//

    //--------
    double paxSize;
    u32 DP;

    u16 width;   //ok
    u16 height;  //ok
    u16 xcenter;  //ok
    u16 ycenter;  //ok

    s32 xstep;
    s32 ystep;
    s32 xoffset;
    s32 yoffset;

    s32 centralPaxelStartX;
    s32 centralPaxelEndX;
    s32 centralPaxelStartY;
    s32 centralPaxelEndY;
    u8 *pInputImg;
    u32 mask;

} calibrationHandler_t;

typedef struct
{
    u16 *RValueGrid;     //used
    u16 *GrValueGrid;    //used
    u16 *GbValueGrid;    //used
    u16 *BValueGrid;     //used
} calibrationOutput_t;

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
calibration_error_codes eepromGenerate(u8 *pInputImg, u32 *pSizeEEPROM, u8 **pbuffEEPROM);

#ifdef __cplusplus
}
#endif

#endif  //_EEPROM_GENERATION_H_
