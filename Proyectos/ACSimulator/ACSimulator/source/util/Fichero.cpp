// =======================
//  FICHERO - Fichero.cpp
// =======================

#include "Fichero.h"  // clase implementada


Fichero::Fichero()
{
   fd = -1;
}

Fichero::~Fichero()
{

}

void Fichero::abrir(void)
{
   abrir(false);
}

void Fichero::crear(void)
{
   umask( getUMask() );
   abrir( true );
}

//------------------------------------------------------------------------------
void Fichero::crearFifo()
{
   umask( getUMask() );
   if (  0 != mkfifo( getNombre().c_str(), mmask ) )
   {
      switch( errno )
      {
       case   EEXIST:
         throw OcupadoExcepcion(__AQUI__, strerror( errno ), errno );

       default:
         throw DesconocidoExcepcion(__AQUI__, strerror( errno ), errno );
      }
   }
};

//------------------------------------------------------------------------------
void Fichero::abrir(const bool bCrear)
{
   fd = open ( getNombre().getCadena(),getFlags() | (bCrear ?  O_CREAT : 0) , mmask );
   if ( fd == -1 )
   {
      switch( errno )
      {
       case   EEXIST:
         throw OcupadoExcepcion(__AQUI__,mNombre + " pathname already exists and O_CREAT" \
                                " and O_EXCL were used" ,errno);
       case   ETXTBSY:
         throw OcupadoExcepcion(__AQUI__,mNombre + " pathname  refers  to  an  executable" \
          " image which is currently being executed and write access was requested.",errno);
       case   EISDIR:
         throw NoAplicaExcepcion(__AQUI__,mNombre + " pathname refers  to  a  directory  and" \
          " the  access requested involved writing (that is, O_WRONLY or O_RDWR is set).",errno);
       case   ENOTDIR:
         throw NoAplicaExcepcion(__AQUI__,mNombre + " A component used as a directory in pathname" \
         " is not, in  fact, a directory, or O_DIRECTORY was specified and pathname" \
         " was not a directory.",errno);
       case   ENAMETOOLONG:
         throw RangoExcepcion(__AQUI__,mNombre + " pathname was too long.",errno);
       case   ENOENT:
         throw NoExisteExcepcion(__AQUI__,mNombre + " O_CREAT is not set and the named file" \
         " does not exist. Or, a directory component in pathname does not exist" \
         " or is a dangling symbolic link.",errno);
       case   ENXIO:
         throw NoExisteExcepcion(__AQUI__,mNombre + " O_NONBLOCK | O_WRONLY is set, the named " \
         " file  is  a FIFO  and no process has the file open for reading. Or, the "\
         " file is a device special file and no corresponding device exists.",errno);
       case   ENODEV:
         throw NoExisteExcepcion(__AQUI__,mNombre + " pathname  refers  to  a  device special" \
          " file and no corresponding device exists.  (This is a Linux kernel   bug" \
          " -  in  this  situation  ENXIO  must  be returned.)",errno);
       case   EACCES:
         throw PermisoExcepcion(__AQUI__,mNombre + " The requested access to the file is not "\
          " allowed, or one  of  the  directories in pathname did not allow search "\
          " (execute) permission, or the  file  did  not exist  yet and write access "\
          " to the parent directory is not allowed.",errno);
       case   EROFS:
         throw PermisoExcepcion(__AQUI__,mNombre + " pathname refers to a file on a read-only " \
          " filesystem and write access was requested.",errno);
       case   ELOOP:
         throw FormatoExcepcion(__AQUI__,mNombre + " Too many symbolic links were encountered " \
         " in resolving pathname, or O_NOFOLLOW was specified but pathname was a "\
         " symbolic link.",errno);
       case   ENOSPC:
         throw LlenoExcepcion(__AQUI__,mNombre + " pathname was to be created but the device " \
         " containing pathname has no room for the new file.",errno);
       case   ENOMEM:
         throw MemoriaExcepcion(__AQUI__,mNombre + " Insufficient kernel memory was available.",errno);
       case   EMFILE:
         throw SinRecursosExcepcion(__AQUI__,mNombre + " The process already has the maximum" \
          " number of files open. OR The limit on the total number of files open on "\
          " the system has been reached.",errno);
       default:
         throw DesconocidoExcepcion(__AQUI__, mNombre + " Error desconocido en open.",errno);
     }
  }
}


SDWordType Fichero::getFlags()
{
   SDWordType m=0;

   if ((true == getLectura()) &&(true == getEscritura()))
   {
      m|=O_RDWR;
   }
   else
   {
      if (true == getLectura()) m|=O_RDONLY;
      if (true == getEscritura()) m|=O_WRONLY;
   }
   if (true == getExclusivo()) m|=O_EXCL;
   if (true == getAppend()) m|=O_APPEND;
   if (true == getSincronizado()) m|=O_SYNC;
   if (true == getNoctty()) m|=O_NOCTTY;
   if (true == getNoBloqueante()) m|=O_NONBLOCK;

   return m;
}

