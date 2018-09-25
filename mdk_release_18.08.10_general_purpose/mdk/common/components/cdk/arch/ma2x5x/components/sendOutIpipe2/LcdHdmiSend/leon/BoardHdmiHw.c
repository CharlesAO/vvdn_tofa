
/**************************************************************************************************

 @File         : lcdHdmiSend.c
 @Author       : MT
 @Brief        : Allow display frames over HDmi, 1080p30 interface.
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : � Movidius Srl 2014, � Movidius Ltd 2015

 Description :


 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "mv_types.h"
#include "IcTypes.h"
#include "DrvI2cMaster.h"
#include "DrvRegUtilsDefines.h"

//Board specific include for MV0212
#ifdef MV0212
#include "MV0212.h"
#include "DrvADV7513.h"
#endif

//Board specific include for MV0182
#ifdef MV0182
#include "Board182Api.h"
#include "DrvADV7513.h"
#endif

#include "BoardHdmiHw.h"

/**************************************************************************************************
 ~~~  Specific #defines
**************************************************************************************************/

//Throw an error for undefined board type
#if (!(defined(MV0182) || defined(MV0212)) && defined(OUTPUT_UNIT_IS_HDMI))
#error HDMI is supported only on mv182 or mv212
#endif

/**************************************************************************************************
 ~~~  Global Data (Only if absolutely necessary)
**************************************************************************************************/

/**************************************************************************************************
 ~~~  Local variables
**************************************************************************************************/

//HDMI Chip config related variables for standard boards.
#if ((defined(MV0182) || defined(MV0212)))
static ADV7513ContfigMode_t    ADV7513Config   = ADV7513_1080P60; // HDMI chip
static I2CM_Device hdmiI2cHandle;

//I2C specific information for MV182
#if (defined(MV0182))
static tyI2cConfig i2cHdmiConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0182_I2C2_SCL_PIN,
     .sdaPin                = MV0182_I2C2_SDA_PIN,
     .speedKhz              = MV0182_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0182_I2C2_ADDR_SIZE_DEFAULT,
    };
#endif

//I2C specific information for MV212
#if (defined(MV0212))
static tyI2cConfig i2cHdmiConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0212_I2C2_SCL_PIN,
     .sdaPin                = MV0212_I2C2_SDA_PIN,
     .speedKhz              = MV0212_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0212_I2C2_ADDR_SIZE_DEFAULT,
    };
#endif

#endif

/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/

int boardHdmiHwInit(void)
{
    int ret;

//MV182 board specific initialization
//TODO: Could be generalized and merged with MV212
#ifdef MV0182
    //Initialize I2C handle to program HDMI
    ret = DrvI2cMInitFromConfig(&hdmiI2cHandle, &i2cHdmiConfiguration);
    if (ret != I2CM_STAT_OK)
    {
        printf("Could not initialize I2C handle for HDMI\n");
        return -1;
    }
#endif

//MV212 board specific initialization
//TODO: Could be generalized and merged with MV182
#ifdef MV0212
    //Initialize I2C handle to program HDMI
    ret = DrvI2cMInitFromConfig(&hdmiI2cHandle, &i2cHdmiConfiguration);
    if (ret != I2CM_STAT_OK)
    {
        printf("Could not initialize I2C handle for HDMI\n");
        return -1;
    }
#endif

    return 0;
}

//
int boardHdmiHwOpen(void) {
    uint8_t status;

#ifdef MV0182
    status = initADV7513reg(&hdmiI2cHandle, ADV7513Config);
    if (status != 0){
        printf("ADV7513 Configuration failed %d \n", status);
        return -1;
    }
#endif

#ifdef MV0212
    status = initADV7513reg(&hdmiI2cHandle, ADV7513Config);
    if (status != 0){
        printf("ADV7513 Configuration failed %d \n", status);
        return -1;
    }
#endif

    return 0;
}

int boardHdmiHwDeinit(void) {

#ifdef MV0182
    //Nothing to do.
#endif

#ifdef MV0212
    //Nothing to do.
#endif


    return 0;
}
