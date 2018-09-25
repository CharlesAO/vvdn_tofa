#ifndef LEON_TESTHWLUTSETUP_H_
#define LEON_TESTHWLUTSETUP_H_

#define SIPP_HW_LUT_BUFFER_WIDTH          384
#define SIPP_HW_LUT_BUFFER_HEIGHT         290
#define SIPP_HW_LUT_BUFFER_NUM_IN_PLANES  3
#define SIPP_HW_LUT_BUFFER_NUM_OUT_PLANES 3
#define SIPP_HW_LUT_INPUT_BUFFER_SIZE     (SIPP_HW_LUT_BUFFER_WIDTH * SIPP_HW_LUT_BUFFER_HEIGHT * SIPP_HW_LUT_BUFFER_NUM_IN_PLANES)
#define SIPP_HW_LUT_OUTPUT_BUFFER_SIZE    (SIPP_HW_LUT_BUFFER_WIDTH * SIPP_HW_LUT_BUFFER_HEIGHT * SIPP_HW_LUT_BUFFER_NUM_OUT_PLANES)

#define LUT_REGION0_SIZE_I  4
#define LUT_REGION1_SIZE_I  4
#define LUT_REGION2_SIZE_I  4
#define LUT_REGION3_SIZE_I  4
#define LUT_REGION4_SIZE_I  4
#define LUT_REGION5_SIZE_I  4
#define LUT_REGION6_SIZE_I  4
#define LUT_REGION7_SIZE_I  4

#define LUT_REGION8_SIZE_I  4
#define LUT_REGION9_SIZE_I  4
#define LUT_REGION10_SIZE_I 4
#define LUT_REGION11_SIZE_I 4
#define LUT_REGION12_SIZE_I 4
#define LUT_REGION13_SIZE_I 4
#define LUT_REGION14_SIZE_I 4
#define LUT_REGION15_SIZE_I 4

#define INTERP_MODE         ENABLED
#define CHANNEL_MODE        ENABLED
#define INT_WIDTH           12
#define LUTS_NO             2
#define LUT_LOAD            ENABLED
#define APB_ACCESS          DISABLED
#define CSC_ENABLE          ENABLED

#define FLOAT_TO_FIXED6_10(A)       (int16_t)((A) * 1024)
#define FLOAT_TO_FIXED4_8(A, B)     (uint16_t)(((A) << 8) + ((B) * 256))
#define FLOAT_TO_FIXED1_7(A, B)     (uint8_t) (((A) << 7) + ((B) * 128))
#define FLOAT_TO_FIXED5_3(A, B)     (uint8_t) (((A) << 3) + ((B) *   8))
#define FLOAT_TO_FIXED2_10(A)       (int16_t)((A) * 1024)
#define FLOAT_TO_FIXED1_12(A)       (int16_t)((A) * 4096)

#define LUT_MAT_11          (FLOAT_TO_FIXED2_10( 0.299f) & 0xfff)
#define LUT_MAT_12          (FLOAT_TO_FIXED2_10( 0.587f) & 0xfff)
#define LUT_MAT_13          (FLOAT_TO_FIXED2_10( 0.114f) & 0xfff)
#define LUT_MAT_21          (FLOAT_TO_FIXED2_10(-0.169f) & 0xfff)
#define LUT_MAT_22          (FLOAT_TO_FIXED2_10(-0.331f) & 0xfff)
#define LUT_MAT_23          (FLOAT_TO_FIXED2_10( 0.500f) & 0xfff)
#define LUT_MAT_31          (FLOAT_TO_FIXED2_10( 0.500f) & 0xfff)
#define LUT_MAT_32          (FLOAT_TO_FIXED2_10(-0.419f) & 0xfff)
#define LUT_MAT_33          (FLOAT_TO_FIXED2_10(-0.081f) & 0xfff)
#define LUT_OFF_1           (  0 << 4)
#define LUT_OFF_2           (128 << 4)
#define LUT_OFF_3           (128 << 4)

// Input/output buffers' format
#define SIPP_FORMAT_8BIT    0x1
#define SIPP_FORMAT_16BIT   0x2
#define SIPP_FORMAT_32BIT   0x4
#define SIPP_FORMAT_PACK10  0x5
#define SIPP_FORMAT_PACK12  0x3

#define LUT_IN_FORMAT       SIPP_FORMAT_16BIT
#define LUT_LD_FORMAT       SIPP_FORMAT_16BIT
#define LUT_OUT_FORMAT      SIPP_FORMAT_8BIT

#define CHANNELS_NO         3
#define LUT_ALLOCATE        4
#define LUT_SIZE            (256 * LUT_ALLOCATE * LUTS_NO)

extern fp16 appLUT[LUT_SIZE] ALIGNED(8);

#endif /* LEON_TESTHWLUTSETUP_H_ */
