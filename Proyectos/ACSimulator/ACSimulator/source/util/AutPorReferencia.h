// =======================
//  FICHERO - AUTPORREFERENCIA.H
// =======================

#ifndef AUTPORREFERENCIA_H
#define AUTPORREFERENCIA_H

#include <iostream>

#include "InterfazAutomata.h"
#include "../lang/Types.h"
#include "../thread/Mutex.h"

#define STEP_NODOS       16

/**
 * <PRE>
 * AutPorReferencia
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: AutPorReferencia.H
 *
 * DESCRIPCION:
 *   Template generico para automatizar objetos por referencia.
 *
 * FECHA CREACION: 29-11-2005
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
class AutPorReferencia : public InterfazAutomata
{
public:

   enum EstadosGenericos {
         Estado00 = 0,Estado01,Estado02,Estado03,Estado04,Estado05,Estado06,Estado07,Estado08,Estado09,
         Estado10,Estado11,Estado12,Estado13,Estado14,Estado15,Estado16,Estado17,Estado18,Estado19,
         Estado20,Estado21,Estado22,Estado23,Estado24,Estado25,Estado26,Estado27,Estado28,Estado29,
         Estado30,Estado31,Estado32,Estado33,Estado34,Estado35,Estado36,Estado37,Estado38,Estado39,
         Estado40,Estado41,Estado42,Estado43,Estado44,Estado45,Estado46,Estado47,Estado48,Estado49,
         Estado50,Estado51,Estado52,Estado53,Estado54,Estado55,Estado56,Estado57,Estado58,Estado59,
         Estado60,Estado61,Estado62,Estado63,Estado64,Estado65,Estado66,Estado67,Estado68,Estado69,
         Estado70,Estado71,Estado72,Estado73,Estado74,Estado75,Estado76,Estado77,Estado78,Estado79,
         Estado80,Estado81,Estado82,Estado83,Estado84,Estado85,Estado86,Estado87,Estado88,Estado89,
         Estado90,Estado91,Estado92,Estado93,Estado94,Estado95,Estado96,Estado97,Estado98,Estado99
   };

   typedef bool (T:: *Condicion)(void);            /**< Metodo del objeto tipo T de condicion. */
   typedef bool (T:: *Semantica)(AutomataParametro);     /**< Metodo del objeto tipo T de transicion (semantica) con parametro AutomataParametro. */
   typedef bool (*FuncSemantica)(AutomataParametro);        /**< Funcion de transicion (semantica) con parametro AutomataParametro. */
   typedef bool (*FuncCondicion)(void);               /**< Funcion de condicion. */
   typedef void (T:: *SemanticaPorDefecto)(AutomataEstado,AutomataEvento);     /**< Metodo del objeto tipo T que se ejecuta si no hay ningun nodo asociado. */
   typedef void (T:: *TrazaAutomata)(AutomataEstado,AutomataEvento,AutomataEstado);     /**< Metodo del objeto tipo T que se ejecuta como traza. */

   struct AutomataNodo {               /**< Estructura del Nodo de un automata.*/
      AutomataEstado       iniEstado;           /**< Estado inicial del automata.*/
      AutomataEvento       Evento;              /**< Evento que se ha detectado.*/
      Condicion            metodo_condicion;    /**< Metodo del objeto tipo T que evalua la condicion de transicion.*/
      FuncCondicion        funcion_condicion;   /**< Funcion que evalua la condicion de transicion.*/
      AutomataEstado       endEstado;           /**< Estado final del automata tras una condicion y transicion correcta.*/
      Semantica            metodo_transicion;   /**< Metodo del objeto tipo T que de transicion (semantica).*/
      FuncSemantica        funcion_transicion;  /**< Funcion de transicion (semantica).*/
      AutomataParametro    parametro;           /**< Paramtero del Metodo/Funcion de transicion.*/
   };

   Condicion   siempre;
   Semantica   nada;

   SemanticaPorDefecto  semanticaPorDefecto;
   TrazaAutomata        trazaAutomata;

/**
 * Constructor del automata.
 * @return Nada.
*/
   AutPorReferencia(void);

