#ifndef ROTATION_MATRIX_MSG_H__
#define ROTATION_MATRIX_MSG_H__

#include <PoolBase.h>

class RotMatMsg /*: public IMessage*/{
  public:
    float rotationMatrix[3];
    uint64_t timeStamp;
};

inline bool operator==(const RotMatMsg& lhs, const RotMatMsg& rhs)
{
    if((lhs.rotationMatrix[0] == rhs.rotationMatrix[0]) &&
                    (lhs.rotationMatrix[1] == rhs.rotationMatrix[1]) &&
                    (lhs.rotationMatrix[2] == rhs.rotationMatrix[2]) &&
                    (lhs.timeStamp == rhs.timeStamp))
        return true;
    else{
        return false;
    }
}


typedef PoPtr<RotMatMsg> RotMatMsgPtr;

#endif
