///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     User custom ::Receive() example
///

#include <Flic.h>
#include <UnitTestApi.h>
#include <MsgBuf.h>
#include <BehQueue.h>

struct MyMsg
{
  uint32_t custField;
};

//################################################
//Customize Pop to return message with custField closest to arg.
template<typename T> int _pop(MsgBuf<T> *b, int rIdx, T *elem);

class CustCmd : public QueueCmd<MyMsg>
{
  public :
     uint32_t target;

     virtual int doPop(MsgBuf<MyMsg> *b, MyMsg *m, TimeSpec *ts)
     {
        int rc;
        Atomic a;

        UNUSED(ts);
        printf("[CustCmd::doPop]\n");
        rc = sem_wait(&b->semUsed);
        if(rc == OK){
           a.Enter();

         //1) Find best match
           int idx = 0;
           int minDiff = 100000000;

           printf("finding msg closet to %ld\n", target);
           for(uint32_t i=0; i<b->nElem; i++)
           {
             if(b->order[i] != EMPTY_SLOT)
             {
               //assert(0 == strstr(b->mssg[0]->GetName(), m->GetName()));
               MyMsg* cur = (MyMsg*)b->mssg[i];

               printf("%ld\n", cur->custField);
               int diff = (int)cur->custField - (int)target;
               if(diff < 0) diff = -diff;
               if(diff < minDiff){
                   minDiff = diff;
                   idx = i;
               }
             }
           }

         //2) The actual pop
           rc += _pop(b, idx, m);
         //3) Only now decrement number of remaining items
           assert(b->semUsedV>0); b->semUsedV--;
           a.Leave();

        }
        return rc;
     };
};
//################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

    MsgBuf<MyMsg> mq;
    mq.Create(4);

    MyMsg m;
    m.custField = 111; mq.Push(NULL, &m);
    m.custField = 222; mq.Push(NULL, &m);
    m.custField = 333; mq.Push(NULL, &m);

    CustCmd cc;
    cc.target = 250;

    MyMsg ret;
    unitTestAssert(mq.Count() == 3);
    mq.Pop(NULL, &ret, &cc);
    unitTestAssert(mq.Count() == 2);
    printf("Popped msg with custField = %ld\n", ret.custField);
    unitTestAssert(ret.custField == 222);

    unitTestFinalReport();
    exit(0);
}
