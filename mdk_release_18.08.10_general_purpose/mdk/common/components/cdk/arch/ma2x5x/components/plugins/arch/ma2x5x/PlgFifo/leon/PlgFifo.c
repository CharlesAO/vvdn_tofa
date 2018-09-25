/**************************************************************************************************

 @File         : PlgFifo.c
 @Author       : MT
 @Brief        : Contain FIFO plug-in Implementation
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :
     Resources used:
         Leon. (LOS or Lrt)

    No interrupt base, no significant processing time. Used just for serialize

 **************************************************************************************************/


/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <assert.h>
#include "PlgFifoApi.h"
#include "FrameMgrApi.h"
#if defined(__sparc)
#include "swcLeonUtils.h"
#endif
/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
// allow avoiding frames and pools control structures data sharing conflict
#if defined(__sparc)
#include "swcLeonUtils.h"
#define StartCriticalSection()            swcLeonSetPIL(15)
#define EndCriticalSection(_x)            swcLeonSetPIL(_x)
#else
#define StartCriticalSection() (1)
#define EndCriticalSection(_x) (_x++)
#endif
/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static int32_t      plgFifoInit(FramePool *outputPools, int nOutputPools, void *pluginObj);
static int32_t      plgFifoFini(void *pluginObj);
static inline void  plgFifoAddSrcX(FrameT *frame, void *pluginObj, uint32_t inputId);
static void         plgFifoAddSrc0(FrameT *frame, void *pluginObj);
static void         plgFifoAddSrc1(FrameT *frame, void *pluginObj);
static void         plgFifoAddSrc2(FrameT *frame, void *pluginObj);
static void         plgFifoAddSrc3(FrameT *frame, void *pluginObj);
static void         plgFifoAddSrc4(FrameT *frame, void *pluginObj);
static void         plgFifoAddSrc5(FrameT *frame, void *pluginObj);
static void         plgTriger(void *pluginObj);

FrameProducedCBFunc plgFifoAddSrc[PLGFIFO_MAX_NR_OF_INPUTS]=
{
        plgFifoAddSrc0,
        plgFifoAddSrc1,
        plgFifoAddSrc2,
        plgFifoAddSrc3,
        plgFifoAddSrc4,
        plgFifoAddSrc5
};

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/
//
void PlgFifoCreate(void *pluginObj) {
    uint32_t i;
    assert(NULL != pluginObj);
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;

    // create FIFO list
    for(i = 0; i < PLGFIFO_SZ; i++) {
        objLoc->fifoList[i].inputId = 0;
        objLoc->fifoList[i].value   = NULL;
        objLoc->fifoList[i].next    = &(objLoc->fifoList[i+1]);
    }
    objLoc->fifoList[PLGFIFO_SZ-1].next = &(objLoc->fifoList[0]);
    objLoc->fifoTop     = &(objLoc->fifoList[0]);
    objLoc->fifoBase    = &(objLoc->fifoList[0]);
    objLoc->fifoLevel   = 0;
    // general plug-in interface initialisation
    objLoc->plg.init             = plgFifoInit;
    objLoc->plg.fini             = plgFifoFini;
    objLoc->triger               = plgTriger;
    objLoc->plg.status           = PLG_STATS_IDLE;
}

/******************************************************************************
 @brief     : Set callbacks based on number of active cameras
 @param     : pluginObj        : Plug-in specific structure
 @param     : NoOfInputs       : Maximum number of cameras used by application
 @return    : void
 @Remark    : function must be called immediately after  PlgFifoCreate
 ******************************************************************************/
void PlgFifoConfig(void *pluginObj, uint32_t NoOfInputs) {
    uint32_t i;
    assert(NULL != pluginObj);
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;

    for(i = 0; i < NoOfInputs; i++) {
        objLoc->cbList[i].callback   = plgFifoAddSrc[i];
        objLoc->cbList[i].pluginObj  = pluginObj;
    }
    objLoc->plg.callbacks        = objLoc->cbList;
}

/**************************************************************************************************
 ~~~ Local Functions Implementation
 **************************************************************************************************/
//
static int32_t plgFifoInit(FramePool *outputPools, int nOutputPools, void *pluginObj) {
    assert(NULL != pluginObj);
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;
//    uint32_t i;
    unsigned int old_value = StartCriticalSection();
    objLoc->outputPools = outputPools;
    objLoc->nOutputPools = nOutputPools;
    // extra clear
    objLoc->fifoTop     = &objLoc->fifoList[0];
    objLoc->fifoBase    = &objLoc->fifoList[0];
    objLoc->fifoLevel   = 0;
    objLoc->plg.status           = PLG_STATS_RUNNING;
    EndCriticalSection(old_value);
    return 0;
}

//
static int32_t plgFifoFini(void *pluginObj) {
    assert(NULL != pluginObj);
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;
    unsigned int old_value = StartCriticalSection();
    // clear FIFO
    objLoc->fifoTop     = &objLoc->fifoList[0];
    objLoc->fifoBase    = &objLoc->fifoList[0];
    objLoc->fifoLevel   = 0;
    objLoc->plg.status           = PLG_STATS_IDLE;
    EndCriticalSection(old_value);
    return 0;
}

// Plug-in can have maximum 6 inputs. This is the general callback implementation for all 6.
static inline void plgFifoAddSrcX(FrameT *frame, void *pluginObj, uint32_t inputId) {
    assert(NULL != pluginObj);
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;
    unsigned int old_value = StartCriticalSection();
    if(PLG_STATS_RUNNING == objLoc->plg.status) {
        assert (inputId <= objLoc->nOutputPools);
        assert(PLGFIFO_SZ > objLoc->fifoLevel);
        objLoc->fifoTop->value = (void*)frame;
        objLoc->fifoTop->inputId = inputId;
        objLoc->fifoTop = objLoc->fifoTop->next;
        objLoc->fifoLevel +=1;
    }
    EndCriticalSection(old_value);
}

//plug-in callback source 0
static void plgFifoAddSrc0(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 0);
}

//plug-in callback source 1
static void plgFifoAddSrc1(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 1);
}

//plug-in callback source 2
static void plgFifoAddSrc2(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 2);
}

//plug-in callback source 3
static void plgFifoAddSrc3(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 3);
}

//plug-in callback source 4
static void plgFifoAddSrc4(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 4);
}

//plug-in callback source 5
static void plgFifoAddSrc5(FrameT *frame, void *pluginObj) {
    plgFifoAddSrcX(frame, pluginObj, 5);
}

//
static void plgTriger(void *pluginObj) {
    PlgFifo* objLoc        = (PlgFifo*)pluginObj;
    void *returnValue = NULL;
    uint32_t i;
    int32_t addExtraConsumer = 0;
    uint32_t inputId = 0;
    if(objLoc->fifoLevel > 0) {
        uint32_t old_value = StartCriticalSection();
        returnValue = objLoc->fifoBase->value;
        inputId     = objLoc->fifoBase->inputId;
        objLoc->fifoBase = objLoc->fifoBase->next;
        objLoc->fifoLevel -=1;
        EndCriticalSection(old_value);
        addExtraConsumer = objLoc->outputPools[inputId].nCallbacks - 1;
        assert(addExtraConsumer >= 0);
        FrameMgrIncreaseNrOfConsumer((FrameT*)returnValue, (uint32_t)addExtraConsumer);
        for (i = 0; i < objLoc->outputPools[inputId].nCallbacks; i++) {
            (objLoc->outputPools[inputId].callbacks[i].callback)((FrameT*)returnValue,
                    objLoc->outputPools[inputId].callbacks[i].pluginObj);
        }
    }
}








