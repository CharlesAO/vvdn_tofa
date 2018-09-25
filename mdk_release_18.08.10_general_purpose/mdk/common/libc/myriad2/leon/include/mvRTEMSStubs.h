/*
 * mvRTEMSStubs.h
 *
 *  Created on: Jul 1, 2014
 */

#ifndef MVRTEMSSTUBS_H_
#define MVRTEMSSTUBS_H_

typedef void *__gthread_key_t;
typedef int   __gthread_once_t;
typedef void *__gthread_mutex_t;
typedef void *__gthread_recursive_mutex_t;

int rtems_gxx_mutex_unlock (__gthread_mutex_t *mutex);

#endif /* MVRTEMSSTUBS_H_ */
