// =======================
//  FICHERO - SOCKET.H
// =======================

#ifndef SOCKET_H
#define SOCKET_H

// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>


// INCLUDES DEL PROJECTO
#include "../lang/throwable/Excepcion.h"
#include "../lang/throwable/NoAplicaExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/SinRecursosExcepcion.h"
#include "../lang/throwable/PermisoExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"
#include "../lang/throwable/NoExisteExcepcion.h"
#include "../lang/throwable/OcupadoExcepcion.h"
#include "../lang/throwable/ParametroExcepcion.h"
#include "../lang/throwable/ComunicacionExcepcion.h"
#include "../lang/throwable/TimeoutExcepcion.h"
#include "../lang/throwable/InterrupcionExcepcion.h"
#include "../lang/throwable/RangoExcepcion.h"
#include "../lang/throwable/MemoriaExcepcion.h"
#include "../lang/throwable/IOExcepcion.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

  /**
   * <PRE>

   * FICHERO: SOCKET.H
   *
   * DESCRIPCION:
   *   Conector (socket) para la recepcion y el envio de datos a un nodo de red.
   *   Soporta los protocolos TCP (orientado a conexion) y UDP (no orientado a
   *   conexion.
   *   Si se utiliza el protocolo TCP, el socket servidor debe obtenerse
   *   invocando el metodo aceptarConexion de la clase ServerSocket.
   *
   * FECHA CREACION: 26-04-2005
   *
   * AUTOR DISENYO:
   *
   * PROGRAMADOR: Jorge Martinez
   *
   * MODIFICATIONES:
   *
   *    |   FECHA    |   AUTOR   |  RAZON                                   |
   *    ---------------------------------------------------------------------
   *    | DD-MM-YYYY |            |                                              |
   *    ---------------------------------------------------------------------
   *
   * </PRE>
   */
class Socket
{
   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
     /**
      * Tipo de protocolo en las comunicaciones de red
      */
      enum ProtocoloRedTypeE { NO_DEFINIDO = 0,
                               TCP         = SOCK_STREAM,
                               UDP         = SOCK_DGRAM };
      enum emode { MODE_READ      = SHUT_RD,
                   MODE_WRITE     = SHUT_WR,
                   MODE_READWRITE = SHUT_RDWR};

      /**
      * Numero maximo de conexiones que se atenderan
      */
      static const SDWordType NUM_MAX_CONEXIONES = 5;
   ////////////////////   PRIVATE      ////////////////////
   private:
     /**
      * Identificador del socket utilzado como descriptor en las lamadas a la API
      */
      SDWordType msdId;

     /**
      * Direccion del nodo remoto con el que se realizara la comunicacion
      */
      Cadena mDireccionRemota;

     /**
      * Nombre del nodo remoto con el que se realizara la comunicacion
      */
      Cadena mNombreRemoto;

     /**
      * Puerto del nodo remoto con el que se realizara la comunicacion
      */
      UWordType muwPuertoRemoto;

     /**
      * Protocolo utilizado en la comunicacion (actualmente TCP o UDP)
      */
      ProtocoloRedTypeE mProtocolo;

     /**
      * Direccion del nodo local asociado al socket
      */
      Cadena mDireccionLocal;

     /**
      * Puerto del nodo local asociado al socket por el que escucha
      */
      UWordType muwPuertoLocal;

      /**
       * Timeouts en milisegundos para el establecimiento de conexion y la
       * recepcion de datos
       */
       SDWordType msdTimeoutConectar;
       SDWordType msdTimeoutRecibir;

   // METODOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public:
     // ===============   CICLO DE VIDA    ==================
      Socket();
      virtual ~Socket();

      // =================   ACCESO       ====================
      void setId( SDWordType sdId);
      void setDireccionRemota( const Cadena& rDireccion );
      void setNombreRemoto( const Cadena& rNombre );
      void setNombreRemoto2( const Cadena& rNombre );
      void setPuertoRemoto( UWordType uwPuerto );
      void setProtocolo( ProtocoloRedTypeE protocolo );
      void setDireccionLocal( const Cadena& rDireccionLocal );
      void setPuertoLocal( UWordType uwPuertoLocal );
      void setTimeoutConectar( UDWordType udMilisegundos );
      void setTimeoutRecibir( UDWordType udMilisegundos );
      bool setKeepAlive(int timeidle, int timeintvl, int cnt = 3); //3 reintentos por defecto

      /// Elimina los timeouts
      void unsetTimeoutConectar();
      void unsetTimeoutRecibir();

