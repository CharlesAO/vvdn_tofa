///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     TNF(Temporal Noise Filtering) example:
///            - 3 input frames are used to produce 1 output frame
///            - at each output frame, just one input is released, the
///              others are needed for future output

#include <Flic.h>
#include <Pool.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>
#include "PlgEcho.h"
#include "PlgProc.h"
#include <ImgFrame.h>

PlgPool<ImgFrame> plgPool1;//isp in pool
PlgPool<ImgFrame> plgPool2;//isp out pool
PlgEchoSend       plgSend; //src
PlgProc           plgProc; //isp
PlgEchoRecv       plgRecv; //out
Pipeline          p;

//############################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    plgSend .Create();
    plgProc .Create();
    plgPool1.Create(&HeapAlloc, 5, 128);
    plgPool2.Create(&HeapAlloc, 5, 128);
    plgRecv .Create();

    p.Add(&plgSend);
    p.Add(&plgProc);
    p.Add(&plgPool1);
    p.Add(&plgPool2);
    p.Add(&plgRecv);

    plgPool1.out.Link(&plgSend.in );
    plgPool2.out.Link(&plgProc.inO);
    plgSend .out.Link(&plgProc.inI);
    plgProc .out.Link(&plgRecv.in );

    p.Start();
    sleep(20);

    p.Stop();
    p.Wait();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
