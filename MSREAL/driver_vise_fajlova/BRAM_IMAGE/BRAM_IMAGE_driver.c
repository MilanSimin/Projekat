
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
#include <linux/of.h>

#include <linux/mm.h> //za memorijsko mapiranje
#include <linux/io.h> //iowrite ioread
#include <linux/slab.h>//kmalloc kfree
#include <linux/platform_device.h>//platform driver
#include <linux/of.h>//of match table
#include <linux/ioport.h>//ioremap


#define BUFF_SIZE 30
#define MAX_PKT_LEN 256*256*4
#define DRIVER_NAME "BRAM_IMAGE_DRIVER"
#define DEVICE_NAME "BRAM_IMAGE"

MODULE_AUTHOR ("FTN");
MODULE_DESCRIPTION("Test Driver for Image Filtering system.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:BRAM");

//********************************************GLOBAL VARIABLES *****************************************//
struct BRAM_info {
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;

};

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;
static struct BRAM_info *bp = NULL;

int position = 0;
int number = 0;
int counter = 0;

//****************************** FUNCTION PROTOTYPES ****************************************//
static int BRAM_probe (struct platform_device *pdev);
static int BRAM_remove (struct platform_device *pdev);
static int BRAM_open (struct inode *pinode, struct file *pfile);
static int BRAM_close (struct inode *pinode, struct file *pfile);
static ssize_t BRAM_read (struct file *pfile, char __user *buf, size_t length, loff_t *offset);
static ssize_t BRAM_write (struct file *pfile, const char __user *buf, size_t length, loff_t *offset);
int BRAM_mmap (struct file *f, struct vm_area_struct *vma_s);

static int __init BRAM_init(void);
static void __exit BRAM_exit(void);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.read = BRAM_read,
	.write = BRAM_write,
	.open = BRAM_open,
	.release = BRAM_close,
	.mmap = BRAM_mmap,

};

static struct of_device_id BRAM_of_match[] = {


	{ .compatible = "bram_image", },
	{ /* end of list */}

};

MODULE_DEVICE_TABLE(of, BRAM_of_match);

static struct platform_driver BRAM_IMAGE_driver = {

	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = BRAM_of_match,
		},

	.probe = BRAM_probe,
	.remove = BRAM_remove,
};


static int BRAM_probe (struct platform_device *pdev) 
{

	struct resource *r_mem;
	int rc = 0;
	r_mem= platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!r_mem){
		printk(KERN_ALERT "Failed to get resource\n");
		return -ENODEV;
	}

	bp = (struct BRAM_info *) kmalloc(sizeof(struct BRAM_info), GFP_KERNEL);
	if(!bp){
		printk(KERN_ALERT "Could not allocate memory\n");
		return -ENOMEM;
	}

	bp->mem_start = r_mem->start;
	bp->mem_end = r_mem->end;
	printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


	if(!request_mem_region(bp->mem_start, bp->mem_end - bp-> mem_start + 1, DRIVER_NAME)){
		printk(KERN_ALERT "Could not lock memory region at %p\n",(void *)bp->mem_start);
		rc = -EBUSY;
		goto error3;
	}

	bp->base_addr = ioremap(bp->mem_start, bp->mem_end - bp->mem_start +1);

	if(!bp->base_addr){
		printk(KERN_ALERT "Could not allocate memory\n");
		rc = -EIO;
		goto error4;
	}

	counter ++;
	printk(KERN_WARNING "BRAM_IMAGE registered\n");
 	return 0;//ALL OK

	error4:
		release_mem_region(bp->mem_start, bp->mem_end - bp->mem_start + 1);	
	error3:
		return rc;


	return 0;
}

static int BRAM_remove(struct platform_device *pdev)
{

	

	printk(KERN_WARNING "BRAM_IMAGE_remove: platform driver removing\n");
	iowrite32(0,bp->base_addr);
	iounmap(bp->base_addr);
	release_mem_region(bp->mem_start, bp->mem_end - bp->mem_start + 1);
	kfree(bp);
	printk(KERN_INFO"BRAM_IMAGE_remove: BRAM_IMAGE removed\n");


	return 0;
}



int BRAM_open (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully opened file\n");
	return 0;

}

int BRAM_close (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully closed file\n");
	return 0;

}

ssize_t BRAM_read (struct file *pfile, char __user *buf, size_t length, loff_t *offset){

	int ret, i=0, pos=0;
	char buff[BUFF_SIZE];
	int len,temp;
	int value;
	//int minor = MINOR(pfile->f_inode->i_rdev);
	
	for(i=0; i<number; i++)
	{
		pos = position + i*4;
		value  = ioread32(bp->base_addr + pos);
		printk (KERN_INFO "value is: %d\n",value);
		printk (KERN_INFO "position is: %d\n",pos);
		temp = scnprintf(buff, BUFF_SIZE, "%d\n", value);
		ret = copy_to_user(buf, buff, len);
		if(ret){
			return -EFAULT;
		}
	}
		

	return len;
}

