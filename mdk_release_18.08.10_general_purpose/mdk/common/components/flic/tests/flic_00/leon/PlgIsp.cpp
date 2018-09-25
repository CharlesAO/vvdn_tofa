///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup ISP plugin
///

#include <PlgIsp.h>

//#############################################
void PlgIsp::Create()
{
  //Create ...
}

//#############################################
void * PlgIsp::threadFunc(void *)
{
    int i = 100;

    while(1){
        printf("%d\n", i);
        i++;
        sleep(1);
    }

    return NULL;
}