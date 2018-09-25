///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool, PoPtr tests
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <Sender.h>
#include <Receiver.h>
#include <MemAllocator.h>
#include <UnitTestApi.h>
#include "ImgFrame.h"

#define N_FRM 10

#define ASSERT unitTestAssert

Pool<ImgFrame> pool;

//###############################################################
void test7_a() //pool
//###############################################################
{
  for(int k=0; k<50; k++)
  {
    pool.Create(&HeapAlloc, N_FRM, 16);
    pool.ShowRefCounts(); //DBG
    ASSERT(pool.FreeCnt()==N_FRM);

    for(int i=0; i<50; i++)
    {
      //Pulling from Pool:
       ImgFramePtr elem;
       elem.ptr = NULL;
       elem.ref = NULL;
       pool.freeFrm.Pop(NULL, &elem);
       ASSERT(pool.FreeCnt()==N_FRM-1);
     //"elem" gets destroyed here, so element will return back to pool
    }
    ASSERT(pool.FreeCnt()==N_FRM);
    pool.Stop  ();
    pool.Delete();
  }
  pool.ShowRefCounts();
}

//###############################################################
void test7_b() //Sender/Receiver
//###############################################################
{
    MReceiver <ImgFramePtr> rcv;
    pool.Create(&HeapAlloc, N_FRM, 16);
    pool.out.Link(&rcv);
    ASSERT(pool.FreeCnt()==N_FRM);

    for(int i=0; i< 50; i++)
    {
       printf("*** %d\n", i);
       ImgFramePtr e1;     ASSERT(OK==rcv.Receive(&e1));
     //mimic propagation to consumer plugins...
       ImgFramePtr e2;     e2 = e1;
       ImgFramePtr e3;     e3 = e2;
     //'e1', 'e2', 'e3' get destroyed here !
       ASSERT(pool.FreeCnt()==N_FRM-1);
    }
   ASSERT(pool.FreeCnt()==N_FRM);

   pool.Stop  ();
   pool.Delete();
   rcv .Delete();
}

//###############################################################
//manual: see destructor order (if dbg print is enabled)
void test7_c()
{
    ImgFramePtr e1("e1");
    printf("Name = %s\n", e1.name);
    {
        ImgFramePtr e2("e2");
        printf("Name = %s\n", e2.name);
        {
            ImgFramePtr e3("e3");
            printf("Name = %s\n", e3.name);
            {
                ImgFramePtr e4("e4");
                printf("Name = %s\n", e4.name);
                {
                    ImgFramePtr e5("e5");
                    printf("Name = %s\n", e5.name);
                }
            }
        }
    }
}

//###############################################################
void test7_d() //monitor pool availability
//###############################################################
{
    ASSERT(N_FRM>=6);
    MReceiver <ImgFramePtr> rcv;
    pool.Create(&HeapAlloc, N_FRM, 16);
    pool.out.Link(&rcv);

    ASSERT(pool.FreeCnt()==N_FRM);
    {
        ImgFramePtr e1("e1");
        ASSERT(OK == rcv.Receive(&e1) );
        ASSERT(pool.FreeCnt()==N_FRM-1);
        {
            ImgFramePtr e2("e2");
            ASSERT(OK == rcv.Receive(&e2) );
            ASSERT(pool.FreeCnt()==N_FRM-2);
            {
                ImgFramePtr e3("e3");
                ASSERT(OK == rcv.Receive(&e3) );
                ASSERT(pool.FreeCnt()==N_FRM-3);
                {
                    ImgFramePtr e4("e4");
                    ASSERT(OK == rcv.Receive(&e4) );
                    ASSERT(pool.FreeCnt()==N_FRM-4);
                    {
                        ImgFramePtr e5("e5");
                        ASSERT(OK == rcv.Receive(&e5) );
                        ASSERT(pool.FreeCnt()==N_FRM-5);
                        {
                            ImgFramePtr e6("e6");
                            ASSERT(OK == rcv.Receive(&e6) );
                            ASSERT(pool.FreeCnt()==N_FRM-6);
                        }//'e6' destroyed
                        ASSERT(pool.FreeCnt()==N_FRM-5);

                    }//'e5' destroyed
                    ASSERT(pool.FreeCnt()==N_FRM-4);

                }//'e4' destroyed
                ASSERT(pool.FreeCnt()==N_FRM-3);

            }//'e3' destroyed
            ASSERT(pool.FreeCnt()==N_FRM-2);

        }//'e2' destroyed
        ASSERT(pool.FreeCnt()==N_FRM-1);

    }//'e1' destroyed
    ASSERT(pool.FreeCnt()==N_FRM-0);

    pool.Stop  ();
    pool.Delete();
    rcv .Delete();
}

