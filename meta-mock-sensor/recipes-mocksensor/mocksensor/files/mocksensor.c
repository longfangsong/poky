#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h> // for copy_to_user
#include <linux/random.h>  // for get_random_bytes

// --- 设备元数据 ---
#define DEVICE_NAME "sensor"
#define CLASS_NAME "sensor_class"
#define MAX_MINOR 1
#define RANDOM_SIZE sizeof(u32) // 读取时返回 4 字节的随机数

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class = NULL;

static int rand_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "%s: Device opened.\n", DEVICE_NAME);
    return 0;
}

static int rand_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "%s: Device closed.\n", DEVICE_NAME);
    return 0;
}

static ssize_t rand_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    u32 random_number;
    int bytes_to_copy;
    unsigned long not_copied;

    bytes_to_copy = min((size_t)RANDOM_SIZE, count);

    if (*f_pos > 0)
        return 0;

    get_random_bytes(&random_number, RANDOM_SIZE);
    printk(KERN_INFO "%s: Generated random number: %u\n", DEVICE_NAME, random_number);

    not_copied = copy_to_user(buf, &random_number, bytes_to_copy);

    if (not_copied)
    {
        printk(KERN_WARNING "%s: Failed to copy %lu bytes to user.\n", DEVICE_NAME, not_copied);
        return -EFAULT;
    }

    *f_pos += bytes_to_copy;

    return bytes_to_copy;
}

static const struct file_operations rand_fops = {
    .owner = THIS_MODULE,
    .open = rand_open,
    .release = rand_release,
    .read = rand_read,
};

static int __init rand_char_init(void)
{
    int ret;

    printk(KERN_INFO "%s: Initializing the random char device.\n", DEVICE_NAME);

    ret = alloc_chrdev_region(&dev_num, 0, MAX_MINOR, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_WARNING "%s: Failed to allocate char device region.\n", DEVICE_NAME);
        return ret;
    }
    printk(KERN_INFO "%s: Registered device Major=%d Minor=%d\n",
           DEVICE_NAME, MAJOR(dev_num), MINOR(dev_num));

    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class))
    {
        printk(KERN_WARNING "%s: Failed to create device class.\n", DEVICE_NAME);
        unregister_chrdev_region(dev_num, MAX_MINOR);
        return PTR_ERR(my_class);
    }

    cdev_init(&my_cdev, &rand_fops);
    my_cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_cdev, dev_num, MAX_MINOR);
    if (ret < 0)
    {
        printk(KERN_WARNING "%s: Failed to add cdev.\n", DEVICE_NAME);
        class_destroy(my_class);
        unregister_chrdev_region(dev_num, MAX_MINOR);
        return ret;
    }

    if (device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME) == NULL)
    {
        printk(KERN_WARNING "%s: Failed to create device node.\n", DEVICE_NAME);
        cdev_del(&my_cdev);
        class_destroy(my_class);
        unregister_chrdev_region(dev_num, MAX_MINOR);
        return -1;
    }

    printk(KERN_INFO "%s: Device /dev/%s created successfully.\n", DEVICE_NAME, DEVICE_NAME);
    return 0;
}

static void __exit rand_char_exit(void)
{
    printk(KERN_INFO "%s: Exiting the random char device module.\n", DEVICE_NAME);

    device_destroy(my_class, dev_num);

    class_destroy(my_class);

    cdev_del(&my_cdev);

    unregister_chrdev_region(dev_num, MAX_MINOR);

    printk(KERN_INFO "%s: Cleanup complete.\n", DEVICE_NAME);
}

module_init(rand_char_init);
module_exit(rand_char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character device that returns a random 32-bit integer on read.");