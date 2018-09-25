/* system.h
*
* This include file contains information that is included in every
* function in the test set.
*
*/

#ifndef _SYS_H
#define _SYS_H

#include <rtems.h>
#include <inttypes.h>
#include <mv_types.h>

#ifdef __cplusplus
extern "C"
{
#endif


/* functions */
rtems_task blink_task(rtems_task_argument unused);

/* global variables */

/*
* Keep the names and IDs in global variables so another task can use them.
*/

#ifdef __cplusplus
}
#endif

#endif

/* end of include file */