/**
 * Destructor del automata.
 * @return Nada.
*/
   virtual ~AutPorReferencia(void);

/**
 * Inicializa el automata.
 * @return Nada.
*/
   virtual void inicializar(bool bCambioEstadoTrasSemantica = true);
/**
 * Establece el objeto a automatizar.
 * @param t Objeto de la clase T a automatizar.
 * @return Nada.
*/
   void setObject(T *t);
/**
 * Obtiene el objeto automatizado.
 * @return Objeto de la clase T automatizado.
*/
   T *getObject(void);
/**
 * Obtiene el objeto automatizado sin tipo.
 * @return Objeto automatizado.
*/
   void *getAutomatedObject(void);
/**
 * Anyade un nodo al automata.
 * @see AutomataNodo
 * @return Si se ha realizado con exito la operacion.
*/
   bool addNodo(AutomataNodo &nodo);
/**
 * Anyade un nodo al automata.
 * @see AutomataNodo
 * @return Si se ha realizado con exito la operacion.
*/
   bool addNodo(AutomataEstado iniEstado,AutomataEvento Evento,Condicion   condicion,AutomataEstado endEstado,Semantica transicion,AutomataParametro  parametro=0);
/**
 * Anyade un nodo al automata.
 * @see AutomataNodo
 * @return Si se ha realizado con exito la operacion.
*/
   bool addNodo(AutomataEstado iniEstado,AutomataEvento Evento,FuncCondicion  condicion,AutomataEstado endEstado,Semantica transicion,AutomataParametro  parametro=0);
/**
 * Anyade un nodo al automata.
 * @see AutomataNodo
 * @return Si se ha realizado con exito la operacion.
*/
   bool addNodo(AutomataEstado iniEstado,AutomataEvento Evento,Condicion   condicion,AutomataEstado endEstado,FuncSemantica   transicion,AutomataParametro  parametro=0);
/**
 * Anyade un nodo al automata.
 * @see AutomataNodo
 * @return Si se ha realizado con exito la operacion.
*/
   bool addNodo(AutomataEstado iniEstado,AutomataEvento Evento,FuncCondicion  condicion,AutomataEstado endEstado,FuncSemantica   transicion,AutomataParametro  parametro=0);
/**
 * Forzar un estado.
 * @param newEstado nuevo estado del automata.
 * @return El nuevo estado.
*/
   AutomataEstado setEstado(AutomataEstado newEstado);
/**
 * Obtiene el estado actual del automata.
 * @return El estado actual.
*/
   AutomataEstado getEstado();
/**
 * Establece la semantica que se ejecutara por defecto
 * en caso de que no exista ningun nodo asociado al
 * evento y estado actual.
 * @param semanticaPorDefecto Semantica que se ejecutara en caso de no cumplirse ningun nodo.
 * @return Nada.
*/
   void setSemanticaPorDefecto(SemanticaPorDefecto semanticaPorDefecto);
/**
 * Establece el metodo que se ejecutara en caso de exito en alguna transicion.
 * @param trazaAutomata Metodo a ejecutar como traza.
 * @return Nada.
*/
   void setTrazaAutomata(TrazaAutomata trazaAutomata);
/**
 * Mueve el automata con un evento.
 * @param evento Evento a evaluar.
 *
 * Procedimiento:
 * 1) Busca un Nodo con iniEstado igual al estado del automata
 *    I con evento igual a Evento.
 * 2) Verifica si la condicion se evalua con exito.
 * 3) Ejecuta la semantica y si se ejecuta con exito ...
 * 4) Cambio el Estado del automata a endEstado.
 * @return Si se ha realizado cambio de estado.
 * @see AutomataNodo
*/
   virtual bool putEvent(AutomataEvento evento);
/**
 * Cargar una tabla predefinida.
 * @param sdwTabla Codigo de la tabla.
 * @return Nada.
*/
   virtual void cargarTabla(SDWordType sdwTabla){};
/**
 * Parar el automata.
 * @return Nada.
*/
   virtual void parar(void){bPausa=true;};
