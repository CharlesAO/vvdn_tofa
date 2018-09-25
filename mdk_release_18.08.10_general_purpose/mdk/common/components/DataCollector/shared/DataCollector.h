#ifndef DATACOLLECTOR_H__
#define DATACOLLECTOR_H__


#ifndef MAX_CAMERA_COUNT
#define MAX_CAMERA_COUNT 2
#endif
#ifndef MAX_PROCESS_THREADS
#define MAX_PROCESS_THREADS 3
#endif

#ifndef MAX_PROCESS_NODE_TYPES
#define MAX_PROCESS_NODE_TYPES 2
#endif

#include "visionBasePlg.h"
#include "swcWhoAmI.h"

typedef enum dataCollectorAlgoType{
    ALGO_NONE = 0,
    ALGO_VTRACK,
    ALGO_STEREO,
}dataCollectorAlgoType;

typedef struct dataCollectorContext
{
    char *camName[MAX_CAMERA_COUNT];
    int camCnt;
    int camFrameRate;
    int latencyControl;
    int processingCount; // the number of processing plugin types. i.e. running vtrack and stereo will need this count to be 2, no matter how many threads are the algos running on.
    visionBasePlg* processingPlugin[MAX_PROCESS_NODE_TYPES][MAX_PROCESS_THREADS]; //this will be a 2 dim array when multiple types of processing available
    int processingthreadCnt[MAX_PROCESS_NODE_TYPES]; //how many threads will be used by each algo
    dataCollectorAlgoType type[MAX_PROCESS_NODE_TYPES]; //This will be an array when multiple processing nodes are available.
    swcProcessorType where[MAX_PROCESS_NODE_TYPES];
    frameSpec frSpec;
}dataCollectorContext;

#ifdef __cplusplus
extern "C"
#endif

void* DataCollectorInit(dataCollectorContext* ctx);
void* DataCollectorStart(dataCollectorContext* ctx);

#endif
