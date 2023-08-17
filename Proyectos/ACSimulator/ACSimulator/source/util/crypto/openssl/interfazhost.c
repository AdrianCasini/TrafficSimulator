#include "interfazhost.h"

char gsemilla[2048]="";
static unsigned long next = 1;

/* RAND_MAX a= 32767 */
unsigned long myrand(void) {
   next = next * 1103515245 + 12345;
   return((unsigned long)(next/65536) % 32768);
}

void mysrand(unsigned long seed) {
   next = seed;
}


void myseed(const void *buf, int num) {
}

int mypseudorand(unsigned char *buf, int num) {
   int i;
   
   for(i=0;i<num;i++) {
      buf[i]=(unsigned char) (myrand()+gsemilla[i%strlen(gsemilla)]);
   }
   return 1;
}

int mybytes(unsigned char *buf, int num) {
   int i;

   for(i=0;i<num;i++) {
      buf[i]=(myrand()+gsemilla[i%strlen(gsemilla)]);
   }
   return 1;
}
void mycleanup(void){
}
void myadd(const void *buf, int num, double entropy) {
}
int mystatus(void) {
   return 1;
}

void mirror(unsigned char *bin,int szbin) {
   int i;
   unsigned char c;

   for(i=0;i<(szbin>>1);i++) {
      c=bin[i];
      bin[i]=bin[szbin-i-1];
      bin[szbin-i-1]=c;
   }
}

void _CryptoStarixHexToBin(char *line,unsigned char *pbin,int szbin) {
   int i;
   unsigned int x;
   char hex[3]="XX";

   for(i=0;i<szbin;i++) {
      hex[0]=line[0];
      hex[1]=line[1];
      if (sscanf(hex,"%x",&x)!=1) x=0;
      pbin[i]=(unsigned char)x;
      line+=2;
   }
}

void _CryptoStarixBinToHex(char *line,unsigned char *pbin,int szbin) {
   int i;
   char hex[3]="XX";
   
   *line=0;
   for(i=0;i<szbin;i++) {
      sprintf(hex,"%02x",pbin[i]);
      strcat(line,hex);
   }
}

int _CryptoStarixObtenerClave(char *pathFicheroClaves,int index,struct _ifz_host_key *key) {
   int fd;
   struct _ifz_host_key *keylist;
   struct stat mstat;

   if ((index<0) || (index>=CRYPTO_IFZ_HOST_FILE_NUM_KEYS)) return CRYPTO_IFZ_HOST_ERROR_KEY_INDEX;

   if (index==0) {
      memset(key,0,sizeof(struct _ifz_host_key));
      key->szm=0x40;
      key->m[0x3f]=0x01;
      key->sze=0x01;
      key->e[0]=0x11;
      return CRYPTO_IFZ_HOST_OK;
   }

   if (stat(pathFicheroClaves,&mstat)!=-1) {
      if ((fd=open(pathFicheroClaves,O_RDONLY))!=-1) {
         if((keylist=(struct _ifz_host_key  *) mmap(NULL,CRYPTO_IFZ_HOST_FILE_SIZE,PROT_READ,MAP_PRIVATE,fd,0))!=MAP_FAILED) {
            memcpy(key,&keylist[index],sizeof(struct _ifz_host_key));
            munmap((char *)keylist,CRYPTO_IFZ_HOST_FILE_SIZE);
            close(fd);

            return CRYPTO_IFZ_HOST_OK;
         }
         close(fd);
      }
      return CRYPTO_IFZ_HOST_ERROR_FILE_OPEN;
   }
   return CRYPTO_IFZ_HOST_ERROR_FILE_NOEXISTS;
   
}

