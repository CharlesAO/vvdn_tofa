///
/// @file DrvRegUtils.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvRegUtils RegUtils Driver
/// @{
/// @brief     Macros and functions to ease register manipulation
///


#ifndef REG_UTILS_H
#define REG_UTILS_H 

#include "DrvRegUtilsDefines.h"
#include "mv_types.h"

// ----------------------------------------------------------------------------
// 1: Includes
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// Program a particular bitfield with a specific value
/// 
/// e.g. DrvRegSetBitField(0x70000000,4,5,0x13) would result in
/// 0x70000000 = 0xCAFE8938 where original value was 0xCAFE8888
/// @param[in] regAddress Address of register to modify
/// @param[in] startBit   Offset of the start of the bitfield to be modified
/// @param[in] numBits    Size of the bitfield in bits
/// @param[in] value      value to be written to the bitfield
/// @return    void
void DrvRegSetBitField(void * regAddress, u32 startBit, u32 numBits, u32 value);


/// Returns the value of a particular bitfield
/// 
/// @param[in] regAddress Address of register to read
/// @param[in] startBit   Offset of the start of the bitfield to be read
/// @param[in] numBits    Size of the bitfield in bits
/// @return    value of the bitfield
u32 DrvRegGetBitField(void * regAddress, u32 startBit, u32 numBits);

/// Poll waiting for any one of the bits in bitSetMask to be set
/// @param[in] Address of register to Poll
/// @param[in] 32 bit mask for bits to be tested
/// @param[in] Delay in micoseconds after which this function will timeout
/// @return    0 on success, non-zero on timeout
int DrvRegWaitForBitsSetAny(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs);

/// Poll waiting for all of the bits in bitSetMask to be set
/// @param[in] Address of register to Poll
/// @param[in] 32 bit mask for bits to be tested
/// @param[in] Delay in micoseconds after which this function will timeout
/// @return    0 on success, non-zero on timeout
int DrvRegWaitForBitsSetAll(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs);

/// Poll waiting for all of the bits in bitSetMask to be cleared
/// @param[in] Address of register to Poll
/// @param[in] 32 bit mask for bits to be tested
/// @param[in] Delay in micoseconds after which this function will timeout
/// @return    0 on success, non-zero on timeout
int DrvRegWaitForBitsClearAll(u32 registerAddress, u32 bitSetMask, u32 timeOutMicroSecs);


/// Perform a bit manipulation on register based on mask value
/// @param[in] Address of register 
/// @param[in] operation to perform on register SET_WORD,SET_BITS,CLR_BITS
/// @param[in] Value to use as mask
/// @return    0 on success, non-zero on timeout
void DrvRegBitMaskOperation(void * regAddress, tyRegMaskOperation operation, u32 value);

/// Diagnostic function which can be used to work out
/// what environment the code is running out (e.g. Real chip or simulation)
/// This can be used to perform smaller tests in simulation than on real chip
/// @return    tyHwPlatform (silicon,movisim,vcs or fpga)
static inline tyHwPlatform DrvRegGetHardwarePlatform(void)
{
    u32 platformCode;
    platformCode = (GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) >> 29) & 0x7;
    switch (platformCode)
    {
    case 0:
        return PLATFORM_SILICON;
    case 1:
        return PLATFORM_VCS;
    case 2:
        return PLATFORM_MOVISIM;
    case 3:
        return PLATFORM_FPGA;
    default:
        return PLATFORM_SILICON;
    }
}

#define DRAM_UNCACHEABLE_MASK 0x40000000
#define CMX_UNCACHEABLE_MASK 0x08000000

static inline void *DrvRegGetUncacheableAddr(const void *addr) {
    if ((uint32_t)addr & DRAM_BASE_ADR)
        addr = (const void *)((uint32_t)addr | DRAM_UNCACHEABLE_MASK);
    else if (((uint32_t)addr & CMX_BASE_ADR) == CMX_BASE_ADR)
        addr = (const void *)((uint32_t)addr | CMX_UNCACHEABLE_MASK);

    return (void *)addr;
}

static inline void *DrvRegGetCacheableAddr(const void *addr) {
    if ((uint32_t)addr & DRAM_BASE_ADR)
        addr = (const void *)((uint32_t)addr & (~DRAM_UNCACHEABLE_MASK));
    else if (((u32)addr & CMX_BASE_ADR) == CMX_BASE_ADR)
        addr = (const void *)((uint32_t)addr & (~CMX_UNCACHEABLE_MASK));

    return (void *)addr;
}

#ifdef __cplusplus
}
#endif

#endif // REG_UTILS_H  

///@}
