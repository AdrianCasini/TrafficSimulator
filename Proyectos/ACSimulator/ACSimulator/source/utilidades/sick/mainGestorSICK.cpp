#include "VERSION"


#include "lang/Types.h"
#include "SICK.h"
#include "TareaGestorSICK.h" 

//-----------------------------------------------------------------
/**
 * <PRE>
  *
  * FICHERO: mainGestorSICK.cpp
  *
  * DESCRIPCION:  Gestor del Laser SICK
  *
  * FECHA CREACION: 20/01/2015
  *
  * AUTOR DISENYO: Adrian Casini
  *
  * PROGRAMADOR:   Adrian Casini
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |        |                   |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
  */
//-----------------------------------------------------------------



//-----------------------------------------------------------------
int main (int argc, char* argv[])
{
   USE_VERSION;
   TareaGestorSICK   mTarea;
   Traza mTraza;
   
   try
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK,"********************************************");
      STrz::basic(ConstTraza::T_TRAZA_SICK,"*** Inicio de la Gestion del Laser SICK  ***");
      STrz::basic(ConstTraza::T_TRAZA_SICK,"********************************************");


      mTarea.setIdProceso(Proceso::PROC_GESTOR_SICK);
      mTarea.setTrazable(&mTraza);
      mTarea.inicializar();
      STrz::basic(ConstTraza::T_TRAZA_SICK,"Inicio Proceso Gestor SICK");
      mTarea.setGrupoArranque ( argv , argc );
      mTarea.esperarSincronismo ( );
      mTarea.arranca();


      STrz::basic(ConstTraza::T_TRAZA_SICK,"*******************************************");
      STrz::basic(ConstTraza::T_TRAZA_SICK,"****  Fin de la Gestion del Laser SICK  ***");
      STrz::basic(ConstTraza::T_TRAZA_SICK,"*******************************************");

   }
   catch( Excepcion & e )
   {
      STrz::basic(ConstTraza::T_TRAZA_SICK,"Proceso %s (%d) finalizado por excepcion",argv[0], getpid());
      STrz::debug (ConstTraza::T_TRAZA_SICK, e);
      e.ImprimirStack();
   }
}




//-----------------------------------------------------------------
