
/* =============================================================================
*
* This product includes software from MM Solutions AD.
* Portions of this software are based in part on the work of MM Solutions AD. 
* Because Movidius has included the MM Solutions AD software in this product, Movidius is 
* required to include the following text that accompanied such software:
* Copyright (c) 2013-2016 MM Solutions AD
*
* =========================================================================== */
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include "LscEEPROMGeneration.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define VALUE_GRID_MATRIX_SIZE (CALIB_HANDLER_Y_GRID_NUMBER * CALIB_HANDLER_X_GRID_NUMBER)
#define CALIB_GRID_MATRIX_SIZE (CALIB_HANDLER_RAW_WIDTH  / 2) * (CALIB_HANDLER_RAW_HEIGHT / 2)
#define EEPROM_BYTES_SIZE 572
#define CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER_SIZE        12
#define CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO_SIZE        12
#define CALIB_STRING_DEFAULT_VALUE 0x20 //this value was inherited from Matlab application
#define CHECKSUM_SIZE 518
#define CHECKSUM_OFFSET 54
#define CRC_VALUE_OFFSET 50
#define DESCRIPTOR_CRC_OFFSET 6
#define DESCRIPTOR_CRC_SIZE 48
#define DESCRIPTOR_CRC_VALUE_OFFSET 2
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
calibrationHandler_t handles;
calibrationOutput_t out;

u16 __attribute__((section(".ddr.bss"))) outBuffRValueGrid[VALUE_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) outBuffGrValueGrid[VALUE_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) outBuffGbValueGrid[VALUE_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) outBuffBValueGrid[VALUE_GRID_MATRIX_SIZE];

u16 __attribute__((section(".ddr.bss"))) buffR[CALIB_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) buffGr[CALIB_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) buffGb[CALIB_GRID_MATRIX_SIZE];
u16 __attribute__((section(".ddr.bss"))) buffB[CALIB_GRID_MATRIX_SIZE];

//buffer that will be filled with EEPROM data
u8 __attribute__((section(".ddr.bss"))) eepromBuff[EEPROM_BYTES_SIZE];

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

