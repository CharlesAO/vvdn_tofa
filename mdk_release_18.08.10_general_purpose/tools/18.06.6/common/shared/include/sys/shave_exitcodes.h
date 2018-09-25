/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2016-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys/shave_exitcodes.h
 *  Description:   Specifies the conventions for the system exit codes, and the
 *                 user interface for interacting with them.
 *  --------------------------------------------------------------------------- */

#ifndef _MCC_SYS_SHAVE_EXITCODES_H_
#define _MCC_SYS_SHAVE_EXITCODES_H_ (1)


#ifndef __myriad2__
# error "sys/shave_exitcodes.h: this header is for the Myriad2 processor!"
#endif

#pragma GCC system_header

#include <sys/__moviconfig.h>


_MV_EXTERNC_BEGIN


/* The system exit codes */
typedef enum SHAVEExitCodes_t {
    /* System reserved values */
    SHAVEExitNormal        = 0x0000,
    SHAVEExitAbortCalled   = 0x0001,
    SHAVEExitAssertCalled  = 0x0002,
    SHAVEExitStackOverflow = 0x0003,

    __reserved_0004        = 0x0004,
    __reserved_0005        = 0x0005,
    __reserved_0006        = 0x0006,
    __reserved_0007        = 0x0007,
    __reserved_0008        = 0x0008,
    __reserved_0009        = 0x0009,
    __reserved_000A        = 0x000A,
    __reserved_000B        = 0x000B,
    __reserved_000C        = 0x000C,
    __reserved_000D        = 0x000D,
    __reserved_000E        = 0x000E,
    __reserved_000F        = 0x000F,

    /* The values 0x0010 through 0x001F (0x1FFF on MA248x) are available for user defined meanings */
    _LegacyMDKExit         = 0x001F    /* This value was used by older versions of the MDK for all exit conditions */

} SHAVEExitCodes_t;


#ifdef __shave__
/* This has to be a macro because the builtin will only accept constant literals and not variables (e.g. function arguments) */
# define shave_sysexit(exitcode) (__builtin_shave_bru_swih_i(exitcode))
#else
/* To Be Determined - Leon interface for getting the tag from SWIH */
#endif


_MV_EXTERNC_END


#endif /* _MCC_SYS_SHAVE_EXITCODES_H_ */
