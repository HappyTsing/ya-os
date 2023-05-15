# expr1 


实现 hello 内核模块，在该内核模块中实现：

- 装载时输出 "hello world"
- 卸载时输出 "goodbye"
- 具有插入、删除、查找、打印操作的单链表

---

> https://space.bilibili.com/476114390/video

内核头文件包位于 `/usr/src` 中，在 Makefile 中包含该文件。

命令：
- uname -a: 查看内核版本
- lsmod: 其本质是查询 `/proc/modules`
- insmod module_name.ko: 每装载一个 module，会在 `/sys/module` 下创建一个文件夹
- rmmod module_name
- dmesg: 查看模块加载/卸载的输出


`printk` 的日志级别：
```c
#define KERN_EMERG    "<0>"    /* system is unusable */
#define KERN_ALERT    "<1>"    /* action must be taken immediately */
#define KERN_CRIT     "<2>"    /* critical conditions */
#define KERN_ERR      "<3>"    /* error conditions */
#define KERN_WARNING  "<4>"    /* warning conditions */
#define KERN_NOTICE   "<5>"    /* normal but significant */
#define KERN_INFO     "<6>"    /* informational */
#define KERN_DEBUG    "<7>"    /* debug-level messages */
```

# expr2

新增系统调用，并重新编译内核。

---

> https://blog.csdn.net/m0_37194132/article/details/85043348

系统调用，指调用内核态的函数。由于用户态无法直接调用内核态函数，因此采用中断的方式从用户态陷入到内核态。通过软中断 0x80，系统会跳转到预设好的内核空间地址，该地址指向系统调用处理函数（entry.S/system_call) 。

通俗的说，所有的系统调用最终都会统一跳转执行 system_call 函数。

此外，内核中存在函数指针数组 system_call_table 数组，该数组的下标是 `系统调用号`，数组中存储的指针指向系统调用的服务例程（即系统调用的目标函数）。

当中断 0x80 发生时，系统调用号会被放入 eax 寄存器中，system_call 函数读取 eax 寄存器获取 `系统调用号`，将其乘4得到偏移地址，以 system_call_table 为基地址，基地址加上偏移地址就是应该执行的系统调用服务例程的地址。

**添加系统调用: 编译内核**

```sh
# 下载源码
cd /usr/src
sudo wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v4.x/linux-4.15.3.tar.xz
sudo tar xvJf linux-4.15.3.tar.xz

# 1. 新增系统调用号
cd /usr/src/linux-4.15.3/arch/sh/include/uapi/asm
sudo vim unistd_64.h
#define __NR_four_oper 395

# 2. 修改系统调用向量表 <number> <abi> <name> <entry point>
cd /usr/src/linux-4.15.3/arch/x86/entry/syscalls
sudo vim syscall_64.tbl
395 64 four_oper sys_four_oper

# 3. 添加系统调用声明
cd /usr/src/linux-4.15.3/include/linux
sudo vim syscalls.h
asmlinkage long sys_four_oper(int count); # 添加在 #endif 前

# 4. 添加系统调用实现
cd /usr/src/linux-4.15.3/kernel
sudo vim sys.c
SYSCALL_DEFINE1(four_oper, int, count){}
long exec_oper(int* result, int num1, int num2, char* op){}

# 5. 安装依赖
sudo apt install libncurses5-dev libssl-dev build-essential openssl  zlibc minizip libidn11-dev libidn11 libelf-dev

# 6. 编译内核
cd /usr/src/linux-4.15.3
sudo make mrproper 
sudo make clean 
sudo make menuconfig # 保持终端宽度最大, 可修改 General setup 中的版本号，选中 64-bit kernel ，再 <save>
sudo make –j32 # CPU核数: 32 
# 安装内核模块
sudo make modules_install
# 安装内核
sudo make install
```

**内核切换**

> https://blog.csdn.net/weixin_40837318/article/details/123798456

```sh
# 查看当前内核 id
grep submenu /boot/grub/grub.cfg
uname -a

# 查看所有内核 id
grep gnulinux /boot/grub/grub.cfg

# 切换内核
sudo vim /etc/default/grub

# 更新 重启
sudo update-grub
```

函数系统调用实现时，采用 `SYSCALL_DEFINE1` 定义，当然还有 `SYSCALL_DEFINE2` 等等，数字表示系统调用的参数个数，第一个参数表示函数名，且 `SYSCALL_DEFINE`  中的 `int, count` 之间存在逗号！

1w 次调用全部放在内核，再与 1w 次用户态的四则运算的时间进行比较。

> Q：为什么内核态执行比用户态的时间少？ 
> A: 发生了用户态到内核态的转变