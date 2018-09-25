///
/// @file HglTempSensor.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglTempSensor
/// @{
/// @brief     HGL API implementation for the Temperature Sensor
///


/// 1: Includes
/// ----------------------------------------------------------------------------
#include "HglTempSensor.h"
#include "DrvRegUtils.h"
#include "HglCommon.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define MODE_OFFSET   4
#define IRQ_EN_OFFSET 8

#define MS_HALF_32 0x0000FFFF
#define LS_HALF_32 0xFFFF0000

#define TSENS_CSS_MODE  (MODE_OFFSET + TSENS_CSS )
#define TSENS_MSS_MODE  (MODE_OFFSET + TSENS_MSS )
#define TSENS_UPA0_MODE (MODE_OFFSET + TSENS_UPA0)
#define TSENS_UPA1_MODE (MODE_OFFSET + TSENS_UPA1)

#define TSENS_CSS_IRQ   (IRQ_EN_OFFSET + TSENS_CSS )
#define TSENS_MSS_IRQ   (IRQ_EN_OFFSET + TSENS_MSS )
#define TSENS_UPA0_IRQ  (IRQ_EN_OFFSET + TSENS_UPA0)
#define TSENS_UPA1_IRQ  (IRQ_EN_OFFSET + TSENS_UPA1)

#define AON_TSENS_CFG_SM_EN         0x8000
#define AON_TSENS_CFG_AUTO_EN_MASK (0xF << 4) // Automatic Mode for all sensors

#define TSENS_CSS_VSAMPLE_OFFS   5
#define TSENS_MSS_VSAMPLE_OFFS  13
#define TSENS_UPA0_VSAMPLE_OFFS 21
#define TSENS_UPA1_VSAMPLE_OFFS 29

// for now use same trim for all 4 sensors
#define AON_TSENS_CFG_CSS_TRIM_VAL      13
#define AON_TSENS_CFG_MSS_TRIM_VAL      13
#define AON_TSENS_CFG_UPA1_TRIM_VAL     13
#define AON_TSENS_CFG_UPA2_TRIM_VAL     13

#define TEMPSENS_VALID_DATA_OFFS 15

// Conversion constants
#define CONST0 (1157.2)
#define CONST1 (1865.8)

// how many times will we spin lock for valid temp sensor reading
#define MAX_LOOPS         99999

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static inline uint32_t hglTempSensorGetDataReg(HglTempSensor sensor)
{
    if (sensor == TSENS_CSS ||
        sensor == TSENS_MSS)
        return AON_TSENS_DATA0;
    else
        return AON_TSENS_DATA1;
}

static inline uint32_t hglTempSensorGetIrqReg(HglTempSensor sensor)
{
    if (sensor == TSENS_CSS ||
        sensor == TSENS_MSS)
    {
        return AON_TSENS_INT0_ADR;
    }
    else
        return AON_TSENS_INT1_ADR;
}

float HglTempSensorConvertRawtoTemp(float N)
{
   return
  (
           (-1.6743E-11)*N*N*N*N +
           (+8.1542E-08)*N*N*N   +
           (-1.8201E-04)*N*N     +
           (+3.1020E-01)*N       +
           (-4.8380E+01)
   );
}

float HglTempSensorConvertTempToRaw(float T)
{
    return
    (
        (1.8322E-08)*T*T*T*T +
        (2.3430E-05)*T*T*T   +
        (8.7018E-03)*T*T     +
        (3.9269E+00)*T       +
        (1.7204E+02)
    );
}

float HglTempSensorConvertRawToVolt(float N)
{
    return ((N + CONST0) / CONST1);
}

float HglTempSensorConvertVoltToRaw(float V)
{
    return (CONST1*V - CONST0);
}

float HglTempSensorGetActualVal(HglTempSensor sensorToRead, HglTempRawToAct convertFunc)
{
    uint16_t sampleData = 0;
    float actualVal = 0.0f;
    float Nread = 0.0f;

    sampleData = HglTempSensorGetData(sensorToRead);

    if (sampleData & (1<<TEMPSENS_VALID_DATA_OFFS))
    {
        Nread = (float)(sampleData & DRV_TSENS_SAMPLE_MASK);
        actualVal = convertFunc(Nread);
    }
    else
    {
        actualVal = TEMPSENS_INVALID;
    }

    return actualVal;
}

void HglTempSensorInit(void)
{
     // for now use same trim for all 4 sensors
    uint32_t trimRegVal = AON_TSENS_CFG_UPA2_TRIM_VAL << 24 |
                          AON_TSENS_CFG_UPA1_TRIM_VAL << 16 |
                          AON_TSENS_CFG_MSS_TRIM_VAL  <<  8 |
                          AON_TSENS_CFG_CSS_TRIM_VAL;

    uint32_t cfgRegVal = 0x0;  // reset all the sensors here

    SET_REG_WORD(AON_TSENS_TRIM_CFG_ADR,  trimRegVal);
    SET_REG_WORD(AON_TSENS_CFG_ADR,       cfgRegVal);
    SET_REG_WORD(AON_TSENS_DATA0, 0x0);
    SET_REG_WORD(AON_TSENS_DATA1, 0x0);
}

