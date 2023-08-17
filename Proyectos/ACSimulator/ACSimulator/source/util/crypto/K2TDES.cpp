#include "K2TDES.h"

K2TDES::K2TDES() {

}

K2TDES::~K2TDES() {

}

void K2TDES::setClaves(unsigned char *claves) {
   TripleDES::setClaves(2,claves);
}

void K2TDES::getClaves(unsigned char *buffer) {
   TripleDES::getClaves(2,buffer);
}

int  K2TDES::generarClaves(void) {
   return(TripleDES::generarClaves(2));
}

bool K2TDES::encriptar(byte *data,int szdata,byte *result) {
   return (TripleDES::EDE(keys,keys+TripleDES_KEY_SIZE,keys,TripleDES_KEY_SIZE,data,szdata,result));
}

bool K2TDES::desencriptar(byte *data,int szdata,byte *result) {
   return (TripleDES::DED(keys,keys+TripleDES_KEY_SIZE,keys,TripleDES_KEY_SIZE,data,szdata,result));
}