/**
 * Continuar el automata (ver parar).
 * @return Nada.
*/
   virtual void continuar(void){bPausa=false;};
/**
 * Definicion del operador = para Automatas.
 * Copia la tabla de Nodos e iguala el Estado.
*/
   //TODO AutPorReferencia &operator=(AutPorReferencia &);

   bool accion(AutomataEvento evento,bool bTest=false);
   bool testTransicion(AutomataEvento evento);

protected:

   T              *out;                         /**< Puntero al objeto automatizado */
   AutomataEstado Estado;                       /**< Estado actual del automata */
   SDWordType     NumNodos;                     /**< Numero de Nodos definidos en el automata */
   SDWordType     MaxNodos;                     /**< Numero Maximo de Nodos del automata */
   AutomataNodo   *Nodo;                        /**< Array de Nodos del automata */
   Mutex          mutex;
   bool           bCambioEstadoTrasSemantica;
   bool           bPausa;

};

/*TODO template <typename T>
AutPorReferencia<T> &AutPorReferencia<T>::operator=(AutPorReferencia<T> &ptauto) {
   memcpy(Nodo,ptauto.Nodo,sizeof(AutomataNodo)* AUTOMATA_MAX_NODOS);
   Estado=ptauto.Estado;
   NumNodos=ptauto.NumNodos;
   return *this;
}*/


template <typename T>
AutPorReferencia<T>::AutPorReferencia(void) {
   out=NULL;
   siempre=NULL;
   nada=NULL;
   Nodo=NULL;
   bPausa=false;

   inicializar();
}

template <typename T>
AutPorReferencia<T>::~AutPorReferencia(void) {
   if (Nodo!=NULL) free(Nodo);
}

template <typename T>
void AutPorReferencia<T>::inicializar(bool bCambioEstadoTrasSemantica) {
   if (Nodo!=NULL) {
      free(Nodo);
      Nodo=NULL;
   }
   NumNodos=0;
   MaxNodos=0;
   Estado=0;
   semanticaPorDefecto=NULL;
   trazaAutomata=NULL;
   this->bCambioEstadoTrasSemantica=bCambioEstadoTrasSemantica;
}

template <typename T>
void AutPorReferencia<T>::setObject(T *t) {
   out=t;
}

template <typename T>
T *AutPorReferencia<T>::getObject(void) {
   return(out);
}

template <typename T>
void *AutPorReferencia<T>::getAutomatedObject(void) {
   return((void *) out);
}

template <typename T>
bool AutPorReferencia<T>::addNodo(AutomataNodo &nodo) {
   if (NumNodos==MaxNodos) {
      MaxNodos+=STEP_NODOS;
      Nodo=(AutomataNodo *) realloc(Nodo,sizeof(AutomataNodo)*MaxNodos);
      if (Nodo==NULL) {
         inicializar(bCambioEstadoTrasSemantica);
         return false;
      }
   }
   if (NumNodos==0) Estado=nodo.iniEstado;
   Nodo[NumNodos]=nodo;
   NumNodos++;
   return true;
}

template <typename T>
bool AutPorReferencia<T>::addNodo(AutomataEstado iniEstado,AutomataEvento Evento,Condicion  condicion,AutomataEstado endEstado, Semantica   transicion,AutomataParametro  parametro) {

      AutomataNodo nodo;
      memset(&nodo,0,sizeof(nodo));

      nodo.iniEstado=iniEstado;
      nodo.Evento=Evento;
      nodo.metodo_condicion=condicion;
      nodo.endEstado=endEstado;
      nodo.metodo_transicion=transicion;
      nodo.parametro=parametro;

      return (addNodo(nodo));

}

template <typename T>
bool AutPorReferencia<T>::addNodo(AutomataEstado iniEstado,AutomataEvento Evento,FuncCondicion condicion,AutomataEstado endEstado, Semantica   transicion,AutomataParametro  parametro) {

      AutomataNodo nodo;
      memset(&nodo,0,sizeof(nodo));

      nodo.iniEstado=iniEstado;
      nodo.Evento=Evento;
      nodo.funcion_condicion=condicion;
      nodo.endEstado=endEstado;
      nodo.metodo_transicion=transicion;
      nodo.parametro=parametro;

      return (addNodo(nodo));

}

