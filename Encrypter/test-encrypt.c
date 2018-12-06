#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];
 
int main(){

   int ret, fd;

   char stringToSend[BUFFER_LENGTH];

   printf("\nEmpezando Prueba de Dispositivo\n");
   
   fd = open("/dev/char-encrypt", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   
   while(1){

      printf("\nEscribe el mensaje a encriptar\n(Escribe 'LEER' para leer el mensaje encriptado y 'EXIT' para salir)\n\n");
      scanf("%[^\n]%*c", stringToSend);                //Read in a string (with spaces)
      //printf("Writing message to the device [%s].\n", stringToSend);
      
      if(!strcmp(stringToSend, "LEER")){

         printf("Reading from the device...\n");
         ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
         if (ret < 0){
            perror("Failed to read the message from the device.");
            return errno;
         }

         printf("The received message is: [%s]\n", receive);
      }
      else if(!strcmp(stringToSend, "EXIT")){
         
         break;
      }
      else{

         ret = write(fd, stringToSend, strlen(stringToSend)); //Envio el mensaje al LKM.

         if (ret < 0){
            perror("\nFallo al escribir el mensaje\n");
            return errno;
         }
      }
   }
   
   printf("Adios!\n");
   return 0;
}
