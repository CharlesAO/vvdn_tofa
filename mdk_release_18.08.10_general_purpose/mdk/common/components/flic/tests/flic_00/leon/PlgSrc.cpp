///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy test source
///

#include <PlgSrc.h>

//#############################################
void PlgSrc::Create()
{
  //
}

//#############################################
void * PlgSrc::threadFunc(void *)
{
    int i = 200;

    while(1){
        printf("%d__\n", i);
        i++;
        sleep(1);
    }

    return NULL;
}