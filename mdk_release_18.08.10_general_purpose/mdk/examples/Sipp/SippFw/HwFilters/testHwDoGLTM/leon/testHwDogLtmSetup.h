// testHwDogLtmSetup.h

#ifndef TEST_HWDOGLTM_SETUP_H_
#define TEST_HWDOGLTM_SETUP_H_

///////////////////////////////////////////////
// Macros
#define NCURVES  8
#define NPOINTS  16

#define MODE_DOG_ONLY    0x0
#define MODE_LTM_ONLY    0x1
#define MODE_DOG_DENOISE 0x2
#define MODE_DOG_LTM     0x3

#define DSMODE_CARRY 0x0
#define DSMODE_DOWN  0x1
#define DSMODE_UPDN  0x2

#define MODE                MODE_DOG_LTM
#define DOG_THR             (uint16_t)(0.015f * 0xff)
#define DOG_OUTPUT_CLAMP    DISABLED
#define DS_MODE             DSMODE_CARRY
#define DOG_HEIGHT          15
#define DOG_STR             (uint16_t)(1.000f * 0xff)
#define LTM_THR             35

#define SIPP_HW_DOG_LTM_BUFFER_WIDTH        1584
#define SIPP_HW_DOG_LTM_BUFFER_HEIGHT       1290
#define SIPP_HW_DOG_LTM_BUFFER_NUM_PLANES   1
#define SIPP_HW_DOG_LTM_BUFFER_SIZE         (SIPP_HW_DOG_LTM_BUFFER_WIDTH * SIPP_HW_DOG_LTM_BUFFER_HEIGHT * SIPP_HW_DOG_LTM_BUFFER_NUM_PLANES)

///////////////////////////////////////////////
// Variables
extern const uint16_t ltm_curves[NPOINTS][NCURVES];
extern u8 KCSet0[];
extern u8 KCSet1[];

#endif // TEST_HWDOGLTM_SETUP_H_