//###############################################################
//Pulling 4 frames from pool directly
void test7_e()
{
    ASSERT(N_FRM>=4);
    pool.Create(&HeapAlloc, N_FRM, 16);
    ASSERT(pool.FreeCnt()==N_FRM);
    {
        ImgFramePtr e1;
        ImgFramePtr e2;
        ImgFramePtr e3;
        ImgFramePtr e4;

        pool.freeFrm.Pop(NULL, &e1); printf("Base=0x%lx\n", (uint32_t)e1.ptr->base);
        pool.freeFrm.Pop(NULL, &e2); printf("Base=0x%lx\n", (uint32_t)e2.ptr->base);
        pool.freeFrm.Pop(NULL, &e3); printf("Base=0x%lx\n", (uint32_t)e3.ptr->base);
        pool.freeFrm.Pop(NULL, &e4); printf("Base=0x%lx\n", (uint32_t)e4.ptr->base);
        ASSERT(pool.FreeCnt()==N_FRM-4);
    }
    ASSERT(pool.FreeCnt()==N_FRM);
    pool.Stop  ();
    pool.Delete();
}

//###############################################################
//Pulling 4 frames from pool via MReceiver
void __attribute__((optimize("O0"))) test7_f()
{
    ASSERT(N_FRM>=4);
    MReceiver <ImgFramePtr> rcv;
    pool.Create(&HeapAlloc, N_FRM, 16);
    pool.out.Link(&rcv);

    {
        ImgFramePtr e1("e1");
        ImgFramePtr e2("e2");
        ImgFramePtr e3("e3");
        ImgFramePtr e4("e4");

        ASSERT(OK == rcv.Receive(&e1) );
        ASSERT(OK == rcv.Receive(&e2) );
        ASSERT(OK == rcv.Receive(&e3) );
        ASSERT(OK == rcv.Receive(&e4) );

        printf("[1] base=0x%lx, ref=%d\n", (uint32_t)e1.ptr->base, e1.ref->cnt);
        printf("[2] base=0x%lx, ref=%d\n", (uint32_t)e2.ptr->base, e2.ref->cnt);
        printf("[3] base=0x%lx, ref=%d\n", (uint32_t)e3.ptr->base, e3.ref->cnt);
        printf("[4] base=0x%lx, ref=%d\n", (uint32_t)e4.ptr->base, e4.ref->cnt);

        ASSERT(pool.FreeCnt() == N_FRM - 4);
    }

    pool.Stop  ();
    pool.Delete();
    rcv .Delete();
}
//###############################################################
void test7_g() //copy constructor
{
   printf("[copy_constructor_test]\n");
   MReceiver <ImgFramePtr> rcv;
   pool.Create(&HeapAlloc, N_FRM, 16);
   pool.out.Link(&rcv);

   ASSERT(pool.FreeCnt() == N_FRM);
   {
     ImgFramePtr e1("e1");
     ASSERT(OK == rcv.Receive(&e1));  ASSERT(e1.ref->cnt == 1);
     ImgFramePtr e2(e1);          ASSERT(e1.ref->cnt == 2);
     ImgFramePtr e3 = e2;         ASSERT(e1.ref->cnt == 3);
     ImgFramePtr e4(e3);          ASSERT(e1.ref->cnt == 4);
     ImgFramePtr e5(e2);          ASSERT(e1.ref->cnt == 5);
     ASSERT(pool.FreeCnt() == N_FRM-1); //just 1 receive was done
   }
   ASSERT(pool.FreeCnt() == N_FRM);

   pool.Stop  ();
   pool.Delete();
   rcv .Delete();
}

//###############################################################
void test7_hFunc3(ImgFramePtr arg)
{
    printf("___test7_hFunc3\n\n");
    ASSERT(arg.ref->cnt == 4);
}

void test7_hFunc2(ImgFramePtr arg)
{
    printf("__test7_hFunc2\n");
    ASSERT(arg.ref->cnt == 3);
    test7_hFunc3(arg);
}
void test7_hFunc1(ImgFramePtr arg)
{
    printf("_test7_hFunc1\n");
    ASSERT(arg.ref->cnt == 2);
    test7_hFunc2(arg);
}