void Fichero::cerrar()
{
   if ( -1 != fd )
   {
      if ( 0 != close ( fd ) )
      {
         switch( errno )
         {
         case EBADF:
            throw NoExisteExcepcion (__AQUI__, mNombre +
               " The filedescriptor underlying stream is not  valid.",errno);
            break;
         case EINTR:
            throw InterrupcionExcepcion (__AQUI__, mNombre +
               " The close() call was interrupted by a signal.",errno);
            break;
         case   EIO:
            throw IOExcepcion(__AQUI__, mNombre + " An I/O error occurred.",errno);
            break;
         default:
            throw DesconocidoExcepcion(__AQUI__, mNombre + " Error desconocido en close.",errno);
            break;
         }
      }
      else
      {
         fd = -1;
      }
   }
}

SDWordType Fichero::escribirLinea( const Cadena& linea )
{
   return escribir( linea + LF );
}


SDWordType Fichero::leerLinea( SByteType *buf, SDWordType size, bool bFinalLinea )
{
   SByteType lsbCaracter;
   SDWordType index; //Indice de escritura sobre el buffer

   index = 0;

   buf[0] = '\0'; //Vaciamos el buffer para meter la nueva informacion

   if ( true == esFinalFichero() )
      return index; // no se ha leido ninguna linea
   else
   {
      lsbCaracter = getCaracter();
      while ( EOF != lsbCaracter )
      {
         if ( !bFinalLinea )
           if ( LF == lsbCaracter )
              break;

         buf[index] = lsbCaracter;
         index++; //Incrementamos la posicion de escritura

         if ( LF == lsbCaracter )
            break;
         else if ( index >= size ) // lsbCaracter != LF
         {
            throw LlenoExcepcion( __AQUI__,  mNombre + " Se ha intentado leer"\
               " una linea demasiado larga" );
         }
         else
            lsbCaracter = getCaracter();
      }
      return index; // se ha leido una linea
   }
}

bool Fichero::leerLinea( Cadena& linea, bool bFinalLinea )
{
   SByteType lsbCaracter;

//   if ( true == esFinalFichero() )
//      return false; // no se ha leido ninguna linea
//   else
//   {
      linea.setEmpty( );
      lsbCaracter = getCaracter();
      while ( EOF != lsbCaracter )
      {
         if ( !bFinalLinea )
           if ( LF == lsbCaracter )
              break;
         linea+= lsbCaracter;
         if ( LF == lsbCaracter )
            break;
         else if ( linea.getLong() == Cadena::LON_MAX_STRING ) // lsbCaracter != LF
         {
            throw LlenoExcepcion( __AQUI__,  getNombre() + " Se ha intentado leer"\
               " una linea demasiado larga" );
         }
         else
            lsbCaracter = getCaracter();
      }

      return linea.isEmpty()?false:true; // se ha leido una linea
//   }
}

void Fichero::sincronizar()
{
   if( FAIL == fsync( fd ) )
   {
      switch( errno )
      {
         case EBADF:
            throw NoExisteExcepcion( __AQUI__,  getNombre() + " fd is not a valid file"\
               " descriptor open for writing", errno );
            break;
         case EROFS:
         case EINVAL:
            throw NoAplicaExcepcion( __AQUI__, getNombre() + " fd is bound to a special file"\
               " which does not support synchronization", errno );
            break;
         default:
            throw DesconocidoExcepcion( __AQUI__,  getNombre() + " Error desconocido en fsync.", errno );
            break;
      }
   }
}

bool Fichero::esFinalFichero()
{
   SWordType swCar;

   swCar=getCaracter();
   if (swCar!=EOF) mover(-1);
   if (swCar==EOF) return true;
   return ( false );
}

bool Fichero::isAbierto() const
{
   if ( -1 != fd )
      return true;
   else
      return false;
};

void Fichero::mover(SDWordType sdwOffset)
{
   setPosicion ( sdwOffset,SEEK_CUR);
}

void Fichero::setPosicion (SDWordType sdwOffset)
{
   setPosicion(sdwOffset,SEEK_SET);
}

SDWordType Fichero::setPosicion (SDWordType sdwOffset,UDWordType udwPosicion)
{
    SDWordType ret;

    if(-1 == ( ret = lseek ( fd, sdwOffset,udwPosicion ) ) )
    {
      switch( errno )
      {
       case ESPIPE:
         throw NoAplicaExcepcion(__AQUI__, mNombre + " fildes  is associated with a pipe, "\
           "socket, or FIFO.",errno);
       case  EOVERFLOW:
          throw RangoExcepcion(__AQUI__, mNombre + " The  resulting file offset cannot be "\
           " represented in an off_t.",errno);
       case EBADF:
         throw NoExisteExcepcion (__AQUI__, mNombre +
           " The stream specified is not a seekable stream.",errno);
         break;
       case EINVAL:
         throw ParametroExcepcion (__AQUI__, mNombre +
           " The whence argument to fseek was not SEEK_SET,SEEK_END,or SEEK_CUR.",errno);
         break;
       default:
          throw DesconocidoExcepcion(__AQUI__, mNombre + " Error desconocido en lseek.",errno);
          break;
      }
    }
    return ret;
}

