///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "DrvCpr.h"
#include "DrvTimer.h"
#include "DrvLeon.h"
#include "app_config.h"
#include "swcLeonUtils.h"
#include "DrvCdma.h"
#include "swcShaveLoader.h"
#include "shared_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define LIST_SIZE              10
#define SRC_MAX_BUFF_SIZE      (32 * 1024)
#define DST_MAX_BUFF_SIZE      (LIST_SIZE * SRC_MAX_BUFF_SIZE)

typedef void *shaveFunc(uint8_t*, uint8_t*, uint32_t);
typedef void *shaveStrideFunc(uint8_t*, uint8_t*, uint32_t, uint32_t,
                              uint32_t, uint32_t, uint32_t);

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern shaveFunc cdma0_cmxDmaSimpleTransaction;
extern shaveFunc cdma1_cmxDmaSimpleTransaction;
extern shaveFunc cdma2_cmxDmaSimpleTransaction;
extern shaveFunc cdma3_cmxDmaSimpleTransaction;
extern shaveFunc cdma4_cmxDmaSimpleTransaction;
extern shaveFunc cdma5_cmxDmaSimpleTransaction;
extern shaveFunc cdma6_cmxDmaSimpleTransaction;
extern shaveFunc cdma7_cmxDmaSimpleTransaction;
extern shaveFunc cdma8_cmxDmaSimpleTransaction;
extern shaveFunc cdma9_cmxDmaSimpleTransaction;
extern shaveFunc cdma10_cmxDmaSimpleTransaction;
extern shaveFunc cdma11_cmxDmaSimpleTransaction;

extern shaveFunc cdma0_cmxDmaListTransaction;
extern shaveFunc cdma1_cmxDmaListTransaction;
extern shaveFunc cdma2_cmxDmaListTransaction;
extern shaveFunc cdma3_cmxDmaListTransaction;
extern shaveFunc cdma4_cmxDmaListTransaction;
extern shaveFunc cdma5_cmxDmaListTransaction;
extern shaveFunc cdma6_cmxDmaListTransaction;
extern shaveFunc cdma7_cmxDmaListTransaction;
extern shaveFunc cdma8_cmxDmaListTransaction;
extern shaveFunc cdma9_cmxDmaListTransaction;
extern shaveFunc cdma10_cmxDmaListTransaction;
extern shaveFunc cdma11_cmxDmaListTransaction;

extern shaveStrideFunc cdma0_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma1_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma2_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma3_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma4_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma5_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma6_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma7_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma8_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma9_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma10_cmxDmaStrideTransaction;
extern shaveStrideFunc cdma11_cmxDmaStrideTransaction;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 __attribute__((section(".cmx_direct.data"))) src_buff[SRC_MAX_BUFF_SIZE];
static u8 __attribute__((section(".ddr_direct.data"))) dst_buff[DST_MAX_BUFF_SIZE];
static DrvCmxDmaTransaction list1[LIST_SIZE] __attribute__((section(".cmx_direct.bss")));
static DrvCmxDmaTransaction list2[LIST_SIZE] __attribute__((section(".cmx_direct.bss")));
static DrvCmxDmaTransactionHnd handle[LIST_SIZE];

// SHAVE data
static uint8_t src_buff_shv[USED_SHAVES_NO][SHV_SRC_BUFF_SIZE] __attribute__((section(".cmx_direct.bss")));
static uint8_t dst_buff_shv[USED_SHAVES_NO][SHV_DST_BUFF_SIZE] __attribute__((section(".ddr_direct.bss")));
static const uint32_t kUsedShave[USED_SHAVES_NO] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static shaveFunc *kFuncTableSimple[USED_SHAVES_NO] = {
    &cdma0_cmxDmaSimpleTransaction,
    &cdma1_cmxDmaSimpleTransaction,
    &cdma2_cmxDmaSimpleTransaction,
    &cdma3_cmxDmaSimpleTransaction,
    &cdma4_cmxDmaSimpleTransaction,
    &cdma5_cmxDmaSimpleTransaction,
    &cdma6_cmxDmaSimpleTransaction,
    &cdma7_cmxDmaSimpleTransaction,
    &cdma8_cmxDmaSimpleTransaction,
    &cdma9_cmxDmaSimpleTransaction,
    &cdma10_cmxDmaSimpleTransaction,
    &cdma11_cmxDmaSimpleTransaction
};

static shaveStrideFunc *kFuncTableStride[USED_SHAVES_NO] = {
    &cdma0_cmxDmaStrideTransaction,
    &cdma1_cmxDmaStrideTransaction,
    &cdma2_cmxDmaStrideTransaction,
    &cdma3_cmxDmaStrideTransaction,
    &cdma4_cmxDmaStrideTransaction,
    &cdma5_cmxDmaStrideTransaction,
    &cdma6_cmxDmaStrideTransaction,
    &cdma7_cmxDmaStrideTransaction,
    &cdma8_cmxDmaStrideTransaction,
    &cdma9_cmxDmaStrideTransaction,
    &cdma10_cmxDmaStrideTransaction,
    &cdma11_cmxDmaStrideTransaction
};

