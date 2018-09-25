#ifndef _SYS_H
#define _SYS_H

#include <rtems.h>
#include <inttypes.h>


#ifdef __cplusplus
extern "C" {
#endif



/* functions */

rtems_task Init(rtems_task_argument argument);
rtems_task SVU_task(rtems_task_argument argument);


/* global variables */
extern rtems_id initId;

/*
 *  Keep the names and IDs in global variables so another task can use them.
 */

/*
 *  Handy macros and static inline functions
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
