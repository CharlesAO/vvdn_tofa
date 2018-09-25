#ifndef PC_CFG_MSG_H
#define PC_CFG_MSG_H

enum MESSAGE_TYPE{
    HOMOGRAPHY=0,
    CONFIDENCE
};

struct PCMessage{
    MESSAGE_TYPE type;
    uint32_t seq_num;
    char data[36];
};


#endif
