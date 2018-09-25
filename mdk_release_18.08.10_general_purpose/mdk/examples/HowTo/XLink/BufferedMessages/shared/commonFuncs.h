#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H

#define NUM_MESSAGES 100
#define DATA_LENGTH (3*1024*1024)
bool sendAndReceiveMessages(int streamId);

void receiveAndReturnMessages(int streamId);





#endif // COMMON_FUNCS_H

