// let the user disable this implementation if needed
#ifndef NO_PTHREAD_LITE_BARE_METAL_IMPL

#if __GNUC__ >= 6
/*
 *  Pthreads-lite implementation for bare metal applications (single threaded)
 *  Required for the C++ library of sparc-myriad-rtems 6.3.0 to work properly.
 *
 *  This is an generalization adaptation from this PR
 *  https://github.com/movidius/mdk/pull/4375
 *  BUG: 26552
 */
#include <machine/_threads.h>
#include <assert.h>
#include <stdint.h>
#include <malloc.h>
#include <errno.h>

int pthread_once(once_flag *once_control, void (*init_routine)(void))
{
   assert(once_control != NULL);
   assert(once_control->_is_initialized == 1);
   if (!once_control->_init_executed)
   {
      once_control->_init_executed = 1;
      init_routine();
   }
   return 0;
}

typedef struct Destructor {
   void *value;
   struct Destructor *next;
   struct Destructor *prev;
   void (*destructor)(void*);
} Destructor;

static Destructor *destructors_list = NULL;

int pthread_key_create(uint32_t *key, void (*destructor)(void*))
{
    Destructor *p = (Destructor*) malloc(sizeof(*p));
    if (p == NULL) {
        return -1;
    }
    p->value = NULL;
    p->prev = NULL;
    p->next = destructors_list;
    p->destructor = destructor;
    destructors_list = p;
    *key = (uint32_t) p;
    return 0;
}

int pthread_key_delete(uint32_t key)
{
    Destructor *p = (Destructor*)key;
    if (p == NULL)
    {
        return -1;
    }
    if (p->prev)
    {
        p->prev->next = p->next;
    }
    else
    {
        destructors_list = p->next;
    }
    if (p->next)
    {
        p->next->prev = p->prev;
    }
    return 0;
}

__attribute__((destructor))
void destroy_all(void)
{
    Destructor *p = destructors_list;
    destructors_list = NULL;
    while (p)
    {
        if (p->value != NULL && p->destructor != NULL)
        {
            p->destructor(p->value);
        }
        Destructor *prev = p;
        p = p->next;
        free(prev);
    }
}

#endif // __GNUC__
#endif // NO_PTHREAD_LITE_BARE_METAL_IMPL

