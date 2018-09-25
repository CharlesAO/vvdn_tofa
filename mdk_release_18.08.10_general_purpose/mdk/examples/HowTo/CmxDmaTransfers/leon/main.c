///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief CMX DMA example
///
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "DrvCpr.h"
#include "DrvLeon.h"
#include "OsDrvCdma.h"
#include "rtems_config.h"
#include "shared_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define NO_WAIT 0
#define WAIT    1

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
//Entry point of the leon_rt processor
extern u32 *lrt_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static uint8_t __attribute__((section(".cmx_direct.data"))) src_buff[SRC_BUFF_SIZE];
static uint8_t __attribute__((section(".ddr_direct.data"))) dst_buff_1[DST_BUFF_SIZE];
static u8 __attribute__((section(".ddr_direct.data"))) dst_buff_2[DST_BUFF_SIZE];
static OsDrvCmxDmaTransactionHnd l1, l2;
static OsDrvCmxDmaTransaction list1[LIST_SIZE] __attribute__((section(".cmx_direct.bss")));
static OsDrvCmxDmaTransaction list2[LIST_SIZE] __attribute__((section(".cmx_direct.bss")));

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static void fillBuffer(uint8_t *buff, uint32_t size) {
  uint32_t i;

  for (i = 0; i < size; i++) {
    buff[i] = i % 256;
  }
}

static int32_t strideCompare(
        uint8_t *src, uint8_t *dst,
        uint32_t src_width, uint32_t dst_width,
        uint32_t src_stride, uint32_t dst_stride,
        uint32_t size) {
    uint32_t src_idx = 0;
    uint32_t dst_idx = 0;
    uint8_t *src_ptr = src;
    uint8_t *dst_ptr = dst;
    do {
        if (src_ptr[src_idx] != dst_ptr[dst_idx]) {
            printf("First error: srcIdx %"PRIu32" dstIdx %"PRIu32
                   " expected %d found %d\n",
                    src_idx, dst_idx, src_ptr[src_idx], dst_ptr[dst_idx]);
            return -1;
        }
        src_idx++;
        if (src_idx % src_width == 0) {
            src_idx = 0;
            src_ptr += src_stride;
        }
        dst_idx++;
        if (dst_idx % dst_width == 0) {
            dst_idx = 0;
            dst_ptr += dst_stride;
        }
        size--;
    } while (size > 0);
    return 0;
}

static int32_t cmxDmaSimpleTransaction(uint32_t transfer_size) {
  fillBuffer(src_buff, transfer_size);
  memset(dst_buff_1, 0, DST_BUFF_SIZE);
  OsDrvCmxDmaCreateTransaction(&l1, &list1[0],
      src_buff, dst_buff_1, transfer_size);
  OsDrvCmxDmaStartTransfer(&l1, WAIT);

  if (memcmp(src_buff, dst_buff_1, transfer_size) == 0)
    return 0;
  else
    return -1;
}

static int32_t cmxDmaStrideTransaction(
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t size) {
  OsDrvCmxDmaTransactionHnd handle;

  fillBuffer(src_buff, SRC_BUFF_SIZE);
  memset(dst_buff_1, 0, DST_BUFF_SIZE);

  OsDrvCmxDmaCreateStrideTransaction(&handle, &list1[0],
      src_buff, dst_buff_1,
      src_width, dst_width,
      src_stride, dst_stride, size);
  OsDrvCmxDmaStartTransfer(&handle, WAIT);

  return strideCompare(src_buff, dst_buff_1, src_width, dst_width,
                       src_stride, dst_stride, size);
}

static int32_t cmxDmaListTransaction(uint32_t transfer_size,
    uint32_t list1_size, uint32_t list2_size) {
  uint32_t i;

  fillBuffer(src_buff, transfer_size);
  memset(dst_buff_1, 0, DST_BUFF_SIZE);
  memset(dst_buff_2, 0, DST_BUFF_SIZE);

  //create the first transaction list
  OsDrvCmxDmaCreateTransaction(&l1, &list1[0],
      src_buff, dst_buff_1, transfer_size);
  for (i = 1; i < list1_size; i++) {
    OsDrvCmxDmaAddTransaction( &l1, &list1[i],
        src_buff, dst_buff_1 + i * transfer_size, transfer_size);
  }

  //create the second transaction list
  OsDrvCmxDmaCreateTransaction(&l2, &list2[0],
      src_buff, dst_buff_2, transfer_size);
  for (i = 1; i < list2_size; i++) {
    OsDrvCmxDmaAddTransaction( &l2, &list2[i],
        src_buff, dst_buff_2 + i * transfer_size, transfer_size);
  }

  OsDrvCmxDmaStartTransfer(&l1, WAIT);
  OsDrvCmxDmaStartTransfer(&l2, WAIT);

  for (i = 0; i < list1_size; i++)
    if (memcmp(src_buff, dst_buff_1 + i * transfer_size, transfer_size) != 0)
      return -1;
  for (i = 0; i < list2_size; i++)
    if (memcmp(src_buff, dst_buff_2 + i * transfer_size, transfer_size) != 0)
      return -1;
  return 0;
}

void *POSIX_Init (void *args) {
  UNUSED(args);

  int32_t sc;
  int32_t status;

  sc = initClocksAndMemory();
  if(sc)
    exit(sc);

  DrvLeonRTStartup((u32)&lrt_start);

  if (sc)
    exit(sc);

  status = OsDrvCmxDmaInitialize(NULL);

  if (status == OS_MYR_DRV_SUCCESS) {
    printf("Running CMX DMA transfers on LOS\n\n");
    printf("CMX DMA simple transfer: ");
    if (cmxDmaSimpleTransaction(TRANSFER_SIZE) == 0)
      printf("transfer completed successfully\n");
    else
      printf("transfer error\n");

    printf("CMX DMA simple stride transfer: ");
    if (cmxDmaStrideTransaction(
        TRANSFER_SIZE / 2, TRANSFER_SIZE / 4,
        TRANSFER_SIZE / 2, TRANSFER_SIZE / 2,
        TRANSFER_SIZE) == 0)
      printf("transfer completed successfully\n");
    else
      printf("transfer error\n");

    printf("CMX DMA list transfer: ");
    if (cmxDmaListTransaction(TRANSFER_SIZE, LIST_SIZE, LIST_SIZE / 2) == 0)
      printf("transfer completed successfully\n");
    else
      printf("transfer error\n");
  }
  else {
    printf("CMX DMA initialization error\n");
  }

  DrvLeonRTWaitForBoot();

  DrvLeonRTWaitExecution();
  exit(0);
}

#pragma GCC diagnostic pop
