#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>

#include <linux/of.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/interrupt.h>


#define MAX_PKT_LEN 640*480*4
#define DRIVER_NAME "ifs"
MODULE_LICENSE("Dual BSD/GPL");

struct ifs_info {

	unsigned long mem_start;
	unsigned long mem_end;
	void __iomem *base_addr;

};

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;
static struct ifs_info *ip = NULL;

u32 *tx_vir_buffer;
dma_addr_t tx_phy_buffer;

//****************************FUNCTION PROTOTYPES************************//
static int ifs_probe(struct platform_device *pdev);
static int ifs_remove(struct platform_device *pdev);
int ifs_open(struct inode *pinode, struct file *pfile);
int ifs_close(struct inode *pinode, struct file *pfile);
ssize_t ifs_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t ifs_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
static ssize_t ifs_mmap(struct file *pfile, struct vm_area_struct *vma_s);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = ifs_open,
	.read = ifs_read,
	.write = ifs_write,
	.release = ifs_close,
	.mmap = ifs_mmap
};

static struct of_device_id ifs_of_match[] = {
	{ .compatible = "bram_image" },
	{ .compatible = "bram_kernel" },
	{ .compatible = "bram_after_conv" },
	{ /*end of list*/ },
};

static struct platform_driver ifs_driver = {

	.driver ={
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = ifs_of_match,
	},
	.probe = ifs_probe,
	.remove = ifs_remove,

};

MODULE_DEVICE_TABLE (of, ifs_of_match);

static int ifs_probe (struct platform_device *pdev){
	struct resource *r_mem;
	int rc = 0;
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!r_mem ){
		printk(KERN_ALERT "Failed to get resource\n");
		return -ENODEV;
	}

	ip= (struct ifs_info *) kmalloc (sizeof(struct ifs_info),GFP_KERNEL);
	if (!ip) {
		printk(KERN_ALERT "could not allocate ifs device\n");
		return -ENOMEM;
	}

	ip -> mem_start = r_mem -> start;
	ip -> mem_end = r_mem -> end;
	//printk(KERN_INFO "Start address: %x \t end address: %x ", r_mem -> start, r_mem -> end)

	if (!request_mem_region(ip->mem_start,ip->mem_end - ip->mem_start +1 ,DRIVER_NAME )){
		printk (KERN_ALERT "could not lock memory region at %p \n ", (void *)ip->mem_start);
		rc = -EBUSY;
		goto error1;
	}

	ip -> base_addr =ioremap (ip-> mem_start, ip->mem_end - ip->mem_start +1);
	if(!ip->base_addr){
		printk(KERN_ALERT "could not allocate memory \n");
		rc = -EIO;
		goto error2;
	}

	printk(KERN_WARNING "ifs platform driver registered \n");
	return 0;

	error2:
		release_mem_region (ip->mem_start, ip->mem_end - ip->mem_start +1);

	error1:
		return rc;

}

static int ifs_remove(struct platform_device *pdev){

	printk(KERN_WARNING "ifs platform driver removed \n");
	iowrite32(0, ip->base_addr);
	iounmap(ip->base_addr);
	release_mem_region(ip->mem_start,ip->mem_end - ip->mem_start+1);
	kfree(ip);
	return 0;

}

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

static ssize_t ifs_mmap(struct file *pfile,struct vm_area_struct *vma_s){

	int ret =0;
	long length =vma_s->vm_end -vma_s->vm_start;

	if (length > MAX_PKT_LEN)
	{
		return -EIO;
		printk(KERN_ERR "Trying to mmap more space than allowed\n");
	}
	ret =dma_mmap_coherent(NULL, vma_s, tx_vir_buffer, tx_phy_buffer, length);
	if(ret<0){
		printk(KERN_ERR "memory map failed\n");
		return ret;

	}
	return 0;
}

static int __init ifs_init(void)
{
   int i=0;
   int ret = 0;
   ret = alloc_chrdev_region(&my_dev_id, 0, 1, DRIVER_NAME);
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
   printk(KERN_INFO "ifs: Module init done\n");

	tx_vir_buffer = dma_alloc_coherent (NULL,MAX_PKT_LEN, &tx_phy_buffer, GFP_DMA | GFP_KERNEL);
	if(!tx_vir_buffer){
		printk(KERN_ALERT "ifs_init: could not allocate dma_alloc_coherent for image");
		goto fail_3;

	}
	else
		printk("ifs_init: Successfully alocated memory for dma buffer");

	for( i = 0; i<MAX_PKT_LEN/4;i++)
		tx_vir_buffer[i]=0x00000000;
	printk(KERN_INFO"ifs_init: Memory reset");


   return 0;
	
	fail_3:
		cdev_del(my_cdev);
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
	platform_driver_unregister(&ifs_driver);
	cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   dma_free_coherent(NULL, MAX_PKT_LEN, tx_vir_buffer, tx_phy_buffer);
   printk(KERN_INFO "Goodbye, cruel world\n");
}


module_init(ifs_init);
module_exit(ifs_exit);

