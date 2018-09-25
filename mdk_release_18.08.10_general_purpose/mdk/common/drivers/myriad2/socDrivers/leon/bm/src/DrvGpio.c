///
/// @file DrvGpio.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvGpio
/// @{
/// @brief     GPIO driver Implementation
///
/// Low level driver controlling the functions of the Myriad 2
/// General Purpose IO Block
///


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "DrvGpioArch.h"
#include "DrvGpio.h"
#include "assert.h"
#include <stdio.h>
#include "swcWhoAmI.h"
#include "HglGpioTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
/// This variable should be removed in future. It used on the deprecated DrvGpioIrqResetAll function to avoid
/// reseting the interrupt sources more than once (at startup time). User should use DrvGpioIrqSrcDisable(u32 irqSrc)
/// instead
static u32 gpioIrqReset = 0;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void DrvGpioIrqResetAll()
{
    u32 irqSrc;
    /// Make sure no GPIO generates GPIO interrupts
    if (gpioIrqReset)
    {
        return;
    }

    for (irqSrc = 0; irqSrc < GPIO_MAX_IRQ_SRCS; irqSrc++)
        SET_REG_WORD(GPIO_INT0_CFG_ADR + 4*irqSrc, 0);

    gpioIrqReset++;
}

s32 DrvGpioIrqDisable(u32 gpioNum, u32 irqSrc)
{
    s32 ret = -1;
    u32 newVal, regVal;

    /// Check for valid GPIO
    assert(gpioNum < GPIO_NUM_PINS);

    regVal = newVal = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);

    /// there are 4 pins muxed in one irq
    for (u32 mask = 0xFF; mask; mask <<= 8, regVal >>= 8) {
        /// 8bit per pin, test, shift 8, re-test
        if ((regVal & D_GPIO_INT_EN_0) &&
            ((regVal & D_GPIO_INT_GPIO_MASK_0) == gpioNum))
        {
            ret = 0;
            newVal &= ~mask;
            break;
        }
     }

    /// if there are any changes to be made, apply new value
    if (!ret)
    {
        /// Disable the interrupt and reset gpioNum
        SET_REG_WORD(GPIO_INT0_CFG_ADR + 4 * irqSrc, newVal);
    }

    return ret;
}

s32 DrvGpioIrqDisableAny(u32 gpioNum)
{
    s32 ret = -1;
    u32 irqSrc, newVal, regVal;

    /// Check for valid GPIO
    assert(gpioNum < GPIO_NUM_PINS);

    for (irqSrc = 0; irqSrc < GPIO_MAX_IRQ_SRCS; irqSrc++)
    {
        regVal = newVal = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4 * irqSrc);

        /// there are 4 pins muxed in one irq
        for (u32 mask = 0xFF; mask; mask <<= 8, regVal >>= 8) {
            // 8bit per pin, test, shift 8, re-test
            if ((regVal & D_GPIO_INT_EN_0) &&
                ((regVal & D_GPIO_INT_GPIO_MASK_0) == gpioNum))
            {
                ret = 0;
                newVal &= ~mask;
            }
        }
        /// if there are any changes to be made, apply them
        if (!ret)
        {
            /// Disable the interrupt and reset gpioNum
            SET_REG_WORD(GPIO_INT0_CFG_ADR + 4 * irqSrc, newVal);
        }
    }

    return ret;
}

void DrvGpioIrqSrcDisable(u32 irqSrc)
{
    assert(irqSrc < GPIO_MAX_IRQ_SRCS);

    /// Disable the IRQ source
    SET_REG_WORD(GPIO_INT0_CFG_ADR + 4*irqSrc, 0);
}

s32 DrvGpioAvailableAnyIrqSrc()
{
    u32 irqSrc, tmp;

    for (irqSrc = 0; irqSrc < GPIO_MAX_IRQ_SRCS; irqSrc++)
    {
        tmp = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);
        /// Check if any GPIO (out of 4 available) is available (not enabled) to trigger interrupt on that source
        if (!(tmp & D_GPIO_INT_EN_0) || !(tmp & D_GPIO_INT_EN_1) ||
            !(tmp & D_GPIO_INT_EN_2) || !(tmp & D_GPIO_INT_EN_3))
        {
            return (s32) irqSrc;
        }
    }

    /// All IRQ Sources are taken
    return -1;
}

