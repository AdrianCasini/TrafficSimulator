// =======================
//  FICHERO - ObjObservadorGenerico.cpp
// =======================

#include "ObjObservadorGenerico.h"  // clase implementada

#include "util/Timer.h"
#include "util/STrz.h"

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
void ObjObservadorGenerico::inicializar(const SDWordType lsdwClave)
{
  msdwId = lsdwClave;
}

void ObjObservadorGenerico::iniciar()
{
   mColaIPC.crear(msdwId, IPC::CREAR | ( ( IPC::USUARIO | IPC::GRUPO )  & ( IPC::LECTURA | IPC::ESCRITURA ) ));
   Thread::iniciar();
}

void ObjObservadorGenerico::finalizar()
{
  // finalizamos el thread
  Thread::destruir();
  setParado();

  pthread_detach(Thread::getId());
}
void ObjObservadorGenerico::tarea()
{
  SByteType sbBuffer[MAXIMO_BUFFER];
  SDWordType lsdwLong;

  // Borramos todos los mensajes previos
  mColaIPC.borrar();

  while(1)
  {
   try
   {
     lsdwLong = mColaIPC.leer(sbBuffer, MAXIMO_BUFFER, 0, 0);
     if ( lsdwLong > 0)
     {
        mMsjEvento.parse(sbBuffer,lsdwLong);
        Timer::esperar(100);
     }
   }
   catch(InterrupcionExcepcion& e)
   {
      STrz::basic(ConstTraza::T_TRAZA_ERROR, "Excepcion InterrupcionExcepcion %s",
                e.getTextoUltimaExcepcion());
      Timer::esperar(1000);
   }
   catch (Excepcion& e)
   {
     STrz::basic(ConstTraza::T_TRAZA_ERROR, "Excepcion %s",
                  e.getTextoUltimaExcepcion());
     STrz::basic(ConstTraza::T_TRAZA_ERROR, "    ID: %u (%x)", msdwId, msdwId);
     Timer::esperar(1000);
   }
  }
}
// =================   ACCESO       ====================
void ObjObservadorGenerico::setEjecutor(EjecutorEvento* lEjecutor)
{
  mMsjEvento.setEjecutor(lEjecutor);
}
// =================   CONSULTA      ===================

SDWordType ObjObservadorGenerico::getId() const
{
  return(msdwId);
}


void ObjObservadorGenerico::setPid(SDWordType lsdwPid)
{
  msdwPid = lsdwPid;
}

SDWordType ObjObservadorGenerico::getPid() const
{
  return(msdwPid);
}

enum ObjObservador::tiposTypeE ObjObservadorGenerico::getTipo() const
{
  return(ObjObservador::GENERICO);
}


// =======================
//  FIN DEL FICHERO - ObjObservador.cpp
// =======================

