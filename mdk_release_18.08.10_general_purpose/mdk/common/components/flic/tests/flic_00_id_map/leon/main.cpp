///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Print ID strings
///

#include <Flic.h>

const NamedId FLIC_ID(ct1);
const NamedId FLIC_ID(ct2);
const NamedId FLIC_ID(ct3);
const NamedId FLIC_ID(ct4);

//############################################
extern "C" void *POSIX_Init (void *)
{
    printf("ct1 string = %s\n", ct1.s);
    printf("ct2 string = %s\n", ct2.s);
    printf("ct3 string = %s\n", ct3.s);
    printf("ct4 string = %s\n", ct4.s);
    exit(0);
}
