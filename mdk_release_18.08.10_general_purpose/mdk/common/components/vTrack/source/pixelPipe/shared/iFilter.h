#ifndef I_FILTER_H
#define I_FILTER_H

class IFilter {
public:
    virtual void startTask(u32 line)=0;
    virtual void postprocessLastResults()=0;
    virtual void waitTask()=0;
    virtual void endFrame()=0;
};





#endif
