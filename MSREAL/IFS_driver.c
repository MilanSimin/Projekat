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
#define DRIVER_NAME "IFS_DRIVER"
#define DEVICE_NAME "IFS"

MODULE_AUTHOR ("FTN");
MODULE_DESCRIPTION("Test Driver for Image Filtering system.");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("custom:IFS");

//********************************************GLOBAL VARIABLES *****************************************//
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
static struct IFS_info *bp1 = NULL;
static struct IFS_info *bp2 = NULL;
static struct IFS_info *bp3 = NULL;

int position = 0;
int number = 0;
int counter = 0;

//****************************** FUNCTION PROTOTYPES ****************************************//
static int IFS_probe (struct platform_device *pdev);
static int IFS_remove (struct platform_device *pdev);
static int IFS_open (struct inode *pinode, struct file *pfile);
static int IFS_close (struct inode *pinode, struct file *pfile);
static ssize_t IFS_read (struct file *pfile, char __user *buf, size_t length, loff_t *offset);
static ssize_t IFS_write (struct file *pfile, const char __user *buf, size_t length, loff_t *offset);
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


	{ .compatible = "image_conv", },
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


static int IFS_probe (struct platform_device *pdev) 
{

	printk(KERN_INFO "Counter is: %d\n", counter);
	struct resource *r_mem;
	int rc = 0;
	r_mem= platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if(!r_mem){
		printk(KERN_ALERT "Failed to get resource\n");
		return -ENODEV;
	}	
	
	switch(counter){

		case 0:

			ip = (struct IFS_info *) kmalloc(sizeof(struct IFS_info), GFP_KERNEL);
			if(!ip){
				printk(KERN_ALERT "Could not allocate memory\n");
				return -ENOMEM;
			}

			ip->mem_start = r_mem->start;
			ip->mem_end = r_mem->end;
			printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


			if(!request_mem_region(ip->mem_start, ip->mem_end - ip-> mem_start + 1, DRIVER_NAME)){
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

			counter++;
			printk(KERN_INFO "IFS driver registered\n");
		 	return 0;//ALL OK

			error2:
				release_mem_region(ip->mem_start, ip->mem_end - ip->mem_start + 1);	
			error1:
				return rc;


		case 1:

			
			bp1 = (struct IFS_info *) kmalloc(sizeof(struct IFS_info), GFP_KERNEL);
			if(!ip){
				printk(KERN_ALERT "Could not allocate memory\n");
				return -ENOMEM;
			}

			bp1->mem_start = r_mem->start;
			bp1->mem_end = r_mem->end;
			printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


			if(!request_mem_region(bp1->mem_start, bp1->mem_end - bp1-> mem_start + 1, DRIVER_NAME)){
				printk(KERN_ALERT "Could not lock memory region at %p\n",(void *)bp1->mem_start);
				rc = -EBUSY;
				goto error3;
			}

			bp1->base_addr = ioremap(bp1->mem_start, bp1->mem_end - bp1->mem_start +1);

			if(!bp1->base_addr){
				printk(KERN_ALERT "Could not allocate memory\n");
				rc = -EIO;
				goto error4;
			}

			counter ++;
			printk(KERN_WARNING "BRAM_image registered\n");
		 	return 0;//ALL OK

			error4:
				release_mem_region(bp1->mem_start, bp1->mem_end - bp1->mem_start + 1);	
			error3:
				return rc;

		case 2:
			
			bp2 = (struct IFS_info *) kmalloc(sizeof(struct IFS_info), GFP_KERNEL);
			if(!bp2){
				printk(KERN_ALERT "Could not allocate memory\n");
				return -ENOMEM;
			}

			bp2->mem_start = r_mem->start;
			bp2->mem_end = r_mem->end;
			printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


			if(!request_mem_region(bp2->mem_start, bp2->mem_end - bp2-> mem_start + 1, DRIVER_NAME)){
				printk(KERN_ALERT "Could not lock memory region at %p\n",(void *)bp2->mem_start);
				rc = -EBUSY;
				goto error6;
			}

			bp2->base_addr = ioremap(bp2->mem_start, bp2->mem_end - bp2->mem_start +1);

			if(!bp2->base_addr){
				printk(KERN_ALERT "Could not allocate memory\n");
				rc = -EIO;
				goto error5;
			}

			counter ++;
			printk(KERN_WARNING "BRAM_kernel registered\n");
		 	return 0;//ALL OK

			error5:
				release_mem_region(bp2->mem_start, bp2->mem_end - bp2->mem_start + 1);	
			error6:
				return rc;


		case 3:
			
			bp3 = (struct IFS_info *) kmalloc(sizeof(struct IFS_info), GFP_KERNEL);
			if(!bp3){
				printk(KERN_ALERT "Could not allocate memory\n");
				return -ENOMEM;
			}

			bp3->mem_start = r_mem->start;
			bp3->mem_end = r_mem->end;
			printk(KERN_INFO "Start address:%x \t end address:%x", r_mem->start, r_mem->end);


			if(!request_mem_region(bp3->mem_start, bp3->mem_end - bp3-> mem_start + 1, DRIVER_NAME)){
				printk(KERN_ALERT "Could not lock memory region at %p\n",(void *)bp3->mem_start);
				rc = -EBUSY;
				goto error8;
			}

			bp3->base_addr = ioremap(bp3->mem_start, bp3->mem_end - bp3->mem_start +1);

			if(!bp3->base_addr){
				printk(KERN_ALERT "Could not allocate memory\n");
				rc = -EIO;
				goto error7;
			}

			printk(KERN_WARNING "BRAM_after_conv registered\n");
		 	return 0;//ALL OK

			error7:
				release_mem_region(bp3->mem_start, bp3->mem_end - bp3->mem_start + 1);	
			error8:
				return rc;
	}

	//return 0;
}

static int IFS_remove(struct platform_device *pdev)
{

	printk(KERN_INFO "Counter is: %d\n", counter);

	switch(counter){

		case 0:
			printk(KERN_WARNING "IFS_remove: platform driver removing\n");
			iowrite32(0,ip->base_addr);
			iounmap(ip->base_addr);
			release_mem_region(ip->mem_start, ip->mem_end - ip->mem_start + 1);
			kfree(ip);
			printk(KERN_INFO"IFS_remove: IFS driver removed\n");

		break;

		case 1:
			printk(KERN_WARNING "BRAM_IMAGE_remove: platform driver removing\n");
			iowrite32(0,bp1->base_addr);
			iounmap(bp1->base_addr);
			release_mem_region(bp1->mem_start, bp1->mem_end - bp1->mem_start + 1);
			kfree(bp1);
			printk(KERN_INFO"BRAM_IMAGE_remove: BRAM_IMAGE removed\n");
			counter--;
		break;

		case 2:
			printk(KERN_WARNING "BRAM_KERNEL_remove: platform driver removing\n");
			iowrite32(0,bp2->base_addr);
			iounmap(bp2->base_addr);
			release_mem_region(bp2->mem_start, bp2->mem_end - bp2->mem_start + 1);
			kfree(bp2);
			printk(KERN_INFO"BRAM_KERNEL_remove: BRAM_KERNEL removed \n");
			counter--;
		break;

		case 3:
			printk(KERN_WARNING "BRAM_AFTER_CONV_remove: platform driver removing\n");
			iowrite32(0,bp3->base_addr);
			iounmap(bp3->base_addr);
			release_mem_region(bp3->mem_start, bp3->mem_end - bp3->mem_start + 1);
			kfree(bp3);
			printk(KERN_INFO"BRAM_AFTER_CONV_remove: BRAM_AFTER_CONV removed\n");
			counter--;
		break;

	}

	//return 0;
}



int IFS_open (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully opened file\n");
	return 0;

}

int IFS_close (struct inode *pinode, struct file *pfile){

	printk(KERN_INFO "Succesfully closed file\n");
	return 0;

}

ssize_t IFS_read (struct file *pfile, char __user *buf, size_t length, loff_t *offset){

	int ret, i=0, pos=0;
	char buff[BUFF_SIZE];
	int len,temp;
	int value, status, cmd, lines, columns, after_conv;
	int minor = MINOR(pfile->f_inode->i_rdev);
	switch(minor){

		case 0:

			status = ioread32(ip->base_addr+12);
			printk(KERN_INFO"Ready is: %d\n", status);
			cmd = ioread32(ip->base_addr +8);
			printk(KERN_INFO"Command is: %d\n", cmd);
			lines = ioread32(ip->base_addr+4);
			printk(KERN_INFO"Lines is: %d\n", lines);
			columns = ioread32(ip->base_addr);
			printk(KERN_INFO"Columns is: %d\n", columns);
			temp = scnprintf(buff, BUFF_SIZE, "%d\n", lines);
			ret=copy_to_user(buf,buff,len);
			if(ret)
			{
				return -EFAULT;
			}

			break;


		case 1:
			for(i=0; i<10; i++)
			{
				pos = position + i*4;
				value  = ioread32(bp1->base_addr + pos);
				printk (KERN_INFO "value is: %d\n",value);
				printk (KERN_INFO "position is: %d\n",pos);
				temp = scnprintf(buff, BUFF_SIZE, "%d\n", value);
				ret = copy_to_user(buf, buff, len);
				if(ret){
					return -EFAULT;
				}
			}
			break;


		case 2:

			for(i=0; i<10; i++)
			{
				pos = position + i*4;
				value  = ioread32(bp1->base_addr + pos);
				printk (KERN_INFO "value is: %d\n",value);
				printk (KERN_INFO "position is: %d\n",pos);
				temp = scnprintf(buff, BUFF_SIZE, "%d\n", value);
				ret = copy_to_user(buf, buff, len);
				if(ret){
					return -EFAULT;
				}
			}
			break;

		case 3:
			for(i=0; i<10; i++){

				after_conv = ioread32(bp3->base_addr+i*4);
				printk(KERN_INFO"after_conv is: %d \n", after_conv);
				printk(KERN_INFO "position is: %d\n",i*4);
				temp = scnprintf(buff, BUFF_SIZE, "%d\n", after_conv);
				ret=copy_to_user(buf,buff,len);
				if(ret)
				{
					return -EFAULT;
				}

			}
			break;
		default:
			printk(KERN_INFO"somethnig went wrong\n");
	}

	return len;
}

ssize_t IFS_write (struct file *pfile, const char __user *buf, size_t length, loff_t *offset){

	char buff[BUFF_SIZE];
	int minor = MINOR(pfile->f_inode->i_rdev);
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

	switch(minor){

		case 0:
			/*if (ret != -EINVAL){
				iowrite32(xpos, ip->base_addr); //columns
				iowrite32(ypos, ip->base_addr +4); //lines
				iowrite32(number, ip->base_addr +8); //cmd
				iowrite32(rgb, ip->base_addr +12); //status

			}*/
			break;


		case 1:
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
					//printk(KERN_INFO "number is: %d",number );
					position = (255*ypos+xpos)*4;
					for(i=0; i<=number; i++)
					{
						pos = position +i*4;
						printk(KERN_INFO "position is: %d\n",pos);
						printk(KERN_INFO "value is: %d\n",rgb);
						iowrite32(rgb,bp1->base_addr+pos);
					}
				}
			}
			else
			{
				printk(KERN_WARNING "IFS_write: Wrong write format\n");
				// return -EINVAL; //parsing error
			}

			break;

		case 2:
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

					position = (3*ypos+xpos)*4;
					printk(KERN_INFO"position is: %d\n",position);
					printk(KERN_INFO "value is :  %d\n",rgb);
					iowrite32(rgb,bp2->base_addr+position);
				}
			}
			else
			{
				printk(KERN_WARNING "IFS_write: Wrong write format\n");
				//return -EINVAL; //parsing error
			}
			break;


		case 3:

			printk(KERN_WARNING "IFS_write: cannot write in this BRAM \n");

		default:
			printk(KERN_INFO"somethnig went wrong\n");
	}

	return length;
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


	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),0), NULL, "bram_image");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_IMAGE\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_IMAGE\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),1), NULL, "bram_kernel");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_KERNEL\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_KERNEL\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),2), NULL, "bram_after_conv");
	if (my_device == NULL){
		printk(KERN_ERR "failed to create device BRAM_AFTER_CONV\n");
		goto fail_1;
	}
	printk(KERN_INFO "created BRAM_AFTER_CONV\n");
	my_device = device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id),3), NULL, "image_conv");

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
	printk(KERN_ALERT "IFS_exit: rmmod called\n");
	platform_driver_unregister(&IFS_driver);
	printk(KERN_INFO"IFS_exit: platform_driver_unregister done\n");
	cdev_del(my_cdev);
	printk(KERN_ALERT "IFS_exit: cdev_del done\n");
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
	printk(KERN_INFO"IFS_exit: device destroy 0\n");
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
	printk(KERN_INFO"IFS_exit: device destroy 1\n");
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
	printk(KERN_INFO"IFS_exit: device destroy 2\n");
	device_destroy(my_class, MKDEV(MAJOR(my_dev_id),3));
	printk(KERN_INFO"IFS_exit: device destroy 3\n");
	class_destroy(my_class);
	printk(KERN_INFO"IFS_exit: class destroy \n");
	unregister_chrdev_region(my_dev_id,4);
	printk(KERN_ALERT "Goodbye from IFS_driver\n");	

}

module_init(IFS_init);
module_exit(IFS_exit);
