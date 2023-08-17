// =======================
//  FICHERO - Mutex.cpp
// =======================

#include "Mutex.h"  // clase implementada
#include "../util/STrz.h"
#include "../lang/throwable/NoAplicaExcepcion.h"

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

   Mutex::Mutex()
   {
      this->udwTipo = PTHREAD_MUTEX_RECURSIVE;
      inicializar();
   }

   Mutex::Mutex( UWordType uwTipo )
   {
      this->udwTipo = uwTipo;
      inicializar();
   };

   Mutex::~Mutex()
   {
      try
      {
         destruir ();
      }
      catch (Excepcion& e)
      {
         e.ImprimirStack ();
      }
   }

   void Mutex::inicializar()
   {
      // Initiclizamos el mutex con el siguiente metodo de funcionamiento
      memset(&mhMutex,0,sizeof(pthread_mutex_t));
      switch( pthread_mutexattr_init(&pattr))
      {
         case 0: //SUCCESS
            break;
         case ENOMEM:
            throw MemoriaExcepcion( __AQUI__, strerror (errno), errno );
            break;

         default:
            throw DesconocidoExcepcion( __AQUI__, strerror (errno), errno );
      }

     // Seleccionamos el tipo de mutex
     switch(pthread_mutexattr_settype(&pattr,udwTipo))
     {
       case    0 :    // Respuesta correcta
        break;

       case EINVAL:
        throw ParametroExcepcion(__AQUI__,"kind is neither PTHREAD_MUTEX_FAST_NP "\
             " nor PTHREAD_MUTEX_RECURSIVE_NP nor PTHREAD_MUTEX_ERRORCHECK_NP",errno);
        break;
       default:
        throw DesconocidoExcepcion(__AQUI__,"Error desconocido en settype mutex.",errno);
        break;
    }
     // Creamos el mutex
     switch(pthread_mutex_init(&mhMutex,&pattr))
     {
       case    0 :    // Respuesta correcta
        break;
       default:
        throw DesconocidoExcepcion(__AQUI__,"Error desconocido en mutex init.",errno);
        break;
     }
   }

   void Mutex::destruir()
   {
      pthread_mutexattr_destroy(&pattr);
      switch(pthread_mutex_destroy(&mhMutex))
      {
        case EBUSY:
          throw OcupadoExcepcion(__AQUI__,"the mutex is currently locked.",errno);
          break;
        case    0:    // Respuesta correcta
          break;
        default:
          throw DesconocidoExcepcion(__AQUI__,"Error desconocido en destroy mutex.",errno);
          break;
      }
   }

   void Mutex::lock() const
   {
         // Bloqueamos el mutex sin esperar un tiempo de espera maximo
         switch(pthread_mutex_lock(const_cast<pthread_mutex_t*>(&mhMutex)))
         {
           case EINVAL:
            throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized.",errno);
            break;
           case EDEADLK:
           throw OcupadoExcepcion(__AQUI__,"the mutex is already locked by  the "\
                " calling thread (``error checking'' mutexes only).",errno);
            break;
           case    0  :    // Respuesta correcta
            break;
           default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en lock mutex.",errno);
            break;
         }
   }
   void Mutex::unlock() const
   {
      switch(pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mhMutex)))
      {
        case  EINVAL:
          throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized",errno);
          break;
        case  EPERM:
          throw OcupadoExcepcion(__AQUI__,"the calling thread does not  own  the "\
           " mutex (``error checking'' mutexes only)",errno);
          break;
        case    0  :    // Respuesta correcta
          break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en unlock mutex.",errno);
         break;
      }
   }


   bool Mutex::tryLock()
   {
      UWordType uwRespuesta;

      uwRespuesta = pthread_mutex_trylock(&mhMutex);
      if(EINVAL == uwRespuesta)
      {
          throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized",errno);
      }
      else
      {
        if(EBUSY == uwRespuesta)
          return(true);
        if(0 == uwRespuesta)
          return(false);
        // Tratamos un estado imposible segun documentacion
        throw DesconocidoExcepcion(__AQUI__,"Error desconocido en trylock mutex.",errno);
      }
   }

// =================   CONSULTA      ===================
UDWordType Mutex::getTipoMutex()
{
    return ( this->udwTipo );
}

  pthread_mutex_t*  Mutex::getId()
  {
     return (&this->mhMutex);
  }

// =================   ACCESO       ====================

// =================   OPERADORES    ===================

