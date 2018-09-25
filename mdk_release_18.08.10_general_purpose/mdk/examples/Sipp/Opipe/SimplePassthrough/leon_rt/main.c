/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    main leon file: oPipe-SIGMA only test (passthrough)
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include "Opipe.h"
#include "app_config.h"
#include "DrvLeon.h"
#include "DrvLeonL2C.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
//Main full buffers in DDR
static uint16_t iBuf [IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint16_t oBuf[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static Opipe p2;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void buildAndCfg(Opipe *p, uint16_t *iBuf, uint16_t *oBuf);



// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main()
{
    DrvLL2CInitWriteThrough();
    swcLeonSetPIL(2); //Interrupts get enabled here, by lowering the priority
    unitTestInit();

    loadMemFromFileSimple("../../../../resources/lowLight_512x128.raw", IMG_W*IMG_H*2, iBuf);
    OpipeReset();   //general inits

    buildAndCfg(&p2, iBuf, oBuf); //app
    OpipeStart(&p2);
    OpipeWait(&p2);

    unitTestMemCompare((void*)iBuf, (void*)oBuf, sizeof(oBuf));
    unitTestFinalReport();

    DrvLeonRTSignalStop();
    return 0;
}
