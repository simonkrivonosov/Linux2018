#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SimonRus");
MODULE_DESCRIPTION("Linux core module.");
MODULE_VERSION("0.01");
#define DEVICE_NAME "lkm_example"
#define EXAMPLE_MSG "Hello, "
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[128];

/* This structure points to all of the device functions */
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};
/* When a process reads from our device, this gets called. */
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
 /* If we’re at the end, loop back to the beginning */
 /* Put data in the buffer */
    copy_to_user(buffer, msg_buffer, strlen(msg_buffer) + 1);
    return bytes_read;
}
/* Called when a process tries to write to our device */
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
    int my_offset = 0;
    strncpy(msg_buffer, EXAMPLE_MSG, strlen(EXAMPLE_MSG));
    my_offset += strlen(EXAMPLE_MSG);
    strncpy(msg_buffer + my_offset, buffer, strlen(buffer) + 1);
    return -EINVAL;
}
/* Called when a process opens our device */
static int device_open(struct inode *inode, struct file *file) {
 /* If device is open, return busy */
    if (device_open_count) {
        return -EBUSY;
    }
    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}
/* Called when a process closes our device */
static int device_release(struct inode *inode, struct file *file) {
 /* Decrement the open counter and usage count. Without this, the module would not unload. */
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}
static int __init lkm_example_init(void) {
    major_num = register_chrdev(0, "lkm_example", &file_ops);
    if (major_num < 0) {
        printk(KERN_ALERT "Could not register device: %d\n", major_num);
        return major_num;
    }
    else {
        printk(KERN_INFO "lkm_example module loaded with device major number %d\n", major_num);
        return 0;
    }
}
static void __exit lkm_example_exit(void) {
 /* Remember — we have to clean up after ourselves. Unregister the character device. */
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "Goodbye, World!\n");
}
/* Register module functions */
module_init(lkm_example_init);
module_exit(lkm_example_exit);