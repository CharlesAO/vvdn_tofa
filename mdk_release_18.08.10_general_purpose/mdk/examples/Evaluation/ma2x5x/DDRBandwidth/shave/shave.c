///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     CMX -> DDR benchmarks
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <svuCommonShave.h>
#include <swcCdma.h>
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
volatile u32 ticks;
dmaTransactionList_t task;
u8   __attribute__((aligned(64))) cmx_buff[64 * 1024];
u32  __attribute__((aligned(64))) transactions;

extern u32 **tail;
// 5: Static Function Prototypes
void *memcpy2_asm( void *dest, const void *src, size_t n );
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

// -----------------------------------------
//  Public functions to be called from LEON
// -----------------------------------------

// benchmark memcpy2_asm
void __attribute__((section(".text.entrypoint"))) __attribute__((aligned(64)))
svuMemcpyAsm(u8 *dst, u8 *src, u32 size)
{
  ticks = swcReadPC0();        // start profiling
  memcpy2_asm(dst, src, size);
  ticks = (swcReadPC0()-ticks);  // stop profiling

  SHAVE_HALT;
}

// benchmark DMA moving source to destination
void __attribute__((section(".text.entrypoint"))) __attribute__((aligned(64)))
svuMemcpyDMA(u8 *dst, u8* src, u32 size)
{
    dmaTransactionList_t *ref;
    register int i;
    u32 id;
    // Initialize tasks handler
    id = dmaInitRequester(1);
    // Next we create a 1d DMA transaction with a custom burst length of 8.
    // Burst size must be 8 because AXI bus consolidation FIFO is 8 level deep.
    // Any burst greater than 8 will cause stalls on the AXI buss.
    // Any burst  lower  than 8 will cause auto-consolidation and will be less efficient,
    // although more efficient than without automatic consolidation.
    // Please be aware source and destination strides are ignored since it is a 1d transaction
    ticks=swcReadPC0();          // start profiling
    for( i=0;i<1000;i++){
        ref= dmaCreateTransactionExt(DMA_1D_TRANSACTION, id, &task, (u8*) src, (u8*) dst, size, 1, 1, 1, 1, 8);
        dmaStartListTask(ref);
        dmaWaitTask(ref);
    }
    ticks = (swcReadPC0()-ticks)/1000;  // stop profiling

    SHAVE_HALT;
}

// benchmark all shaves moving with DMA (this is only one shave)
void __attribute__((section(".text.entrypoint"))) __attribute__((aligned(64)))
svuSystemTestDMA(u8 *dst, u8* src, u32 size, u32 repeatCount, volatile u32 *stopAllShaves)
{
    dmaTransactionList_t *ref;
    register u32 i;
    u32 id;
    ticks=0;
    cmx_buff[0]=0;
    // Initialize tasks handler
    id = dmaInitRequester(0);
    // Next we create a 1d DMA transaction with a custom burst length of 8.
    // Burst size must be 8 because AXI bus consolidation FIFO is 8 level deep.
    // Any burst greater than 8 will cause stalls on the AXI buss.
    // Any burst  lower  than 8 will cause auto-consolidation and will be less efficient,
    // although more efficient than without automatic consolidation.
    // Please be aware source and destination strides are ignored since it is a 1d transaction
    ticks=swcReadPC0();          // start profiling
    for( i=0;i<repeatCount;i++){
        ref= dmaCreateTransactionExt(DMA_1D_TRANSACTION, id, &task, (u8*) src, (u8*) dst, size, 1, 1, 1, 1, 8);
        dmaStartListTask(ref);
        dmaWaitTask(ref);
        transactions++;
        if(*stopAllShaves){
            break;
        }
    }
    *stopAllShaves=1;
    ticks = (swcReadPC0()-ticks);  // stop profiling

    SHAVE_HALT;
}
