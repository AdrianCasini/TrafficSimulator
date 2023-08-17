// =======================
//  FICHERO - ListaGrupos.cpp
// =======================

#include "ListaGrupos.h"  // clase implementada
#include "IteradorListaGrupos.h"
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
ListaGrupos::ListaGrupos()
{
  mIterador.set(&mGrupos);
  mpGrupoAct = NULL;
}

ListaGrupos::~ListaGrupos()
{

}
// =================   OPERACIONES   ===================
void ListaGrupos::add(ProcesoHijo& lProceso)
{
  Grupo lGrupo;
  Grupo *lpGrupo;

  if ( lProceso.getGrupoProcesos() < MAXIMOS_GRUPOS )
  {
    lpGrupo = mGrupos.getObject((SDWordType)lProceso.getGrupoProcesos());
    if ( NULL == lpGrupo )
    {
      // Grupo nuevo lo insertamos en la lista
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Nuevo grupo ID [%d]",lProceso.getGrupoProcesos());
     mGrupos.add( lGrupo,lProceso.getGrupoProcesos());
     lpGrupo = mGrupos.getObject((SDWordType)lProceso.getGrupoProcesos());
     if ( NULL != lpGrupo )
     {
      lpGrupo->setId(lProceso.getGrupoProcesos());
      lpGrupo->add(lProceso);
     }
     else
     {
       STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Error al crear grupo ID [%d]",lProceso.getGrupoProcesos());
     }
    }
    else
    {
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Add procesos al grupo ID [%d]",
      lProceso.getGrupoProcesos());
      // Ya existe el grupo
      lpGrupo->add ( lProceso );
    }
  }
  else
  {
    STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Error anadir grupo");
    throw RangoExcepcion(__AQUI__,"Numero de grupo no permitido");
  }
}
// =================   ACCESO       ====================
// =================   CONSULTA      ===================
Grupo* ListaGrupos::getPrimero()
{
  mpGrupoAct = mIterador.getPrimero();
  return(mpGrupoAct);
}

Grupo* ListaGrupos::getSiguiente()
{
  mpGrupoAct = mIterador.getSiguiente();
  return(mpGrupoAct);
}

UWordType ListaGrupos::getNumGrupos()
{
 return(mGrupos.getNumItems());
}

UWordType ListaGrupos::getNumProcesos()
{
  IteradorDLL<Grupo,MAXIMOS_GRUPOS> lIterador;
  Grupo *lpGrupo;
  UWordType luwNumero;

  lIterador.set(&mGrupos);
  lpGrupo = lIterador.getPrimero();
  luwNumero = 0;
  while ( NULL != lpGrupo )
  {
    luwNumero = luwNumero + lpGrupo->getNum();
    lpGrupo = lIterador.getSiguiente();
  }
  return(luwNumero);
}

Grupo* ListaGrupos::getGrupoAct()
{
  return( mpGrupoAct );
}

bool ListaGrupos::getArrancado()
{
  if ( NULL != mpGrupoAct )
   return (!mpGrupoAct->getArrancando());
  else
   return (false);
}

void ListaGrupos::toString()
{
 ProcesoHijo *lpProc;
 Grupo *lpGrupo;
 IteradorDLL<Grupo,MAXIMOS_GRUPOS> lIteradorGrupo;
 IteradorDLL<ProcesoHijo,Grupo::MAXIMOS_PROCESOS_HIJO> lIteradorProcesos;

 lIteradorGrupo.set(&mGrupos);
 STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Listado de grupos");
 lpGrupo = lIteradorGrupo.getPrimero();
 while ( NULL != lpGrupo )
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Grupo [%d]",lpGrupo->getId());
   lpGrupo->getIterador(lIteradorProcesos);
   lpProc = lIteradorProcesos.getPrimero();
   while ( NULL != lpProc )
   {
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"|-Proceso: [%s] id[%d]",
           lpProc->getNombreProceso().getCadena(),lpProc->getIdProc());
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"|--Tipo:       [%c]",lpProc->getTipoProceso());
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"|--Background: [%d]",lpProc->getBackground());
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"|--Num parada: [%d] espera %d msg",
      lpProc->getOrdenParada(),lpProc->getTiempoEsperaParada());
     STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"|--Ejecucion:  [%s]",
      ProcesoHijo::getTexto(lpProc->getTipoProc()));
     lpProc = lIteradorProcesos.getSiguiente();
   }
   lpGrupo = lIteradorGrupo.getSiguiente();
 }
}

void ListaGrupos::getIterador(IteradorListaGrupos& lIterador)
{
  lIterador.set(&mGrupos);
}

// =======================
//  FIN DEL FICHERO - ListaGrupos.cpp
// =======================



