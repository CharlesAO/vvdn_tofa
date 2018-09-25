#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <mqueue.h>

#include "helper.h"

void createMsgQueue(const char *name, int maxmsg, int msgsize) {
    mq_unlink(name);
    /// Open fifo
    struct mq_attr attr;
    attr.mq_maxmsg = maxmsg;
    attr.mq_msgsize = msgsize;
    attr.mq_flags = 0;
    mqd_t mqdes = mq_open(name, O_RDWR | O_CREAT, 0664, &attr);
    if (mqdes == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }
    mq_close(mqdes);
}
