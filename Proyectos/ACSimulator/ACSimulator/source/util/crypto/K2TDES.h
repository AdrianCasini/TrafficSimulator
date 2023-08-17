#ifndef _K2TDES    
#define _K2TDES

#include "TripleDES.h"

/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: K2TDES.h
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


class K2TDES : public TripleDES
{
   public:
   
      K2TDES();
      virtual ~K2TDES();

      void setClaves(unsigned char *claves);
      void getClaves(unsigned char *buffer);
      int  generarClaves(void);

      bool encriptar(byte *data,int szdata,byte *result);
      bool desencriptar(byte *data,int szdata,byte *result);
      
};

#endif