template <typename T>
bool AutPorReferencia<T>::addNodo(AutomataEstado iniEstado,AutomataEvento Evento,Condicion  condicion,AutomataEstado endEstado, FuncSemantica  transicion,AutomataParametro  parametro) {

      AutomataNodo nodo;
      memset(&nodo,0,sizeof(nodo));

      nodo.iniEstado=iniEstado;
      nodo.Evento=Evento;
      nodo.metodo_condicion=condicion;
      nodo.endEstado=endEstado;
      nodo.funcion_transicion=transicion;
      nodo.parametro=parametro;

      return (addNodo(nodo));

}

template <typename T>
bool AutPorReferencia<T>::addNodo(AutomataEstado iniEstado,AutomataEvento Evento,FuncCondicion condicion,AutomataEstado endEstado, FuncSemantica  transicion,AutomataParametro  parametro) {

      AutomataNodo nodo;
      memset(&nodo,0,sizeof(nodo));

      nodo.iniEstado=iniEstado;
      nodo.Evento=Evento;
      nodo.funcion_condicion=condicion;
      nodo.endEstado=endEstado;
      nodo.funcion_transicion=transicion;
      nodo.parametro=parametro;

      return (addNodo(nodo));

}

template <typename T>
AutomataEstado AutPorReferencia<T>::getEstado() {
   return (Estado);
}

template <typename T>
AutomataEstado AutPorReferencia<T>::setEstado(AutomataEstado newEstado) {
   Estado=newEstado;
   return (Estado);
}

template <typename T>
void AutPorReferencia<T>::setSemanticaPorDefecto(SemanticaPorDefecto semanticaPorDefecto) {
   this->semanticaPorDefecto=semanticaPorDefecto;
}

template <typename T>
void AutPorReferencia<T>::setTrazaAutomata(TrazaAutomata trazaAutomata) {
   this->trazaAutomata=trazaAutomata;
}

template <typename T>
bool AutPorReferencia<T>::putEvent(AutomataEvento evento) {
   bool bRet;

   mutex.lock();
   bRet=accion(evento,false);
   mutex.unlock();
   return(bRet);
}

template <typename T>
bool AutPorReferencia<T>::accion(AutomataEvento evento,bool bTest) {
   SDWordType i;

   if (bPausa && !bTest) return true;

   for(i=0;i<NumNodos;i++) {
      if (Nodo[i].iniEstado==Estado) {
         if (Nodo[i].Evento==evento) {
            do {
               if (Nodo[i].metodo_condicion!=0) {
                  if(!(out->*Nodo[i].metodo_condicion)()) {
                     break;
                  }
               }
               if (Nodo[i].funcion_condicion!=0) {
                  if(!(Nodo[i].funcion_condicion())) {
                     break;
                  }
               }

               if (bTest) return true;

               if (!bCambioEstadoTrasSemantica) Estado=Nodo[i].endEstado;
               if (Nodo[i].metodo_transicion!=0) {
                  if(!(out->*Nodo[i].metodo_transicion)(Nodo[i].parametro)) {
                     return false;
                  }
               }
               if (Nodo[i].funcion_transicion!=0) {
                  if(!(Nodo[i].funcion_transicion(Nodo[i].parametro))) {
                     return false;
                  }
               }
               if (trazaAutomata!=0) {
                  (out->*trazaAutomata)(Nodo[i].iniEstado,Nodo[i].Evento,Nodo[i].endEstado);
               }
               if (bCambioEstadoTrasSemantica) Estado=Nodo[i].endEstado;
               return true;
            } while (0);
         }
      }
   }
   if (!bTest) {
      if (semanticaPorDefecto!=0) {
         (out->*semanticaPorDefecto)(Estado,evento);
      }
   }
   return false;
}

template <typename T>
bool AutPorReferencia<T>::testTransicion(AutomataEvento evento) {
   bool bRet;

   mutex.lock();
   bRet=accion(evento,true);
   mutex.unlock();
   return(bRet);
}


#endif
