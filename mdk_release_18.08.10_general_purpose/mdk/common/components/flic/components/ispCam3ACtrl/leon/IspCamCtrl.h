/*
 * IspCamCtrl.h
 *
 *  Created on: Feb 9, 2017
 */

#ifndef PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_ISPCAMCTRL_H_
#define PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_ISPCAMCTRL_H_

#include "DrvSensor.h"
#include "ipipe.h"
#include "IspCam3ACtrl.h"

class IspCamCtrl {
public:
    IspCamCtrl(const char *cam);
    virtual ~IspCamCtrl();
    icSource src;
    void startCam(void);
    void stopCam(void);
    const char *name;
    int fileDesc; // file description
private:
    void initSourceConfig();
};

#endif /* PROJECTS_IPIPE3_COMPONENTS_ISPCAM3ACTRL_ISPCAMCTRL_H_ */
