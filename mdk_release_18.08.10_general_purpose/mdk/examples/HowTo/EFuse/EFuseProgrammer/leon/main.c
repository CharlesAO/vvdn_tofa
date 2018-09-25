///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Basic eFuse programmer example
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <DrvCpr.h>
#include <DrvTimer.h>
#include <DrvEfuse.h>
#include <DrvGpio.h>
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define EXECUTION_GUARD

// efuse_vddq enable pin, active high
// This must be changed to the GPIO if Vddq is to be controlled via GPIO
#define EFUSE_VDDQ_GPIO_EN              0

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void efuseVddqSetState(u32 enable)
{
    u32 prevEnabled = DrvGpioGetRaw(EFUSE_VDDQ_GPIO_EN);
    DrvGpioSetPin(EFUSE_VDDQ_GPIO_EN, enable);
    // Ensure efuse_vddq is stabilized before any further operations
    if(!prevEnabled && enable)
        DrvTimerSleepMicro(1000);
}

int main(void)
{
    efuseMaskSet set;
    u64 ifcfg;
    int res;

    // These are dummy keys and will result in an unusable MA2155
    // if Secure Boot is enabled with these keys.
    u8 __attribute__((unused)) aes_key[EFUSE_SIZEOF_AES_KEY] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }; // avoid 'unused variable' error from scan build
    u8 __attribute__((unused)) ecc_pubkey[EFUSE_SIZEOF_ECC_PUBKEY] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }; // avoid 'unused variable' error from scan build
    // AES and ECC keys are only relevant on MA2155

    initClocksAndMemory();

    printf("eFuse programmer example\n\n");

#ifdef EXECUTION_GUARD
    printf("By default this example will place an MA2155 into an unusable state.\n");
    printf("Therefore, full execution of the example code is disabled by default.\n\n");
    printf("In order to use this example, modify the values to be programmed\n");
    printf("and comment out the definition of EXECUTION_GUARD before re-running.\n");
    return 0;
#endif

    // This example programs the EFuses with the values that are used
    // for GPIO Boot mode 0x2, which is a USBD boot mode.
    // Additionally, Secure Boot is enabled and (dummy) keys are programmed.

    ifcfg = EFUSE_IFCFG_USBD_FSEL(0x31) | EFUSE_IFCFG_USBD_CLKSEL_EXT;

    DrvEfuseClearMaskSet(&set);
    // as IFCFG is a 64-bit value, the DrvEfuseSetValueMask64() function is used here
    DrvEfuseSetValueMask64(&set, EFUSE_BOOTIFCFG_START, EFUSE_BOOTIFCFG_END, ifcfg);
    DrvEfuseSetValueMask32(&set, EFUSE_BOOTPLLCFG_START, EFUSE_BOOTPLLCFG_END, EFUSE_PLLCFG_RANGE_B);
    DrvEfuseSetValueMask32(&set, EFUSE_BOOTIFSEL_START, EFUSE_BOOTIFSEL_END, EFUSE_IFSEL_USBD);

    // The following fields are for MA2155 only:
    DrvEfuseSetValueMask32(&set, EFUSE_SECURITY_CODE_START, EFUSE_SECURITY_CODE_END, EFUSE_SECURITY_SECURE);
    // As the AES key is byte-reversed, the DrvEfuseSetValueMaskPtrRev() must be used here.
    DrvEfuseSetValueMaskPtrRev(&set, EFUSE_AES_KEY_START, EFUSE_AES_KEY_END, aes_key);
    // The ECC public-key is not byte-reversed, so DrvEfuseSetValueMaskPtr() is used.
    DrvEfuseSetValueMaskPtr(&set, EFUSE_ECC_PUBKEY_START, EFUSE_ECC_PUBKEY_END, ecc_pubkey);

    // The EFuse driver functions controls internal power.
    // However, external power to Vddq must be switched on before attempting to program any EFuses.
    efuseVddqSetState(1);

    // The DrvEfuseProgramWithMask() function will attempt to program
    // EFuses that have had their mask set by the DrvEfuseSetValueMaskXXX() calls above.
    // Note that any EFuses that already have the desired value will be untouched.
    // Use of the DRVEFUSE_FLAG_REDUNDANCY flag means that available redundancy EFuses
    // will be used if necessary.
    res = DrvEfuseProgramWithMask(&set, DRVEFUSE_FLAG_REDUNDANCY);
    if(res != 0) {
        printf("DrvEfuseProgramWithMask - error %d\n", res);
    } else {
        printf("EFuses successfully programmed and verified successfully\n");
        // As, in this example, we are enabling Secure Boot, it is best to disable
        // any unused redundancy EFuses in order to disallow any changes to the EFuse
        // contents.
        // This is not a hard requirement, and should not be done if any EFuses will
        // later need to be programmed.
        res = DrvEfuseLockRedundancy(DRVEFUSE_LOCK_TOKEN);
        if(res != 0) {
            printf("DrvEfuseLockRedundancy - error %d\n", res);
        } else {
            printf("EFuses successfully locked\n");
        }
    }

    // The EFuse driver will turn off internal power, but it is also best to
    // turn off external power to Vddq after programming EFuses.
    efuseVddqSetState(0);

    return 0;
}
