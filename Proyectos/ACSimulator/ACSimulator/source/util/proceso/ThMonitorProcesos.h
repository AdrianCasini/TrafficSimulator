// =======================
//  FICHERO - THMONITORPROCESOS.H
// =======================

#ifndef THMONITORPROCESOS_H
#define THMONITORPROCESOS_H


// ================================================================
// INCLUDES
// ================================================================

// INCLUDES DEL SISTEMA

#include <sys/types.h>
#include <sys/wait.h>

// INCLUDES DEL PROJECTO

#include "thread/Mutex.h"
#include "thread/Thread.h"
#include "thread/Semaforo.h"
#include "ProcesoHijo.h"
#include "OrdenesProceso.h"
#include "EventosProceso.h"
#include "util/DLList.h"
#include "util/IteradorDLL.h"

// INCLUDES LOCALES

// ================================================================
// DECLARACION DE CLASE
// ================================================================

/**
 * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: THMONITORPROCESOS.H
  *
  * DESCRIPCION: Control de arranque y parada de procesos.
  *
  * FECHA CREACION: 08-06-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Oscar Lopez Calvo
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |             |                                              |
  *    ---------------------------------------------------------------------
*
  * </PRE>
 */

class ThMonitorProcesos : public Thread, public  OrdenesProceso
{

   // ATRIBUTOS
   // =====================================================
   ////////////////////   PUBLIC       ////////////////////
   public :
   ////////////////////   PRIVATE      ////////////////////
   private :
      static const UWordType   MAXIMOS_PROCESOS = 50;
      // Indica el grupo actual de arranque
      DLList<ProcesoHijo*,MAXIMOS_PROCESOS>  mLista;
      IteradorDLL<ProcesoHijo*,MAXIMOS_PROCESOS>  mIterador;
      EventosProceso*           mpEventos;
      // Semafor para la ejecucion de aplicaciones
      Mutex                     mMutex;

   // METODOS
   // =====================================================

   ////////////////////   PUBLIC       ////////////////////
   public :

      // ===============   CICLO DE VIDA    ==================

     /**
      *  Constructor de la clase
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      ThMonitorProcesos();

     /**
      * Destructor de la clase.
      * @param   Ninguno
      * @return  Ninguno
      * @exception  Ninguno
      */
      virtual ~ThMonitorProcesos();

      // ===============   OPERACIONES    ==================

     /**
      *  Iniciliaza todos los objetos de la clase.
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  Ninguno
      */
      void inicializar();


     /**
      *   Hilo de ejecucion de la clase, se encargar de arrancar los procesos
      *   y los timers , y de esperar el final de la ejecucion de los procesos
      *   que controla. Forma "parte" de TareaGestorProcesos para gestionar en
      *   conjunto el control completo de los procesos asignados.
      *
      *  @param   Ninguno
      *  @return  Ninguno
      *  @exception  En caso de excepciones no controladas, se aborta la ejecucion del hilo
      *    y se envian a los ficheros de trazas.
      */
      void tarea();

      void arranca(ProcesoHijo* lProc);
      void setEventos(EventosProceso* lEventos);
      EventosProceso* getEventos();
   private:
      bool hayProcesoActual();
      void arrancar();
};
// ================================================================
// DEFINICION DE METODOS INLINE
// ================================================================


// ================================================================
// REFERENCIAS EXTERNAS
// ================================================================

#endif //THMONITORPROCESOS_H

// ===========================
//  FIN DEL FICHERO - THMONITORPROCESOS.H
// ===========================
