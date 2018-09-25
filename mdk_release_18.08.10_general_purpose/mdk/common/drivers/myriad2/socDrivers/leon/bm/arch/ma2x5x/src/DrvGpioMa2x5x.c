///
/// @file DrvGpioMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvGpioMa2x5x
/// @{
/// @brief     GPIO Functions used for the MA2x5x myriad version.
///
/// This is the implementation of  the GPIO functions that are specific to the MA2x5x myriad version
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "DrvGpio.h"
#include "DrvGpioMa2x5x.h"
#include "DrvGpioArch.h"
#include "assert.h"
#include <stdio.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define D_GPIO_PAD_LOCALDATA_HI_HW   (1 << 5) // Replacement value for D_GPIO_PAD_LOCALDATA_HI
#define D_GPIO_PAD_PULL_CFG_BITMASK  (3 << 5) // Same as D_GPIO_PAD_BUS_KEEPER (all bits set)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void DrvGpioInitialiseRange(const drvGpioInitArrayType initArray)
{
    s32 index = 0, i;
    u8 padsNeedUpdate = FALSE;

    /// First check if any register needs a pad config update
    do
    {
        assert((initArray[index].action & ~(ACTION_UPDATE_PIN |
               ACTION_UPDATE_PAD |
               ACTION_UPDATE_LEVEL |
               ACTION_TERMINATE_ARRAY)) == 0); // Invalid action probably means array wasn't terminated

        if (initArray[index].action & ACTION_UPDATE_PAD)
            padsNeedUpdate = TRUE;
        index++;
    } while ((initArray[index].action & ACTION_TERMINATE_ARRAY) == 0);

    if (padsNeedUpdate)
    {
        s32 valGpioOut;

        for (i = GPIO_NUM_PINS - 1; i >= -1; i--)
        {
            /// Optionally patch the values here
            for (index = 0; (initArray[index].action & ACTION_TERMINATE_ARRAY) == 0; index++)
            {
                if (initArray[index].action & ACTION_UPDATE_PAD)
                {
                    // If i==-1 then this test should fail.
                    if ((i >= initArray[index].startGpio) && (i <= initArray[index].endGpio))
                    {
                        valGpioOut = initArray[index].padConfig;
			// Write back the these register after optionally patching them
		        DrvGpioPadSet(i, valGpioOut);
                    }
                }
            }
        }
    }

    index = 0;
    do
    {
        if (initArray[index].action & ACTION_UPDATE_LEVEL)
            for (i = initArray[index].startGpio; i <= initArray[index].endGpio;
                            i++)
            {
                if (initArray[index].pinLevel == PIN_LEVEL_LOW)
                    DrvGpioSetPinLo(i);
                else
                    DrvGpioSetPinHi(i);
            }
        if (initArray[index].action & ACTION_UPDATE_PIN)
            DrvGpioModeRange(initArray[index].startGpio,
                             initArray[index].endGpio,
                             initArray[index].pinConfig);
        index++;
    } while ((initArray[index].action & ACTION_TERMINATE_ARRAY) == 0);

}

void DrvGpioSetMode(u32 gpioNum, u32 val)
{
   assert(gpioNum < GPIO_NUM_PINS);

   u32 GPIOregVal;

   GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNum);

   GPIOregVal &= ~(0x07F); // keep the rest of the config
   GPIOregVal |= val;

   SET_REG_WORD(GPIO_MODE0_ADR + 4*gpioNum, GPIOregVal);
}

void DrvGpioMode(u32 gpioNum, u32 val)
{
    assert(gpioNum < GPIO_NUM_PINS);

    u32 GPIOregVal;

   GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNum);

   GPIOregVal &= ~(0x07F); // keep the rest of the config
   GPIOregVal |= val;

   SET_REG_WORD(GPIO_MODE0_ADR + 4*gpioNum, GPIOregVal);
}

void DrvGpioModeRange(u32 gpioNum1, u32 gpioNum2, u32 val)
{
    u32 i;
    u32 GPIOregVal;

    assert(gpioNum1 < GPIO_NUM_PINS);
    assert(gpioNum2 < GPIO_NUM_PINS);

    for (i = gpioNum1; i <= gpioNum2; ++i)
    {
	GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*i);

   	GPIOregVal &= ~(0x07F); // keep the rest of the config
   	GPIOregVal |= val;

   	SET_REG_WORD(GPIO_MODE0_ADR + 4*i, GPIOregVal);
    }
}

void DrvGpioModeListVal(u32 *pList, u32 size, u32 val)
{
    u32 i;

    for (i = 0; i <= size; ++i)
    {
	assert((*pList) < GPIO_NUM_PINS);
	DrvGpioSetMode((*pList), val);
        ++pList;
    }
}

void DrvGpioModeListList(u32 *pList, u32 size, u32 *val)
{
    u32 i;

    for (i = 0; i <= size; ++i)
    {
	assert((*pList) < GPIO_NUM_PINS);
	DrvGpioSetMode((*pList), (*val) & D_GPIO_MODE_MASK);
        ++pList;
        ++val;
    }
}

