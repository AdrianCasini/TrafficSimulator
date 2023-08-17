// =======================
//  FICHERO - Semaforo.cpp
// =======================

#include "Semaforo.h"  // clase implementada

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

   Semaforo::Semaforo(UDWordType uwVal)
      : uwContador(uwVal), uwTipoSem(SEMAFORO_BINARIO)
   {

   }

   Semaforo:: ~Semaforo()
   {

   }

   void Semaforo::setTipoSemaforo(UWordType uwTipo)
   {
     switch ( uwTipo )
     {
        case SEMAFORO_BINARIO:
        case SEMAFORO_INCREMENTAL:
         this->uwTipoSem = uwTipo;
         break;
        default:
         throw ParametroExcepcion(__AQUI__,"Parametro erroneo en setTipoSemaforo.");
     }
   }

   void  Semaforo::wait( bool bForzar )
   {
     MutexLocker lProteccion ( mMutex );

     switch ( uwTipoSem )
     {
       case SEMAFORO_BINARIO:
        if ( ( 0 == uwContador ) || ( true == bForzar ) )
          VarsCondicion::wait(mMutex);
        uwContador = 0;
        break;
       case SEMAFORO_INCREMENTAL:
        if ( uwContador > 0 )
          uwContador--;
         else
         {
           VarsCondicion::wait(mMutex);
           uwContador--;
         }
        break;
     }
   }


   void  Semaforo::signal()
   {
     MutexLocker lProteccion ( mMutex );

     switch ( uwTipoSem )
     {
       case SEMAFORO_BINARIO:
          uwContador = 1;
          break;
       case SEMAFORO_INCREMENTAL:
         if( UWordTypeMax == uwContador)
           throw RangoExcepcion (__AQUI__,"El semaforo tiene el maximo numero"\
           " de signals");
          uwContador++;
          break;
     }
     VarsCondicion::signal();

   }

   void Semaforo::inicializar()
   {
     try
     {
        // Inicializamos la varialbe de condicion
        VarsCondicion::inicializar();
     }
     catch(Excepcion& e)
     {
        throw ParametroExcepcion(__AQUI__,"No se puede crear el Semaforo.",-1,&e);
     }
     // Inicializamos la variable de condicion
     pthread_cond_init(getId() , NULL);
   }

   void Semaforo::destruir()
   {
     VarsCondicion::destruir();
   }

// =================   OPERADORES    ===================
// =================   ACCESO       ====================
// =================   CONSULTA      ===================

////////////////////   PROTECTED    /////////////////////
////////////////////   PRIVATE      ////////////////////

// =======================
//  FIN DEL FICHERO - Semaforo.cpp
// =======================







