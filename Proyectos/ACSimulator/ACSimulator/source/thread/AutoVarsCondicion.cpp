// =================================
//  FICHERO - AutoVarsCondicion.cpp
// =================================

#include "AutoVarsCondicion.h"


//------------------------------------------------------------------------------
AutoVarsCondicion::AutoVarsCondicion ()
   : VarsCondicion()
{
   VarsCondicion::inicializar ();
}

//------------------------------------------------------------------------------
AutoVarsCondicion::~AutoVarsCondicion ()
{
   try
   {
      VarsCondicion::destruir ();
   }
   catch (Excepcion& e)
   {
      e.ImprimirStack ();
   }
}
