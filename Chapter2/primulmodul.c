#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/param.h>
#include <linux/jiffies.h>

int simple_init(void)
{
    printk(KERN_INFO "Numarul HZ este: %d\n", HZ);
    
    return 0;
}

void simple_exit(void)
{
    printk(KERN_INFO "Jiffies este: %ld\n", jiffies);
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Un prim modul");
MODULE_AUTHOR("EDI");
