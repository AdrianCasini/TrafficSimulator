// =======================
//  FICHERODATOS - FicheroDatos.cpp
// =======================

#include "FicheroDatos.h"  // clase implementada

FicheroDatos::FicheroDatos()
{
   mFichero = NULL;
   mpsbBuffer = (PSByteType)malloc (MAX_LONG_LINEA);
   mLongBuffer = MAX_LONG_LINEA;
}

FicheroDatos::~FicheroDatos()
{
   free (mpsbBuffer);
}

void FicheroDatos::abrir ()
{
   mFichero = fopen ( getNombre().getCadena (),  getModo (false).getCadena () );
   if (mFichero == NULL)
      errorAbrirCrear (errno);
}

void FicheroDatos::crear ()
{
   mFichero = fopen ( getNombre().getCadena (),  getModo ( true ).getCadena () );
   if (mFichero == NULL)
      errorAbrirCrear (errno);
}

void FicheroDatos::cerrar ()
{
   if( NULL != mFichero )
   {
      if ( EOF ==  fclose ( mFichero ) )
      {
         switch ( errno )
         {
            case EBADF:
               throw NoExisteExcepcion ( __AQUI__, mNombre +
                  " The filedescriptor underlying stream is not  valid.", errno );
               break;
            case EINTR:
               throw InterrupcionExcepcion ( __AQUI__, mNombre +
                  " The close() call was interrupted by a signal.", errno );
               break;
            case   EIO:
               throw IOExcepcion( __AQUI__,
                  mNombre + " An I/O error occurred.", errno );
               break;
            default:
               throw DesconocidoExcepcion ( __AQUI__,
                  mNombre + " Error desconocido en close.", errno );
               break;
         }
      }
      else
      {
         mFichero = NULL;
      }
   }
}

void FicheroDatos::setBuffer (char* buffer, SDWordType sdwTamanyo)
{
   int modo;

   if (buffer == NULL || sdwTamanyo == 0)
      modo = _IONBF;
   else
      modo = _IOFBF;
   setvbuf (mFichero, buffer, modo, sdwTamanyo);
}

SDWordType FicheroDatos::leer( void *buf, SDWordType udwcount )
{
   SDWordType sdwResult;

   if ( esFinalFichero () )
      return false;

   clearerr ( mFichero );

   sdwResult = fread ( buf, 1, udwcount, mFichero );

   if ( 0 != ferror ( mFichero ) )
   {
      throw IOExcepcion ( __AQUI__, " An I/O error occurred." );
   }

   return sdwResult;
}

SDWordType FicheroDatos::escribir(const void *buf,SDWordType udwCount)
{
   SDWordType res;

   res = fwrite(buf,1,udwCount,mFichero);
   if ( getSincronizado() )
      sincronizar();
   return res;
}

SDWordType FicheroDatos::escribir( SDWordType sdwOffset, const void *buf, SDWordType udwcount )
{
   setPosicion( sdwOffset );
   return escribir( buf, udwcount );
}

SDWordType FicheroDatos::escribir(const Cadena& lValor)
{
   return (escribir(lValor.getCadena(),(SDWordType)lValor.getLong()));
}

void FicheroDatos::sincronizar()
{
   fflush (mFichero);
   fsync (fileno (mFichero));
}

SDWordType FicheroDatos::insertarLinea(const Cadena& lTexto,bool lFinalLinea)
{
   UWordType luwPosInicial;
   Cadena mTemporal;

   luwPosInicial = getPosicion();
   mTemporal = lTexto;
   if(true == lFinalLinea)
      mTemporal += '\n';

   if( true == esFinalFichero() )
   {
      // Es el final del fichero no tenemos que insertar con
      // escribir al final del fichero ya tenemos
      return( escribir(mTemporal) );
   }
   else
   {
      // Desplazamos el texto
      return(insertarCaracteres(mTemporal.getCadena(),mTemporal.getLong()));
   }
}

