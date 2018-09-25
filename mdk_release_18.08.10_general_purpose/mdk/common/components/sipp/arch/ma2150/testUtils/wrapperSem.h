
#ifndef __WRAPPER_SEMAPHORE__
#define __WRAPPER_SEMAPHORE__

#ifdef WIN32
#include <Windows.h>
#else
#include <semaphore.h>
#endif
#include <memory>

class Semaphore
{
public:
    Semaphore(unsigned int InitialCount, unsigned int MaxCount);
    ~Semaphore();

    int Post();
    int Wait();

private:
#ifdef WIN32
    HANDLE win_s_handle;
#else
    sem_t posix_sem;
#endif
};

// typedef std::shared_ptr<Semaphore> Semaphore_sp; - No noee dto include this as it forces c++11 lib usage
// and don'yt have time to test the cygwin versions here

#endif // __WRAPPER_SEMAPHORE__
