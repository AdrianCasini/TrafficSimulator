 // =======================
//  FICHERO - MsjEvento.cpp
// =======================

#include "MsjEvento.h"  // clase implementada

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
MsjEvento::MsjEvento()
{
  mpEjecutorEvento = NULL;
  mpDatos = (struct eventoTypeS*)msbBuffer;
}

MsjEvento::~MsjEvento()
{

}
// =================   OPERACIONES   ===================
void MsjEvento::parse(CPSByteType lpsbDatos,SDWordType lsdwLong)
{
  struct eventoTypeS* lpDatos = (struct eventoTypeS*)lpsbDatos;
  if (lsdwLong >= (SDWordType)sizeof(struct eventoTypeS) )
  {
   if ( mpEjecutorEvento != NULL )
   {
    mEvento.setId(lpDatos->lsdwTipo);
    mEvento.setDato(getInicioDatos(lpsbDatos),lpDatos->lubLong);
    mpEjecutorEvento->evento(mEvento);
   }
   else
   {
     throw  NoAplicaExcepcion(__AQUI__,"No esta asignado el ejecutor de los"\
           " metodos definidos en MsjEvento.");
   }
  }
  else
  {
    throw FormatoExcepcion(__AQUI__,"El formato del mensaje es incorrecto");
  }
}

void MsjEvento::setEvento(const Evento& lEvento)
{
  mpDatos->lsdwTipo = lEvento.getId();
  mpDatos->lubLong = lEvento.getLong();
  lEvento.copyDatos(getInicioDatos((PSByteType)mpDatos),mpDatos->lubLong);
}

UWordType MsjEvento::getLongitud()
{
  return(sizeof(struct eventoTypeS) + mpDatos->lubLong);
}

void MsjEvento::setEjecutor(EjecutorEvento *lpEjecutor)
{
  mpEjecutorEvento = lpEjecutor;
}

PSByteType MsjEvento::getInicioDatos(PSByteType lpsbDatos)
{
  return(lpsbDatos + sizeof(struct eventoTypeS));
}

CPSByteType MsjEvento::getInicioDatos(CPSByteType lpsbDatos)
{
  return(lpsbDatos + sizeof(struct eventoTypeS));
}

CPSByteType MsjEvento::getDatos()
{
 return(msbBuffer);
}

// =======================
//  FIN DEL FICHERO - MsjEvento.cpp
// =======================



