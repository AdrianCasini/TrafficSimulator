// =======================
//  FICHERO - IPC.cpp
// =======================

#include "IPC.h"

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

// Main configuration file executed by the kernel on startup
const SByteType IPC::ARCHIVO_FTOK[] = "/sbin/init";

////////////////////   PUBLIC       ////////////////////
// ===============   CICLO DE VIDA    ==================
IPC::IPC()
{
   resetAtributos();
}


// ===============   ACCESO   ==========================}
void IPC::setFlg( const SDWordType& sdFlg )
{
   msdFlg = sdFlg;
}

//-----------------------------------------------------------------------------
void IPC::setId( const SDWordType& sdId )
{
   msdId = sdId;
}

//-----------------------------------------------------------------------------
void IPC::setKey( const SDWordType & sdKey )
{
   msdKey = sdKey;
};

//-----------------------------------------------------------------------------
void IPC::resetAtributos()
{
   setKey( 0 );
   setId( 0 );
   setFlg( 0 );
};

// ===============   OPERACIONES  ======================
SDWordType IPC::generaClave(const UWordType& uwIdentificador,
                                                   CPSByteType lcpsbFichero)
{
   SDWordType lsdKey;
   Cadena mTexto;

#ifdef _IPCPRIVADO
   mTexto = "touch /var/tmp/ipcprivate";
   mTexto += (UWordType)getuid();
   system(mTexto.getCadena());
   mTexto = "/var/tmp/ipcprivate";
   mTexto += (UWordType)getuid();
   lsdKey = ftok(mTexto.getCadena() , uwIdentificador );
#else
   if  (lcpsbFichero == NULL)
      lsdKey = ftok( ARCHIVO_FTOK, uwIdentificador );
   else
      lsdKey = ftok( lcpsbFichero, uwIdentificador );
#endif

   if ( lsdKey == FAIL )
   {
      switch (errno)
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "Permission denied.", errno);
         case ENOENT:
            throw NoExisteExcepcion( __AQUI__, "A  component  of the path"\
               " file_name does not exist, or the path is an empty string.",
               errno );
         case ELOOP:
            throw FormatoExcepcion( __AQUI__, "Too many symbolic links"\
               " encountered while  traversing the path", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "Out of memory.", errno );
         case ENAMETOOLONG:
            throw RangoExcepcion( __AQUI__, "File name too long.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // La clave se ha generado correctamente
      return lsdKey;
}
