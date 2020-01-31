#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/uio_driver.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>

#define PAGES 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bitchebe");
MODULE_DESCRIPTION("UIO VTF driver");

static struct uio_info* info;
static struct platform_device *pdev;
static struct task_struct *ts;
uint8_t * buffer = NULL;

static int uio_vtf_hotplug_open(struct uio_info *info, struct inode *inode)
{
    pr_info("%s called\n", __FUNCTION__);
    return 0;
}

static int uio_vtf_hotplug_release(struct uio_info *info, struct inode *inode)
{
    kthread_should_stop();
    pr_info("%s called\n", __FUNCTION__);
    return 0;
}

void free_mmap_pages(void *mem, int npages)
{
    int i;

    for(i = 0; i < npages * PAGE_SIZE; i += PAGE_SIZE) {
        ClearPageReserved(virt_to_page(((unsigned long)mem) + i));
    }
    kfree(mem);
}

int uio_vtf_hotplug_mmap(struct uio_info *info, struct vm_area_struct *vma){
    unsigned long len = vma->vm_end - vma->vm_start;

    unsigned long pfn = virt_to_phys((void *)buffer)>>PAGE_SHIFT;
    int ret ;

    ret = remap_pfn_range(vma, vma->vm_start, pfn, len, vma->vm_page_prot);
    if (ret < 0) {
        pr_err("could not map the address area\n");
        free_mmap_pages(buffer,PAGES);
        return -EIO;
    }

    pr_info("memory map called success \n");

    return ret;
}

void* alloc_mmap_pinned_pages(int npages)
{
    int i;
    uint8_t * buffer = kzalloc(PAGE_SIZE * npages,GFP_KERNEL);

    if (!buffer)
        return buffer;

    for(i = 0; i < npages * PAGE_SIZE; i += PAGE_SIZE) {
        SetPageReserved(virt_to_page(((unsigned long)buffer) + i));
    }

    pr_info("page alloc success \n");
    return (void*)buffer;
}

int kthread_vtf(void *arg)
{
	int count = 0;
	const int poll_interval = 2000;

	while(!kthread_should_stop())
	{
		msleep_interruptible(poll_interval);
                uio_event_notify(info);
		printk("chocokeki@debug - gathering : %d\n", count++);
	}

	return 0;
}

static int __init uio_vtf_hotplug_init(void)
{

    pdev = platform_device_register_simple("uio_vtf_device",
                                            0, NULL, 0);
    if (IS_ERR(pdev)) {
        pr_err("Failed to register platform device.\n");
        return -EINVAL;
    }

    info = kzalloc(sizeof(struct uio_info), GFP_KERNEL);
    
    if (!info)
        return -ENOMEM;
    
    if ( !(buffer=alloc_mmap_pinned_pages(PAGES))) {
        return -ENOMEM;
    }

    info->name = "uio_vtf_driver";
    info->version = "0.1";
    info->mem[0].addr = (phys_addr_t) buffer;
    if (!info->mem[0].addr)
        goto uiomem;
    info->mem[0].memtype = UIO_MEM_LOGICAL;
    info->mem[0].size = PAGE_SIZE;
    info->irq = UIO_IRQ_CUSTOM;
    info->handler = 0;
    info->mmap = uio_vtf_hotplug_mmap;
    info->open = uio_vtf_hotplug_open;
    info->release = uio_vtf_hotplug_release;
	
    if(uio_register_device(&pdev->dev, info)) {
        pr_err("Unable to register UIO device!\n");
        goto devmem;
    }

    ts = kthread_run(kthread_vtf, info, "uio_kthread_vtf");

    pr_info("VTF uio hotplug driver loaded\n");
    return 0;

devmem:
    kfree((void *)info->mem[0].addr);
uiomem:
    kfree(info);
    
    return -ENODEV;
}

static void __exit uio_vtf_hotplug_cleanup(void)
{
//    pr_info("Cleaning up module.\n");

    if (pdev)
        platform_device_unregister(pdev);    
    
    free_mmap_pages(buffer, PAGES);

    printk("Cleaning up module.\n");
}

module_init(uio_vtf_hotplug_init);
module_exit(uio_vtf_hotplug_cleanup);
