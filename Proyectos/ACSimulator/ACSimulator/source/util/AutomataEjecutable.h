// =======================
//  FICHERO - AUTOMATAEJECUTABLE.H
// =======================

#ifndef AUTOMATA_RUN_H
#define AUTOMATA_RUN_H

#include "Automata.h"
#include "Fifo.h"
#include "ThreadMetodo.h"

/**
 * <PRE>
 * AutomataEjecutable
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: Automata.H
 *
 * DESCRIPCION:
 *   Template generico para automatizar objetos de cualquier clase en un hilo independiente.
 *   Esta automatizacion se arranca en un hilo independiente.
 *   Los otros threads insertan eventos al automata a traves de una Fifo.
 *
 * FECHA CREACION: 30-11-2005
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
 * </PRE>
*/

template <typename T>
class AutomataEjecutable : public Automata<T> {
public:
/**
 * Constructor del automata.
 * @return Nada.
*/ 
   AutomataEjecutable(void);
/**
 * Mueve el automata con un evento.
 * @param evento Evento a evaluar.
 * @return Si se ha realizado cambio de estado.
 * @see Automata_Nodo
*/
   bool putEvent(AutomataEvento evnt);
/**
 * Inicia el hilo independiente del automata.
 * @return Nada.
 * @see Thread
*/
   void iniciar(void);
/**
 * Finaliza el hilo independiente del automata.
 * @return Nada.
 * @see Thread
*/
   void finalizar(void);

   static const SWordType ST_STOP = 0;
   static const SWordType ST_START = 1;
   static const SWordType ST_END = 2;

   UWordType                              status;        /**< Estado del automata ejecutable.*/
   Fifo<AutomataEvento,16>                colaEventos;   /**< Fifo de eventos del automata ejecutable. */
   ThreadMetodo< AutomataEjecutable<T> > thread;       /**< Hilo del automata ejecutable. */

private:

   void tarea(void);                         /**< Implementacion de Thread.*/


};

template <typename T>
AutomataEjecutable<T>::AutomataEjecutable(void) : Automata<T>() {
   status=ST_STOP;
   thread.setMetodo(this,&AutomataEjecutable<T>::tarea);
}

template <typename T>
bool AutomataEjecutable<T>::putEvent(AutomataEvento evento) {
   return(colaEventos.add(evento));
}

template <typename T>
void AutomataEjecutable<T>::iniciar(void) {
   thread.iniciar();
}

template <typename T>
void AutomataEjecutable<T>::finalizar(void) {
   thread.destruir();
   thread.esperarFinalHilo();
}

template <typename T>
void AutomataEjecutable<T>::tarea(void) {
   AutomataEvento evento;

   while (status!=ST_END) {      
      while(colaEventos.get(evento)) {
         Automata<T>::accion(evento);
      }
   }
}





#endif
