#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/uaccess.h>          // Required for the copy to user function

#define DEVICE_NAME "encrypt_driver"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
	.owner 	= THIS_MODULE,
	.read 	= device_read,
	.write 	= device_write,
	.open 	= device_open,
	.release = device_release
};


static int 	majorNumber;
//static bool Device_Open = false;
//static char *msg_Ptr;
static char message[256] = {0};
static int 	numberOpens = 0; 
static short size_of_message;

int hola_module_init(void) {

	printk(KERN_INFO "Hola Mundo!\n");

	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);

	if (majorNumber < 0) {
	  printk(KERN_ALERT "Fallo al registrar dispositivo %d\n", majorNumber);
	  return majorNumber;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", majorNumber);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod -m 666 /dev/%s c %d 0'.\n", DEVICE_NAME, majorNumber);
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return 0;
}

void hola_module_exit(void) {

	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "Adios Mundo Cruel!\n");
}

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file) {

	numberOpens++;
	//sprintf(msg, "ABRISTE EL DEVICE W8");
	//printk(KERN_INFO "KEONDA PERRO");
	//msg_Ptr = msg;

	//try_module_get(THIS_MODULE);

	printk(KERN_ALERT "Inside %s function; Device has been opened %d times\n", __FUNCTION__, numberOpens);

	return 0;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	//Device_Open = false;		/* We're now ready for our next caller */

	printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	//module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset) {

	printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);

	int error_count = 0;
	error_count = copy_to_user(buffer, message, size_of_message);
 	
 	if (error_count == 0){            // if true then have success
      printk(KERN_INFO "Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }

	return 0;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t * off){

	printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);

	sprintf(message, "%s(%zu letters)", buffer, len);
	size_of_message = strlen(message);
	printk(KERN_INFO "\nReceived %zu characters from the user\n", len);

	return len;

}


module_init(hola_module_init);
module_exit(hola_module_exit);