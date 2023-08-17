// =======================
//  FICHERO - VarsCondicion.cpp
// =======================

#include "VarsCondicion.h"  // clase implementada

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

    VarsCondicion::VarsCondicion()
    {
     Timeout.tv_sec = 0;
     Timeout.tv_nsec = 0;
    }

    VarsCondicion:: ~VarsCondicion()
    {

    }

   // =================   OPERADORES    ===================

    void  VarsCondicion::wait(Mutex& lMutex)
    {
     // Realizamos el bloqueo esta funcion no devuelve ningun tipo de error.
      if(0 != pthread_cond_wait(&idVarCon , lMutex.getId()))
         throw DesconocidoExcepcion(__AQUI__, strerror( errno ), errno);
    }

    void VarsCondicion::setTimeout(CUWordType  cuwTimemsg)
    {
     struct timeval Ahora;
     SDWordType     sdwResultado;

     // Leemos el tiempo actual
     sdwResultado = gettimeofday(&Ahora,NULL);
     if(0 != sdwResultado)
     {
       switch(sdwResultado)
       {
         case EPERM:
          throw PermisoExcepcion(__AQUI__,"settimeofday is called by someone"  \
          " other  than  the superuser.",errno);
         case EINVAL:
          throw ParametroExcepcion(__AQUI__,"Timezone (or something else) is invalid.",errno);
          break;
         case EFAULT:
          throw MemoriaExcepcion(__AQUI__,"One  of  tv  or  tz pointed outside y" \
          " our accessible address space.",errno);
          break;
         default:
          throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
          break;
       }
     }
     else
     {
       Timeout.tv_sec = Ahora.tv_sec + (cuwTimemsg / 1000);
       Timeout.tv_nsec = Ahora.tv_usec + ((cuwTimemsg % 1000) * 1000);
       while( Timeout.tv_nsec >= 1000000 )
       {
          Timeout.tv_sec++;
          Timeout.tv_nsec = Timeout.tv_nsec - 1000000;
       }
       Timeout.tv_nsec*=1000;
     }
    }

    void VarsCondicion::waitTime(Mutex& lMutex)
    {
     switch(pthread_cond_timedwait(&idVarCon,lMutex.getId(),&Timeout))
     {
       case EINVAL:
        throw ParametroExcepcion(__AQUI__,"Cond, mutex, or abstime is invalid "\
         "different mutexes for concurrent waits, mutex is not owned by calling thread",errno);
        break;
       case ETIMEDOUT:
         throw TimeoutExcepcion(__AQUI__,"The  condition variable was not"\
          "signaled until the timeout specified by abstime",errno);
         break;
       case EINTR:
         throw InterrupcionExcepcion(__AQUI__,"Pthread_cond_timedwait was" \
          "interrupted by a signal",errno);
         break;
       case   0:    // Respuesta correcta se a realizado un signal de la variable
                    // de condicion.
         break;
       case   1:    // A la espera de un signal o un timeout
         break;
       default:
        throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
        break;
     }
    }

    void VarsCondicion::waitTime (MutexMock& lMutex )
    {
      //STrz::error( ConstTraza::T_TRAZA_SOAP, "waitTime %p", &lMutex );
      switch ( pthread_cond_timedwait( &idVarCon, lMutex.getId(), &Timeout ) ) {
         case EINVAL:
            throw ParametroExcepcion( __AQUI__, "Cond, mutex, or abstime is invalid "
            "different mutexes for concurrent waits, mutex is not owned by calling thread",errno);
            break;
         case ETIMEDOUT:
            throw TimeoutExcepcion(__AQUI__,"The  condition variable was not"
            "signaled until the timeout specified by abstime",errno);
            break;
         case EINTR:
            throw InterrupcionExcepcion(__AQUI__,"Pthread_cond_timedwait was"
            "interrupted by a signal",errno);
            break;
         case 0: // Respuesta correcta se a realizado un signal de la variable
            // de condicion.
            break;
         case 1: // A la espera de un signal o un timeout
            break;
         default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cond_timedwait",errno);
            break;
      }
   }

    void  VarsCondicion::signal()
    {
     // Realizamos el signal esta funcion no devuelve ningun tipo de error.
      if(0 != pthread_cond_signal(&idVarCon))
        throw DesconocidoExcepcion(__AQUI__,"Error en pthread_cond_signal",errno);
    }

    void VarsCondicion::inicializar()
    {
       // Inicializamos la variable de condicion
      if(0 != pthread_cond_init(&idVarCon , NULL) )
       throw DesconocidoExcepcion(__AQUI__,"Error al crear la variable de condicion",errno);
    }

    void VarsCondicion::destruir()
    {
     if ( EBUSY == pthread_cond_destroy(&idVarCon) )
      throw OcupadoExcepcion(__AQUI__," EBUSY  some threads are currently waiting on cond.");
    }

    // =================   ACCESO       ====================

    pthread_cond_t*   VarsCondicion::getId()
    {
     return ( &idVarCon );
    }

// =======================
//  FIN DEL FICHERO - VarsCondicion.cpp
// =======================







