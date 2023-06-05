#include "charDevDriver.h"
#include "DriverFileOperation.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HappyTsing");
MODULE_DESCRIPTION("A Simple Liunx Char Driver Example");
static struct file_operations charDevDriver_fops = {
    .owner = THIS_MODULE,
    .open = DriverOpen,
    .release = DriverRelease,
    .read = DriverRead,
    .write = DriverWrite,
};

static int __init charMsgDriverInit(void)
{
   int ret;
   ret = register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &charDevDriver_fops); // 注册字符设备驱动
   if (ret < 0)
   {
      printk("charDevDriver init failed\n");
   }
   printk(KERN_ALERT "charDevDriver register success./n");
   return 0;
}
static void __exit charMsgDriverExit(void)
{
   unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME); // 在驱动的卸载函数中释放主设备号
   printk(KERN_ALERT "charDevDriver unregister success./n");
}

module_init(charMsgDriverInit);
module_exit(charMsgDriverExit);
