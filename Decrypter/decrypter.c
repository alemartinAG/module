#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h>             
#include <linux/uaccess.h>         // Required for the copy to user function

#define  DEVICE_NAME "char-decrypt"
#define  CLASS_NAME  "ebb"
 
MODULE_LICENSE("GPL");            
 
static int    majorNumber;                  
static char   message[256] = {0};           
static short  size_of_message;
static int    numberOpens = 0;              // Cantidad de veces que se abrio el dispositivo
static struct class*  ebbcharClass  = NULL; ///< The device-driver class struct pointer
static struct device* ebbcharDevice = NULL; ///< The device-driver device struct pointer

static const int code_value = 5; //Valor a sumar en la encriptacion

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
 
//File Operations soportadas por el dispositivo
static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};
 
//Se llama cuando se carga el modulo
static int __init decrypt_init(void){

   printk(KERN_INFO "Decrypt: Inicializando el modulo de desencriptacion\n");
 
   // Se asigna un major number de manera dinamica
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "Fallo al registrar el major number\n");
      return majorNumber;
   }
 
   // Registro de la clase del dispositivo
   ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(ebbcharClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Fallo al registrar clase del dispositivo\n");
      return PTR_ERR(ebbcharClass);          // Correct way to return an error on a pointer
   }
 
   // Registro del driver del dispositivo
   ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(ebbcharDevice)){               // Clean up si hay algun error
      class_destroy(ebbcharClass);           // Repeated code but the alternative is goto statements
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to create the device\n");
      return PTR_ERR(ebbcharDevice);
   }

   printk(KERN_INFO "Decrypt: dispositivo registrado correctamente con major number %d\n", majorNumber);
   
   return 0;
}
 
//Se llama cuando se descarga el modulo
static void __exit decrypt_exit(void){
   
   device_destroy(ebbcharClass, MKDEV(majorNumber, 0));     // remuevo el dispositivo
   class_unregister(ebbcharClass);                         
   class_destroy(ebbcharClass);                             
   unregister_chrdev(majorNumber, DEVICE_NAME);

   printk(KERN_INFO "Decrypt: dispositivo eliminado!\n");
}
 
//Se llama cada vez que se abre el dispositivo
static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "Decrypt: El dispositivo ha sido abierto %d veces\n", numberOpens);
   return 0;
}
 
//Se llama cuando el dispositivo es leido desde el espacio de usuario
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   
   int error_count = 0;
   error_count = copy_to_user(buffer, message, size_of_message);
 
   if (error_count==0){            // Si no hay errores
      printk(KERN_INFO "Decrypt: Se enviaron %d caracteres al usuario\n", size_of_message);
      return (size_of_message = 0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "Decrypt: Fallo al enviar %d caracteres al usuario\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}
 
//Se llama cuando el dispositivo es escrito desde el espacio de usuario
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){

   sprintf(message, "%s", buffer);   
   size_of_message = strlen(message);

   int i = 0;

   printk(KERN_INFO "\nDecrypt: %zu caracteres recibidos del usuario\n", len);

   printk(KERN_INFO "\nDecrypt: Mensaje Recibido: %s\n", message);

   while(i < len){
      message[i] = message[i] - code_value;
      i++;
   }

   printk(KERN_INFO "\nDecrypt: Mensaje Encriptado: %s\n", message);

   return len;
}


//Se llama cuando el dispositivo es cerrado o liberado por el programa del espacio de usuario
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "EBBChar: Device successfully closed\n");
   return 0;
}
 
//macros
module_init(decrypt_init);
module_exit(decrypt_exit);