u8 DrvGpioIrqSrcNotUsed(u32 irqSrc)
{
    u32 tmp;

    tmp = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);
    /// Check if none of the GPIO (out of 4 available) used
    if (!(tmp & D_GPIO_INT_EN_0) && !(tmp & D_GPIO_INT_EN_1)
        && !(tmp & D_GPIO_INT_EN_2) && !(tmp & D_GPIO_INT_EN_3))
    {
        return 1;
    }

    /// At least one IRQ Source is taken.
    return 0;
}

u8 DrvGpioAvailableIrqSrc(u32 irqSrc)
{
    u32 tmp;

    tmp = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);
    /// Check if any GPIO (out of 4 available) is available (not enabled) to trigger interrupt on that source
    if (!(tmp & D_GPIO_INT_EN_0) || !(tmp & D_GPIO_INT_EN_1)
        || !(tmp & D_GPIO_INT_EN_2)
        || !(tmp & D_GPIO_INT_EN_3))
    {
        /// Success, available
        return 1;
    }

    /// All IRQ Sources are taken
    return 0;
}

s32 DrvGpioIrqJustConfigAnyGpio(u32 gpioNum, u32 irqSrc)
{
    u32 tmp = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);

    /// GPIO 0 not enabled?
    if ((tmp & D_GPIO_INT_EN_0) == 0)
    {
        tmp |= D_GPIO_INT_GPIO_MASK_0 & (gpioNum << 0);
        tmp |= D_GPIO_INT_EN_0;
    }
    /// GPIO 1 not enabled?
    else if ((tmp & D_GPIO_INT_EN_1) == 0)
    {
        tmp |= D_GPIO_INT_GPIO_MASK_1 & (gpioNum << 8);
        tmp |= D_GPIO_INT_EN_1;
    }
    /// GPIO 2 not enabled?
    else if ((tmp & D_GPIO_INT_EN_2) == 0)
    {
        tmp |= D_GPIO_INT_GPIO_MASK_2 & (gpioNum << 16);
        tmp |= D_GPIO_INT_EN_2;
    }
    /// GPIO 3 not enabled?
    else if ((tmp & D_GPIO_INT_EN_3) == 0)
    {
        tmp |= D_GPIO_INT_GPIO_MASK_3 & (gpioNum << 24);
        tmp |= D_GPIO_INT_EN_3;
    }
    /// All GPIOs taken? To avoid this, call u8 DrvGpioIrqSrcNotUsed(u32 irqSrc) before
    else
    {
        return -1;
    }

    /// If GPIO available, enable it to trigger interrupts
    DrvGpioSetMode(gpioNum, D_GPIO_MODE_7 | D_GPIO_DIR_IN);
    SET_REG_WORD(GPIO_INT0_CFG_ADR + 4*irqSrc, tmp);

    /// Success
    return 0;
}

s32 DrvGpioIrqConfigAnyGpio(u32 gpioNum, u32 irqSrc, u32 irqType, u32 priority,
                            irq_handler callback)
{
    s32 rc = 0;

    rc = DrvGpioIrqJustConfigAnyGpio(gpioNum, irqSrc);
    if (rc)
        return rc;

    DrvIcbSetupIrq( IRQ_GPIO_0 + irqSrc, priority, irqType, callback);

    /// Success
    return rc;
}

void DrvGpioIrqConfig(u32 gpio0, u32 gpio1, u32 gpio2, u32 gpio3, u32 irqSrc,
                      u32 irqType, u32 priority, irq_handler callback)
{
    u32 tmp;

    assert(irqSrc < GPIO_MAX_IRQ_SRCS);

    /// If gpio irq reset wasn't done
    if (!gpioIrqReset)
        /// By default all gpios will trigger on all irqSrc, need to reset them
        DrvGpioIrqResetAll();

    /// By reading first the value of INT CFG REG and ORing it with new values, we avoid clearing the previous config
    tmp = GET_REG_WORD_VAL(GPIO_INT0_CFG_ADR + 4*irqSrc);

    if (gpio0 < GPIO_NUM_PINS)
    {
        tmp |= D_GPIO_INT_EN_0 | (D_GPIO_INT_GPIO_MASK_0 & (gpio0 << 0));
        DrvGpioSetMode(gpio0, D_GPIO_MODE_7 | D_GPIO_DIR_IN);
    }
    if (gpio1 < GPIO_NUM_PINS)
    {
        tmp |= D_GPIO_INT_EN_1 | (D_GPIO_INT_GPIO_MASK_1 & (gpio1 << 8));
        DrvGpioSetMode(gpio1, D_GPIO_MODE_7 | D_GPIO_DIR_IN);
    }
    if (gpio2 < GPIO_NUM_PINS)
    {
        tmp |= D_GPIO_INT_EN_2 | (D_GPIO_INT_GPIO_MASK_2 & (gpio2 << 16));
        DrvGpioSetMode(gpio2, D_GPIO_MODE_7 | D_GPIO_DIR_IN);
    }
    if (gpio3 < GPIO_NUM_PINS)
    {
        tmp |= D_GPIO_INT_EN_3 | (D_GPIO_INT_GPIO_MASK_3 & (gpio3 << 24));
        DrvGpioSetMode(gpio3, D_GPIO_MODE_7 | D_GPIO_DIR_IN);
    }

    SET_REG_WORD(GPIO_INT0_CFG_ADR + 4*irqSrc, tmp);
    DrvIcbSetupIrq( IRQ_GPIO_0 + irqSrc, priority, irqType, callback);
}

