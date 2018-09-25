/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2017-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys/utsname.h
 *  Description:   Implements a simplified version of the 'uname' function as
 *                 defined in the OpenGroup's interface '<sys/utsname.h>.
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_SYS_UTSNAME_H_
#define _MCC_SYS_UTSNAME_H_ (1)


#ifndef __myriad2__
# error "sys/utsname.h: this header is for the Myriad2 processor!"
#endif

#pragma GCC system_header

#include <sys/__moviconfig.h>


_MV_EXTERNC_BEGIN


typedef struct utsname {
    char machine[8];    /* Always the chip family name - e.g "ma2x5x" */
    char nodename[1];   /* Always the empty string "" */
    char release[6];    /* Always the string "shave" */
    char sysname[1];    /* Always the empty string "" */
    char version[24];   /* Always the version of 'moviCompile' it is provided with - e.g. "00.83.01 Build 121336" */
};

_MV_EXTERN_DECL int uname(struct utsname*);


_MV_EXTERNC_END


#endif /* _MCC_SYS_UTSNAME_H_ */
