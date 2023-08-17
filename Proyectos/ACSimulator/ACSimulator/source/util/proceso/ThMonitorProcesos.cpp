// =======================
//  FICHERO - ThMonitorProcesos.CPP
// =======================

#include "ThMonitorProcesos.h"

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

  ThMonitorProcesos::ThMonitorProcesos()
  {
    mpEventos = NULL;
  }

  ThMonitorProcesos::~ThMonitorProcesos()
  {

  }

  void ThMonitorProcesos::inicializar()
  {
    mIterador.set(&mLista);
  }

  void ThMonitorProcesos::tarea()
  {
   SDWordType    sdwPID;
   SDWordType    sdwResultado;

   while (1)
   {
     try
     {
      // Arrancamos el proceso
      if ( true == hayProcesoActual() )
      {
       arrancar();
      }
      // Miramos el final de los procesos
      sdwPID = waitpid(-1,&sdwResultado,WNOHANG);
      if( (-1 != sdwPID) && (0 != sdwPID))
      {
       if (WIFEXITED(sdwResultado))
        getEventos()->parado ( sdwPID ,WEXITSTATUS(sdwResultado) );
       else
        getEventos()->parado ( sdwPID ,-1);
      }
      Timer::esperar ( 100 );
     }
     catch(Excepcion& e)
     {
      STrz::basic(ConstTraza::T_TRAZA_MSJSISTEMA,"Excepcion thMonitor");
      STrz::basic(ConstTraza::T_TRAZA_MSJSISTEMA,e);
     }
   }
  }

  void ThMonitorProcesos::arrancar()
  {
    SDWordType lsdwPid;
    ProcesoHijo *lpProcActual;
    MutexLocker lProteccion (mMutex);

    lpProcActual = *mIterador.getPrimero();
    if ( lpProcActual != NULL )
    {
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,
       "Lanzamos el proceso: %s del grupo %d",lpProcActual->getNombreProceso().getCadena(),
        lpProcActual->getGrupoProcesos());
      STrz::basic(ConstTraza::T_TRAZA_CTRLPROC," ->  Argumentos %s",
       lpProcActual->getParametros().getCadena());
      // Lanzamos su ejecucion si no esta inactivo o parado
      try  {
       lpProcActual->lanzarEjec();
       lsdwPid = lpProcActual->getPID();
       mIterador.borrar();
       getEventos()->arrancado ( lsdwPid );
      }
      catch(Excepcion& e)
      {
        STrz::basic(ConstTraza::T_TRAZA_CTRLPROC,"El proceso no arranca");
        exit ( -1 );
      }
    }
  }

 void ThMonitorProcesos::arranca(ProcesoHijo* lProc)
 {
    MutexLocker lProteccion (mMutex);
    mLista.add(lProc,0);
 }

// =================   ACCESO       ====================
 void ThMonitorProcesos::setEventos(EventosProceso* lEventos)
 {
   mpEventos = lEventos;
 }

// =================   CONSULTA      ===================

 bool ThMonitorProcesos::hayProcesoActual()
 {
   MutexLocker lProteccion (mMutex);
   if (mLista.getNumItems() > 0)
    return(true);
   else
    return(false);
 }

 EventosProceso* ThMonitorProcesos::getEventos()
 {
   if (NULL != mpEventos)
    return(mpEventos);
   else
    throw MemoriaExcepcion(__AQUI__,"No esta asignado");
 }


  // ===============================
  //  FIN DEL FICHERO - THMONITORPROCESOS.CPP
  // ===============================

