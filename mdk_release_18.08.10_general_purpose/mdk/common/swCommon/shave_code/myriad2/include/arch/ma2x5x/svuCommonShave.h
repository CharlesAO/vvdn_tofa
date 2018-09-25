///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup svuCommonShaveApi Common Shave API
/// @ingroup  svuCommonShaveApi
/// @{
/// @brief API manipulating shave functionalities
///
/// Allows shaves manipulating mutexes and other features.
/// For a more detailed understanding please reread the relevant sections in the n the MDKMyriad2Programmer_Guide document.
///

#ifndef __SVU_COMMON_SHAVE__
#define __SVU_COMMON_SHAVE__

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include "svuCommonShaveDefines.h"
#include "ShDrvMutex.h"
#include "ShDrvCmxFifo.h"

__asm(//Disable false warning as workaround until resolution of Bugzilla http://dub30/bugzilla/show_bug.cgi?id=22833
".nowarn 32\n"
".include svuCommonDefinitions.incl\n"
".nowarnend\n");

// 2: Macros
// ----------------------------------------------------------------------------

/// @brief Shave halt instruction
#define SHAVE_HALT { __asm volatile ( \
    "// 'SHAVE_HALT' defined in svuCommonShave.h used in " __FILE__ "\n\t" \
    "NOP"              "\n\t" \
    "BRU.swih 0x001F"  "\n\t" \
    "NOP 6"            "\n\t" \
    ::: "memory"); __builtin_unreachable (); }

/// @brief Switch u32 contents of variable
/// @param[in] var - variable to switch contents to
///
static inline void swcShaveSwapU32VarContents(volatile u32 *variable)
{
    u32 var;
    var=*variable;
    __asm("// 'swcShaveSwapU32VarContents()' defined in svuCommonShave.h used in " __FILE__ "\n\t" \
        "SAU.SWZ %[var_], %[var_], [0123]" \
        : [var_] "+r" (var) \
        : \
        : "cc");
    *variable=var;
}

/// @brief Switch u32 contents of variable and place results in a different variable
/// @param[in]  varin  - variable to switch contents
/// @param[out] varout - variable to place switched contents into
///
static inline void swcShaveSwapU32VarsContents(volatile u32 variablein, volatile u32 *variableout)
{
    u32 varin,varout;
    varin=variablein;
    __asm("// 'swcShaveSwapU32VarsContents()' defined in svuCommonShave.h used in " __FILE__ "\n\t" \
    	"SAU.SWZ %[varout_], %[varin_], [0123]" \
        : [varout_] "=r" (varout) \
        : [varin_] "r" (varin) \
        : "cc");
    *variableout=varout;
}

/// @brief Swaps endianness of a 32-bit integer (use this preferably on constants, not variables)
/// @param[in] value - u32 integer to be swapped
/// @return swapped integer
static inline u32 swcShaveSwapU32(u32 value)
{
  return ((((u32)((value) & 0x000000FF)) << 24) | \
        ( ((u32)((value) & 0x0000FF00)) <<  8) | \
        ( ((u32)((value) & 0x00FF0000)) >>  8) | \
        ( ((u32)((value) & 0xFF000000)) >> 24));
}

/// @brief Get shave number
/// @return      shave number
///
static inline u32 scGetShaveNumber(void)
{
    return __builtin_shave_getcpuid();
}

/// @brief Get TRF register value
/// @param[in] regNum - TRF register
/// @return Value of TRF register
///
static inline u32 scGetTrfReg(u32 regNum)
{
        u32 local_var_result, local_var_trfBaseAdrS0, local_var_tmp; \
        int regNumClobbered = regNum; \
        asm (\
            "// 'scGetTrfReg()' defined in svuCommonShave.h used in " __FILE__                                       "\n\t" \
            "cmu.cpti %[tmp], P_SVID  || iau.shl %[regNumClobbered], %[regNumClobbered], 2"                          "\n\t" \
            "lsu0.ldil %[trfBaseAdrS0], 0x1100 || lsu1.ldih %[trfBaseAdrS0], 0x20F0   || iau.shl %[tmp], %[tmp], 16" "\n\t" \
            "iau.add %[tmp], %[tmp], %[regNumClobbered]"                                                             "\n\t" \
            "iau.add %[tmp], %[tmp], %[trfBaseAdrS0]"                                                                "\n\t" \
            "lsu0.ld.32 %[tmp], %[tmp]"                                                                              "\n\t" \
            "nop 6"                                                                                                  "\n\t" \
            "cmu.cpii %[result], %[tmp]"                          \
            : [result] "=r" (local_var_result),   /* =&r is not neede because there are no input-only register operands */ \
              [regNumClobbered] "+r" (regNumClobbered),           \
              [trfBaseAdrS0] "=r" (local_var_trfBaseAdrS0),                 \
              [tmp] "=r" (local_var_tmp)                                    \
            :                                                     \
            : "cc", "memory"                                      \
        );                                                            \
        return local_var_result;                                                       \
}

