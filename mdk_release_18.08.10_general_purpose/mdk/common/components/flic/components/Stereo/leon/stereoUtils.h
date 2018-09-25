
#ifndef STEREO_UTILS_H

string get_unique_str(uint64_t flic_id,const char *threadname);
timespec my_get_time();
uint64_t getTimeStamps();
double getTimeStampsTicks(void);
#define STEREO_UTILS_H
#endif