/// =====================================================================================
///
///        @file:      demo1.h
///        @brief:
///        @created:   11/13/2017 07:20:22 PM
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <stdint.h>
#include "fe_cvdisplay.h"
/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------
typedef struct{
    int width;
    int height;
    frameFmt_t format;
}frameSpec_t;

typedef enum{
    CV_IMAGE_DISPLAY,
    CV_MAT_DISPLAY
}displayMode_T;

typedef struct{
    void *data;
    const char *windowname;
    frameSpec_t spec;
    displayMode_T mode;
    float scaling;
    int seqNo;
    const char* resultString;
}cvShowMsg_t;

void createMsgQueue(const char *name, int maxmsg, int msgsize);
