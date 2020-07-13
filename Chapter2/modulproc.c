#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static  struct proc_ops p_ops = {
    //.owner = THIS_MODULE,
    .proc_read  = proc_read
};

static u64 init_jiffies;

int proc_init(void)
{
    init_jiffies = jiffies; 
    proc_create(PROC_NAME, 0666, NULL, &p_ops);
    
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    u64 nr_sec_init = (jiffies / HZ) - (init_jiffies / HZ);
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    
    if (completed)
    {
        completed = 0;
        return 0;
    }
    
    completed = 1;
    
    rv = sprintf(buffer, "%llu\n", nr_sec_init);
    copy_to_user(usr_buf, buffer, rv);
    
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Un prim modul");
MODULE_AUTHOR("EDI");
