#ifndef _RSAClavePrivada    
#define _RSAClavePrivada


#include "RSAClavePublica.h"

/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: RSAClavePrivada.h
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


class RSAClavePrivada : public RSAClavePublica
{
   public:
   
      RSAClavePrivada();
      virtual ~RSAClavePrivada();

      void generar(int size,char *key=NULL);
      void inicializar(unsigned char *exp,int szexp,unsigned char *mod,int szmod,unsigned char *pexp,int szpexp);
      int desencriptar(const unsigned char *ciphertext,uint szciph,unsigned char *message);
      int getExponentePrivado(unsigned char *pexp);

      
};

#endif
