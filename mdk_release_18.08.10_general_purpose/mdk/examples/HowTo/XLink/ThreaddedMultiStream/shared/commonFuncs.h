#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H

#define DATA_LENGTH (3*1024*1024)
#define NUM_MESSAGES 100
#define NUM_STREAMS 2
#define NUM_THREADS_PER_STREAM 2
void* sendAndReceiveMessages(void* streamId);

void receiveAndReturnMessages(int streamId);





#endif // COMMON_FUNCS_H

