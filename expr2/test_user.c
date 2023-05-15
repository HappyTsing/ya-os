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
    printf("conut is: %d\n",count);
    clock_t start, finish;  
    double  duration;
    start = clock(); 
    int i;
    int result;
    int times = count/4;
    for(i=0;i<times;++i)
    {
        char op_add = '+';
        exec_oper(&result,10,10,&op_add);
    }

    printf("four_oper op_add is ok. op_add count is:%d\n",i);

    for(i=0;i<times;++i)
    {
        char op_sub = '-';
        exec_oper(&result,20,10,&op_sub);
    }
    printf("four_oper op_sub is ok. op_sub count is:%d\n",i);
    for(i=0;i<times;++i)
    {
        char op_mul = '*';
        exec_oper(&result,10,10,&op_mul);
    }
    printf("four_oper op_mul is ok. op_mul count is:%d\n",i);
    for(i=0;i<times;++i)
    {
        char op_div = '\\';
        exec_oper(&result,20,10,&op_div);
    }
    printf("four_oper op_div is ok. op_div count is:%d\n",i);
    finish = clock(); 
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration ); 
    return 0;
}
