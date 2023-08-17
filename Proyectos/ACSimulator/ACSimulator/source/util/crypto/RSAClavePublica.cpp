#include "RSAClavePublica.h"

RSAClavePublica::RSAClavePublica() {
   rsa=NULL;
}


RSAClavePublica::~RSAClavePublica() {
   if (rsa!=NULL) {
      RSA_free(rsa);
   }
}


void RSAClavePublica::inicializar(unsigned char *exp,int szexp,unsigned char *mod,int szmod) {

   rsa=RSA_new();

   mirror(mod,szmod);
   mirror(exp,szexp);

   rsa->n=BN_bin2bn((const char unsigned*)mod, szmod, NULL);
   rsa->e=BN_bin2bn((const char unsigned*)exp, szexp, NULL);
   

}

int RSAClavePublica::encriptar(const unsigned char *message,uint szmsg,unsigned char *ciphertext)
{
   int ret;

   ret=RSA_public_encrypt(szmsg, message,ciphertext,rsa,RSA_PKCS1_OAEP_PADDING);

   return (ret);
}

int RSAClavePublica::getModulo(unsigned char *mod) {

   BN_bn2bin(rsa->n,mod);
   
   mirror(mod,BN_num_bytes(rsa->n));

   return(BN_num_bytes(rsa->n));
}

int RSAClavePublica::getExponentePublico(unsigned char *exp) {
   
   BN_bn2bin(rsa->e,exp);

   mirror(exp,BN_num_bytes(rsa->e));
   
   return(BN_num_bytes(rsa->e));
}

void RSAClavePublica::mirror(unsigned char *bin,int szbin) {
   int i;
   unsigned char c;

   for(i=0;i<(szbin>>1);i++) {
      c=bin[i];
      bin[i]=bin[szbin-i-1];
      bin[szbin-i-1]=c;
   }
}

