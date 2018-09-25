#include "wrapperSem.h"

/// Constructor
Semaphore::Semaphore (unsigned int InitialCount, unsigned int MaxCount)
{
#ifdef WIN32
    win_s_handle = CreateSemaphore(NULL, InitialCount, MaxCount, NULL);
#else
    sem_init(&posix_sem, 0, InitialCount);
#endif
}

/// Destructor
Semaphore::~Semaphore()
{
#ifdef WIN32
    CloseHandle(win_s_handle);
#else
    sem_destroy(&posix_sem);
#endif
}

/// Member functions
int Semaphore::Post()
{
#ifdef WIN32
    ReleaseSemaphore(win_s_handle, 1, NULL);
#else
    sem_post(&posix_sem);
#endif
    return 0;
}

int Semaphore::Wait()
{
#ifdef WIN32
    WaitForSingleObject(win_s_handle, INFINITE);
#else
    sem_wait(&posix_sem);
#endif
    return 0;
}