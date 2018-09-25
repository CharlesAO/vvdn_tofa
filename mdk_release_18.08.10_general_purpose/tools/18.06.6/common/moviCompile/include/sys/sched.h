/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2015-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   sys/sched.h
 *  Description:   SHAVE Specific implementation of the 'sys/sched.h' interface.
 *                 SHAVE does not support threads, so use a dummy implementation.
 *  --------------------------------------------------------------------------- */

#ifndef _SYS_SCHED_H_
#define _SYS_SCHED_H_ (1)


#pragma GCC system_header

#include <sys/__moviconfig.h>


_MV_EXTERNC_BEGIN


#ifdef __shave__
__attribute__((always_inline)) inline int sched_yield() { return 0; }

struct sched_param
{
  int sched_priority;
};
#else  /* __shave__ */
# error "This version of 'sys/sched.h' is specific to the SHAVE processor"
#endif /* __shave__ */


_MV_EXTERNC_END


#endif /* _SYS_SCHED_H_ */
