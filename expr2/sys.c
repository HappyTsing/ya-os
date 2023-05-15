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
                printk("divided number can not be zero. \n");
        }

    }
    else
    {
        printk("operator is empty. \n");
    }
    return 0;
}
SYSCALL_DEFINE1(four_oper, int, count){
    printk("conut is: %d\n",count);
    struct timeval tstrat,tend;
    do_gettimeofday(&tstrat);
    int i;
    int result;
    int times = count/4;
    for(i=0;i<times;++i)
    {
        char op_add = '+';
        exec_oper(&result,10,10,&op_add);
    }
    printk("four_oper op_add is ok. op_add count is:%d\n",i);
    for(i=0;i<times;++i)
    {
        char op_sub = '-';
        exec_oper(&result,20,10,&op_sub);
    }
    printk("four_oper op_sub is ok. op_sub count is:%d\n",i);
    for(i=0;i<times;++i)
    {
        char op_mul = '*';
        exec_oper(&result,10,10,&op_mul);
    }
    printk("four_oper op_mul is ok. op_mul count is:%d\n",i);
    for(i=0;i<times;++i)
    {
        char op_div = '\\';
        exec_oper(&result,20,10,&op_div);
    }
    printk("four_oper op_div is ok. op_div count is:%d\n",i);
    do_gettimeofday(&tend);
     // tv_sec 表示秒，距 1970:01:01:0000 的时差，不足1秒的的部分用微秒表示，存在 tv_usec 中，取值范围为 0～999999
    printk("four_oper running time is %ld usec/n",1000000*(tend.tv_sec-tstrat.tv_sec)+(tend.tv_usec-tstrat.tv_usec));
    return 0;
}