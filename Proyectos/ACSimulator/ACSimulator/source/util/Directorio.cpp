//---------------------------------------------------------------------------
// Directorio.cpp
//---------------------------------------------------------------------------

#include "Directorio.h"  // clase implementada

//------------------------------------------------------------------------------
Directorio::Directorio ( )
{
   namelist = NULL;
}

//------------------------------------------------------------------------------
Directorio::~Directorio ( )
{
   freeNameList ( );
}

//------------------------------------------------------------------------------
void Directorio::inicializar ( const Cadena& cadDir )
{
   setRutaDirectorio ( cadDir );
}

//------------------------------------------------------------------------------
Cadena Directorio::getRutaDirectorio ( )
{
   return mcadRuta;
}

//------------------------------------------------------------------------------
void Directorio::setRutaDirectorio ( const Cadena& cadDir )
{
   mcadRuta = cadDir;
}

//------------------------------------------------------------------------------
SDWordType Directorio::leer ( )
{
   freeNameList ( );

   if ( false == mcadRuta.isEmpty ( ) )
   {
      sdwNumFiles = scandir ( mcadRuta.getCadena(), &namelist, 0, 0 );
   }
   else
   {
      sdwNumFiles = 0;
   }
   sdwIndexFile = 0;

   return sdwNumFiles;
}

//------------------------------------------------------------------------------
Cadena Directorio::siguiente ( enum Directorio::TipoEntradas eTipos )
{
   SDWordType i;
   Cadena cadRetorno;

   cadRetorno = "";

   if ( sdwIndexFile < sdwNumFiles+1 )
   {
      sdwIndexFile++;
      if ( ( i = findNext ( eTipos ) ) != -1 )
      {
         sdwIndexFile = i;
         cadRetorno = namelist [ i ]->d_name;
      }
      else
      {
         if ( sdwNumFiles > 0 )
         {
            sdwIndexFile = -1;
         }
      }
   }

   return cadRetorno;
}

//------------------------------------------------------------------------------
bool Directorio::existe ( Cadena cadDir, enum Directorio::TipoEntradas eTipos )
{
   struct stat buf;
   bool bResult;

   bResult = false;

   if( true == cadDir.contieneVariableEntorno() )
      cadDir.resuelveVariableEntorno();

   // Comprobamos si lo que queremos abrir es un fichero o un directorio.
   if (  0 == stat ( cadDir.getCadena(), &buf ) )
   {
      switch ( eTipos )
      {
         case FICHERO:
            bResult = S_ISREG ( buf.st_mode );
            break;
         case DIRECTORIO:
            bResult = S_ISDIR ( buf.st_mode );
            break;
         default:
            bResult = true;
            break;
      }
   }

   return bResult;
}

