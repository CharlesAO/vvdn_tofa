///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Independent plugins (unlinked) test
///

#include <Flic.h>
#include <PlgAaa.h>
#include <PlgSrc.h>
#include <PlgIsp.h>
#include <PlgOut.h>

PlgSrc   plgSrc;
PlgAaa   plgAaa;
PlgIsp   plgIsp;
PlgOut   plgOut;
Pipeline p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    plgAaa.Create();
    plgSrc.Create();
    plgIsp.Create();
    plgOut.Create();

    p.Add(&plgAaa);
    p.Add(&plgSrc);
    p.Add(&plgIsp);
    p.Add(&plgOut);

    p.Start();
    p.Wait();

    exit(0);
}
