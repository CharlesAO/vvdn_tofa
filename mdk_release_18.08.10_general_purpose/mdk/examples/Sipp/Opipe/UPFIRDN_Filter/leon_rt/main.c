/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file ;7x7 Scale = 2/1
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
static uint8_t   iBuf[IMG_W  *IMG_H  ] SECTION(".cmx.data") ALIGNED(16);
static uint8_t   oBuf[IMG_W*2*IMG_H*2] SECTION(".cmx.data") ALIGNED(16);
static uint8_t   eBuf[IMG_W*2*IMG_H*2] SECTION(".cmx.data") ALIGNED(16);

static Opipe p;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main()
{
    DrvLL2CInitWriteThrough();
    swcLeonSetPIL(2); //Interrupts get enabled here, by lowering the priority
    unitTestInit();

    //Read test image
    loadMemFromFileSimple("../../../../resources/inpU8_256x64.raw",  IMG_W*IMG_H,   iBuf);
    loadMemFromFileSimple("../../../../resources/expU8_512x128.raw", IMG_W*IMG_H*4, eBuf);
    OpipeReset();   //general inits

    BuildAndCfgPipe(&p, iBuf, oBuf); //app
    OpipeStart   (&p);
    OpipeWait    (&p);

    saveMemoryToFile((uint32_t)oBuf, sizeof(oBuf), "out.bin");
    unitTestMemCompare((void*)oBuf, (void*)eBuf, sizeof(oBuf));
    unitTestFinalReport();

    DrvLeonRTSignalStop();
    return 0;
}