/// @brief Read own window register
/// @param[in] windowdNumber - Window number
/// @return  address
static inline u32 swcReadOwnWindowRegister(u32 windowdNumber)
{
        u32 local_var_result, local_var_tmpAddr; \
        int windowdNumberClobbered = windowdNumber; \
        asm (\
            "// 'swcReadOwnWindowRegister()' defined in svuCommonShave.h used in " __FILE__     "\n\t" \
            "LSU0.LDIL %[tmpAddr] 0x0010 || LSU1.LDIH %[tmpAddr], 0x0FF0"          "\n\t" \
            "IAU.SHL %[windowdNumberClobbered], %[windowdNumberClobbered], 2"      "\n\t" \
            "IAU.ADD %[tmpAddr], %[tmpAddr], %[windowdNumberClobbered]"            "\n\t" \
            "LSU0.LD.32 %[result], %[tmpAddr]"                                     "\n\t" \
            "NOP 6"                   \
            : [result] "=r"(local_var_result) , /* =&r is not neede because there are no input-only register operands */ \
              [tmpAddr] "=r" (local_var_tmpAddr) , \
              [windowdNumberClobbered] "+r" (windowdNumberClobbered) \
            : \
            : "cc", "memory" \
        ); \
        return local_var_result;
}

/// @brief Write own window register
/// @param[in] windowdNumber       - window number to which value will be written
/// @param[in] targetWindowBaseAddr - target window base address
/// @return      void
static inline void swcWriteOwnWindowRegister(u32 windowdNumber, u32 targetWindowBaseAddr)
{
        int local_var_tmpAddr; \
        int windowdNumberClobbered = windowdNumber; \
        asm volatile( \
            "// 'swcWriteOwnWindowRegister()' defined in svuCommonShave.h used in " __FILE__     "\n\t" \
            "LSU0.LDIL %[tmpAddr] 0x0010 || LSU1.LDIH %[tmpAddr], 0x0FF0"          "\n\t" \
            "IAU.SHL %[windowdNumberClobbered], %[windowdNumberClobbered], 2"      "\n\t" \
            "IAU.ADD %[tmpAddr], %[tmpAddr], %[windowdNumberClobbered]"            "\n\t" \
            "LSU0.ST.32 %[targetWindowBaseAddr_] %[tmpAddr]"  \
            : [tmpAddr] "=&r" (local_var_tmpAddr), \
              [windowdNumberClobbered] "+r" (windowdNumberClobbered) \
            : [targetWindowBaseAddr_] "r" (targetWindowBaseAddr) \
            : "cc", "memory" \
    );
}

/// @brief Read PC0
/// @return PC0 value
///
static inline u32 swcReadPC0(void)
{
        u32 local_var_result; \
        asm ( \
            "// 'swcReadPC0()' defined in svuCommonShave.h used in " __FILE__ "\n\t" \
            "lsu0.lda.32 %[result], 4, 0x1830"     "\n\t" \
            "NOP 6" \
            : [result] "=r" (local_var_result) \
            : \
            : "memory" \
        ); \
        return local_var_result;
}

/// @brief Read PC1
/// @return PC1 value
///
static inline u32 swcReadPC1(void)
{
        u32 local_var_result; \
        asm ( \
            "// 'swcReadPC1()' defined in svuCommonShave.h used in " __FILE__ "\n\t" \
            "lsu0.lda.32 %[result], 4, 0x1838"    "\n\t" \
            "NOP 6" \
            : [result] "=r" (local_var_result) \
            : \
            : "memory" \
        ); \
        return local_var_result;
}


