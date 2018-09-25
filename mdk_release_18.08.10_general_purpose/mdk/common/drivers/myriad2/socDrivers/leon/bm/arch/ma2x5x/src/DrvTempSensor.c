///
/// @file DrvTempSensor.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvTempSensor
/// @{
/// @brief     BM Temperature Sensor driver
///


#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "DrvTimer.h"
#include "HglTempSensor.h"
#include "DrvTempSensor.h"
#include "DrvCommon.h"
#include "DrvIcb.h"

#define TEMPSENS_NUM 4
#define TEMPSENS_INVALID_THRESHOLD (0xDEAD)
#define TEMPSENS_DEFAULT_PRIORITY   14

static struct
{
    DrvTempSensorMode        mode;
    DrvTempSensorSampleType  type;
    uint16_t                 threshold;
    DrvTempSensorCb          callback;
}tempSensState[TEMPSENS_NUM];

static struct
{
    uint32_t priority;
    uint32_t initialised;
}tempSensControl;

static void tempSensIrqHandler(uint32_t source)
{
    UNUSED(source);
    uint16_t sample = 0, rawSample = 0, nonThresholdReached = 0;
    
    // There is only one interrupt line for all sensors,
    // therefore we don't know which sensor triggered the interrupt
    // and we have to iterate through all of them
    for (int sensor = 0; sensor < TEMPSENS_NUM; ++sensor)
    {
         if (tempSensState[sensor].callback != NULL && tempSensState[sensor].threshold != TEMPSENS_INVALID_THRESHOLD)
         {
            sample = HglTempSensorSingleDataRead((DrvTempSensor)sensor);  //if invalid data was read, the content is zero, so no threshold will be reached
            rawSample = sample & DRV_TSENS_SAMPLE_MASK;
            if (rawSample >= tempSensState[sensor].threshold)
            {
                tempSensState[sensor].callback((DrvTempSensor)sensor, rawSample);
                DrvTempSensorDisableIrq(sensor);
            }
            else
            {
                nonThresholdReached++;
            }
         }
    }
    if (nonThresholdReached == TEMPSENS_NUM)
    {
        // None of the sensors reach the temperature Threshold,
        // so clear the IRQs manually
    	for (int sensor = 0; sensor < TEMPSENS_NUM; ++sensor)
    	{
    		HglTempSensorIrqClr(sensor);
    	}
    }
}

float DrvTempSensorConvertRawtoTemp(float N)
{
    return HglTempSensorConvertRawtoTemp(N);
}

float DrvTempSensorConvertRawtoVolt(float N)
{
    return HglTempSensorConvertRawToVolt(N);
}

void DrvTempSensorInitialise(DrvTempSensConfig *params)
{
    if (!tempSensControl.initialised)
    {
        HglTempSensorInit();

        DrvIcbSetupIrq(IRQ_AON, params->priority, POS_EDGE_INT, tempSensIrqHandler);
        
        tempSensControl.priority    = params->priority;
        tempSensControl.initialised = TRUE;
    }
}

void DrvTempSensorInit(void)
{
    DrvTempSensConfig initParam = { .priority =  TEMPSENS_DEFAULT_PRIORITY };

    DrvTempSensorInitialise(&initParam);
}

int32_t DrvTempSensorSetMode(DrvTempSensor sensor, DrvTempSensorMode mode, DrvTempSensorSampleType sampleType)
{
    int32_t status = MYR_DRV_SUCCESS;

    if (!tempSensControl.initialised)
    {
        return MYR_DRV_NOT_INITIALIZED;
    }

    status = HglTempSensorSetMode(sensor, mode);
    if (status)
        return MYR_DRV_ERROR;


    status = HglTempSensorSetSampleType(sensor, sampleType);
    if (status)
        return MYR_DRV_ERROR;

    tempSensState[sensor].mode = mode;
    tempSensState[sensor].type = sampleType;

    return status;
}

int32_t DrvTempSensorGetSample(DrvTempSensor sensor, float *value)
{
    int32_t status = MYR_DRV_ERROR;
    
    if (!tempSensControl.initialised)
    {
        return MYR_DRV_NOT_INITIALIZED;
    }

    if (tempSensState[sensor].type == TSENS_SAMPLE_TEMP)
    {
        *value = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawtoTemp);
    }
    else
    {
        *value = HglTempSensorGetActualVal(sensor, HglTempSensorConvertRawToVolt);
    }
    
    if (*value != TEMPSENS_INVALID)
    {
        status = MYR_DRV_SUCCESS;
    }
    return status;
}

int32_t DrvTempSensorEnableIrq(DrvTempSensor sensor, float threshold, DrvTempSensorCb callback)
{
    uint16_t thrLevel = 0;
    int32_t status = MYR_DRV_SUCCESS;

    if (!tempSensControl.initialised)
        return MYR_DRV_NOT_INITIALIZED;

    if (tempSensState[sensor].mode != TSENS_CONTINUOUS_MODE || callback == NULL)
        return MYR_DRV_ERROR;

    if (tempSensState[sensor].type == TSENS_SAMPLE_TEMP)
    {
        thrLevel = (uint16_t)HglTempSensorConvertTempToRaw(threshold);   
    }
    else
    {
        thrLevel = (uint16_t)HglTempSensorConvertVoltToRaw(threshold);
    }
    
    status = HglTempSensorSetIrqThreshold(sensor, thrLevel);
    if (status)
        return MYR_DRV_ERROR;

    DrvIcbDisableIrq(IRQ_AON);
    tempSensState[sensor].callback  = callback;
    tempSensState[sensor].threshold = thrLevel;
    
    status = HglTempSensorEnableDeviceIrq(sensor);
    DrvIcbEnableIrq(IRQ_AON);
    if (status)
        return MYR_DRV_ERROR;


    return MYR_DRV_SUCCESS;
}

int32_t DrvTempSensorDisableIrq(DrvTempSensor sensor)
{
    if (!tempSensControl.initialised)
        return MYR_DRV_NOT_INITIALIZED;

    DrvIcbDisableIrq(IRQ_AON);
    tempSensState[sensor].threshold = TEMPSENS_INVALID_THRESHOLD;
    tempSensState[sensor].callback  = NULL;

    HglTempSensorDisableDeviceIrq(sensor);
    DrvIcbEnableIrq(IRQ_AON);

    HglTempSensorIrqClr(sensor);

    return MYR_DRV_SUCCESS;
}

// for now just get CSS value
int DrvTempSensorGetTemp(float *temperature)
{
    int32_t status = MYR_DRV_SUCCESS;
    
    if (!tempSensControl.initialised)
    {
        return MYR_DRV_NOT_INITIALIZED;
    }

    status = DrvTempSensorSetMode(TSENS_CSS, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    if (status)
        return MYR_DRV_ERROR;

    status = DrvTempSensorGetSample(TSENS_CSS, temperature);
    if (status)
        return MYR_DRV_ERROR;

    return status;
}

int32_t DrvTempSensorGetIndividualTemp(DrvTempSensor sensorToRead, float *temperature)
{
    int32_t status = MYR_DRV_SUCCESS;

    if (!tempSensControl.initialised)
    {
        return MYR_DRV_NOT_INITIALIZED;
    }

    status = DrvTempSensorSetMode(sensorToRead, TSENS_ONE_SHOT_MODE, TSENS_SAMPLE_TEMP);
    if (status)
        return MYR_DRV_ERROR;

    status = DrvTempSensorGetSample(sensorToRead, temperature);
    if (status)
        return MYR_DRV_ERROR;

    return MYR_DRV_SUCCESS;
}

///@}
