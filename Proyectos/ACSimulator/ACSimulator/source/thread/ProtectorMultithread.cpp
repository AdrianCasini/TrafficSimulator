// =======================
//  FICHERO - ProtectorMultithread.cpp
// =======================

#include "ProtectorMultithread.h"

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
ProtectorMultithread::ProtectorMultithread( const MultithreadSeguro* pMultithreadSeguro,
   SWordType swId )
{
   mswId = swId;
   mpMultithreadSeguro = pMultithreadSeguro;
   if ( NULL!= mpMultithreadSeguro )
      mpMultithreadSeguro->lock( mswId );
};
//------------------------------------------------------------------------------
ProtectorMultithread::~ProtectorMultithread()
{
   if ( NULL != mpMultithreadSeguro )
      mpMultithreadSeguro->unlock( mswId );
   mpMultithreadSeguro = NULL;
};

// =================   OPERADORES    ===================
// =================   OPERACIONES   ===================
// =================   ACCESO       ====================
// =================   CONSULTA      ===================

////////////////////   PROTECTED    /////////////////////

////////////////////   PRIVATE      ////////////////////

// ===============================
//  FIN DEL FICHERO - ProtectorMultithread.cpp
// ===============================
