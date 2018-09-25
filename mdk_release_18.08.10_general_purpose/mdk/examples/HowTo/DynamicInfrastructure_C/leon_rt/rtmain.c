/*Leon RT main app*/

#include <stdio.h>
#include <DrvLeon.h>
#include <swcShaveLoader.h>
#include "BayerISPModel.h"
#include "BayerPipelineParams.h"
#include "theDynContext.h"

#define NO_OF_MESG_APP_INSTANCES_THREE 2
BayerISPModel myPl;

extern unsigned char MesgThreeappdyndata[];
extern unsigned char DEFAULT_SHAVE_GROUPgrpdyndata[];

extern DynamicContext_t MODULE_DATA(MesgThree);

unsigned int __attribute__((section(".cmx.bss"))) someRTArray[20];

int main(void) {
    int i;
    int shaveUsed[NO_OF_MESG_APP_INSTANCES_THREE];
    int status[NO_OF_MESG_APP_INSTANCES_THREE];
    /* these are the shaves we want to use */
    swcShaveUnit_t shaveListMesgThree[NO_OF_MESG_APP_INSTANCES_THREE] = {1,2};

    printf("And now from LeonRT.\n");

    status[0] = swcSetupDynShaveApps(&MODULE_DATA(MesgThree), shaveListMesgThree, NO_OF_MESG_APP_INSTANCES_THREE);
    if (status[0] != MYR_DYN_INFR_SUCCESS)
        MYR_DYN_INFR_CHECK_CODE(status[0]);

    for (i = 0; i < NO_OF_MESG_APP_INSTANCES_THREE; i++) {
        status[i] = swcRunShaveAlgo(&MODULE_DATA(MesgThree), &shaveUsed[i]);
        if (status[i] != MYR_DYN_INFR_SUCCESS)
            MYR_DYN_INFR_CHECK_CODE(status[i]);
        swcWaitShave(shaveUsed[i]);
    }

    printf("And let's see a SIPP pipeline working:\n");
    sippInitialize();
    buildBayerISPModel(&myPl);
    configBayerISPModel(&myPl);
    readBayerISPModelInput(&myPl);
    sippProcessFrame(myPl.pl);
    writeBayerISPModelOutput(&myPl);
    printf("SIPP pipeline ran\n");

    /* free the Leon RT applications memory */
    status[0] = swcCleanupDynShaveApps(&MODULE_DATA(MesgThree));
    if (status[0] != MYR_DYN_INFR_SUCCESS)
        MYR_DYN_INFR_CHECK_CODE(status[0]);

    DrvLeonRTSignalStop();
    return 0;
}