//------------------------------------------------------------------------------
bool Directorio::crearDirectorio ( Cadena cadDestino )
{
   bool bReturn;

   bReturn = false;

   if ( false == cadDestino.isEmpty ( ))
   {
      if( true == cadDestino.contieneVariableEntorno() )
         cadDestino.resuelveVariableEntorno();

      if ( -1 == mkdir ( cadDestino.getCadena(), S_IRWXU | S_IRWXG | S_IRWXO ) )
      {
         switch( errno )
         {
            case EPERM:
               throw NoAplicaExcepcion ( __AQUI__, cadDestino + " The filesystem containing" \
                  " pathname does not support the creation of directories.", errno );
            case EEXIST:
               throw OcupadoExcepcion ( __AQUI__, cadDestino + " pathname already exists" \
                  " (not necessarily as a directory). This includes the case where"\
                  " pathname is a symbolic link, dangling or not.", errno );
            case EFAULT:
               throw ParametroExcepcion ( __AQUI__, cadDestino + " pathname points outside" \
                  " your accessible address space.", errno );
            case EACCES:
               throw PermisoExcepcion ( __AQUI__, cadDestino + " The parent directory does" \
                  " not allow write permission to the process, or one of the directories" \
                  " in pathname did not allow search (execute) permission.", errno );
            case ENAMETOOLONG:
               throw ParametroExcepcion ( __AQUI__, cadDestino + " pathname was too long.", errno );
            case ENOENT:
               throw NoExisteExcepcion ( __AQUI__, cadDestino + " A directory component in"\
                  " pathname does not exist or is a dangling symbolic link.", errno );
            case ENOTDIR:
               throw NoAplicaExcepcion ( __AQUI__, cadDestino + " A component used as a"\
                  " directory in pathname is not, in fact, a directory.", errno );
            case ENOMEM:
               throw SinRecursosExcepcion ( __AQUI__, cadDestino + " Insufficient kernel"\
               " memory was available.", errno );
            case EROFS:
               throw PermisoExcepcion ( __AQUI__, cadDestino + " pathname refers to a file"\
                  " on a read-only filesystem.", errno );
            case ELOOP:
               throw ParametroExcepcion ( __AQUI__, cadDestino + " Too many symbolic links"\
                  " were encountered in resolving pathname.", errno );
            case ENOSPC:
               throw SinRecursosExcepcion ( __AQUI__, cadDestino + " The device containing"\
               " pathname has no room for the new directory. O the new directory cannot"\
               " be created because the user's disk quota is exhausted.", errno );
            default:
               throw DesconocidoExcepcion ( __AQUI__, cadDestino + " Error desconocido en mkdir.", errno);
         }
      }
      else
      {
         bReturn = true;
      }

   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "El nombre del directorio especificado esta vacio." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
// FIXME: Eliminar esta llamada. Se puede realizar con Fichero.crear().
bool Directorio::crearFichero ( const Cadena& cadDestino )
{
   int fileDesc;
   bool bReturn;

   bReturn = false;

   if ( -1 != ( fileDesc = creat ( cadDestino.getCadena(),
         S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) ) )
   {
      bReturn = true;
      close( fileDesc );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::mover ( Cadena& cadDestino )
{
   Cadena mcadOrigen;
   Cadena mcadDestino;

   mcadOrigen = mcadRuta + Cadena("/") + Cadena ( namelist [ sdwIndexFile ]->d_name );
   mcadDestino = cadDestino + Cadena("/") + Cadena ( namelist [ sdwIndexFile ]->d_name );

   return mover (  mcadOrigen, mcadDestino );
}



//------------------------------------------------------------------------------
bool Directorio::copiar ( Cadena& cadOrigen, Cadena& cadDestino )
{
   bool bReturn;
   SDWordType sdwStatus;
   Cadena mcadExe;

   bReturn = false;

   if ( false == cadDestino.isEmpty ( ) && false == cadOrigen.isEmpty ( ) )
   {
      if( true == cadDestino.contieneVariableEntorno() )
         cadDestino.resuelveVariableEntorno ();
      if( true == cadOrigen.contieneVariableEntorno() )
         cadOrigen.resuelveVariableEntorno ();

      mcadExe = Cadena ( "cp -f " ) + cadOrigen + Cadena ( " " ) + cadDestino + Cadena ( " 1>/dev/null 2>/dev/null" );

      sdwStatus = system ( mcadExe.getCadena ( ) );

      if ( sdwStatus > -1 )
      {
         if ( 0 == WEXITSTATUS ( sdwStatus ) )
         {
            bReturn = true;
         }
         else
         {
            throw IOExcepcion ( __AQUI__,  " An I/O error occurred." );
         }
      }
      else
      {
            throw IOExcepcion ( __AQUI__,  " An I/O error occurred." );
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "Alguna de las rutas especificadadas esta vacia." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::mover ( Cadena& cadOrigen, Cadena& cadDestino )
{
   bool bReturn;
   SDWordType sdwStatus;
   Cadena mcadExe;

   bReturn = false;

   if ( false == cadDestino.isEmpty ( ) && false == cadOrigen.isEmpty ( ) )
   {
      if( true == cadDestino.contieneVariableEntorno() )
         cadDestino.resuelveVariableEntorno ();
      if( true == cadOrigen.contieneVariableEntorno() )
         cadOrigen.resuelveVariableEntorno ();

      mcadExe = Cadena ( "mv -f " ) + cadOrigen + Cadena ( " " ) + cadDestino + Cadena ( " 1>/dev/null 2>/dev/null" );

      sdwStatus = system ( mcadExe.getCadena ( ) );

      if ( sdwStatus > -1 )
      {
         if ( 0 == WEXITSTATUS ( sdwStatus ) )
         {
            bReturn = true;
         }
         else
         {
            throw IOExcepcion ( __AQUI__,  " An I/O error occurred." );
         }
      }
      else
      {
            throw IOExcepcion ( __AQUI__,  " An I/O error occurred." );
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "Alguna de las rutas especificadadas esta vacia." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::getStat ( const Cadena& cadPath, struct stat& mstat )
{
   bool bReturn;

   bReturn = false;

   if ( -1 == stat ( cadPath.getCadena(), &mstat ) )
   {
      switch( errno )
      {
         case EBADF:
            throw NoExisteExcepcion ( __AQUI__, cadPath + " Filedes is bad.", errno );
            break;
         case ENOENT:
            throw ParametroExcepcion ( __AQUI__, cadPath +
            " A component of the path file_name does not exist, or the path is an empty string.", errno );
            break;
         case ENOTDIR:
            throw RangoExcepcion ( __AQUI__, cadPath +
               " A component of the path is not a directory.", errno );
            break;
         case ELOOP:
               throw ParametroExcepcion ( __AQUI__, cadPath +
               " Too many symbolic links encountered while traversing the path.", errno );
               break;
         case EFAULT:
            throw RangoExcepcion ( __AQUI__, cadPath + " Bad address.", errno );
            break;
         case EACCES:
            throw PermisoExcepcion ( __AQUI__, cadPath + " Permission denied.", errno );
            break;
         case ENOMEM:
            throw MemoriaExcepcion ( __AQUI__, cadPath + " Out of memory.", errno);
            break;
         case ENAMETOOLONG:
               throw ParametroExcepcion ( __AQUI__, cadPath + " File name too long.", errno);
               break;
         default:
            throw DesconocidoExcepcion ( __AQUI__, cadPath + " Error desconocido en stat.", errno);
            break;
      }
   }
   else
   {
      bReturn = true;
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::eliminar ( enum Directorio::TipoEntradas eTipos )
{
   Cadena mcadPath;
   bool bReturn;

   bReturn = false;

   if ( false == mcadRuta.isEmpty() && NULL != namelist [ sdwIndexFile ]->d_name )
   {
      mcadPath = mcadRuta + Cadena ( "/" ) + Cadena ( namelist [ sdwIndexFile ]->d_name );
      bReturn = eliminar ( mcadPath, eTipos );
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada no es valida." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::eliminarContenidoDirectorio ( Cadena cadPath )
{
   bool bResult;
   Directorio mDirectorioEliminar;
   Cadena Elemento;

   bResult = false;

   if ( true == existe ( cadPath, Directorio::DIRECTORIO ) )
   {
      mDirectorioEliminar.inicializar ( cadPath );
      if ( 0 < mDirectorioEliminar.leer () )
      {
         while ( false == (Elemento = mDirectorioEliminar.siguiente ( Directorio::FICHERO )).isEmpty() )
         {
            mDirectorioEliminar.eliminar ( Directorio::FICHERO );
         }
      }

      bResult = true; // Tenga o no ficheros a eliminar.

   }

   return bResult;
}

//------------------------------------------------------------------------------
bool Directorio::eliminar ( Cadena cadPath, enum Directorio::TipoEntradas eTipos )
{
   struct stat buf;
   bool bResult;
   bool bEliminable;

   bResult = false;
   bEliminable = false;

   if ( false == cadPath.isEmpty() )
   {
      // Verificamos que se elimina o un fichero o un directorio.
      cadPath.resuelveVariableEntorno( );
      if ( true == getStat ( cadPath.getCadena(), buf ) )
      {
         switch ( eTipos )
         {
            case FICHERO:
               if ( S_ISREG ( buf.st_mode ) )
               {
                     bResult = eliminarFichero ( cadPath );
               }
               else
               {
                  throw NoAplicaExcepcion ( __AQUI__, cadPath + " No es un fichero." );
               }
               break;
            case DIRECTORIO:
               if ( S_ISDIR ( buf.st_mode ) )
               {
                  bResult = eliminarDirectorio ( cadPath );
               }
               else
               {
                  throw NoAplicaExcepcion ( __AQUI__, cadPath + " No es un directorio." );
               }
               break;
            default:
               throw NoAplicaExcepcion ( __AQUI__, "El parametro especificado no es correcto." );
               break;
         }
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada esta vacia." );
   }
   return bResult;
}

//------------------------------------------------------------------------------
bool Directorio::eliminarFichero ( const Cadena& cadPath )
{
   bool bReturn;

   bReturn = false;

   if ( false == cadPath.isEmpty() )
   {
      if ( -1 == unlink ( cadPath.getCadena ( ) ) )
      {
         switch ( errno )
         {
            case EACCES:
               throw  PermisoExcepcion(__AQUI__, cadPath +
                     " Write  access  to the directory" \
                     " containing pathname is not allowed for the process's effective" \
                     " uid, or one  of  the  directories in pathname did not allow" \
                     " search (execute) permission.", errno );
               break;
            case EPERM:
               throw PermisoExcepcion(__AQUI__, cadPath +
                     "The filesystem does not allow unlinking of files." \
                     "The system does not allow unlinking of directories, " \
                     "or unlinking  of  directories  requires  privileges " \
                     "that  the  current  process doesn't have.  (This is " \
                     "the POSIX prescribed error return.)", errno );
            case EISDIR:
               throw NoAplicaExcepcion(__AQUI__, cadPath +
                     " pathname refers to a directory.  " \
                     " (This is the  nonPOSIX value returned by Linux since 2.1.132.)", errno );
               break;
            case EBUSY:
               throw OcupadoExcepcion(__AQUI__, cadPath +
                     " (not on Linux) " \
                     "The  file pathname cannot be unlinked because it is " \
                     "being used by the system or another process and the " \
                     "implementation considers this an error.", errno );
               break;
            case EFAULT:
               throw MemoriaExcepcion(__AQUI__, cadPath +
                     " pathname  points  outside  your" \
                     " accessible address space.", errno );
               break;
            case ENAMETOOLONG:
               throw RangoExcepcion(__AQUI__, cadPath + " pathname was too long.", errno );
               break;
            case ENOENT:
               throw NoExisteExcepcion(__AQUI__, cadPath + " A component in pathname does not" \
                  " exist or is a dangling symbolic link, or pathname is empty.", errno );
               break;
            case ENOTDIR:
               throw NoExisteExcepcion(__AQUI__, cadPath + " A component used as a directory in" \
                  " pathname is not,in fact, a directory.", errno );
               break;
            case ENOMEM:
               throw MemoriaExcepcion(__AQUI__, cadPath + " Insufficient kernel memory was available.", errno );
               break;
            case EROFS:
               throw PermisoExcepcion(__AQUI__, cadPath + " pathname refers to a file on a " \
                  "read-only  filesystem.", errno );
               break;
            case ELOOP:
                  throw NoAplicaExcepcion(__AQUI__, cadPath + " Too many symbolic links were " \
                  "encountered in translating pathname.", errno );
               break;
            case EIO:
                  throw IOExcepcion(__AQUI__, cadPath + " An I/O error occurred.", errno );
               break;
            default:
               throw DesconocidoExcepcion(__AQUI__, cadPath + " Error desconocido en unlink.", errno );
               break;
         }
      }
      else
      {
         bReturn = true;
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada no es vacia." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
bool Directorio::eliminarDirectorio ( const Cadena& cadPath )
{
   bool bReturn;

   bReturn = false;

   if ( false == cadPath.isEmpty() )
   {
      if( 0 != rmdir ( cadPath.getCadena ( ) ) )
      {
         switch( errno )
         {
            case EPERM:
               throw  PermisoExcepcion( __AQUI__, cadPath +
                  " The filesystem containing pathname does not support the removal of directories or " \
                  "the directory containing pathname has the sticky-bit (S_ISVTX) set and the " \
                  "process's effective uid is neither the uid of the file to be deleted nor that of " \
                  "the directory containing it.", errno );
               break;
            case EFAULT:
               throw MemoriaExcepcion ( __AQUI__, cadPath +
                  "Pathname points outside your accessible address space.", errno );
               break;
            case EACCES:
               throw PermisoExcepcion ( __AQUI__, cadPath +
               " Write  access to the directory containing pathname was not allowed for the " \
               "process's effective uid, or one of the directories in pathname did not allow " \
               "search (execute) permission.", errno );
               break;
            case ENAMETOOLONG:
               throw ParametroExcepcion ( __AQUI__, cadPath + " Pathname was too long.", errno );
               break;
            case ENOENT:
               throw NoExisteExcepcion ( __AQUI__, " A directory component in pathname " \
                  "does not exist or is a dangling symbolic link.", errno );
               break;
            case ENOTDIR:
               throw NoAplicaExcepcion ( __AQUI__, " Pathname, or a component used as a " \
                  "directory in pathname, is not, in fact, a directory.", errno );
               break;
            case ENOTEMPTY:
               throw NoAplicaExcepcion ( __AQUI__, " Pathname contains entries other than . and .. .", errno );
               break;
            case EINVAL:
               throw NoAplicaExcepcion ( __AQUI__, " Pathname has .  as last component.", errno );
               break;
            case EBUSY:
               throw OcupadoExcepcion(__AQUI__, " Pathname is the current working directory " \
                  "or root directory of some process.", errno );
               break;
            case ENOMEM:
               throw SinRecursosExcepcion ( __AQUI__,
                  " Insufficient kernel memory was available.", errno );
               break;
            case EROFS:
               throw PermisoExcepcion ( __AQUI__,
                  " Pathname refers to a file on a read-only filesystem.", errno );
               break;
            case ELOOP:
               throw ParametroExcepcion ( __AQUI__,
                  " Too many symbolic links were encountered in resolving pathname.", errno );
               break;
            default:
               throw DesconocidoExcepcion(__AQUI__, cadPath + " Error desconocido en rmdir.", errno );
               break;
         }
      }
      else
      {
         bReturn = true;
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada no es vacia." );
   }

   return bReturn;
}

//------------------------------------------------------------------------------
void Directorio::freeNameList ( )
{
   SDWordType i;

   if ( namelist != NULL )
   {
      if ( sdwNumFiles > 0 )
      {
         for ( i = 0; i < sdwNumFiles; i++ )
         {
            free ( namelist [ i ] );
         }
      }
      free ( namelist );
      namelist = NULL;
   }
   sdwNumFiles = -1;
   sdwIndexFile = -1;
}

//------------------------------------------------------------------------------
SDWordType  Directorio::findNext ( enum Directorio::TipoEntradas eTipos )
{
   SDWordType i;
   struct stat buf;
   Cadena mcadPath;
   bool bExist;

   bExist = false;

   if ( sdwNumFiles > 0 )
   {
      for ( i = sdwIndexFile; i < sdwNumFiles ; i++ )
      {
         mcadPath = mcadRuta + Cadena ( "/" ) + Cadena( namelist[i]->d_name );
         // Comprobamos el estado del fichero.
         if ( true == getStat ( mcadPath, buf ) )
         {
            switch ( eTipos )
            {
               case FICHERO:
                  if ( S_ISREG ( buf.st_mode ) )
                  {
                     bExist = true;
                  }
                  break;
               case DIRECTORIO:
                  // Descartamos los directorios....
                  if ( 0 != strcmp ( namelist[ i ]->d_name, "." )
                        && 0 != strcmp ( namelist[ i ]->d_name, ".." ) )
                  {
                     if ( S_ISDIR ( buf.st_mode ) )
                     {
                        bExist = true;
                     }
                  }
                  break;
               default:
                  bExist = true;
                  break;
            }

            if ( bExist == true )
            {
               break;
            }
         }
      }
   }
   // Si no encontramos nigun elemento en la lista.
   if ( bExist == false )
   {
      i = -1;
   }

   return i;
}


int secure_remove(const char* filename)
{
   Cadena cmd;
   struct stat buf;

   if (  0 == stat ( filename, &buf ) )
   {
      if(S_ISDIR ( buf.st_mode ))
      {
         errno=EISDIR;
         return -1;
      }
   }
   else
   {
      errno=ENOENT;
      return -1;
   }

   cmd.formCadena ("/usr/bin/shred -u -f -x -n 0 -z %s", filename);
   if (system (cmd.getCadena ()) != 0)
      return -1;
   else
      return 0;
}

bool Directorio::eliminarSeguro ( enum TipoEntradas eTipos )
{
   Cadena mcadPath;
   bool bReturn;

   bReturn = false;

   if ( false == mcadRuta.isEmpty() && NULL != namelist [ sdwIndexFile ]->d_name )
   {
      mcadPath = mcadRuta + Cadena ( "/" ) + Cadena ( namelist [ sdwIndexFile ]->d_name );
      bReturn = eliminarSeguro ( mcadPath, eTipos );
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada no es valida." );
   }

   return bReturn;
}

bool Directorio::eliminarContenidoDirectorioSeguro ( Cadena cadPath )
{
   bool bResult;
   Directorio mDirectorioEliminar;
   Cadena Elemento;

   bResult = false;

   if ( true == existe ( cadPath, Directorio::DIRECTORIO ) )
   {
      mDirectorioEliminar.inicializar ( cadPath );
      if ( 0 < mDirectorioEliminar.leer () )
      {
         while ( false == (Elemento = mDirectorioEliminar.siguiente ( Directorio::FICHERO )).isEmpty() )
         {
            mDirectorioEliminar.eliminarSeguro ( Directorio::FICHERO );
         }
      }

      bResult = true; // Tenga o no ficheros a eliminar.

   }

   return bResult;
}

bool Directorio::eliminarSeguro ( Cadena cadPath, enum TipoEntradas eTipos )
{
   struct stat buf;
   bool bResult;
   bool bEliminable;

   bResult = false;
   bEliminable = false;

   if ( false == cadPath.isEmpty() )
   {
      // Verificamos que se elimina o un fichero o un directorio.
      cadPath.resuelveVariableEntorno( );
      if ( true == getStat ( cadPath.getCadena(), buf ) )
      {
         switch ( eTipos )
         {
            case FICHERO:
               if ( S_ISREG ( buf.st_mode ) )
               {
                     bResult = eliminarFicheroSeguro ( cadPath );
               }
               else
               {
                  throw NoAplicaExcepcion ( __AQUI__, cadPath + " No es un fichero." );
               }
               break;
            case DIRECTORIO:
               if ( S_ISDIR ( buf.st_mode ) )
               {
                  bResult = eliminarDirectorio ( cadPath );
               }
               else
               {
                  throw NoAplicaExcepcion ( __AQUI__, cadPath + " No es un directorio." );
               }
               break;
            default:
               throw NoAplicaExcepcion ( __AQUI__, "El parametro especificado no es correcto." );
               break;
         }
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada esta vacia." );
   }
   return bResult;
}

bool Directorio::eliminarFicheroSeguro ( const Cadena& cadPath )
{
   bool bReturn;

   bReturn = false;

   if ( false == cadPath.isEmpty() )
   {
      if ( -1 == secure_remove ( cadPath.getCadena ( ) ) )
      {
         switch ( errno )
         {
            case EACCES:
               throw  PermisoExcepcion(__AQUI__, cadPath +
                     " Write  access  to the directory" \
                     " containing pathname is not allowed for the process's effective" \
                     " uid, or one  of  the  directories in pathname did not allow" \
                     " search (execute) permission.", errno );
               break;
            case EPERM:
               throw PermisoExcepcion(__AQUI__, cadPath +
                     "The filesystem does not allow unlinking of files." \
                     "The system does not allow unlinking of directories, " \
                     "or unlinking  of  directories  requires  privileges " \
                     "that  the  current  process doesn't have.  (This is " \
                     "the POSIX prescribed error return.)", errno );
            case EISDIR:
               throw NoAplicaExcepcion(__AQUI__, cadPath +
                     " pathname refers to a directory.  " \
                     " (This is the  nonPOSIX value returned by Linux since 2.1.132.)", errno );
               break;
            case EBUSY:
               throw OcupadoExcepcion(__AQUI__, cadPath +
                     " (not on Linux) " \
                     "The  file pathname cannot be unlinked because it is " \
                     "being used by the system or another process and the " \
                     "implementation considers this an error.", errno );
               break;
            case EFAULT:
               throw MemoriaExcepcion(__AQUI__, cadPath +
                     " pathname  points  outside  your" \
                     " accessible address space.", errno );
               break;
            case ENAMETOOLONG:
               throw RangoExcepcion(__AQUI__, cadPath + " pathname was too long.", errno );
               break;
            case ENOENT:
               throw NoExisteExcepcion(__AQUI__, cadPath + " A component in pathname does not" \
                  " exist or is a dangling symbolic link, or pathname is empty.", errno );
               break;
            case ENOTDIR:
               throw NoExisteExcepcion(__AQUI__, cadPath + " A component used as a directory in" \
                  " pathname is not,in fact, a directory.", errno );
               break;
            case ENOMEM:
               throw MemoriaExcepcion(__AQUI__, cadPath + " Insufficient kernel memory was available.", errno );
               break;
            case EROFS:
               throw PermisoExcepcion(__AQUI__, cadPath + " pathname refers to a file on a " \
                  "read-only  filesystem.", errno );
               break;
            case ELOOP:
                  throw NoAplicaExcepcion(__AQUI__, cadPath + " Too many symbolic links were " \
                  "encountered in translating pathname.", errno );
               break;
            case EIO:
                  throw IOExcepcion(__AQUI__, cadPath + " An I/O error occurred.", errno );
               break;
            case EINVAL:
                  throw IOExcepcion(__AQUI__, cadPath + "An error on mmap occurred.", errno );
               break;
            default:
               throw DesconocidoExcepcion(__AQUI__, cadPath + " Error desconocido en unlink.", errno );
               break;
         }
      }
      else
      {
         bReturn = true;
      }
   }
   else
   {
      throw NoAplicaExcepcion ( __AQUI__, "La ruta especificadada no es vacia." );
   }

   return bReturn;
}














//---------------------------------------------------------------------------
// Directorio.cpp
//---------------------------------------------------------------------------
