#define KERNEL_SIZE         7
#define THRESHOLD           0x1FF
#define OUT_SEL             ((KERNEL_SIZE >> 1) * (KERNEL_SIZE) + (KERNEL_SIZE >> 1))
#define LUMA_ABLEND_EN      0x1
#define LUMA_SAMPLE_EN      0x1

#define OFFSET              (u8)(-.2*255)
#define SLOPE               6

#define SIPP_HW_MED_CHROMA_BUFFER_WIDTH       192
#define SIPP_HW_MED_CHROMA_BUFFER_HEIGHT      145
#define SIPP_HW_MED_CHROMA_BUFFER_NUM_PLANES  3
#define SIPP_HW_MED_CHROMA_BUFFER_SIZE        (SIPP_HW_MED_CHROMA_BUFFER_WIDTH * SIPP_HW_MED_CHROMA_BUFFER_HEIGHT * SIPP_HW_MED_CHROMA_BUFFER_NUM_PLANES)
#define SIPP_HW_MED_LUMA_BUFFER_WIDTH         (SIPP_HW_MED_CHROMA_BUFFER_WIDTH << LUMA_SAMPLE_EN)
#define SIPP_HW_MED_LUMA_BUFFER_HEIGHT        (SIPP_HW_MED_CHROMA_BUFFER_HEIGHT << LUMA_SAMPLE_EN)
#define SIPP_HW_MED_LUMA_BUFFER_NUM_PLANES    1
#define SIPP_HW_MED_LUMA_BUFFER_SIZE          (SIPP_HW_MED_LUMA_BUFFER_WIDTH * SIPP_HW_MED_LUMA_BUFFER_HEIGHT * SIPP_HW_MED_LUMA_BUFFER_NUM_PLANES)
