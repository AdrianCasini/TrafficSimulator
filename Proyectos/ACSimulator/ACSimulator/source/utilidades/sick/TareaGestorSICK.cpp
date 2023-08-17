// =======================
//  FICHERO - TareaGestorSICK.cpp
// =======================

#include "TareaGestorSICK.h" 

//------------------------------------------------------------------------------
TareaGestorSICK::TareaGestorSICK()
{

}
//------------------------------------------------------------------------------
TareaGestorSICK::~TareaGestorSICK()
{
}

//------------------------------------------------------------------------------
void TareaGestorSICK::inicializarTarea()
{
   try
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK, "*** Inicio Proceso SICK ****");
      mSick.inicializar();

   }
   catch( Excepcion & e )
   {
      STrz::debug(ConstTraza::T_TRAZA_SICK,"Excepcion en Inicializar Tarea!!!!");
      throw NoExisteExcepcion(__AQUI__,"Error.", errno, &e);
   }
}

//------------------------------------------------------------------------------
void TareaGestorSICK::parse(PUByteType lpsbDatos, UWordType luwLong, SDWordType sdwIdRemitente)
{
}

//----------------------------------------------------------------------------
void TareaGestorSICK::finalizarTarea()
{
   mSick.finalizar();
}