int CryptoStarixGenerarFicheroClaves(char *pathFicheroClaves,char *semilla) {
   int fd;
   int i;
   int ret;
   RSA *rsa;
   RAND_METHOD rm;
   struct stat mstat;
   struct _ifz_host_key key[CRYPTO_IFZ_HOST_FILE_NUM_KEYS];

   if (((semilla!=NULL) && (strlen(semilla)>=CRYPTO_IFZ_HOST_MAX_SEED_SIZE)) || (strlen(semilla)==0)) {
         return CRYPTO_IFZ_HOST_ERROR_FILE_SEED;
   }

   strcpy(gsemilla,semilla);

   if (stat(pathFicheroClaves,&mstat)==-1) {

      memcpy(&rm,RAND_get_rand_method(),sizeof(RAND_METHOD));
   
      rm.seed=&myseed;
      rm.pseudorand=&mypseudorand;
      rm.bytes=&mybytes;
      rm.cleanup=&mycleanup;
      rm.add=&myadd;
      rm.status=&mystatus;
   
      RAND_set_rand_method(&rm);

      srand(1);
      for(i=0;i<strlen(semilla);i++) {
         mysrand(myrand()+semilla[i]);
      }

      for (i=0;i<CRYPTO_IFZ_HOST_FILE_NUM_KEYS;i++) {
         if ((rsa=RSA_generate_key(512,0x11,NULL,NULL))!=NULL) {
            key[i].szm=BN_num_bytes(rsa->n);
            key[i].sze=BN_num_bytes(rsa->e);
            key[i].szd=BN_num_bytes(rsa->d);
            
            BN_bn2bin(rsa->n,key[i].m);
            BN_bn2bin(rsa->e,key[i].e);
            BN_bn2bin(rsa->d,key[i].d);

            RSA_free(rsa);
         }
         else {
            return CRYPTO_IFZ_HOST_ERROR;
         }

      }
      if ((fd=open(pathFicheroClaves,O_CREAT | O_EXCL | O_RDWR, 0644))!=-1) {
         if ((ret=write(fd,key,sizeof(key)))==sizeof(key)) {
            close(fd);
            return CRYPTO_IFZ_HOST_OK;
         }
      }
      else {
         return CRYPTO_IFZ_HOST_ERROR_FILE_CREATE;
      }
      
   }
   else {
      return CRYPTO_IFZ_HOST_ERROR_FILE_EXISTS;
   }
   
   return CRYPTO_IFZ_HOST_ERROR;
}

int CryptoStarixObtenerClaveSEGU(char *pathFicheroClaves,int index,char *buffer,int szbuffer) {
   int ret;
   struct _ifz_host_key key;
   int i;
   char hex[16];

   if ((ret=_CryptoStarixObtenerClave(pathFicheroClaves,index,&key))==CRYPTO_IFZ_HOST_OK) {
      if ((buffer!=NULL) && (szbuffer>=140)) {

         //Important crypto++ treballa amb claus big-endian i open ssl little-endian
         mirror(key.e,key.sze);
         mirror(key.m,key.szm);

         sprintf(buffer,"%02x",key.sze);
         for (i=0;i<key.sze;i++) {
            sprintf(hex,"%02x",key.e[i]);
            strcat(buffer,hex);
         }
         for (i=0;i<key.szm;i++) {
            sprintf(hex,"%02x",key.m[i]);
            strcat(buffer,hex);
         }
         strcat(buffer,"$");
         return CRYPTO_IFZ_HOST_OK;
      }
   }
   else {
      return ret;
   }
   
   return CRYPTO_IFZ_HOST_ERROR;
}