static shaveFunc *kFuncTableList[USED_SHAVES_NO] = {
    &cdma0_cmxDmaListTransaction,
    &cdma1_cmxDmaListTransaction,
    &cdma2_cmxDmaListTransaction,
    &cdma3_cmxDmaListTransaction,
    &cdma4_cmxDmaListTransaction,
    &cdma5_cmxDmaListTransaction,
    &cdma6_cmxDmaListTransaction,
    &cdma7_cmxDmaListTransaction,
    &cdma8_cmxDmaListTransaction,
    &cdma9_cmxDmaListTransaction,
    &cdma10_cmxDmaListTransaction,
    &cdma11_cmxDmaListTransaction
};

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

static void completionCallback(DrvCmxDmaTransactionHnd *handle, void* ctx) {
    UNUSED(handle);
    uint32_t size = (uint32_t)ctx;
    if (memcmp(src_buff, dst_buff, size) == 0)
      printf("transfer finished successfully\n");
    else
      printf("transfer error\n");
}

static void cmxDmaSimpleTransaction(uint32_t transfer_size) {
    fillBuffer(src_buff, transfer_size);

    DrvCmxDmaCreateTransaction(&handle[0], &list1[0],
            src_buff, dst_buff, transfer_size);
    DrvCmxDmaStartTransfer(&handle[0],
        completionCallback, (void*)transfer_size);
    DrvCmxDmaWaitTransaction(&handle[0]);
}

static int32_t cmxDmaStrideTransaction(
    uint32_t src_width, uint32_t dst_width,
    uint32_t src_stride, uint32_t dst_stride,
    uint32_t size) {
  DrvCmxDmaTransactionHnd handle;

  fillBuffer(src_buff, SRC_BUFF_SIZE);
  memset(dst_buff, 0, DST_BUFF_SIZE);

  DrvCmxDmaCreateStrideTransaction(&handle, &list1[0],
      src_buff, dst_buff,
      src_width, dst_width,
      src_stride, dst_stride, size);
  DrvCmxDmaStartTransfer(&handle, NULL, NULL);
  DrvCmxDmaWaitTransaction(&handle);

  return strideCompare(src_buff, dst_buff, src_width, dst_width,
                       src_stride, dst_stride, size);
}

static int32_t cmxDmaListTransaction(uint32_t transfer_size,
    uint32_t list1_size, uint32_t list2_size) {
  uint32_t i;

  uint8_t *ptr_l2_dst;
  fillBuffer(src_buff, transfer_size);
  memset(dst_buff, 0, DST_BUFF_SIZE);
  DrvCmxDmaTransactionHnd hnd1, hnd2;

  // create the first transaction list
  DrvCmxDmaCreateTransaction(&hnd1, &list1[0],
      src_buff, dst_buff, transfer_size);
  for (i = 1; i < list1_size; i++) {
    DrvCmxDmaAddTransaction( &hnd1, &list1[i],
        src_buff, dst_buff + i * transfer_size, transfer_size);
  }

  // create the second transaction list
  ptr_l2_dst = dst_buff + list2_size * transfer_size;
  DrvCmxDmaCreateTransaction(&hnd2, &list2[0],
      src_buff, ptr_l2_dst, transfer_size);
  for (i = 1; i < list2_size; i++) {
    DrvCmxDmaAddTransaction( &hnd2, &list2[i],
        src_buff, ptr_l2_dst + i * transfer_size, transfer_size);
  }

  // link the two lists into a single list
  DrvCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);

  DrvCmxDmaStartTransfer(&hnd1, NULL, NULL);
  DrvCmxDmaWaitTransaction(&hnd1);

  for (i = 0; i < list1_size + list2_size; i++)
    if (memcmp(src_buff, dst_buff + i * transfer_size, transfer_size) != 0)
      return -1;
  return 0;
}

// SHAVE related functions
static void startShaveTests(shaveFunc *func_table[]) {
    uint32_t i;

    // intialize source and destination buffers
    for (i = 0; i < USED_SHAVES_NO; i++) {
        fillBuffer(src_buff_shv[i], SHV_SRC_BUFF_SIZE);
        memset(dst_buff_shv[i], 0xFF, SHV_DST_BUFF_SIZE);
    }
    for (i = 0; i < USED_SHAVES_NO; i++)
    {
        swcResetShave(kUsedShave[i]);
        swcSetAbsoluteDefaultStack(kUsedShave[i]);
        swcStartShaveCC(kUsedShave[i], (uint32_t)func_table[i], "iii",
                (uint32_t)src_buff_shv[i], (uint32_t)dst_buff_shv[i], SHV_TRANSFER_SIZE);
    }
    for (i = 0; i < USED_SHAVES_NO; i++) {
        swcWaitShave(kUsedShave[i]);
    }
}

