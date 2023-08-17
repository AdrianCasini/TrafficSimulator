#include "RSAClavePrivada.h"

RSAClavePrivada::RSAClavePrivada() 
{
   rsa=NULL;
}


RSAClavePrivada::~RSAClavePrivada() 
{
   if (rsa!=NULL) {
      RSA_free(rsa);
   }
}


void RSAClavePrivada::generar(int size,char *key) {
   if (rsa!=NULL) {
      RSA_free(rsa);
   }
   rsa=RSA_generate_key(size,0x11,NULL,NULL);

}

void RSAClavePrivada::inicializar(unsigned char *exp,int szexp,unsigned char *mod,int szmod,unsigned char *pexp,int szpexp) {
   
   RSAClavePublica::inicializar(exp,szexp,mod,szmod);
   rsa->d=BN_bin2bn((const char unsigned*)pexp, szpexp, NULL);
}

int RSAClavePrivada::getExponentePrivado(unsigned char *pexp) {
   BN_bn2bin(rsa->d,pexp);
   
   return(BN_num_bytes(rsa->d));
}

int RSAClavePrivada::desencriptar(const unsigned char *ciphertext,uint szciph,unsigned char *message)
{
   int ret;

   ret=RSA_private_decrypt(szciph, ciphertext,message,rsa,RSA_PKCS1_OAEP_PADDING);
   
   return ret;
}