      // =================   CONSULTA      ===================
      Cadena            getDireccionRemota() const;
      Cadena            getNombreRemoto() const;
      UWordType         getPuertoRemoto() const;
      ProtocoloRedTypeE getProtocolo() const;
      Cadena            getDireccionLocal() const;
      UWordType         getPuertoLocal() const;
      UDWordType        getTimeoutConectar() const;
      UDWordType        getTimeoutRecibir() const;
      bool              hasTimeoutConectar() const;
      bool              hasTimeoutRecibir() const;

       // =================   OPERACIONES   ===================
       /**
       * <PRE>
       *   Crea un conector con el protocolo, puerto y direccion de red actuales
       *
       * PARAMETROS:
       *
       * VALORES RETORNO:
       *
       * EXCEPCIONES:
       *   NoAplicaExcepcion, MemoriaExcepcion, SinRecursosExcepcion,
       *   PermisoExcepcion, DesconocidoExcepcion
       *
       * NOTAS:
       *
       * </PRE>
       */
       virtual void abrir();
       virtual void abrir2();

       /**
       * <PRE>
       *   Libera el socket
       *
       * PARAMETROS:
       *
       * VALORES RETORNO:
       *
       * EXCEPCIONES:
       *   NoExisteExcepcion, InterrupcionExcepcion, IOExcepcion,
       *   DesconocidoExcepcion
       *
       * NOTAS:
       *
       * </PRE>
       */
       virtual void cerrar();

       /**
       * <PRE>
       *   Desconecta el socket
       *
       * PARAMETROS:
       *
       * VALORES RETORNO:
       *
       * EXCEPCIONES:
       *   NoExisteExcepcion, ComunicacionExcepcion, DesconocidoExcepcion
       *
       * NOTAS:
       *
       * </PRE>
       */
       virtual void desconectar();

       /**
       * <PRE>
       *   Vincua el conector a una direccion de red y puerto locales
       *
       * PARAMETROS:
       *
       * VALORES RETORNO:
       *
       * EXCEPCIONES:
       *   NoExisteExcepcion, OcupadoExcepcion, PermisoExcepcion,
       *   ParametroExcepcion, DesconocidoExcepcion
       *
       * NOTAS:
       *
       * </PRE>
       */
       void nombrar() const;

       /**
       * <PRE>
       *   En protocolos orientado a conexion (TCP) solicita al servidor una
       *   conexion y espera confirmacion. En protocolos no orientados a conexion
       *   (UDP), solo especifica la direccion del nodo con el que se comunicara
       *
       * PARAMETROS:
       *
       * VALORES RETORNO:
       *
       * EXCEPCIONES:
       *   NoExisteExcepcion, ParametroExcepcion, OcupadoExcepcion,
       *   ComunicacionExcepcion, TimeoutExcepcion, SinRecursosExcepcion,
       *   NoAplicaExcepcion, PermisoExcepcion, DesconocidoExcepcion
       *
       * NOTAS:
       *   Si se invoca este metodo en un conector con protocolo UDP solo podra
       *   recibir datos desde el nodo actual
       *
       * </PRE>
       */
       virtual void conectar();

       /**
       * <PRE>
       *   Envia datos al nodo actual
       *
       * PARAMETROS:
       *   sbDatos -> Puntero a la zona de memoria donde residen los datos a enviar
       *   sdLongitud -> Longitud en bytes de los datos a enviar
       *
       * VALORES RETORNO:
       *  Numero de bytes que se han enviado
       *
       * EXCEPCIONES:
       *   TimeoutExcepcion, NoAplicaExcepcion, NoExisteExcepcion,
       *   ComunicacionExcepcion, ParametroExcepcion, InterrupcionExcepcion,
       *   RangoExcepcion, SinRecursosExcepcion, MemoriaExcepcion,
       *   DesconocidoExcepcion

       *
       * NOTAS:
       *
       * </PRE>
       */
       virtual SDWordType enviar( const SByteType* sbDatos, const SDWordType& sdLongitud )
         const;

       SDWordType enviar( const Cadena& datos ) const;

