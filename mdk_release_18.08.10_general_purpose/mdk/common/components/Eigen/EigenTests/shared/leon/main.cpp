#include <stdio.h>
#include <DrvSvu.h>
#include "Shave.h"
#include "app_config.h"
#include <stdlib.h>
#include <iostream>
#include <UnitTestApi.h>
#include "DrvTimer.h"
#include <DrvShaveL2Cache.h>

#include <swcShaveLoader.h>
#include <swcTestUtils.h>
#include <swcCrc.h>

using namespace std;

#define SHAVE_USED 0

extern unsigned int eigenTest0_main;


int main(void)
{

  // DrvTimerInit();
  // u64 cycles_elapsed;
   performanceStruct perfStr;
  // tyTimeStamp timer_data;
   
    s32 sc;
    unitTestInit();
    sc = initClocksAndMemory();
    if(sc)
        return sc;

    //DrvTimerStartTicksCount(&timer_data);
    
    Shave *shave;
    Shave *test;

    //This is an example usecase of c++ and shave
    shave = Shave::gethandle(SHAVE_USED);
    test = Shave::gethandle(SHAVE_USED);

    if (((u32)shave)!=((u32)test)){
      printf("Error in singleton design pattern!\n");
    }


    swcShaveProfInit(&perfStr);
    shave->init();
    shave->reset();
    shave->setdefaultstack();

    swcShaveProfStartGathering(0, &perfStr);
    shave->start((u32)&eigenTest0_main);
    shave->wait();
    shave->deinit();
    swcShaveProfStopGathering(0, &perfStr);

    //Retrieving i18 register with the return of the eigenTest0_main.
    int testStatus = DrvSvutIrfRead(0,18);
    if(testStatus == 0)
      unitTestLogPass();
    else
      unitTestLogFail();

    Shave::destroy(SHAVE_USED);
    unitTestFinalReport();

  // DrvTimerGetElapsedTicks(&timer_data, &cycles_elapsed);
  //cout<<"Nr. of cycles : "<< (int)cycles_elapsed << endl;

    return 0;


}
