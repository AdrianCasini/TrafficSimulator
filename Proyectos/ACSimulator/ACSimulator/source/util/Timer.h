// =======================
//  FICHERO - Timer.h
// =======================

#ifndef TIMER_H
#define TIMER_H


#include <signal.h>
#include <time.h>
#include <sys/time.h>

#include "../lang/throwable/SinRecursosExcepcion.h"
#include "../lang/throwable/InterrupcionExcepcion.h"
#include "../lang/throwable/FormatoExcepcion.h"
#include "../lang/throwable/DesconocidoExcepcion.h"
#include "DLList.h"
#include "EjecutorTimer.h"
#include "../thread/Thread.h"
#include "../thread/Mutex.h"

#define MICROSEC  1000000

/**
  * <PRE>
  *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
  *     ---------------------------------------
  *
  * FICHERO: TIMER.H
  *
  * DESCRIPCION:
  *   La clase Timer permite ejecutar periodicamente un procedimiento
  *   La clase maneja una lista de objetos de clase 'EjecutorTimer' y estos
  *   procedimiento se definen en el metodo virtual 'tareaTimer' de esa clase.
  *
  * FECHA CREACION: 29-06-2005
  *
  * AUTOR DISENYO:
  *
  * PROGRAMADOR: Toni Bassas Serramia
  *
  * MODIFICATIONES:
  *
  *    |   FECHA    |   AUTOR   |  RAZON                                   |
  *    ---------------------------------------------------------------------
  *    | DD-MM-YYYY |           |                                          |
  *    ---------------------------------------------------------------------
  *
  * </PRE>
*/
class Timer : public Thread
{
public:
   static const UDWordType MAX_TEMPORIZ = 30; /**< Numero maximo de timers. */
   static const UDWordType INFINITAS = 0;     /**< Infinitas temporizaciones. */

   //constantes de tiempo
   static const UDWordType SEGUNDOS = 1000;
   static const UDWordType MINUTOS = SEGUNDOS * 60;
   static const UDWordType HORAS = MINUTOS * 60;
   static const UDWordType DIAS = HORAS * 24;
   static const UDWordType SEMANAS = DIAS * 7;

   // ===============   CICLO DE VIDA    ==================
   Timer();             /**< Constructor. */
   virtual ~Timer();    /**< Destructor. */
   // =================   OPERACIONES   ===================

   /**
   * Inicializar el timer.
   * @param udwMsec resolucion minima del timer en milisegundos.
   * @return Nada.
   */
   void inicializar(UDWordType udwMsec=250);
   /**
   * Finalizar el timer.
   * @return Nada.
   */
   void finalizar();
   /**
   * Armar el timer en modo relativo.
   * @param udwTiempo Tiempo del timer en milisegundos.
   * @param pEjecTimer Ejecutor del timer.
   * @param udwNumTemporiz Numero de veces que se ejecutara el timer. 0 = infinito.
   * @param uwId Id del timer.
   * @return Nada.
   */
   void armar(const UDWordType udwTiempo, EjecutorTimer* pEjecTimer,
      const UDWordType udwNumTemporiz, UDWordType uwId,bool bEjecutarYa=false);
   /**
   * Armar el timer en modo absoluto.
   * @param ubHora Hora de ejecucion del timer.
   * @param ubMinuto Minuto de ejecucion del timer.
   * @param ubSegundo Segundo de ejecucion del timer.
   * @param pEjecTimer Ejecutor del timer.
   * @param udwNumTemporiz Numero de veces que se ejecutara el timer. 0 = infinito.
   * @param bEjecutarYa Empezar a ejecutar el timer desde este mismo momento.
   * @param uwId Id del timer.
   * @return Nada.
   */
   void armar(const UByteType ubHora,const UByteType ubMinuto,const UByteType ubSegundo,
       EjecutorTimer* pEjecTimer,
      const UDWordType udwNumTemporiz, UDWordType uwId);
   /**
   * Armar el timer en modo aviso de cambio de hora.
   * @param udwDesfase Desfase minimo para considerar cambio de hora (en milisegundos).
   * @param pEjecTimer Ejecutor del timer.
   * @param uwId Id del timer.
   * @return Nada.
   */
   void armar(const UDWordType udwDesfase, EjecutorTimer* pEjecTimer,UDWordType uwId);
   /**
   * Desarmar el timer.
   * @param uwId Id del timer.
   * @return Nada.
   */
   void desarmar( UDWordType uwId );
   /**
   * Consulta del estado del timer.
   * @param uwId Id del timer.
   * @return Si esta armado.
   */
   bool armado( UDWordType uwId );
   /**
   * Esperar.
   * @param uwMilis Tiempo de espera en milisegundos.
   * @return Nada.
   */
   static void esperar( UDWordType udMilis );
   /**
   * Obtener el numero de timers armados.
   * @return Numero de timers armados.
   */
   SDWordType getNumTimers(void);
   /**
   * Obtener el tiempo que queda para ejecutar un timer.
   * @param uwId Id del timer.
   * @return Tiempo en milisegundos.
   */
   UDWordType getTiempoRestante(UDWordType uwId);
   /**
   * Congelar un timer @see continuar.
   * @param uwId Id del timer.
   * @return Si se ha realizado con exito.
   */
   bool pausar(UDWordType uwId);
   /**
   * Continuar un timer congelado @see pausar.
   * @param uwId Id del timer.
   * @return Si se ha realizado con exito.
   */
   bool continuar(UDWordType uwId);


   bool execNow(UDWordType uwId,UDWordType udwTiempo,bool block);

private:

   static const UDWordType MODO_RELATIVO           = 0;
   static const UDWordType MODO_ABSOLUTO           = 1;
   static const UDWordType MODO_AVISO_CAMBIO_HORA  = 2;

   bool bExit;
   bool bDone;
   UWordType udwMsec;

   struct _Timer
   {
      UDWordType     uwId;
      UDWordType     udMode;
      UDWordType     udBaseTicks;
      UDWordType     udTicks;
      SDWordType     sdSteps;
      EjecutorTimer* pEjecTimer;
      UDWordType     udEjecuciones;
      bool           bPaused;
      //modo absoluto
      UByteType      ubHora;
      UByteType      ubMinuto;
      UByteType      ubSegundo;
      SDWordType     sdwUltimaEjecucion;
      //modo cambio horario
      UDWordType     udwDesfase;
      struct timeval nexttime;
   };

   DLList<_Timer,MAX_TEMPORIZ>   Timers;
   void                          tarea(void);
   void                          adjust(struct timeval *tmv);
   void  MyGetTimeOfDay(struct timeval *tv, struct timezone *tz);
   bool                          setPausa(UDWordType uwId,bool bValor);

};

#endif //TIMER_H

