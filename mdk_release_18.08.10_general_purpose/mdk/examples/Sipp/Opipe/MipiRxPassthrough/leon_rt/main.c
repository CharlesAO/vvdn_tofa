///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file : 10bit MipiRx (loopback) passthrough (MULTIFRAME)
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include "Opipe.h"
#include "OpipeApps.h"
#include "app_config.h"
#include "DrvLeon.h"
#include "DrvLeonL2C.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define N_FRAMES 2

//Circular OUT CMX buffer
#define I_CBUFF_H (3*IRQ_RATE)
#define O_CBUFF_H (2*IRQ_RATE)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------

//Main full buffers in DDR
static uint16_t   iBuf          [IMG_W * IMG_H] SECTION(".cmx.data") ALIGNED(16); //10bit
static uint16_t   oBuf[N_FRAMES][IMG_W * IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint16_t   oCircBuffCmx[O_CBUFF_H * IMG_W] SECTION(".cmx.data") ALIGNED(16);

static volatile uint32_t    frameNo;   //cur frame no
static OpipeRx              pRx;
static oMipiTxLoopbackParam mipiTxCfg; //test drive


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void EofCallback(Opipe *p)
{
    UNUSED(p);

    printInt(frameNo);

    frameNo++;
    if(frameNo < N_FRAMES)
    {
        pRx.pOut->ddr.base = (uint32_t)oBuf[frameNo]; //next out buffers
        OpipeStart(&pRx.p); //restart...
        oStartMipiTxLoopback(SIPP_MIPI_TX0_ID);
    }
}


int main()
{
    DrvLL2CInitWriteThrough();
    swcLeonSetPIL(2); //Interrupts get enabled here, by lowering the priority
    unitTestInit();

    loadMemFromFileSimple("../../../../resources/lowLight_512x128.raw", IMG_W * IMG_H * 2, iBuf);

    OpipeReset();   //general inits

    OpipeCreateRx(&pRx, IMG_W, IMG_H,
            SIPP_MIPI_RX1_ID, BPP(2),
            (uint32_t)oCircBuffCmx, O_CBUFF_H);
    pRx.p.cbEndOfFrame = EofCallback;

    appSetParams(&pRx.p);
    pRx.pOut->ddr.base = (uint32_t)oBuf[0]; //initial output buffer and params
    OpipeStart   (&pRx.p);

    //Mipi loopback config and start:
    defaultMipiTxLoopParams(&mipiTxCfg, SIPP_MIPI_TX0_ID, (uint32_t)iBuf, BPP(2), IMG_W, IMG_H);
    oCfgMipiTxLoopback     (&mipiTxCfg);

    oStartMipiTxLoopback   (SIPP_MIPI_TX0_ID);

    //Wait for all frames to complete
    while(frameNo < N_FRAMES){
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP;NOP;NOP;
    }

    //Checks
    for(frameNo=0; frameNo < N_FRAMES; frameNo++)
    {
        saveMemoryToFile((uint32_t)oBuf[frameNo], sizeof(oBuf), "out.raw");
        unitTestMemCompare((void*) oBuf[frameNo], (void*)iBuf, sizeof(iBuf));
    }
    unitTestFinalReport();

    DrvLeonRTSignalStop();

    return 0;
}
