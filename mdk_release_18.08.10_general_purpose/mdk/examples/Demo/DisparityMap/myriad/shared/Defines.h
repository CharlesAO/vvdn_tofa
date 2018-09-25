#ifndef __DEFINES_SPARSE_H__
#define __DEFINES_SPARSE_H__

// Image size
#define MAX_WIDTH		640
#define MAX_HEIGHT		480

// Number of SHAVEs used
#define SHAVES_USED     4

// Patches configuration
#define MAX_PATCHES         4
#define PATCH_W_OVERLAP     0
#define PATCH_H_OVERLAP     (2*MEDIAN_KERNEL_SIZE) // minimum overlap needed
#define PATCH_WIDTH         MAX_WIDTH
#define PATCH_HEIGHT        130

// Unit test CRC for VGA static images from Middleburry dataset (Input/inputFrameLeft_640x480.bin and Input/inputFrameRight_640x480.bin)
#define GOLDEN_CRC			480864087
#define GOLDEN_CYCLES		22109822 //21869420
#define CYCLES_TOLERANCE	5

// Padding
#define MAX_PADDING_8       7   // maximum padding needed to align each line to 8 bytes
#define MAX_PADDING_16      15  // maximum padding needed to align each line to 16 bytes
#define BASE_BUFFER_DISPARITIES_SIZE ( (MAX_WIDTH+MAX_PADDING_8)*MEDIAN_KERNEL_SIZE )

// PC - Myriad2 defines
#ifdef __PC__
 #define ALIGNED(x)
#else
 #define ALIGNED(x) __attribute__((aligned(x)))
#endif

#endif