void test7_h()
{
    printf("[PoPtr_as_fn_argument]\n");
    MReceiver <ImgFramePtr> rcv;
    pool.Create(&HeapAlloc, N_FRM, 16);
    pool.out.Link(&rcv);

    for(int i=0; i<10; i++)
    {
      ImgFramePtr e1("e1");
      ASSERT(OK == rcv.Receive(&e1));
      ASSERT(e1.ref->cnt == 1);
      test7_hFunc1(e1); //pass by value OK
      ASSERT(e1.ref->cnt == 1);
    }

    pool.Stop  ();
    pool.Delete();
    rcv .Delete();
}

//###############################################################
void test7_i() // release via nullptr !!!
//###############################################################
{
    for(int i=0; i<10; i++)
    {
        printf("Release via = nullptr\n");

        ASSERT(N_FRM>=6);
        MReceiver <ImgFramePtr> rcv;
        pool.Create(&HeapAlloc, N_FRM, 16);
        pool.out.Link(&rcv);

        ImgFramePtr e1("e1");
        ImgFramePtr e2("e2");
        ImgFramePtr e3("e3");
        ImgFramePtr e4("e4");
        ImgFramePtr e5("e5");
        ImgFramePtr e6("e6");

        ASSERT(OK == rcv.Receive(&e1) ); ASSERT(pool.FreeCnt()==N_FRM-1);
        ASSERT(OK == rcv.Receive(&e2) ); ASSERT(pool.FreeCnt()==N_FRM-2);
        ASSERT(OK == rcv.Receive(&e3) ); ASSERT(pool.FreeCnt()==N_FRM-3);
        ASSERT(OK == rcv.Receive(&e4) ); ASSERT(pool.FreeCnt()==N_FRM-4);
        ASSERT(OK == rcv.Receive(&e5) ); ASSERT(pool.FreeCnt()==N_FRM-5);
        ASSERT(OK == rcv.Receive(&e6) ); ASSERT(pool.FreeCnt()==N_FRM-6);

      //Cool stuff: release by "= nullptr" assignment
        e6 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-5);
        e5 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-4);
        e4 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-3);
        e3 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-2);
        e2 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-1);
        e1 = nullptr;   ASSERT(pool.FreeCnt()==N_FRM-0);

        pool.Stop  ();
        pool.Delete();
        rcv .Delete();
    }
}

//###############################################################
void test7_j() // null object approach
//###############################################################
{
    for(int i=0; i<10; i++)
    {
        printf("Release via = nullObject\n");

        ASSERT(N_FRM>=6);
        MReceiver <ImgFramePtr> rcv;
        pool.Create(&HeapAlloc, N_FRM, 16);
        pool.out.Link(&rcv);

        ImgFramePtr e1("e1");
        ImgFramePtr e2("e2");
        ImgFramePtr e3("e3");
        ImgFramePtr e4("e4");
        ImgFramePtr e5("e5");
        ImgFramePtr e6("e6");
        ImgFramePtr nullObj; //doesn't refer anything !!!

        ASSERT(OK == rcv.Receive(&e1) ); ASSERT(pool.FreeCnt()==N_FRM-1);
        ASSERT(OK == rcv.Receive(&e2) ); ASSERT(pool.FreeCnt()==N_FRM-2);
        ASSERT(OK == rcv.Receive(&e3) ); ASSERT(pool.FreeCnt()==N_FRM-3);
        ASSERT(OK == rcv.Receive(&e4) ); ASSERT(pool.FreeCnt()==N_FRM-4);
        ASSERT(OK == rcv.Receive(&e5) ); ASSERT(pool.FreeCnt()==N_FRM-5);
        ASSERT(OK == rcv.Receive(&e6) ); ASSERT(pool.FreeCnt()==N_FRM-6);

      //Cool stuff: release by "= nullptr" assignment
        e6 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-5);
        e5 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-4);
        e4 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-3);
        e3 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-2);
        e2 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-1);
        e1 = nullObj;   ASSERT(pool.FreeCnt()==N_FRM-0);

        pool.Stop  ();
        pool.Delete();
        rcv .Delete();
    }
}


//###############################################################
//Pool stuff
//###############################################################
void test7()
{
   for(int i=0; i<1; i++)
   {
      test7_a(); //PASS
      test7_b(); //PASS
      test7_c(); //manual
      test7_d(); //PASS
      test7_e(); //PASS
      test7_f(); //PASS
      test7_g(); //PASS
      test7_h();
      test7_i(); //nullptr
      test7_j(); //null-object
   }
}
