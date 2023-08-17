// =======================
//  FICHERO - SOCKET.CPP
// =======================

#ifdef _DEBUG
   #include "util/STrz.h"
#endif

#include "Socket.h"
#include <signal.h>
#define MIL       ( ( SDWordType )1E3 )
#define UN_MILLON ( ( SDWordType )1E6 )

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////
// ===============   CICLO DE VIDA    ==================
Socket::Socket()
{
   struct sigaction a;

   resetAtributos();

   a.sa_handler = SIG_IGN;
   sigaction (SIGPIPE, &a, NULL);
};

Socket::~Socket()
{

};

////////////////////   PROTECTED    ////////////////////
// =================   ACCESO       ====================
void Socket::setId( SDWordType sdId )
{
   msdId = sdId;
};

//------------------------------------------------------------------------------
void Socket::setFlags( SDWordType sdFlags )
{
   if ( FAIL == fcntl( msdId, F_SETFL, sdFlags ) )
      throwFcntlExcepcion( errno );
};

// =================   CONSULTA      ===================
SDWordType Socket::getFlags() const
{
   SDWordType lsdFlags;

   // Obtencion del estado y modo actual del socket
   lsdFlags = fcntl( msdId, F_GETFL, NULL );
   if ( FAIL == lsdFlags )
      throwFcntlExcepcion( errno );
   return lsdFlags;
};

//-----------------------------------------------------------------------------
void Socket::setDireccionRemota( const Cadena& rDireccion )
{
   mDireccionRemota = rDireccion;
};

//-----------------------------------------------------------------------------
void Socket::setNombreRemoto( const Cadena& rNombre )
{
   setDireccionRemota( NombreToIP( rNombre ) );
};

//-----------------------------------------------------------------------------
void Socket::setNombreRemoto2( const Cadena& rNombre )
{
   mNombreRemoto=rNombre;
};

//-----------------------------------------------------------------------------
void Socket::setPuertoRemoto( UWordType uwPuerto )
{
   muwPuertoRemoto = uwPuerto;
};

//-----------------------------------------------------------------------------
void Socket::setProtocolo( ProtocoloRedTypeE protocolo )
{
   mProtocolo = protocolo;
};

//-----------------------------------------------------------------------------
void Socket::setDireccionLocal( const Cadena& rDireccionLocal )
{
   mDireccionLocal = rDireccionLocal;
};

//-----------------------------------------------------------------------------
void Socket::setPuertoLocal( UWordType uwPuertoLocal )
{
   muwPuertoLocal = uwPuertoLocal;
};

//-----------------------------------------------------------------------------
void Socket::setTimeoutConectar( UDWordType udMilisegundos )
{
   msdTimeoutConectar = udMilisegundos;
}
//-----------------------------------------------------------------------------
void Socket::setTimeoutRecibir( UDWordType udMilisegundos )
{
   msdTimeoutRecibir = udMilisegundos;
};

//-----------------------------------------------------------------------------
void Socket::unsetTimeoutConectar()
{
   msdTimeoutConectar = FAIL;
};

//-----------------------------------------------------------------------------
void Socket::unsetTimeoutRecibir()
{
   msdTimeoutRecibir = FAIL;
};
//-----------------------------------------------------------------------------
void Socket::resetAtributos()
{
   setId( -1 );
   setDireccionLocal( "" );
   setPuertoLocal( 0 );
   setDireccionRemota( "" );
   setPuertoRemoto( 0 );
   setProtocolo( NO_DEFINIDO );
   setNombreRemoto2( "" );
   unsetTimeoutConectar();
   unsetTimeoutRecibir();
};

// =================   CONSULTA      ===================
SDWordType Socket::getId() const
{
   return msdId;
};

//-----------------------------------------------------------------------------
Cadena Socket::getDireccionRemota() const
{
   return mDireccionRemota;
};

//-----------------------------------------------------------------------------
Cadena Socket::getNombreRemoto() const
{
   return mNombreRemoto;
};

//-----------------------------------------------------------------------------
UWordType Socket::getPuertoRemoto() const
{
   return muwPuertoRemoto;
};

//-----------------------------------------------------------------------------
Socket::ProtocoloRedTypeE Socket::getProtocolo() const
{
   return mProtocolo;
};

//-----------------------------------------------------------------------------
Cadena Socket::getDireccionLocal() const
{
   return mDireccionLocal;
};

//-----------------------------------------------------------------------------
UWordType Socket::getPuertoLocal() const
{
   return muwPuertoLocal;
};

//------------------------------------------------------------------------------
UDWordType Socket::getTimeoutConectar() const
{
   return msdTimeoutConectar;
};

//------------------------------------------------------------------------------
UDWordType Socket::getTimeoutRecibir() const
{
   return msdTimeoutRecibir;
};

