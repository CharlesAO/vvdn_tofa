/**
 * @file rtems/score/cpuopts.h
 */

/* target cpu dependent options file */
/* automatically generated -- DO NOT EDIT!! */
#ifndef _RTEMS_SCORE_CPUOPTS_H
#define _RTEMS_SCORE_CPUOPTS_H

/* if RTEMS_DEBUG is enabled */
/* #undef RTEMS_DEBUG */

/* if multiprocessing is enabled */
/* #undef RTEMS_MULTIPROCESSING */

/* if using newlib */
#define RTEMS_NEWLIB 1

/* if posix api is supported */
#define RTEMS_POSIX_API 1

/* if SMP is enabled */
/* #undef RTEMS_SMP */

/* PARAVIRT is enabled */
/* #undef RTEMS_PARAVIRT */

/* if profiling is enabled */
/* #undef RTEMS_PROFILING */

/* if networking is enabled */
#define RTEMS_NETWORKING 1

/* if driver manager api is supported */
/* #undef RTEMS_DRVMGR_STARTUP */

/* RTEMS version string */
#define RTEMS_VERSION "4.11.99.0"

/* Define to 1 if ada/gnat bindings are built-in */
/* #undef __RTEMS_ADA__ */

/* sizeof(mode_t) */
#define __RTEMS_SIZEOF_MODE_T__ 4

/* sizeof(off_t) */
#define __RTEMS_SIZEOF_OFF_T__ 8

/* sizeof(time_t) */
#define __RTEMS_SIZEOF_TIME_T__ 8

/* sizeof(blksize_t) */
#define __RTEMS_SIZEOF_BLKSIZE_T__ 4

/* sizeof(blkcnt_t) */
#define __RTEMS_SIZEOF_BLKCNT_T__ 4

/* major version portion of an RTEMS release */
#define __RTEMS_MAJOR__ 4

/* minor version portion of an RTEMS release */
#define __RTEMS_MINOR__ 11

/* revision version portion of an RTEMS release */
#define __RTEMS_REVISION__ 99

#endif /* _RTEMS_SCORE_CPUOPTS_H */
