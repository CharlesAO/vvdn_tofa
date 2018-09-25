///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <swcTestUtils.h>
#include <DrvDdr.h>
#include <UnitTestApi.h>
#include "app_config.h"
#include "DrvLeonL2C.h"
#include "DrvCmxDma.h"
#include "DrvTimer.h"
#include "swcLeonUtils.h"
#include "DrvSvu.h"
#include "DrvCpr.h"
#include "DrvShaveL2Cache.h"



// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// first address in CMX
#define CMX_BEGIN (0x70000000ul)
// assuming CMX is 2MB
#define CMX_END   (0x70000000ul + 2*1024*1024)

// define array size macro
#define arraySize(array) ((sizeof((array)))/sizeof((array)[0]))

// maximum length of DDR buffer (CMX will be alocated separately)
#define BUFF_SIZE       (1024 * 1024) //1024 KB
// compute address location in CMX based on CMX base location
#define AddrRelToCMX(relativeAddress)    (0x70000000ul+(relativeAddress))

typedef void (*leonExec)(u32 pFuncMemcpy, u8* dst, u8* src, u32 size, u8 shaves);

typedef struct {
    u8 * from;
    u8 * to;
}transferLocation;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
__attribute__((section(".cmx.data"))) volatile u32 stopAllShaves;
extern volatile u32 shave_ticks;
extern volatile u32 shave_transactions;

// "public" functions in shave lib
extern u8 shave_svuMemcpyAsm, shave_svuMemcpyDMA, shave_svuSystemTestDMA;

// variables in shave lib
extern u8 shave_cmx_buff;

// shave dynamic lib
extern u8 shavedynlib;

// CmxDma Global Tail variable storing the beggiing of each of the max 4 transaction list's beginning
extern u32 *___globalTail[4];
// shave dynamic lib CmxDma GlobalTail variable.
extern u32 **shave_tail[];

u32 sysclk;

// 4: Static Local Data
// ----------------------------------------------------------------------------

static u32 verificationPassed = 1;
static u32 useCase = 1;

static u8 *ddr_buff0=(u8*)0x80000000; // buffer in ddr
static u8 *ddr_buff1=(u8*)0x81000000; // buffer in ddr

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void prettyPrintSize(u32 size)
{
    if(size<1024){
        printf("%3lu B",size);
    }
    else{
        printf("%3luKB",size/1024);
    }
}

u32 addressInCMX(u32 address)
{
    if ((CMX_BEGIN <= address) && (address < CMX_END))
        return 1;
    else
        return 0;
}

void prettyPrintMemType(u32 address)
{

    if ( addressInCMX(address) )
        printf(" CMX ");
    else
        printf(" DDR ");
}

// Used to fill out buffer with predictible data
static void fillBuffer(u8 *data, u32 size)
{
    for (u32 i = 0; i < size; i++)
        data[i] = i;
}

// returns the cycles time knowing frequency of the system clock
fp32 cyclesToTime(u32 cycles)
{
    // frequency is expected to be in sysclk in kHz
    return (0.001f/(fp32)sysclk)*cycles;
}

// returns the same address as addr but uncached
void * convertToUncachedAddr(void * addr)
{
    if ((u32) addr & 0x80000000)
        addr = (void*) ((u32) addr | 0x40000000);
    else
        // Assume CMX
        addr = (void*) ((u32) addr | 0x08000000);
    return addr;
}

u32 getShaveVar(u32 shaveNr, u32 relAddr)
{
    swcLeonDisableCaches();
    u32 shaveTicks= *(u32*)convertToUncachedAddr((void*)swcSolveShaveRelAddr(relAddr,shaveNr));
    swcLeonEnableCaches(1);

    return shaveTicks;
}

