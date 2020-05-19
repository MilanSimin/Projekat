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

#include <linux/dma-mapping.h>  //dma access
#include <linux/mm.h>  //dma access
#include <linux/interrupt.h>  //interrupt handlers

#define BUFF_SIZE 20
#define DRIVER_NAME "IFS_DRIVER"
#define MAX_PKT_LEN 256*256*4
#define DEVICE_NAME "IFS"

MODULE_AUTHOR ("FTN");
MODULE_DESCRIPTION("Test Driver for VGA controller IP.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:vga_dma controller");

//****************************** GLOBAL VARIABLES *******************************************

struct IFS_info {
	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;
	
};

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;
static struct IFS_info *ip = NULL;

dma_addr_t tx_phy_buffer;
u32 *tx_vir_buffer;

//****************************** FUNCTION PROTOTYPES ****************************************
static int IFS_probe (struct platform_device *pdev);
static int IFS_remove (struct platform_device *pdev);
static int IFS_open (struct inode*, struct file*);
static int IFS_close (struct inode*, struct file*);
static ssize_t IFS_read (struct file*, char*, size_t, loff_t*);
static ssize_t IFS_write (struct file*, const char*, size_t, loff_t*);
int IFS_mmap (struct file *f, struct vm_area_struct *vma_s);

static int __init IFS_init(void);
static void __exit IFS_exit(void);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,	
	.read = IFS_read,
	.write = IFS_write,
	.open = IFS_open,
	.release = IFS_close,
	.mmap = IFS_mmap,

};

static struct of_device_id IFS_of_match[] = {
	
	{ .compatible = "bram_image", },
	{ .compatible = "bram_kernel", },
	{ .compatible = "bram_after_conv" },
	{ /* end of list */}

};

MODULE_DEVICE_TABLE(of, IFS_of_match);

static struct platform_driver IFS_driver = {
	
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = IFS_of_match,
		},
		
	.probe = IFS_probe,
	.remove = IFS_remove,
};


static int IFS_probe (struct platform_device *pdev) {

	struct resource *r_mem;
	int rc = 0;
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!r_mem){
		printk(KERN_ALERT "Failed to get resource\n");
		return -ENODEV;
	} 
	ip = (struct IFS_info *) kmalloc(sizeof(struct IFS_info), GFP_KERNEL);
	if(!ip){
		printk(KERN_ALERT "Could not allocate memory\n");
		return -ENOMEM;
	}
	ip->mem_start = r_mem->start;
	ip->mem_end = r_mem->end;
	//printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


	if(!request_mem_region(ip->mem_start, ip->mem_end - ip-> mem_start + 1, "IFS")){
		printk(KERN_ALERT "Could not lock memory region at %p\n",(void *)ip->mem_start);
		rc = -EBUSY;
		goto error1;

	}

	ip->base_addr = ioremap(ip->mem_start, ip->mem_end - ip->mem_start +1);

	if(!ip->base_addr){
		printk(KERN_ALERT "Could not allocate memory\n");
		rc = -EIO;
		goto error2;
	}

	printk(KERN_WARNING "led platform driver registered\n");
 	return 0;//ALL OK

	error2:
		release_mem_region(ip->mem_start, ip->mem_end - ip->mem_start + 1);	
	error1:
		return rc;

}

static int IFS_remove(struct platform_device *pdev)
{
	printk(KERN_WARNING "IFS platform driver removed\n");
	iowrite32(0, ip->base_addr);
	iounmap(ip->base_addr);
	release_mem_region(ip->mem_start, ip->mem_end - ip->mem_start + 1);
	kfree(ip);
	return 0;
}



int IFS_open (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully opened file\n");
	return 0;

}
	
int IFS_close (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully closed file\n");
	return 0;

}

ssize_t IFS_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){

	//int​ minor = MINOR(pfile->f_inode->i_rdev);

	printk(KERN_INFO "Provera read file \n");
	return 0;
}

