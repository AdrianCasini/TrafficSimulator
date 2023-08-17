
//---------------------------------------------------------------------------
// GestorSICK.h
//---------------------------------------------------------------------------
#ifndef GESTORSICK_H
#define GESTORSICK_H

//#include "lang/throwable/TimeoutExcepcion.h"

//#include "util/Timer.h"


//#include "net/ServerSocket.h"
//#include "net/Socket.h"
#include "../../lang/Cadena.h"



//------------------------------------------------------------------------------
/**
 * <PRE>
  *
  * FICHERO: GestorSICK.h
  *
  * DESCRIPCION:  
  *
  * FECHA CREACION: 20/01/2015
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
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class GestorSICK 
{
   
   public:
      GestorSICK();
      virtual ~GestorSICK();

      static const int SK_BUFFER_MAX = 65535; 
      //ThreadMetodo<GestorSICK> thConectar;
      //ThreadMetodo<GestorSICK> thLeer;

      Cadena            mcadIP;
      UDWordType        mudwTcpport;
      bool              mbConnected;


      //ConfigSICK        mConfig;
      //Socket            mSocket;     
      //BaseDatos         DB;
      //CfgBaseDatos      DBcfg;

      //--main functions
      void inicializar();
      void conectar();
      void leer();
      void finalizar();
      void leerConfiguracion();
      void seteoDefault();

      //--Tratamiento Mensajes
      void  parse_informacion(char * ptr);
      void  parse_error(char * ptr);
      void  parse_vehicle_result(char * ptr);
      void  parse_vehicle_ext_result(char * ptr);
      void  parse_status(char * ptr);
      void  parse_object_start(char * ptr);
      void  parse_object_stop(char * ptr);
      void  parse_object_location(char * ptr);
      void  parse_heart_beat(char * ptr);

      //--Envio--Mensajes--
      void enviando_configuracion();
      void enviando_inicializacion();

      //--Generales soporte
      Cadena  getTimeStamp( unsigned long long * ptr );
      bool    DB_consulta(Cadena cadSQL);
      bool    DB_conectar();



};

#endif




