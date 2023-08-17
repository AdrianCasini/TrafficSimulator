// =======================
//  FICHERO - Arranque.cpp
// =======================

#include "Arranque.h"  // clase implementada

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

void Arranque::arranca(CtrlGrupos::ModoGrupoTypeE lModo)
{
  ProcesoHijo* lProcHijo;

  STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Arrancamos en modo %s",getLiteral(lModo));
  setGrupoAct( getLista()->getPrimero() );
  setModo( lModo );
  if ( NULL != getGrupoAct() )
  {
    // Buscamos los procesos del modo que se quiere arrancar
    getEventos()->inicioArranque ( getGrupoAct() );
    lProcHijo = primerProceso ();
    if ( NULL != lProcHijo )
    {
     lProcHijo->setEstadoProc( ProcesoHijo::EST_ARRANCANDO );
     getOrdenes()->arranca( lProcHijo );
    }
    else
     checkFinArranqueGrupo(lProcHijo);
  }
}


void Arranque::parado( SDWordType lpid, SDWordType idExit)
{
 ProcesoHijo* lProcHijo;

 lProcHijo = mIteradorGrupos.get ( lpid );
 if ( NULL != lProcHijo )
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso %s parado exit = %d",
    lProcHijo->getNombreProceso().getCadena(),idExit);
   lProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
   lProcHijo->setPID(-1);
   if ( (true == lProcHijo->getProcVia()) || ( false == lProcHijo->getFinaliza()))
   {
     if ( true == lProcHijo->getMaster() )
      getEventos( )->error (lProcHijo);
   }
   else
   {
     if ( ( true == lProcHijo->getMaster() ) && (idExit != 0) )
      getEventos( )->error (lProcHijo);
   }

   if ( false == lProcHijo->getBackground() )
   {
     lProcHijo = siguienteProceso();
     if ( NULL == lProcHijo )
      checkFinArranqueGrupo(lProcHijo);
     else
     {
      lProcHijo->setEstadoProc( ProcesoHijo::EST_ARRANCANDO );
      getOrdenes()->arranca ( lProcHijo );
     }
   }
   else
   {
    checkFinArranqueGrupo(lProcHijo);
   }
 }
}

void Arranque::arrancado ( SDWordType lpid )
{
 ProcesoHijo* lProcHijo;
 ProcesoHijo::EstadoProcesoTypeE lEstAnterior;

 lProcHijo = mIteradorGrupos.get ( lpid );
 if ( NULL != lProcHijo )
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso %s arrancado",
    lProcHijo->getNombreProceso().getCadena(),ProcesoHijo::getLiteral(lProcHijo->getEstadoProc()));
   lEstAnterior = lProcHijo->getEstadoProc();

   if ( false == lProcHijo->getProcVia() )
    lProcHijo->setEstadoProc( ProcesoHijo::EST_ARRANCADO );

   if ( (true == lProcHijo->getBackground()) && (lEstAnterior != ProcesoHijo::EST_RESUCITANDO))
   {
     lProcHijo = siguienteProceso();
     if ( NULL == lProcHijo )
      checkFinArranqueGrupo(lProcHijo);
     else
     {
      lProcHijo->setEstadoProc( ProcesoHijo::EST_ARRANCANDO );
      getOrdenes()->arranca ( lProcHijo );
     }
   }
 }
}

Grupo* Arranque::siguiente ()
{
  ProcesoHijo* lProcHijo;

  setGrupoAct(getLista()->getSiguiente());
  if ( NULL != getGrupoAct() )
  {
    getEventos()->inicioArranque ( getGrupoAct() );
    lProcHijo = primerProceso ();
    if ( NULL != lProcHijo )
    {
     lProcHijo->setEstadoProc( ProcesoHijo::EST_ARRANCANDO );
     getOrdenes()->arranca ( lProcHijo );
    }
    else
     checkFinArranqueGrupo(lProcHijo);
  }
  return ( getGrupoAct() );
}

ProcesoHijo* Arranque::primerProceso ()
{
  ProcesoHijo*  lProcHijo;

  lProcHijo = CtrlGrupos::primerProceso();
  while ( lProcHijo != NULL )
  {
    if ( true == checkProc(lProcHijo) )
     return(lProcHijo);
    lProcHijo = CtrlGrupos::siguienteProceso();
  }
  return(NULL);
}

ProcesoHijo* Arranque::siguienteProceso ()
{
  ProcesoHijo*  lProcHijo;

  lProcHijo = CtrlGrupos::siguienteProceso();
  while ( lProcHijo != NULL )
  {
    if ( true == checkProc(lProcHijo) )
     return(lProcHijo);
    lProcHijo = CtrlGrupos::siguienteProceso();
  }
  return(NULL);
}

bool Arranque::checkProc(ProcesoHijo* lpProc)
{
  switch( getModo() )
  {
   case CtrlGrupos::EXPLOTACION:
    if ( (lpProc->getTipoProc() == ProcesoHijo::PROC_EXPLOTACION ) ||
         (lpProc->getTipoProc() == ProcesoHijo::PROC_SIEMPRE ) )
           return(true);
    break;
   case CtrlGrupos::TEST:
    if ( (lpProc->getTipoProc() == ProcesoHijo::PROC_TEST ) ||
         (lpProc->getTipoProc() == ProcesoHijo::PROC_SIEMPRE ) )
           return(true);
     break;
   default:
    break;
  }
  return(false);
}

void Arranque::checkFinArranqueGrupo(ProcesoHijo* lpProc)
{
  if ( lpProc == NULL )
  {
    // Evento de un proceso que no es de via
    getEventos()->finalArranque ( getGrupoAct() , true);
  }
  else
  {
    if ( true == getGrupoAct()->getProcNoViaParados() )
     getEventos()->finalArranque ( getGrupoAct() , false);
  }
}


// =======================
//  FIN DEL FICHERO - Arranque.cpp
// =======================



