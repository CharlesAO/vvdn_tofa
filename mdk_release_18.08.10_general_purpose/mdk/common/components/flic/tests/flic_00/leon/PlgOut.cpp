///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy test output
///

#include <PlgOut.h>

//#############################################
void PlgOut::Create()
{
   //...
}

//#############################################
void * PlgOut::threadFunc(void *)
{
    int i = 300;

    while(1){
        printf("%d>>>>\n", i);
        i++;
        sleep(1);
    }

    return NULL;
}