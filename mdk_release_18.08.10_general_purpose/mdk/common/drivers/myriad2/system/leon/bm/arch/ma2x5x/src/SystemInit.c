#include <DrvRegUtils.h>
#include <stdio.h>

#define CMX_TARGET_READ_MARGIN       ( 0x3 )                                // Read Margin value of 0x3 is optimum
#define CMX_CUT_READ_MARGIN          ( (1 << 4) | CMX_TARGET_READ_MARGIN )  // Bit 4 Enables config of 0x3
#define CMX_RAM_READ_MARGIN_CONFIG   ( (CMX_CUT_READ_MARGIN << 24) | \
                                       (CMX_CUT_READ_MARGIN << 16) | \
                                       (CMX_CUT_READ_MARGIN <<  8) | \
                                       (CMX_CUT_READ_MARGIN <<  0) )

// This function adjusts the CMX Read Margin timing parameters to their optimum
// tuned values for voltage and temperature. It does not have any impact on the
// read/write performance of the CMX memory. These parameters effect sub-cycle timing
// performance and as such don't have any effect on the runtime of code using CMX.
static void adjustCmxRamTimingsOnStartup(void) {
    int slice;

    for (slice=0;slice < CFG_NUM_SLICES;slice++)
    {
        SET_REG_WORD(CMX_RAM_CTRL_SL_0 + (sizeof(uint32_t) * slice), CMX_RAM_READ_MARGIN_CONFIG);
    }
}

void SystemInit(void) {
    adjustCmxRamTimingsOnStartup();
}

