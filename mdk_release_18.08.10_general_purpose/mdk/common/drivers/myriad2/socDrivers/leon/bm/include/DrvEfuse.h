///
/// @file DrvEfuse.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvEfuse Efuse Driver
/// @{
/// @brief Macros and functions for the Efuse Driver.
///

#ifndef DRV_EFUSE_H_
#define DRV_EFUSE_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include "DrvEfuseDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Defines
// ----------------------------------------------------------------------------
#define EFUSE_NUM_BITS              1024    // efuse array bits
#define EFUSE_NUM_RED_BITS          4       // redundancy setttings (slots)

// definitions below provide a higher-level interface to the EFuse driver

#define DRVEFUSE_FLAG_READBACK      (1 << 0)    // fill input set (values) with result after programming
#define DRVEFUSE_FLAG_NO_VERIFY     (1 << 1)    // default is to verify EFuses at end. this flag disables this behaviour
#define DRVEFUSE_FLAG_REDUNDANCY    (1 << 2)    // automatically fill redudancy setting (if available)

#define DRVEFUSE_LOCK_TOKEN         0xbaadf00d  // to avoid possibility of accidental locking of redudancy bits

#define DRVEFUSE_SUCCESS            0   // operation was successful
#define DRVEFUSE_NO_RED             -1  // no redudancy setting available
#define DRVEFUSE_INVALID_SYS_FREQ   -2  // system frequency is too small for accurate timings generation
#define DRVEFUSE_PWMERR_REDFAILED   -3  // attempt to program redudancy setting failed
#define DRVEFUSE_PWMERR_FAILVERIFY  -4  // verify failed
#define DRVEFUSE_LOCKRED_BADTOKEN   -5  // an incorrect token value was provided
#define DRVEFUSE_FIX_FAILED         -6  // array bit fix operation failed
#define DRVEFUSE_INPUT_ERROR        -7  // incorrect parameters

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
struct _efuseSet {
    u8 values[EFUSE_NUM_BITS/8];
} __attribute__ ((aligned (sizeof(u32))));
typedef struct _efuseSet efuseSet;

typedef struct _efuseMaskSet {
    efuseSet values;
    efuseSet mask;
} efuseMaskSet;

typedef struct _rfuse {
    uint16_t enabled:1;
    uint16_t val:1;
    uint16_t addr:10;
    uint16_t reserved:3;
    uint16_t disabled:1;
} __attribute__ ((aligned (sizeof(u16)))) rFuse;

typedef struct _redFuseSet {
    rFuse values[EFUSE_NUM_RED_BITS];
} redFuseSet;

typedef enum
{
    // read fuses without margin
    NORMAL_MODE = 0,
    // read fuses with margin
    MARGIN_MODE = 1 
} efuseMargin_t;