ssize_t BRAM_write (struct file *pfile, const char __user *buf, size_t length, loff_t *offset){

	char buff[BUFF_SIZE];
	//int minor = MINOR(pfile->f_inode->i_rdev);
	int ret = 0, i = 0, pos=0;
	unsigned int xpos=0, ypos=0;
	unsigned int rgb=0;
	ret = copy_from_user(buff, buf, length);

	if(ret){
		printk("copy from user failed \n");
		return -EFAULT;
	}
	buff[length] = '\0';

	if(buff[0] == '(')
	{

		sscanf(buff,"(%d,%d);%d", &xpos, &ypos, &rgb); 
		//printk(KERN_INFO "Prvi slucaj, bez broja pre (\n");

	} else {

		sscanf(buff, "%d(%d,%d);%d", &number, &xpos, &ypos, &rgb);
		//printk(KERN_INFO "Drugi slucaj, sa brojem pre (\n");

	}

	
	if(ret != -EINVAL) //checking for parsing error
		{
		if (xpos > 255)
		{
			printk(KERN_WARNING "BRAM_write: X_axis position exceeded, maximum is 255 and minimum 0 \n");
		}
		else if (ypos > 255)
		{
			printk(KERN_WARNING "BRAM_write: Y_axis position exceeded, maximum is 255 and minimum 0 \n");
		}
		else
		{
			//printk(KERN_INFO "number is: %d",number );
			position = (255*ypos+xpos)*4;
			for(i=0; i<=number; i++)
			{
				pos = position +i*4;
				printk(KERN_INFO "position is: %d\n",pos);
				printk(KERN_INFO "value is: %d\n",rgb);
				iowrite32(rgb,bp->base_addr+pos);
			}
		}
	}
	else
	{
		printk(KERN_WARNING "BRAM_write: Wrong write format\n");
		// return -EINVAL; //parsing error
	}

			

	return length;
}

int BRAM_mmap(struct file *f, struct vm_area_struct *vma_s){

	int ret = 0;
	unsigned long vsize = vma_s->vm_end - vma_s->vm_start; // velicina addr prostora koji zahteva aplikacija
	unsigned long psize = bp->mem_end - bp->mem_start; // velicina addr prostora koji zauzima jezgro
	vma_s->vm_page_prot = pgprot_noncached(vma_s-> vm_page_prot);
	printk(KERN_INFO "Buffer is being memory mapped\n");

	if (vsize > psize)
	{
		printk(KERN_ERR "Trying to mmap more space than it's allocated, mmap failed\n");
		return -EIO;
	}
	ret = vm_iomap_memory(vma_s, bp->mem_start, vsize);
	if(ret)
	{
		printk(KERN_ERR "memory maped failed\n");
		return ret;

	}
	printk(KERN_INFO "MMAP is a success\n");
	return 0;
}


static int __init BRAM_init(void)
{
	int ret = 0;
	ret = alloc_chrdev_region(&my_dev_id, 0, 1, DRIVER_NAME);
	if(ret != 0){

		printk(KERN_ERR "Failed to register char device\n");
		return ret;
	}
	printk(KERN_INFO"Char device region allocated\n");

	my_class = class_create(THIS_MODULE,"BRAM_class");
	if (my_class == NULL){
		printk(KERN_ERR "Failed to create class\n");
		goto fail_0;
	}
	printk(KERN_INFO "BRAM Class created\n");

	my_device = device_create(my_class, NULL, my_dev_id, NULL, "bram_image");

	if(my_device == NULL){
		printk(KERN_ERR "Failde to create device BRAM_image\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM: bram_image\n");

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
	printk(KERN_INFO "Hello from BRAM_IMAGE_driver\n");

	return platform_driver_register(&BRAM_IMAGE_driver);

	fail_2:
		device_destroy(my_class, my_dev_id);
	fail_1:
		class_destroy(my_class);
	fail_0:
		unregister_chrdev_region(my_dev_id, 1);
	return -1;

}

static void __exit BRAM_exit(void)
{
	printk(KERN_ALERT "IFS_exit: rmmod called\n");
	platform_driver_unregister(&BRAM_IMAGE_driver);
	//printk(KERN_INFO"IFS_exit: platform_driver_unregister done\n");
	cdev_del(my_cdev);
	//printk(KERN_ALERT "IFS_exit: cdev_del done\n");
	device_destroy(my_class, my_dev_id);
	class_destroy(my_class);
	//printk(KERN_INFO"IFS_exit: class destroy \n");
	unregister_chrdev_region(my_dev_id,1);
	printk(KERN_ALERT "Goodbye from IFS_driver\n");		

}

module_init(BRAM_init);
module_exit(BRAM_exit);