static calibration_error_codes initHandler(u8 *pInputImg, calibrationHandler_t *pCalibHandle)
{
	switch(CALIB_HANDLER_SENSOR_BITS_PER_PIXEL)
	{
	case 10:
		pCalibHandle->mask = 0x03FF;
		break;
	case 12:
		pCalibHandle->mask = 0x0FFF;
		break;

	}
    // Fill input parameters
    pCalibHandle->pInputImg = pInputImg;
    pCalibHandle->rawwidth = CALIB_HANDLER_RAW_WIDTH;		// image width x
    if(CALIB_HANDLER_RAW_WIDTH %2 == 1)
        return CALIB_INVALID_WIDTH;
    pCalibHandle->rawheight = CALIB_HANDLER_RAW_HEIGHT;    	// image height y
    if(CALIB_HANDLER_RAW_HEIGHT %2 == 1)
        return CALIB_INVALID_HEIGHT;
    if(CALIB_HANDLER_PATHERN != CALIB_BAYER_FORMAT_GRBG &&
       CALIB_HANDLER_PATHERN != CALIB_BAYER_FORMAT_RGGB &&
       CALIB_HANDLER_PATHERN != CALIB_BAYER_FORMAT_GBRG &&
       CALIB_HANDLER_PATHERN != CALIB_BAYER_FORMAT_BGGR )
        return CALIB_INVALID_PATTERN; //invalid pattern

    pCalibHandle->pathern = CALIB_HANDLER_PATHERN;
    pCalibHandle->XGridNumber = CALIB_HANDLER_X_GRID_NUMBER; // Number of ROI on X axis,!!! Auto grid calculation works for not even grid only
    pCalibHandle->YGridNumber = CALIB_HANDLER_Y_GRID_NUMBER; // Number of ROI on Y axis,!!! Auto grid calculation works for not even grid only
    pCalibHandle->paxSize = CALIB_HANDLER_PAX_SIZE;
    pCalibHandle->DP = CALIB_HANDLER_DP;                      // data pedestal value for raw bit dept

    //	EEPROM Data needed
    pCalibHandle->descr_size = CALIB_HANDLER_DESCR_SIZE;

    //  Module description
    pCalibHandle->layout_version_major = CALIB_HANDLER_LAYOUT_VERSION_MAJOR;
    pCalibHandle->layout_version_minor = CALIB_HANDLER_LAHOUT_VERSION_MINOR;

    memset (pCalibHandle->sensor_module_serial_number, CALIB_STRING_DEFAULT_VALUE, CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER_SIZE);
    memset (pCalibHandle->supplier_module_tracking_info, CALIB_STRING_DEFAULT_VALUE, CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO_SIZE);

    strncpy((char *) pCalibHandle->sensor_module_serial_number +
            CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER_SIZE -
            strlen(CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER),
            CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER,
            strlen(CALIB_HANDLER_SENSOR_MODULE_SERIAL_NUMBER));
    strncpy((char *) pCalibHandle->supplier_module_tracking_info +
            CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO_SIZE -
            strlen(CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO),
            CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO,
            strlen(CALIB_HANDLER_SENSOR_MODULE_TARCKING_INFO));

    //  Common sensor description
    pCalibHandle->module_dtp_id = CALIB_HANDLER_MODULE_DTP_ID;
    pCalibHandle->ch_reserved_1 = CALIB_HANDLER_CH_RESERVED_1;
    pCalibHandle->sensor_base_iso = CALIB_HANDLER_SENSOR_BASE_ISO;
    pCalibHandle->sensor_bits_per_pixel = CALIB_HANDLER_SENSOR_BITS_PER_PIXEL;
    pCalibHandle->ch_reserved_2 = CALIB_HANDLER_CH_RESERVED_2;

    //  Content Validation
    pCalibHandle->data_size = CALIB_HANDLER_DATA_SIZE;

    //  LSC/AWB
    //	Common Algorithm HEADER
    if((CALIB_HANDLER_ALG_ID != CALIB_ALGO_INVALID )&&
       (CALIB_HANDLER_ALG_ID != CALIB_ALGO_AWB )&&
       (CALIB_HANDLER_ALG_ID != CALIB_ALGO_LSC )&&
       (CALIB_HANDLER_ALG_ID != CALIB_ALGO_AF_VCM )&&
       (CALIB_HANDLER_ALG_ID != CALIB_ALGO_AF_PIEZO )&&
       (CALIB_HANDLER_ALG_ID != MAX_CALIB_ALGO ))
        return CALIB_INVALID_ALG_ID;
    pCalibHandle->alg_id = CALIB_HANDLER_ALG_ID;
    pCalibHandle->ch_reserved_3 = CALIB_HANDLER_CH_RESERVED_3;
    pCalibHandle->data_len = CALIB_HANDLER_DATA_LEN;

    //	LSC HEADER
    pCalibHandle->lsc_num_plains = CALIB_HANDLER_LSC_NUM_PLAINS;

    // if((CALIB_HANDLER_LSC_FORMAT != CALIB_LSC_FORMAT_INTERLEAVED_UNCOMPRESSED) &&
    //    (CALIB_HANDLER_LSC_FORMAT != CALIB_LSC_FORMAT_INTERLEAVED_PACKED) &&
    //    (CALIB_HANDLER_LSC_FORMAT != CALIB_LSC_FORMAT_INTERLEAVED_NORMALIZED) &&
    //    (CALIB_HANDLER_LSC_FORMAT != MAX_CALIB_LSC_FORMAT))
    //     return -5;
    //for the moment only CALIB_LSC_FORMAT_INTERLEAVED_UNCOMPRESSED is supported
    if((CALIB_HANDLER_LSC_FORMAT != CALIB_LSC_FORMAT_INTERLEAVED_UNCOMPRESSED))
        return CAlIB_INVALID_LSC_FORMAT;

    pCalibHandle->lsc_format = CALIB_HANDLER_LSC_FORMAT;
    pCalibHandle->lsc_num_color_temperatures =
    CALIB_HANDLER_LSC_NUM_COLOR_TEMPERATURES;
    pCalibHandle->reserved = CALIB_HANDLER_LSC_RESERVED;

    //	Calibration data per color temperaturea
    pCalibHandle->lsc_datasize = CALIB_HANDLER_LSC_DATA_SIZE;
    pCalibHandle->lsc_color_temperature = CALIB_HANDLER_LSC_COLOR_TEMPERATURE;

    //  Calculate grid coordinates (rawwidth/2 because of the 4 channels)
    pCalibHandle->width = floor(pCalibHandle->rawwidth / 2);
    pCalibHandle->height = floor(pCalibHandle->rawheight / 2);
    pCalibHandle->xcenter = floor(pCalibHandle->width / 2);
    pCalibHandle->ycenter = floor(pCalibHandle->height / 2);

    // Calculate step(pixels) between ROI
    pCalibHandle->xstep = floor(
                    pCalibHandle->xcenter / pCalibHandle->XGridNumber);
    pCalibHandle->ystep = floor(
                    pCalibHandle->ycenter / pCalibHandle->YGridNumber);

    // Calculate offset for first ROI from first pixel [1,1]
    pCalibHandle->xoffset = pCalibHandle->xcenter
                            - pCalibHandle->xstep * pCalibHandle->XGridNumber;
    pCalibHandle->yoffset = pCalibHandle->ycenter
                            - pCalibHandle->ystep * pCalibHandle->YGridNumber;

    // Correcting central pixel x,y start and end
    pCalibHandle->centralPaxelStartX = pCalibHandle->xoffset
                    + pCalibHandle->xstep * pCalibHandle->XGridNumber
                                       - floor(pCalibHandle->paxSize / 2);
    pCalibHandle->centralPaxelEndX = pCalibHandle->xoffset
                                     + pCalibHandle->xstep
                                     + 2 * pCalibHandle->xstep
                                       * floor(pCalibHandle->XGridNumber / 2)
                                     + floor(pCalibHandle->paxSize / 2);
    pCalibHandle->centralPaxelStartY = pCalibHandle->yoffset
                    + pCalibHandle->ystep * pCalibHandle->YGridNumber
                                       - floor(pCalibHandle->paxSize / 2);
    pCalibHandle->centralPaxelEndY = pCalibHandle->yoffset
                                     + pCalibHandle->ystep
                                     + 2 * pCalibHandle->ystep
                                       * floor(pCalibHandle->YGridNumber / 2)
                                     + floor(pCalibHandle->paxSize / 2);

    return CALIB_SUCCESS;
}

