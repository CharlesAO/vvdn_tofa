// ***************************************************************************
// Copyright (C) 2011 Movidius Ltd. All rights reserved
// ---------------------------------------------------------------------------
// File       : mvRTEMSStubs.c
// Description: Stubbing rtems functions out.
// ---------------------------------------------------------------------------
// HISTORY
// Version        | Date       | Owner         | Purpose
// ---------------------------------------------------------------------------
// 0.1            | 01.07.2014 | Cristian Olar | Initial version
// ***************************************************************************

#include "mvRTEMSStubs.h"
#include "mv_types.h"

#define EAGAIN 1
#define EINVAL 2

#define ENOMEM 12

#define ONCE_STATE_NOT_RUN  0
#define ONCE_STATE_RUNNING  1
#define ONCE_STATE_COMPLETE 2

int rtems_gxx_mutex_destroy(int *mutex)
{
    UNUSED(mutex);

    //This should not get called. If it does, it should return an error.
    return -1;
}

int rtems_gxx_mutex_lock(int *mutex)
{
    UNUSED(mutex);

    //This should not get called. If it does, it should return an error.
    return -1;
}

int rtems_gxx_key_delete(void* key)
{
    UNUSED(key);

    int eno = 0;
    //A key should never be allocated
    return eno;
}

void rtems_gxx_mutex_init(void* *mutex)
{
    UNUSED(mutex);

    //Nothing to do here in the bare-metal case
    return;
}

int rtems_gxx_key_create(void* *key, void (*dtor)(void *))
{
    UNUSED(key);
    UNUSED(dtor);

    //Return the error saying the system cannot allocate a key since we are running bare metal.
    return EAGAIN;
}

int _Once(int *once_state, void (*init_routine)(void))
{
    int eno = 0;

    if (*once_state != ONCE_STATE_COMPLETE)
    {

        /*
         * Getting to here means the once_control is locked so we have:
         *  1. The init has not run and the state is ONCE_STATE_NOT_RUN.
         *  2. The init has finished and the state is ONCE_STATE_COMPLETE (already
         *     caught by the previous if).
         *  3. The init is being run by this thread and the state
         *     ONCE_STATE_RUNNING so we are nesting. This is an error.
         */

        switch (*once_state)
        {
        case ONCE_STATE_NOT_RUN:
            *once_state = ONCE_STATE_RUNNING;
            (*init_routine)();
            *once_state = ONCE_STATE_COMPLETE;
            break;
        case ONCE_STATE_RUNNING:
            eno = EINVAL;
            break;
        default:
            break;
        }

    }

    return eno;
}

int rtems_gxx_once(int *once, void (*func)(void))
{
    return _Once(once, func);
}

int rtems_gxx_recursive_mutex_lock(int *mutex)
{
    return rtems_gxx_mutex_lock(mutex);
}

void rtems_gxx_recursive_mutex_init(void* *mutex)
{
    rtems_gxx_mutex_init(mutex);
}

int rtems_gxx_mutex_unlock(__gthread_mutex_t *mutex)
{
    UNUSED(mutex);

// This should never be called. If it is called than it should return an error.
    return -1;
}

void *rtems_gxx_getspecific(__gthread_key_t key)
{
    UNUSED(key);

    void *p = NULL;

    return p;
}

int rtems_gxx_setspecific(__gthread_key_t key, const void *ptr)
{
    UNUSED(key);
    UNUSED(ptr);

    int eno = ENOMEM;

    return eno;
}

int rtems_gxx_recursive_mutex_unlock(__gthread_recursive_mutex_t *mutex)
{
    return rtems_gxx_mutex_unlock(mutex);
}

void _Mutex_recursive_Acquire(void)
{
    return;
}

void _Mutex_recursive_Release(void)
{
    return;
}

void _Mutex_Acquire(void)
{
    return;
}

void _Mutex_Release(void)
{
    return;
}

