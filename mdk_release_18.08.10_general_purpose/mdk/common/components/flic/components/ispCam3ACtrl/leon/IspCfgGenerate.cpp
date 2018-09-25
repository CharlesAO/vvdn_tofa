///
/// @file      IspCfgGenerate.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Generates the Isp Config
///            Platform(s) supported : ma2x8x
/// @ author   truicam
///

#include "assert.h"
#include "IspCfgGenerate.h"

void setIcIspConfig(icIspConfig *ic);

IspCfgGenerate::IspCfgGenerate() {
    // TODO Auto-generated constructor stub
//    pthread_attr_t attr;
//    int rc;
//    if((rc = sem_init(&ispCfgSemNewReq, 0, 0)) == -1)
//        printf("pthread_create: %s\n", strerror(rc));

    for (int i = 0; i < MAX_ISP_CONFIG_LIST; i++) {
        setIcIspConfig(&ispCfg[i]);
        cfgList[i].inUse = false;
        cfgList[i].ispCfg = &ispCfg[i];
    }

//    assert(0 == pthread_attr_init           (&attr));
//    assert(0 == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
//    assert(0 == pthread_attr_setschedpolicy (&attr, SCHED_RR));
//    assert(0 == pthread_create(&ispCfgThread, &attr, threadFunc, NULL));
//    assert(0 == pthread_attr_destroy   (&attr));
}

icIspConfig* IspCfgGenerate::reqCfg() {
    //if(sem_post(&ispCfgSemNewReq) == -1)  {
    //    printf("sem_post error\n");
    //}
    for (int i =0; i< MAX_ISP_CONFIG_LIST; i++) {
        if(cfgList[i].inUse == false) {
            //cfgList[i].inUse = true;
            cfgList[i].ispCfg->userData = (void*)&cfgList[i];
            return (cfgList[i].ispCfg);
        }
    }
    assert(0);
    return NULL;
}

void IspCfgGenerate::configAccepted(icIspConfig *cfg) {
    UNUSED(cfg);
}
void IspCfgGenerate::configSkipped(void* userData) {
    struct CfgList *cfgCt =  (struct CfgList *)userData;
    cfgCt->inUse = false;
}
void IspCfgGenerate::sofEvent(void) {
    //UNUSED(cfg);
}
void IspCfgGenerate::eofEvent(void) {
    //UNUSED(cfg);
}
void IspCfgGenerate::configConsumed(void* userData) {
    struct CfgList *cfgCt =  (struct CfgList *)userData;
    cfgCt->inUse = false;
}

IspCfgGenerate::~IspCfgGenerate() {
    //sem_destroy(&ispCfgSemNewReq);

    // TODO Auto-generated destructor stub
}

void* IspCfgGenerate::threadFunc(void *arg) {
    UNUSED(arg);
    while (1) {
        if(sem_wait(&ispCfgSemNewReq) == -1) {
            printf("sem_wait error\n");
        }
    }
    return NULL;
}