//------------------------------------------------------------------------------
bool Socket::setKeepAlive(int timeidle, int timeintvl, int cnt)
{
   int iOptVal, ret;

   //Tiempo antes de empezar con los keepAlives en segundos
   iOptVal = timeidle;
   ret = setsockopt(msdId, SOL_TCP, TCP_KEEPIDLE, &iOptVal, sizeof(iOptVal));
   if (ret == -1)
   {
     switch (errno)
     {
        case EBADF:
          throw NoExisteExcepcion( __AQUI__, "sockfd is not a valid descriptor.",
                                errno);
        case ENOTSOCK:
          throw ParametroExcepcion( __AQUI__, "Argument is a descriptor for" \
                                 " a file, not a socket.", errno );
        case EFAULT:             // Estos 3 valores estan asegurados por
        case EINVAL:             // codigo que no se produciran
        case ENOPROTOOPT:
        default:
          throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
     }
   }

   //Tiempo entre envios de keepAlives en segundos
   iOptVal = timeintvl;
   ret = setsockopt(msdId, SOL_TCP, TCP_KEEPINTVL, &iOptVal, sizeof(iOptVal));
   if (ret == -1)
   {
     switch (errno)
     {
        case EBADF:
          throw NoExisteExcepcion( __AQUI__, "sockfd is not a valid descriptor.",
                                errno);
        case ENOTSOCK:
          throw ParametroExcepcion( __AQUI__, "Argument is a descriptor for" \
                                 " a file, not a socket.", errno );
        case EFAULT:             // Estos 3 valores estan asegurados por
        case EINVAL:             // codigo que no se produciran
        case ENOPROTOOPT:
        default:
          throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
     }
   }

   //Numero de reintentos antes de cerrar el socket
   iOptVal = cnt;
   ret = setsockopt(msdId, SOL_TCP, TCP_KEEPCNT, &iOptVal, sizeof(iOptVal));
   if (ret == -1)
   {
     switch (errno)
     {
        case EBADF:
          throw NoExisteExcepcion( __AQUI__, "sockfd is not a valid descriptor.",
                                errno);
        case ENOTSOCK:
          throw ParametroExcepcion( __AQUI__, "Argument is a descriptor for" \
                                 " a file, not a socket.", errno );
        case EFAULT:             // Estos 3 valores estan asegurados por
        case EINVAL:             // codigo que no se produciran
        case ENOPROTOOPT:
        default:
          throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
     }
   }

   //Activa el modo de keepAlives
   iOptVal = 1;
   ret = setsockopt(msdId, SOL_SOCKET, SO_KEEPALIVE, &iOptVal, sizeof(iOptVal));
   if (ret == -1)
   {
     switch (errno)
     {
        case EBADF:
          throw NoExisteExcepcion( __AQUI__, "sockfd is not a valid descriptor.",
                                errno);
        case ENOTSOCK:
          throw ParametroExcepcion( __AQUI__, "Argument is a descriptor for" \
                                 " a file, not a socket.", errno );
        case EFAULT:             // Estos 3 valores estan asegurados por
        case EINVAL:             // codigo que no se produciran
        case ENOPROTOOPT:
        default:
          throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
     }
   }

   return (ret == 0);
}


//------------------------------------------------------------------------------
bool Socket::hasTimeoutConectar() const
{
   if ( FAIL == msdTimeoutConectar )
      return false;
   else
      return true;
};

//------------------------------------------------------------------------------
bool Socket::hasTimeoutRecibir() const
{
   if ( FAIL == msdTimeoutRecibir )
      return false;
   else
      return true;
};