static calibration_error_codes  ReadPlainRaw(u16 *R, u16 *Gr, u16 *Gb, u16 *B,
                         calibrationHandler_t *calibHandle)
{
    // Read Plain RAW file
    u32 heightRaw;
    u32 widthRaw;
    u32 i;
    u32 j;
    u16 *pImg;

    heightRaw = calibHandle->rawheight;
    widthRaw = calibHandle->rawwidth;
    pImg = (u16*) calibHandle->pInputImg;

    switch(calibHandle->pathern)
    {
    case CALIB_BAYER_FORMAT_RGGB:
        for (i = 0; i < heightRaw / 2; i++)
            for (j = 0; j < widthRaw / 2; j++)
            {
                R [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                Gr[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 1]) & calibHandle->mask;
                Gb[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                B [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 1]) & calibHandle->mask;
            }
    break;

    case CALIB_BAYER_FORMAT_GRBG:
        for (i = 0; i < heightRaw / 2; i++)
            for (j = 0; j < widthRaw / 2; j++)
            {
                Gr[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                R [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 1]) & calibHandle->mask;
                B[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                Gb [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 1]) & calibHandle->mask;
            }
    break;

    case CALIB_BAYER_FORMAT_GBRG:
        for (i = 0; i < heightRaw / 2; i++)
            for (j = 0; j < widthRaw / 2; j++)
            {
                Gb [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                B [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 1]) & calibHandle->mask;
                R [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                Gr[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 1]) & calibHandle->mask;
            }
    break;

    case CALIB_BAYER_FORMAT_BGGR:
        for (i = 0; i < heightRaw / 2; i++)
            for (j = 0; j < widthRaw / 2; j++)
            {
                B [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                Gb[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 0) * widthRaw + j * 2 + 1]) & calibHandle->mask;
                Gr[i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 0]) & calibHandle->mask;
                R [i * (widthRaw / 2) + j] = (pImg[(i * 2 + 1) * widthRaw + j * 2 + 1]) & calibHandle->mask;
            }
    break;
    default:
        return CALIB_INVALID_IN_IMG_FORMAT;
        break;
    }

    return CALIB_SUCCESS;
}

