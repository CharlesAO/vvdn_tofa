///
/// @file DrvRegUtils.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvRegUtils
/// @{
/// @brief     Register Manipulation functions
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvRegUtils.h"
#include "assert.h"
#include "DrvTimer.h"


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data 
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void DrvRegBitMaskOperation(void * regAddress, tyRegMaskOperation operation, u32 value)
{
    u32 orgValue;

    switch (operation)
    {
    case SET_WORD:
        SET_REG_WORD(regAddress,value);
        break;
    case SET_BITS:
        orgValue = GET_REG_WORD_VAL(regAddress);
        orgValue |= value;
        SET_REG_WORD(regAddress,orgValue);
        break;
    case CLR_BITS:
        orgValue = GET_REG_WORD_VAL(regAddress);
        orgValue &= ~value;
        SET_REG_WORD(regAddress,orgValue);
        break; 
    }
    return;
}

void DrvRegSetBitField(void * regAddress, u32 startBit, u32 numBits, u32 value)
{
    u32 initialValue;
    u32 mask;
    initialValue = GET_REG_WORD_VAL(regAddress);
    mask = GEN_BIT_MASK(numBits);
    // Ensure that the value isn't larger than the number of bits requested
    value &= mask;  
    mask = mask << startBit;
    // Clear the relevant bits in the initial value
    initialValue = initialValue & (~mask);
    // now OR in the new value
    initialValue = initialValue | (value << startBit);
    SET_REG_WORD(regAddress,initialValue);
    return;
}

u32 DrvRegGetBitField(void * regAddress, u32 startBit, u32 numBits)
{
    u32 initialValue;
    u32 mask;
    initialValue = GET_REG_WORD_VAL(regAddress);

    // Shift down to the bits of interest
    initialValue = initialValue >> startBit;
    mask = GEN_BIT_MASK(numBits); // Mask for the bits of interest

    return (initialValue & mask);
}

int DrvRegWaitForBitsSetAny(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs)
{
    u64 timeStamp;
    u32 timeoutReached;

    DrvTimerInitTimeStamp(&timeStamp,timeOutMicroSecs);
    while ((GET_REG_WORD_VAL(registerAddress) & bitSetMask) == 0)
    {
        DrvTimerCheckTimeStamp(&timeStamp, &timeoutReached);
        if (timeoutReached)
            return -1; // Signal the timeout
    }
    return 0;
}

int DrvRegWaitForBitsSetAll(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs)
{
    u64 timeStamp;
    u32 timeoutReached;

    DrvTimerInitTimeStamp(&timeStamp,timeOutMicroSecs);
    while ((GET_REG_WORD_VAL(registerAddress) & bitSetMask) != bitSetMask)
    {
        DrvTimerCheckTimeStamp(&timeStamp, &timeoutReached);
        if (timeoutReached)
            return -1; // Signal the timeout
    }
    return 0;
}

int DrvRegWaitForBitsClearAll(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs)
{
    u64 timeStamp;
    u32 timeoutReached;

    DrvTimerInitTimeStamp(&timeStamp,timeOutMicroSecs);
    while ( (GET_REG_WORD_VAL(registerAddress) & bitSetMask))
    {
        DrvTimerCheckTimeStamp(&timeStamp, &timeoutReached);
        if (timeoutReached)
            return -1; // Signal the timeout
    }
    return 0;
}


///@}
