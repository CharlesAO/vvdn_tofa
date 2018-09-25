/// =====================================================================================
///
///        @file:      fe_hevc.h
///        @brief:     
///        @created:   11/14/2017 02:51:42 PM
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///
#include <assert.h>
#include <string.h>
#include <pthread.h>
#define MAX_LEN_FIFO_NAME 30
#define MAX_LEN_MQ_NAME 30
#define MAX_LEN_WINDOW_NAME 30
/// System Includes
/// -------------------------------------------------------------------------------------

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

class Hevc{
public:
    Hevc(const char *fifo, const char *mqName, const char *windowName, float scaling)
    {
        unsigned int strlFifo = strlen(fifo) + 1;
        unsigned int strlMq = strlen(mqName) + 1;
        unsigned int strlWindow = strlen(windowName) + 1;

        assert(strlFifo <= MAX_LEN_FIFO_NAME);
        assert(strlMq <= MAX_LEN_MQ_NAME);
        assert(strlWindow <= MAX_LEN_WINDOW_NAME);

        memcpy(this->fifoName, fifo, strlFifo);
        memcpy(this->mqName, mqName, strlMq);
        memcpy(this->windowName, windowName, strlWindow);

        this->scaling = scaling;
    }

    static void *ThreadItf(void *This);
    void *Thread();

    void Start();
    void Stop();
private:
    char fifoName[MAX_LEN_FIFO_NAME];
    char mqName[MAX_LEN_MQ_NAME];
    char windowName[MAX_LEN_WINDOW_NAME];
    pthread_t thread;
    float scaling;
};
void *hevcdecode(void* fifo, void *mqName);