// ======== SHAVE MUTEX backward compatibility translation layer ==============

/// @brief Mutex Request
///
/// @note For a detailed explanation, please see the "Mutexes" section in the 
///       MDKMyriad2Programmer_Guide
///
/// @param[in] mutex_num - mutex number requested
///
/// @return     void
///
#define scMutexRequest ShDrvMutexRequest
#define scMutexRequestNoWorkaround ShDrvMutexRequest

/// @brief  This will release mutex
///
/// @note For a detailed explanation, please see the "Mutexes" section in the 
///       MDKMyriad2Programmer_Guide
///
/// @param[in]  mutex_num - mutex number that will be released
///
/// @return     void
///
#define scMutexRelease ShDrvMutexRelease

// ============================================================================

// ============= CMX FIFO backward compatibility translation layer ============

/// @brief Write a 32-bit value to a Shave FIFO
/// @param[in] shaveNr - the shave number which FIFO should be written
/// @param[in] val     - The u32 value that should be written to FIFO
///
#define scFifoWrite ShDrvCmxFifoWriteWord

/// @brief Write a 64-bit value to a Shave FIFO
/// @param[in] shaveNr - the shave number which FIFO should be written
/// @param[in] val     - The u64 value that should be written to FIFO
/// @return void
///
#define scFifoWriteDword ShDrvCmxFifoWriteDWord

/// @brief Read a 64-bit value from the FIFO of the current shave
/// @return The 64-bit value read from FIFO
///
#define scFifoReadDword ShDrvCmxFifoReadDWord

/// @brief Read a 64-bit value from the FIFO of the current shave
/// @param[in] shaveNr - the shave number which FIFO should be read
/// @return The 64-bit value read from FIFO.
///
#define scFifoReadShaveDword ShDrvCmxFifoNReadDWord

/// @brief Write a 64-bit value to a Shave FIFO directly into the FIFO memory.
/// This permits to alter the values already contained in FIFO without 
/// changing the FIFO pointers.
/// @param[in] shaveNr - The shave number which FIFO should be written
/// @param[in] index   - The entry index where the value should be written to.
///                 There is a total of 16 x 64-bit entries for EACH shave, so 
///                 this number shouldn't be bigger than 15.
/// @param[in] val     - The u64 value that should be written to FIFO
/// @return void
///
#define scFifoWriteDirectDword ShDrvCmxFifoWriteDirectDWord

/// @brief Read a 64-bit value from a Shave FIFO directly from the FIFO memory.
///
/// This permits to read the values from FIFO without changing the FIFO 
/// pointers.
/// @param[in]shaveNr -  The shave number which FIFO should be read
/// @param[in]index   - The entry index from where the value should be read.
///                  There is a total of 16 x 64-bit entries for EACH shave, 
///                   so this number shouldn't be bigger than 15.
/// @return The 64-bit value read directly from FIFO.
///
#define scFifoReadDirectDword ShDrvCmxFifoReadDirectDWord

/// @brief Read a 32-bit value from the FIFO of the current shave
/// @return The 32-bit value read from FIFO
#define scFifoRead ShDrvCmxFifoReadWord

/// @brief Read a 32-bit value from the FIFO of the shave given as parameter
/// @param[in] shaveNr - the shave number which FIFO should be read
/// @return The 32-bit value read from FIFO
#define scFifoReadShave ShDrvCmxFifoNReadWord

/// @brief Configures the FIFO status bit to route through to the SHAVE for 
///        direct monitoring.
/// @param[in] shaveNr - The Shave for which to enable the direct monitoring.
/// @param[in] val     - Configuration value.
#define scFifoMonitorSelect ShDrvCmxFifoMonitorSelect

/// @brief Wait for an element in the monitored FIFO
/// @return 
#define scFifoWaitElement ShDrvCmxFifoMonitorWaitElement

/// @brief Control RAM power modes. This permits to switch the FIFO RAM to 
///        different power states.
///
/// This can be achieved by passing an 8-bit value, where the most significant 
/// three bits trigger one of the three power states.
///  - bit 5 - When set, this bit shuts down power to periphery and memory 
///            core, no memory data retention.
///  - bit 6 - When set, this bit triggers the 'Light Sleep' mode: the memory 
///            goes into low leakage mode, there is no change in the output 
///            state.
///  - bit 7 - When set, this bit triggers the 'Deep Sleep' mode: it shuts 
///            down power to periphery and maintains memory contents. The 
///            outputs of the memory are pulled low.
/// @param[in] value - The input value
#define scFifoRAMControl ShDrvCmxFifoRAMControl

