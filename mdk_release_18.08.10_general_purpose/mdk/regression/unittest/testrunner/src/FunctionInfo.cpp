#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "FunctionInfo.h"

std::auto_ptr<FunctionInfo> FunctionInfo::instance(0);

FunctionInfo::FunctionInfo() {}

unsigned int FunctionInfo::useUserSeed = 0;
unsigned int FunctionInfo::randSeed = 0;
float FunctionInfo::expectedCycles = 0;
