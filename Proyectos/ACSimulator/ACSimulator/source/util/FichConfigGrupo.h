
#ifndef _FICH_CONFIG_GRUPO_H_
#define _FICH_CONFIG_GRUPO_H_

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO:   FichConfigGrupo.h
  *
  * DESCRIPCION:
  *
  *  Tratamiento de los ficheros de configuracion (.cfg) guia de estilos de
  *  codificacion.
  *
  * FECHA CREACION: 14-03-2006
  *
  * $Id: FichConfigGrupo.h,v 1.3 2006/03/16 09:17:50 frocher Exp $
  *
  * AUTOR DISENO:
  *
  * PROGRAMADOR: Francesc Rocher
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |                |                                                             |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */

#include "FichConfig.h"

class FichConfigGrupo : public FichConfig
{
      /**
       *   La clase \t FichConfigGrupo permite la existencia de diversos
       *   grupos bajo un mismo fichero de configuracion. Cada grupo puede
       *   contener identificadores repetidos. Esto es util para situaciones
       *   en las que se deben repetir los mismos parametros de configuracion
       *   (o muy similares) para conceptos ligeramente distintos. Por
       *   ejemplo, en el fichero de configuracion del gestor de cortinas
       *   (motivo por el que se crea esta clase), pueden aparecer hasta tres
       *   cortinas configuradas, cada una de ellas con parametros, si no
       *   iguales, muy parecidos.
       *
       *   El acceso a este tipo de ficheros de configuracion es muy
       *   basico. Solo se proveen los metodos \t getPrimero, para acceder al
       *   primer parametro de configuracion del fichero, y \t getSiguiente
       *   para acceder secuencialmente al resto de parametros. Ver mas
       *   abajo para mas detalles.
       *
       *   Este tipo de ficheros de configuracion esta pensado para que cada
       *   parametro tenga solo un valor. Su estructura recuerda mucho a los
       *   ficheros \t '.ini' de \t windows. Por ejemplo,
       *
       *      # Cortina 1, configuracion del puerto serie
       *      #
       *      CORTINA;     1
       *      DISPOSITIVO; /dev/ttyS0
       *      VELOCIDAD;   9600
       *      PARIDAD;     no
       *      BITS_DATOS;  8
       *      BITS_STOP;   1
       *
       *      # Cortina 2, configuracion del puerto serie
       *      #
       *      CORTINA;     2
       *      DISPOSITIVO; /dev/ttyS1
       *      VELOCIDAD;   9600
       *      PARIDAD;     par
       *      BITS_DATOS;  7
       *      BITS_STOP;   0
       */


   public:

      explicit  FichConfigGrupo( void );
      virtual  ~FichConfigGrupo( void );

      /**
       *   Accede al primer parametro de configuracion del fichero. Devuelve
       *   \t true si se ha encontrado, y entonces instancia con el valor
       *   leido los argumentos \t parametro y \t valor.
       *
       */
      bool getPrimero( Cadena &parametro, Cadena &valor );

      /**
       *   Accede al siguiente parametro de configuracion del fichero, a
       *   partir del ultimo acceso realizado. Devuelve \t true si se ha
       *   encontrado, y entonces instancia con el valor leido los argumentos
       *   \t parametro y \t valor.
       */
      bool getSiguiente( Cadena &parametro, Cadena &valor );

      /**
       *   Devuelve el numero de linea en la que se ha encontrado el ultimo
       *   parametro leido.
       */
      inline int getLinea( void ) const
      {
         return( _linea );
      }


   private:

      int  _linea;
      bool _primero;

      bool leer( Cadena &parametro, Cadena &valor );
};

#endif // _FICH_CONFIG_GRUPO_H_
