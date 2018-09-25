#ifndef _STEREO_LOGGER_H
#define _STEREO_LOGGER_H

#include <vector>
#include <stdint.h>
#include <string>

using namespace std;
enum event_type { START_PROC = 0, END_PROC, SEND, RECV};

typedef struct
{
    uint32_t event_id;
    string event_string;
} event;

typedef struct
{
    uint32_t group_id;
    string group_string;
} group;

typedef struct
{
    uint32_t group_id;
    uint32_t event_id;
    event_type type;
    timespec ts;
    uint64_t utime;
} event_log;

class Logger // Singleton
{
public:
    void RegisterEvent(uint32_t &id, string str);
    void RegisterGroup(uint32_t &id, string str);
    void AddEvent(uint32_t gid,uint32_t eid, event_type type,timespec ts,uint64_t utime);
    void ParseLog();
    void ResetLog();
    static Logger& getInstance()
    {
        static Logger instance; 
        return instance;
    }
    Logger(Logger const&)          = delete;
    void operator=(Logger const&)  = delete;
private:
    Logger() {}
vector <event> events;
vector <group> groups;
vector <event_log> log;
uint32_t next_event_id = 0;
uint32_t next_group_id = 0;

};

#endif // _STEREO_LOGGER_H




