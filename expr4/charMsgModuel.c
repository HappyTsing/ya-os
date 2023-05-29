#include "charDevDirver.h"
#include "DriverFileOperation.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HappyTsing");
MODULE_DESCRIPTION("A Simple Liunx Char Driver Example");

struct file_operations_fops = {
    .owner = THIS_MODULE,
    .open = DriverOpen,
    .release = DriverRelease,
    .read = DriverRead,
    .write = DriverWrite,
};

static int __init charMsgDriverInit(void)
{
   printk(KERN_ALERT "insmod os4 charMsgModule success./n");

}
static void __exit charMsgDriverExit(void)
{
   printk(KERN_ALERT "rmmod os4 charMsgModule success./n");

}

module_init(charMsgDriverInit);
module_exit(charMsgDriverExit);