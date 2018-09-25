///
/// @file DrvDdrMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvDdrMa2x5x
/// @{
/// @brief     TODO_ADD_ONE_LINE_TEST_DESCRIPTION_HERE
///

// ----------------------------------------------------------------------------

#include <mv_types.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <DrvTimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <DrvCpr.h>
#include <DrvDdr.h>

// Functions Implementations
// ----------------------------------------------------------------------------

int DrvDdrIsInitialized(void)
{
    tyHwPlatform platform = DrvRegGetHardwarePlatform();
    if ((platform == PLATFORM_MOVISIM) ||
        (platform == PLATFORM_FPGA))
    {
        return 1; // Only Init DDR if not running in moviSim/FPGA as they don't support DDRC
    }

    // First ensure that the AON clock is present before we go talking to power islands
    DrvCprSysDeviceAction(CSS_DOMAIN,ENABLE_CLKS,DEV_CSS_AON);

    if (DrvCprPowerGetStatus() & (1 << POWER_ISLAND_DSS))
    {
        if ((GET_REG_WORD_VAL(CPR_AUX_CLK_RST_ADR) & (1 << CSS_AUX_DDR_CORE_CTRL)) != 0)
        {
            // If power island is on and the core reset is deasserted, then we very likely don't need to reinit the DDR.
            return 1;
        }
    }
    return 0;
}

u64 DrvDdrMrReadRaw(u32 reg);

u32 DrvDdrMrRead(u32 reg)
{
    // Assumes that byte lane 1 is the true byte lane 0 from the die's perspective
    return (DrvDdrMrReadRaw(reg) >> 8) & 0xff;
}

u32 DrvDdrGetMr8DensityGigaBit()
{
    u32 density = (DrvDdrMrRead(8) >> 2) & 0xf;
    switch (density)
    {
        case 4:
            return 1;
        case 6:
            return 4;
        case 7:
            return 8;
        case 8:
            return 16;
        case 9:
            return 32;
        case 0xd:
            return 12;
        case 0xe:
            return 6;
        default:
            assert(0);
    }
    return 0; // never reached.
}

///@}
