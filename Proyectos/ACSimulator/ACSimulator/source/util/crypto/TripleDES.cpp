#include "TripleDES.h"

TripleDES::TripleDES() {

}

TripleDES::~TripleDES() {

}

void TripleDES::setClaves(int numclaves,unsigned char *claves) {
   memcpy(keys,claves,TripleDES_KEY_SIZE*numclaves);
}

void TripleDES::getClaves(int numclaves,unsigned char *buffer) {
   memcpy(buffer,keys,TripleDES_KEY_SIZE*numclaves);
}

int TripleDES::generarClaves(int numclaves) {
   createKey(keys,TripleDES_KEY_SIZE*numclaves);
   return(TripleDES_KEY_SIZE*numclaves);
}


bool TripleDES::EDE(byte *k1,byte *k2,byte *k3,int key_size,byte *data,int szdata,byte *result) {
   int i;

   DES_key_schedule dk1;
   DES_key_schedule dk2;
   DES_key_schedule dk3;

   DES_key_sched((const_DES_cblock *)k1,&dk1);
   DES_key_sched((const_DES_cblock *)k2,&dk2);
   DES_key_sched((const_DES_cblock *)k3,&dk3);

   for(i=0;i<szdata;i+=key_size) {
#if OPENSSL_VERSION_NUMBER <= 0x0090705FL
      DES_ecb3_encrypt((const unsigned char *)(data+i),(unsigned char *) (result+i),&dk1, &dk2, &dk3,DES_ENCRYPT);
#else //0x009080bfL
      DES_ecb3_encrypt((const_DES_cblock *)(data+i),(DES_cblock *) (result+i),&dk1, &dk2, &dk3,DES_ENCRYPT);
#endif
   }

   return true;
}

bool TripleDES::DED(byte *k1,byte *k2,byte *k3,int key_size,byte *data,int szdata,byte *result) {
   int i;

   DES_key_schedule dk1;
   DES_key_schedule dk2;
   DES_key_schedule dk3;

   DES_key_sched((const_DES_cblock *)k1,&dk1);
   DES_key_sched((const_DES_cblock *)k2,&dk2);
   DES_key_sched((const_DES_cblock *)k3,&dk3);

   for(i=0;i<szdata;i+=key_size) {
#if OPENSSL_VERSION_NUMBER <= 0x0090705FL
      DES_ecb3_encrypt((const unsigned char *)(data+i),(unsigned char *) (result+i),&dk1, &dk2, &dk3,DES_DECRYPT);
#else
      DES_ecb3_encrypt((const_DES_cblock *)(data+i),(DES_cblock *) (result+i),&dk1, &dk2, &dk3,DES_DECRYPT);
#endif
   }

   return true;
}

void TripleDES::createKey(unsigned char *key,int szkey) {
   struct timeval tv;
   int i;
   
   gettimeofday(&tv,NULL); 
   srand(tv.tv_usec);

   for(i=0;i<szkey;i++) {
      key[i]=(rand() % 256 );
   }
}



