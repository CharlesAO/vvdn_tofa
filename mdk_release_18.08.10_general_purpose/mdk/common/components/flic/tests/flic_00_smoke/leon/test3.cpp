///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Closest-Timestamp POP test
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <GetClosest.h>

class MsgX {
  public:
    uint32_t data; //don't really care
    uint64_t myTs;
};

//Must tell FLIC how to get TS from an arbitrary message struct:
template<> uint64_t ClosestTsGet<MsgX>(MsgX *m)
{
    return m->myTs;
}

//###############################################################
//Closest Timestamp
//###############################################################
void test3()
{
    for(uint32_t i=0; i<400; i += 10)
    {
        MsgBuf   <MsgX> buff;

        buff.Create(4);

        MsgX msg;
        msg.myTs = 100;  buff.Push(NULL, &msg);
        msg.myTs = 200;  buff.Push(NULL, &msg);
        msg.myTs = 300;  buff.Push(NULL, &msg);
        msg.myTs = 400;  buff.Push(NULL, &msg);

        ClosestTsCmd<MsgX> cmd;
        cmd.targetTs = i;
        buff.Pop(NULL, &msg, &cmd); printf("%ld for %ld\n", (uint32_t)msg.myTs, i);

        buff.Destroy();
        buff.Delete ();
    }
}