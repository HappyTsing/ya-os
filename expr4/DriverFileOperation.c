#include "DriverFileOperation.h"

int DriverOpen(struct inode *pInodeStruct, struct file *pFileStruct)
{
    DeviceOpenNumbers++;
    printk(DEVICE_NAME ":device opened %d times.\n", DeviceOpenNumbers);
    return 0;
}
int DriverRelease(struct inode *pInodeStruct, struct file *pFileStruct)
{
    printk(DEVICE_NAME ":close invoked.\n");
    return 0;
}
ssize_t DriverRead(struct file *pFileStruct, char __user *pBuffer, size_t count, loff_t *pOffset)
{
    int ret = 0;
    ret = copy_to_user(pBuffer, msg, size_of_msg);
    if (ret == 0)
    {
        printk(DEVICE_NAME ":send %d characters to user.\n", size_of_msg);
        return (size_of_msg = 0);
    }
    else
    {
        printk(DEVICE_NAME ":failed to read from device.\n");
        return -EFAULT;
    }
}
ssize_t DriverWrite(struct file *pFileStruct, const char __user *pBuffer, size_t count, loff_t *pOffset)
{
    int ret = copy_from_user(msg, pBuffer, count);
    // sprintf(msg,"%s(%d letters)",pBuffer,count);
    size_of_msg = count;
    if (ret == 0)
    {
        printk(DEVICE_NAME ":received %d characters from the user.\n", size_of_msg);
    }
    else
    {
        printk(DEVICE_NAME "kernel recevdata failed!");
    }
    return 0;
}