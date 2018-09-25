///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump private
///

#ifndef FRAMEPUMPPRIVATE_H__
#define FRAMEPUMPPRIVATE_H__

#include "FramePump.h"
#include <rtems.h>


struct FramePumpVideoContext {
    char *name;

    const char *ctlName;
    int ctlFd;

    const char *ctlNameShared;
    int ctlFdShared;

    const struct FramePumpHwFunc *hwFnc;
    struct FramePumpControlFunctions *ctlFnc;

    struct FramePumpGpioDescriptor *gpios;

    struct FramePumpHwContext *hwCtx;

    const struct FramePumpSensorDriver *spec;

    struct FramePumpOutSpec outSpec;

    rtems_id inFifo;
    rtems_id outFifo;

    FramePumpSensorSpecificData sensorData;
};

struct FramePumpControlFunctions {
    rtems_status_code (*init) (const struct FramePumpVideoContext *,
                                FramePumpSensorSpecificData *);
    rtems_status_code (*open) (struct FramePumpVideoContext *);
    rtems_status_code (*close) (struct FramePumpVideoContext *);
    rtems_status_code (*config) (struct FramePumpVideoContext *,
                                struct FramePumpControlConfig *);
    rtems_status_code (*power_up) (struct FramePumpVideoContext *);
    rtems_status_code (*power_down) (struct FramePumpVideoContext *);
    rtems_status_code (*frameCb) (const struct FramePumpVideoContext *,
                                struct FramePumpBuffer *);
};

struct FramePumpHwFunc {
    rtems_status_code (*init) (struct FramePumpVideoContext *, struct FramePumpVideoDevice *);
    rtems_status_code (*open) (struct FramePumpVideoContext *);
    rtems_status_code (*close) (struct FramePumpVideoContext *);

    rtems_status_code (*start) (struct FramePumpVideoContext *);
    rtems_status_code (*stop) (struct FramePumpVideoContext *);

    rtems_status_code (*eof) (struct FramePumpVideoContext *);
    rtems_status_code (*eol) (struct FramePumpVideoContext *);
    rtems_status_code (*stopHandler) (struct FramePumpVideoContext *);
    rtems_status_code (*startHandler) (struct FramePumpVideoContext *);
};

typedef struct FramePumpSensorMetadataROI {
    uint32_t x0, y0;
    uint32_t columns;
    uint32_t lines;
} FramePumpSensorMetadataROI;

typedef struct FramePumpSensorDriver {
    uint32_t width;
    uint32_t height;
    uint32_t windowColumnStart;
    uint32_t windowRowStart;

    struct FramePumpSensorMetadataROI metaROI;

    FramePumpDatapathType type;
    FramePumpDatapathSpec spec;

    const FramePumpI2CConfigDescriptor *i2cConfigSteps;
    const uint32_t              nbOfI2CConfigSteps;

    struct FramePumpControlFunctions *ctlFnc;

} FramePumpSensorDriver;

#endif
