#include "interfazhost.h"


char keyfile[2048];
char pan[2048];
char pan_crypted[2048];
char tdes_crypted[2048];
int ret;
int idx;

int main(int argc,char *argv[]) {
   int i;
   char hex[3];


   if (argc<5) {
      printf("\nUso: decryptpan [fichero claves] [indice] [tdes encriptada] [pan encriptado]\n");
      exit(0);
   }

   
   strcpy(keyfile,argv[1]);
   idx=atoi(argv[2]);
   strcpy(tdes_crypted,argv[3]);
   strcpy(pan_crypted,argv[4]);   

   if ((ret=CryptoStarixDesencriptarPAN(keyfile,idx,tdes_crypted,pan_crypted,pan))==0) {
      printf("PAN: \"%s\"\n",pan);
      exit(1);
   }
   else {
      printf("CryptoStarixDesencriptarPAN error %d\n",ret);
      if (ret==CRYPTO_IFZ_HOST_ERROR_PAN) {
         printf("PAN erroneo (hex): ");
         for(i=0;i<16;i++) {
            sprintf(hex,"%02X",pan[i]);
            printf(hex);
         }
         printf("\n");
      }
   }
   exit(0);


}
