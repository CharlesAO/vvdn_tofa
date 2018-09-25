#if 0
#include <vector>

#include "profile.h"

class ProfilerInit {
    public:
        ProfilerInit(){
#ifdef SHAVE_FPROFILE
            void (*preRunInit)();
            preRunInit();
#elif defined(LRT_PROFILE) || defined(LOS_PROFILE)
            void __leonProfilerInit(void);
            __leonProfilerInit();
#endif
        }
        ~ProfilerInit(){
            __profilerEnd();
        }
};

/*static*/ ProfilerInit _profiler;
#endif