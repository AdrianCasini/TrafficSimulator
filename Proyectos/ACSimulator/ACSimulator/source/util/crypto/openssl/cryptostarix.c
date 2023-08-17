
#include <string.h>
#include "cryptostarix.h"
#include "interfazhost.h"
#include "natuser.h"

#ifdef __cplusplus
extern "C"
{
#endif


NATFCT GENCLV NATARGDCL(nparm,parmptr,parmdec);
NATFCT GETCLV  NATARGDCL(nparm,parmptr,parmdec);
NATFCT DECRIPT NATARGDCL(nparm,parmptr,parmdec);

#define _PARAM_PATH_SIZE      255
#define _PARAM_SEED_SIZE      40 
#define _PARAM_SEGU_SIZE      517
#define _PARAM_CRYPTDES_SIZE  128
#define _PARAM_CRYPPAN_SIZE   32


NATFCT GENCLV NATARGDCL(nparm,parmptr,parmdec) 
{

   int ret;
   char pathFicheroClaves[_PARAM_PATH_SIZE+1];
   char semilla[_PARAM_SEED_SIZE+1];

   memmove(pathFicheroClaves,(char *)parmptr[0],_PARAM_PATH_SIZE);
   pathFicheroClaves[_PARAM_PATH_SIZE]=0;
   memmove(semilla,(char *)parmptr[1],_PARAM_SEED_SIZE);
   semilla[_PARAM_SEED_SIZE]=0;

   ret=CryptoStarixGenerarFicheroClaves(pathFicheroClaves,semilla);

   return ret;

}

NATFCT GETCLV NATARGDCL(nparm,parmptr,parmdec) 
{

   int ret;
   char pathFicheroClaves[_PARAM_PATH_SIZE+1];
   NATTYP_I2 index;
   char buffer[1024];
   int szbuffer;

   memmove(pathFicheroClaves, (char *)parmptr[0],_PARAM_PATH_SIZE);
   pathFicheroClaves[_PARAM_PATH_SIZE]=0;
   memmove((char *) &index,(char *)parmptr[1],sizeof(NATTYP_I2));

   ret=CryptoStarixObtenerClaveSEGU(pathFicheroClaves,index,buffer,sizeof(buffer));
   if (ret==0) {
      szbuffer=strlen(buffer);
      memmove((char*) parmptr[2],buffer,szbuffer);
      memset((char*) (parmptr[2]+szbuffer),' ',_PARAM_SEGU_SIZE-szbuffer);
   }

   return ret;

}

NATFCT DECRIPT NATARGDCL(nparm,parmptr,parmdec) 
{

   int ret;
   char pathFicheroClaves[_PARAM_PATH_SIZE+1];
   NATTYP_I2 index;
   char cryptedTDES[_PARAM_CRYPTDES_SIZE+1];
   char cryptedPAN[_PARAM_CRYPPAN_SIZE+1];
   char pan[17];

   memmove(pathFicheroClaves,(char *) parmptr[0],_PARAM_PATH_SIZE);
   pathFicheroClaves[_PARAM_PATH_SIZE]=0;
   memmove((char *) &index,(char *)parmptr[1],sizeof(NATTYP_I2));
   memmove(cryptedTDES,(char *) parmptr[2],_PARAM_CRYPTDES_SIZE);
   cryptedTDES[_PARAM_CRYPTDES_SIZE]=0;
   memmove(cryptedPAN,(char *) parmptr[3],_PARAM_CRYPPAN_SIZE);
   cryptedPAN[_PARAM_CRYPPAN_SIZE]=0;

   ret=CryptoStarixDesencriptarPAN(pathFicheroClaves,index,cryptedTDES,cryptedPAN,pan);
   if (ret==0) {
      memmove((char*) parmptr[4],pan,16);
   }

   return ret;

}



#ifdef __cplusplus
}
#endif