static inline double mean2MoviCostum(u16 *buff, u32 startY, u32 endY, u32 startX,
                              u32 endX,
                              u32 matrixWidth)
{
    u32 i;
    u32 j;
    double sum = 0;
    double mean = 0;
    for (i = startY; i <= endY; i++)
        for (j = startX; j <= endX; j++)
        {
            sum = sum + buff[i * matrixWidth + j];
        }
    mean = sum / (double)((endY - startY + 1)*(endX - startX   + 1));
    return mean;
}

static double mean2Costum(u16 *buff, calibrationHandler_t *pCalibHandler)
{
    s32 i;
    s32 j;
    double sum = 0;
    double mean = 0;

    for (i = pCalibHandler->centralPaxelStartY - 1;
                    i < pCalibHandler->centralPaxelEndY; i++)
        for (j = pCalibHandler->centralPaxelStartX - 1;
                        j < pCalibHandler->centralPaxelEndX; j++)
        {

            sum = sum + buff[i * pCalibHandler->width + j];
        }
    mean = sum / (double) ((pCalibHandler->centralPaxelEndY - pCalibHandler->centralPaxelStartY + 1)
                           * (pCalibHandler->centralPaxelEndX - pCalibHandler->centralPaxelStartX + 1));
    return mean;
}

static double max4Double(double *buff)
{
    u32 i;
    double max = 0;
    for (i = 0; i < 4; i++)
        if (max < buff[i])
            max = buff[i];
    return max;
}

static u16 min4Half(u16 e0, u16 e1, u16 e2, u16 e3)
{
    u16 min = e0;

    if(min < e1)
    	min = e1;

    if(min < e2)
        min = e2;

    if(min < e3)
    	min = e3;

    return min;
}


static double CheckUnderexposure(double *centerChannels,
                                 calibrationHandler_t *pCalibHandler)
{
    double underExpWarning;

    if (((pow(2, (double) pCalibHandler->sensor_bits_per_pixel)) * 0.50) > max4Double(
                    centerChannels))
        underExpWarning = max4Double(centerChannels);
    else
        underExpWarning = 0;

    return underExpWarning;
}

static double CheckOverexposure(double *centerChannels,
                                calibrationHandler_t *pCalibHandler)
{
    double overExpWarning;

    if (((pow(2, (double) pCalibHandler->sensor_bits_per_pixel)) * 0.98) < max4Double(
                    centerChannels))
        overExpWarning = max4Double(centerChannels);
    else
        overExpWarning = 0;
    return overExpWarning;
}

static void writeToBuffer(u8 *pBuff, u8 *pData, u32 length, u32 *destOffset)
{
    u32 i;
    for (i = 0; i < length; i++)
        pBuff[i + *destOffset] = pData[i];
    *destOffset = *destOffset + length;
}

static u32 crc32Costum(u8 *pBuffer, u32 byteLength)
{
    u32 crc = 0xFFFFFFFF;
    u32 poly = 0xEDB88320;
    u32 i;
    u32 j;
    u8 *data;
    u32 mask;
    data = pBuffer;
    for (i = 0; i < byteLength; i++)
    {
        crc = crc ^ *data;
        for (j = 0; j < 8; j++)
        {
            mask = ~(crc & 0x01);
            if (mask == 0xFFFFFFFF)
                mask = 0;
            else
                mask = mask + 1;
            crc = (crc >> 1) ^ (poly & mask);
        }
        data++;

    }
    crc = ~crc;
    return crc;
}