void DrvGpioSetPin(u32 gpioNum, u32 value)
{
    assert(gpioNum < GPIO_NUM_PINS);
    assert(value < 2);

    /// First, configure the mode 7 register value for pin, so no glitch is introduced
    if (value == 0)
        DrvGpioSetPinLo(gpioNum);
    else
        DrvGpioSetPinHi(gpioNum);

    /// Always ensure that GPIO is enabled as output
    DrvGpioSetMode(gpioNum, D_GPIO_DIR_OUT | D_GPIO_MODE_7);
}

u32 DrvGpioGetPin(u32 gpioNum)
{
    assert(gpioNum < GPIO_NUM_PINS);

    /// Always ensure that GPIO is enabled as input
    DrvGpioSetMode(gpioNum, D_GPIO_DIR_IN | D_GPIO_MODE_7);

    return DrvGpioGet(gpioNum);
}

void DrvGpioToggleState(u32 gpioNum)
{
    u32 value, regNo, pinNo;
    regNo = gpioNum / 32;
    pinNo = gpioNum - regNo*32;

    assert(gpioNum < GPIO_NUM_PINS);

    /// Get the current output state of the pin
    value = GET_REG_WORD_VAL(GPIO_DATA_OUT0_ADR + 4*regNo) & (1 << pinNo);

    /// First, configure the mode 7 register value for pin, so no glitch is introduced
    /// Drive out the opposite state
    if (value==0)
        DrvGpioSetPinHi(gpioNum);
    else
        DrvGpioSetPinLo(gpioNum);

    /// Always ensure that GPIO is enabled as output
    DrvGpioSetMode(gpioNum, D_GPIO_DIR_OUT | D_GPIO_MODE_7);
}

void DrvGpioSetPinHi(u32 gpioNum)
{
    u32 regNo, pinNo;
    regNo = gpioNum / 32;
    pinNo = gpioNum - regNo * 32;

    assert(gpioNum < GPIO_NUM_PINS);

    SET_REG_WORD(GPIO_DATA_HIGH0_ADR + 4*regNo, (1 << pinNo));
}

void DrvGpioSetPinLo(u32 gpioNum)
{
    u32 regNo, pinNo;
    regNo = gpioNum / 32;
    pinNo = gpioNum - regNo * 32;

    assert(gpioNum < GPIO_NUM_PINS);

    SET_REG_WORD(GPIO_DATA_LOW0_ADR + 4*regNo, (1 << pinNo));
}

u32 DrvGpioGetMode(u32 gpioNum)
{
    assert(gpioNum < GPIO_NUM_PINS);

    return GET_REG_WORD_VAL(GPIO_MODE0_ADR + 4*gpioNum);
}