static void shaveSimpleTransfersCheck(void) {
    uint32_t i;

    for (i = 0; i < USED_SHAVES_NO; i++) {
        printf("SHAVE: %"PRIu32": ", kUsedShave[i]);
        if (memcmp(src_buff_shv[i], dst_buff_shv[i], SHV_TRANSFER_SIZE) == 0)
          printf("transfer completed successfully\n");
        else
          printf("transfer error\n");
    }
}

static void shaveListTransfersCheck(void) {
    uint32_t i, j;
    int32_t err_cnt;

    for (i = 0; i < USED_SHAVES_NO; i++) {
        err_cnt = 0;
        printf("SHAVE: %"PRIu32": ", kUsedShave[i]);
        for (j = 0; j < 2 * LIST_SIZE; j++) {
            if (memcmp(src_buff_shv[i], dst_buff_shv[i] + j * SHV_TRANSFER_SIZE,
                  SHV_TRANSFER_SIZE) != 0)
            err_cnt++;
        }
        if (err_cnt > 0)
          printf("transfer error\n");
        else
          printf("transfer completed successfully\n");
    }
}

static void shaveStrideTest(void){
    uint32_t i;

    // intialize source and destination buffers
    for (i = 0; i < USED_SHAVES_NO; i++) {
        fillBuffer(src_buff_shv[i], SHV_SRC_BUFF_SIZE);
        memset(dst_buff_shv[i], 0xFF, SHV_DST_BUFF_SIZE);
    }
    for (i = 0; i < USED_SHAVES_NO; i++)
    {
        swcResetShave(kUsedShave[i]);
        swcSetAbsoluteDefaultStack(kUsedShave[i]);
        swcStartShaveCC(kUsedShave[i], (uint32_t)kFuncTableStride[i], "iiiiiii",
                (uint32_t)src_buff_shv[i], (uint32_t)dst_buff_shv[i],
                SHV_TRANSFER_SIZE / 4, SHV_TRANSFER_SIZE / 8,
                SHV_TRANSFER_SIZE / 4, SHV_TRANSFER_SIZE / 8,
                SHV_TRANSFER_SIZE);
    }
    for (i = 0; i < USED_SHAVES_NO; i++) {
        swcWaitShave(kUsedShave[i]);
    }
    for (i = 0; i < USED_SHAVES_NO; i++) {
      printf("SHAVE: %"PRIu32": ", kUsedShave[i]);
      if (strideCompare(
            src_buff_shv[i], dst_buff_shv[i],
            SHV_TRANSFER_SIZE / 4, SHV_TRANSFER_SIZE / 8,
            SHV_TRANSFER_SIZE / 4, SHV_TRANSFER_SIZE / 8,
            SHV_TRANSFER_SIZE) == 0)
          printf("transfer completed successfully\n");
      else
          printf("transfer error\n");
    }
}

int main(void)
{
    int32_t status;

    status = initClocksAndMemory();
    if(status)
        return status;
    swcLeonSetPIL(0);

    DrvLeonRTSignalBootCompleted();

    status = DrvCmxDmaInitialize(NULL);

    if (status == MYR_DRV_SUCCESS) {
      printf("Running CMX DMA transfers on LRT\n\n");

      printf("CMX DMA simple transfer: ");
      if (status == MYR_DRV_SUCCESS)
        cmxDmaSimpleTransaction(SRC_BUFF_SIZE);

      printf("CMX DMA simple stride transfer: ");
      if (cmxDmaStrideTransaction(
            TRANSFER_SIZE / 2, TRANSFER_SIZE / 4,
            TRANSFER_SIZE / 2, TRANSFER_SIZE / 2,
            TRANSFER_SIZE) == 0)
        printf("transfer completed successfully\n");
      else
        printf("transfer error\n");

      printf("CMX DMA list transfer: ");
      if (cmxDmaListTransaction(TRANSFER_SIZE, LIST_SIZE / 2, LIST_SIZE / 2) == 0)
        printf("transfer completed successfully\n");
      else
        printf("transfer error\n");

      // SHAVE tests
      printf("Running CMX DMA transfers on SHAVEs\n\n");
      printf("Simple transfer on SHAVEs\n");
      startShaveTests(kFuncTableSimple);
      shaveSimpleTransfersCheck();

      printf("\nStride transfer on SHAVEs\n");
      shaveStrideTest();

      printf("\nList transfer on SHAVEs\n");
      startShaveTests(kFuncTableList);
      shaveListTransfersCheck();
    }
    else {
      printf("CMX DMA initialization error\n");
    }

    DrvLeonRTSignalStop();
    return 0;
}

