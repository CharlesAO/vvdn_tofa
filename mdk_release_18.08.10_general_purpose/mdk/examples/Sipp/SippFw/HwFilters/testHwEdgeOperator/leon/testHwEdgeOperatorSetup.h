#ifndef LEON_TESTHWEDGEOPERATORSETUP_H_
#define LEON_TESTHWEDGEOPERATORSETUP_H_

#define SIPP_HW_EDGE_OP_BUFFER_WIDTH      1296
#define SIPP_HW_EDGE_OP_BUFFER_HEIGHT     972
#define SIPP_HW_EDGE_OP_BUFFER_NUM_PLANES  1
#define SIPP_HW_EDGE_OP_BUFFER_SIZE  (SIPP_HW_EDGE_OP_BUFFER_WIDTH * \
              SIPP_HW_EDGE_OP_BUFFER_HEIGHT * SIPP_HW_EDGE_OP_BUFFER_NUM_PLANES)

// Application specific parameters
#define INPUT_MODE          NORMAL_MODE
#define OUTPUT_MODE         ORIENT_8BIT
#define THETA_MODE          NORMAL_THETA
#define THETA_OVX           ENABLED
#define MAGN_SCALE_FACT     0x3C00

#endif /* LEON_TESTHWEDGEOPERATORSETUP_H_ */