int CryptoStarixDesencriptarPAN(char *pathFicheroClaves,int index,char *cryptedTDES,char *cryptedPAN,char *PAN) {
   int ret;
   struct _ifz_host_key key;
   RSA *rsa;
   unsigned char bin[1024];
   unsigned char tdeskey[64];
   char hex[2048];
   int i;
   int equchar;

   DES_key_schedule k1;
   DES_key_schedule k2;

   _CryptoStarixHexToBin(cryptedTDES,bin,sizeof(bin));

   if ((ret=_CryptoStarixObtenerClave(pathFicheroClaves,index,&key))==CRYPTO_IFZ_HOST_OK) {

      rsa=RSA_new();
      _CryptoStarixBinToHex(hex,key.m,key.szm);
      ret=BN_hex2bn(&rsa->n, hex);
      _CryptoStarixBinToHex(hex,key.e,key.sze);
      ret=BN_hex2bn(&rsa->e, hex);
      _CryptoStarixBinToHex(hex,key.d,key.szd);
      ret=BN_hex2bn(&rsa->d, hex);
      
      ret=RSA_private_decrypt(64, bin,tdeskey,rsa,RSA_PKCS1_OAEP_PADDING);
   
      RSA_free(rsa);
      if (ret==16) {

         _CryptoStarixHexToBin(cryptedPAN,bin,sizeof(bin));

         DES_key_sched((const_DES_cblock *)tdeskey,&k1);
         DES_key_sched((const_DES_cblock *)(tdeskey+8) ,&k2);

#ifdef __LINUX__
         DES_ecb2_encrypt((const_DES_cblock *)bin,(DES_cblock *) PAN,
               &k1, &k2,DES_DECRYPT);
         DES_ecb2_encrypt((const_DES_cblock *)(bin+8),(DES_cblock *)(PAN+8),
               &k1, &k2,DES_DECRYPT);
#else
         DES_ecb2_encrypt(bin,(unsigned char *) PAN,
               &k1, &k2,DES_DECRYPT);
         DES_ecb2_encrypt((bin+8),(unsigned char *)(PAN+8),
               &k1, &k2,DES_DECRYPT);
#endif

         PAN[16]=0;
         equchar=0;
         for(i=0;i<13;i++) {
            if (!isdigit(PAN[i])) return CRYPTO_IFZ_HOST_ERROR_PAN;
         }
         for(i=13;i<16;i++) {
            if (PAN[i]=='=') {
               if (equchar==1) return CRYPTO_IFZ_HOST_ERROR_PAN;
               equchar=1;
            }
            else {
               if (!isdigit(PAN[i])) return CRYPTO_IFZ_HOST_ERROR_PAN;
            }
         }
         return CRYPTO_IFZ_HOST_OK;

      }
      return CRYPTO_IFZ_HOST_ERROR_TDES;
   }
   else {
      return ret;
   }
   
   return CRYPTO_IFZ_HOST_ERROR;
}


#ifdef _TEST_CRYPTO_IFZ_HOST

int CryptoStarixEncriptarTDES(char *pathFicheroClaves,int index,unsigned char *claveTDES,char *buffer,int szbuffer) {
   int ret;
   struct _ifz_host_key key;
   RSA *rsa;
   char hex[2048];
   unsigned char bin[2048];

   if ((ret=_CryptoStarixObtenerClave(pathFicheroClaves,index,&key))==CRYPTO_IFZ_HOST_OK) {     

      rsa=RSA_new();

      _CryptoStarixBinToHex(hex,key.m,key.szm);
      ret=BN_hex2bn(&rsa->n, hex);
      _CryptoStarixBinToHex(hex,key.e,key.sze);
      ret=BN_hex2bn(&rsa->e, hex);
      
      ret=RSA_public_encrypt(16, claveTDES,bin,rsa,RSA_PKCS1_OAEP_PADDING);
      RSA_free(rsa);
      if (ret!=-1) {
         if (szbuffer>2*ret) {
            _CryptoStarixBinToHex(buffer,bin,ret);
            return CRYPTO_IFZ_HOST_OK;
         }
      }

   }
   else {
      return ret;
   }
   return CRYPTO_IFZ_HOST_ERROR;
}

int CryptoStarixEncriptarPAN(unsigned char *claveTDES,char *PAN,char *buffer,int szbuffer) {
   
   unsigned char bin[32];
   int i;
   char hex[16];

   DES_key_schedule k1;
   DES_key_schedule k2;

   if ((buffer!=NULL) && (szbuffer>=33)) {

      DES_key_sched((const_DES_cblock *)claveTDES,&k1);
      DES_key_sched((const_DES_cblock *)(claveTDES+8) ,&k2);

#ifndef __SOLARIS_8__
      DES_ecb2_encrypt((const_DES_cblock *)PAN,(DES_cblock *) bin,
               &k1, &k2,DES_ENCRYPT);
      DES_ecb2_encrypt((const_DES_cblock *)(PAN+8),(DES_cblock *)(bin+8),
               &k1, &k2,DES_ENCRYPT);
#else
      DES_ecb2_encrypt((unsigned char *)PAN,bin,
               &k1, &k2,DES_ENCRYPT);
      DES_ecb2_encrypt((unsigned char *)(PAN+8),(bin+8),
               &k1, &k2,DES_ENCRYPT);
#endif
 
      buffer[0]=0;
      for(i=0;i<16;i++) {
         sprintf(hex,"%02x",bin[i]);
         strcat(buffer,hex);
      }
      return CRYPTO_IFZ_HOST_OK;

   }

   return CRYPTO_IFZ_HOST_ERROR;
}

#endif
