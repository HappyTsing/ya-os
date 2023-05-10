#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");

static char*whom = "happytsing";
static int num = 1;

// 约定链表头的 index 为 0，head 节点不会被打印
struct listnode {	
   int value_;
   struct listnode* next_;
};

// index为插入后新节点的index
int list_insert(struct listnode* head, int index, int value){
   if(head == NULL){
      printk(KERN_ERR "list head is null.\n");
      return -1;
   }
   if(index == 0){
      printk(KERN_ERR "error index.\n");
      return -1;
   }
   struct listnode* curr;
   curr = head;
   int i;
   // 找到插入的位置
   for (i=1;i<index;i++){
      curr=curr->next_;
      if(curr==NULL){
         printk(KERN_ERR "insert index out of range.\n");
         return -1;
      }
   }
   // 此时将新区块弄到 curr->next_ 即可
   struct listnode* new_node;
   new_node=kmalloc(sizeof(*new_node),GFP_KERNEL);
   new_node->value_ = value;
   new_node->next_ = curr->next_;
   curr->next_ = new_node;
   printk(KERN_INFO "Insert Index %d || Value %d.\n",index,value);
   return 1;
}

int list_delete(struct listnode * head, int index){
   if(head == NULL){
      printk(KERN_ERR "list head is null.\n");
      return -1;
   }
   if(index == 0){
      printk(KERN_ERR "error index.\n");
      return -1;
   }
   struct listnode* curr;
   curr = head;
   int i;
   // 找到删除的位置的前一个
   for (i=1;i<index;i++){
      curr=curr->next_;
      if(curr==NULL){
         printk(KERN_ERR "delete index out of range.\n");
         return -1;
      }
   }
   struct listnode* delete_node;
   delete_node = curr->next_;
   if (delete_node ==NULL){
         printk(KERN_ERR "delete index out of range.\n");
         return -1;
   }
   curr->next_ = curr->next_->next_;
   printk(KERN_INFO "Delete Index %d || Value %d.\n",index,delete_node->value_);
   kfree(delete_node);
   return 1;
}

int list_search(struct listnode * head, int value){
   if(head == NULL){
      printk(KERN_ERR "list head is null.\n");
      return -1;
   }
   struct listnode* curr;
   curr = head;
   int index;
   index=0;
   while(curr->next_ != NULL){
      curr = curr->next_;
      index++;
      if(curr->value_ == value){
         printk(KERN_INFO "Search Value %d || Index %d",value,index);
         return 1;
      }
   }
   printk(KERN_INFO "Search Value %d Not Found",value);
   return -1;
}

void list_print(struct listnode* head){
   if(head == NULL){
      printk(KERN_ERR "list head is null.\n");
      return;
   }
   struct listnode* curr;
   curr = head->next_; // 不打印头
   int index;
   index = 1;
   printk(KERN_INFO "======== Print List Start ========\n");
   while(curr!=NULL){
      printk(KERN_INFO "Index %d || Value %d\n",index,curr->value_);
      index++;
      curr = curr->next_;
   }
   printk(KERN_INFO "======== Print List End ========\n");
   return;
}

//传递命令参数 S_IRUGO 指明参数可以被所有人读取 
module_param(whom,charp,S_IRUGO);
module_param(num,int,S_IRUGO);

//程序中必须有下列两个函数
static int hello_init(void)
{
   int i;
   for (i=0;i<num;i++){
	//使用的是printk 不是printf（其是C库函数）
   	printk(KERN_ALERT "hello,%s\n",whom);
   }

   struct listnode *head;
   head=kmalloc(sizeof(*head),GFP_KERNEL);
   if(head != NULL){
      head->value_ = 0;
      head->next_ = NULL;
      printk(KERN_ALERT "the head val: %d\n",head->value_);
   }
   list_insert(head,1,1);
   list_insert(head,2,3);
   list_insert(head,2,2);
   list_insert(head,5,3); // out of range
   list_print(head);

   list_search(head,2);
   list_search(head,4); // not found

   list_delete(head,2);
   list_delete(head,4); // out of range
   list_print(head);
   return 0;
}

static void hello_exit(void)
{
   printk(KERN_ALERT "goodbye,kernel/n");
}

//加载or卸载模块
module_init(hello_init);
module_exit(hello_exit);
// 可选 
MODULE_AUTHOR("HappyTsing");
MODULE_DESCRIPTION("OS experiment 1/n");
MODULE_VERSION("v1.0");
MODULE_ALIAS("expr1");

