#include "interfazhost.h"


char keyfile[2048];
char buffer[2048]="";
int ret;

int main(int argc,char *argv[]) {

   int i;

   if (argc<2) {
      printf("\nUso: viewclaves [fichero]\n");
      exit(0);
   }
   
   strcpy(keyfile,argv[1]);

   
   for (i=0;i<100;i++) {
      if ((ret=CryptoStarixObtenerClaveSEGU(keyfile,i,buffer,sizeof(buffer)))!=0) {
         printf("CryptoStarixObtenerClaveSEGU error %d\n",ret);
         exit(0);
      }
      else {
         printf("*%02d*:%s\n",i,buffer);
      }
   }

   exit(1);


}
