#include "interfazhost.h"
#include <sys/time.h>
#include <time.h>


char keyfile[2048];
char pan[2048];
char pan_crypted[2048];
char tdes_crypted[2048];
unsigned char clavesdes[2048];
char buffer[2048]="";
int ret;
int idx;

int main(int argc,char *argv[]) {
   struct timeval tv;
   int i;

   if (argc<4) {
      printf("\nUso: cryptpan [fichero claves] [indice] [pan]\n");
      exit(0);
   }

   gettimeofday(&tv,NULL);
   srand(tv.tv_usec);

   for(i=0;i<128;i++) {
      clavesdes[i]=(unsigned char) rand();
   }
   
   strcpy(keyfile,argv[1]);
   idx=atoi(argv[2]);
   strcpy(pan,argv[3]);

   if ((ret=CryptoStarixEncriptarTDES(keyfile,idx,clavesdes,tdes_crypted,sizeof(tdes_crypted)))==0) {
      printf("TDES encriptada: %s\n",tdes_crypted);
      if ((ret=CryptoStarixEncriptarPAN(clavesdes,pan,pan_crypted,sizeof(pan_crypted)))==0) {
         printf("PAN  encriptado: %s\n",pan_crypted);
         exit(1);
      }
      else {
         printf("CryptoStarixEncriptarPAN error %d\n",ret);
      }
   }
   else {
      printf("CryptoStarixEncriptarTDES error %d\n",ret);
   }
   exit(0);


}