typedef enum
{
    // do not apply redundancy fix
    NO_FIX = 0, 
    // apply redundancy fix
    APPLY_FIX = 1 
} efuseFix_t;

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
/// Program EFuses with a prepared efuseMaskSet
/// 
/// All bits that have their mask bit set to 1 are programmed.
/// If the DRVEFUSE_FLAG_REDUNDANCY flag is specified, any available redudancy bits
/// will be used to correct bits that need to transition from a 1 to a 0.
/// Additionally, after all retries, if the DRVEFUSE_FLAG_REDUNDANCY flag is
/// specified, any available redudancy bits will be used to correct bits that need
/// to transition from a 0 to a 1.
/// If no redudancy bits are available, or if the DRVEFUSE_FLAG_REDUNDANCY flag is
/// not specified and the correct values cannot be programmed without using redudancy
/// bits then an error will be returned.
/// Unless the DRVEFUSE_FLAG_NO_VERIFY flag is specified, a final verification pass
/// will check the final programmed values against the desired values.
/// If the DRVEFUSE_FLAG_READBACK flag is specified, the entire set of EFuses will
/// be read into the provided efuseMaskSet (the mask bits will be unchanged).
/// @param[in] maskSet - A pointer to the efuseMaskSet to use.
/// @param[in] flags   - 0 or a bitwise OR of one or more DRVEFUSE_FLAG_xxx values.
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ - the system frequency is not supported.
/// @return    DRVEFUSE_NO_RED - no redudancy bit settings available to perform the redundancy fix
/// @return    DRVEFUSE_PWMERR_REDFAILED - attempt to program redundancy setting failed
/// @return    DRVEFUSE_PWMERR_FAILVERIFY - verification failed
/// @return    DRVEFUSE_INPUT_ERROR - input parameters are not correct
int DrvEfuseProgramWithMask(efuseMaskSet *maskSet, u32 flags);
/// Read all EFuses into an efuseSet
/// 
/// All EFuses are read and the provided efuseSet is filled with the values read.
/// To read into an efuseMaskSet structure, pass a pointer to the values member.
/// @param[out] fSet - A pointer to the efuseSet structure to be filled.
/// @param[in] fix - Apply redundancy corrections to the outputed fSet
///     (0 = not corrected, 1 = corrected).
/// @param[in] margin - Read type (NO_MARGIN or MARGIN).
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ  - the system frequency is not supported.
/// @return    DRVEFUSE_INPUT_ERROR - input parameters are not correct
int DrvEfuseReadEFuses(efuseSet *fSet, efuseFix_t fix, efuseMargin_t margin);
/// Read all redundancy fuses into an redFuseSet
/// 
/// All redundancy fuses are read and the provided redFuseSet is filled with the values read.
/// @param[out] rSet - A pointer to the redFuseSet structure to be filled with
///     redundancy setting data.
/// @param[in] margin - Read type (NO_MARGIN or MARGIN).
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ  - the system frequency is not supported.
/// @return    DRVEFUSE_INPUT_ERROR - input parameters are not correct
int DrvEfuseReadRedundancy(redFuseSet *rSet, efuseMargin_t margin);
/// All unused redudancy bits are disabled by programming their disabled bit.
/// 
/// This function can be used to ensure that the EFuses cannot be changed using
/// redudancy bits. It is recommended that this is done after programming the
/// EFuses for Secure Boot.
/// The token is to avoid the possibility of accidentally locking the EFuses.
/// @param[in] token - This must have the value DRVEFUSE_LOCK_TOKEN.
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ  - the system frequency is not supported,
/// @return    DRVEFUSE_LOCKRED_BADTOKEN  - the token value was incorrect
int DrvEfuseLockRedundancy(u32 token);
/// Fix an eFuse array bit at a specific address.
/// 
/// This function can fix (override programmed value) for an eFuse array bit at
/// a specific address by assigning new value.
/// @param[in] setting_index - Redundancy setting index (0 <= setting_index < EFUSE_NUM_RED_BITS)
/// or first available (-1).
/// @param[in] address - Address of the bit to be fixed.
/// @param[in] value - Value to be assigned to the fixed bit.
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ  - the system frequency is not supported,
/// @return    DRVEFUSE_NORED         - the specified index is not available
/// @return    DRVEFUSE_FIX_FAILED - fix attempted but failed. Specified index setting disabled.
/// @return    DRVEFUSE_INPUT_ERROR - input parameters are not correct
int DrvEfuseFixBitUsingRedundancy(int setting_index, int address, int value);
/// Disable a specific redundancy setting.
/// 
/// This function can disable a specific redundancy setting (one of the four)
/// @param[in] setting_index - Redundancy setting index (0 <= setting_index < EFUSE_NUM_RED_BITS).
/// @return    DRVEFUSE_SUCCESS - operation was successful
/// @return    DRVEFUSE_INVALID_SYS_FREQ  - the system frequency is not supported
/// @return    DRVEFUSE_INPUT_ERROR - input parameter is not correct
int DrvEfuseDisableRedundancySetting(int setting_index);

// efuseSet and efuseMaskSet data manipulation helper functions
// ----------------------------------------------------------------------------