static void Save_EEPROM_data(calibrationHandler_t *pCalibHandler,
                             calibrationOutput_t *pOut,
                             u8 * pEEPROMBuff)
{
    u32 i, j;
    u32 destOffset = 0;
    u16 valueToWrite;
    u32 crc;

    //Description Validation
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->descr_size, sizeof(u16),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->descr_checksum,
                  sizeof(u32),
                  &destOffset);

    //  Module description
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->layout_version_major,
                  sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->layout_version_minor,
                  sizeof(u8),
                  &destOffset);

    writeToBuffer(pEEPROMBuff,
                  (u8*) &pCalibHandler->sensor_module_serial_number,
                  12,
                  &destOffset);
    writeToBuffer(pEEPROMBuff,
                  (u8*) &pCalibHandler->supplier_module_tracking_info,
                  12,
                  &destOffset);

    //  Common sensor description
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->module_dtp_id, sizeof(u32),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->pathern, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->ch_reserved_1, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->rawwidth, sizeof(u16),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->rawheight, sizeof(u16),
                  &destOffset);
    u16 sensor_base_iso = pCalibHandler->sensor_base_iso * 256;
    writeToBuffer(pEEPROMBuff, (u8*) &sensor_base_iso, sizeof(u16),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->sensor_bits_per_pixel,
                  sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->ch_reserved_2, sizeof(u8),
                  &destOffset);

    //  Content Validation
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->data_size, sizeof(u32),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->checksum, sizeof(u32),
                  &destOffset);

    //  LSC/AWB
    // Common Algorithm HEADER
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->alg_id, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->ch_reserved_3, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->data_len, sizeof(u16),
                  &destOffset);

    // LSC HEADER
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->XGridNumber, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->YGridNumber, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->lsc_num_plains, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->lsc_format, sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->lsc_num_color_temperatures,
                  sizeof(u8),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->reserved, sizeof(u8),
                  &destOffset);

    // Calibration data per color temperature
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->lsc_datasize, sizeof(u16),
                  &destOffset);
    writeToBuffer(pEEPROMBuff, (u8*) &pCalibHandler->lsc_color_temperature,
                  sizeof(u16),
                  &destOffset);


    for (j = 1; j <= pCalibHandler->YGridNumber; j++)
        for (i = 1; i <= pCalibHandler->XGridNumber; i++)
        {
            valueToWrite =  pOut->RValueGrid[(j - 1) * (u16)(pCalibHandler->XGridNumber) + i - 1];
            writeToBuffer(pEEPROMBuff, (u8 *)(&valueToWrite), sizeof(u16), &destOffset);

            valueToWrite =  pOut->GrValueGrid[(j - 1) * (u16)(pCalibHandler->XGridNumber)
                                            + i
                                            - 1];
            writeToBuffer(pEEPROMBuff, (u8 *)(&valueToWrite), sizeof(u16), &destOffset);

            valueToWrite =  pOut->GbValueGrid[(j - 1) * (u16)(pCalibHandler->XGridNumber)
                                            + i
                                            - 1];
            writeToBuffer(pEEPROMBuff,(u8 *)( &valueToWrite), sizeof(u16), &destOffset);

            valueToWrite =  pOut->BValueGrid[(j - 1) * (u16) (pCalibHandler->XGridNumber)
                                           + i
                                           - 1];
            writeToBuffer(pEEPROMBuff, (u8 *)(&valueToWrite), sizeof(u16), &destOffset);
        }
    crc = crc32Costum( pEEPROMBuff + CHECKSUM_OFFSET, CHECKSUM_SIZE);
    pCalibHandler->checksum = crc;
    destOffset = CRC_VALUE_OFFSET;
    writeToBuffer(pEEPROMBuff, (u8*) (&pCalibHandler->checksum), sizeof(u32), &destOffset);

    crc = crc32Costum(pEEPROMBuff + DESCRIPTOR_CRC_OFFSET, DESCRIPTOR_CRC_SIZE);
    pCalibHandler->descr_checksum = crc;
    destOffset = DESCRIPTOR_CRC_VALUE_OFFSET;
    writeToBuffer(pEEPROMBuff, (u8*)(&pCalibHandler->descr_checksum), sizeof(u32),&destOffset);
}

