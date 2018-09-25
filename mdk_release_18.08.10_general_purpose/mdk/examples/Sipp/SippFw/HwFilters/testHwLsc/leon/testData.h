#ifndef TESTHWLSC_SETUP_H_
#define TESTHWLSC_SETUP_H_

extern uint16_t bayerGainMesh[];

#define SIPP_HW_LSC_BUFFER_WIDTH       3264
#define SIPP_HW_LSC_BUFFER_HEIGHT      2448
#define SIPP_HW_LSC_BUFFER_NUM_PLANES  1
#define SIPP_HW_LSC_BUFFER_SIZE  (SIPP_HW_LSC_BUFFER_WIDTH * SIPP_HW_LSC_BUFFER_HEIGHT * SIPP_HW_LSC_BUFFER_NUM_PLANES)

#define FORMAT              BAYER
#define DATA_WIDTH          10

#define IMAGE_WIDTH         3264
#define IMAGE_HEIGHT        2448

#define GM_WIDTH            58
#define GM_HEIGHT           44
#define GM_PLANE_NO         1

#define LSC_IN_FORMAT       SIPP_FORMAT_16BIT
#define LSC_OUT_FORMAT      SIPP_FORMAT_16BIT
#define LSC_GM_FORMAT       SIPP_FORMAT_16BIT

#endif
