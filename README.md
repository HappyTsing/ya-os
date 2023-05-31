# expr1 

**实验内容**

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

**实验内容**

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

# expr3

**实验内容**

- 编写一个完成整数四则运算的动态模块
- 使用insmod和rmmod命令插入、删除模块
- 在模块初始化函数内，定义全局函数指针变量，令其指向模块运算函数，最后导出全局指针变量到全局符号表
- 新增一个动态模块，同实验2中的系统功能调用具有相同的接口。该动态模块调用被导出的函数指针，间接调用动态模块中的四则运算函数，完成整数四则运算
- 进行等量四则运算，测试，同实验二的测试结果对比

---

**内核模块与应用程序的区别**

- 执行方式: 应用程序由main函数开始执行，流程基本有程序自身控制; 驱动函数没有main函数，由回调方式驱动运行
- 回调方式: 先向内核注册函数，然后应用程序触发这些函数的执行

**内核模块的编写规则**


- 模块不能像应用程序那样在系统平台下自由运行和结束，而是被系统的内核调用运行和释放。因此模块必须提供两个函数给内核调用：
  - init_module()
  - cleanup_module()
- 必须提供一种途径向内核告知自己是什么模块
  - MODULE_LICENSE() 宏函数用来声明自己是遵循何种规则的模块
- 模块初始化可以设定或模块之间调用，可使用：
  - module_param()
  - EXPORT_SYMBOL_GPL()
- 为了便于使用者方便使用该模块，可以提供一些模块使用信息说明，如：
  - MODULE_DESCRIPTION()
  - MODULE_AUTHOR()
  - MODULE_DEVICE_TABLE()
  - MODULE_VERSION()
  - MODULE_ALIAS()
> init_module()、cleanup_module()和 MODULE_LICENSE()必须的，其它都是根据需要可选择的



```
# todo 测试执行
cd expr3
make

```

# expr4

**实验内容**

1. 在linux环境下编写实验一个基于内存的虚拟字节设备驱动程序，具备打开、关闭、读、写的功能
- 设备注册
- 设备操作的实现
- 设备注销
2. 安装字符驱动模块
3. 编写用户测试程序，测试安装的字符驱动

---

**设备驱动程序**

设备驱动程序，是一种可以使计算机和设备通信的特殊程序，可以说相当于硬件的接口，操作系统只能通过这个接口，才能控制硬件设备的工作

**Linux设备驱动分类**

字符设备
- 只能一个字节一个字节读写的设备，不能随机读取设备内存中的某一数据，读取数据需要按照先后数据
- 字符驱动设备程序通常至少要实现open、close、read和write系统调用
- 字符设备是面向流的设备，常见字符设备：鼠标、键盘、串口、控制台和LED设备等

块设备
- 块设备驱动程序主要通过传输固定大小的数据（一般为512或1k）来访问设备
- 块设备通过buffer cache(内存缓冲区)访问，可以随机存取，即：任何块都可以读写，不必考虑它在设备的什么地方
- 常见块设备：电脑硬盘、SD卡、U盘、光盘等

网络设备
- 任何网络事务都经过一个网络接口形成，即一个能够和其他主机交换数据的设备
- 常见网络设备：网卡设备、蓝牙设备

**应用程序、库、内核、驱动程序的关系**
- 应用程序：调用一系列函数库，通过对文件的操作完成一系列功能
- 函数库：部分函数涉及到硬件操作或内核的支持，由内核完成对应功能，称其为系统调用
- 内核：处理系统调用，根据设备文件类型、主设备号、从设备号，调用设备驱动程序
- 设备驱动：直接与硬件通信

**实验原理**

Linux函数（系统调用）是应用程序和操作系统内核之间的接口，而设备驱动程序时内核和硬件设备之间的接口，设备驱动程序屏蔽硬件细节，且设备被映射成特殊的文件进行处理。

Linux为文件和设备提供了一个一致性的接口，用户操作设备文件与操作普通文件类似。例如open()函数打开设备文件，read(),wirte(),close()等常规文件函数对目标设备进行操作。

在Linux内核中：
- 使用struct cdev结构体来抽象一个字符设备
- 通过一个dev_t类型的设备号（分为主（major）、次设备号（minor））确定字符设备唯一性
- 通过struct file_operations类型的操作方法集来定义字符设备提供个VFS的接口函数

Linux下一切皆是“文件”，字符设备也是这样，file_operations结构体中的成员函数是字符设备程序设计的主题内容，这些函数实际会在用户层程序进行Linux的open()、close()、write()、read()等系统调用时最终被调用

字符设备是3大类设备（字符设备、块设备、网络设备）中较简单的一类设备，其驱动程序中完成的主要工作是初始化、添加和删除cdev结构体，申请和释放设备号，以及填充file_operation结构体中操作函数，并实现file_operations结构体中的read()、write()、ioctl()等重要函数。
