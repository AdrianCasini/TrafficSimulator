// =======================
//  FICHERO - Marcha.cpp
// =======================

#include "Marcha.h"  // clase implementada

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

void Marcha::parado( SDWordType lpid, SDWordType idExit)
{
 ProcesoHijo* lProcHijo;
 STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso con pid %d parado exit %d",lpid,idExit);
 lProcHijo = mIteradorGrupos.get ( lpid );
 if ( NULL != lProcHijo )
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso %s parado",
    lProcHijo->getNombreProceso().getCadena());
   switch (lProcHijo->getEstadoProc())
   {
     case ProcesoHijo::EST_PARANDO:
     case ProcesoHijo::EST_PARADO:
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
       "Final por orden del Gestor de Procesos.");
      lProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
      lProcHijo->setPID(-1);
      lProcHijo->setHoraMuerte();
      break;
     case ProcesoHijo::EST_RESUCITANDO:
      lProcHijo->setHoraMuerte();
      lProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
      lProcHijo->setPID(-1);
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
       "Final del proceso mientras se rearrancaba",lpid);
      break;
     case  ProcesoHijo::EST_ARRANCADO:
      lProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
      lProcHijo->setPID(-1);
      if ( (true == lProcHijo->getProcVia()) || ( false == lProcHijo->getFinaliza()) )
      {
        STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
         "Final del proceso %d de manera espontanea.",lpid);
        if ( true == lProcHijo->getMaster() )
         getEventos( )->error (lProcHijo);
      }
      else
      {
        STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
         "Final del proceso %d",lpid);
        if ( ( true == lProcHijo->getMaster()) && (idExit != 0) )
         getEventos( )->error (lProcHijo);
      }
      break;
     default:
      lProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
      lProcHijo->setPID(-1);
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
       "Final del proceso %d con estado %d",lpid,lProcHijo->getEstadoProc());
      break;
   }
 }
 else
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso con pid %d desconocido",lpid);
 }
}




// =======================
//  FIN DEL FICHERO - Marcha.cpp
// =======================



