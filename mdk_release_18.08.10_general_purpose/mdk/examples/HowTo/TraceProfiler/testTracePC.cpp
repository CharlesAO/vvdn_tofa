// g++ testTracePC.cpp -I common/swCommon/include

#include <logMsg.h>

void prnt(const char* fmt, ...){
    va_list va;
    va_start(va, fmt);
    logMessage(1, fmt, va);
    va_end(va);

}

int main(){
    logMessage(3, "test %d", 1);
    prnt("bla %d", 5);
    mv_fatal << "Critical error " << 33; 
    return 0;
}
