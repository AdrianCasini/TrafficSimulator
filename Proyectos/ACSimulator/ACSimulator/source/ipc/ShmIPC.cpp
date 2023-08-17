#include "ShmIPC.h"

////////////////////   PUBLIC       ////////////////////
// =================   CICLO DE VIDA    ================
ShmIPC::ShmIPC()
{
   resetAtributos();
}

// =================   OPERACIONES   ===================
bool ShmIPC::existe(const SDWordType& sdKey, const SDWordType& sdSize) const
{
   SDWordType lsdId;

   lsdId = shmget( sdKey, sdSize, ( IPC::USUARIO | IPC::GRUPO ) & ( IPC::LECTURA ) );
   if (  FAIL == lsdId ) // la creacion del recurso ha fallado
   {
      switch ( errno )
      {
         case EACCES:
            /*
            throw PermisoExcepcion( __AQUI__, "The user does not have"\
            " permission to access the shared memory segment.", errno );
            */
            return(false);
         break;
         case EEXIST:
            throw OcupadoExcepcion( __AQUI__, "IPC_CREAT | IPC_EXCL was"\
            " specified and the segment exists.", errno );
         case ENOENT:
            /*
            throw NoExisteExcepcion( __AQUI__, "No segment exists for the given"\
            " key, and IPC_CREAT was not specified.", errno );
            */
            return(false);
         break;
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "A new segment was to be created"
            " and size < SHMMIN or size > SHMMAX, or no new segment was to be"\
            " created, a segment with given key existed, but size is greater"\
            " than the size  of that segment.", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "No memory could be allocated"\
            " for segment overhead.", errno );
         case ENOSPC:
            throw LlenoExcepcion( __AQUI__, "All possible shared memory id's"\
            " have been taken (SHMMNI) or if allocating a segment of the"\
            " requested size would cause the system to exceed the system-wide"\
            " limit on shared memory (SHMALL).", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
  return(true);
}




// =================   OPERACIONES   ===================
void ShmIPC::crear(const SDWordType& sdKey, const SDWordType& sdSize,
                   const SDWordType& sdFlags)
{
   SDWordType lsdId;

   //printf("******* Key:<0x%X>\tSize:<%d>\tFlags:<0%o>\n", sdKey, sdSize, sdFlags );
   lsdId = shmget( sdKey, sdSize, sdFlags );
   if (  FAIL == lsdId ) // la creacion del recurso ha fallado
   {
      //printf("******* Errno:<%d>\n", errno);
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The user does not have"\
            " permission to access the shared memory segment.", errno );
         case EEXIST:
            throw OcupadoExcepcion( __AQUI__, "IPC_CREAT | IPC_EXCL was"\
            " specified and the segment exists.", errno );
         case ENOENT:
            throw NoExisteExcepcion( __AQUI__, "No segment exists for the given"\
            " key, and IPC_CREAT was not specified.", errno );
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "A new segment was to be created"
            " and size < SHMMIN or size > SHMMAX, or no new segment was to be"\
            " created, a segment with given key existed, but size is greater"\
            " than the size  of that segment.", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "No memory could be allocated"\
            " for segment overhead.", errno );
         case ENOSPC:
            throw LlenoExcepcion( __AQUI__, "All possible shared memory id's"\
            " have been taken (SHMMNI) or if allocating a segment of the"\
            " requested size would cause the system to exceed the system-wide"\
            " limit on shared memory (SHMALL).", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // el recurso se ha creado correctamente
   {
      setKey( sdKey );
      setId( lsdId );
      setFlg( sdFlags );
      setSize( sdSize );
   }
};

//------------------------------------------------------------------------------
void ShmIPC::borrar() const
{
   void *lpAddr;

   lpAddr = getAddr();
   if (NULL != lpAddr)
      memset( lpAddr, 0, getSize() );
   else; // no esta vinculado a ningun recurso
};

//------------------------------------------------------------------------------
void ShmIPC::destruir()
{
   if (FAIL == shmctl( getId(), IPC_RMID, 0 ) ) // error borrando el recurso
   {
      switch ( errno )
      {
         case EPERM:
            throw PermisoExcepcion( __AQUI__, "The effective user ID of the"\
            " calling process is not the creator (as found in shm_perm.cuid),"\
            " the owner (as found in shm_perm.uid), or the super-user.", errno );
         case EIDRM:
           break;
         case EINVAL:
            throw NoExisteExcepcion( __AQUI__, "shmid is not a valid identifier.",
            errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror(errno), errno );
      }
   }
};

//------------------------------------------------------------------------------
void ShmIPC::vincular(const SDWordType& sdFlags)
{
   void* lpAddr; // direccion de inicio de la memoria compartida

   // Dejamos que el sistema proponga la direccion de inicio de la memoria
   lpAddr = shmat(getId(), 0, sdFlags);
   if (lpAddr == NULL) // no se ha podido obtener la direccion de la memoria
   {
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The  calling  process has no"\
            " access permissions for the requested attach type.", errno );
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "Invalid shmid value, unaligned"\
            " (i.e., not page-aligned and SHM_RND was not specified) or"\
            " invalid shmaddr value, or failing attach at brk, or SHM_REMAP"\
            " was specified and shmaddr was NULL.", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "Could not allocate memory for"\
            " the descriptor or for the page tables.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else  // la memoria compartida se ha viculado correctamente
      setAddr( lpAddr );
}

//------------------------------------------------------------------------------
void ShmIPC::desVincular() const
{
   if ( FAIL == shmdt( (char*)getAddr() ) ) // no se ha podido desvincular la memoria compartida
   {
      switch (errno )
      {
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "There is no shared memory segment"\
            " attached at shmaddr.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // la memoria se ha desvinculado correctamente
};

// =================   ACCESO       ====================
void ShmIPC::setSize(const SDWordType& sdSize)
{
   msdSize = sdSize;
}

//------------------------------------------------------------------------------
void ShmIPC::setAddr(void* pAddr)
{
   mpAddr = (void*) pAddr;
}

// =================   CONSULTA      ===================
SDWordType ShmIPC::getSize() const
{
   return msdSize;
}

//------------------------------------------------------------------------------
void* ShmIPC::getAddr() const
{
   if ( NULL == mpAddr )
   {
      throw NoExisteExcepcion( __AQUI__, "La direccion de la memoria compartida"\
      " es NULL." );
   }
   else
      return mpAddr;
}

////////////////////   PROTECTED    ////////////////////
// =================   ACCESO       ====================
void ShmIPC::resetAtributos()
{
   IPC::resetAtributos();
   setAddr ( NULL );
   setSize ( 0 ) ;
}