static int32_t hglTempSensorEnable(HglTempSensor sensorToEnable, HglTempSensorMode mode)
{

    switch(sensorToEnable)
    {
     case TSENS_CSS:
         SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_CSS);
         break;
     case TSENS_MSS:
         SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_MSS);
         break;
     case TSENS_UPA0:
         SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA0);
         break;
     case TSENS_UPA1:
         SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA1);
         break;
     default:
         return HGL_ERROR;
    }

    if (mode == TSENS_ONE_SHOT_MODE)
    {
       switch(sensorToEnable)
       {
        case TSENS_CSS:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_CSS_MODE);
            break;
        case TSENS_MSS:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_MSS_MODE);
            break;
        case TSENS_UPA0:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA0_MODE);
            break;
        case TSENS_UPA1:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA1_MODE);
            break;
        default:
            return HGL_ERROR;
       }
    }
    else if (mode == TSENS_CONTINUOUS_MODE)
    {
        switch(sensorToEnable)
        {
        case TSENS_CSS:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_CSS_MODE);
            break;
        case TSENS_MSS:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_MSS_MODE);
            break;
        case TSENS_UPA0:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA0_MODE);
            break;
        case TSENS_UPA1:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA1_MODE);
            break;
        default:
            return HGL_ERROR;
       }
       SET_REG_BITS_MASK(AON_TSENS_CFG_ADR, AON_TSENS_CFG_SM_EN);
    }
    return HGL_SUCCESS;
}

static int32_t hglTempSensorDisable(HglTempSensor sensorToDisable)
{
    uint32_t dataReg = hglTempSensorGetDataReg(sensorToDisable);

    switch(sensorToDisable)
    {
     case TSENS_CSS:
         CLR_REG_BITS_MASK(AON_TSENS_CFG_ADR, (1<<TSENS_CSS) | (1<<TSENS_CSS_MODE));
         break;
     case TSENS_MSS:
         CLR_REG_BITS_MASK(AON_TSENS_CFG_ADR, (1<<TSENS_MSS) | (1<<TSENS_MSS_MODE));
         break;
     case TSENS_UPA0:
         CLR_REG_BITS_MASK(AON_TSENS_CFG_ADR, (1<<TSENS_UPA0) | (1<<TSENS_UPA0_MODE));
         break;
     case TSENS_UPA1:
         CLR_REG_BITS_MASK(AON_TSENS_CFG_ADR, (1<<TSENS_UPA1) | (1<<TSENS_UPA1_MODE));
         break;
     default:
         return HGL_ERROR;
    }

    // clear valid bit
    if (sensorToDisable%2)
        CLR_REG_BITS_MASK(dataReg, LS_HALF_32);
    else
        CLR_REG_BITS_MASK(dataReg, MS_HALF_32);

    uint32_t checkAutoMode = GET_REG_WORD_VAL(AON_TSENS_CFG_ADR);
    if(!(checkAutoMode & AON_TSENS_CFG_AUTO_EN_MASK))
        CLR_REG_BITS_MASK(AON_TSENS_CFG_ADR, AON_TSENS_CFG_SM_EN);

    return HGL_SUCCESS;
}

int32_t HglTempSensorSetMode(HglTempSensor sensor, HglTempSensorMode mode)
{
    int32_t status = HGL_SUCCESS;

    switch(mode)
    {
        case TSENS_ONE_SHOT_MODE:
        case TSENS_CONTINUOUS_MODE:
        {
            status = hglTempSensorDisable(sensor);
            if (status)
                goto SET_MODE_ERR;

            status = hglTempSensorEnable(sensor, mode);
            if (status)
                goto SET_MODE_ERR;
            break;
        }
        case TSENS_DISABLE:
        {
            status = hglTempSensorDisable(sensor);
            if (status)
                goto SET_MODE_ERR;
            break;
        }
        default:
            status = HGL_ERROR;
            goto SET_MODE_ERR;
            break;
    }
    SET_MODE_ERR:
        return status;
}

int32_t HglTempSensorSetSampleType(HglTempSensor sensor, HglTempSensorSampleType sampleType)
{
    switch(sensor)
    {
    case TSENS_CSS:
        CLR_REG_BIT_NUM(AON_TSENS_TRIM_CFG_ADR, TSENS_CSS_VSAMPLE_OFFS);
        SET_REG_BITS_MASK(AON_TSENS_TRIM_CFG_ADR, sampleType << TSENS_CSS_VSAMPLE_OFFS);
        break;
    case TSENS_MSS:
        CLR_REG_BIT_NUM(AON_TSENS_TRIM_CFG_ADR, TSENS_MSS_VSAMPLE_OFFS);
        SET_REG_BITS_MASK(AON_TSENS_TRIM_CFG_ADR, sampleType << TSENS_MSS_VSAMPLE_OFFS);
        break;
    case TSENS_UPA0:
        CLR_REG_BIT_NUM(AON_TSENS_TRIM_CFG_ADR, TSENS_UPA0_VSAMPLE_OFFS);
        SET_REG_BITS_MASK(AON_TSENS_TRIM_CFG_ADR, sampleType << TSENS_UPA0_VSAMPLE_OFFS);
        break;
    case TSENS_UPA1:
        CLR_REG_BIT_NUM(AON_TSENS_TRIM_CFG_ADR, TSENS_UPA1_VSAMPLE_OFFS);
        SET_REG_BITS_MASK(AON_TSENS_TRIM_CFG_ADR, sampleType << TSENS_UPA1_VSAMPLE_OFFS);
        break;
    default:
        return HGL_ERROR;

    }
    return HGL_SUCCESS;
}

