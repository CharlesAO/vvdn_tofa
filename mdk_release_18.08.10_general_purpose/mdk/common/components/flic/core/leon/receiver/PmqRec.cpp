///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Posix MQ-based receiver (for POD messages only !)
///

#include <PmqRec.h>
#include <stdio.h>
#include <sys/types.h>
#include <FlicAtomic.h>

//#####################################################################
// Automatically generate mq names (for now a 4 char "qXXX")
// using chars in range [33..126] meaning ['!'..'~']
void getNextName(char *name)
{
    static int c1 = 33;
    static int c2 = 33;
    static int c3 = 33;

    Atomic a;
    a.Enter();
      c1++;
      if(c1 == 127){c1 = 33; c2++;}
      if(c2 == 127){c2 = 33; c3++;}
      if(c3 == 127){c3 = 33; c1++;}
    a.Leave();

    sprintf(name, "q%c%c%c", c3,c2,c1);
  //printf("getNextName : %s\n", name);
}
