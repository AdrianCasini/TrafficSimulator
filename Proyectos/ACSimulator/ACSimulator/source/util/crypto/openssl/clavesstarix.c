#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "interfazhost.h"

char keyfile[2048];
char seed[2048]="";

char *p;

int main(int argc,char *argv[]) {

   if (argc<2) {
      printf("\nUso: clavesstarix [fichero]\n");
      exit(0);
   }
   
   strcpy(keyfile,argv[1]);

   p=getpass("Password: ");
   strcpy(seed,p);
   p=getpass("Repita el Password: ");
   if (strcmp(seed,p)!=0) {
      printf("\n*ERROR*: Los Passwords no coinciden.\n");
      return 0;
   }
   
   printf("Generando fichero claves en %s ...\n",keyfile);

   switch(CryptoStarixGenerarFicheroClaves(keyfile,seed)) {
      case CRYPTO_IFZ_HOST_ERROR_FILE_EXISTS:
         printf("\n*ERROR*: %s ya existe.\n",keyfile);
         break;
      case CRYPTO_IFZ_HOST_ERROR_FILE_CREATE:
         printf("\n*ERROR*: %s no se puede crear.\n",keyfile);
         break;
      case CRYPTO_IFZ_HOST_ERROR_FILE_SEED:
         printf("\n*ERROR*: password incorrecto.\n");
         break;
      case CRYPTO_IFZ_HOST_OK:
         printf("\nOK: %s creado.\n",keyfile);
         exit(0);
         break;
   }   

   return 1;


}
