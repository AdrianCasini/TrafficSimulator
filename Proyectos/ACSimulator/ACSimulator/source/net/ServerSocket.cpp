// =======================
//  FICHERO - ServerSocket.cpp
// =======================

#include "ServerSocket.h"

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC      ////////////////////
// ===============   CICLO DE VIDA    ==================
ServerSocket::ServerSocket()
{
   setProtocolo( Socket::TCP ); // socket orientado a conexion
};

// =================   OPERACIONES   ===================
void ServerSocket::habilitarConexiones( SDWordType sdNumConexiones) const
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
void ServerSocket::aceptarConexion( Socket& socket) const
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
      if ( FAIL == s )
      {
         switch ( errno )
         {
            case EBADF:
               throw NoExisteExcepcion( __AQUI__, "An  invalid  file  descriptor"
                   " was  given in one of the sets. (Perhaps a file descriptor that "
                   " was already closed, or one on which an error has occurred.)" );
            case EINTR:
               throw InterrupcionExcepcion( __AQUI__, "A signal was caught; see signal(7)." );
            case EINVAL:
               throw NoAplicaExcepcion( __AQUI__, "nfds  is  negative  or  the value"
                     " contained within timeout is invalid." );
            case ENOMEM:
               throw MemoriaExcepcion( __AQUI__, "unable to allocate memory for internal tables." );
            default:
               throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
         }
      }
      else if (s > 0)
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
      socket.setNombreRemoto2(obtenerNombreRemoto(lAddrCliente));
   }
};

// ===============================
//  FIN DEL FICHERO - ServerSocket.cpp
// ===============================