SWordType FicheroDatos::insertarCaracteres(const void *buf,SDWordType luwNumCaracteres)
{
   UWordType luwLeidosAct;
   SByteType lsbTexto [MAX_LONG_LINEA];
   UWordType luwPosAct;
   UWordType luwFaltan;
   UWordType luwPosInicial;

   luwPosInicial = getPosicion();

   if(true == esFinalFichero() )
   {
       // Es el final del fichero no tenemos que insertar con
       // escribir al final del fichero ya tenemos
       return(escribir(buf,luwNumCaracteres));
   }
   else
   {
       // Desplazamos el texto
       luwFaltan = getTamano () - luwPosInicial;
       luwPosAct = getTamano ();
       while( (luwFaltan > 0) && (false == esFinalFichero()))
       {
          if(luwFaltan >= MAX_LONG_LINEA)
          {
             setPosicion( luwPosAct - MAX_LONG_LINEA );
             luwLeidosAct = leer (lsbTexto , MAX_LONG_LINEA );
          }
          else
          {
             setPosicion( luwPosAct - luwFaltan );
             luwLeidosAct = leer (lsbTexto , luwFaltan );
          }
          setPosicion( luwPosAct - luwLeidosAct + luwNumCaracteres);
          escribir((PSByteType)lsbTexto,luwLeidosAct);
          luwPosAct -= luwLeidosAct;
          luwFaltan -= luwLeidosAct;
        }
        // Insertamos el nuevo texto
        setPosicion(luwPosInicial );
        escribir(buf,luwNumCaracteres);
        setPosicion(luwPosInicial);
   }
   return(luwNumCaracteres);
}

SWordType FicheroDatos::borrarCaracteres(UWordType luwNumCaracteres)
{
   UWordType luwTamTotal;
   UWordType luwLeidosTotal;
   UWordType luwLeidosAct;
   UWordType luwTamQuitar;
   SByteType lsbTexto [MAX_LONG_LINEA];
   UWordType luwPosInicial;

   if(esFinalFichero())
      return(-1);

   luwLeidosTotal = 0;
   if(luwNumCaracteres > 0)
   {
      // Si no es final del fichero borramos el caracter y movemos el resto
      // una posicion para atras
      luwPosInicial = getPosicion();
      luwTamQuitar = getTamano () - luwNumCaracteres;
      luwTamTotal = getTamano () - luwPosInicial - luwNumCaracteres;

      while((luwLeidosTotal < luwTamTotal) && (false == esFinalFichero() ))
      {
         setPosicion(luwPosInicial + luwLeidosTotal + luwNumCaracteres);
         luwLeidosAct = leer(lsbTexto,MAX_LONG_LINEA);
         setPosicion(luwPosInicial + luwLeidosTotal);
         escribir((PSByteType)lsbTexto,luwLeidosAct);
         luwLeidosTotal += luwLeidosAct;
      }

      cerrar ( );
      // Eliminamos la parte final del fichero.
      truncate ( getNombre().getCadena(), luwTamQuitar);
      abrir ( );
      irInicio ( );
   }
   return(luwLeidosTotal);
}

bool FicheroDatos::borrarCaracter()
{
   if(-1 == borrarCaracteres(1) )
      return(false);
   else
      return(true);
}

UWordType FicheroDatos::borrarLinea(bool lFinalLinea)
{
   SByteType lcarActual;
   UWordType luwNum;
   UWordType luwPosInicial;

   if(esFinalFichero())
      return(0);

   luwNum = 0;
   luwPosInicial = getPosicion();
   // Calculamos el numero de caracteres
   lcarActual = fgetc(mFichero);
   while((  '\n' != lcarActual ) && (false == esFinalFichero() ))
   {
      // Leemos el siguiente caracter
      lcarActual = fgetc(mFichero);
      luwNum++;
   }
   setPosicion(luwPosInicial);
   if( (true == lFinalLinea ) && ( '\n' == lcarActual ))
    borrarCaracteres(luwNum + 1);
   else
    borrarCaracteres(luwNum);
   return(luwNum);
}

