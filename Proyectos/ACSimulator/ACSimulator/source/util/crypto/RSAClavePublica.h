#ifndef _RSAClavePublica    
#define _RSAClavePublica

#include <string.h>

#include <openssl/rsa.h>
/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: RSAClavePublica.h
 *
 * DESCRIPCION:
 *
 *
 * FECHA CREACION: 12-07-2007
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR: Toni Bassas Serramia
 *
 * MODIFICATIONES:
 *
 *    |   FECHA    |   AUTOR   |  RAZON                                   |
 *    ---------------------------------------------------------------------
 *    | DD-MM-YYYY |           |                                          |
 *    ---------------------------------------------------------------------
 * </PRE>
*/


class RSAClavePublica
{
   public:
   
      RSAClavePublica();
      virtual ~RSAClavePublica();

      void inicializar(unsigned char *exp,int szexp,unsigned char *mod,int szmod);
      int encriptar(const unsigned char *message,uint szmsg,unsigned char *ciphertext);

      int getModulo(unsigned char *mod);
      int getExponentePublico(unsigned char *exp);

   protected:

      RSA   *rsa;

      void mirror(unsigned char *bin,int szbin);
};

#endif
