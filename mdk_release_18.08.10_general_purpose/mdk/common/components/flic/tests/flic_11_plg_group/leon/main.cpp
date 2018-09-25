///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Plugin Group
///

#include <Flic.h>
#include "PlgSrc.h"
#include "PlgSnk.h"
#include "PlgBuf.h"
#include "PlgGrp.h"
#include <UnitTestApi.h>

PlgSrc  plgSrc;
PlgSnk  plgSnk;
Pipeline     p;

//discrete plugs
PlgBuf  plgBufA;
PlgBuf  plgBufB;
PlgBuf  plgBufC;
PlgBuf  plgBufD;
PlgBuf  plgBufE;
PlgBuf  plgBufF;

//preexisting groups
PlgGrp  plgGrp1;
PlgGrp  plgGrp2;

//###########################################
void run_pipe()
{
    p.Start();
    sleep(2);
    p.Stop();
    p.Wait();
    p.Delete();
}

//###########################################
void test_discrete()
{
    printf("Using discrete plugs ... \n");

    plgSrc .Create();
    plgSnk .Create();
    plgBufA.Create();
    plgBufB.Create();
    plgBufC.Create();
    plgBufD.Create();
    plgBufE.Create();
    plgBufF.Create();

    p.Add(&plgSrc);
    p.Add(&plgSnk);
    p.Add(&plgBufA);
    p.Add(&plgBufB);
    p.Add(&plgBufC);
    p.Add(&plgBufD);
    p.Add(&plgBufE);
    p.Add(&plgBufF);


    plgSrc .out.Link(&plgBufA.in);
    plgBufA.out.Link(&plgBufB.in);
    plgBufB.out.Link(&plgBufC.in);
    plgBufC.out.Link(&plgBufD.in);
    plgBufD.out.Link(&plgBufE.in);
    plgBufE.out.Link(&plgBufF.in);
    plgBufF.out.Link(&plgSnk .in);

    run_pipe();
}

//###########################################
void test_plg_group()
{
    printf("Using plug-group ... \n");

    plgSrc .Create();
    plgSnk .Create();
    plgGrp1.Create();
    plgGrp2.Create();

    p.Add(&plgSrc);
    p.Add(&plgSnk);
    plgGrp1.AddTo(&p);
    plgGrp2.AddTo(&p);

    plgSrc.out.  Link(plgGrp1.in);
    plgGrp1.out->Link(plgGrp2.in);
    plgGrp2.out->Link(&plgSnk.in);

    run_pipe();
}

//###########################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    for(int i=0;i<N_RUNS;i++) test_discrete();
    for(int i=0;i<N_RUNS;i++) test_plg_group();

    unitTestFinalReport();
    exit(0);
}
