#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/list.h>

struct color
{
    int red;
    int green;
    int blue;
    
    struct list_head list;
};


static int mod_init(void)
{
    static LIST_HEAD(color_list);
    struct color *violet, *black;
    struct color *ptr, *next;
    
    printk(KERN_INFO "Am intrat!");
    
    
    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 138;
    violet->green = 226;
    violet->blue = 43;
    INIT_LIST_HEAD(&violet->list);
    
    black = kmalloc(sizeof(*black), GFP_KERNEL);
    black->red = 0;
    black->green = 0;
    black->blue = 0;
    INIT_LIST_HEAD(&black->list);
    
    list_add_tail(&violet->list, &color_list);
    list_add_tail(&black->list, &color_list);
    
    list_for_each_entry(ptr, &color_list, list){
        printk(KERN_INFO "Rosu: %d\n", ptr->red);
    }
    
    
    list_for_each_entry_safe(ptr, next, &color_list, list){
        printk(KERN_INFO "Sters.\n");
        list_del(&ptr->list);
        kfree(ptr);
        
    }
    
    
    return 0;
}

static void mod_exit(void)
{
    
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Liste");
MODULE_AUTHOR("EDI");