s32 checkBiggerValuesInCenter(u8 width, u8 height, u16 *grid)
{
    u8 xCenterL; // x center left
    u8 xCenterR; // x center right
    u8 yCenterU; // y center up
    u8 yCenterD; // y center down
    u8 i, j;

    // In the case that length (width or height) is multiple of 2,
    // there are two elements that are in the middle of the image:
    // xCenterL - xCenterR and yCenterU - yCenterD,
    // otherwise only one element will be in the center:
    // xCenterL = xCenterR and  yCenterU = yCenterD

    xCenterL = (width - 1) / 2;
    xCenterR = width / 2;

    yCenterU = (height - 1) / 2;
    yCenterD = height / 2;

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            //check values from left to right
            if (i < xCenterL)
            {
                if (grid[i + j * width] > grid[i + j * width + 1])
                {
                    return -1;
                }
            }

            if (i > xCenterR)
            {
                if (grid[i + j * width] > grid[i + j * width - 1])
                {
                    return -2;
                }
            }

            if (j < yCenterU)
            {
                if (grid[i + j * width] > grid[i + (j + 1) * width])
                {
                    return -3;
                }
            }

            if (j > yCenterD)
            {
                if (grid[i + j * width] > grid[i + (j - 1) * width])
                {
                    return -4;
                }
            }
        }
    }

    return 0;
}

