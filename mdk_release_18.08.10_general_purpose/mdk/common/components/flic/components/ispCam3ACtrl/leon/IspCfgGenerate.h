///
/// @file      IspCfgGenerate.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Header for the Isp Config.
/// @ author   truicam
///

#ifndef PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_LEON_ISPCFGGENERATE_H_
#define PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_LEON_ISPCFGGENERATE_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <semaphore.h>

#include "ipipe.h"
#include "IspCommon.h"

#define MAX_ISP_CONFIG_LIST 10

class IspCfgGenerate {
public:
    IspCfgGenerate();
    virtual ~IspCfgGenerate();
    icIspConfig *reqCfg();
    void configAccepted(icIspConfig *cfg);
    void configSkipped(void* userData);
    void sofEvent(void);
    void eofEvent(void);
    void configConsumed(void* userData);
private:
    icIspConfig ispCfg[MAX_ISP_CONFIG_LIST];
    struct CfgList {
        bool inUse;
        icIspConfig *ispCfg;
    };
    struct CfgList cfgList[MAX_ISP_CONFIG_LIST];
    pthread_t   ispCfgThread;
    sem_t       ispCfgSemNewReq;
    void        * threadFunc(void *);
    //struct IspCfgList {
};

#endif /* PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_LEON_ISPCFGGENERATE_H_ */
