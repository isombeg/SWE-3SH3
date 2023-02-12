#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"

// Global vars
unsigned long init_jiffies;

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

int proc_init(void){
    // gets jiffies at initialization
    init_jiffies = jiffies;
    
    // creates the /proc/seconds entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

void proc_exit(void){
    // removes the /proc/seconds entry
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}


/**
 * This function is called each time the /proc/seconds is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.

 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
    // todo: display time elapsed
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
            completed = 0;
            return 0;
    }

    completed = 1;
    
    // compute time elapsed
    unsigned long time_elapsed = (jiffies - init_jiffies)/HZ;
    

    rv = sprintf(buffer, "Seconds elapsed %lu\n", );

    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("SGG");
