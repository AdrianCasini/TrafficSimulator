// =======================
//  FICHERO - Thread.cpp
// =======================

#include "Thread.h"  // clase implementada
 #include "util/STrz.h"  // clase implementada

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
   Thread::Thread()
   {
      this->midHilo = 0;
      this->mEstado = PARADO;
      mFinalizar = false;
   }

   Thread:: ~Thread()
   {
     if(PARADO !=  this->mEstado)
     {
#ifdef _DEBUG
        try
        {
           char  cmd[64];
           FILE *cmdline = 0;
           sprintf( cmd, "/proc/%d/cmdline", getpid());
           cmdline = fopen( cmd, "r" );
           if( cmdline )
           {
              if( not( fscanf( cmdline, "%s", &( cmd[0] ))))
                 sprintf( cmd, "fscanf?" );
              fclose( cmdline );
           }
           else
              sprintf( cmd, "fopen?" );
           printf( "\nError objeto destruido con Thread arrancado\n"
                   "  |-- proceso\n"
                   "  |     |-- pid = %d\n"
                   "  |     |-- obj = %p\n"
                   "  |     `-- cmdline = %s\n"
                   "  |-- Orden de finalizar\n",
                   getpid(),this, cmd );
#endif
           destruir();
#ifdef _DEBUG
           printf( "  |-- Esperando final\n" );
#endif
           esperarFinalHilo();
#ifdef _DEBUG
           printf( "  `-- FINALIZADO\n" );
       }
       catch(Excepcion& e)
       {
          e.ImprimirStack();
       }
#endif
     }
   }

// =================   OPERADORES    ===================


   void Thread::puenteSalida(void* pThis)
   {
      // Ejecutamos la funcion de salida
      static_cast<Thread *>(pThis)->tareaSalida();
      //static_cast<Thread *>(pThis)->setParado();
      static_cast<Thread *>(pThis)->mEstado=PARADO;
   }

   void *Thread::puenteEntrada(void* pThis)
   {
     try
     {
       // Seleccionamos el estado y el tipo de cancelacion del hilo
        switch(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL))
        {
          case EINVAL:
            throw ParametroExcepcion(__AQUI__,"Cancelacion del Thread no se puede realizar setcancelstate parametro incorrecto.",errno);
          case 0:
            break;
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_setcancelstate.",errno);
            break;
        }

        switch(pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL))
        {
          case EINVAL:
           throw ParametroExcepcion(__AQUI__,"Cancelacion del Thread no se puede realizar el setcanceltype parametro incorrecto.",errno);
          case 0: break;
          default:
           throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_setcanceltype.",errno);
           break;
      }

       // Colocamos la funcion de salida
       pthread_cleanup_push ( puenteSalida , (void *)pThis);
       // Lanzamos el hilo
       static_cast<Thread *>(pThis)->tarea();
       // Ejecutamos la funcion de salida despues de la cancelacion del hilo
       pthread_cleanup_pop(1);
     }
     catch(Excepcion& e)
     {
       // Finalizacion incorrecta del thread o las excepciones no han sido
       // tratadas por la funcion tarea()
       e.ImprimirStack();
     }
     return(NULL);
   }

   void Thread::tareaSalida()
   {
     // En el caso de que la herencia no implemente tareaSalida.
   }

   void Thread::puntoDeCancelacion()
   {
     // Punto o marca de cancelacion del hilo actual
     pthread_testcancel();
   }
   void Thread::esperarFinalHilo()
   {
      if( PARADO != mEstado )
      {
         if(pthread_self() != midHilo)
         {
           switch(pthread_join(midHilo,NULL))
           {
            case  EINVAL:
              throw ParametroExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, parametro incorrecto.",errno);
              break;
            case  EDEADLK:
//              throw OcupadoExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, el hilo referenciado no esta activo.",errno);
              break;
            case  ESRCH:
  //            throw NoExisteExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, el hilo referenciado no existe.",errno);
              break;
            case    0  :    // Respuesta correcta
              mEstado = PARADO;
              break;
            default:
              throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_join.",errno);
              break;
           }
         }
         else
         {
           throw ParametroExcepcion(__AQUI__,"No se puede esperar la finalizacion de un mismo hilo (Abrazo mortal).");
         }
         //mEstado = PARADO;
      }
   }

   void Thread::iniciar()
   {
     if(PARADO == mEstado )
     {
        //  Creamos el thread
        switch( pthread_attr_init(&mAtributos) )
        {
          case 0:    // Respuesta correcta
            break;

          case ENOMEM:
             throw MemoriaExcepcion( __AQUI__, strerror (errno), errno );

          default:
             throw DesconocidoExcepcion( __AQUI__, strerror (errno), errno );
        }

        switch(pthread_attr_setdetachstate(&mAtributos,PTHREAD_CREATE_JOINABLE))
        {
          case EINVAL:
            throw ParametroExcepcion(__AQUI__,"No se puede configurar el hilo, parametro incorrecto.",errno);
            break;;
          case    0  :    // Respuesta correcta
            break;
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_attr_setdetachstate.",errno);
            break;
        }

        switch( pthread_create (&midHilo,&mAtributos,puenteEntrada,(void *)this))
        {
          case EAGAIN:
            throw LlenoExcepcion(__AQUI__,"No se puede crear el hilo, memoria insuficiente o maximo numero de hilos creados.",errno);
            break;
          case EPERM:
            throw LlenoExcepcion(__AQUI__,"the caller does not have appropiate permission to set the required scheduling parameters o scheduling policy.",errno);
            break;
          case EINVAL:
            throw LlenoExcepcion(__AQUI__,"the value specified by attr is invalid.",errno);
            break;
          case    0  :    // Respuesta correcta
            break;
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_create.",errno);
            break;
        }
        mEstado = ARRANCADO;
     }
   }

   void Thread::destruir( bool lbDetach )
   {
      if(PARADO != mEstado)
      {
        // Realizamos una peticion de cancelacion del hilo
      if ( lbDetach )
       pthread_detach(midHilo);
        mEstado = SOLICITUD_CANCELACION;
        switch(pthread_cancel(midHilo))
        {
          case ESRCH:
            //throw NoExisteExcepcion(__AQUI__,"No se puede cancelar el hilo, no se encuentra el hilo identificado.",errno);
            break;
          case    0  :    // Respuesta correcta
            break;
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cancel.",errno);
            break;
        }

        switch( pthread_attr_destroy(&mAtributos) )
        {
          case 0:    // Respuesta correcta
            break;

          default:
             throw DesconocidoExcepcion( __AQUI__, strerror (errno), errno );

        }
      }
   }

