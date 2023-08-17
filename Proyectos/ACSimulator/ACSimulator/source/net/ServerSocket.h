// =======================
//  FICHERO - ServerSocket.h
// =======================

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

// INCLUDES DEL PROJECTO
#include "Socket.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>
   *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
   *     ---------------------------------------
   *
   * FICHERO: ServerSocket.h
   *
   * DESCRIPCION:
   *   Conector (socket) servidor que permite establecer una conexion con un
   *   nodo cliente
   *
   * FECHA CREACION: 25-04-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Jorge Martinez
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |           |                                               |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class ServerSocket : public Socket
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
     /**
      * Numero maximo de conexiones que se atenderan
      */
      static const SDWordType NUM_MAX_CONEXIONES = 5;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC      /////////////////////
   public:
      // ===============   CICLO DE VIDA    ==================
      ServerSocket();

      // =================   OPERACIONES   ===================
     /**
      * <PRE>
      *   Habilita la cola asociada al conector para alojar peticiones de
      *   clientes que intantan establecer una conexion
      *
      * PARAMETROS:
      *   sdNumConexiones -> Numero maximo de conexiones permitidas. Por defecto
      *   toma el valor NUM_MAX_CONEXIONES
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *   OcupadoExcepcion, NoExisteExcepcion, NoAplicableExcepcion,
      *   DesconocidoExcepcion
      *
      * NOTAS:
      *
      * </PRE>
      */
      void habilitarConexiones( SDWordType sdNumConexiones = NUM_MAX_CONEXIONES)
         const;

     /**
      * <PRE>
      *   Lee una peticion de conexion de un cliente y retorna un socket preparado
      *   para la comunicacion con el cliente
      *
      * PARAMETROS:
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *   TimeoutExcepcion, NoExisteExcepcion, NoAplicableExcepcion,
      *   InterrupcionExcepcion, ComunicacionExcepcion, SinRecursosExcepcion,
      *   ParametroExcepcion, MemoriaExcepcion, DesconocidoExcepcion
      *
      * NOTAS:
      *
      * </PRE>
      */
      void aceptarConexion( Socket& socket) const;

   ////////////////////   PRIVATE      ////////////////////
   private:
      // =================   OPERACIONES   ===================
      /**
      * Retorna por referencia el nodo remoto (direccion y puerto ) al que
      * esta conectado el nodo actual
      * @param direccion Cadena donde se retorna la direccion del nodo remoto
      * @param uwPuerto UWordType donde se retorna el puerto del nodo remoto
      *
      * @exception
      *
      */
      void obtenerNodoRemoto( Cadena& direccion, UWordType& uwPuerto ) const;
};

#endif //SERVERSOCKET_H

// ===========================
//  FIN DEL FICHERO - ServerSocket.h
// ===========================
