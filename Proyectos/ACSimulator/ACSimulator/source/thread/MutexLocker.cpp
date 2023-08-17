// =======================
//  FICHERO - MutexLocker.cpp
// =======================

#include "MutexLocker.h"  // clase implementada

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

  MutexLocker::MutexLocker(Mutex& lMutex)
  {
     mpMutex = &lMutex;
     mpMutex->lock();
  }

  MutexLocker::MutexLocker(const Mutex& lMutex)
  {
     mpMutex = const_cast< Mutex * >( &lMutex );
     mpMutex->lock();
  }

  MutexLocker::MutexLocker(Mutex* lpMutex)
  {
     mpMutex = lpMutex;
     if ( mpMutex )
        mpMutex->lock();
  };

  MutexLocker:: ~MutexLocker()
  {
     if ( mpMutex )
        mpMutex->unlock();
  }


// =======================
//  FIN DEL FICHERO - MutexLocker.cpp
// =======================







