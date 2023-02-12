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

<<<<<<< HEAD
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

=======
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

<<<<<<< HEAD
/* This function is called when the module is loaded. */
=======
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
int proc_init(void){
    // gets jiffies at initialization
    init_jiffies = jiffies;
    
    // creates the /proc/seconds entry
<<<<<<< HEAD
=======
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

<<<<<<< HEAD
/* This function is called when the module is removed. */
=======
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
void proc_exit(void){
    // removes the /proc/seconds entry
    remove_proc_entry(PROC_NAME, NULL);

    printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}


/**
<<<<<<< HEAD
 * This function is called each time the /proc/seconds is read. Computes the number of seconds elapsed and writes
 * it to /proc/seconds file. 
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    
    // compute time elapsed
    unsigned long time_elapsed = (jiffies - init_jiffies)/HZ;

    //ensure function returns 0 after running
=======
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

>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
    if (completed) {
            completed = 0;
            return 0;
    }

<<<<<<< HEAD
    completed = 1;    

    rv = sprintf(buffer, "Seconds elapsed %lu\n", time_elapsed);
=======
    completed = 1;
    
    // compute time elapsed
    unsigned long time_elapsed = (jiffies - init_jiffies)/HZ;
    

    rv = sprintf(buffer, "Seconds elapsed %lu\n", );
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133

    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}

/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
<<<<<<< HEAD
MODULE_AUTHOR("SGG");
=======
MODULE_AUTHOR("SGG");
>>>>>>> 41194abe2fc57b12443c182b8888be03fc71a133