static calibration_error_codes sanityCheck(calibrationHandler_t *phandles,
                                           calibrationOutput_t *pOut)
{
    u32 numPaxelY;
    u32 numPaxelX;
    u16 R, Gr, Gb, B;

    u32 rGridMaxValue;
    u32 grGridMaxValue;
    u32 gbridMaxValue;
    u32 bGridMaxValue;

    rGridMaxValue =  8 * min4Half(pOut->RValueGrid [ 0],                                                                                  //up   - left
         		                   pOut->RValueGrid [ phandles->XGridNumber -1],                                                          //up   - right
     							   pOut->RValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber)],                               //down - left
     	                           pOut->RValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber) + phandles->XGridNumber -1]);   //down - right

    grGridMaxValue = 8 * min4Half(pOut->GrValueGrid [ 0],                                                                               //up   - left
     		                    pOut->GrValueGrid [ phandles->XGridNumber -1],                                                          //up   - right
    						    pOut->GrValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber)],                               //down - left
                                pOut->GrValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber) + phandles->XGridNumber -1]);   //down - right

    gbridMaxValue =  8 * min4Half(pOut->GbValueGrid [ 0],                                                                              //up   - left
     		                   pOut->GbValueGrid [ phandles->XGridNumber -1],                                                          //up   - right
    						   pOut->GbValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber)],                               //down - left
                               pOut->GbValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber) + phandles->XGridNumber -1]);   //down - right

    bGridMaxValue =  8 * min4Half(pOut->BValueGrid [ 0],                                                                              //up   - left
     		                   pOut->BValueGrid [ phandles->XGridNumber -1],                                                          //up   - right
    						   pOut->BValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber)],                               //down - left
  	                           pOut->BValueGrid [(phandles->YGridNumber -1) * (phandles->XGridNumber) + phandles->XGridNumber -1]);   //down - right

    //printf("Y= %d");
    for (numPaxelY = 1; numPaxelY <= phandles->YGridNumber; numPaxelY++)
    {
        for (numPaxelX = 1; numPaxelX <= phandles->XGridNumber; numPaxelX++)
        {
            R = pOut->RValueGrid[(numPaxelY - 1) * phandles->XGridNumber
                            + numPaxelX - 1];
            Gr = pOut->GrValueGrid[(numPaxelY - 1) * phandles->XGridNumber
                            + numPaxelX - 1];
            Gb = pOut->GbValueGrid[(numPaxelY - 1) * phandles->XGridNumber
                            + numPaxelX - 1];
            B = pOut->BValueGrid[(numPaxelY - 1) * phandles->XGridNumber
                            + numPaxelX - 1];

            //check zero values
            if (R == 0 || Gr == 0 || Gb == 0 || B == 0)
            {
                return CALIB_ER_0_VALUE_EXIST;
            }

            // check some values equal with 1 for each color channel
            if (R == 1 || Gr == 1 || Gb == 1 || B == 1)
            {
                if (R != 1 || Gr != 1 || Gb != 1 || B != 1)
                {
                    return CALIB_ER_ONE_VAL_CONSISTENCY;
                }
            }

            // check maximum value
            if(R > rGridMaxValue || Gr > grGridMaxValue || Gb > gbridMaxValue || B > bGridMaxValue)
            {
                 return CALIB_ER_VAL_EXCEEDS_LIMIT;
            }
        }
    }

    if (checkBiggerValuesInCenter(phandles->XGridNumber, phandles->YGridNumber,
                                  pOut->RValueGrid))
        return CALIB_ER_LARGER_VALUES_CENTER;

    if (checkBiggerValuesInCenter(phandles->XGridNumber, phandles->YGridNumber,
                                  pOut->GrValueGrid))
        return CALIB_ER_LARGER_VALUES_CENTER;

    if (checkBiggerValuesInCenter(phandles->XGridNumber, phandles->YGridNumber,
                                  pOut->GbValueGrid))
        return CALIB_ER_LARGER_VALUES_CENTER;

    if (checkBiggerValuesInCenter(phandles->XGridNumber, phandles->YGridNumber,
                                  pOut->BValueGrid))
        return CALIB_ER_LARGER_VALUES_CENTER;

    return CALIB_SUCCESS;
}
static calibration_error_codes calculate_EEPROM_data(calibrationHandler_t *phandles,calibrationOutput_t *pOut)
{
    u32 i;
    double centerGainR;
    double centerGainGr;
    double centerGainGb;
    double centerGainB;
    double centerChannels[4];
    double paxelDistanceX;
    double paxelDistanceY;
    double overExpWarning;
    double underExpWarning;

    u32 currPaxelStartX;
    u32 currPaxelStartY;

    u32 currPaxelXEnd;
    u32 currPaxelYEnd;

    u32 numPaxelY;
    u32 numPaxelX;
    calibration_error_codes sc;
    pOut->RValueGrid = outBuffRValueGrid;
    pOut->GrValueGrid = outBuffGrValueGrid;
    pOut->GbValueGrid = outBuffGbValueGrid;
    pOut->BValueGrid = outBuffBValueGrid;

    sc = ReadPlainRaw(buffR, buffGr, buffGb, buffB, phandles);
    if(sc){
        return sc;

    }// Subtract data pedestal
    if (phandles->DP != 0)
    {
        for (i = 0; i < (u32)(((phandles->rawheight) / 2) * ((phandles->rawwidth) / 2)); i++){
            if (buffR[i] > phandles->DP)
                buffR[i] -= phandles->DP;
            else
                buffR[i] = 0;

            if (buffGr[i] > phandles->DP)
                buffGr[i] -= phandles->DP;
            else
                buffGr[i] = 0;

            if (buffGb[i] > phandles->DP)
                buffGb[i] -= phandles->DP;
            else
                buffGb[i] = 0;

            if (buffB[i] > phandles->DP)
                buffB[i] -= phandles->DP;
            else
                buffB[i] = 0;
        }
    }

    centerGainR = round(mean2Costum(buffR, phandles));
    centerGainGr = round(mean2Costum(buffGr, phandles));
    centerGainGb = round(mean2Costum(buffGb, phandles));
    centerGainB = round(mean2Costum(buffB, phandles));

    centerChannels[0] = centerGainR;
    centerChannels[1] = centerGainGr;
    centerChannels[2] = centerGainGb;
    centerChannels[3] = centerGainB;

    paxelDistanceX =  ((double) phandles->width - ((double) (phandles->XGridNumber))
                        * ((double) phandles->paxSize)
                                               / 2)
                     / (((double) phandles->XGridNumber) - 1);
    paxelDistanceY =
                     ((double) phandles->height - ((double) (phandles->YGridNumber))
                                                * ((double) phandles->paxSize)
                                                / 2)
                     / (((double) phandles->YGridNumber) - 1);


    for (numPaxelY = 1; numPaxelY <= phandles->YGridNumber; numPaxelY++)
        for (numPaxelX = 1; numPaxelX <= phandles->XGridNumber; numPaxelX++)
        {
            currPaxelStartX =
                            (u32) round(((double) numPaxelX - 1) * ((double) phandles->paxSize) / 2 + ((double) numPaxelX
                                            - 1)
                                                                                                    * paxelDistanceX);
            currPaxelStartY =
                            (u32) round(((double) numPaxelY - 1) * ((double) phandles->paxSize) / 2 + ((double) numPaxelY
                                            - 1)
                                                                                                    * paxelDistanceY);
            if (currPaxelStartX == 0)
                currPaxelStartX = 1;
            if (currPaxelStartY == 0)
                currPaxelStartY = 1;

            currPaxelXEnd = (u32) (currPaxelStartX + phandles->paxSize / 2);
            currPaxelYEnd = (u32) (currPaxelStartY + phandles->paxSize / 2);

            pOut->RValueGrid[(numPaxelY - 1) * phandles->XGridNumber + numPaxelX - 1] =
                            round(mean2MoviCostum(buffR, currPaxelStartY - 1,
                                                  currPaxelYEnd - 1,
                                                  currPaxelStartX - 1,
                                                  currPaxelXEnd - 1,
                                                  phandles->width));
            pOut->GrValueGrid[(numPaxelY - 1) * phandles->XGridNumber + numPaxelX - 1] =
                            round(mean2MoviCostum(buffGr, currPaxelStartY - 1,
                                                  currPaxelYEnd - 1,
                                                  currPaxelStartX - 1,
                                                  currPaxelXEnd - 1,
                                                  phandles->width));
            pOut->GbValueGrid[(numPaxelY - 1) * phandles->XGridNumber + numPaxelX - 1] =
                            round(mean2MoviCostum(buffGb, currPaxelStartY - 1,
                                                  currPaxelYEnd - 1,
                                                  currPaxelStartX - 1,
                                                  currPaxelXEnd - 1,
                                                  phandles->width));
            pOut->BValueGrid[(numPaxelY - 1) * phandles->XGridNumber + numPaxelX - 1] =
                            round(mean2MoviCostum(buffB, currPaxelStartY - 1,
                                                  currPaxelYEnd - 1,
                                                  currPaxelStartX - 1,
                                                  currPaxelXEnd - 1,
                                                  phandles->width));
        }

    // End Calculate AWB and LSC ratios
    // Check for over and underexposure of the raw image



    //sanity check
    sc = sanityCheck(phandles, pOut);

    if (sc)
        return sc;


    // Saving output
    Save_EEPROM_data(phandles, pOut, eepromBuff);

    // Check for over and underexposure of the raw image
    overExpWarning = CheckOverexposure(centerChannels, phandles);
          if (overExpWarning != 0)
              return CALIB_OVEREXPOSURE;

          underExpWarning = CheckUnderexposure(centerChannels, phandles);
          if (underExpWarning != 0)
              return CALIB_UNDEREXPOSURE;

    return sc;
}

calibration_error_codes eepromGenerate(u8 *pInputImg, u32 *pSizeEEPROM, u8 **pbuffEEPROM)
{
    s32 sc;
    sc = initHandler(pInputImg, &handles);
    if(sc)
       return sc;

    *pSizeEEPROM = EEPROM_BYTES_SIZE;
    *pbuffEEPROM = eepromBuff;

    sc = calculate_EEPROM_data(&handles,&out);


    return sc;
}
