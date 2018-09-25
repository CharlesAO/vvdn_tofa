
/* =============================================================================
*
* This product includes software from MM Solutions AD.
* Portions of this software are based in part on the work of MM Solutions AD. 
* Because Movidius has included the MM Solutions AD software in this product, Movidius is 
* required to include the following text that accompanied such software:
* Copyright (c) 2013-2016 MM Solutions AD
*
* =========================================================================== */

//input image settings
#ifndef CALIB_HANDLER_RAW_WIDTH
#define CALIB_HANDLER_RAW_WIDTH 4056
#endif
#ifndef CALIB_HANDLER_RAW_HEIGHT
#define CALIB_HANDLER_RAW_HEIGHT 3040
#endif
#define CALIB_HANDLER_PATHERN CALIB_BAYER_FORMAT_RGGB
#define CALIB_HANDLER_X_GRID_NUMBER 9
#define CALIB_HANDLER_Y_GRID_NUMBER 7
#define CALIB_HANDLER_PAX_SIZE 64
#define CALIB_HANDLER_DP 50
// EEPROM Data needed up to version 1.4
#define CALIB_HANDLER_DESCR_SIZE 48
#define CALIB_HANDLER_LAYOUT_VERSION_MAJOR 1
#define CALIB_HANDLER_LAHOUT_VERSION_MINOR 1
//  Module description
static const char CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER[] = "01234567890";
static const char CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO[] = "27/06/2016";
//Common sensor description
#define CALIB_HANDLER_MODULE_DTP_ID  8
#define CALIB_HANDLER_CH_RESERVED_1  0
#define CALIB_HANDLER_SENSOR_BASE_ISO   100
#define CALIB_HANDLER_SENSOR_BITS_PER_PIXEL   10
#define CALIB_HANDLER_CH_RESERVED_2   0
//Content Validation
#define CALIB_HANDLER_DATA_SIZE   518
//Common Algorithm HEADER
#define CALIB_HANDLER_ALG_ID   CALIB_ALGO_LSC
#define CALIB_HANDLER_CH_RESERVED_3   0
#define CALIB_HANDLER_DATA_LEN   514
//LSC HEADER
#define CALIB_HANDLER_LSC_NUM_PLAINS   4
#define CALIB_HANDLER_LSC_FORMAT   CALIB_LSC_FORMAT_INTERLEAVED_UNCOMPRESSED
#define CALIB_HANDLER_LSC_NUM_COLOR_TEMPERATURES   1
#define CALIB_HANDLER_LSC_RESERVED   0
//Calibration data per color temperature
#define CALIB_HANDLER_LSC_DATA_SIZE   506
#define CALIB_HANDLER_LSC_COLOR_TEMPERATURE   5000
