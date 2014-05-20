#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include "Charioctl.h"
#include <linux/proc_fs.h>
MODULE_LICENSE("Dual BSD/GPL");
#define BUFFER_SIZE 1024
#define DEVICENAME "chardev"
#define DP_MAJOR 80
#define DP_MINOR 0
static char device_buffer[BUFFER_SIZE];
struct semaphore sem;/*the chardev open or not*/
dev_t dev_num;
int ret;
int major_num;
struct cdev *Filedev;
static Letter_Method method;
static int char_open(struct inode *, struct file *);
static int char_read(struct file *file, char *, size_t, loff_t *);
static int char_write(struct file *file, char *, size_t, loff_t *);
static int char_close(struct inode *, struct file *);
static int char_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
char *ReverseArray(char *orign, int b)
{
 int a=0;
 char swap;
 for(a;a<--b;a++)
    {
      swap=orign[a];
      orign[a]=orign[b];
      orign[b]=orign[a]; 
     }
return orign;

}
static int char_read(struct file *file, char *buffer, size_t length, loff_t *ffset)
{
    int maxbytes;
    int will_read;
    int already_read;
    int i;
    int a;
    int b; 
    printk(KERN_INFO "switch begin...method: %d\n",method);
    switch(method)
    {
       case Up:
            i=0;
            while(i<length)
                {
                    if(device_buffer[i]!=32)
                   {
                     device_buffer[i]=(char)(device_buffer[i]-('a'-'A'));
                   }
                 printk(KERN_INFO "Debice Buffer: %d\n",device_buffer[i]);  
                 printk(KERN_INFO "Debice num: %d\n",i);  
               i++;}
             break;
        case Down:
            i=0;
            while(i<length)
                {
                    if(device_buffer[i]!=32)
                   {
                     device_buffer[i]=(char)(device_buffer[i]+('a'-'A'));
                   }
                 printk(KERN_INFO "Debice Buffer: %d\n",device_buffer[i]);  
                 printk(KERN_INFO "Debice num: %d\n",i);  
               i++;}
             break;
        case Reverse:
              a=0;
              b=length;
              char swap;
              for(a;a<b--;a++)
                 {
                   swap=device_buffer[a];
                   device_buffer[a]=device_buffer[b];
                   device_buffer[b]=swap; 
                 }
            break;
    }
    maxbytes=BUFFER_SIZE-*ffset;
    if(maxbytes>length)
       {will_read=length;}
       else
       will_read=maxbytes;
    if(will_read==0)
    printk(KERN_INFO "Reached the end of the device\n");
    already_read=will_read-copy_to_user(buffer,device_buffer+*ffset,will_read);
    printk(KERN_INFO "Reached the end of the device %d\n",already_read);
    *ffset+=already_read;
    printk(KERN_INFO "Debice has been read\n");
    return already_read;

}
static int char_write(struct file *file, char *buffer, size_t length, loff_t *ffset)
{
    int maxbytes;
    int will_write;
    int already_write;
    maxbytes=BUFFER_SIZE-*ffset;
    if(maxbytes>length)
       {will_write=length;}
    else
        will_write=maxbytes;

    already_write=will_write-copy_from_user(device_buffer+*ffset,buffer,will_write);
    printk(KERN_INFO " The device has been written %d\n",already_write);
    
     *ffset+=already_write;
     printk(KERN_INFO "Debice has been write\n");
     return already_write;
}


static int char_open(struct inode *inode, struct file *file)
   {
     if(down_interruptible(&sem)!=0)
       {
         printk(KERN_ALERT "Another process open this device \n");
         return -1;
       }
       printk(KERN_ALERT "Device opened successfully\n");
       return 0;
    }

static int char_close(struct inode *inode, struct file *file){
up(&sem);
printk(KERN_INFO " The device has been closed \n");
return ret;
}
static int char_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
  {
   int error=0;
   printk(KERN_INFO " The IOControl is begininig...... \n");
   switch(cmd)
   {
       case IOCTL_SET_MSG :
            printk(KERN_INFO " This is the IOCTL_SET_MSG. \n");
            error=copy_from_user(&method,(int *)arg,sizeof(int));
            break;
       case IOCTL_GET_MSG:
            printk(KERN_INFO " This is the IOCTL_Get_MSG. \n");
            error=copy_to_user((int *)arg,&method,sizeof(int));
            break;
       default:
            printk(KERN_INFO " This is the IOCTL default. \n");
            error=ENOTSUPP;
            break;
   }   
   printk(KERN_INFO " The IOCTL is ended. \n");
   return error;
  }

struct file_operations char_ops={
.read=char_read,
.write=char_write,
.open=char_open,
.release=char_close,
.ioctl=char_ioctl,
};

int file_proc_open(struct inode *inode, struct file *file)
{
   printk(KERN_ALERT "Fie::Entering the file_proc_open \n");
   return 0;
}
int file_proc_release(struct inode *inode, struct file *file)
{
   printk(KERN_ALERT "Fie::Entering the file_proc_release \n");
   return 0;
}
ssize_t file_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ffset){
  int len=0;
  char *tembuf=NULL;
  tembuf=kmalloc(sizeof(char)*1024,GFP_KERNEL);
  if(*ffset>0)
     return 0;
  printk(KERN_ALERT "Fie::Entering the file_proc_read \n");
  sprintf(tembuf,"The File Driver is running!\n");
  len=strlen(tembuf)+1;
  if(len>0)
  copy_to_user(buf,tembuf,len); 
  kfree(tembuf);
  *ffset=len;
  return len;

}
ssize_t file_proc_write(struct file *file, char __user *buf, size_t count, loff_t *ffset){
return 0;
}
static struct file_operations proc_ops={
.open=file_proc_open,
.release=file_proc_release,
.read=file_proc_read,
.write=file_proc_write,
};
static void file_create_proc(void)
{
proc_create("chardev",0,NULL,&proc_ops);
}
static void file_remove_proc(void)
{
remove_proc_entry("chardev",NULL);
}

 int char_init(void)
{
    dev_num=MKDEV(DP_MAJOR,DP_MINOR);
    //ret=alloc_chrdev_region(&dev_num,0,1,DEVICENAME);
    if(register_chrdev_region(&dev_num,1,DEVICENAME)){ /*register_chrdev_region success,return 0*/
    printk(KERN_ALERT "Register char dev error \n");
    return -1;}
         else
    printk(KERN_INFO "Register File dev successfully \n");
    //major_num=MAJOR(dev_num);

    printk(KERN_INFO "The major number of the File Device:%d \n",DP_MAJOR);
    Filedev=cdev_alloc();
    if(Filedev==NULL){
         printk(KERN_INFO "Register Filedev is NULL \n");
         return -1;}
    cdev_init(Filedev,&char_ops);
    Filedev->ops=&char_ops;
    Filedev->owner=THIS_MODULE;
    ret=cdev_add(Filedev,dev_num,1);
    if(ret <0)
      {
        printk(KERN_ALERT "add File dev error");
        file_remove_proc();
        return ret;
         }	
    else
        printk(KERN_INFO "The  File Device are installed successfully \n");
        file_create_proc();
        sema_init(&sem,1);
    return 0;
}

static void __exit module_close(void){
  cdev_del(Filedev);
  printk(KERN_ALERT "Removed the File from Kernel \n");
  unregister_chrdev_region(dev_num,1);
  printk(KERN_INFO "Unregisted teh device number \n");

}
module_init(char_init);
module_exit(module_close);
