# Usage

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