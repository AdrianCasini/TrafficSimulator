#ifndef _TRIPLE_DES    
#define _TRIPLE_DES

#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <openssl/des.h>

/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: TripleDES.h
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

#define TripleDES_KEY_SIZE     8

typedef unsigned char byte;

class TripleDES
{
   public:
   
      TripleDES();
      virtual ~TripleDES();

   protected:

      void setClaves(int numclaves,unsigned char *claves);
      void getClaves(int numclaves,unsigned char *buffer);
      int  generarClaves(int numclaves);

      unsigned char keys[TripleDES_KEY_SIZE*3];

      void createKey(unsigned char *key,int szkey);

      bool EDE(byte *k1,byte *k2,byte *k3,int key_size,byte *data,int szdata,byte *result);
      bool DED(byte *k1,byte *k2,byte *k3,int key_size,byte *data,int szdata,byte *result);

};

#endif
