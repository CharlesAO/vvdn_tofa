#ifndef _HWFILT_MED_H_
#define _HWFILT_MED_H_

extern "C" void initMedHw(u8* dst, u8* src, u32 width, u32 height, u32 med_k);
extern "C" void runMedHw();

#endif // _HWFILT_MED_H_
