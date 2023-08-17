// =======================
//  FICHERO - Grupo.cpp
// =======================

#include "Grupo.h"  // clase implementada

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
Grupo::Grupo()
{
  mbArrancado = false;
  mswNumero = -1;
  mIterador.set(&mLista);
}

Grupo::~Grupo()
{

}
// =================   OPERACIONES   ===================
void Grupo::add(ProcesoHijo& lpProceso)
{
  mLista.add(lpProceso,0);
}
// =================   ACCESO       ====================
void Grupo::setId(SWordType lswNumero)
{
  mswNumero = lswNumero;
}

void Grupo::setArrancado ( bool lbArrancado )
{
 mbArrancado = lbArrancado;
}

// =================   CONSULTA      ===================
SWordType Grupo::getNum()
{
  return(mLista.getNumItems());
}

SWordType Grupo::getId()
{
 return(mswNumero);
}

ProcesoHijo* Grupo::getPrimero()
{
 return ( mIterador.getPrimero() );
}

ProcesoHijo* Grupo::getSiguiente()
{
 return ( mIterador.getSiguiente() );
}

ProcesoHijo* Grupo::getPrimero(ProcesoHijo::EstadoProcesoTypeE lEstado)
{
  ProcesoHijo *lpProc;

  lpProc = getPrimero();
  while ( NULL != lpProc )
  {
    if ( lpProc->getEstadoProc() == lEstado )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return(NULL);
}

ProcesoHijo* Grupo::getSiguiente(ProcesoHijo::EstadoProcesoTypeE lEstado)
{
  ProcesoHijo *lpProc;

  lpProc = getSiguiente();
  while ( NULL != lpProc )
  {
    if ( lpProc->getEstadoProc() == lEstado )
     return(lpProc);
    lpProc = getSiguiente();
  }
  return(NULL);
}

bool Grupo::getArrancado()
{
  return (mbArrancado);
}

bool Grupo::getArrancando()
{
  ProcesoHijo *lpProc;
  IteradorDLL<ProcesoHijo,MAXIMOS_PROCESOS_HIJO> lIterador;

  getIterador(lIterador);
  lpProc = lIterador.getPrimero();
  while ( lpProc != NULL )
  {
   if ( lpProc->getEstadoProc() == ProcesoHijo::EST_ARRANCANDO )
    return ( true );
   lpProc = lIterador.getSiguiente();
  }
  return ( false );
}

bool Grupo::getProcNoViaParados()
{
  ProcesoHijo *lpProc;
  IteradorDLL<ProcesoHijo,MAXIMOS_PROCESOS_HIJO> lIterador;

  getIterador(lIterador);
  lpProc = lIterador.getPrimero();
  while ( lpProc != NULL )
  {
   if ( (lpProc->getEstadoProc() == ProcesoHijo::EST_ARRANCADO) &&
        (false == lpProc->getProcVia()) &&  (true == lpProc->getFinaliza()))
         return ( false );
   lpProc = lIterador.getSiguiente();
  }
  return ( true );
}

void Grupo::activar ( SemIPC& lSemaforo )
{
  ProcesoHijo* lProcHijo;
  IteradorDLL<ProcesoHijo,MAXIMOS_PROCESOS_HIJO> lIterador;

  getIterador(lIterador);
  lProcHijo = lIterador.getPrimero();
  while ( NULL != lProcHijo )
  {
    if ( true == lProcHijo->getActivo() )
     lSemaforo.signal( lProcHijo->getIdSemaforo() ,false);
    lProcHijo = lIterador.getSiguiente();
  }
}

void Grupo::getIterador(IteradorDLL<ProcesoHijo,Grupo::MAXIMOS_PROCESOS_HIJO> &lIterador)
{
  lIterador.set(&mLista);
}

// =======================
//  FIN DEL FICHERO - Grupo.cpp
// =======================



