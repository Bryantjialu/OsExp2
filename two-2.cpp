// 读取

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

// FIFO 文件名
#define FIFO_PATH "fifo_file"

int main() {
    // 创建 FIFO 文件，如果存在就不再创建
    if (mkfifo(FIFO_PATH, 0666) < 0 && errno != EEXIST) {
        perror("create fifo failed");
        return -1;
    } else { 
        char cont_r[255];
        // 以只读的方式打开
        int fd = open(FIFO_PATH, O_CREAT | O_RDONLY, 0666);
        if (fd > 0) {
            while (1) {
                // 读取 FIFO 中的内容
                read(fd, cont_r, 255);
                printf("read: %s\n", cont_r);
            }
            close(fd);
        }
    }

    return 0;
}