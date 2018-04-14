#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

int main(int argc, char** argv)
{
    FILE *rdFP = NULL;
    FILE *writeFP = NULL;
    char buf[BUFSIZ + 1];
    int num_of_chars = 0;

    memset(buf, '\0', sizeof(buf));

    //打开ls作为读接口
    rdFP = popen("ls -l", "r");
    if(!rdFP)
    {
        ERR_EXIT("failed to open read pipe");
    }
    //打开grep作为写接口
  wrFP = popen("grep \\\\-rw-rw-r--", "w");
    if(!wrFP)
    {
        ERR_EXIT("failed to open write pipe");
    }
    if(rdFP && wrFP)
    {
        //从ls读取BUFSIZ字符
        num_of_chars = fread(buf, sizeof(char), BUFSIZ, rdFP);
        while(num_of_chars > 0)
        {
            buf[num_of_chars] = '\0';
            //把数据写入grep
            fwrite(buf, sizeof(char), num_of_chars, wrFP);
            //循环读取数据直到读完所有数据
            num_of_chars = fread(buf, sizeof(char), BUFSIZ, rdFP);
        }
        //关闭文件流
        pclose(rdFP);
        pclose(wrFP);
    }
    exit(EXIT_SUCCESS);
}