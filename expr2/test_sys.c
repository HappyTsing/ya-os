#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
    int count = 10000*10000;
    long ret = syscall(395,count);
    printf("result is %ld\n",ret);
    return 0;
}
