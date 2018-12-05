#include <linux/init.h>
#include <linux/module.h>

int encrypt_module_init(void){

	printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);
	return 0;
}

void encrypt_module_exit(void){

	printk(KERN_ALERT "Inside %s function\n", __FUNCTION__);
}

module_init(encrypt_module_init);
module_exit(encrypt_module_exit);