/// Copy bits within a range from one efuseSet to another.
///
/// efuseMaskSet contents can also be copied by passing pointers to either the
/// values or mask members.
/// @param[in] dst      - A pointer to the destination efuseSet
/// @param[in] src      - A pointer to the source efuseSet. This set will not be modified.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
void DrvEfuseCopy(efuseSet *dst, const efuseSet *src, u32 startBit, u32 endBit);
/// Zeros the contents of an efuseSet
/// 
/// @param[in] set - A pointer to the efuseSet to clear
void DrvEfuseClearSet(efuseSet *set);
/// Zeros the contents of an efuseMaskSet
/// 
/// @param[in] maskSet - A pointer to the efuseMaskSet to clear
void DrvEfuseClearMaskSet(efuseMaskSet *maskSet);
/// Sets the mask bits of a range within an efuseMaskSet to the specified value
/// 
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] state    - If zero, the mask bits will be cleared. If non-zero, the mask bits will be set.
void DrvEfuseSetMask(efuseMaskSet *maskSet, u32 startBit, u32 endBit, int state);
/// Set or clears a single bit within an efuseSet
/// 
/// @param[in] set      - A pointer to the efuseSet to modify.
/// @param[in] bitIndex - The bit position to modify.
/// @param[in] value    - If zero, the bit will be cleared. If non-zero, the bit will be set.
void DrvEfuseSetValue1(efuseSet *set, u32 bitIndex, u32 value);
/// Copies a 32-bit value into an efuseSet
/// 
/// Bit 0 of the value will go into startBit. The range must be 32 bits or smaller.
/// @param[in] set      - A pointer to the efuseSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - The 32-bit value to copy.
void DrvEfuseSetValue32(efuseSet *set, u32 startBit, u32 endBit, u32 value);
/// Copies a 64-bit value into an efuseSet
/// 
/// Bit 0 of the value will go into startBit. The range must be 64 bits or smaller.
/// The function is for use with the 64-bit IFCFG field.
/// @param[in] set      - A pointer to the efuseSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - The 64-bit value to copy.
void DrvEfuseSetValue64(efuseSet *set, u32 startBit, u32 endBit, u64 value);
/// Copies a byte array into an efuseSet
/// 
/// Bit 0 of the first byte will go into startBit. Bit 0 of the second byte will go
/// into (startBit+8).
/// @param[in] set      - A pointer to the efuseSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - A pointer to the byte array to copy. This array will not be modified.
void DrvEfuseSetValuePtr(efuseSet *set, u32 startBit, u32 endBit, const u8 *value);
/// Copies a byte array into an efuseSet in reverse
/// 
/// Bit 7 of the first byte will go into endBit. Bit 7 of the second byte will go
/// into (endBit-0). The byte order is reversed but the bit order is maintained.
/// This function is for use with the byte-reversed AES_KEY field.
/// @param[in] set      - A pointer to the efuseSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - A pointer to the byte array to copy. This array will not be modified.
void DrvEfuseSetValuePtrRev(efuseSet *set, u32 startBit, u32 endBit, const u8 *value);
/// Set or clears a single bit within an efuseMaskSet, additionally setting the mask bit
/// 
/// Regardless of the value parameter, the mask bit at the specified bit index will
/// be set to 1.
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] bitIndex - The bit position to modify.
/// @param[in] value    - If zero, the bit will be cleared. If non-zero, the bit will be set.
void DrvEfuseSetValueMask1(efuseMaskSet *maskSet, u32 bitIndex, u32 value);
/// Copies a 32-bit value into an efuseMaskSet, additionally settings mask bits for the given range
/// 
/// Bit 0 of the value will go into startBit. The range must be 32 bits or smaller.
/// Regardless of the value parameter, the mask bits for the specified range will
/// all be set to 1.
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - The 32-bit value to copy.
void DrvEfuseSetValueMask32(efuseMaskSet *maskSet, u32 startBit, u32 endBit, u32 value);
/// Copies a 64-bit value into an efuseMaskSet, additionally setting mask bits for the given range
/// 
/// Bit 0 of the value will go into startBit. The range must be 64 bits or smaller.
/// Regardless of the value parameter, the mask bits for the specified range will
/// all be set to 1.
/// The function is for use with the 64-bit IFCFG field.
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - The 64-bit value to copy.
void DrvEfuseSetValueMask64(efuseMaskSet *maskSet, u32 startBit, u32 endBit, u64 value);
/// Copies a byte array into an efuseMaskSet, additionally setting mask bits for the given range
/// 
/// Bit 0 of the first byte will go into startBit. Bit 0 of the second byte will go
/// into (startBit+8).
/// Regardless of the input array values, the mask bits for the specified range will
/// all be set to 1.
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - A pointer to the byte array to copy. This array will not be modified.
void DrvEfuseSetValueMaskPtr(efuseMaskSet *maskSet, u32 startBit, u32 endBit, const u8 *value);
/// Copies a byte array into an efuseMaskSet in reverse, additionally setting mask bits for the given range
/// 
/// Bit 7 of the first byte will go into endBit. Bit 7 of the second byte will go
/// into (endBit-0). The byte order is reversed but the bit order is maintained.
/// Regardless of the input array values, the mask bits for the specified range will
/// all be set to 1.
/// This function is for use with the byte-reversed AES_KEY field.
/// @param[in] maskSet  - A pointer to the efuseMaskSet to modify.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - A pointer to the byte array to copy. This array will not be modified.
void DrvEfuseSetValueMaskPtrRev(efuseMaskSet *maskSet, u32 startBit, u32 endBit, const u8 *value);

