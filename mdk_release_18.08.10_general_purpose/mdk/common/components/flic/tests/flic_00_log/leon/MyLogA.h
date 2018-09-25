///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy printf based logger
///

#ifndef __MY_LOG_A_H__
#define __MY_LOG_A_H__

#include <Flic.h>
#include <stdarg.h>

class MyLogA : public IBuffLog<MyMsg>
{
   public:
    void LogPushBegin(Io *w, MyMsg *m){
        UNUSED(m);
        if(1) printf("__push_start on [%.8s]\n",(char*)&w->name);
    }
    void LogPopBegin (Io *w){
        if(1) printf("__pop_start on [%.8s]\n", (char*)&w->name);
    }
    void LogPushEnd(Io *w, MyMsg *m, int rc){
        printf("PUSH .data = %3ld from %.8s (rc = %d)\n", m->data, (char*)&w->name, rc);
    }
    void LogPopEnd(Io *w, MyMsg *m, int rc){
        printf("POP  .data = %3ld from %.8s (rc = %d)\n", m->data, (char*)&w->name, rc);
    }
    void Print(const char *fmt, ...){
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end (args);
    }

};

#endif