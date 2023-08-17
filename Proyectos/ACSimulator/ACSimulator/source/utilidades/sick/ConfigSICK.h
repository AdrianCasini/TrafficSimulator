//---------------------------------------------------------------------------
// ConfigSICK.h
//---------------------------------------------------------------------------
#ifndef CONFIGSICK_H
#define CONFIGSICK_H

#include "util/STrz.h"
#include "gestor/lang/Types.h"
#include "util/FicheroMemoria.h"
#include <unistd.h>
#include <map>
#include "SICK.h"

//---------------------------------------------------------------------------
/**
 * <PRE>
  *
  * FICHERO: ConfigSICK.h
  *
  * DESCRIPCION:  Gestion de los Valores de Configuracion
  *
  * FECHA CREACION: 24/01/2015
  *
  * AUTOR DISENYO: Adrian Casini
  *
  * PROGRAMADOR:   Adrian Casini
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |        |                   |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */
//---------------------------------------------------------------------------


class ConfigSICK
{

   public:

      ConfigSICK();
      virtual ~ConfigSICK();
      void inicializar();
      void mostrarInfo();

      Cadena getIP();
      Cadena getUser();
      Cadena getPasswd();
      UDWordType getTcpPort();
      Cadena getDirImagenes();
      Cadena getName();   
      UDWordType getHBInterval(); 
      UDWordType getSTInterval();
      bool getOBJSt(); 
      bool getOBJLoc();  
      bool getStatus(); 
      UDWordType getFormat();
      UDWordType getBinaryData(); 
      

      void seteoDefault();
      Cadena getValor(Cadena cadVarEntorno);

      typedef map<Cadena, Cadena> MapParametros;
      MapParametros  mMapParametros;


};


#endif // CONFIGSICK_H
