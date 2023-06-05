#ifndef charDevDriver_H
#define charDevDriver_H
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/ide.h>

#define DEVICE_NAME "charMsgDriver" // 驱动名
#define CLASS_NAME "charMsg"
#define MAJOR_NUMBER 200 // 主设备号
#define MSG_COUNT 1024

static char msg[MSG_COUNT] = {0}; // 用于保存从用户空间传输的数据，用于write函数
static int DeviceOpenNumbers = 0; // 字符驱动设备文件打开次数，本质位于 /dev 文件夹下，通过 sudo mknod /dev/os4_charMsgDriver c 200 0 创建
static short size_of_msg;         // 记录用户空间传输的数据大小

// 没有用到
static struct class *pcharMsgDriverClass = NULL;
static struct device *pcharMsgDriverDevice = NULL;
static int majorNumber;

#endif