/// @brief Get the value that was set for the "Almost full" FIFO fill level
/// @return the 'Almost full' level which is currently set for all the FIFOs
#define scFifoGetAlmostFullLevel ShDrvCmxFifoGetAlmostFullLevel

/// @brief Set the 'Almost full' level for all the FIFOs
/// @param[in] level - The number of elements which can be written in FIFO 
///                    until the 'Almost full' bit is triggered.
///
#define scFifoSetAlmostFullLevel ShDrvCmxFifoSetAlmostFullLevel

/// @brief Get the read pointer value for a specific Shave
/// @param[in] shaveNr - The Shave for which the pointer value should be read
/// @return The read pointer value.
///
#define scFifoGetReadPtrValue ShDrvCmxFifoGetReadPtrValue

/// @brief Get the write pointer value for a specific Shave
/// @param[in] shaveNr - The Shave for which the pointer value should be read
/// @return The write pointer value.
///
#define scFifoGetWritePtrValue ShDrvCmxFifoGetWritePtrValue

/// @brief Get the current fill level for a specific shave
/// @param[in] shaveNr - The Shave for which the fill level should be read
/// @return The number of elements currently available in FIFO
///
#define scFifoGetFillLevel ShDrvCmxFifoGetFillLevel

/// @brief Check whether a FIFO is full or not
/// @param[in] shaveNr -  The Shave for which the FIFO should be checked
/// @return
///  - 0 - The FIFO is full. All the following writes won't have any effect if 
///        the values that already reside there wouldn't be read.
///  - 1 - The FIFO is not full. This doesn't mean that the FIFO is empty.
#define scFifoIsFull ShDrvCmxFifoIsFull

/// @brief Check whether a FIFO has reached the 'almost full' level of filling
/// @param[in] shaveNr -  The Shave for which the FIFO should be checked
/// @return
///  - 0 - The FIFO doesn't have enough elements in the FIFO to trigger the 
///        'Almost full' bit.
///  - 1 - The FIFO has triggered the 'Almost full' level.
#define scFifoIsAlmostFull ShDrvCmxFifoIsAlmostFull

/// @brief Check whether a FIFO is empty
/// @param[in] shaveNr -  The Shave for which the FIFO should be checked
/// @return
///  - 0 - The FIFO is not empty(i.e. there is at least one element in FIFO)
///  - 1 - The FIFO is empty
#define scFifoIsEmpty ShDrvCmxFifoIsEmpty

/// @brief Write a 32-bit value to a Shave FIFO directly into the FIFO memory.
///
/// This permits to alter the values already contained in FIFO without 
/// changing the FIFO pointers.
/// @param[in] shaveNr - The shave number which FIFO should be written
/// @param[in] index   - The entry index where the value should be written to.
///                 There is a total of 16 x 64-bit entries for EACH shave, so 
///                 this number shouldn't be bigger than 15.
/// @param[in] val     - The u32 value that should be written to FIFO
#define scFifoWriteDirectWord ShDrvCmxFifoWriteDirectWord

/// @brief Read a 32-bit value directly from the FIFO memory without affecting 
/// the FIFO pointers.
/// @param[in] shaveNr - The Shave for which the FIFO should be read.
/// @param[in] index   - The entry index from where the value should be read.
///                 There is a total of 16 x 64-bit entries for EACH shave, so 
///                 this number shouldn't be bigger than 15.
/// @return The 32-bit value read directly from FIFO
#define scFifoReadDirectWord ShDrvCmxFifoReadDirectWord

/// @brief Atomic read a 64-bit value from the CMX FIFO of the current shave
/// @param[in] shaveNr - the shave number which FIFO should be read
/// @return The 64-bit value read from FIFO.
///         The upper byte of the return value will be set to 0x00 in case of success,
///         or to 0xFF if the FIFO was empty
///
#define scFifoReadShaveDwordAtomic ShDrvCmxFifoNReadDWordAtomic

// ============================================================================

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// @}
#endif
