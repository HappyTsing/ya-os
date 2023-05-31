#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_LENGTH 1024
static cahr msgBuffer[BUFFER_LENGTH];

int main()
{
    int ret,fd;
    char toSend[BUFFER_LENGTH] = {0};
    printf("\n");
    fd = open("/dev/charMsgDriver",O_RDWR);
    if(fd<0)
    {
        perror("failed to open charMsgDriver device.\n");
        return errno;
    }
    printf("type something to the kernel module\n");
    scanf("%[^\n]%*c",toSend);
    printf("writing message to the kernel.\n");
    ret = write(fd,toSend,strlen(toSend));
    if(ret<0)
    {
        perror("failed to write the mesage\n");
        return errno;
    }
    printf("Press Enter to read back from the device.\n");
    getchar();
    printf("read from the device.\n");
    ret = read(fd,msgBuffer,BUFFER_LENGTH);
    if(ret<0)
    {
        perror("failed to read message from the device.\n");
        return errno;
    }
    printf("the msg rom device is:[%s]\n",msgBuffer);
    return 0;
}