////////////////////   PROTECTED    /////////////////////
////////////////////   PRIVATE      ////////////////////



    MutexMock::MutexMock()
    {
       // Initiclizamos el mutex con el siguiente metodo de funcionamiento
       memset(&mhMutex,0,sizeof(pthread_mutex_t));
       this->udwTipo = PTHREAD_MUTEX_RECURSIVE;
    }
    MutexMock::MutexMock (const MutexMock &p)
    {
       memcpy(&mhMutex,(pthread_mutex_t*)&p.mhMutex,sizeof(pthread_mutex_t));
       memcpy(&pattr,(pthread_mutexattr_t*)&p.pattr,sizeof(pthread_mutexattr_t));
       this->udwTipo = p.udwTipo;
    }
    MutexMock::~MutexMock()
    {

    }
    void MutexMock::inicializar()
    {
      pthread_mutexattr_init(&pattr);
      // Seleccionamos el tipo de mutex
      switch(pthread_mutexattr_settype(&pattr,udwTipo))
      {
        case EINVAL:
         throw ParametroExcepcion(__AQUI__,"kind is neither PTHREAD_MUTEX_FAST_NP "\
              " nor PTHREAD_MUTEX_RECURSIVE_NP nor PTHREAD_MUTEX_ERRORCHECK_NP",errno);
         break;
        case    0 :    // Respuesta correcta
         break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en settype mutex.",errno);
         break;
     }
      // Creamos el mutex
      switch(pthread_mutex_init(&mhMutex,&pattr))
      {
        case    0 :    // Respuesta correcta
         break;
        default:
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en mutex init.",errno);
         break;
      }
    }
    void MutexMock::destruir()
    {
       pthread_mutexattr_destroy(&pattr);
       switch(pthread_mutex_destroy(&mhMutex))
       {
         case EBUSY:
           throw OcupadoExcepcion(__AQUI__,"the mutex is currently locked.",errno);
           break;
         case    0:    // Respuesta correcta
           break;
         default:
           throw DesconocidoExcepcion(__AQUI__,"Error desconocido en destroy mutex.",errno);
           break;
       }
    }
    void MutexMock::lock() const
    {
       // Bloqueamos el mutex sin esperar un tiempo de espera maximo
       switch(pthread_mutex_lock(const_cast<pthread_mutex_t*>(&mhMutex)))
       {
         case EINVAL:
            STrz::error(ConstTraza::T_TRAZA_SIU, "the mutex has not been properly initialized.", this);
          throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized.",errno);
          break;
         case EDEADLK:
            STrz::error(ConstTraza::T_TRAZA_SIU, "the mutex is already locked by  the", this);
         throw OcupadoExcepcion(__AQUI__,"the mutex is already locked by  the "\
              " calling thread (``error checking'' mutexes only).",errno);
          break;
         case    0  :    // Respuesta correcta
          break;
         default:
            STrz::error(ConstTraza::T_TRAZA_SIU, "Error desconocido en lock mutex.", this);
          throw DesconocidoExcepcion(__AQUI__,"Error desconocido en lock mutex.",errno);
          break;
       }
    }
    void MutexMock::unlock() const
    {
       switch(pthread_mutex_unlock(const_cast<pthread_mutex_t*>(&mhMutex)))
       {
         case  EINVAL:
           throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized",errno);
           break;
         case  EPERM:
           throw OcupadoExcepcion(__AQUI__,"the calling thread does not  own  the "\
            " mutex (``error checking'' mutexes only)",errno);
           break;
         case    0  :    // Respuesta correcta
           break;
         default:
          throw DesconocidoExcepcion(__AQUI__,"Error desconocido en unlock mutex.",errno);
          break;
       }
    }
    bool MutexMock::tryLock()
    {
       UWordType uwRespuesta;

       uwRespuesta = pthread_mutex_trylock(&mhMutex);
       if(EINVAL == uwRespuesta)
       {
           throw ParametroExcepcion(__AQUI__,"the mutex has not been properly initialized",errno);
       }
       else
       {
         if(EBUSY == uwRespuesta)
           return(true);
         if(0 == uwRespuesta)
           return(false);
         // Tratamos un estado imposible segun documentacion
         throw DesconocidoExcepcion(__AQUI__,"Error desconocido en trylock mutex.",errno);
       }
    }
    // =================   CONSULTA      ===================
    UDWordType MutexMock::getTipoMutex()
    {
       return ( this->udwTipo );
    }
    pthread_mutex_t*  MutexMock::getId()
    {
      return (&this->mhMutex);
    }
    // =================   ACCESO       ====================
    void MutexMock::setTipoMutex(UWordType uwTipo)
    {
     this->udwTipo = uwTipo;
    }
// =======================
//  FIN DEL FICHERO - Mutex.cpp
// =======================



