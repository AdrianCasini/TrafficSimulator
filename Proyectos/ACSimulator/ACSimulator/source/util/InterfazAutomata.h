#ifndef INTERFAZAUTOMATA_H 
#define INTERFAZAUTOMATA_H

#include <iostream>
#include "../lang/Types.h"


typedef SDWordType   AutomataEvento;      /**< Evento del automata.*/
typedef SDWordType   AutomataEstado;      /**< Estado del automata.*/
typedef SDWordType   AutomataParametro;   /**< Parametro del Metodo/Funcion de transicion.*/


class InterfazAutomata 
{
   public:
      
   /**
   * Mover el automata con un evento determinado.
   * @param evento Evento.
   * @return Nada.
   */
   virtual bool putEvent(AutomataEvento evento)=0;
   /**
   * Obtiene el estado actual del automata.
   * @return El estado actual.
   */
   virtual AutomataEstado getEstado()=0;
   /**
   * Cargar una tabla determinada para el automata.
   * @param sdwTabla Codigo de la tabla del automata.
   * @return Nada.
   */
   virtual void cargarTabla(SDWordType sdwTabla)=0;
   /**
   * Parar el automata.
   * @return Nada.
   */
   virtual void parar(void)=0;
   /**
   * Continuar el automata (ver parar).
   * @return Nada.
   */
   virtual void continuar(void)=0;
   /**
   * Obtener una referencia al objeto generico automatizado.
   * @return Puntero a objeto generico (void).
   */
   virtual void *getAutomatedObject(void)=0;
};

#endif