       /**
       * <PRE>
       *   Recupera los datos enviados desde otro nodo. Si el protocolo es UDP
       *   actualiza la direccion y el puerto del conector permitiendo responder
       *   al nodo que ha realizado el envio sin necesidad de llamar a los
       *   metodos setDireccionRemota() y setPuertoRemoto()
       *
       * PARAMETROS:
       *   sbDatos -> Puntero a la zona de memoria donde se alojaran los datos
       *     recibidos
       *   sdLongitudMax -> Capacidad maxima en bytes de la zona reservada para
       *   la recepcion de los datos
       *
       * VALORES RETORNO:
       *  Numero de bytes recibidos
       *
       * EXCEPCIONES:
       *   NoExisteExcepcion, ComunicacionExcepcion, TimeoutExcepcion,
       *   InterrrupcionExcepcion, ParametroExcepcion, MemoriaExcepcion,
       *   DesconocidoExcepcion
       *
       * NOTAS:
       *
       * </PRE>
       */
       virtual SDWordType recibir(SByteType* sbDatos, const SDWordType& sdLongitud );
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
      virtual void aceptarConexion( Socket& socket) const;
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

       /**
        * @return Cadena con el nombre host remooto correcpondiente al socket cliente
        */
       Cadena obtenerNombreRemoto(struct sockaddr_in &lAddrCliente) const;

       /**
        * @return Cadena con la direccion IP correcpondiente al nodo rNombre
        */
       static Cadena NombreToIP( const Cadena& rNombre );

       /**
        *apaga el socket en modo de solo lectura, solo escritura o lectura y escritura
        */
       void apagado(const enum emode modo);



   ////////////////////   PROTECTED    ////////////////////
   protected:
     // =================   ACCESO       ====================


   public:
     // =================   CONSULTA      ===================
     SDWordType getId() const;
     void resetAtributos();

   ////////////////////   PRIVATE      ////////////////////
   protected:
      // =================   ACCESO       ====================
     /**
      * <PRE>
      *   Establece en el conector la direccion y el puerto remotos extraidos de
      *   una estructura sockaddr_in
      *
      * PARAMETROS:
      *   addr -> Estructura con los datos a leer
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *
      * NOTAS:
      *
      * </PRE>
      */
      void setDireccionRemota( struct sockaddr_in& addr);
      void setFlags( SDWordType sdFlags );

      // =================   CONSULTA      ===================
      SDWordType getFlags() const;

      // =================   OPERACIONES   ===================
      void throwConnectExcepcion( SDWordType sdErrno ) const;
      void throwSelectExcepcion( SDWordType sdErrno ) const;
      void throwRecvExcepcion( SDWordType sdErrno ) const;
      void throwFcntlExcepcion( SDWordType sdErrno ) const;
      void throwGetsockoptExcepcion( SDWordType sdErrno ) const;

      timeval milisegundosToTimeval( UDWordType udMilisegundos ) const;

      /**
       * Fija el modo bloqueante o no bloqueante de las operaciones del socket
       */
      void setBloqueo( bool bFlagBloqueo );

     /**
      * <PRE>
      *   Convierte una direccion de nodo en el formato decimal estandar para
      *   el usuario (cuatro cifras menores que 256 separadas por puntos) al
      *   formato binario utilizado por la clase
      *
      * PARAMETROS:
      *   psbDireccion -> Direccion de un nodo en formato decimal estandar para
      *   el usuario (p.e.: 127.0.0.1)
      *
      * VALORES RETORNO:
      *   Direccion codificada en el formato binario utilizado internamente
      *
      * EXCEPCIONES:
      *   ParametroExcepcion
      *
      * NOTAS:
      *
      * </PRE>
      */
      UDWordType codificarDireccion( const Cadena& direccion ) const;
      static Cadena decodificarDireccion( UDWordType udDireccion );
public:
     /**
      * <PRE>
      *   Inicializa un struct sockaddr_in, utilizada en las llamadas a la API,
      *   con los valores actuales del protocolo y el nodo asociado al conector
      *   (direccion y puerto local)
      *
      * PARAMETROS:
      *   addr -> Estructura a llenar. Es un parametro de salida
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *    Las propias de 'codificarDireccion'
      *
      * NOTAS:
      *
      * </PRE>
      */
      void rellenarEstructuraConDireccionLocal( struct sockaddr_in& addr) const;

     /**
      * <PRE>
      *   Inicializa un struct sockaddr_in, utilizada en las llamadas a la API,
      *   con los valores actuales del protocolo y el nodo con el que se
      *   comunica el conector (direccion y puerto remoto).
      *
      * PARAMETROS:
      *   addr -> Estructura a llenar. Es un parametro de salida
      *
      * VALORES RETORNO:
      *
      * EXCEPCIONES:
      *    Las propias de 'codificarDireccion'
      *
      * NOTAS:
      *
      * </PRE>
      */
      void rellenarEstructuraConDireccionRemota( struct sockaddr_in& addr) const;
};

// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================

// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //SOCKET_H

// ===========================
//  FIN DEL FICHERO - SOCKET.H
// ===========================
