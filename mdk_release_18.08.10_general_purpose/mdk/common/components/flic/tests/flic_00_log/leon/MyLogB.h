///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     mvLog based logger
///

#ifndef __MY_LOG_B_H__
#define __MY_LOG_B_H__

#include <Flic.h>
#include <stdarg.h>
#include <mvLog.h>

class MyLogB : public IBuffLog<MyMsg>
{
   public:
    mvLog_t level;
    MyLogB(){level = MVLOG_ERROR;}

    void LogPushEnd(Io *w, MyMsg *m, int rc){
        mvLog(level, "PuSh .dat = %3ld on %.8s (rc = %d)", m->data, (char*)&w->name, rc);
    }
    void LogPopEnd(Io *w, MyMsg *m, int rc){
        mvLog(level, "PoP  .dat = %3ld on %.8s (rc = %d)", m->data, (char*)&w->name, rc);
    }
    void Print(const char *fmt, ...)
    {
        char msg[128];
        va_list args;
        va_start(args, fmt);
        vsnprintf(msg, sizeof(msg), fmt, args);
        va_end  (args);
        //Trim trailing '\n' if exists (as mvLog adds a \n anyway)
        int lastChar = strlen(msg)-1;
        if(lastChar > 0)
         if(msg[lastChar] == '\n')
            msg[lastChar] = 0;

        mvLog(level, msg);
    }
};

#endif