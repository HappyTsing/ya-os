#include <stdio.h>
#include <unistd.h>
#include <time.h>
long exec_oper(int* result, int num1, int num2, char* op){
    if(op)
    {
        if(*op == '+')
            *result = num1 + num2;
        else if(*op == '-')
            *result = num1 - num2;
        else if(*op == '*')
            *result = num1 * num2;
        else if(*op == '\\')
        {
            if(num2 != 0)
                *result = num1 / num2;
            else
                printf("divided number can not be zero. \n");
        }

    }
    else
    {
        printf("operator is empty. \n");
    }
    return 0;
}

int main()
{
    int count = 10000*10000;
    clock_t start, finish; 
    double  duration;

    start = clock(); 
    printf("conut is: %d\n",count);  // printf 耗时，进入内核态
    finish = clock(); 
    printf( "printf: %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC ); 


    start = clock(); 
    int i;
    int result;
    finish = clock(); 
    printf( "赋值: %f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC ); 


    int j;
    j=0;
    start = clock(); 
    for(i=0;i<count;++i)
    {
        // 多一个加法，就会多 0.15 秒的时间。
        char op_add = '+';
        ++j;
        // j++;
        exec_oper(&result,10,10,&op_add);
    }
    printf("j is %d\n",j);
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "循环 %f seconds\n", duration ); 
    return 0;
}
