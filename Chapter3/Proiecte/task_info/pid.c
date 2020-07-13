#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/types.h>

#define buf_size 64

ssize_t read_pid(struct file *, char __user *, size_t, loff_t *);
ssize_t write_pid(struct file *, const char __user *, size_t, loff_t *);

static const struct proc_ops p_ops = {
    .proc_read = read_pid,
    .proc_write = write_pid
};

static struct task_struct *tsk;

static int mod_init(void)
{
    printk(KERN_INFO "Am intrat!\n");
    proc_create("pid", 0666, NULL, &p_ops);
    
    return 0;
}

static void mod_exit(void)
{
    printk(KERN_INFO "Am iesit!\n");
    remove_proc_entry("pid", NULL);
}

ssize_t read_pid(struct file *f, char __user *usr_buf, size_t count, loff_t *pos)
{
    int nr = 0;
    static int terminat = 0;
    
    if (terminat)
    {
        terminat = 0; // marcam sfarsitul citirii
        return 0; // intoarcem 0 pt a indica EOF
    }
    
    terminat = 1;
    
    if (tsk != NULL)
    {
        char buf[buf_size];
        nr = sprintf(buf, "Nume: %s, PID: %d, Status: %ld\n", tsk->comm, tsk->pid, tsk->state);
        copy_to_user(usr_buf, buf, nr);
    }
    
    return nr;
}

ssize_t write_pid(struct file *f, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem;
    long rez;
    pid_t pid;
    
    k_mem = (char*) kmalloc(count, GFP_KERNEL);
    copy_from_user(k_mem, usr_buf, count);
    kstrtol(k_mem, 10, &rez);
    kfree(k_mem);
    pid = rez;
    
    tsk = pid_task(find_vpid(pid), PIDTYPE_PID);
    //if (ts != NULL)
    //   printk(KERN_INFO "Comanda: %s\n", ts->comm);
    
    return count;
}


module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Informatii taskuri");
MODULE_AUTHOR("EDI");