//looped reading for continuous mode, with double validation (bug 24116)
uint16_t HglTempSensorGetData(HglTempSensor sensorToRead)
{
    uint32_t loopCnt = 0;
    uint32_t dataReg = hglTempSensorGetDataReg(sensorToRead);
    int32_t  valid = 0, prevValid;
    volatile uint32_t regVal = 0;
    volatile uint16_t *sampleData = (uint16_t*)&regVal;

    if ((sensorToRead == TSENS_MSS) || (sensorToRead == TSENS_UPA1))
        sampleData = ((uint16_t*)&regVal) + 1;    //read the second halfword of the shared sensor word

    while(1)
    {
        regVal = GET_REG_WORD_VAL(dataReg);
        prevValid = valid;
        valid = (*sampleData) & (1<<TEMPSENS_VALID_DATA_OFFS);
        if (prevValid && valid)  //two subsequent valid samples were read
            break;

        if (loopCnt++ == MAX_LOOPS)
            return 0; //the returned data is zero and the flag is invalid
    }

    return *sampleData;
}

//single reading to be used by the one shoot mode ISR
uint16_t HglTempSensorSingleDataRead(HglTempSensor sensorToRead)
{
    uint32_t dataReg = hglTempSensorGetDataReg(sensorToRead);
    int32_t  valid = 0;
    volatile uint32_t regVal = 0;
    volatile uint16_t *sampleData = (uint16_t*)&regVal;

    if ((sensorToRead == TSENS_MSS) || (sensorToRead == TSENS_UPA1))
        sampleData = ((uint16_t*)&regVal) + 1;   //read the second halfword of the shared sensor word

    regVal = GET_REG_WORD_VAL(dataReg);
    valid = (*sampleData) & (1<<TEMPSENS_VALID_DATA_OFFS);
    if (valid)
        return *sampleData;
    else
        return 0; //the returned data is zero and the flag is invalid
}

void HglTempSensorIrqClr(HglTempSensor sensor)
{
    SET_REG_BIT_NUM(AON_TSENS_IRQ_CLR_ADR, sensor);
}

int32_t HglTempSensorDisableDeviceIrq(HglTempSensor sensor)
{
    switch(sensor)
    {
        case TSENS_CSS:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_CSS_IRQ);
            break;
        case TSENS_MSS:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_MSS_IRQ);
            break;
        case TSENS_UPA0:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA0_IRQ);
            break;
        case TSENS_UPA1:
            CLR_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA1_IRQ);
            break;
        default:
            return HGL_ERROR;;
    }
    return HGL_SUCCESS;
}

int32_t HglTempSensorEnableDeviceIrq(HglTempSensor sensor)
{
    switch(sensor)
    {
        case TSENS_CSS:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_CSS_IRQ);
            break;
        case TSENS_MSS:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_MSS_IRQ);
            break;
        case TSENS_UPA0:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA0_IRQ);
            break;
        case TSENS_UPA1:
            SET_REG_BIT_NUM(AON_TSENS_CFG_ADR, TSENS_UPA1_IRQ);
            break;
        default:
            return HGL_ERROR;
    }
        return HGL_SUCCESS;
}

int32_t HglTempSensorSetIrqThreshold(HglTempSensor sensor, uint32_t levelThresh)
{
    uint32_t intReg = hglTempSensorGetIrqReg(sensor);
    uint32_t rg;

    if (sensor == TSENS_CSS ||
        sensor == TSENS_UPA0)
    {
        // Write to bits 9:0 for the corresponding sensor
        // Read the sensor value first and zero bits 9:0
        rg = (GET_REG_WORD_VAL(intReg) & 0xffff0000);
        UNUSED(rg); // avoid 'unused variable' error from scan build
        // Add the new level threshold value to bits 9:0
        rg |= levelThresh;
        // Write back to register
        SET_REG_WORD(intReg, rg);
    }
    else if (sensor == TSENS_MSS ||
             sensor == TSENS_UPA1)
    {
        // Write to bits 25:16 for the corresponding sensor
        // Read the sensor value and zero bits 25:16
        rg = (GET_REG_WORD_VAL(intReg) & 0x0000ffff);
        UNUSED(rg); // avoid 'unused variable' error from scan build
        // Add the new level threshold value to bits 25:16
        rg |= (levelThresh << 16);
        // Write back to register
        SET_REG_WORD(intReg, rg);
    }

    return HGL_SUCCESS;
}

///@}
