///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IC Driver for the HDMI ADV7513 chip.
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvADV7513.h"
#include "DrvADV7513Configs.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define ADV7513_SLAVEADDRESS 0x39

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 protocolWriteSample[] = I2C_PROTO_WRITE_8BA;
static u8 protocolReadSample[] = I2C_PROTO_READ_8BA;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static u8 readADV7513 (I2CM_Device * dev, u8 regAddr,u8 * dataBuffer, u32 dataBufferSize){
    u32 stat;
    stat = DrvI2cMTransaction(dev, ADV7513_SLAVEADDRESS, regAddr, protocolReadSample, (u8 *)(dataBuffer), dataBufferSize);
    if (stat != I2CM_STAT_OK)
         return FALSE;
    return TRUE;
}

static u8 writeADV7513 (I2CM_Device * dev, u8 regAddr,u8 * dataBuffer, u32 dataBufferSize){
    u32 stat;
       stat = DrvI2cMTransaction(dev, ADV7513_SLAVEADDRESS, regAddr, protocolWriteSample, (u8 *)(dataBuffer), dataBufferSize);
       if (stat != I2CM_STAT_OK)
            return FALSE;
       return TRUE;
}

u8 initADV7513reg(I2CM_Device * dev, ADV7513ContfigMode_t cfg){
    u8 i;
    // 0= success
    u8 rv = 0;
    u8 value;

    adv7513ElementType *extraAdv513Regs = NULL;
    u32 extraAdv513RegsSize = 0;
    if (cfg == ADV7513_720P30)
    {
        extraAdv513Regs = (adv7513ElementType *) &extra720Adv513Regs[0];
        extraAdv513RegsSize = sizeof(extra720Adv513Regs) / sizeof(adv7513ElementType);
    }
    else if (cfg == ADV7513_1080P60)
    {
        extraAdv513Regs = (adv7513ElementType *) &extra1080Adv513Regs[0];
        extraAdv513RegsSize = sizeof(extra1080Adv513Regs) / sizeof(adv7513ElementType);
    }
    else
    {
        return 1;
    }

    for (i = 0;(i < sizeof(initAdv513Regs) / sizeof(adv7513ElementType))/*&&
                (status !=FALSE)*/;i++)
    {
        //printf("w regs: addr 0x%x\n", initAdv513Regs[i].address);
        if(writeADV7513(dev, initAdv513Regs[i].address,&initAdv513Regs[i].value,1)==FALSE)
        {
            return 2;
        }
    }

    for (i = 0; (i < sizeof(initAdv513Regs) / sizeof(adv7513ElementType))/*&&
     (status !=FALSE)*/; i++)
    {
        value = 0;
        if (readADV7513(dev, initAdv513Regs[i].address, &value, 1) == TRUE)
        {
            //printf("r regs: addr 0x%x\n", initAdv513Regs[i].address);
            if (initAdv513Regs[i].checkBAckWrittenVal == 1)
            {
                if (initAdv513Regs[i].value != value)
                {

                    //printf("addr 0x%x val 0x%x expected 0x%x\n",
                    //       initAdv513Regs[i].address & 0xff,
                    //       value & 0xff, initAdv513Regs[i].value & 0xff);
                    return 3;

                }
            }
        }
    }

    for (i = 0; i < extraAdv513RegsSize; i++)
        {
            //printf("w extra regs: addr 0x%x\n", extraAdv513Regs[i].address);
            if(writeADV7513(dev, extraAdv513Regs[i].address, &extraAdv513Regs[i].value,1) == FALSE)
            {
                return 4;
            }
        }

    for (i = 0; i < extraAdv513RegsSize; i++)
    {
        value = 0;
        if (readADV7513(dev, extraAdv513Regs[i].address, &value, 1) == TRUE)
        {
            //printf("r extra regs: addr 0x%x\n", extraAdv513Regs[i].address);
            if (extraAdv513Regs[i].checkBAckWrittenVal == 1)
            {
                if (extraAdv513Regs[i].value != value)
                {

                    //printf("extra regs: addr 0x%x val 0x%x expected 0x%x\n",
                    //       extraAdv513Regs[i].address & 0xff,
                    //       value & 0xff, extraAdv513Regs[i].value & 0xff);
                    return 5;

                }
            }
        }
    }

    return rv;
}
