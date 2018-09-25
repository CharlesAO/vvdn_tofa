///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file: oPipe: 5 concurrent pipes test
///           (passthrough modes)

// 1: Includes
// ----------------------------------------------------------------------------
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include "app_config.h"
#include "Opipe.h"
#include "DrvLeon.h"
#include "DrvLeonL2C.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
//Main full buffers in DDR
static uint16_t  iBuf1[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint16_t  iBuf2[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static uint16_t  oBuf1[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint16_t  oBuf2[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static uint8_t   iBuf3[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint8_t   oBuf3[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static uint8_t   iBuf4[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint8_t   oBuf4[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static uint8_t   iBuf5[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);
static uint8_t   oBuf5[IMG_W*IMG_H] SECTION(".cmx.data") ALIGNED(16);

static Opipe    p1,p2,p3,p4,p5;


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void buildAndCfgP1(Opipe *p, uint16_t *iBuf, uint16_t *oBuf);
void buildAndCfgP2(Opipe *p, uint16_t *iBuf, uint16_t *oBuf);
void buildAndCfgP3(Opipe *p, uint8_t  *iBuf, uint8_t  *oBuf);
void buildAndCfgP4(Opipe *p, uint8_t  *iBuf, uint8_t  *oBuf);
void buildAndCfgP5(Opipe *p, uint8_t  *iBuf, uint8_t  *oBuf);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int main()
{
    DrvLL2CInitWriteThrough();
    swcLeonSetPIL(2); //Interrupts get enabled here, by lowering the priority
    unitTestInit();

    //Read test images
    loadMemFromFileSimple("../../../../resources/lowLight_512x128.raw",   IMG_W*IMG_H*2, iBuf1);
    loadMemFromFileSimple("../../../../resources/lowLight2_512x128.raw",   IMG_W*IMG_H*2, iBuf2);
    loadMemFromFileSimple("../../../../resources/lowLightU8a_512x128.raw", IMG_W*IMG_H,   iBuf3);
    loadMemFromFileSimple("../../../../resources/lowLightU8b_512x128.raw", IMG_W*IMG_H,   iBuf4);
    loadMemFromFileSimple("../../../../resources/lowLightU8c_512x128.raw", IMG_W*IMG_H,   iBuf5);

    OpipeReset();   //general inits

    buildAndCfgP1(&p1, iBuf1, oBuf1);
    buildAndCfgP2(&p2, iBuf2, oBuf2);
    buildAndCfgP3(&p3, iBuf3, oBuf3);
    buildAndCfgP4(&p4, iBuf4, oBuf4);
    buildAndCfgP5(&p5, iBuf5, oBuf5);

    OpipeStart(&p1);
    OpipeStart(&p2);
    OpipeStart(&p3);
    OpipeStart(&p4);
    OpipeStart(&p5);

    OpipeWait(&p1);
    OpipeWait(&p2);
    OpipeWait(&p3);
    OpipeWait(&p4);
    OpipeWait(&p5);

    unitTestMemCompare((void*)iBuf1, (void*)oBuf1, sizeof(oBuf1));
    unitTestMemCompare((void*)iBuf2, (void*)oBuf2, sizeof(oBuf2));
    unitTestMemCompare((void*)iBuf3, (void*)oBuf3, sizeof(oBuf3));
    unitTestMemCompare((void*)iBuf4, (void*)oBuf4, sizeof(oBuf4));
    unitTestMemCompare((void*)iBuf5, (void*)oBuf5, sizeof(oBuf5));

    unitTestFinalReport();

    DrvLeonRTSignalStop();
    return 0;
}
