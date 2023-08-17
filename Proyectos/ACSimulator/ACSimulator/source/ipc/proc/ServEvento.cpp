// =======================
//  FICHERO - ServEvento.cpp
// =======================

#include "ServEvento.h"  // clase implementada

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
ServEvento::ServEvento()
{
  mbInicializado = false;
}

ServEvento::~ServEvento()
{
  try
  {
     if ( true == mbInicializado )
     {
       destruir();
     }
  }
  catch(Excepcion& e)
  {
     // Problemas al destruir la colaIPC
  }
}

// =================   OPERACIONES   ===================
void ServEvento::inicializar(SDWordType sdwId)
{
 if ( false == mbInicializado )
 {
  mColaIPC.crear( sdwId, IPC::CREAR | ( ( IPC::USUARIO | IPC::GRUPO )  & ( IPC::LECTURA | IPC::ESCRITURA ) ) );
  mbInicializado = true;
 }
}

void ServEvento::evento(const Evento& lEvento)
{
 MsjEvento lMsjEvento;
 SDWordType lTmp = IPC::NO_BLOQUEAR;

 lMsjEvento.setEvento(lEvento);
 mColaIPC.escribir(lMsjEvento.getDatos(),lMsjEvento.getLongitud(),lTmp);
}

void ServEvento::destruir()
{
 mColaIPC.destruir();
 mbInicializado = false;
}

// =================   ACCESO       ====================
// =================   CONSULTA      ===================

// =======================
//  FIN DEL FICHERO - ServEvento.cpp
// =======================