void DrvGpioSetPwm0(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN0_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW0_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm1(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN1_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW1_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm2(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN2_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW2_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm3(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN3_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW3_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm4(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN4_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW4_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm5(u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    SET_REG_WORD(GPIO_PWM_LEADIN5_ADR,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW5_ADR,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioSetPwm(u32 pwmBlock, u32 repeat, u32 leadIn, u32 hiCnt, u32 lowCnt)
{
    assert(pwmBlock < GPIO_PWM_BLOCKS);

    SET_REG_WORD(GPIO_PWM_LEADIN0_ADR + 4*pwmBlock,
                 (repeat & D_GPIO_PWM_LEADIN_REPEAT_MASK) | ((leadIn << 16) & D_GPIO_PWM_LEADIN_LEADIN_MASK));
    SET_REG_WORD(GPIO_PWM_HIGHLOW0_ADR + 4*pwmBlock,
                 ((hiCnt<<16) & D_GPIO_PWM_HIGHLOW_HIGH_MASK) | (lowCnt & D_GPIO_PWM_HIGHLOW_LOW_MASK));
}

void DrvGpioEnPwm0(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN0_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN0_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN0_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN0_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm1(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN1_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN1_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN1_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN1_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm2(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN2_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN2_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN2_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN2_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm3(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN3_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN3_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN3_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN3_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm4(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN4_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN4_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN4_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN4_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm5(u32 en)
{
    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN5_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN5_ADR) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN5_ADR,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN5_ADR) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

void DrvGpioEnPwm(u32 pwmBlock, u32 en)
{
    assert(pwmBlock < GPIO_PWM_BLOCKS);

    if (en)
    {
        SET_REG_WORD(GPIO_PWM_LEADIN0_ADR + 4*pwmBlock,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN0_ADR + 4*pwmBlock) | D_GPIO_PWM_LEADIN_ENABLE_MASK);
    }
    else
    {
        SET_REG_WORD(GPIO_PWM_LEADIN0_ADR + 4*pwmBlock,
                     GET_REG_WORD_VAL(GPIO_PWM_LEADIN0_ADR + 4*pwmBlock) & (~D_GPIO_PWM_LEADIN_ENABLE_MASK));
    }
}

u32 DrvGpioGet(u32 gpioNum)
{
    u32 regNo, pinNo, pin;

    assert(gpioNum < GPIO_NUM_PINS);

    regNo = gpioNum / 32;
    pinNo = gpioNum - regNo * 32;
    pin = GET_REG_WORD_VAL(GPIO_DATA_IN0_ADR + 4*regNo);

    return (pin >> pinNo) & 1;
}

u32 DrvGpioGetRaw(u32 gpioNum)
{
    u32 regNo, pinNo, pin;

    assert(gpioNum < GPIO_NUM_PINS);

    regNo = gpioNum / 32;
    pinNo = gpioNum - regNo * 32;
    pin = GET_REG_WORD_VAL(GPIO_DATA_IN_RAW0_ADR + 4*regNo);

    return (pin >> pinNo) & 1;
}

void GpioPadSetArrayVal(u32 gpioArray[], u32 size, u32 val)
{
    u32 i;

    for (i = 0; i < size; ++i)
    {
        assert(gpioArray[i] < GPIO_NUM_PINS);
        DrvGpioPadSetFast(gpioArray[i], val);
    }
}

void DrvGpioPadSetArrayVal(u32 gpioArray[], u32 size, u32 val)
{
    u32 i;

    for (i = 0; i < size; ++i)
    {
        assert(gpioArray[i] < GPIO_NUM_PINS);
        DrvGpioPadSetFast(gpioArray[i], val);
    }
}

void GpioPadSetArrayArray(u32 gpioArray[], u32 size, u32 valArray[])
{
    u32 i;

    for (i = 0; i < size; ++i)
    {
        assert(gpioArray[i] < GPIO_NUM_PINS);
        DrvGpioPadSetFast(gpioArray[i], valArray[i]);
    }
}

void DrvGpioPadSetArrayArray(u32 gpioArray[], u32 size, u32 valArray[])
{
    u32 i;

    for (i = 0; i < size; ++i)
    {
        assert(gpioArray[i] < GPIO_NUM_PINS);
        DrvGpioPadSetFast(gpioArray[i], valArray[i]);
    }
}

s32 DrvGpioTimerSetConfig(const u32 timestamp_num,
                          const DrvGpioTriggerType trigger)
{
    // Ensure that we are running on the Leon OS.
    if (swcWhoAmI() != PROCESS_LEON_OS) {
        return -1;
    }
    return HglGpioTimerSetConfig(timestamp_num, trigger);
}

s32 DrvGpioTimerGet(const u32 timestamp_num, u64 *const timestamp)
{
    // Ensure that we are running on the Leon OS.
    if (swcWhoAmI() != PROCESS_LEON_OS) {
        return -1;
    }
    return HglGpioTimerGet(timestamp_num, timestamp);
}

#pragma GCC diagnostic pop

///@}
