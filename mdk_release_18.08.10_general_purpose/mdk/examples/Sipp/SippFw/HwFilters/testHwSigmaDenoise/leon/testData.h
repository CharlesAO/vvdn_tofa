#define FORMAT              BAYER
#define PASSTHROUGH_BIT     ENABLED
#define DATA_WIDTH          10
#define TEST_IMAGE_DW       10
#define NOISE_FLOOR         (uint16_t)(.05f * ((1 << DATA_WIDTH) - 1))

#define  GR_THR_MUL_0        (uint8_t)(.025f * 255)
#define  GR_THR_MUL_1        (uint8_t)(.050f * 255)
#define  R_THR_MUL_0        (uint8_t)(.025f * 255)
#define  R_THR_MUL_1        (uint8_t)(.050f * 255)
#define  B_THR_MUL_0        (uint8_t)(.025f * 255)
#define  B_THR_MUL_1        (uint8_t)(.050f * 255)
#define  GB_THR_MUL_0        (uint8_t)(.025f * 255)
#define  GB_THR_MUL_1        (uint8_t)(.050f * 255)

#define BLACK0              (0x1 << ((DATA_WIDTH - TEST_IMAGE_DW) * (DATA_WIDTH >= TEST_IMAGE_DW)))
#define BLACK1              (0x2 << ((DATA_WIDTH - TEST_IMAGE_DW) * (DATA_WIDTH >= TEST_IMAGE_DW)))
#define BLACK2              (0x3 << ((DATA_WIDTH - TEST_IMAGE_DW) * (DATA_WIDTH >= TEST_IMAGE_DW)))
#define BLACK3              (0x4 << ((DATA_WIDTH - TEST_IMAGE_DW) * (DATA_WIDTH >= TEST_IMAGE_DW)))

#define SIPP_HW_SIGMA_DNS_BUFFER_WIDTH      1600
#define SIPP_HW_SIGMA_DNS_BUFFER_HEIGHT     1300
#define SIPP_HW_SIGMA_DNS_BUFFER_NUM_PLANES 1
#define SIPP_HW_SIGMA_DNS_BUFFER_SIZE       (SIPP_HW_SIGMA_DNS_BUFFER_WIDTH* SIPP_HW_SIGMA_DNS_BUFFER_HEIGHT* SIPP_HW_SIGMA_DNS_BUFFER_NUM_PLANES *2)