void setSvuOneBigPartition(void)
{
    u32 shaveIndex;

    for(u32 shaveIndex=0;shaveIndex<12;shaveIndex++)
    {
        swcSetAbsoluteDefaultStack(shaveIndex);
        swcSetShaveWindowsToDefault(shaveIndex);
    }
    // Set the shave L2 Cache mode
    DrvShaveL2CacheSetMode(SHAVEL2C_MODE_NORMAL);
    // Clear partitioning to setup L2 for first shave (as opposed to several smaller partitions for all shave)
    DrvShaveL2CacheClearPartitions();
    // Set Shave L2 cache partitions
    DrvShaveL2CacheSetupPartition(SHAVEPART256KB);
    // Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();
    // Assign the one partition defined to all shaves
    for (shaveIndex = 0; shaveIndex < 12; shaveIndex++)
    {
      DrvShaveL2CacheSetLSUPartId(shaveIndex, 0);
    }
    for(int shaveIndex=0;shaveIndex<12;shaveIndex++){
        DrvShaveL2CachePartitionFlushAndInvalidate(shaveIndex % MAX_SHAVE_L2C_PARTITIONS); //wrap around
    }
}

void setSvuSeveralSmallPartitions(void)
{
    u32 shaveIndex;

    for(u32 shaveIndex=0;shaveIndex<12;shaveIndex++)
    {
        swcSetAbsoluteDefaultStack(shaveIndex);
        swcSetShaveWindowsToDefault(shaveIndex);
    }

    // Set the shave L2 Cache mode
    DrvShaveL2CacheSetMode(SHAVEL2C_MODE_NORMAL);
    // Clear partitioning to setup L2 for all shaves (as opposed to one big partition for shave 0)
    DrvShaveL2CacheClearPartitions();
    // Set Shave L2 cache partitions
    for(shaveIndex=0;shaveIndex<12;shaveIndex++){
        DrvShaveL2CacheSetupPartition(SHAVEPART16KB);
    }
    // Allocate Shave L2 cache set partitions
    DrvShaveL2CacheAllocateSetPartitions();
    // Assign each partition defined to each shave
    for (shaveIndex = 0; shaveIndex < 12; shaveIndex++)
    {
      DrvShaveL2CacheSetLSUPartId(shaveIndex, shaveIndex % MAX_SHAVE_L2C_PARTITIONS); //wrap around
    }
    for(shaveIndex=0;shaveIndex<12;shaveIndex++){
        DrvShaveL2CachePartitionFlushAndInvalidate(shaveIndex % MAX_SHAVE_L2C_PARTITIONS); //wrap around
        DrvSvuL1DataCacheCtrl(shaveIndex, SVUL1INSTRCACHE_BYPASS);     //SVUL1INSTRCACHE_DEFAULT);
    }
}


// verify that moving the data succeeded
static void verifyData(u8 *dst, u8 *src, u32 size)
{
    src = convertToUncachedAddr(src);
    dst = convertToUncachedAddr(dst);

    //check the modified part
    for (u32 i = 0; i < size; i++)
        if (src[i] != dst[i]){
            verificationPassed=0; // check failed - data was not moved correctly
            printf("Failed %lu\n", i);
        }
}