void GpioPadSetAllTable(u32 table[])
{
    u32 i;

    for (i = 0; i < GPIO_NUM_PINS; ++i)
    {
	DrvGpioPadSet(i, table[GPIO_NUM_PINS - i - 1] & 0xFFF);
    }
}

void DrvGpioPadSetAllTable(u32 table[])
{
    u32 i;

    for (i = 0; i < GPIO_NUM_PINS; ++i)
    {
	DrvGpioPadSet(i, table[GPIO_NUM_PINS - i - 1] & 0xFFF);
    }
}

void GpioPadGetAllTable(u32 table[])
{
    u32 i;

    for (i = 0; i < GPIO_NUM_PINS; ++i)
    {
	table[GPIO_NUM_PINS - i - 1] = DrvGpioPadGet(i);
    }
}

void GpioPadSet(u32 gpioNo, u32 val)
{
    DrvGpioPadSet(gpioNo, val);
}

void DrvGpioPadSet(u32 gpioNo, u32 val)
{
  u32 GPIOregVal;

  assert(gpioNo < GPIO_NUM_PINS);

  if (val & D_GPIO_PAD_LOCALCTRL_UPDATE)
  {
      // If the pin is configured as output
      if (!(val & D_GPIO_PAD_LOCAL_PIN_IN))
      {
          // Clear Pad Pull config, and write the correct bit mask
          val &= ~D_GPIO_PAD_PULL_CFG_BITMASK;
          if (val & D_GPIO_PAD_LOCALDATA_HI)
              val |= D_GPIO_PAD_LOCALDATA_HI_HW;
      }
      // GPIO Pad config bits [11:8], needed bits from val are already shifted by 5
      SET_REG_WORD(GPIO_PAD_CFG_CTRL, ((val << 3) & 0xF00) | gpioNo);
  }
  else
  {
      GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNo);

      GPIOregVal &= ~(0x3F80);  // reset bits [13:7] of the GPIO_MODE reg
      GPIOregVal |= (val << 7); // GPIO PAD config bits are GPIO_MODE[13:7]

      SET_REG_WORD(GPIO_MODE0_ADR + 4*gpioNo, GPIOregVal);
  }
}

void GpioPadSetFast(u32 gpioNo, u32 val)
{
    assert(gpioNo < GPIO_NUM_PINS);

    DrvGpioPadSet(gpioNo, val);
}

void DrvGpioPadSetFast(u32 gpioNo, u32 val)
{
    assert(gpioNo < GPIO_NUM_PINS);

    DrvGpioPadSet(gpioNo, val);
}

void GpioPadSetFastMask(u32 gpioNo, u32 maskClear, u32 maskSet)
{
   u32 padCfg;

   assert(gpioNo < GPIO_NUM_PINS);

   padCfg = DrvGpioPadGet(gpioNo);
   DrvGpioPadSet(gpioNo, (padCfg & (~maskClear)) | maskSet);
}

void DrvGpioPadSetFastMask(u32 gpioNo, u32 maskClear, u32 maskSet)
{
   u32 padCfg;

   assert(gpioNo < GPIO_NUM_PINS);

   padCfg = DrvGpioPadGet(gpioNo);
   DrvGpioPadSet(gpioNo, (padCfg & (~maskClear)) | maskSet);
}

u32 GpioPadGet(u32 gpioNo)
{
  u32 GPIOregVal;

  assert(gpioNo < GPIO_NUM_PINS);

  GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNo);

  GPIOregVal &= (0x3F80);  // extract bits [13:7] of the GPIO_MODE reg
  GPIOregVal >>= 7;        // GPIO PAD config bits are GPIO_MODE[13:7]

  return GPIOregVal;
}

u32 DrvGpioPadGet(u32 gpioNo)
{
  u32 GPIOregVal;

  assert(gpioNo < GPIO_NUM_PINS);

  GPIOregVal = GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNo);

  GPIOregVal &= (0x3F80);  // extract bits [13:7] of the GPIO_MODE reg
  GPIOregVal >>= 7;        // GPIO PAD config bits are GPIO_MODE[13:7]

  return GPIOregVal;
}

void GpioPadSetRangeVal(u32 gpioStart, u32 gpioEnd, u32 val)
{
    u32 i;

    assert(gpioStart < GPIO_NUM_PINS);
    assert(gpioEnd < GPIO_NUM_PINS);

    if (gpioStart > gpioEnd)
    {
        /// Exit if the range is invalid
        return;
    }

    for(i = gpioStart; i<= gpioEnd; i++)
    {
	DrvGpioPadSet(i, val);
    }
}

void DrvGpioPadSetRangeVal(u32 gpioStart, u32 gpioEnd, u32 val)
{
    u32 i;

    assert(gpioStart < GPIO_NUM_PINS);
    assert(gpioEnd < GPIO_NUM_PINS);

    if (gpioStart > gpioEnd)
    {
        /// Exit if the range is invalid
        return;
    }

    for(i = gpioStart; i<= gpioEnd; i++)
    {
	DrvGpioPadSet(i, val);
    }
}

///@}
