///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup MemoryTransfer Memory Transfer 
/// @{
/// @brief    Memory Transfer module API
///
/// Used for manipulating memory transfers
//


#ifndef _SWC_MEMORY_TRANSFER_H_
#define _SWC_MEMORY_TRANSFER_H_

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Function that copies from source to destination
/// @param[in] - Destination address
/// @param[in] - Source address
/// @param[in] - Length to copy
/// @return void
///
void swcU32memcpy(u32* dst, u32* src, u32 len);

/// @brief Function that sets memory with a givven value
/// @param[in] - Destination address
/// @param[in] - Length to copy
/// @param[in] - Value to set
/// @return void
///
void swcU32memsetU32(u32 *addr, u32 lenB, u32 val);
/// @}
#ifdef __cplusplus
}
#endif

#endif /* _SWC_MEMORY_TRANSFER_H_ */
