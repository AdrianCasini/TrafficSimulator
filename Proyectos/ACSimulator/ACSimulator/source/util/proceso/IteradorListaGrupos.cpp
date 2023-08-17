// =======================
//  FICHERO - IteradorListaGrupos.cpp
// =======================

#include "IteradorListaGrupos.h"  // clase implementada

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

// =================   OPERACIONES   ===================
UWordType IteradorListaGrupos::getNumProcesos(ProcesoHijo::TiposProcesosTypeE lTipo)
{
  UWordType luwNumero;
  ProcesoHijo *lpProc;

  luwNumero = 0;
  lpProc = getPrimero();
  while (lpProc != NULL)
  {
   if ( lTipo == lpProc->getTipoProc() )
    luwNumero++;
   lpProc = getSiguiente();
  }
  return(luwNumero);
}

ProcesoHijo* IteradorListaGrupos::getPrimero(ProcesoHijo::EstadoProcesoTypeE lEstado)
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if (lEstado == lpProc->getEstadoProc())
     return ( lpProc );
    lpProc = getSiguiente();
  }
  return(NULL);
}

ProcesoHijo* IteradorListaGrupos::getSiguiente(ProcesoHijo::EstadoProcesoTypeE lEstado)
{
  ProcesoHijo *lpProc;

  lpProc = getSiguiente();
  while ( NULL != lpProc )
  {
    if (lEstado == lpProc->getEstadoProc())
     return ( lpProc );
    lpProc = getSiguiente();
  }
  return(NULL);
}

bool IteradorListaGrupos::getProcesosActivos()
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( true == lpProc->getActivo() )
     return(true);
    lpProc = getSiguiente();
  }
  return(false);
}

bool IteradorListaGrupos::getProcesosPorParar()
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( (ProcesoHijo::EST_ARRANCADO == lpProc->getEstadoProc()) ||
     (ProcesoHijo::EST_ARRANCANDO == lpProc->getEstadoProc()) ||
     (ProcesoHijo::EST_RESUCITANDO == lpProc->getEstadoProc()) )
      return(true);
    lpProc = getSiguiente();
  }
  return(false);
}

ProcesoHijo* IteradorListaGrupos::getPrimerActivo()
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( true == lpProc->getActivo() )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return( NULL );
}

ProcesoHijo* IteradorListaGrupos::getSiguienteActivo()
{
  ProcesoHijo *lpProc;

  lpProc = getSiguiente();
  while ( NULL != lpProc )
  {
    if ( true == lpProc->getActivo() )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return( NULL );
}

ProcesoHijo* IteradorListaGrupos::getPrimero(ProcesoHijo::TiposProcesosTypeE lTipo)
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( lTipo == lpProc->getTipoProc() )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return( NULL );
}

ProcesoHijo* IteradorListaGrupos::getSiguiente(ProcesoHijo::TiposProcesosTypeE lTipo)
{
  ProcesoHijo *lpProc;

  lpProc = getSiguiente();
  while ( NULL != lpProc )
  {
    if ( lTipo == lpProc->getTipoProc() )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return( NULL );
}

ProcesoHijo* IteradorListaGrupos::get( SDWordType lswPid )
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
   if ( lswPid == lpProc->getPID() )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return( NULL );
}

UWordType IteradorListaGrupos::getNumParando()
{
  ProcesoHijo *lpProc;
  UWordType luwNumero;

  luwNumero = 0;
  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( ProcesoHijo::EST_PARANDO == lpProc->getEstadoProc() )
     luwNumero++;
    lpProc = getSiguiente();
  }
  return( luwNumero );
}

ProcesoHijo* IteradorListaGrupos::getPrimero()
{
  Grupo *lpGrupo;
  ProcesoHijo *lpProc;
  lpGrupo = mIteradorGrupo.getPrimero();
  if ( lpGrupo != NULL )
  {
    lpGrupo->getIterador(mIteradorProcesos);
    lpProc = mIteradorProcesos.getPrimero();
    if ( NULL != lpProc )
     return ( lpProc );
    else
     return (getSiguiente());
  }
  return ( NULL );
}

ProcesoHijo* IteradorListaGrupos::getSiguiente()
{
  ProcesoHijo *lpProc;
  Grupo *lpGrupo;

  lpProc = mIteradorProcesos.getSiguiente();
  if ( NULL == lpProc )
  {
     lpGrupo = mIteradorGrupo.getSiguiente();
     if ( NULL != lpGrupo )
     {
       lpGrupo->getIterador(mIteradorProcesos);
       lpProc = mIteradorProcesos.getPrimero();
       if ( NULL != lpProc )
        return ( lpProc );
       else
        return (getSiguiente());
     }
     return(NULL);
  }
  return ( lpProc );
}

// =================   ACCESO       ====================
void IteradorListaGrupos::set(DLList<Grupo,ListaGrupos::MAXIMOS_GRUPOS> *lpLista)
{
  mIteradorGrupo.set( lpLista );
}


// =======================
//  FIN DEL FICHERO - IteradorListaGrupos.cpp
// =======================