// executes various tasks on shave, computes and prints the throughput
void leonSh0Exec(u32 pFuncMemcpy, u8* dst, u8* src, u32 size, u8 shaves){
    const u8 shaveIndex = 0; // shave used
    (void)shaves;

    prettyPrintMemType((u32)src);
    printf("to");
    prettyPrintMemType((u32)dst);

    memset((u8*) src, 0, size); // zeroing source
    memset((u8*) dst, 0, size); // zeroing destination
    fillBuffer(src, size);      // filling source with useful and predictible data

    swcLoadshvdlib(&shavedynlib,shaveIndex);
    swcResetShave(shaveIndex);
    swcShaveProfStartGatheringFields(shaveIndex, PERF_CLK_CYCLE_COUNT);  // shave profile will count cycles spended on processing

    // Init ___globalTail variable address for the shave context,
    // in the shave library the variable adress is windowed and it's a pointer that needs to be updated.
    u32 **pShvGlobalTail = (u32 **)swcSolveShaveRelAddr((u32)&shave_tail, shaveIndex);
    pShvGlobalTail[0] = (u32 *)(&___globalTail[0]);
    pShvGlobalTail[1] = (u32 *)(&___globalTail[1]);
    pShvGlobalTail[2] = (u32 *)(&___globalTail[2]);
    pShvGlobalTail[3] = (u32 *)(&___globalTail[3]);

    // exec pFuncMemcpy and wait for it to finish
    swcStartShaveCC(shaveIndex, swcSolveShaveRelAddr(pFuncMemcpy,shaveIndex) , "iii", (u32) dst, (u32) src, size);
    swcWaitShave(shaveIndex); // after this, it is expected that shave ticks will be holding the cycles spent on memory copying

    for(int i=0;i<MAX_SHAVE_L2C_PARTITIONS;i++)     // flush and invalidate all possible partitions;
        DrvShaveL2CachePartitionFlushAndInvalidate(i);

    // get cycles spent on memory copying and calculate
    printf("%9.4f MB/s", ((fp32)size / (1024.0f * 1024.0f)) / (fp32) (cyclesToTime(getShaveVar(shaveIndex,(u32)&shave_ticks))));
    verifyData(dst, src, size);
}

// executes a task on all shaves, computes and prints the throughput
void leonShMultipleExec(u32 pFuncMemcpy, u8* dst, u8* src, u32 size, u8 shaves){
    u8 shaveIndex; // shave used
    fp32 transferRate = 0.0;
    u64 leonTicks1 = 0;
    u64 leonTicks2 = 0;
    u32 **pShvGlobalTail;
    u32 shave_svuCmxDst_buff[12];
    u32 shave_svuCmxSrc_buff[12];
    u32 shave_function[12];

    const u32 repeatCount = 1000;

    for(u32 shaveIndex=0;shaveIndex<shaves;shaveIndex++)
    {
        shave_svuCmxDst_buff[shaveIndex]=swcSolveShaveRelAddr((u32)dst,shaveIndex);
        shave_svuCmxSrc_buff[shaveIndex]=swcSolveShaveRelAddr((u32)src,shaveIndex);
        shave_function[shaveIndex]      =swcSolveShaveRelAddr(pFuncMemcpy,shaveIndex);

        if(!addressInCMX(shave_svuCmxDst_buff[shaveIndex])){
            shave_svuCmxDst_buff[shaveIndex]+=size*shaveIndex;
        }
        if(!addressInCMX(shave_svuCmxSrc_buff[shaveIndex])){
            shave_svuCmxSrc_buff[shaveIndex]+=size*shaveIndex;
        }
    }

    prettyPrintMemType((u32)shave_svuCmxSrc_buff[0]);
    printf("to");
    prettyPrintMemType((u32)shave_svuCmxDst_buff[0]);

    for(u32 i=0;i<shaves;i++){
        memset((u8*)    shave_svuCmxDst_buff[i], 0, size); // zeroing source and destination
        fillBuffer((u8*)shave_svuCmxSrc_buff[i],    size); // filling source with useful and predictible data
    }

    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        swcResetShave(shaveIndex);
        swcSetAbsoluteDefaultStack(shaveIndex);
        swcLoadshvdlib(&shavedynlib,shaveIndex);
        DrvShaveL2CachePartitionFlushAndInvalidate(shaveIndex % MAX_SHAVE_L2C_PARTITIONS); //wrap around

        // Init ___globalTail variable address for the shave context,
        // in the shave library the variable adress is windowed and it's a pointer that needs to be updated.
        pShvGlobalTail = (u32 **)swcSolveShaveRelAddr((u32)&shave_tail, shaveIndex);
        pShvGlobalTail[0] = (u32 *)(&___globalTail[0]);
        pShvGlobalTail[1] = (u32 *)(&___globalTail[1]);
        pShvGlobalTail[2] = (u32 *)(&___globalTail[2]);
        pShvGlobalTail[3] = (u32 *)(&___globalTail[3]);

    }
    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        swcShaveProfStartGatheringFields(shaveIndex, PERF_CLK_CYCLE_COUNT);  // shave will count cycles spended on processing
    }

    stopAllShaves=0;
    DrvTimerGetSystemTicks64(&leonTicks1); // start profiling leon
    // exec function on all shaves
    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        swcStartShaveCC(shaveIndex, shave_function[shaveIndex], "iiiii", shave_svuCmxDst_buff[shaveIndex], shave_svuCmxSrc_buff[shaveIndex], size, repeatCount, &stopAllShaves);
    }
    // and wait for all shaves to finish
    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        swcWaitShave(shaveIndex);
    }
    DrvTimerGetSystemTicks64(&leonTicks2); // get profiling leon cycles

    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        transferRate+= ((fp32)size * getShaveVar(shaveIndex,(u32)&shave_transactions) / (1024.0f * 1024.0f)) / (fp32) (cyclesToTime(getShaveVar(shaveIndex, (u32)&shave_ticks))) ;
    }



    for(shaveIndex=0;shaveIndex<shaves;shaveIndex++){
        verifyData( (u8*)shave_svuCmxDst_buff[shaveIndex], (u8*)shave_svuCmxSrc_buff[shaveIndex], size);
    }

    printf("%7.2f MB/s", transferRate);
}

