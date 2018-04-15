#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int msgId = atoi(argv[1]);
    if ((msgctl(msgId, IPC_RMID, NULL)) < 0)
    {
        printf("error in msgctl %s", __FILE__);
        exit(1);
    }
    printf("delete msg queue:%d\n", msgId);

    return 0;
}