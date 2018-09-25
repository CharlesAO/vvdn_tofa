///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave source code
///

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "scCmxDma.h"
#include "svuCommonShave.h"
#include "swcWhoAmI.h"
#include "shared_config.h"

#define SHAVE_ITER_NUM 100

ScCmxDmaTransaction list1[SHV_LIST_SIZE];
ScCmxDmaTransaction list2[SHV_LIST_SIZE];

void Entry(void){}

__attribute__((dllexport))
void cmxDmaSimpleTransaction(uint8_t *src_buff, uint8_t *dst_buff, uint32_t size) {
  ScCmxDmaTransactionHnd handle;

  ScCmxDmaInitialize(NULL);
  ScCmxDmaCreateTransaction(&handle, &list1[0], src_buff, dst_buff, size);
  ScCmxDmaStartTransfer(&handle);
  ScCmxDmaWaitTransaction(&handle);

  SHAVE_HALT;
}

__attribute__((dllexport))
int32_t cmxDmaStrideTransaction(
    uint8_t *src_buff, uint8_t *dst_buff,
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t size) {
  ScCmxDmaTransactionHnd handle;

  ScCmxDmaCreateStrideTransaction(&handle, &list1[0],
      src_buff, dst_buff,
      src_width, dst_width,
      src_stride, dst_stride, size);
  ScCmxDmaStartTransfer(&handle);
  ScCmxDmaWaitTransaction(&handle);

  SHAVE_HALT;
}

__attribute__((dllexport))
void cmxDmaListTransaction(uint8_t *src_buff, uint8_t *dst_buff, uint32_t size) {
  uint32_t i;
  ScCmxDmaTransactionHnd l1;
  ScCmxDmaTransactionHnd l2;

  ScCmxDmaInitialize(NULL);
  ScCmxDmaCreateTransaction(&l1, &list1[0],
      src_buff, dst_buff, size);
  ScCmxDmaCreateTransaction(&l2, &list2[0],
      src_buff, dst_buff + SHV_LIST_SIZE * size, size);

  for (i = 1; i < SHV_LIST_SIZE; i++) {
    ScCmxDmaAddTransaction(&l1, &list1[i],
        src_buff, dst_buff + i * size, size);
    ScCmxDmaAddTransaction(&l2, &list2[i],
        src_buff, dst_buff + (SHV_LIST_SIZE * size) + i * size, size);
  }

  ScCmxDmaStartTransfer(&l1);
  ScCmxDmaStartTransfer(&l2);

  ScCmxDmaWaitTransaction(&l1);
  ScCmxDmaWaitTransaction(&l2);

  SHAVE_HALT;
}