void benchmark(leonExec leon, u32 shave, u32 shCount, u8 *shList, char *testTitle, char *description , u32 widthsCount, u32 *widths, u32 transfers, transferLocation transfer[])
{
    u32 shaveCount;
    printf("\n%ld. Benchmark %-15s \n",useCase, testTitle);
    printf("%s\n",description);
    useCase++;
    // walk through
    for(u32 widthIndex=0; widthIndex<widthsCount; widthIndex++){
        for(u32 shaveCountIndex=0; shaveCountIndex<shCount; shaveCountIndex++){
            for(u32 transIndex=0;transIndex<transfers;transIndex++){
                if(shList!=0){
                    shaveCount = shList[shaveCountIndex];
                }else{
                    shaveCount = 0;
                }
                if(shCount>1){
                    printf("%2ld shaves", shaveCount);
                }
                prettyPrintSize(widths[widthIndex]);
                DrvShaveL2CacheSetMode(SHAVEL2C_MODE_NORMAL);

                leon(shave, transfer[transIndex].from, transfer[transIndex].to, widths[widthIndex], shaveCount);
                printf("\n");
            }
        }
    }
}


void benchmarkShave0memcpy(void)
{
    u8* uncachedDdr = (u8*)((u32)ddr_buff0 | 0x40000000 );
    // benchmark various sizes
    u32 widths[]={1*1024, 32*1024, 128*1024};
    transferLocation transfers[]={  {uncachedDdr, ddr_buff1},
                                    {uncachedDdr, (u8*)AddrRelToCMX(0x40000)},
                                    {(u8*)AddrRelToCMX(0x40000),ddr_buff1},
                                    {(u8*)AddrRelToCMX(0x40000),(u8*)AddrRelToCMX(0xC0000)}
                                };
    char memcpyU64Desc[] = "Shave 0 will transfer memory via CPU";

    benchmark(leonSh0Exec, (u32) &shave_svuMemcpyAsm, 1, 0, "shave memcpy", memcpyU64Desc, arraySize(widths), widths, arraySize(transfers), transfers);
}

void benchmarkShave0cpyDMA(void)
{
    // benchmark various sizes
    u32 widths[]={1*1024, 32*1024, 128*1024};
    transferLocation transfers[]={  {ddr_buff0, ddr_buff1},
                                    {ddr_buff0, (u8*)AddrRelToCMX(0x40000)},
                                    {(u8*)AddrRelToCMX(0x40000),ddr_buff1},
                                    {(u8*)AddrRelToCMX(0x40000),(u8*)AddrRelToCMX(0xC0000)}
                                };
    char memDmaDesc[] = "Shave 0 will transfer memory via DMA";
    benchmark(leonSh0Exec, (u32) &shave_svuMemcpyDMA, 1, 0, "shave DMA copy", memDmaDesc , arraySize(widths), widths, arraySize(transfers), transfers);
}