/// Retrieves the value for a single bit value from an efuseSet
/// 
/// @param[in] set      - A pointer to the efuseSet to read from. This set will not be modified.
/// @param[in] bitIndex - The bit position to read.
/// @return    0 if the bit is clear, non-zero if the bit is set.
u32 DrvEfuseGetValue1(const efuseSet *set, u32 bitIndex);
/// Retrieves the value for a range of up to 32-bits from an efuseSet
/// 
/// Bit 0 of the return value will contain the value of the bit at startBit in the
/// provided efuseSet.
/// To retrieve the value for a range larger than 32-bits, use the DrvEfuseGetValue64()
/// or DrvEfuseGetValuePtr() functions.
/// @param[in] set      - A pointer to the efuseSet to read from. This set will not be modified.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @return    The value read from the efuseSet
u32 DrvEfuseGetValue32(const efuseSet *set, u32 startBit, u32 endBit);
/// Retrieves the value for a range of up to 64-bits from an efuseSet
/// 
/// Bit 0 of the return value will contain the value of the bit at startBit in the
/// provided efuseSet.
/// To retrieve the value for a range larger than 64-bits DrvEfuseGetValuePtr() function.
/// @param[in] set      - A pointer to the efuseSet to read from. This set will not be modified.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @return    The value read from the efuseSet
u64 DrvEfuseGetValue64(const efuseSet *set, u32 startBit, u32 endBit);
/// Retrieves the value for a range of up to 64-bits from an efuseSet
/// 
/// Bit 0 of the first byte in the output array will contain the value of the bit
/// at startBit in the provided efuseSet.
/// The byte array must be large enough to contain the entire range. The
/// EFUSE_FIELD_SIZEOF macro can be used to calculate this size.
/// @param[in] set      - A pointer to the efuseSet to read from. This set will not be modified.
/// @param[in] startBit - The bit position of the start of the range (inclusive).
/// @param[in] endBit   - The bit position of the end of the range (inclusive).
/// @param[in] value    - The byte array to fill.
void DrvEfuseGetValuePtr(const efuseSet *set, u32 startBit, u32 endBit, u8 *value);

/// Compares the contents of two efuseSets
/// 
/// If the outSet parameter is non-NULL, this efuseSet has all bits set to 0
/// where set1 and set2 match, and all bits set to 1 where set1 and set2 differ.
/// @param[in] set1   - A pointer to the first set to compare.
/// @param[in] set2   - A pointer to the second set to compare.
/// @param[in] outSet - An optional pointer to an efuseSet to fill with the deltas. 
/// @return    0 if the sets match, non-zero if the sets are different.
int DrvEfuseCompareSets(const efuseSet *set1, const efuseSet *set2, efuseSet *outSet);

// calculates the number of bytes needed for a range given the start and end bit indices (inclusive)
#define EFUSE_FIELD_SIZEOF(startBit, endBit)    (((endBit) - (startBit) + 8) / 8)

#ifdef __cplusplus
}
#endif


#endif

///@}
