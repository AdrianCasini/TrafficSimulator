// =======================
//  FICHERO - CtrlGrupos.cpp
// =======================

#include "CtrlGrupos.h"  // clase implementada

// ================================================================
// INICIALIZACIONES ESTATICAS
// ================================================================

CtrlGrupos::ModoGrupoTypeE CtrlGrupos::mModo = CtrlGrupos::SIN_ASIGNAR;
// ================================================================
// DEFINICION DE CLASES INTERNAS
// ================================================================

// ================================================================
// DEFINICIONES DE CLASE
// ================================================================

////////////////////   PUBLIC       ////////////////////

// ===============   CICLO DE VIDA    ==================
CtrlGrupos::CtrlGrupos()
{
  mpOrdenesProceso = NULL;
  mpGrupoActual = NULL;
  mpEventos = NULL;
  mModo = SIN_ASIGNAR;
  mpLista = NULL;
}

CtrlGrupos::~CtrlGrupos()
{

}
// =================   OPERACIONES   ===================
 void CtrlGrupos::arranca(CtrlGrupos::ModoGrupoTypeE lModo)
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Arrancamos en modo %s",getLiteral(lModo));
 }

 Grupo* CtrlGrupos::siguiente ()
 {
   return (NULL);
 }

 void CtrlGrupos::parado( SDWordType lpid, SDWordType idExit)
 {
   ProcesoHijo *lpProcHijo;

   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso Pid: %d parado exit = %d",lpid,idExit);
   lpProcHijo = mIteradorGrupos.get ( lpid );
   if ( NULL != lpProcHijo )
   {
    lpProcHijo->setEstadoProc( ProcesoHijo::EST_PARADO );
    lpProcHijo->setPID(-1);
   }
 }

 void CtrlGrupos::arrancado ( SDWordType lpid )
 {
   STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"Proceso Pid: %d arrancado",lpid);
 }
// =================   ACCESO       ====================
void CtrlGrupos::set(ListaGrupos* lLista)
{
  mpLista = lLista;
  if (NULL != lLista)
   lLista->getIterador(mIteradorGrupos);
}

void CtrlGrupos::set(OrdenesProceso* lOrdenes)
{
  mpOrdenesProceso = lOrdenes;
}

void CtrlGrupos::set(EventosGrupo* lEventos)
{
  mpEventos = lEventos;
}

void CtrlGrupos::setGrupoAct( Grupo* lpGrupo)
{
  mpGrupoActual = lpGrupo;
  if ( NULL != lpGrupo )
   mpGrupoActual->getIterador(mIterador);
}

void CtrlGrupos::setModo(ModoGrupoTypeE lModo)
{
  mModo = lModo;
}

// =================   CONSULTA      ===================
UWordType CtrlGrupos::getNumGrupoActual()
{
  if ( NULL != mpGrupoActual )
   return( (UWordType) mpGrupoActual->getId());
  else
   throw NoAplicaExcepcion(__AQUI__,"No hay grupo asignado");
}

CPSByteType CtrlGrupos::getLiteral(CtrlGrupos::ModoGrupoTypeE lModo)
{
  switch(lModo)
  {
   case EXPLOTACION: return("EXPLOTACION");
   case TEST: return("TEST");
   default: return("DESCONOCIDO");
  }
}

OrdenesProceso* CtrlGrupos::getOrdenes()
{
  if (NULL != mpOrdenesProceso )
   return( mpOrdenesProceso );
  else
   throw MemoriaExcepcion(__AQUI__,"No esta asignado");
}

EventosGrupo* CtrlGrupos::getEventos()
{
  if (NULL != mpEventos )
   return( mpEventos );
  else
   throw MemoriaExcepcion(__AQUI__,"No esta asignado");
}

CtrlGrupos::ModoGrupoTypeE CtrlGrupos::getModo()
{
  return(mModo);
}

ListaGrupos* CtrlGrupos::getLista()
{
  if (NULL != mpLista )
   return( mpLista );
  else
   throw MemoriaExcepcion(__AQUI__,"No esta asignado");
}

ProcesoHijo* CtrlGrupos::primerProceso ()
{
  if ( NULL != mpGrupoActual )
    return (mIterador.getPrimero());
  else
   return (NULL);
}

ProcesoHijo* CtrlGrupos::siguienteProceso ()
{
  if ( NULL != mpGrupoActual )
    return (mIterador.getSiguiente());
  else
   return (NULL);
}

Grupo *CtrlGrupos::getGrupoAct()
{
  return (mpGrupoActual);
}

// =======================
//  FIN DEL FICHERO - CtrlGrupos.cpp
// =======================