// =================   OPERACIONES    ===================
void Socket::abrir()
{
   SDWordType lsdId;

   lsdId = socket( AF_INET, getProtocolo(), 0 );
   if ( FAIL == lsdId )
   {
      switch ( errno )
      {
         case EPROTONOSUPPORT:
            throw NoAplicaExcepcion( __AQUI__, "The  protocol type or the "\
            " specified protocol is not supported within this domain.", errno );
         case EAFNOSUPPORT:
            throw NoAplicaExcepcion( __AQUI__, "The implementation does not"\
            " support the specified address family.", errno );
         case EINVAL:
            throw NoAplicaExcepcion( __AQUI__, "Unknown protocol, or protocol"\
            " family not available.", errno );
         case ENFILE:
            throw MemoriaExcepcion( __AQUI__, "Not  enough  kernel memory to"\
            " allocate a new socketstructure.", errno );
         case ENOBUFS:
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "Insufficient memory is"\
            " available. The socket cannot be created until sufficient"\
            " resources are freed.", errno );
         case EMFILE:
            throw SinRecursosExcepcion( __AQUI__, "Process file table overflow.",
            errno );
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "Permission to create a socket"\
            " of the specified type and/or protocol is denied.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // el conector se abrio correctamente
   {
      setId( lsdId );
#ifdef _DEBUG
      STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Socket abierto: fd=%d", lsdId );
#endif
   }
};

void Socket::abrir2()
{
   SDWordType lsdId;

   lsdId = socket( PF_INET, getProtocolo(), 0 );
   if ( FAIL == lsdId )
   {
      switch ( errno )
      {
         case EPROTONOSUPPORT:
            throw NoAplicaExcepcion( __AQUI__, "The  protocol type or the "\
            " specified protocol is not supported within this domain.", errno );
         case EAFNOSUPPORT:
            throw NoAplicaExcepcion( __AQUI__, "The implementation does not"\
            " support the specified address family.", errno );
         case EINVAL:
            throw NoAplicaExcepcion( __AQUI__, "Unknown protocol, or protocol"\
            " family not available.", errno );
         case ENFILE:
            throw MemoriaExcepcion( __AQUI__, "Not  enough  kernel memory to"\
            " allocate a new socketstructure.", errno );
         case ENOBUFS:
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "Insufficient memory is"\
            " available. The socket cannot be created until sufficient"\
            " resources are freed.", errno );
         case EMFILE:
            throw SinRecursosExcepcion( __AQUI__, "Process file table overflow.",
            errno );
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "Permission to create a socket"\
            " of the specified type and/or protocol is denied.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // el conector se abrio correctamente
   {
      setId( lsdId );
#ifdef _DEBUG
      STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Socket abierto: fd=%d", lsdId );
#endif
   }
};

//-----------------------------------------------------------------------------
void Socket::cerrar()
{
   if ( -1 != msdId ) // si ya esta cerrado no se hace nada
   {
      if ( FAIL == close( msdId ) )
      {
         switch ( errno )
         {
            case EBADF:
               throw NoExisteExcepcion( __AQUI__, "fd isn't a valid open file"\
               " descriptor.", errno );
            case EINTR:
               throw InterrupcionExcepcion( __AQUI__, "The close() call was"\
               " interrupted by a signal.", errno );
            case EIO:
               throw IOExcepcion( __AQUI__, "An I/O error occurred.", errno );
            default:
               throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
         }
      }
      else // el conector se cerro correctamente
      {
#ifdef _DEBUG
         STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Socket cerrado: fd=%d", msdId );
#endif
         setId( -1 );
      }
   }
};

//-----------------------------------------------------------------------------
void Socket::desconectar()
{
   if ( -1 != msdId ) // si ya esta cerrado no se hace nada
   {
      if ( FAIL == shutdown( msdId, SHUT_RDWR ) )
      {
         switch ( errno )
         {
            case ENOTCONN:
               throw ComunicacionExcepcion( __AQUI__, "The socket is not"\
               " connected, and no target has been given.", errno );
            case ENOTSOCK:
               throw NoExisteExcepcion( __AQUI__, "The argument s is not a socket.",
               errno );
            default:
               throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
         }
      }
      else // el conector se desconecto correctamente
      {
#ifdef _DEBUG
         STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Socket desconectado: fd=%d", msdId );
#endif
      }
   }
};

//-----------------------------------------------------------------------------
void Socket::nombrar() const
{
   struct sockaddr_in lAddrLocal;

   rellenarEstructuraConDireccionLocal( lAddrLocal );
   if ( FAIL == bind( msdId, (struct sockaddr* ) &lAddrLocal,
                      sizeof( struct sockaddr ) ) )
   {
      switch ( errno )
      {
         case EBADF:
            throw NoExisteExcepcion( __AQUI__, "sockfd is not a valid descriptor.",
            errno );
         case EINVAL:
            throw OcupadoExcepcion( __AQUI__, "The socket is already bound to"\
            " an address.", errno );
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The address is protected, and"\
            " the user is not the super-user.", errno );
         case ENOTSOCK:
            throw ParametroExcepcion( __AQUI__, "Argument is a descriptor for"\
            " a file, not a socket.", errno );
         case EADDRINUSE: /* Address already in use */
            throw OcupadoExcepcion( __AQUI__, strerror( errno ), errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // el conector se ha vinculado al nodo de red correctamente
};

//-----------------------------------------------------------------------------
void Socket::conectar()
{
   struct timeval     lTimeout;
   struct timeval*    lpTimeout;
   struct sockaddr_in lAddr;
   fd_set             lWritefds;
   SDWordType         lsdOptVal;
   socklen_t          lOptLen;
   SDWordType         lswResultado;
   SDWordType         lsdFlags; // estado y modo del socket

   rellenarEstructuraConDireccionRemota( lAddr );
   lsdFlags = getFlags(); // guardamos el estado del bloqueo antes de la conexion
   setBloqueo( false );
   lswResultado = connect( msdId, ( sockaddr* ) &lAddr, sizeof( lAddr ) );
   setFlags( lsdFlags ); // restauramos el estado del bloqueo
   if ( FAIL != lswResultado )
      return; // conexion establecida
   else
   {
      if ( EINPROGRESS != errno )
         throwConnectExcepcion( errno );
      else // intentando establecer conexion
      {
         FD_ZERO( &lWritefds );
         FD_SET( msdId, &lWritefds );
         // Timeout
         lpTimeout = &lTimeout;
         if ( true == hasTimeoutConectar() )
            lTimeout = milisegundosToTimeval( getTimeoutConectar() );
         else
            lpTimeout = NULL;

         // Esperamos hasta que se establezca la conexion
         lswResultado = select( msdId + 1, NULL,  &lWritefds, NULL, lpTimeout );
         if ( FAIL == lswResultado )
            throwSelectExcepcion( errno );
         else if ( 0 == lswResultado )
         {
            throw TimeoutExcepcion( __AQUI__, "Se ha superado el tiempo maximo"\
            " de establecimiento de conexion" );
         }
         else
         {
            // Comprobamos si la conexion se ha establecido correctamente
            lOptLen = sizeof( lsdOptVal );
            if ( FAIL == getsockopt( msdId, SOL_SOCKET, SO_ERROR, &lsdOptVal, &lOptLen ) )
               throwGetsockoptExcepcion( errno );
            else
            {
               if ( 0 == lsdOptVal )
                  return;  // conexion establecida
               else
                  throwConnectExcepcion( lsdOptVal );
            }
         }
      }
   }
};

//-----------------------------------------------------------------------------
SDWordType Socket::enviar( const SByteType* sbDatos,
   const SDWordType& sdLongitud ) const
{
   SDWordType          lsdNumBytesEnviados;
   struct sockaddr_in  lAddr;

   lsdNumBytesEnviados = FAIL;
   if ( TCP == getProtocolo() ) // envio orientado a conexion (TCP)
      lsdNumBytesEnviados = send( msdId, sbDatos, sdLongitud, 0 );
   else if ( UDP == getProtocolo() )  // envio no orientado a conexion (UDP)
   {
      rellenarEstructuraConDireccionRemota( lAddr );
      lsdNumBytesEnviados = sendto( msdId, sbDatos, sdLongitud, 0,
                                   ( struct sockaddr* ) &lAddr,
                                    sizeof( struct sockaddr ) );
   }
   if ( FAIL == lsdNumBytesEnviados)
   {
      switch ( errno )
      {
         case EAGAIN: // o EWOULDBLOCK
            throw TimeoutExcepcion( __AQUI__, "The socket is marked"\
            " non-blocking and the requested operation would block.", errno );
         case EISCONN:
            throw NoAplicaExcepcion( __AQUI__, "The connection-mode socket was"\
            " connected already but a recipient was specified.", errno );
         case EBADF:
            throw NoExisteExcepcion( __AQUI__, "An invalid descriptor was"\
            " specified.", errno );
         case ENOTSOCK:
            throw NoExisteExcepcion( __AQUI__, "The argument s is not a socket.",
            errno );
         case ECONNRESET:
            throw ComunicacionExcepcion( __AQUI__, "Connection reset by peer.",
            errno );
         case ENOTCONN:
            throw ComunicacionExcepcion( __AQUI__, "The socket is not"\
            " connected, and no target has been given.", errno );
         case EPIPE:
            return 0;
         case EDESTADDRREQ:
            throw ParametroExcepcion( __AQUI__, "The socket is not"\
            " connection-mode, and no peer address is set.", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "An invalid user space address"\
            " was specified for a parameter.", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "Invalid argument passed.",
            errno );
         case EOPNOTSUPP:
            throw ParametroExcepcion( __AQUI__, "Some bit in the flags"\
            " argument is inappropriate for the socket type.", errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "A signal occurred before"\
            " any data was transmitted.", errno );
         case EMSGSIZE:
            throw RangoExcepcion( __AQUI__, "The socket type requires that"\
            " message be sent atomically, and the size of the message to be"\
            " sent made this impossible.", errno );
         case ENOBUFS:
            throw SinRecursosExcepcion( __AQUI__, "The output queue for a"\
            " network interface was full.", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "No memory available.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // los datos han sido enviados en su totalidad o parcialmente
      return lsdNumBytesEnviados;
};

//-----------------------------------------------------------------------------
SDWordType Socket::enviar( const Cadena& rDatos ) const
{
   return enviar( rDatos.getCadena(), rDatos.getLong() );
};

//-----------------------------------------------------------------------------
SDWordType Socket::recibir( SByteType* sbDatos,
   const SDWordType& sdLongitudMax )
{
   SDWordType         lsdNumBytesRecibidos;
   SDWordType         lsdLonAddr;
   struct sockaddr_in lAddr;
   struct timeval     lTimeout;
   struct timeval*    lpTimeout;
   fd_set             lReadfds; // socket de lectura
   fd_set             lErrorfds; // errores
   SDWordType         lswResultadoSelect;

   if ( msdId < 0 )
      throw NoExisteExcepcion( __AQUI__, "El socket no esta abierto" );

   FD_ZERO( &lReadfds );
   FD_SET( msdId, &lReadfds );
   FD_ZERO( &lErrorfds );
   FD_SET( msdId, &lErrorfds );

   // Timeout
   if ( true == hasTimeoutRecibir() )
   {
      lTimeout = milisegundosToTimeval( getTimeoutRecibir() );
      lpTimeout = &lTimeout;
   }
   else
      lpTimeout = NULL;
   // Esperamos que el socket reciba algun dato
   lswResultadoSelect = select( msdId + 1, &lReadfds, NULL, &lErrorfds, lpTimeout );
   if ( FAIL == lswResultadoSelect )
      throwSelectExcepcion( errno );
   if ( 0 == lswResultadoSelect )
   {
      throw TimeoutExcepcion( __AQUI__, "Se ha superado el tiempo maximo para"\
      " recibir datos por el socket" );
   }
   if ( FD_ISSET( msdId, &lErrorfds ) )
   {
      throw ComunicacionExcepcion( __AQUI__, "Se ha producido un error en la conexion" );
   }

   // Lectura de los datos recibidos
   lsdLonAddr = sizeof(struct sockaddr_in);
   if ( TCP == getProtocolo() ) // envio orientado a conexion (TCP)
      lsdNumBytesRecibidos = recv( msdId, sbDatos, sdLongitudMax, 0 );
   else if ( UDP == getProtocolo() )  // envio no orientado a conexion (UDP)
   {
       lsdNumBytesRecibidos = recvfrom( msdId, sbDatos, sdLongitudMax, 0,
         (struct sockaddr*) &lAddr, (socklen_t*) &lsdLonAddr );
   }
   else
      throw NoAplicaExcepcion(__AQUI__,"Protocolo incorrecto");

   if ( 0 == lsdNumBytesRecibidos)
      throw ComunicacionExcepcion( __AQUI__, "Se ha perdido la conexion" );
   if ( FAIL == lsdNumBytesRecibidos)
      throwRecvExcepcion( errno );

   // Los datos han sido recibidos en su totalidad o parcialmente
   if ( UDP == getProtocolo() ) // recuperamos la direccion del nodo emisor
      setDireccionRemota( lAddr );
   return lsdNumBytesRecibidos;
};
//------------------------------------------------------------------------------
void Socket::habilitarConexiones( SDWordType sdNumConexiones) const
{
   if ( FAIL == listen( getId(), sdNumConexiones ) )
   {
      switch ( errno )
      {
         case EADDRINUSE:
            throw OcupadoExcepcion( __AQUI__, "Another socket is already"\
            " listening on the same port.", errno );
         case EBADF:
            throw NoExisteExcepcion( __AQUI__, "The argument s is not a valid"\
            " descriptor.", errno );
         case ENOTSOCK:
            throw NoExisteExcepcion( __AQUI__, "The argument s is not a socket.",
            errno );
         case EOPNOTSUPP:
            throw NoAplicaExcepcion( __AQUI__, "The socket is not of a type"\
            " that supports the listen operation.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // la cola de peticiones de conexion se ha habilitado correctamente
};

//-----------------------------------------------------------------------------
void Socket::aceptarConexion( Socket& socket) const
{
   SDWordType         lsdIdCliente;
   struct sockaddr_in lAddrCliente;
   SDWordType         lsdLonAddrCliente, s;
   Cadena             lDireccion;
   UWordType          luwPuerto;
   fd_set             set;

   lsdLonAddrCliente = sizeof (struct sockaddr_in);

   do
   {
      FD_ZERO(&set);
      FD_SET(getId (), &set);
      s = select (getId () + 1, &set, NULL, NULL, NULL);
      if (s > 0)
      {
         lsdIdCliente = accept (getId (), (struct sockaddr*) &lAddrCliente,
            (socklen_t*) &lsdLonAddrCliente);
      }
   } while (s <= 0);

   if ( FAIL == lsdIdCliente)
   {
      switch ( errno )
      {
         case EAGAIN: // equivalente a EWOULDBLOCK
            throw TimeoutExcepcion( __AQUI__, "The socket is marked "\
            " non-blocking and no connections are present to be accepted.",
            errno );
         case EBADF:
            throw NoExisteExcepcion( __AQUI__, "The descriptor is invalid.",
            errno );
         case ENOTSOCK:
            throw NoExisteExcepcion( __AQUI__, "The descriptor references a"\
            " file, not a socket.", errno );
         case EOPNOTSUPP:
            throw NoAplicaExcepcion( __AQUI__, "The referenced socket is not"\
            " of type SOCK_STREAM.", errno );
         case EINVAL:
            throw NoAplicaExcepcion( __AQUI__, "Socket is not listening for"\
            " connections.", errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "The system call was"\
            " interrupted by a signal that was caught before a valid"\
            " connection arrived.", errno );
         case ECONNABORTED:
            throw ComunicacionExcepcion( __AQUI__, "A connection has been"\
            " aborted.", errno );
         case EPROTO:
            throw ComunicacionExcepcion( __AQUI__, "Protocol error.", errno );
         case EPERM:
            throw ComunicacionExcepcion( __AQUI__, "Firewall rules forbid "\
            " connection.", errno );
         case EMFILE:
            throw SinRecursosExcepcion( __AQUI__, "The per-process limit of"\
            " open file descriptors has been reached.", errno );
         case ENFILE:
            throw SinRecursosExcepcion( __AQUI__, "The system maximum for file"\
            " descriptors has been reached.", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "The addr parameter is not in"\
            " a writable part of the user address space.", errno );
         case ENOBUFS:
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "Not enough free socket buffer"\
            " memory.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // se ha establecido una conexion con un cliente
   {
      socket.setId( lsdIdCliente );
      socket.setProtocolo( getProtocolo() );
      // Nodo remoto al que se ha conectado
      socket.obtenerNodoRemoto( lDireccion, luwPuerto );
      socket.setDireccionRemota( lDireccion );
      socket.setPuertoRemoto( luwPuerto );
   }
};
//------------------------------------------------------------------------------
Cadena Socket::obtenerNombreRemoto(struct sockaddr_in &lAddrCliente) const{
   struct hostent *host;
   Cadena hostremoto("");
   host=gethostbyaddr((char*)&lAddrCliente.sin_addr,sizeof(lAddrCliente.sin_addr),lAddrCliente.sin_family);
   if(host){
      Cadena aux(host->h_name);
      hostremoto=aux;
   }
   return hostremoto;
}
//------------------------------------------------------------------------------
void Socket::obtenerNodoRemoto( Cadena& direccion, UWordType& uwPuerto ) const
{
   struct sockaddr     lNodo;
   struct sockaddr_in* lpAddr;
   socklen_t           lLongitudNodo;

   lLongitudNodo = sizeof( lNodo );
   if ( FAIL == getpeername( msdId,  &lNodo, &lLongitudNodo ) )
   {
      switch( errno )
      {
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "The argument s is not a valid"\
         " descriptor.", errno );
      case ENOTSOCK:
         throw ParametroExcepcion( __AQUI__, "The argument s is a file, not a"\
         " socket.", errno );
      case EFAULT:
         throw ParametroExcepcion( __AQUI__, "The  name parameter points to"\
         " memory not in a valid part of the process address space.", errno );
      case ENOTCONN:
         throw ComunicacionExcepcion( __AQUI__, "The socket is not connected",
         errno );
      case ENOBUFS:
         throw MemoriaExcepcion( __AQUI__, "Insufficient resources were"\
         " available in the system to perform the operation.", errno );
      default:
         throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else
   {
      lpAddr = ( sockaddr_in* )&lNodo;
      direccion = inet_ntoa( lpAddr->sin_addr );
      uwPuerto = ntohs( lpAddr->sin_port );
   }
};
//------------------------------------------------------------------------------
Cadena Socket::NombreToIP( const Cadena& rNombre )
{
   struct hostent* lpHostent;

   lpHostent = gethostbyname( rNombre.getCadena() );
   if ( NULL == lpHostent )
   {
      switch( h_errno )
      {
         case HOST_NOT_FOUND:
            throw NoExisteExcepcion( __AQUI__, "The specified host is unknown" );
         break;
         case NO_ADDRESS:
            throw NoExisteExcepcion( __AQUI__, "The requested name is valid"\
            " but does not have an IP address" );
         break;
         case NO_RECOVERY:
            throw DesconocidoExcepcion( __AQUI__, "A non-recoverable name"\
            " server error occurred" );
         break;
         case TRY_AGAIN:
            throw OcupadoExcepcion( __AQUI__, "A temporary error occurred"\
            " on an authoritative name server. Try again later" );
         break;
         default:
            throw DesconocidoExcepcion( __AQUI__, "Error en la llamada"\
            " gethostbyname", h_errno );
      }
   }
   else
      return decodificarDireccion( *( UDWordType *)( lpHostent->h_addr_list[ 0 ] ) );

}

////////////////////   PRIVATE      ////////////////////
// =================   OPERACIONES   ===================
void Socket::throwConnectExcepcion( SDWordType sdErrno ) const
{
   switch ( sdErrno )
   {
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "The file descriptor is not a"\
         " valid index in the descriptor table.", sdErrno );
      case ENOTSOCK:
         throw NoExisteExcepcion( __AQUI__, "The file descriptor is not"\
         " associated with a socket.", sdErrno );
      case ENETUNREACH:
         throw NoExisteExcepcion( __AQUI__, "Network is unreachable.", sdErrno );
      case EFAULT:
         throw ParametroExcepcion( __AQUI__, "The socket structure address"\
         " is outside the user's address space.", sdErrno );
      case EISCONN:
         throw OcupadoExcepcion( __AQUI__, "The socket is already connected.",
         sdErrno );
      case EADDRINUSE:
         throw OcupadoExcepcion( __AQUI__, "Local address is already in use.",
         sdErrno );
      case EALREADY:
         throw OcupadoExcepcion( __AQUI__, "The socket is non-blocking and"\
         " a previous connection attempt has not yet been completed.", sdErrno );
      case ECONNREFUSED:
         throw ComunicacionExcepcion( __AQUI__, "No one listening on the"\
         " remote address.", sdErrno );
      case ETIMEDOUT:
         throw TimeoutExcepcion( __AQUI__, "Timeout while attempting"\
         " connection. The server may be too busy to accept new connections.",
         sdErrno );
      case EINPROGRESS:
         throw TimeoutExcepcion( __AQUI__, "The socket is non-blocking and"\
         " the connection cannot be completed immediately.", sdErrno );
      case EAGAIN:
         throw SinRecursosExcepcion( __AQUI__, "No more free local ports or"\
         " insufficient entries in the routing cache.", sdErrno );
      case EAFNOSUPPORT:
         throw NoAplicaExcepcion( __AQUI__, "The passed address didn't have"\
         " the correct address family in its sa_family field.", sdErrno );
      case EACCES:
      case EPERM:
         throw PermisoExcepcion( __AQUI__, "The user tried to connect to a"\
         " broadcast address without having the socket broadcast flag"\
         " enabled or the connection request failed because of a local"\
         " firewall rule.", sdErrno );
      default:
         throw DesconocidoExcepcion( __AQUI__, strerror( sdErrno ), sdErrno );
   }
};

//------------------------------------------------------------------------------
void Socket::throwSelectExcepcion( SDWordType sdErrno ) const
{
   switch ( sdErrno )
   {
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "An invalid file descriptor was"\
         " given in one of  the sets.", sdErrno );
      case EINTR:
         throw InterrupcionExcepcion( __AQUI__, "A non blocked signal was"\
         " caught", sdErrno );
      case EINVAL:
         throw ParametroExcepcion( __AQUI__, "n is negative or the value"
         " contained within timeout is invalid.", sdErrno );
      case ENOMEM:
         throw MemoriaExcepcion( __AQUI__, "select was unable to allocate"\
         " memory  for  internal tables.", sdErrno );
      default:
         throw DesconocidoExcepcion( __AQUI__, strerror( sdErrno ), sdErrno );
   }
};

//------------------------------------------------------------------------------
void Socket::throwRecvExcepcion( SDWordType sdErrno ) const
{
   switch ( sdErrno )
   {
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "The argument s is an invalid"\
         " descriptor.", sdErrno );
      case ENOTSOCK:
         throw NoExisteExcepcion( __AQUI__, "The argument s does not refer"\
         " to a socket.", sdErrno );
      case ECONNREFUSED:
         throw ComunicacionExcepcion( __AQUI__, "A remote host refused to"\
         " allow the network connection.", sdErrno );
      case ENOTCONN:
         throw ComunicacionExcepcion( __AQUI__, "The socket is associated"\
         " with a connection-oriented protocol and has not been connected",
         sdErrno );
      case ECONNRESET:
         throw ComunicacionExcepcion( __AQUI__, strerror( errno ), errno );
      case EAGAIN:
         throw TimeoutExcepcion( __AQUI__, "The socket is marked"
         " non-blocking and the receive operation would block, or a receive"\
         " timeout had been set and the timeout expired before data was"\
         " received.", sdErrno );
      case EINTR:
         throw InterrupcionExcepcion( __AQUI__, "The receive was"\
         " interrupted by delivery of a signal before any data were"\
         " available.", sdErrno );
      case EFAULT:
         throw ParametroExcepcion( __AQUI__, "The receive buffer pointer(s)"\
         " point outside the process's address space.", sdErrno );
      case EINVAL:
         throw ParametroExcepcion( __AQUI__, "Invalid argument passed.",
         sdErrno );
      case ENOMEM:
         throw MemoriaExcepcion( __AQUI__, "Could not allocate memory for"\
         " recvmsg.", sdErrno );
      default:
        throw DesconocidoExcepcion( __AQUI__, strerror( sdErrno ), sdErrno );
   }
}
//------------------------------------------------------------------------------
void Socket::throwFcntlExcepcion( SDWordType sdErrno ) const
{
   switch ( sdErrno )
   {
      case EACCES:
      case EAGAIN:
         throw PermisoExcepcion( __AQUI__, "Operation is prohibited by locks"\
         " held by other processes. Or, operation is prohibited because the"\
         " file has been memory-mapped by another process.", sdErrno );
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "fd is not an open file descriptor,"\
         " or the command was F_SETLK or F_SETLKW and the file descriptor open"\
         " mode doesn't match with the type of lock requested.", sdErrno );
      case EDEADLK:
         throw NoAplicaExcepcion( __AQUI__, "It was detected that the specified"\
         " F_SETLKW command would cause a deadlock.", sdErrno );
      case EFAULT:
         throw ParametroExcepcion( __AQUI__, "lock is outside your accessible"\
         " address space.", sdErrno );
      case EINTR:
         throw InterrupcionExcepcion( __AQUI__, "For F_SETLKW, the command was"\
         " interrupted by a signal. For F_GETLK and F_SETLK, the command was"\
         " interrupted by a signal before the lock was checked or acquired."\
         " Most likely when locking a remote file (e.g. locking over NFS), but"\
         " can sometimes happen locally.", sdErrno );
      case EINVAL:
         throw ParametroExcepcion( __AQUI__, "For F_DUPFD, arg is negative or"\
         " is greater than the maximum allowable value. For F_SETSIG, arg is"\
         " not an allowable signal number.", sdErrno );
      case EMFILE:
         throw SinRecursosExcepcion( __AQUI__, "For F_DUPFD, the process"\
         " already has the maximum number of file descriptors open.", sdErrno );
      case ENOLCK:
         throw SinRecursosExcepcion( __AQUI__, "Too many segment locks open,"\
         " lock table is full, or a remote locking protocol failed (e.g."\
         " locking over NFS).", sdErrno );
      case EPERM:
         throw PermisoExcepcion( __AQUI__, "EPERM Attempted to clear the O_APPEND"\
         " flag on a file that has the append-only attribute set.", sdErrno );
      default:
         throw DesconocidoExcepcion( __AQUI__, strerror( sdErrno ), sdErrno );
   }
};

//------------------------------------------------------------------------------
void Socket::throwGetsockoptExcepcion( SDWordType sdErrno ) const
{
   switch ( sdErrno )
   {
      case EBADF:
         throw NoExisteExcepcion( __AQUI__, "The argument s is not a valid"\
         " descriptor.", sdErrno );
      case ENOTSOCK:
         throw ParametroExcepcion( __AQUI__, "The argument s is a file, not a"\
         " socket.", sdErrno );
      case EFAULT:
         throw ParametroExcepcion( __AQUI__, "The address pointed to by optval"\
         " is not in a valid part of the process address space. For getsockopt,"\
         " this error may also be returned if optlen is not in a valid part of"\
         " the process address space.", sdErrno );
      case ENOPROTOOPT:
         throw DesconocidoExcepcion( __AQUI__, "The option is unknown at the"\
         " level indicated.", sdErrno );
      case EINVAL:
         throw ParametroExcepcion( __AQUI__, "optlen invalid in setsockopt",
         sdErrno );
      default:
        throw DesconocidoExcepcion( __AQUI__, strerror( sdErrno ), sdErrno );
   }
};
//------------------------------------------------------------------------------
timeval Socket::milisegundosToTimeval( UDWordType udMilisegundos ) const
{
   timeval lTimeval;

   lTimeval.tv_sec  = udMilisegundos / MIL;
   if ( udMilisegundos < SDWordTypeMax / MIL ) // para evitar desbordamientos
      lTimeval.tv_usec = udMilisegundos * MIL % UN_MILLON;
   else
      lTimeval.tv_usec = UN_MILLON / 2; // error de +/-0.5 segundos sobre ~1 hora
   return lTimeval;
};

//------------------------------------------------------------------------------
void Socket::setBloqueo( bool bFlagBloqueo )
{
   SDWordType lsdFlags;

   lsdFlags = getFlags(); // obtencion del estado y modo actual del socket
   if ( true == bFlagBloqueo ) // bloqueante
      lsdFlags &= ~O_NONBLOCK;
   else  // no bloqueante
      lsdFlags |= O_NONBLOCK;
   setFlags( lsdFlags ); // establecemos el modo de bloqueo
};

//------------------------------------------------------------------------------
UDWordType Socket::codificarDireccion (const Cadena& rDireccion ) const
{
   struct in_addr lAddr; // estructura con la direccion del nodo en binario

   if ( 0 == inet_aton( rDireccion.getCadena() , &lAddr ) )
      throw ParametroExcepcion( __AQUI__, "No se ha podido codificar la direccion" );
   else // la direccion se ha traducido correctamente
      return lAddr.s_addr;
};

//------------------------------------------------------------------------------
Cadena Socket::decodificarDireccion( UDWordType udDireccion )
{
   struct in_addr lAddr; // estructura con la direccion del nodo en binario
   SByteType*     lpsbDireccion;

   lAddr.s_addr = udDireccion;
   lpsbDireccion = inet_ntoa( lAddr );
   if ( 0 == lpsbDireccion )
      throw ParametroExcepcion( __AQUI__, "No se ha podido decodificar la direccion" );
   else
      return ( lpsbDireccion );
};

//-----------------------------------------------------------------------------
void Socket::rellenarEstructuraConDireccionLocal( struct sockaddr_in& addr) const
{
   addr.sin_family = AF_INET;
   if ( Cadena("") == getDireccionLocal() ) // El sistema asignara la IP de la maquina
      addr.sin_addr.s_addr = htonl( INADDR_ANY );
   else
      addr.sin_addr.s_addr = codificarDireccion( getDireccionLocal() );
   if ( 0 == getPuertoLocal() ) // El sistema asignara un puerto libre
      addr.sin_port = htons( 0 );
   else
      addr.sin_port = htons( getPuertoLocal() );
   memset( &(addr.sin_zero), 0, 8 );
}

//-----------------------------------------------------------------------------
void Socket::rellenarEstructuraConDireccionRemota( struct sockaddr_in& addr) const
{
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = codificarDireccion( getDireccionRemota() );
   addr.sin_port = htons( getPuertoRemoto() );
   memset( &(addr.sin_zero), 0, 8 );
}

//-----------------------------------------------------------------------------
void Socket::setDireccionRemota( struct sockaddr_in& addr)
{
   setPuertoRemoto( ntohs(addr.sin_port) );
   setDireccionRemota( inet_ntoa( addr.sin_addr) );
}

void Socket::apagado(const enum emode modo)
{
   if ( -1 != msdId ) // si ya esta cerrado no se hace nada
   {
      if ( FAIL == shutdown( getId(),modo))
      {
         switch(errno){
         case EBADF:
            throw NoExisteExcepcion( __AQUI__, "The argument s is not a valid descriptor.", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "Invalid argument passed.",errno );
         case ENOTCONN:
            throw ComunicacionExcepcion( __AQUI__, "The socket is not connected, and no target has been given.", errno );
         case ENOTSOCK:
            throw NoExisteExcepcion( __AQUI__, "The argument s is not a socket.",errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
         }
      }
      else
      {
#ifdef _DEBUG
         STrz::debug( ConstTraza::T_TRAZA_SOCKETS, "Shutdown en modo %d del socket fd=%d", modo, msdId);
#endif
      }
   }
}

// ===============================
//  FIN DEL FICHERO - SOCKET.CPP
// ===============================
