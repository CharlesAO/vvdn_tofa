///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Smoke test
///

#include <rtems.h>
#include <stdlib.h>
#include <UnitTestApi.h>

void test1(); //create/use/destroy cycle
void test2(); //code generation estimation for MsgBuf
void test3(); //pop closest TS
void test4(); //overwrite oldest
void test5(); //mSend->sReceive
void test6(); //code generation estimation for MSnd/SRecv
void test7(); //pool/PoPtr
void test8(); //deep copy
void test9(); //timed Send/Receive
void testA(); //Sender error codes

//############################################
extern "C" void *POSIX_Init (void *args)
{
    unitTestInit();
    UNUSED(args);

    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    testA();

    unitTestFinalReport();
    exit(0);
}
