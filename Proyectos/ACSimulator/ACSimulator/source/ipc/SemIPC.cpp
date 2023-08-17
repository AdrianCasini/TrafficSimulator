// =======================
//  FICHERO - SemIPC.cpp
// =======================

#include "SemIPC.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////

// ===============   CICLO DE VIDA    ==================
SemIPC::SemIPC()
{
   resetAtributos();
}

// =================   ACCESO       ====================
void SemIPC::setNsems(const SDWordType& sdNsems)
{
   msdNsems = sdNsems;
};

//------------------------------------------------------------------------------
void SemIPC::setValor( const UWordType uwValor, SWordType swSemaforo ) const
{
   UWordType  luwIdx;
   SDWordType lsdCmd; // operacion sobre 1 o todos los semaforos
   UWordType  lauwValores[ MAX_NUM_SEMAFOROS ];
   union semun {
      int val;               /* value for SETVAL */
      struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
      unsigned short *array; /* array for GETALL & SETALL */
      struct seminfo *__buf; /* buffer for IPC_INFO */
      void *__pad;
   } arg;

   if ( TODOS == swSemaforo ) // aplicar sobre todos los semaforos
   {
      for ( luwIdx = 0; luwIdx < getNsems(); luwIdx++ )
         lauwValores[ luwIdx ] = uwValor;
      lsdCmd = SETALL;
      arg.array = lauwValores;
      swSemaforo = 0;
   }
   else // aplicar sobre un semaforo
   {
      lsdCmd = SETVAL;
      arg.val = uwValor;
   }

   if ( FAIL == semctl( getId(), swSemaforo, lsdCmd, arg ) )
   {
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process has no"\
               " access permissions needed to execute", errno);
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "El semaforo ha sido eliminado",
               errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "El descriptor no es valido",
               errno );
         case ERANGE:
           throw RangoExcepcion( __AQUI__, "The  argument  cmd  has  value"\
             " SETALL or SETVAL and the value to which semval has to be set"\
             " (for some semaphore of the set) is less than 0 or greater than"\
             " the implementation value SEMVMX.", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // el valor se asigno correctamente
};

//------------------------------------------------------------------------------
void SemIPC::resetAtributos()
{
   IPC::resetAtributos();
   setNsems( 0 );
}

// =================   CONSULTA      ===================
SDWordType SemIPC::getNsems() const
{
   return msdNsems;
};

//------------------------------------------------------------------------------
SDWordType SemIPC::getValor( const UWordType & uwSemaforo ) const
{
   SDWordType lsdValor;

   lsdValor = semctl( getId(), uwSemaforo, GETVAL);
   if (FAIL == lsdValor)
   {
      switch( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process has no"\
            " access permissions needed to execute", errno);
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "El semaforo ha sido eliminado",
               errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "El descriptor no es valido",
               errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else
      return lsdValor; // el valor se recupero correctamente
};

// =================   OPERACIONES   ===================
void SemIPC::existe (SDWordType lId)
{
   SDWordType lsdId;

   lsdId = semget( lId, 1, ( IPC::USUARIO | IPC::GRUPO ) & ( IPC::LECTURA )  );
   if ( lsdId == FAIL) // la creacion del recurso ha fallado
   {
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "A semaphore set exists for key,"\
               " but the calling process does not have permission to access.",
               errno );
         case EEXIST:
            throw OcupadoExcepcion( __AQUI__, "Se ha intentado crear un"\
               " semaforo ya existente", errno );
         case ENOENT:
            throw NoExisteExcepcion( __AQUI__, "Se ha intentando vincular el"
               " objeto a un semaforo que no existe", errno );
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "El numero de semaforos es"
               " incorrecto", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "A semaphore set has to be"\
               " created but the system has not enough memory for the new data"\
               " structure.", errno );
         case ENOSPC:
            throw LlenoExcepcion( __AQUI__, "Se ha superado el numero maximo de"\
               " semaforos permitidos", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
}
//------------------------------------------------------------------------------
void SemIPC::crear( const SDWordType& sdKey, const SDWordType& sdNSems,
                    const SDWordType& sdFlags )
{
   SDWordType lsdId;

   lsdId = semget( sdKey, sdNSems, sdFlags );
   if ( lsdId == FAIL) // la creacion del recurso ha fallado
   {
      switch ( errno )
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "A semaphore set exists for key,"\
               " but the calling process does not have permission to access.",
               errno );
         case EEXIST:
            throw OcupadoExcepcion( __AQUI__, "Se ha intentado crear un"\
               " semaforo ya existente", errno );
         case ENOENT:
            throw NoExisteExcepcion( __AQUI__, "Se ha intentando vincular el"
               " objeto a un semaforo que no existe", errno );
         case EINVAL:
            throw RangoExcepcion( __AQUI__, "El numero de semaforos es"
               " incorrecto", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "A semaphore set has to be"\
               " created but the system has not enough memory for the new data"\
               " structure.", errno );
         case ENOSPC:
            throw LlenoExcepcion( __AQUI__, "Se ha superado el numero maximo de"\
               " semaforos permitidos", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else // el recurso se ha creado correctamente
   {
      setKey( sdKey );
      setId( lsdId );
      setFlg( sdFlags );
      setNsems( sdNSems );
   }
};

//------------------------------------------------------------------------------
void SemIPC::borrar()
{
   setValor( 0 );
};

//-----------------------------------------------------------------------------
void SemIPC::destruir()
{
   if (FAIL == semctl( getId(), 0, IPC_RMID)) // error borrando el recurso
   {
      switch ( errno )
      {
        case EACCES: // igual que EPERM
        case EPERM:
           throw PermisoExcepcion( __AQUI__, "The calling process has no"\
           " access permissions needed to execute.", errno);
        case EIDRM:
           break;
        case EINVAL:
           throw ParametroExcepcion( __AQUI__, "El descriptor no es valido",
              errno );
        default:
           throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
};

//-----------------------------------------------------------------------------
void SemIPC::wait( const SWordType & swSemaforo, const SDWordType& sdFlags) const
{
   SWordType lswWait;

   lswWait = SemIPC::WAIT;
   operacion( swSemaforo, lswWait, sdFlags);
};

//-----------------------------------------------------------------------------
void SemIPC::signal( const SWordType & swSemaforo, const SDWordType& sdFlags) const
{
   SWordType lswSignal;

   lswSignal = SemIPC::SIGNAL;
   operacion( swSemaforo, lswSignal, sdFlags);
};


////////////////////   PRIVATE      ////////////////////
// =================   OPERACIONES   ===================
void SemIPC::operacion( const SWordType& swSemaforo,
                        const SWordType& swOperacion,
                        const SDWordType& sdFlags) const
{
   struct sembuf lOperaciones[ MAX_NUM_SEMAFOROS ];
   UWordType     luwNumOperaciones;
   UWordType     luwIdx;

   if ( TODOS == swSemaforo ) // aplicar sobre todos los semaforos
   {
      luwNumOperaciones = getNsems();
      for ( luwIdx = 0; luwIdx < luwNumOperaciones; luwIdx++ )
      {
         lOperaciones[ luwIdx ].sem_num = luwIdx;
         lOperaciones[ luwIdx ].sem_op  = swOperacion;
         lOperaciones[ luwIdx ].sem_flg = sdFlags;
      }
   }
   else // aplicar sobre un semaforo
   {
      luwNumOperaciones = 1;
      lOperaciones[ 0 ].sem_num = swSemaforo;
      lOperaciones[ 0 ].sem_op  = swOperacion;
      lOperaciones[ 0 ].sem_flg = sdFlags;
   }
   if ( FAIL == semop(getId(), lOperaciones, luwNumOperaciones ) ) // la operacion ha fallado
   {
      switch (errno)
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process has no"\
            " access permissions on the semaphore set as required by one of"\
            " the specified operations.", errno);
         case EAGAIN:
            throw TimeoutExcepcion( __AQUI__, "La operacion no se ha podido"\
            " realizar en el tiempo especificado", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "La operacion o el timeout son"\
            " incorrectos", errno );
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "El semaforo ha sido eliminado",
              errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "Se ha recibido una senal"\
            " mientras se realizaba una operacion sobre un semaforo", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "El indice del semaforo es"\
            " incorrecto", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "No hay suficiente memoria para"\
            " poder deshacer la operacion", errno );
         case ERANGE:
            throw RangoExcepcion( __AQUI__, "El valor del semaforo es superior"\
             " al maximo permitido", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // la operacion se realizo con exito
};


void SemIPC::operacion( const SWordType& swNumSemaforo, const SWordType swOperaciones[], const SWordType swMaxOperaciones, const SDWordType sdFlags) const
{
   struct sembuf lOperaciones[ swMaxOperaciones ];

   for (SWordType i = 0; i < swMaxOperaciones; i++)
   {
      lOperaciones[ i ].sem_num = swNumSemaforo;
      lOperaciones[ i ].sem_op  = swOperaciones[i];
      lOperaciones[ i ].sem_flg = sdFlags;
   }

   if ( FAIL == semop(getId(), lOperaciones, swMaxOperaciones ) ) // la operacion ha fallado
   {
      switch (errno)
      {
         case EACCES:
            throw PermisoExcepcion( __AQUI__, "The calling process has no"\
            " access permissions on the semaphore set as required by one of"\
            " the specified operations.", errno);
         case EAGAIN:
            throw TimeoutExcepcion( __AQUI__, "La operacion no se ha podido"\
            " realizar en el tiempo especificado", errno );
         case EFAULT:
            throw ParametroExcepcion( __AQUI__, "La operacion o el timeout son"\
            " incorrectos", errno );
         case EIDRM:
            throw NoExisteExcepcion( __AQUI__, "El semaforo ha sido eliminado",
              errno );
         case EINTR:
            throw InterrupcionExcepcion( __AQUI__, "Se ha recibido una senal"\
            " mientras se realizaba una operacion sobre un semaforo", errno );
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "El indice del semaforo es"\
            " incorrecto", errno );
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, "No hay suficiente memoria para"\
            " poder deshacer la operacion", errno );
         case ERANGE:
            throw RangoExcepcion( __AQUI__, "El valor del semaforo es superior"\
             " al maximo permitido", errno );
         default:
            throw DesconocidoExcepcion( __AQUI__, strerror( errno ), errno );
      }
   }
   else; // la operacion se realizo con exito

}

