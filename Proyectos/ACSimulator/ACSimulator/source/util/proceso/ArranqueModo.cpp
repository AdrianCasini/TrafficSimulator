// =======================
//  FICHERO - ArranqueModo.cpp
// =======================

#include "ArranqueModo.h"  // clase implementada

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

bool ArranqueModo::checkProc(ProcesoHijo* lpProc)
{
  switch( getModo() )
  {
   case CtrlGrupos::EXPLOTACION:
    if ( lpProc->getTipoProc() == ProcesoHijo::PROC_EXPLOTACION )
           return(true);
    break;
   case CtrlGrupos::TEST:
    if ( lpProc->getTipoProc() == ProcesoHijo::PROC_TEST )
           return(true);
     break;
   default:
    break;
  }
  return(false);
}

// =======================
//  FIN DEL FICHERO - ArranqueModo.cpp
// =======================



