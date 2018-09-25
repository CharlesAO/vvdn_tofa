///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Application shared configuration
///
///

#ifndef SHARED_CONFIG_H
#define SHARED_CONFIG_H

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define LIST_NO            2
#define LIST_SIZE          10
#define TRANSFER_SIZE      16 * 1024
#define SRC_BUFF_SIZE      TRANSFER_SIZE
#define DST_BUFF_SIZE      (LIST_NO * LIST_SIZE * SRC_BUFF_SIZE)

#define USED_SHAVES_NO     12

#define SHV_LIST_NO        3
#define SHV_LIST_SIZE      16
#define SHV_TRANSFER_SIZE  8 * 1024
#define SHV_SRC_BUFF_SIZE  SHV_TRANSFER_SIZE
#define SHV_DST_BUFF_SIZE  (SHV_LIST_NO * SHV_LIST_SIZE * SHV_TRANSFER_SIZE)

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#endif // SHARED_CONFIG_H
