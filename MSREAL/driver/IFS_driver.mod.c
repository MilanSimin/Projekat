#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfb080ece, "module_layout" },
	{ 0xd1f18866, "cdev_del" },
	{ 0x8a8062bf, "platform_driver_unregister" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x3e6badd6, "class_destroy" },
	{ 0x38e4b428, "__platform_driver_register" },
	{ 0xc8afcc70, "device_destroy" },
	{ 0xb9757c96, "cdev_add" },
	{ 0x4408f2c7, "cdev_alloc" },
	{ 0xe9e59beb, "device_create" },
	{ 0x2d0d89b2, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xf99fc839, "kmalloc_caches" },
	{ 0xe97c4103, "ioremap" },
	{ 0xae9849dd, "__request_region" },
	{ 0x92f552c6, "kmem_cache_alloc" },
	{ 0x3143af08, "platform_get_resource" },
	{ 0xc94d8e3b, "iomem_resource" },
	{ 0x37a0cba, "kfree" },
	{ 0x4384eb42, "__release_region" },
	{ 0xedc03953, "iounmap" },
	{ 0xb69e9c1a, "vm_iomap_memory" },
	{ 0xf4fa543b, "arm_copy_to_user" },
	{ 0xf9e73082, "scnprintf" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x20c55ae0, "sscanf" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x822137e2, "arm_heavy_mb" },
	{ 0x5f754e5a, "memset" },
	{ 0x28cc25db, "arm_copy_from_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x7c32d0f0, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cbram_image");
MODULE_ALIAS("of:N*T*Cbram_imageC*");
MODULE_ALIAS("of:N*T*Cbram_kernel");
MODULE_ALIAS("of:N*T*Cbram_kernelC*");
MODULE_ALIAS("of:N*T*Cbram_after_conv");
MODULE_ALIAS("of:N*T*Cbram_after_convC*");
MODULE_ALIAS("of:N*T*Cimage_conv");
MODULE_ALIAS("of:N*T*Cimage_convC*");