ssize_t IFS_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){

	char buff[BUFF_SIZE];
	int​ minor = MINOR(pfile->f_inode->i_rdev);
	int ret = 0;
	unsigned int xpos=0,ypos=0;
	unsigned long long rgb=0;
	unsigned char rgb_buff[10];
	ret = copy_from_user(buff, buf, length);  
	if(ret){
		printk("copy from user failed \n");
		return -EFAULT;
	}  
	buff[length] = '\0';

	sscanf(buff,"%d,%d,%s", &xpos, &ypos, rgb_buff);  
	ret = kstrtoull(rgb_buff, 0, &rgb);

	switch(minor){

		case 0:
			if(ret != -EINVAL) //checking for parsing error
			{
				if (xpos > 255)
				{
					printk(KERN_WARNING "IFS_write: X_axis position exceeded, maximum is 255 and minimum 0 \n");
				}
				else if (ypos > 255)
				{
					printk(KERN_WARNING "IFS_write: Y_axis position exceeded, maximum is 255 and minimum 0 \n");
				}
				else
				{
					iowrite32((256*ypos + xpos)*4, ip->base_addr);
					iowrite32(rgb,ip->base_addr);
				}
			}
			else
			{
				printk(KERN_WARNING "IFS_write: Wrong write format\n");
				// return -EINVAL; //parsing error
			}        
			return length;
			
			break;
	
		case 1:
			if(ret != -EINVAL) //checking for parsing error
			{
				if (xpos > 3)
				{
					printk(KERN_WARNING "IFS_write: X_axis position exceeded, maximum is 3 and minimum 0 \n");
				}
				else if (ypos > 3)
				{
					printk(KERN_WARNING "IFS_write: Y_axis position exceeded, maximum is 3 and minimum 0 \n");
				}
				else
				{
					iowrite32((3*ypos + xpos)*4, ip->base_addr);
					iowrite32(rgb,ip->base_addr);
				}
			}
			else
			{
				printk(KERN_WARNING "IFS_write: Wrong write format\n");
				// return -EINVAL; //parsing error
			}        
			return length;
	
			break;

		case 2: 
		
			break;
	
		case 3: 

			break;

		default:
			printk(KERN_INFO"somethnig went wrong\n");

	}

	printk(KERN_INFO "Provera write file \n");	
	
	

}

int IFS_mmap(struct file *f, struct vm_area_struct *vma_s){

	int ret = 0;
	unsigned long vsize = vma_s->vm_end - vma_s->vm_start; // velicina addr prostora koji zahteva aplikacija
	unsigned long psize = ip->mem_end - ip->mem_start; // velicina addr prostora koji zauzima jezgro
	vma_s->vm_page_prot = pgprot_noncached(vma_s-> vm_page_prot);
	printk(KERN_INFO "Buffer is being memory mapped\n");

	if (vsize > psize)
	{
		printk(KERN_ERR "Trying to mmap more space than it's allocated, mmap failed\n");
		return -EIO;		
	}
	ret = vm_iomap_memory(vma_s, ip->mem_start, vsize);
	if(ret) 
	{	
		printk(KERN_ERR "memory maped failed\n");
		return ret;	
		
	}
	printk(KERN_INFO "MMAP is a success\n");
	return 0;
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
		printk(KERN_ERR "Failed to create class\n");
		goto fail_0;
	}
	printk(KERN_INFO "Class created\n");


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
	printk(KERN_INFO "created IFS\n");

	my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, num_of_minors);
	if(ret)
	{
		printk(KERN_ERR "Failde to add cdev \n");
		goto fail_2;	
	}
	printk(KERN_INFO "cdev_added\n");
	printk(KERN_INFO "Hello from IFS_driver\n");

	return platform_driver_register(&IFS_driver);
	
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
	
	platform_driver_unregister(&IFS_driver);
	cdev_del(my_cdev);
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),3));
	class_destroy(my_class);
	unregister_chrdev_region(my_dev_id,4);
	printk(KERN_ALERT "Goodbye from IFS_driver\n");	

}

module_init(IFS_init);
module_exit(IFS_exit);
