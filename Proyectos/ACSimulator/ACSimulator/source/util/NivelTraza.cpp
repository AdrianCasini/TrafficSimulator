// =======================
//  FICHERO - NivelTraza.cpp
// =======================

#include "NivelTraza.h"  // clase implementada

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
// =================   OPERADORES    ===================

////////////////////   PROTECTED    /////////////////////
////////////////////   PRIVATE      ////////////////////

// =================   ACCESO       ====================
// =================   CONSULTA      ===================
// =================   OPERACIONES   ===================
NivelTraza::NivelTrazaTypeE NivelTraza::getNivel(const Cadena& lTexto)
{
  if ( lTexto == "DEBUG") return(TRAZAR_DEBUG);
  if ( lTexto == "BASIC") return(TRAZAR_BASIC);
  if ( lTexto == "NO") return(TRAZAR_NO);
  else  return(DESCONOCIDO);
}

CPSByteType NivelTraza::getLiteral(NivelTrazaTypeE lNivel)
{
  switch(lNivel)
  {
   case DESCONOCIDO: return("Desconocido");
   case TRAZAR_NO: return("NO");
   case TRAZAR_BASIC: return("BASIC");
   case TRAZAR_DEBUG: return("DEBUG");
   default: return("DESCONOCIDO");
  }
}

// =======================
//  FIN DEL FICHERO - NivelTraza.cpp
// =======================

