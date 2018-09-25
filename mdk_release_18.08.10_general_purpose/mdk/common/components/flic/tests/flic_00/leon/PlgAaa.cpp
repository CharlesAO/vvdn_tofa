///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Mockup 3A plugin
///

#include <PlgAaa.h>

//#############################################
void PlgAaa::Create()
{
   // ...
}

//#############################################
void * PlgAaa::threadFunc(void *)
{
    while(1){
        printf("3A\n");
        sleep(1);
    }

    return NULL;
}