SDWordType Fichero::getPosicion ()
{
   return ( setPosicion(0,SEEK_CUR) );
}

void Fichero::irFin(void)
{
   setPosicion(0,SEEK_END);
}

void Fichero::irInicio(void)
{
   setPosicion(0,SEEK_SET);
}

SDWordType Fichero::escribir ( const void *buf, UDWordType udwcount )
{
   SDWordType ret=-1;

   if(-1 == (ret=write(fd,buf,udwcount)) )
   {
      switch( errno )
      {
       case EBADF:
          throw NoExisteExcepcion (__AQUI__, mNombre + " " + fd +
           " is not a valid file descriptor or is not open for writing.",errno);
         break;
       case EINVAL:
         throw ParametroExcepcion (__AQUI__, mNombre +
           " Is attached to an object which is unsuitable for writing.",errno);
         break;
       case EFAULT:
          throw RangoExcepcion(__AQUI__, mNombre + " is outside your accessible address space.",errno);
           break;
       case EFBIG:
          throw RangoExcepcion(__AQUI__, mNombre + " Too big.",errno);
           break;
       case EPIPE:
            throw NoAplicaExcepcion(__AQUI__, mNombre + " fildes  is associated with a pipe, "\
           "socket, or FIFO whose reading end is closed.",errno);
            break;
       case EAGAIN:
         throw ParametroExcepcion (__AQUI__, mNombre +
           " Non-blocking I/O has been selected using O_NONBLOCK and the write would block.",errno);
         break;
       case EINTR:
            throw InterrupcionExcepcion(__AQUI__, mNombre +
               " The call was interrupted by a signal before any data was written.",errno);
            break;
       case ENOSPC:
          throw RangoExcepcion(__AQUI__, mNombre +
             " The device containing the file referred to by fd has no room for the data.",errno);
          break;
       case EIO:
           throw IOExcepcion(__AQUI__, mNombre +
              " The device containing the file referred to by fd has no room for the data.",errno);
           break;

       default:
          throw DesconocidoExcepcion(__AQUI__, mNombre + " Error desconocido en write.",errno);
          break;
      }
    }
    return ret;

}

SDWordType Fichero::escribir( SDWordType sdwOffset, const void *buf, UDWordType udwcount )
{
   setPosicion( sdwOffset );
   return escribir( buf, udwcount );
}

SDWordType Fichero::escribir(const Cadena& mTexto, SDWordType sdwLongitud)
{
   if ( -1 == sdwLongitud ) sdwLongitud = mTexto.getLong();
   return escribir((void*)mTexto.getCadena(),sdwLongitud);
}

 SDWordType Fichero::leer(void *buf, SDWordType udwcount)
 {
    SDWordType ret=-1;
    Cadena cadAux;

    if(-1 == (ret=read(fd,buf,udwcount)) )
    {
      switch( errno )
      {
       case EBADF:
          throw NoExisteExcepcion (__AQUI__, mNombre +
           " Is not a valid file descriptor or is not open for writing.",errno);
         break;
       case EINVAL:
         throw ParametroExcepcion (__AQUI__, mNombre +
           " Is attached to an object which is unsuitable for writing.",errno);
         break;
       case EFAULT:
          throw RangoExcepcion(__AQUI__, mNombre + " is outside your accessible address space.",errno);
           break;
       case EISDIR:
            throw ParametroExcepcion(__AQUI__, mNombre + " Refers to a directory." ,errno);
            break;
       case ENOSPC:
          throw RangoExcepcion(__AQUI__, mNombre +
             " The device containing the file referred to by fd has no room for the data.",errno);
          break;
       case EIO:
           throw IOExcepcion(__AQUI__, mNombre +
              " The device containing the file referred to by fd has no room for the data.",errno);
           break;
       case EAGAIN:
         throw ParametroExcepcion (__AQUI__, mNombre +
           " Non-blocking I/O has been selected using O_NONBLOCK and the write would block.",errno);
         break;
       case EINTR:
            throw InterrupcionExcepcion(__AQUI__, mNombre +
               " The call was interrupted by a signal before any data was written.",errno);
            break;

       default:
          throw DesconocidoExcepcion(__AQUI__, mNombre + " Error desconocido en leer.",errno);
          break;
      }
    }
    return ret;
}

SWordType Fichero::getCaracter()
{
    UByteType        ubCaracter;
    SDWordType    sdwreaded;

    sdwreaded=leer(&ubCaracter,1);

    if (sdwreaded==0)
    {
       return(EOF);
    }
    return(ubCaracter);

}

 UDWordType Fichero::getTamano ()
 {
   struct stat buf;
   mNombre.resuelveVariableEntorno();
   if ( true == Directorio::getStat ( mNombre, buf ) )
   {
      return ( buf.st_size );
   }

   return 0;
 }

void Fichero::saltaLinea()
{
    SWordType   swCaracter;

    swCaracter = getCaracter();
    while ( (EOF != swCaracter ) && ('\n' != swCaracter) )
    {
      swCaracter = getCaracter ( );
    }
}

// =======================
//  FIN DEL FICHERO - Fichero.cpp
// =======================
