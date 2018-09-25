/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

#define SIPP_HW_SHARPEN_BUFFER_WIDTH       512
#define SIPP_HW_SHARPEN_BUFFER_HEIGHT      512
#define SIPP_HW_SHARPEN_BUFFER_NUM_PLANES  1
#define SIPP_HW_SHARPEN_BUFFER_SIZE     (SIPP_HW_SHARPEN_BUFFER_WIDTH * SIPP_HW_SHARPEN_BUFFER_HEIGHT * SIPP_HW_SHARPEN_BUFFER_NUM_PLANES)

#define SHARPENING 0
#define BLURRING   1

#define KERNEL_SIZE         7
#define OUTPUT_CLAMP        0
#define MODE                SHARPENING
#define OUTPUT_DELTAS       0
#define THR                 0x211e

#define STRENGTH_DARKEN     0x3999   // 0.7f
#define STRENGTH_LIGHTEN    0x3d33   // 1.3f

#define CLIPPING_ALPHA      0x3c00   // 1.0f

#define LIMIT               8
#define OVERSHOOT           0x3c51   // (1 + LIMIT/100.0f)
#define UNDERSHOOT          0x3b69   // (1/OVERSHOOT)

#define RANGE_STOP0         0x1c04   // 1.0f  /255
#define RANGE_STOP1         0x2d05   // 20.0f /255
#define RANGE_STOP2         0x3ae6   // 220.0f/255
#define RANGE_STOP3         0x3bd7   // 250.0f/255
