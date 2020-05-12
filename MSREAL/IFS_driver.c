#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>	
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/string.h>

#define BUFF_SIZE 20

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

int storage[10];
int pos = 0;
int endRead = 0;

int IFS_open (struct inode*, struct file*);
int IFS_close (struct inode*, struct file*);
ssize_t IFS_read (struct file*, char*, size_t, loff_t*);
ssize_t IFS_write (struct file*, const char*, size_t, loff_t*);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.read = IFS_read,
	.write = IFS_write,
	.open = IFS_open,
	.release = IFS_close,

};

int IFS_open (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully opened file\n");
	return 0;

}
	
int IFS_close (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully closed file\n");
	return 0;

}

ssize_t IFS_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){


	int ret;
	char buff[BUFF_SIZE];
	long int len;
	int minor = MINOR(pfile->f_inode->i_rdev);
	if(endRead){
		endRead = 0;
		printk(KERN_INFO "Succesfully read from file\n");
		return 0;
	}	
	len = scnprintf(buff, BUFF_SIZE, " %d", storage[minor]);
	ret = copy_to_user(buffer, buff ,len);
	
	if(ret)
		return -EFAULT;
	
	return len;

}

ssize_t IFS_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){

	char buff[BUFF_SIZE];
	int value;
	int ret;
	int minor = MINOR(pfile->f_inode->i_rdev);

	ret = copy_from_user(buff, buffer, length);
	if(ret)
		return -EFAULT;
	
	buff[length-1]='\0';
	
	ret = sscanf (buff, "%d", &value);
	storage[minor] = value;
	printk(KERN_INFO "Succesfully wrote value %d at position %d\n", value, minor);
	return length;
	
	/*if (ret==2)
	{
		if(position >=0 && position <= 9)
		{
			storage	[position] = value;
			printk(KERN_INFO "Succesfully wrote value %d in position %d \n", value, position);

		}
		else
		{
			printk(KERN_WARNING "Position should be between 0 and 9\n");
		}			
			


	}
	else
	{	
		printk(KERN_WARNING "Wrong command formant \n expected: n,m\n\tn-position\n\tm-value\n\n");

	}

	return length;*/

}


static int __init IFS_init(void)
{
	int num_of_minors = 4;
	int ret = 0;
	ret = alloc_chrdev_region(&my_dev_id, 0, num_of_minors, "IFS_region");
	if(ret != 0){

		printk(KERN_ERR "Failed to register char device\n");
		return ret;
	}
	printk(KERN_INFO"Char device region allocated\n");

	my_class = class_create(THIS_MODULE,"IFS_class");
	if (my_class == NULL){
		printk(KERN_ERR"Failed to create class\n");
		goto fail_0;
	}
	printk(KERN_INFO " Class created\n ");
	
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),0), NULL, "BRAM_IMAGE");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_IMAGE\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_IMAGE\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),1), NULL, "BRAM_KERNEL");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_KERNEL\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_KERNEL\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),2), NULL, "BRAM_AFTER_CONV");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_AFTER_CONV\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_AFTER_CONV\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),3), NULL, "IFS");
	
	if(my_device == NULL){
		printk(KERN_ERR "Failde to create device IFS\n");
		goto fail_1;
	}
	printk(KERN_INFO "Device created IFS\n");

	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if(ret)
	{
		printk(KERN_ERR "Failde to add cdev \n");
		goto fail_2;	
	}
	printk(KERN_INFO "cdev_added\n");
	printk(KERN_INFO "Hello from IFS_driver\n");

	return 0;

	fail_2:
		device_destroy(my_class, my_dev_id);
	fail_1:
		class_destroy(my_class);
	fail_0:
		unregister_chrdev_region(my_dev_id, 1);


	return -1;

}

static void __exit IFS_exit(void)
{
	cdev_del(my_cdev);
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),3));
	class_destroy(my_class);
	unregister_chrdev_region(my_dev_id,1);
	printk(KERN_ALERT "Goodbye from IFS_driver\n");	

}

module_init(IFS_init);
module_exit(IFS_exit);
