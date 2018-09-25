///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup swcLeonUtils Leon Utilities API
/// @ingroup  swcLeonUtils
/// @{
/// @brief     API manipulating Leon functionalities
///
/// Allows manipulating leon caches and other features
///

#ifndef __LEON_UTILS_H__
#define __LEON_UTILS_H__

#include "swcLeonUtilsDefines.h"
#include <mv_types.h>

/// @brief Pointer type
/// @note the pointer type is only relevant if it is addressing < 32bit values
typedef enum {
	/// @brief normal leon pointer
    be_pointer,
	/// @brief little-endian/shave pointer
    le_pointer
} pointer_type;

#ifdef __cplusplus
extern "C" {
#endif

#define NATIVE_POINTER_TYPE le_pointer

/// @brief Swaps endianness of a 32-bit integer (usefull when sharing data between Leon and Shave)
/// @param[in] value u32 integer to be swapped
/// @returns swapped integer
#define swcLeonSwapU32(value) \
        ((((u32)((value) & 0x000000FF)) << 24) | \
                ( ((u32)((value) & 0x0000FF00)) <<  8) | \
                ( ((u32)((value) & 0x00FF0000)) >>  8) | \
                ( ((u32)((value) & 0xFF000000)) >> 24))


/// @brief Swaps endianness of a 16-bit integer (usefull when sharing data between Leon and Shave)
/// @param[in] value u16 integer to be swapped
/// @returns swapped integer
#define swcLeonSwapU16(value) \
        ((((u16)((value) & 0x00FF)) << 8) | \
                (((u16)((value) & 0xFF00)) >> 8))



/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - u8 variable value read bypassing cache
#define swcLeonReadNoCacheU8(addr) \
    ({ \
        unsigned char local_var_result; \
        asm volatile( \
            "lduba [%[addr_]] 1, %[result]" \
            : [result] "=r" (local_var_result) \
            : [addr_] "r" (addr) \
            : "memory" \
        ); \
        local_var_result; \
    })

/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - i8 variable value read bypassing cache
#define swcLeonReadNoCacheI8(addr) \
    ({ \
        signed char local_var_result; \
        asm volatile( \
            "ldsba [%[addr_]] 1, %[result]" \
            : [result] "=r" (local_var_result) \
            : [addr_] "r" (addr) \
            : "memory" ); \
        local_var_result; \
    })


/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - u16 variable value read bypassing cache
#define swcLeonReadNoCacheU16(addr) \
    ({ \
        unsigned short local_var_result; \
        asm volatile( \
            "lduha [%[addr_]] 1, %[result]" \
            : [result] "=r" (local_var_result) \
            : [addr_] "r" (addr) \
            : "memory" ); \
        local_var_result; \
    })


/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - i16 variable value read bypassing cache
#define swcLeonReadNoCacheI16(addr) \
    ({ \
        signed short local_var_result; \
        asm volatile( \
            "ldsha [%[addr_]] 1, %[result]" \
            : [result] "=r"(local_var_result) \
            : [addr_] "r" (addr) \
            : "memory" ); \
        local_var_result; \
    })


/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - u32 variable value read bypassing cache
#define swcLeonReadNoCacheU32(addr) \
    ({ \
        unsigned int local_var_result; \
        asm volatile( \
            "lda [%[addr_]] 1, %[result]" \
            : [result] "=r" (local_var_result) \
            : [addr_] "r" (addr) \
            : "memory" ); \
        local_var_result; \
    })

/// @brief Reads data bypassing leon LRAM cache
/// @param[in] addr u32 address to read
/// @return - s32 variable value read bypassing cache
#define swcLeonReadNoCacheI32(addr) ((int)swcLeonReadNoCacheU32(addr))


/// @brief Reads data bypassing leon L1 cache
/// @param[in] addr address of u64 to read
/// @return - u64 variable value read bypassing cache
#define swcLeonReadNoCacheU64(addr) \
    ({ \
        u64 local_var_result; \
        asm volatile( \
            "ldda  [%[addr_]] 1, %[result]" \
            : [result] "=r" (local_var_result) \
            : [addr_] "r" ((addr)) \
            : "memory" ); \
        local_var_result; \
    })


/// @brief Reads data bypassing leon L1 cache
/// @param[in] addr s64 address to read
/// @return - s64 variable value read bypassing cache
#define swcLeonReadNoCacheI64(addr) ((s64)swcLeonReadNoCacheU64(addr))


/// @brief Writes data bypassing leon LRAM cache
/// @param[in] addr - u32 address to write
/// @param[in] data - i8/u8 variable to write
#define swcLeonWriteNoCache8(addr,data) \
    asm volatile( \
        "stba %[data_], [%[addr_]] 1" \
        : \
        : [addr_] "r"(addr), \
          [data_] "r"(data) \
        : "memory" \
    )

/// @brief Writes data bypassing leon LRAM cache
/// @param[in] addr - u32 address to write
/// @param[in] data - i16/u16 variable to write
#define swcLeonWriteNoCache16(addr,data) \
    asm volatile( \
        "stha %[data_], [%[addr_]] 1" \
        : \
        : [addr_] "r" (addr), \
          [data_] "r" (data) \
        : "memory" \
    )

/// @brief Writes data bypassing leon LRAM cache
/// @param[in] addr - u32 address to write
/// @param[in] data - s32/u32 variable to write
#define swcLeonWriteNoCache32(addr,data) \
    asm volatile( \
        "sta %[data_], [%[addr_]] 1" \
        : \
        : [addr_] "r" (addr), \
          [data_] "r" (data) \
        : "memory" \
    )

/// @brief Writes data bypassing leon L1 cache
/// @param[in] addr - u64 address to write
/// @param[in] data - s64/u64 variable to write
#define swcLeonWriteNoCache64(addr,data) \
    asm volatile( \
        "stda  %[data_], [%[addr_]] 1" \
        : \
        : [data_] "r"((u64)(data)), \
          [addr_] "r" ((addr)) \
        : "memory" \
    )

/// @brief Flush Leon Instruction and Data Caches
#define swcLeonFlushCaches() \
    asm volatile( "flush" ::: "memory" )

/// @brief Flush Leon Data Cache
#define swcLeonDataCacheFlush()                        \
    asm volatile(                                      \
        "sta %%g0, [%%g0] %[dcache_flush_asi]"         \
        :                                              \
        : [dcache_flush_asi] "I" (__DCACHE_FLUSH_ASI)  \
        : "memory"                                     \
    )

#define swcLeonFlushDcache() swcLeonDataCacheFlush()
#define swcLeonDataCacheFlushNoWait() swcLeonDataCacheFlush()

/// @brief Flush Leon Instruction Cache
#define swcLeonInstructionCacheFlush()                  \
    ({                                                  \
        unsigned local_var_tmp;                                   \
        asm volatile(                                   \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"   "\n\t" \
            "bset %[ccr_flags], %[tmp]"          "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"          \
            : [tmp] "=&r" (local_var_tmp)                         \
            : [ccr_flags] "r" (CCR_FI),                 \
              [__CCR_ASI_] "I" (__CCR_ASI)              \
            : "memory"                                  \
        ); \
    })

#define swcLeonFlushIcache() swcLeonInstructionCacheFlush()

/// @brief Check if Leon cache flush is pending
#define swcLeonIsCacheFlushPending()    \
    ({                                  \
        unsigned local_var_tmp, local_var_result;           \
        asm (                           \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"  \
            : [tmp] "=r" (local_var_tmp)                  \
            : [__CCR_ASI_] "I" (__CCR_ASI)      \
            : "memory"                  \
        );                              \
        local_var_result = (local_var_tmp >> 14) & 3;       \
        local_var_result;                         \
    })

/// @brief Enable Leon Instruction and Data Caches
/// @param[in] flush flag: 0 = don't flush cache ; 1 = flush cache
#ifndef MYRIAD2
#define swcLeonEnableCaches(flush)                                \
    asm volatile(                                                 \
        "sta %[ccr_value], [%%g0] %[__CCR_ASI_]"                  \
        :                                                         \
        : [ccr_value] "r" (CCR_IB | CCR_ICS_ENABLED | CCR_DCS_ENABLED |   \
                ((flush) ? (CCR_FI | CCR_FD) : 0)),               \
          [__CCR_ASI_] "I" (__CCR_ASI)                            \
        : "memory"                                                \
    )
#else
#define swcLeonEnableCaches(flush)                                \
    asm volatile(                                                 \
        "sta %[ccr_value], [%%g0] %[__CCR_ASI_]"                  \
        :                                                         \
        : [ccr_value] "r" (CCR_ICS_ENABLED | CCR_DCS_ENABLED |    \
                ((flush) ? (CCR_FI | CCR_FD) : 0)),               \
          [__CCR_ASI_] "I" (__CCR_ASI)                            \
        : "memory"                                                \
    )
#endif

/// @brief Enable Leon Instruction Cache
/// @param[in] flush flag: 0 = don't flush cache ; 1 = flush cache
#ifndef MYRIAD2
#define swcLeonEnableIcache(flush)      \
    ({ \
        unsigned local_var_tmp; \
        asm volatile(                                      \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"      "\n\t" \
            "bset %[ccr_flags], %[tmp]"             "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"             \
            : [tmp] "=&r" (local_var_tmp)                            \
            : [ccr_flags] "r" (CCR_IB | CCR_ICS_ENABLED | ((flush)?CCR_FI:0)), \
              [__CCR_ASI_] "I" (__CCR_ASI)                 \
            : "memory"                                     \
        ); \
    })
#else
#define swcLeonEnableIcache(flush) \
    ({ \
        unsigned local_var_tmp; \
        asm volatile(                                       \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"       "\n\t" \
            "bset %[ccr_flags], %[tmp]"              "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"              \
            : [tmp] "=&r" (local_var_tmp)                 \
            : [ccr_flags] "r" (CCR_ICS_ENABLED | ((flush)?CCR_FI:0)), \
              [__CCR_ASI_] "I" (__CCR_ASI)                  \
            : "memory"                                      \
        ); \
    })
#endif


/// @brief Enable Leon Data Cache
/// @param[in] flush flag: 0 = don't flush cache ; 1 = flush cache
#define swcLeonEnableDcache(flush) \
    ({ \
        unsigned local_var_tmp; \
        asm volatile(                                        \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"        "\n\t" \
            "bset %[ccr_flags], %[tmp]"               "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"               \
            : [tmp] "=&r" (local_var_tmp)                  \
            : [ccr_flags] "r" ((CCR_DCS_ENABLED) | ((flush) ? CCR_FD : 0)), \
              [__CCR_ASI_] "I" (__CCR_ASI)                   \
            : "memory" \
        ); \
    })

/// @brief Disable Leon Instruction and Data Caches
#define swcLeonDisableCaches()          \
    asm volatile( "sta %%g0, [%%g0] 2" ::: "memory" )


/// @brief Disable Leon Data Cache
#define swcLeonDisableDcache() \
    ({ \
        unsigned local_var_tmp; \
        asm volatile( \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"        "\n\t" \
            "bclr %[ccr_clearthese], %[tmp]"          "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"               \
            : [tmp] "=&r" (local_var_tmp)                  \
            : [ccr_clearthese] "r" (CCR_DCS_ENABLED),        \
              [__CCR_ASI_] "I" (__CCR_ASI)                   \
            : "memory" \
        ); \
    })


/// @brief Disable Leon Instruction Cache
#ifndef MYRIAD2
#define swcLeonDisableIcache() \
    ({ \
        unsigned local_var_tmp; \
        asm volatile( \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"        "\n\t" \
            "bclr %[ccr_cleathese], %[tmp]"           "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"               \
            : [tmp] "=&r" (local_var_tmp)                  \
            : [ccr_clearthese] "r" (CCR_IB | CCR_ICS_ENABLED), \
              [__CCR_ASI_] "I" (__CCR_ASI)                   \
            : "memory"                                       \
        ); \
    })
#else
#define swcLeonDisableIcache() \
    ({ \
        unsigned local_var_tmp; \
        asm volatile( \
            "lda [%%g0] %[__CCR_ASI_], %[tmp]"        "\n\t" \
            "bclr %[ccr_clearthese], %[tmp]"          "\n\t" \
            "sta %[tmp], [%%g0] %[__CCR_ASI_]"               \
            : [tmp] "=&r" (local_var_tmp)                  \
            : [ccr_clearthese] "r" (CCR_ICS_ENABLED),        \
              [__CCR_ASI_] "I" (__CCR_ASI)                   \
            : "memory"                                       \
        ); \
    })
#endif

/// @brief Flushes Leon data cache, and wait while the flush is pending. (DO NOT USE)
///
/// @note It is not recommended to use this function
/// Leon DCache flush takes 128 cycles as it processes
/// each line of the cache at 1 cycle per line
/// There is no advantage to wait until the flush is not pending anymore.
/// use the swcLeonDataCacheFlush() macro instead.
void swcLeonDataCacheFlushBlockWhilePending(void);

/// @brief Stops Leon
void swcLeonHalt(void);

#if defined(__RTEMS__)
u32 swcLeonSetPIL(u32 pil);
u32 swcLeonGetPIL(void);
#else
/// @brief Sets the Processor Interrupt Level atomically
///
/// @param[in] pil - processor interrupt level
/// @return        - previous processor interrupt level
int swcLeonSetPIL( u32 pil);  // in traps.S

/// @brief Get IRQ Level
///
/// Returns PIL field from the Leon's PSR.
/// @return current PIL field
static inline u32 swcLeonGetPIL(void) {
    u32 result;
    _ASM(
        "rd %%psr, %[result]"                         "\n\t"
        "and %[result], %[MASK_PSR_PIL_],%[result]"   "\n\t"
        "srl %[result], %[POS_PSR_PIL_], %[result]"
        : [result] "=r" (result) //=&r not needed because all input operands are constants, and not registers
        : [MASK_PSR_PIL_] "I" (MASK_PSR_PIL),
          [POS_PSR_PIL_] "I" (POS_PSR_PIL)
        : "memory"
    );
    return result;
}
#endif

/// @brief Flushes all the interrupt windows before the caller's to the stack
/// @note You'd ideally call this before your main app loop, if any -
/// allows you to avoid window_overflow's for the next 6-deep calls
void swcLeonFlushWindows(void);

/// @brief Disable traps
/// @attention Enter/leave a critical section in a clean way - do NOT call any function
/// between these!
/// @return
///				- 1 if traps were enabled
///				- 0 if traps were not enabled

#define swcLeonDisableTraps() \
({ \
    int temp; \
    int old_et; \
    asm volatile ( \
        "rd      %%psr, %[ret]"            "\n\t" \
        "andn    %[ret], 0x00000020, %[temp]"  "\n\t" \
        "wr      %[temp], %%psr"           "\n\t" \
        " and    %[ret], 0x00000020, %[ret]"   "\n\t" \
        " srl    %[ret], 5, %[ret]"        "\n\t" \
        " nop" \
        : [ret] "=r" (old_et), \
          [temp] "=r" (temp) \
        :: "memory", "cc"); \
    old_et; \
})


/// @brief Enable traps
/// @attention Enter/leave a critical section in a clean way - do NOT call any function
///  between these!!!
/// @return
///				- 1 if traps were enabled
///				- 0 if traps were not enabled

#define swcLeonEnableTraps() \
({ \
    int temp; \
    int old_et; \
    asm volatile ( \
        "rd      %%psr, %[ret]"            "\n\t" \
        "or      %[ret], 0x00000020, %[temp]"  "\n\t" \
        "wr      %[temp], %%psr"           "\n\t" \
        " and    %[ret], 0x00000020, %[ret]"   "\n\t" \
        " srl    %[ret], 5, %[ret]"        "\n\t" \
        " nop" \
        : [ret] "=r" (old_et), \
          [temp] "=r" (temp) \
        :: "memory", "cc"); \
    old_et; \
})


/// @brief Read the ET flag from Leon's PSR
/// @return A 32 bit value representative of ET value. If !=0 then Traps Are ENABLED
static inline u32 swcLeonAreTrapsEnabled(void) {
    u32 result;
    // read current PSR and mask out everything except ET
    _ASM(
        "rd %%psr, %[result]"                  "\n\t"
        "and %[result], %[PSR_ET_], %[result]"
        : [result] "=r" (result) //=&r not needed because all input operands are constants, and not registers
        : [PSR_ET_] "I" (PSR_ET)
        : "memory"
    );
    return result;
}

/// @brief Read the value of the PSR register
static inline u32 swcLeonGetPSR(void) {
    u32 result;
    _ASM(
        "rd %%psr, %[result]"
        : [result] "=r" (result)
        :
        : "memory"
    );
    return result;
}

/// @brief Read the value of the WIM register
static inline u32 swcLeonGetWIM(void) {
    u32 result;
    _ASM(
        "rd %%wim, %[result]"
        : [result] "=r" (result)
        :
        : "memory"
    );
    return result;
}

/// @brief Sets correct byte depending on endianess
/// @param[in] pointer void* pointer to byte to set
/// @param[in] pt pointer_type be_pointer or le_pointer
/// @param[in] value u8 value to set
static inline void swcLeonSetByte(void *pointer, pointer_type pt, u8 value) {
    if (pt == NATIVE_POINTER_TYPE)
        *(u8 *)(pointer) = value;
    else
        *(u8 *)( ((u32)pointer)^3 ) = value;
}

/// @brief Gets correct byte depending on endianess
/// @param[in] pointer void* pointer to byte to set
/// @param[in] pt pointer_type be_pointer or le_pointer
static inline u8 swcLeonGetByte(const void *pointer, pointer_type pt) {
    if (pt == NATIVE_POINTER_TYPE)
        return *(u8 *)(pointer);
    else
        return *(u8 *)( ((u32)pointer)^3 );
}

/// @brief Sets correct byte depending on endianess
/// @param[in] pointer void* pointer to byte to set
/// @param[in] pt pointer_type be_pointer or le_pointer
/// @param[in] value u8 value to set
static inline void swcLeonSetHalfWord(void *pointer, pointer_type pt, u16 value) {
    if (pt == NATIVE_POINTER_TYPE)
        *(u16 *)(pointer) = value;
    else
        *(u16 *)( ((u32)pointer)^2 ) = value;
}

/// @brief Gets correct byte depending on endianess
/// @param[in] pointer void* pointer to byte to set
/// @param[in] pt pointer_type be_pointer or le_pointer
static inline u16 swcLeonGetHalfWord(const void *pointer, pointer_type pt) {
    if (pt == NATIVE_POINTER_TYPE)
        return *(u16 *)(pointer);
    else
        return *(u16 *)( ((u32)pointer)^2 );
}

/// @brief Generic memory copying function to copy le/be buffers to le/be buffers
///
/// - The buffers may be unaligned, and they may have an unaligned size.
/// - The buffers may be anywhere in memory, data is accessed using word-access only
/// - The buffers may not overlap! If you need overlapping buffers, then see swcLeonMemMove().
///   Exceptions to the no-overlap rule:
///    1. Same endianness buffers may overlap if you know for sure that the destination
///       will always be before the source (meaning (u32)src >= (u32)dst),
///       assert(src >= dst);
///    2. Different endianness buffers may overlap if (u32)src >= (u32)dst + 3
///       if (src_pt != dst_pt) assert(src >= dst + 3);
/// @param[out] dst The destination buffer.
/// @param[in] dst_pt The endianness of the destination buffer
/// @param[in] src The source buffer
/// @param[in] src_pt The endianness of the source buffer
/// @param[in] count Number of bytes to copy. It is not required for this to be divisible by 4.
void swcLeonMemCpy(void *dst, pointer_type dst_pt, const void *src, pointer_type src_pt, u32 count);

/// @brief Same as swcLeonMemCpy, except buffers may overlap.
///
/// The distance between overlapping buffer pointers of opposite endianness must be >= 3
///
/// if (src_pt != dst_pt) assert( abs(src - dst) >=3 );
/// @param[out] dst The destination buffer.
/// @param[in] dst_pt The endianness of the destination buffer
/// @param[in] src The source buffer
/// @param[in] src_pt The endianness of the source buffer
/// @param[in] count Number of bytes to copy. It is not required for this to be divisible by 4.
void swcLeonMemMove(void *dst, pointer_type dst_pt, const void *src, pointer_type src_pt, u32 count);

/// @brief Swap the endianness of a buffer in place
///
/// The buffer pointer and count may be unaligned, but you have to make sure
/// that sufficient bytes are aligned before and after the buffer, to fit the flipped buffer.
/// @param[in, out] buf    - Buffer to work on
/// @param[in]      pt     - initial endianness of the buffer
/// @param[in]      count  - number of bytes
void swcLeonSwapBuffer(void *buf, pointer_type pt, u32 count);

/// @brief 32 bit read from the Leon Asr17 (Processor Configuration Register)
/// @return 32 bit value read back
static inline unsigned int swcLeonRead32Asr17(void)
{
    unsigned int pcrValue; // used for returned value
    asm volatile ("rd %%asr17, %[pcrValue]" :
                [pcrValue] "=r" (pcrValue) );
    return pcrValue;
}

/// @brief 32 bit read from Processor Configuration Register
/// @return 32 bit value read back
static inline unsigned int swcLeonGetProcessorConfig(void)
{
    return swcLeonRead32Asr17();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Leon Address Space Access Helper functions
//  The following functions allow C level access to the different address spaces exposed
//  by the Leon processor
//  Note: There is some duplication between the address spaces accessed below and the application specific functionality
//  implemented above (e.g. Cache control). Where appropriate the application specific functions should be used.
//  The functions below allow 32 bit r/w access to all address spaces with in orthogonal API.
//
//  Summary of Leon Address Spaces
//  0x01  -- Forced Cache Miss
//  0x02  -- Cache Control Registers
//  0x08  -- Normal Cached Access (Replace if cacheable)
//  0x09  -- Normal Cached Access (Replace if cacheable)
//  0x0A  -- Normal Cached Access (Replace if cacheable)
//  0x0B  -- Normal Cached Access (Replace if cacheable)
//  0x0C  -- L1 Instruction Cache Tags
//  0x0D  -- L1 Instruction Cache Data
//  0x0E  -- L1 Data Cache Tags
//  0x0F  -- L1 Data Cache Data
//  0x10  -- Flush Instruction Cache // MMU Flush
//  0x11  -- Flush Data Cache
//  0x13  -- MMU Flush context
//  0x14  -- MMU Diagnostic DCache Context Access
//  0x15  -- MMU Diagnostic ICache Context Access
//  0x19  -- MMU Registers
//  0x1C  -- MMU Cache Bypass
//  0x1D  -- MMU Diagnostic Access
//  0x1E  -- MMU Snoop tags diag access
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/// @brief 32 bit write to the Leon 0x1 Address space (Forced Cache Miss)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi01(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x01" // store val to addr at ASI 0x01
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );

}

/// @brief 32 bit read from the Leon 0x1 Address space (Forced Cache Miss)
/// @param[in] addr - address where to read from
/// @return  32 bit value read back
static inline unsigned int swcRead32Asi01(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x01, %[result]" // load result from addr at ASI 0x01
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief Force a Leon L1 data cache miss
///
/// Reads the value pointed by addr directly from memory.
/// Fills/Updates the whole L1C line
///
/// @param[in] addr u32 address to read
/// @note swcLeonReadNoCache* will become deprecated
///
#define swcLeonL1DForceCacheLineMiss(addr) swcRead32Asi01(addr)

/// @brief 32 bit write to the Leon 0x2 Address space (Cache Control Registers)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi02(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x02" // store val to addr at ASI 0x02
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x2 Address space (Cache Control Registers)
/// @param[in] addr - address where to read from
/// @return  32 bit value read back
static inline unsigned int swcRead32Asi02(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x02, %[result]" // load result from addr at ASI 0x02
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0xC Address space (L1 ICache Tags)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi0C(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x0C" // store val to addr at ASI 0x0C
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );

}

/// @brief 32 bit read from the Leon 0xC Address space (L1 ICache Tags)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi0C(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x0C, %[result]" // load result from addr at ASI 0x0C
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0xD Address space (L1 ICache Data)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi0D(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x0D" // store val to addr at ASI 0x0D
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0xD Address space (L1 ICache Data)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi0D(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x0D, %[result]" // load result from addr at ASI 0x0D
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}


/// @brief 32 bit write to the Leon 0xE Address space (L1 DCache Tags)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi0E(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x0E" // store val to addr at ASI 0x0E
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0xE Address space (L1 DCache Tags)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi0E(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x0E, %[result]" // load result from addr at ASI 0x0E
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0xF Address space (L1 DCache Data)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi0F(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x0F" // store val to addr at ASI 0x0F
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0xF Address space (L1 DCache Tags)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi0F(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x0F, %[result]" // load result from addr at ASI 0x0F
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x10 Address space (Flush L1 ICache // MMU Flush Page)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi10(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x10" // store val to addr at ASI 0x10
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x10 Address space (Flush L1 ICache // MMU Flush Page)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi10(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x10, %[result]" // load result from addr at ASI 0x10
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x11 Address space (Flush L1 DCache)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi11(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x11" // store val to addr at ASI 0x11
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x11 Address space (Flush L1 DCache)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi11(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x11, %[result]" // load result from addr at ASI 0x11
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x13 Address space (MMU Flush context)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi13(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x13" // store val to addr at ASI 0x13
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x13 Address space (MMU Flush context)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi13(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x13, %[result]" // load result from addr at ASI 0x13
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x14 Address space (MMU Diag dcache context access)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi14(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x14" // store val to addr at ASI 0x14
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x14 Address space (MMU Diag dcache context access)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi14(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x14, %[result]" // load result from addr at ASI 0x14
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x15 Address space (MMU Diag icache context access)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi15(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x15" // store val to addr at ASI 0x15
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x15 Address space (MMU Diag icache context access)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi15(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x15, %[result]" // load result from addr at ASI 0x15
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x19 Address space (MMU Register access)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi19(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x19" // store val to addr at ASI 0x19
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x19 Address space (MMU Register access)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi19(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x19, %[result]" // load result from addr at ASI 0x19
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x1C Address space  (MMU Bypass)
/// @param[in] addr - address where to write
/// @param[in] val - 32 bit value to write
static inline void swcWrite32Asi1C(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x1C" // store val to addr at ASI 0x1C
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x1C Address space   (MMU Bypass)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi1C(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x1C, %[result]" // load result from addr at ASI 0x1C
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 64 bit write to the Leon 0x1C Address space  (MMU Bypass)
/// @param[in] addr - address where to write
/// @param[in] val  - 64 bit value to write
static inline void swcWrite64Asi1C(u32 addr, u64 val)
{
    asm volatile (
        "stda %[val], [%[addr]] 0x1C" // store val to addr at ASI 0x1C
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
    );
}

/// @brief 64 bit read from the Leon 0x1C Address space   (MMU Bypass)
/// @param[in] addr - address where to read from
/// @return 64 bit value read back
static inline u64 swcRead64Asi1C(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "ldda [%[addr]] 0x1C, %[result]" // load result from addr at ASI 0x1C
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x1D Address space  (MMU Diag access)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi1D(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x1D" // store val to addr at ASI 0x1D
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x1D Address space  (MMU Diag access)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi1D(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x1D, %[result]" // load result from addr at ASI 0x1D
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}

/// @brief 32 bit write to the Leon 0x1E Address space  (MMU snoop Diag access)
/// @param[in] addr - address where to write
/// @param[in] val  - 32 bit value to write
static inline void swcWrite32Asi1E(u32 addr, u32 val)
{
    asm volatile (
        "sta %[val], [%[addr]] 0x1E" // store val to addr at ASI 0x1E
        : // output
        : [val] "r" (val),
          [addr] "r" (addr) // inputs
        : "memory"
    );
}

/// @brief 32 bit read from the Leon 0x1E Address space  (MMU snoop  Diag access)
/// @param[in] addr - address where to read from
/// @return 32 bit value read back
static inline unsigned int swcRead32Asi1E(u32 addr)
{
    unsigned int result; // used for returned value
    asm volatile (
        "lda [%[addr]] 0x1E, %[result]" // load result from addr at ASI 0x1E
        : [result] "=r" (result) // output
        : [addr] "r" (addr) // input
        : "memory"
    );
    return result;
}
/// @}
#ifdef __cplusplus
}
#endif

#endif //__LEON_UTILS_H__