void benchmarkAllShavesCpyDMA(void)
{
    u32 widths[]={16*1024, 32*1024};
    transferLocation transfers[]={  {&shave_cmx_buff, ddr_buff0},
                                    {ddr_buff0, &shave_cmx_buff}
                                };
    char memDmaDesc[] = "Multiple shave processors transfer memory simultaneously via DMA";
    u8 shaveList[]  = {1,2,4,8,12};
    benchmark(leonShMultipleExec, (u32) &shave_svuSystemTestDMA, arraySize(shaveList), shaveList, "multiple shaves DMA", memDmaDesc , arraySize(widths), widths, arraySize(transfers), transfers);
}

// made a constructor in order to call it asap before using any DDR variables
void __attribute__((constructor)) ddrInit()
{
    tyDdrConfig customDdrConfig;
    DrvCprInit();
    DrvDdrGetDefaultConfiguration(&customDdrConfig);

    // choose one of the below:
    //   - address_map_R12_R0__B2_B0__C8_C0_32_bit_die
    //   - address_map_B2_B0__R12_R0__C8_C0_32_bit_die
    //   - address_map_B2_B1__R12_R0__B0__C8_C0_32_bit_die
    //   - address_map_B2__R12_R0__B1_B0__C8_C0_32_bit_die
    //   - address_map_R13_R0__B2_B0__C9_C0_32_bit_die
    //   - address_map_B2_B0__R13_R0__C9_C0_32_bit_die
    customDdrConfig.address_map = address_map_R13_R0__B2_B0__C9_C0_32_bit_die;

    // this is the code from DDRInit.elf, but called with customDdrConfig param
    if(DrvDdrIsInitialized()){
        printf("\n");
        printf("__          __                  _               \n");
        printf("\\ \\        / /                 (_)              \n");
        printf(" \\ \\  /\\  / /__ _  _ __  _ __   _  _ __    __ _ \n");
        printf("  \\ \\/  \\/ // _` || '__|| '_ \\ | || '_ \\  / _` |\n");
        printf("   \\  /\\  /| (_| || |   | | | || || | | || (_| |\n");
        printf("    \\/  \\/  \\__,_||_|   |_| |_||_||_| |_| \\__, |\n");
        printf("                                           __/ |\n");
        printf("                                          |___/ \n");
        printf("DDR appears to be initialized already, probably not with the right settings.\n");
        printf("The application will try to reinitialize it.\n");

        // Do not use DrvDdrShutdown for error recovery. It is bad practice, it is here only temporary.
        // TODO This use of DrvDdrShutdown is a workaround and will be deleted after the required fix
        DrvDdrShutdown();
    }
    DrvDdrInitialise(&customDdrConfig);
    DrvTimerInit();
}

int main(void)
{
    // initialize
    unitTestInit();

    initClocksAndMemory();
    DrvCprStartAllClocks();
    swcLeonSetPIL(0);

    setSvuOneBigPartition();

    DrvLL2CInitWriteThrough(); // put leon L2 in write-through mode

    swcLeonEnableCaches(1);

    // sysclk is used to compute time from clocks
    sysclk=DrvCprGetSysClockKhz(); // get system clock in kHz

    printf("\nDDR bandwidth benchmark\n\n");

    printf("System running at %lu Mhz\n", (u32)sysclk/1000);
    printf("DDR    clocked at %lu Mhz\n", (u32)DrvCprGetClockFreqKhz(AUX_CLK_DDR_CORE_CTRL, NULL)/1000*2);

    benchmarkShave0memcpy();
    benchmarkShave0cpyDMA();
    benchmarkAllShavesCpyDMA();

    if(verificationPassed)
        printf("All transactions were verified and passed\n");
    else
        printf("All transactions were verified and some failed\n");

    printf("All benchmarks finished\n\n\n");

    unitTestFinalReport();
}