void Thread::detach()
{
   if( PARADO == mEstado )
   {
      switch( pthread_detach( midHilo ) )
      {
         case    0  :    // Respuesta correcta
            break;

         case EINVAL:
            throw ParametroExcepcion( __AQUI__, strerror (errno), errno );
            break;

         case ESRCH:
            throw NoExisteExcepcion( __AQUI__, strerror (errno), errno );
            break;

         default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_cancel.",errno);
            break;
      }

      switch( pthread_attr_destroy(&mAtributos) )
      {
         case 0:    // Respuesta correcta
            break;

         default:
            throw DesconocidoExcepcion( __AQUI__, strerror (errno), errno );
      }
   }
}

   bool Thread::comparar() const
   {
     pthread_t  lidHiloLeido;

     if(PARADO != mEstado)
     {
        lidHiloLeido = pthread_self();
        // Comparamos los identificadores de los dos hilos
        if(lidHiloLeido == midHilo)
         return (true);
        else
         return (false);
     }
     return(false);
   }

   void Thread::join(Thread& mTh)
   {
     if(PARADO != mEstado)
     {
        switch(pthread_join(mTh.getId(),NULL))
        {
          case  EINVAL:
            throw ParametroExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, parametro incorrecto.",errno);
            break;
          case  EDEADLK:
            throw OcupadoExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, el hilo referenciado no esta activo.",errno);
            break;
          case  ESRCH:
            //throw NoExisteExcepcion(__AQUI__,"Error no se puede esperar la finalizacion del hilo, el hilo referenciado no existe.",errno);
            break;
          case    0  :    // Respuesta correcta
            break;
          default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_join.",errno);
            break;
        }
      }
      else
      {
         throw NoAplicaExcepcion(__AQUI__,"No se puede realizar un join con estado PARADO");
      }
   }

   void Thread::anularCancelacion() const
   {
     // Seleccionamos el estado y el tipo de cancelacion del hilo
      switch(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL))
      {
        case EINVAL:
          throw ParametroExcepcion(__AQUI__,"Cancelacion del Thread no se puede realizar setcancelstate parametro incorrecto.",errno);
        case 0:
          break;
        default:
          throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_setcancelstate.",errno);
          break;
      }
  }

   void Thread::permitirCancelacion() const
   {
     // Seleccionamos el estado y el tipo de cancelacion del hilo
         switch(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL))
         {
           case EINVAL:
             throw ParametroExcepcion(__AQUI__,"Cancelacion del Thread no se puede realizar setcancelstate parametro incorrecto.",errno);
           case 0:
             break;
           default:
             throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_setcancelstate.",errno);
             break;
         }

         switch(pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL))
         {
           case EINVAL:
            throw ParametroExcepcion(__AQUI__,"Cancelacion del Thread no se puede realizar el setcanceltype parametro incorrecto.",errno);
           case 0: break;
           default:
            throw DesconocidoExcepcion(__AQUI__,"Error desconocido en pthread_setcanceltype.",errno);
            break;
       }
   }


// =================   ACCESO       ====================
   void Thread::setParado()
   {
      if(true == comparar() )
      {
         if(SOLICITUD_CANCELACION == mEstado)
         {
               // Solo maracmos el estado parado desde
               // el mismo hilo en caso contrario
               // ignoramos la orden
               this->mEstado = PARADO;
         }
         else
         {
               if(PARADO != mEstado)
               {
                  // Salida forzada desde el mismo hilo,
                  // y si no estamos ya parados realizamos un exit
                  this->mEstado = PARADO;
               }
         }
      }
   }
// =================   CONSULTA      ===================

   pthread_t Thread::getId() const
   {
      return(midHilo);
   }

   bool Thread::isFinalizado(void)
   {
      return(PARADO ==  this->mEstado);
   }

   //-----------------------------------------------------------------------------
   void Thread::requestFinalizar()
   {
      MutexLocker mLocker( mMutexFinalizar );
      mFinalizar = true;
   };

   //-----------------------------------------------------------------------------
   bool Thread::queryFinalizar()
   {
      MutexLocker mLocker( mMutexFinalizar );
      return mFinalizar;
   };

   //-----------------------------------------------------------------------------
   void Thread::finalizar()
   {
      destruir();
      esperarFinalHilo();
   };

////////////////////   PROTECTED    /////////////////////
////////////////////   PRIVATE      ////////////////////

// =======================
//  FIN DEL FICHERO - Thread.cpp
// =======================







