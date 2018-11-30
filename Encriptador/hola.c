#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DEVICE_NAME "char-encrypt"

static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops = {
	.read = device_read,
	.write = device_write
};


static int Major;

int init_module(void)
{
	printk(KERN_INFO "Hola Mundo!\n");

	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Fallo al registrar dispositivo %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Adios Mundo Cruel!\n");
}


static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset){

	return 0;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off){

	return 0;

}


