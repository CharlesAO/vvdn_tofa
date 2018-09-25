#ifndef CFG_MSG_H
#define CFG_MSG_H
#include "Pool.h" //PoPtr

enum configMessageType{
    CFG_HOMOGRAPHY=0,
    CFG_CONFIDENCE //more will be added
};

struct ConfigMessage{
    configMessageType type;
    uint32_t seq_num;
    char data[36];
};

typedef PoPtr<ConfigMessage> MessagePtr;

inline bool operator==(const MessagePtr& lhs, const MessagePtr& rhs)
{
    if((lhs.ptr         == rhs.ptr  ) &&
      (lhs.ref          == rhs.ref  ) &&
      (lhs.pool         == rhs.pool ) &&
      (lhs.ptr->seq_num == rhs.ptr->seq_num)){
        return true;
      }
   else{
       return false;
  }
}

inline bool operator==(const ConfigMessage& lhs, const ConfigMessage& rhs)
{
    if((lhs.seq_num         == rhs.seq_num  ) &&
      (lhs.type          == rhs.type  )){
        return true;
      }
   else{
       return false;
  }
}
#endif
