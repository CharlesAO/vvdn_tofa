#ifndef __KERNEL_CONFIG_H__
#define __KERNEL_CONFIG_H__

// Frame settings
#define FULL_TEST_FRAME_WIDTH       1920
#define FULL_TEST_FRAME_HEIGHT      1080
#define FRAME_SIZE                  (FULL_TEST_FRAME_WIDTH * FULL_TEST_FRAME_HEIGHT * 3/2)

// Kernel settings
#define LINES_NUM   7
#define MAXWIDTH    FULL_TEST_FRAME_WIDTH

#define COLUMN_PADDING          8
#define LINE_PADDING            7
#define UPPER_LINE_PADDING      (LINE_PADDING/2)

#define FULLWIDTH               (MAXWIDTH + COLUMN_PADDING)

#define KERNEL_SIZE             7
#define NUMBER_KERNELS          3
// Sections
#define CMX_DMA     __attribute__((section(".cmx.cdmaDescriptors")))
#define CMX_DATA    __attribute__((section(".cmx.data")))
#define DDR_BUF     __attribute__((section(".ddr.bss"))) 

#endif
