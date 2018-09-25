/*
 * testHwChromaDnsSetup.h
 *
 *  Created on: 1 Oct 2015
 */

#ifndef LEON_TESTHWCHROMADNSSETUP_H_
#define LEON_TESTHWCHROMADNSSETUP_H_


#define SIPP_HW_CHR_DNS_BUFFER_WIDTH      792
#define SIPP_HW_CHR_DNS_BUFFER_HEIGHT     645
#define SIPP_HW_CHR_DNS_BUFFER_NUM_PLANES  3
#define SIPP_HW_CHR_DNS_BUFFER_SIZE  (SIPP_HW_CHR_DNS_BUFFER_WIDTH * SIPP_HW_CHR_DNS_BUFFER_HEIGHT * SIPP_HW_CHR_DNS_BUFFER_NUM_PLANES)

#define FIRST_STEP_EN       ENABLED
#define SECOND_STEP_EN      ENABLED
#define THIRD_STEP_EN       DISABLED
#define HOR_ENABLE          ((THIRD_STEP_EN << 2) | (SECOND_STEP_EN << 1) | FIRST_STEP_EN)
#define LIMIT               5
#define FORCE_WT_H          0
#define FORCE_WT_V          0
#define TWO_PLANE_MODE      DISABLED
#define THREE_PLANE_MODE    DISABLED
#define OFFSET              ( int8_t)-2
#define SLOPE               (uint8_t)(.12 * 0xff)

#define HOR_T1              3
#define HOR_T2              7
#define HOR_T3              11
#define VER_T1              3
#define VER_T2              7
#define VER_T3              11

#define CHROMA_R_SCALE      1.25f
#define GREY_CR             (uint8_t)(255/3 * CHROMA_R_SCALE)
#define CHROMA_G_SCALE      2.25f
#define GREY_CG             (uint8_t)(255/3 * CHROMA_G_SCALE)
#define CHROMA_B_SCALE      1.75f
#define GREY_CB             (uint8_t)((255/3 * CHROMA_B_SCALE)*(!TWO_PLANE_MODE))
#define GREY_PT             DISABLED

#define CORNER_COEFF        0x07
#define CENTER_EDGE_COEFF   0x1c
#define CENTER_COEFF        0x74


#endif /* LEON_TESTHWCHROMADNSSETUP_H_ */
