#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

int ifs_open(struct inode *pinode, struct file *pfile);
int ifs_close(struct inode *pinode, struct file *pfile);
ssize_t ifs_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t ifs_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = ifs_open,
	.read = ifs_read,
	.write = ifs_write,
	.release = ifs_close,
};


int ifs_open(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully opened file\n");
		return 0;
}

int ifs_close(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully closed file\n");
		return 0;
}

ssize_t ifs_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
		printk(KERN_INFO "Succesfully read from file\n");
		return 0;
}

ssize_t ifs_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
		printk(KERN_INFO "Succesfully wrote into file\n");
		return length;
}

static int __init ifs_init(void)
{
   int ret = 0;
   ret = alloc_chrdev_region(&my_dev_id, 0, 1, "ifs");
   if (ret){
      printk(KERN_ERR "failed to register char device\n");
      return ret;
   }
   printk(KERN_INFO "char device region allocated\n");

   my_class = class_create(THIS_MODULE, "ifs_class");
   if (my_class == NULL){
      printk(KERN_ERR "failed to create class\n");
      goto fail_0;
   }
   printk(KERN_INFO "class created\n");
   
   my_device = device_create(my_class, NULL, my_dev_id, NULL, "ifs");
   if (my_device == NULL){
      printk(KERN_ERR "failed to create device\n");
      goto fail_1;
   }
   printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
      printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
   printk(KERN_INFO "cdev added\n");
   printk(KERN_INFO "ifs world\n");

   return 0;

	fail_2:
		device_destroy(my_class, my_dev_id);
   fail_1:
      class_destroy(my_class);
   fail_0:
      unregister_chrdev_region(my_dev_id, 1);
   return -1;
}

static void __exit ifs_exit(void)
{
	cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   printk(KERN_INFO "Goodbye, cruel world\n");
}


module_init(ifs_init);
module_exit(ifs_exit);