SDWordType FicheroDatos::leerLinea (PSByteType* linea, size_t* linSz, bool bFinalLinea)
{
   SDWordType ret;

   if (esFinalFichero ())
      return 0;

   fgets (*linea, *linSz, mFichero);
   ret = ((char*)memchr (*linea, '\0', *linSz) - *linea);
   PSByteType buffer = *linea;
   if (!bFinalLinea && ret > 0 && buffer[ret - 1] == '\n')
      buffer[ret - 1] = '\0';
   else if (ret >= 0)
      buffer[ret] = '\0';

   return (ret == -1 ? 0 : ret);
}

bool FicheroDatos::leerLinea(Cadena& lLinea,bool bFinalLinea)
{
   size_t lRes;

   lRes = leerLinea(&mpsbBuffer,&mLongBuffer,bFinalLinea);
   if(lRes > 0)
      lLinea = mpsbBuffer;

   return(lRes > 0);
}

bool FicheroDatos::esFinalFichero()
{
   bool bFin;

   bFin = (fgetc (mFichero) == EOF);
   if (!bFin)
      fseek (mFichero, -1, SEEK_CUR);
   return bFin;
}

bool FicheroDatos::isAbierto () const
{
   return (mFichero != NULL);
}

void FicheroDatos::mover(SDWordType sdwOffset)
{
   setPosicion (sdwOffset, SEEK_CUR);
}

SDWordType FicheroDatos::getPosicion ()
{
   return ftell (mFichero);
}

void FicheroDatos::setPosicion (SDWordType sdwOffset)
{
   setPosicion (sdwOffset, SEEK_SET);
}

void FicheroDatos::irFin()
{
   setPosicion (0, SEEK_END);
}

void FicheroDatos::irInicio()
{
   setPosicion (0, SEEK_SET);
}

SWordType FicheroDatos::getCaracter ( )
{
   SByteType c;
   SDWordType sdRead;

   clearerr ( mFichero );

   sdRead = fread ( &c, 1, 1, mFichero );

   if (  0 == sdRead )
   {
       return EOF;
   }

   if ( 0 != ferror ( mFichero ) )
   {
      throw IOExcepcion ( __AQUI__, " An I/O error occurred." );
   }

   return c;
}

void FicheroDatos::saltaLinea()
{
   leerLinea(&mpsbBuffer,&mLongBuffer,true);
}

Cadena FicheroDatos::getModo(bool bCrear)
{
   Cadena m;

   if (bCrear)
      m = "w";
   else if (bLectura && bEscritura && !bAppend)
      m = "r+";
   else if (bLectura && bEscritura && bAppend)
      m = "a+";
   else if (bLectura && !bEscritura && !bAppend)
      m = "r";
   else if (!bLectura && bEscritura && !bAppend)
      m = "w";
   else if (!bLectura && bEscritura && bAppend)
      m = "a";
   else
      m = "r";

   return m;
}

SDWordType FicheroDatos::setPosicion (SDWordType sdwOffset, UDWordType udwPosicion)
{
   SDWordType sdRet;

   sdRet = fseek (mFichero, sdwOffset, udwPosicion);
   if (sdRet < 0)
   {
      switch( errno )
      {
         case ESPIPE:
            throw NoAplicaExcepcion(__AQUI__,"fildes  is associated with a pipe, "\
               "socket, or FIFO.",errno);
         case EOVERFLOW:
            throw RangoExcepcion(__AQUI__,"The  resulting file offset cannot be "\
               " represented in an off_t.",errno);
         case EBADF:
            throw NoExisteExcepcion (__AQUI__,
               "The stream specified is not a seekable stream.",errno);
            break;
         case EINVAL:
            throw ParametroExcepcion (__AQUI__,
               "The whence argument to fseek was not SEEK_SET,SEEK_END,or SEEK_CUR.",errno);
            break;
         default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en lseek.",errno);
      }
   }

   return sdRet;
}

UDWordType FicheroDatos::getTamano ( )
{
   struct stat buf;
   mNombre.resuelveVariableEntorno();
   if ( true == Directorio::getStat ( mNombre, buf ) )
   {
      return buf.st_size;
   }

   return 0;
}


void FicheroDatos::errorAbrirCrear ( int err )
{
   switch ( err )
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
