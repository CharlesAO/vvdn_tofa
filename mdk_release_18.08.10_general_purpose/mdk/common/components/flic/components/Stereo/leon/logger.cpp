

#include "logger.h"

void Logger::RegisterEvent(uint32_t &id, string str)
{
#ifdef LOGGER_ENABLED
    id = next_event_id++;
    events.push_back({id,str});
#else
    (void)id;
    (void)str;
#endif
}
void Logger::RegisterGroup(uint32_t &id, string str)
{
#ifdef LOGGER_ENABLED
    id = next_group_id++;
    groups.push_back({id,str});
#else
    (void)id;
    (void)str;
#endif
}
void Logger::AddEvent(uint32_t gid,uint32_t eid, event_type type,timespec ts,uint64_t utime)
{
#ifdef LOGGER_ENABLED
    log.push_back({gid,eid,type,ts,utime});
#else
    (void)gid;
    (void)eid;
    (void)ts;
    (void)type;
    (void)utime;
#endif
}


void Logger::ParseLog()
{

    for(event_log& e : log)
    {
       printf("%s %s type: %d ts:%lld:%ld utime:%llu\n",groups[e.group_id].group_string.c_str(),events[e.event_id].event_string.c_str(),e.type,e.ts.tv_sec,e.ts.tv_nsec,e.utime);
    }


}

void Logger::ResetLog()
{
    next_event_id = 0;
    next_group_id = 0;
    log.clear();
    groups.clear();
    events.clear();
}