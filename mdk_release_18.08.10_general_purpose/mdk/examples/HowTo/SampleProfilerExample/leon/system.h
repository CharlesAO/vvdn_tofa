/*  system.h
 *
 *  This include file contains information that is included in every
 *  function in the test set.
 *
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#ifndef _SYS_H
#define _SYS_H

#include <rtems.h>
#include <inttypes.h>


#ifdef __cplusplus
extern "C" {
#endif

#define APP_UPA_CLOCKS                 ( (1 << UPA_SH0)      |  \
                                         (1 << UPA_SH1)      |  \
                                         (1 << UPA_SH2)      |  \
                                         (1 << UPA_SH3)      |  \
                                         (1 << UPA_SH4)      |  \
                                         (1 << UPA_SH5)      |  \
                                         (1 << UPA_SH6)      |  \
                                         (1 << UPA_SH7)      |  \
                                         (1 << UPA_SH8)      |  \
                                         (1 << UPA_SH9)      |  \
                                         (1 << UPA_SH10)     |  \
                                         (1 << UPA_SH11)     |  \
                                         (1 << UPA_SHAVE_L2) |  \
                                         (1 << UPA_CDMA)     |  \
                                         (1 << UPA_CTRL)     )

/* functions */

rtems_task Init(rtems_task_argument argument);

rtems_task SVU_task1(rtems_task_argument argument);

/* global variables */
extern rtems_id initId;

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */

#define TASK(i) (1 << (31 - (i)))

/*
 *  Macro to hide the ugliness of printing the time.
 */

#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
