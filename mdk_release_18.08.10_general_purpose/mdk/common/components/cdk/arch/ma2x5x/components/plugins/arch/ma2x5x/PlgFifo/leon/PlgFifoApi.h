/**************************************************************************************************

 @File         : PlgFifoApi.h
 @Author       : MT
 @Brief        : Contain FIFO plug-in interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :
    FIFO plug-in will be used to sequence other plug-in that use same HW resources, so is not
    allow to run that plug-in in parallel.
    This plug-in use a dummy output frame pools, with will not contain frames, and will not be
    linked with them. Input frame, that is added in FIFO, will not be released, and will be send
    directly to corresponding FIFO consumer, at trigger time. Output pools associated with FIFO,
    have to contain just the list of callbacks, to correct FIFO consumers. Important to notice
    that number on input FIFO callbacks, need to be equal with number of output pools.
    FIFO plug-in will take care to send frame coming from input callback 1 to functions
    associated with output pool number 1.

     Resources used:
         Leon. (LOS or Lrt)

    No interrupt base, no significant processing time. Used just for serialize

 **************************************************************************************************/

#ifndef _PLG_FIFO_API_H_
#define _PLG_FIFO_API_H_

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "PlgTypes.h"


/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#define PLGFIFO_SZ    32 /// Max allowed FIFO elements
#define PLGFIFO_MAX_NR_OF_INPUTS 6 /// Max number of input/output plug-in linked
/**************************************************************************************************
 ~~~  Basic typedefs
 **************************************************************************************************/
typedef struct PlgFifoElemS {
    uint32_t  inputId;
    void *value; // all the information have to be a void pointer
    struct PlgFifoElemS *next;
} PlgFifoElem;

// !!! TYPE PLUGIN DESCRIPTOR
typedef struct PlgFifoS {
    PlgType plg;
    // Specific plug-in future
    void (*triger)(void *pluginObj);
    // Local plug-in data, Not externally usage
    FrameProducedCB   cbList[PLGFIFO_MAX_NR_OF_INPUTS];
    FramePool    *outputPools;
    uint32_t        nOutputPools;
    PlgFifoElem     fifoList[PLGFIFO_SZ];
    PlgFifoElem     *fifoTop;
    PlgFifoElem     *fifoBase;
    volatile unsigned int fifoLevel;
}PlgFifo;


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/

extern void PlgFifoCreate(void *pluginObj);
extern void PlgFifoConfig(void *pluginObj, uint32_t NoOfInputs);

#endif //_PLG_FIFO_API_H_
